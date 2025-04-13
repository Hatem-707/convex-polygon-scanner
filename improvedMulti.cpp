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
#include "fstream"
#include "reusableFunctions.hpp"

using namespace std;

void grahamScanMulti(vector<pair<int, int>> pointSet, bool* ret, mutex* mtx, int target) {
    vector<pair<int, int>> hull = grahamScan(pointSet);
    if (hull.size() == target) {
        (*ret) = false;
    }
}

bool examineNewPointMulti(pair<int, int> newPoint, vector<pair<int, int>> previousSet, int targetPoly, int setSize)
{

    vector<bool> combinations(setSize, false);
    vector<pair<int, int>> polygon(targetPoly);

    for (pair<int, int> point1 : previousSet)
    {
        for (pair<int, int> point2 : previousSet)
        {
            if ((crossProduct(newPoint, point1, point2) == 0 && point1 != point2) | newPoint == point1 | newPoint == point2) {
                return false;
            }
        }
    }

    bool good = true;
    mutex mtx;

    fill(combinations.begin(), combinations.begin() + targetPoly - 1, true);
    vector<vector<pair<int, int>>> batch;
    do
    {
        polygon.clear();
        for (int index = 0; index < combinations.size(); index++)
        {
            if (combinations[index])
                polygon.push_back(previousSet[index]);
        }
        polygon.push_back(newPoint);
        batch.push_back(polygon);


        if (batch.size() == 14) {
            vector<thread> threads;

            for (int i = 0; i < 14; ++i) {
                threads.emplace_back(grahamScanMulti, batch[i], &good, &mtx, targetPoly);
            }
            for (auto& t : threads)
            {
                t.join();
            }
            batch.clear();
        }
        if (!good)
            return false;
    } while (prev_permutation(combinations.begin(), combinations.end()));
    return true;
}

int main(int argc, const char* argv[])
{
    int n = 16;
    int h = 7;
    int x = 100;
    int y = 100;
    srand(time(0));
    string saveFile = "16points.txt";

    int toBoTested = 100;
    int success = 0;
    while (toBoTested--)
    {
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
                &mtx, n, h, x, y, &emptySet, &found, &iterations);
        }

        for (auto& t : threads)
        {
            t.join();
        }

        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        std::cout << endl;

        string output = format("Number of iterations for generation: {}\n", iterations);
        std::cout << output;

        long long increments = 0;
        int valid = 0;
        int toBeAdded = 16;
        for (int added = 0; added < toBeAdded; ++added)
        {
            if (added > valid)
                break;
            for (int i = 0; i < x; ++i)
            {
                for (int j = 0; j < y; ++j)
                {
                    ++increments;
                    if (examineNewPointMulti({ i, j }, emptySet, h, emptySet.size()) == true)
                    {
                        valid++;
                        emptySet.push_back({ i, j });
                    }
                    if (valid == toBeAdded)
                        break;
                }
                if (valid == toBeAdded)
                    break;
            }
        }
        string output2 = format("Number of iterations for extending: {} to set size {}\n", increments, emptySet.size());
        std::cout << output2;
        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        std::cout << endl;
        if (valid == toBeAdded)
        {
            vector<pair<int, int>> confirmationHull = checkPointsForPolygonH(emptySet.size(), h, emptySet);
            if (confirmationHull.size() == 1)
            {
                success++;
                std::cout << "Confirmed!!!\n";
                ofstream file;
                file.open(saveFile, ios::app);
                for (pair<int, int> point : emptySet)
                {
                    file << point.first << " " << point.second << "|";
                }
                file << endl;
                file.close();
            }
            else
                std::cout << "Mistake!!!\n";
        }
        else
            std::cout << "couldn't find set of size 32 \n";
        std::cout << "run: " << toBoTested << "\n";
        std::cout << "successes: " << success << "\n";
    }
    return 0;
}