#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include "raylib.h"
#include "thread"
#include "mutex"
#include "reusableFunctions.hpp"



void threadFunctionCache(mutex &mtx, int n, int h, vector<pair<int, int>> &emptySet)
{
}

void threadFunctionDimensions(mutex &mtx, int n, int h, vector<pair<int, int>> &emptySet)
{
}

int main()
{
    const int width = 1000;
    const int height = 1000;
    const int numberOfPoints = 13;
    const int sides = 6;
    int locatedPoints = 0;
    set<pair<int, int>> points;
    vector<pair<int, int>> pointsVector;
    srand(time(0));

    while (locatedPoints < numberOfPoints)
    {
        int x = rand() % width;
        int y = rand() % height;
        bool collinear = false;

        for (auto it1 = points.begin(); it1 != points.end() && !collinear; ++it1)
        {
            for (auto it2 = next(it1); it2 != points.end(); ++it2)
            {
                int product = crossProduct(*it1, *it2, (pair<int, int>){x, y});

                if (product == 0)
                {
                    collinear = true;
                    break;
                }
            }
        }

        if (!collinear)
        {
            points.emplace(x, y);
            pointsVector.push_back((pair<int, int>){x, y});
            locatedPoints++;
        }
    }

    vector<vector<int>> shapes((factorial(numberOfPoints) / (factorial(sides) * factorial(numberOfPoints - sides))), vector<int>(sides));

    vector<bool> combinationsVector(numberOfPoints);
    std::fill(combinationsVector.begin(), combinationsVector.begin() + sides, true);
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

    vector<int> pointsX(sides);
    vector<int> pointsY(sides);
    vector<vector<pair<int, int>>> convexPolygons;
    for (const vector<int> shape : shapes)
    {
        vector<pair<int, int>> currentPoints;
        for (int index : shape)
        {
            currentPoints.push_back(pointsVector[index]);
        }
        vector<pair<int, int>> hull = grahamScan(currentPoints);
        if (hull.size() == sides)
            convexPolygons.push_back(hull);
    }

    InitWindow(width, height, "General Position Points");
    SetTargetFPS(60);

    vector<Color> polygonColors;
    for (int i = 0; i < convexPolygons.size(); i++)
    {
        Color currentColor = {rand() % 255, rand() % 255, rand() % 255, 255};
        polygonColors.push_back(currentColor);
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (const auto &p : points)
        {
            DrawCircle(p.first, p.second, 5, BLACK);
        }

        for (int m = 0; m < convexPolygons.size(); m++)
        {
            vector<pair<int, int>> polygon = convexPolygons[m];
            for (int i = 0; i < sides; i++)
            {
                DrawLine(polygon[i].first, polygon[i].second, polygon[(i + 1) % sides].first, polygon[(i + 1) % sides].second, polygonColors[m]);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}