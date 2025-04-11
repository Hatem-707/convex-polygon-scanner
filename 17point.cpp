#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include "format"
#include "reusableFunctions.hpp"

using namespace std;

bool examineNewPoint(pair<int, int> newPoint, vector<pair<int, int>> previousSet, int targetPoly, int setSize)
{
    vector<bool> combinations(setSize, false);
    vector<pair<int, int>> polygon(targetPoly);

    for (pair<int, int> point1 : previousSet)
    {
        for (pair<int, int> point2 : previousSet)
        {
            if ((crossProduct(newPoint, point1, point2) == 0 && point1 != point2) | newPoint == point1 | newPoint == point2)
                return false;
        }
    }

    fill(combinations.begin(), combinations.begin() + targetPoly - 1, true);
    do
    {
        polygon.clear();
        for (int index = 0; index < combinations.size(); index++)
        {
            if (combinations[index])
                polygon.push_back(previousSet[index]);
        }

        polygon.push_back(newPoint);
        vector<pair<int, int>> hull = grahamScan(polygon);
        if (hull.size() == targetPoly)
            return false;
    } while (prev_permutation(combinations.begin(), combinations.end()));
    return true;
}

void checkPointsRange(int startX, int endX, int y, vector<pair<int, int>>& emptySet,
    int h, int& valid, int toBeAdded, long long& increments, mutex& mtx) {
    for (int i = startX; i < endX && valid < toBeAdded; ++i) {
        for (int j = 0; j < y && valid < toBeAdded; ++j) {
            {
                lock_guard<mutex> lock(mtx);
                ++increments;
            }

            if (examineNewPoint({ i, j }, emptySet, h, emptySet.size())) {
                lock_guard<mutex> lock(mtx);
                if (valid < toBeAdded) {
                    valid++;
                    emptySet.push_back({ i, j });
                }
            }
        }
    }
}

int main(int argc, const char* argv[])
{
    int n = 16;
    int h = 7;
    int x = 150;
    int y = 150;
    srand(time(0));
    while (1) {
        long long iterations = 0;
        vector<pair<int, int>> emptySet;
        bool found = false;
        mutex mtx;
        vector<thread> threads;

        for (int i = 0; i < 14; i++)
        {
            threads.emplace_back(
                static_cast<void (*)(mutex*, int, int, int, int, vector<pair<int, int>> *, bool*, long long*)>(
                    threadFunctionEmptySet),
                &mtx, n, h, x/2, y/2, &emptySet, &found, &iterations);
        }

        for (auto& t : threads)
        {
            t.join();
        }

        for (pair<int, int>& point : emptySet)
        {
            point.first = point.first + x/4;
            point.second = point.second + y/4;
            std::cout << point.first << " " << point.second << "|";
        }
        cout << endl;

        string output = format("Number of iterations for generation: {}\n", iterations);
        std::cout << output;

        long long increments = 0;
        int valid = 0;
        int toBeAdded = 16;
        mutex pointsMtx;
        vector<thread> searchThreads;

        const int numThreads = 14;  // Adjust based on your CPU
        const int chunkSize = x / numThreads;

        for (int t = 0; t < numThreads; ++t) {
            int startX = t * chunkSize;
            int endX = (t == numThreads - 1) ? x : (t + 1) * chunkSize;

            searchThreads.emplace_back(checkPointsRange, startX, endX, y,
                ref(emptySet), h, ref(valid), toBeAdded,
                ref(increments), ref(pointsMtx));
        }

        for (auto& t : searchThreads) {
            t.join();
        }
        string output2 = format("Number of iterations for extending: {} to {}\n", increments, emptySet.size());
            std::cout << output2;
            for (pair<int, int> point : emptySet)
            {
                std::cout << point.first << " " << point.second << "|";
            }
        cout << endl;
        
        if (valid == toBeAdded)
        {
            break;
        }
        else cout << "couldn't find set of size 32 \n";
    }
    return 0;
}