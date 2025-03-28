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

int main(int argc, const char *argv[])
{
    int n = 13;
    int h = 6;
    int x = 100;
    int y = 100;
    srand(time(0));

    long long iterations = 0;
    vector<pair<int, int>> emptySet;
    bool found = false;
    mutex mtx;
    vector<thread> threads;

    for (int i = 0; i < 14; i++)
    {
        threads.emplace_back(
            static_cast<void (*)(mutex *, int, int, int, int, vector<pair<int, int>> *, bool *, long long *)>(
                threadFunctionEmptySet),
            &mtx, n, h, x, y, &emptySet, &found, &iterations);
    }

    for (auto &t : threads)
    {
        t.join();
    }

    for (pair<int, int> point : emptySet)
    {
        std::cout << point.first << " " << point.second << "|";
    }
    cout << endl;

    string output = format("Number of iterations for generation: {}\n", iterations);
    std::cout << output;

    long long increments = 0;
    int valid = 0;
    for (int added = 0; added < 3; ++added)
    {
        if(added>valid) break;
        for (int i = 0; i < x; ++i)
        {
            for (int j = 0; j < y; ++j)
            {
                ++increments;
                if (examineNewPoint({i, j}, emptySet, h, emptySet.size()) == true)
                {
                    valid++;
                    emptySet.push_back({i, j});
                }
                if (valid == 3)
                    break;
            }
            if (valid == 3)
                break;
        }
    }
    if (valid == 3)
    {
        string output2 = format("Number of iterations for extending: {}\n", increments);
        std::cout << output2;
        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        cout << endl;
        vector<pair<int, int>> confirmationHull = checkPointsForPolygonH(emptySet.size(), h, emptySet);
        if (confirmationHull.size() == 1)
            cout << "Confirmed!!!\n";
        else
            cout << "Mistake!!!\n";
    }else cout << "couldn't find set of size 16 \n";
    return 0;
}