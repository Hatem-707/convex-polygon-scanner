#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
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

float crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c){
    // formula: (b-a)x(c-a), (ux*vy - uy*vx)
    pair<int, int> vec1 = {b.first - a.first, b.second - a.second};
    pair<int, int> vec2 = {c.first - a.first, c.second - a.second};
    return (vec1.first*vec2.second)-(vec1.second*vec2.first);
}


vector<pair<int, int>> grahamScan(vector<pair<int, int>> points){
    pair<int,int> minPoint = {INT32_MIN, INT32_MAX};
    for (pair<int, int> point: points){
        if (point.second < minPoint.second || (point.second == minPoint.second && point.first > minPoint.first)){
            minPoint = point;
        }
    }

    vector<pair<int, int>> others;

    for (pair<int, int> point: points){
        if (point != minPoint) others.push_back(point);
    }

    sort(others.begin(), others.end(), [minPoint](pair<int,int> a, pair<int,int>b){
        int orientation = (b.second-minPoint.second)*(minPoint.first-a.first) - (a.second - minPoint.second)*(minPoint.first - b.first);
        if (orientation == 0){
            return (a.first-minPoint.first)*(a.first-minPoint.first) + (a.second - minPoint.second)*(a.second - minPoint.second) < 
                    (b.first-minPoint.first)*(b.first-minPoint.first) + (b.second - minPoint.second)*(b.second - minPoint.second);         
        }
        return orientation<0;
    });
    vector<pair<int, int>> hull;
    hull.push_back(minPoint);
    hull.push_back(others[0]);
    for (int i=1; i<others.size(); i++){
        if(crossProduct(hull[hull.size()-2], hull[hull.size()-1], others[i])>=0){
            hull.push_back(others[i]);
        }else{
            while(crossProduct(hull[hull.size()-2], hull[hull.size()-1], others[i])<0){
                hull.pop_back();
            }
            hull.push_back(others[i]);
        }
    }

    return hull;

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
                int product = crossProduct(*it1, *it2, (pair<int,int>){x,y});

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

    vector<vector<int>> shapes((factorial(numberOfPoints)/(factorial(sides)*factorial(numberOfPoints-sides))), vector<int>(sides));

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

    vector<int> pointsX(sides);
    vector<int> pointsY(sides);
    vector<vector<pair<int, int>>> convexPolygons;
    for (const vector<int> shape : shapes)
    {
        vector<pair<int, int>> currentPoints;
        for (int index: shape){
            currentPoints.push_back(pointsVector[index]);
        }
        vector<pair<int, int>> hull = grahamScan(currentPoints);
        if (hull.size() == sides) convexPolygons.push_back(hull);

    }

    InitWindow(width, height, "General Position Points");
    SetTargetFPS(60);

    vector<Color> polygonColors;
    for (int i=0; i<convexPolygons.size(); i++){
        Color currentColor =  {rand()%255, rand()%255, rand()%255, 255};
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

        for (int m=0; m<convexPolygons.size(); m++){
            vector<pair<int, int>> polygon = convexPolygons[m];
            for (int i=0; i<sides; i++){
                DrawLine(polygon[i].first, polygon[i].second, polygon[(i+1)%sides].first, polygon[(i+1)%sides].second, polygonColors[m]);
            }
        }        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}