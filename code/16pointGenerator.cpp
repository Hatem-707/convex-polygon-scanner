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


int main(int argc, const char *argv[])
{
    int n = 13;
    int h = 6;
    int x = 100;
    int y = 100;
    srand(time(0));
    string saveFile = "16points.txt";

    int toBoTested = 500;
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
        int toBeAdded = 3;
        for (int added = 0; added < toBeAdded; ++added)
        {
            if (added > valid)
                break;
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
                    if (valid == toBeAdded)
                        break;
                }
                if (valid == toBeAdded)
                    break;
            }
        }
        if (valid == toBeAdded)
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
            std::cout << "couldn't find set of size 16 \n";
        std::cout << "run: " << toBoTested << "\n";
        std::cout << "successes: " << success << "\n";
    }
    return 0;
}