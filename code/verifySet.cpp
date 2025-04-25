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
        {148, 113}, {62, 181}, {124, 102}, {180, 112}, {50, 193},
        {92, 163}, {47, 17}, {132, 150}, {88, 93}, {106, 111},
        {113, 151}, {163, 199}, {45, 35}, {104, 67}, {130, 101},
        {72, 66}, {28, 0}, {46, 30}, {58, 38}, {69, 45},
        {69, 64}, {86, 93}, {105, 67}, {134, 104}, {137, 158}
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