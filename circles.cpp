#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <numeric>
#include "fstream"
#include "format"
#include "reusableFunctions.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    int n = 12;
    int h = 6;

    fstream outputFile;
    outputFile.open("circles2.txt", ios::app);

    for (int r = 26; r < 1e8; r *= 5)
    {

        long long x = r / 2;
        long y = 8;

        cout << x << " " << y << "\n";

        vector<long long> history;
        srand(time(0));
        for (int m = 0; m < 15; m++)
        {
            long long iterations = 0;
            vector<pair<long long, long long>> emptySet;
            bool found = false;
            mutex mtx;
            vector<thread> threads;
            if (x != 0 && y != 0)
            {
                for (int i = 0; i < 14; i++)
                {
                    threads.emplace_back(
                        static_cast<void (*)(mutex *, int, int, long long, long long, vector<pair<long long, long long>> *, bool *, long long *)>(
                            threadFunctionEmptySet),
                        &mtx, n, h, x, y, &emptySet, &found, &iterations);
                }

                for (auto &t : threads)
                {
                    t.join();
                }
            }
            history.push_back(iterations);
        }

        long long avg = reduce(history.begin(), history.end(), 0ll, plus<>()) / 15;

        outputFile << avg << " " << x << " " << y << "\n";
        cout << avg << " " << x << " " << y << "\n";
    }
    outputFile.close();

    return 0;
}