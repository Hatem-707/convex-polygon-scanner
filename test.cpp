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

void increamentalThread(pair<int, int> point, vector<pair<int, int>>* emptySet, int h, int * valid, mutex* mtx){
    if (examineNewPoint(point, *emptySet, h, (*emptySet).size())){
        lock_guard<mutex> lock(*mtx);
        (*valid)++;
        (*emptySet).push_back(point);
    }
}

int main(int argc, const char *argv[])
{
    int n = 16;
    int h = 7;
    int x = 100;
    int y = 100;
    srand(time(0));
    string saveFile = "17points.txt";

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
        std::cout << endl;

        string output = format("Number of iterations for generation: {}\n", iterations);
        std::cout << output;

        long long increments = 0;
        int valid = 0;
        int toBeAdded = 16;
        for (int added = 0; added < toBeAdded; ++added)
        {
            mutex innerMtx;
            vector<pair<int, int>> batch;
            if (added > valid){
                cout<<"Maximum set length: "<<emptySet.size()<<endl;
                break;
            }
            for (int i = 0; i < x; ++i)
            {
                for (int j = 0; j < y; ++j)
                {
                    if(batch.size() == 14){
                        vector<thread> innerThreads;
                        for (int m=0; m<14; ++m){
                            innerThreads.emplace_back(increamentalThread, batch[m], &emptySet, h, &valid, &innerMtx);
                        }
                        for (auto& t : innerThreads) {
                            t.join();
                        }
                        increments+=14;
                        batch.clear();
                    }else
                        batch.push_back({i, j});
                    
                    if (!(valid < toBeAdded))
                        break;
                }
                if (!(valid < toBeAdded))
                    break;
            }
        }
        if (!(valid < toBeAdded))
        {
            string output2 = format("Number of iterations for extending: {}\n", increments);
            std::cout << output2;
            for (pair<int, int> point : emptySet)
            {
                std::cout << point.first << " " << point.second << "|";
            }
            std::cout << endl;
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
                file<<endl;
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