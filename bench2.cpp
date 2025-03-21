#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include "fstream"
#include "format"
#include "reusableFunctions.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    int n = 12;
    int h = 6;
    int x = 20;
    int y = 20;
    fstream outputFile;
    outputFile.open("benchmark2-1.txt", ios::app);

    for (int u = 0; u < 100; u++)
    {
        srand(25469+u);
        long long iterations = 0;
        vector<pair<int, int>> emptySet;
        bool found = false;
        mutex mtx;
        vector<thread> threads;

        double initialClock = clock();
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

        double finalClock = clock();

        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        cout << endl;

        string output = format("Number of iterations: {}\n", iterations);
        std::cout << output;
        outputFile<<iterations<<" "<<(double) (finalClock-initialClock)/CLOCKS_PER_SEC<<"\n";
    }

    return 0;
}