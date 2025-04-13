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

int main(int argc, const char* argv[]) {
    int n = 29;
    int h = 7;
    int x = 15;
    int y = 14;
    srand(time(0));

    long long iterations = 0;
    vector<pair<int, int>> emptySet = {
        {58, 67}, {58, 3}, {15, 99}, {10, 64}, {8, 66}, {5, 6}, {12, 12}, {69, 24},
        {85, 77}, {32, 23}, {15, 26}, {30, 58}, {42, 34}, {49, 24}, {19, 27}, {35, 29},
        {0, 37}, {0, 40}, {0, 32}, {0, 34}, {0, 36}, {0, 30}, {0, 38}, {0, 33},
        {0, 35}, {0, 31}, {0, 41}, {0, 39}, {0, 42}
    };

    vector<pair<int, int>> confirmationHull = checkPointsForPolygonH(emptySet.size(), h, emptySet);
    if (confirmationHull.size() == 1)
        cout << "Confirmed!!!\n";
    else {
        cout << "Mistake!!!\n";
        for (pair<int, int> point : confirmationHull)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        cout << endl;
    }

    return 0;
}