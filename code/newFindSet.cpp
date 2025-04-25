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

int main(int argc, const char *argv[]) {
    int n = 13;
    int h = 6;
    int x = 15;
    int y = 14;
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

    for (auto &t : threads) {
        t.join();
    }

    for (pair<int, int> point : emptySet) {
        std::cout << point.first << " " << point.second << "|";
    }
    cout<<endl;

    string output = format("Number of iterations: {}\n", iterations);
    std::cout << output;

    return 0;
}