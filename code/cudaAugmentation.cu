/* graham_scan_cuda.cu */

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <thrust/sort.h>
#include <thrust/execution_policy.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <mutex>

using namespace std;

// --- Device-side data structures & routines ---

struct Point { int x, y; };

// Device cross product/orientation
__device__ inline int crossDev(const Point &a, const Point &b, const Point &c) {
    // (b - a) x (c - b)
    return (b.y - a.y) * (c.x - b.x)
         - (b.x - a.x) * (c.y - b.y);
}

// Device Graham scan: returns hull size
__device__ int grahamScanDevice(Point *pts, int n) {
    // 1) find pivot: lowest y, if tie highest x
    int minIdx = 0;
    for (int i = 1; i < n; ++i) {
        if (pts[i].y < pts[minIdx].y ||
           (pts[i].y == pts[minIdx].y && pts[i].x > pts[minIdx].x)) {
            minIdx = i;
        }
    }
    // swap pivot to index 0
    Point tmp = pts[0]; pts[0] = pts[minIdx]; pts[minIdx] = tmp;
    Point pivot = pts[0];

    // 2) simple insertion sort by polar angle around pivot
    //    (thrust::sort isn't directly usable with device lambda here in all cases)
    for (int i = 2; i < n; ++i) {
        for (int j = i; j > 1; --j) {
            int o = (pts[j].y - pivot.y) * (pivot.x - pts[j-1].x)
                  - (pts[j-1].y - pivot.y) * (pivot.x - pts[j].x);
            if (o < 0 || (o == 0 &&
                (pts[j].x - pivot.x)*(pts[j].x - pivot.x) + (pts[j].y - pivot.y)*(pts[j].y - pivot.y)
              < (pts[j-1].x - pivot.x)*(pts[j-1].x - pivot.x) + (pts[j-1].y - pivot.y)*(pts[j-1].y - pivot.y))) {
                // swap
                Point ctmp = pts[j]; pts[j] = pts[j-1]; pts[j-1] = ctmp;
            } else break;
        }
    }

    // 3) build hull in local stack
    extern __shared__ Point hull[];  // allocate at kernel launch: sharedMem = sizeof(Point)*n
    int m = 0;
    hull[m++] = pts[0];
    hull[m++] = pts[1];
    for (int i = 2; i < n; ++i) {
        while (m > 1 && crossDev(hull[m-2], hull[m-1], pts[i]) < 0) {
            --m;
        }
        hull[m++] = pts[i];
    }
    return m;
}

// Kernel: process one candidate polygon per thread
__global__ void grahamScanMultiKernel(Point *allPts, int *offsets, int *counts, bool *results, int target) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    // each thread handles combination idx
    Point *pts = allPts + offsets[idx];
    int n = counts[idx];
    // allocate shared mem for 'hull' of size n
    int hullSize = grahamScanDevice(pts, n);
    results[idx] = (hullSize != target);
}

// --- Host-side wrapper for multi-point examination using CUDA ---

bool examineNewPointMultiCUDA(pair<int,int> newPoint, const vector<pair<int,int>> &prevSet, int targetPoly) {
    // early colinearity/duplication checks (host)
    for (auto &p1 : prevSet) {
        for (auto &p2 : prevSet) {
            if ((crossProduct({newPoint.first,newPoint.second}, p1, p2) == 0 && p1 != p2)
             || newPoint == p1 || newPoint == p2)
                return false;
        }
    }

    int setSize = (int)prevSet.size();
    vector<bool> comb(setSize, false);
    fill(comb.begin(), comb.begin() + targetPoly - 1, true);

    const int batchSize = 128;  // tune for your GPU
    vector<Point> flatPts; flatPts.reserve(batchSize * targetPoly);
    vector<int>    offsets;    offsets.reserve(batchSize);
    vector<int>    counts;     counts.reserve(batchSize);
    vector<bool>   results;

    // device buffers (will be re-used)
    Point *d_allPts = nullptr;
    int   *d_offsets = nullptr, *d_counts = nullptr;
    bool  *d_results = nullptr;
    int    maxB       = batchSize;

    size_t ptsBufBytes = sizeof(Point) * batchSize * targetPoly;
    size_t offBytes    = sizeof(int) * batchSize;
    size_t resBytes    = sizeof(bool) * batchSize;

    cudaMalloc(&d_allPts, ptsBufBytes);
    cudaMalloc(&d_offsets, offBytes);
    cudaMalloc(&d_counts, offBytes);
    cudaMalloc(&d_results, resBytes);

    bool globalOk = true;
    do {
        // build one combination
        vector<pair<int,int>> poly;
        poly.reserve(targetPoly);
        for (int i = 0; i < setSize; ++i)
            if (comb[i]) poly.push_back(prevSet[i]);
        poly.push_back(newPoint);

        // flatten
        offsets.push_back((int)flatPts.size());
        counts .push_back((int)poly.size());
        for (auto &p : poly)
            flatPts.push_back({p.first, p.second});

        if ((int)offsets.size() == batchSize) {
            // copy to device
            cudaMemcpy(d_allPts,   flatPts.data(),      flatPts.size()*sizeof(Point), cudaMemcpyHostToDevice);
            cudaMemcpy(d_offsets,  offsets.data(),      offsets.size()*sizeof(int),   cudaMemcpyHostToDevice);
            cudaMemcpy(d_counts,   counts.data(),       counts.size()*sizeof(int),    cudaMemcpyHostToDevice);

            // launch
            int threads = 128;
            int blocks  = (batchSize + threads - 1) / threads;
            size_t sharedMemBytes = sizeof(Point) * targetPoly;
            grahamScanMultiKernel<<<blocks,threads,sharedMemBytes>>>(
                d_allPts, d_offsets, d_counts, d_results, targetPoly
            );
            cudaDeviceSynchronize();

            // retrieve
            results.resize(batchSize);
            cudaMemcpy(results.data(), d_results, resBytes, cudaMemcpyDeviceToHost);

            // check
            for (bool ok : results) if (!ok) { globalOk = false; break; }
            if (!globalOk) break;

            // reset
            flatPts.clear(); offsets.clear(); counts.clear();
        }

        if (!globalOk) break;
    } while (prev_permutation(comb.begin(), comb.end()));

    // process any leftovers
    if (globalOk && !offsets.empty()) {
        int smallN = (int)offsets.size();
        cudaMemcpy(d_allPts, flatPts.data(), flatPts.size()*sizeof(Point), cudaMemcpyHostToDevice);
        cudaMemcpy(d_offsets, offsets.data(), smallN*sizeof(int), cudaMemcpyHostToDevice);
        cudaMemcpy(d_counts, counts.data(),   smallN*sizeof(int), cudaMemcpyHostToDevice);
        int threads = 64;
        int blocks  = (smallN + threads - 1) / threads;
        size_t sharedMemBytes = sizeof(Point) * targetPoly;
        grahamScanMultiKernel<<<blocks,threads,sharedMemBytes>>>(d_allPts, d_offsets, d_counts, d_results, targetPoly);
        cudaDeviceSynchronize();
        vector<bool> smallRes(smallN);
        cudaMemcpy(smallRes.data(), d_results, smallN*sizeof(bool), cudaMemcpyDeviceToHost);
        for (bool ok : smallRes) if (!ok) { globalOk = false; break; }
    }

    cudaFree(d_allPts);
    cudaFree(d_offsets);
    cudaFree(d_counts);
    cudaFree(d_results);

    return globalOk;
}

// --- Main (adjusted to call CUDA variant) ---
int main(int argc, const char* argv[]) {
    const int n = 13;
    const int h = 6;
    const int x = 200;
    const int y = 200;
    srand((unsigned)time(0));
    const string saveFile = "1points.txt";

    int toBeTested = 100;
    int successes  = 0;

    time_t start = time(0);

    while (toBeTested--) {
        long long iterations = 0;
        vector<pair<int,int>> emptySet;
        bool found = false;
        mutex mtx;
        vector<thread> pool;

        // generate initial emptySet via your threadFunctionEmptySet CPU code
        for (int i = 0; i < 14; ++i) {
            pool.emplace_back(
                threadFunctionEmptySet,
                &mtx, n, h, x, y, &emptySet, &found, &iterations
            );
        }
        for (auto &t : pool) t.join();

        cout << "Seed set: ";
        for (auto &p : emptySet) cout << p.first << "," << p.second << " ";
        cout << '\n';

        long long incr = 0;
        int valid = 0;
        const int toAdd = 3;
        for (int added = 0; added < toAdd; ++added) {
            if (added > valid) break;
            for (int i = 0; i < x; ++i) {
                for (int j = 0; j < y; ++j) {
                    ++incr;
                    if (examineNewPointMultiCUDA({i,j}, emptySet, h)) {
                        ++valid;
                        emptySet.emplace_back(i,j);
                    }
                    if (valid == toAdd) break;
                }
                if (valid == toAdd) break;
            }
        }

        // cout << format("Extend iterations: {} --> size {}\n", incr, emptySet.size());

        if (valid == toAdd) {
            auto confHull = checkPointsForPolygonH((int)emptySet.size(), h, emptySet);
            if (confHull.size() == 1) {
                ++successes;
                cout << "Confirmed!\n";
                ofstream f(saveFile, ios::app);
                for (auto &p : emptySet)
                    f << p.first << " " << p.second << "|";
                f << '\n';
            } else {
                cout << "Mistake!\n";
            }
        } else {
            cout << "Failed to extend to size " << toAdd << "\n";
        }

        cout << "Runs left: " << toBeTested << ", Successes: " << successes << "\n";
    }

    time_t end = time(0);

    cout<<"Time: "<<end-start<<endl

    return 0;
}
