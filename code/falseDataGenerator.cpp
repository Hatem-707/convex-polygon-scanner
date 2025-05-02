#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include "reusableFunctions.hpp"

using namespace std;

int main() {
    int toBeGenrated = 350;
    ofstream outputFile;
    outputFile.open("newFalseData.txt", ios::out);
    while (toBeGenrated)
    {
        srand(time(0));
        vector<pair<int, int>> set = randomPointGenerator(16, 100, 100);
        vector<pair<int, int>> hull = checkPointsForPolygonH(16, 6, set);
        if (hull.size() == 6) {
            for (pair<int, int> point : set) {
                outputFile << point.first << " " << point.second << "|";
            }
            outputFile << endl;
            toBeGenrated--;
        }
    }
    outputFile.close();
}
