#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <fstream>
#include "reusableFunctions.hpp"

using namespace std;

void findSet(int width, int height, int numberOfPoints, int sides, bool &found, long long &trials, set<pair<int, int>> &points, vector<pair<int, int>> &pointsVector, vector<vector<pair<int, int>>> &convexPolygons, mutex &mtx)
{
    while (!found)
    {
        {
            lock_guard<mutex> lock(mtx);
            if (found)
                return;
            trials++;
        }

        int locatedPoints = 0;
        set<pair<int, int>> localPoints;
        vector<pair<int, int>> localPointsVector;
        vector<vector<pair<int, int>>> localConvexPolygons;
        while (locatedPoints < numberOfPoints)
        {
            int x = rand() % width;
            int y = rand() % height;
            bool collinear = false;

            // Check collinearity with all pairs of existing points
            for (auto it1 = localPoints.begin(); it1 != localPoints.end() && !collinear; ++it1)
            {
                for (auto it2 = next(it1); it2 != localPoints.end(); ++it2)
                {
                    // Calculate area of triangle (integer arithmetic)
                    int area = x * (it1->second - it2->second) +
                               it1->first * (it2->second - y) +
                               it2->first * (y - it1->second);

                    if (area == 0)
                    {
                        collinear = true;
                        break;
                    }
                }
            }

            if (!collinear)
            {
                localPoints.emplace(x, y);
                localPointsVector.push_back((pair<int, int>){x, y});
                locatedPoints++;
            }
        }

        vector<vector<int>> shapes((factorial(numberOfPoints) / (factorial(sides) * factorial(numberOfPoints - sides))), vector<int>(sides));

        vector<bool> combinationsVector(numberOfPoints);
        fill(combinationsVector.begin(), combinationsVector.begin() + sides, true);
        int fillIndex = 0;
        do
        {
            vector<int> temp;
            for (int i = 0; i < numberOfPoints; i++)
            {
                if (combinationsVector[i])
                {
                    temp.push_back(i);
                }
            }
            shapes[fillIndex] = temp;
            fillIndex++;
        } while (prev_permutation(combinationsVector.begin(), combinationsVector.end()));

        for (const vector<int> shape : shapes)
        {
            vector<pair<int, int>> currentPoints;
            for (int index : shape)
            {
                currentPoints.push_back(localPointsVector[index]);
            }
            vector<pair<int, int>> hull = grahamScan(currentPoints);
            if (hull.size() == sides)
                localConvexPolygons.push_back(hull);
        }

        if (localConvexPolygons.size() == 0)
        {
            lock_guard<mutex> lock(mtx);
            if (!found)
            {
                found = true;
                points = localPoints;
                pointsVector = localPointsVector;
                convexPolygons = localConvexPolygons;
            }
        }
    }
}

int main()
{
    const int width = 20;
    const int height = 20;
    const int numberOfPoints = 12;
    const int sides = 6;
    fstream outputFile;
    outputFile.open("benchmark1-2.txt", ios::app);
    
    for (int iteration = 0; iteration < 100; iteration++)
    {
        bool found = false;
        long long trials = 0;
        srand(25469+iteration);
        

        set<pair<int, int>> points;
        vector<pair<int, int>> pointsVector;
        vector<vector<pair<int, int>>> convexPolygons;
        mutex mtx;

        vector<thread> threads;
        double initialClocks = clock();
        for (int i = 0; i < 14; i++)
        {
            threads.emplace_back(findSet, width, height, numberOfPoints, sides, ref(found), ref(trials), ref(points), ref(pointsVector), ref(convexPolygons), ref(mtx));
        }

        for (auto &t : threads)
        {
            t.join();
        }
        double finalClocks = clock();
        cout << "Number of trials: " << trials << " iteration: "<< iteration <<endl;
        outputFile<<trials<<" "<< (double)(finalClocks-initialClocks) /CLOCKS_PER_SEC<<"\n";
    }

    // InitWindow(width, height, "General Position Points");
    // SetTargetFPS(60);

    // vector<Color> polygonColors;
    // for (int i = 0; i < convexPolygons.size(); i++) {
    //     Color currentColor = {rand() % 255, rand() % 255, rand() % 255, 255};
    //     polygonColors.push_back(currentColor);
    // }

    // while (!WindowShouldClose()) {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);

    //     for (const auto &p : points) {
    //         DrawCircle(p.first, p.second, 5, BLACK);
    //     }

    //     for (int m = 0; m < convexPolygons.size(); m++) {
    //         vector<pair<int, int>> polygon = convexPolygons[m];
    //         for (int i = 0; i < sides; i++) {
    //             DrawLine(polygon[i].first, polygon[i].second, polygon[(i + 1) % sides].first, polygon[(i + 1) % sides].second, polygonColors[m]);
    //         }
    //     }

    //     EndDrawing();
    // }

    // CloseWindow();
    // return 0;
}