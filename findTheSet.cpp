#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include "raylib.h"

using namespace std;

long long factorial(int x)
{
    long long product = 1;
    for (int i = 0; i < x; i++)
    {
        product *= (i + 1);
    }
    return product;
}

float crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    return (b.first - a.first) * (c.second - a.second) - (b.second - a.second) * (c.first - a.first);
}

vector<pair<int, int>> grahamScan(vector<pair<int, int>> points) {
    // Find the point with the lowest y-coordinate, break ties by x-coordinate
    pair<int, int> minPoint = points[0];
    for (const auto& point : points) {
        if (point.second < minPoint.second || (point.second == minPoint.second && point.first < minPoint.first)) {
            minPoint = point;
        }
    }

    // Sort points by polar angle with minPoint
    sort(points.begin(), points.end(), [minPoint](const pair<int, int>& a, const pair<int, int>& b) {
        int orientation = (a.first - minPoint.first) * (b.second - minPoint.second) - (a.second - minPoint.second) * (b.first - minPoint.first);
        if (orientation == 0) {
            return (a.first - minPoint.first) * (a.first - minPoint.first) + (a.second - minPoint.second) * (a.second - minPoint.second) <
                   (b.first - minPoint.first) * (b.first - minPoint.first) + (b.second - minPoint.second) * (b.second - minPoint.second);
        }
        return orientation > 0;
    });

    // Initialize the convex hull
    vector<pair<int, int>> hull;
    for (const auto& point : points) {
        while (hull.size() >= 2 && crossProduct(hull[hull.size() - 2], hull.back(), point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}

void findSet(int width, int height, int numberOfPoints, int sides, bool &found, int &trials, set<pair<int, int>> &points, vector<pair<int, int>> &pointsVector, vector<vector<pair<int, int>>> &convexPolygons, mutex &mtx) {
    while (!found) {
        {
            lock_guard<mutex> lock(mtx);
            if (found) return;
            trials++;
        }

        int locatedPoints = 0;
        set<pair<int, int>> localPoints;
        vector<pair<int, int>> localPointsVector;
        vector<vector<pair<int, int>>> localConvexPolygons;

        srand(time(0) + trials);
        while (locatedPoints < numberOfPoints) {
            int x = rand() % width;
            int y = rand() % height;
            bool collinear = false;

            // Check collinearity with all pairs of existing points
            for (auto it1 = localPoints.begin(); it1 != localPoints.end() && !collinear; ++it1) {
                for (auto it2 = next(it1); it2 != localPoints.end(); ++it2) {
                    // Calculate area of triangle (integer arithmetic)
                    int area = x * (it1->second - it2->second) +
                               it1->first * (it2->second - y) +
                               it2->first * (y - it1->second);

                    if (area == 0) {
                        collinear = true;
                        break;
                    }
                }
            }

            if (!collinear) {
                localPoints.emplace(x, y);
                localPointsVector.push_back((pair<int, int>){x, y});
                locatedPoints++;
            }
        }

        vector<vector<int>> shapes((factorial(numberOfPoints)/(factorial(sides)*factorial(numberOfPoints-sides))), vector<int>(sides));

        vector<bool> combinationsVector(numberOfPoints);
        fill(combinationsVector.begin(), combinationsVector.begin() + sides, true);
        int fillIndex = 0;
        do {
            vector<int> temp;
            for (int i = 0; i < numberOfPoints; i++) {
                if (combinationsVector[i]) {
                    temp.push_back(i);
                }
            }
            shapes[fillIndex] = temp;
            fillIndex++;
        } while (prev_permutation(combinationsVector.begin(), combinationsVector.end()));

        for (const vector<int> shape : shapes) {
            vector<pair<int, int>> currentPoints;
            for (int index: shape) {
                currentPoints.push_back(localPointsVector[index]);
            }
            vector<pair<int, int>> hull = grahamScan(currentPoints);
            if (hull.size() == sides) localConvexPolygons.push_back(hull);
        }

        if (localConvexPolygons.size() == 0) {
            lock_guard<mutex> lock(mtx);
            if (!found) {
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
    bool found = false;
    int trials = 0;
    srand(time(0));

    set<pair<int, int>> points;
    vector<pair<int, int>> pointsVector;
    vector<vector<pair<int, int>>> convexPolygons;
    mutex mtx;

    vector<thread> threads;
    for (int i = 0; i < 12; i++) {
        threads.emplace_back(findSet, width, height, numberOfPoints, sides, ref(found), ref(trials), ref(points), ref(pointsVector), ref(convexPolygons), ref(mtx));
    }

    for (auto &t : threads) {
        t.join();
    }

    cout << "Number of trials: " << trials << endl;

    InitWindow(width, height, "General Position Points");
    SetTargetFPS(60);

    vector<Color> polygonColors;
    for (int i = 0; i < convexPolygons.size(); i++) {
        Color currentColor = {rand() % 255, rand() % 255, rand() % 255, 255};
        polygonColors.push_back(currentColor);
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto &p : points) {
            DrawCircle(p.first, p.second, 5, BLACK);
        }

        for (int m = 0; m < convexPolygons.size(); m++) {
            vector<pair<int, int>> polygon = convexPolygons[m];
            for (int i = 0; i < sides; i++) {
                DrawLine(polygon[i].first, polygon[i].second, polygon[(i + 1) % sides].first, polygon[(i + 1) % sides].second, polygonColors[m]);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}