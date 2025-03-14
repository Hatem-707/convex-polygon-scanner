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

float crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    // Computes (b - a) x (c - a)
    return (b.first - a.first) * (c.second - a.second) - 
           (b.second - a.second) * (c.first - a.first);
}

class Stack
{
private:
    vector<pair<int,int>> data;
public:
    Stack(int size);
    ~Stack();
    int top = 0;
    void push(pair<int, int> entry) {
        data[top] = entry;
        top++;
    }
    pair<int,int> pop(){
        if (top != 0) {
            top--;
            return data[top];
        }

    }
    pair<int,int> peek(int index){
        if (top != 0)
            return data[top-index];
    }
};

Stack::Stack(int size)
{
 data = vector<pair<int, int>>(size, (pair<int,int>) {0,0});   
}

Stack::~Stack() = default;


vector<pair<int, int>> grahamScan(vector<pair<int, int>> points) {
    // Find the pivot (lowest y; if tie, lowest x)
    auto pivot = *min_element(points.begin(), points.end(), [](auto a, auto b) {
        return (a.second < b.second) || (a.second == b.second && a.first < b.first);
    });
    
    // Remove pivot from points
    vector<pair<int, int>> others;
    for (const auto &p : points) {
        if (p != pivot)
            others.push_back(p);
    }
    
    // Sort by polar angle using atan2
    sort(others.begin(), others.end(), [pivot](auto a, auto b) {
        double angleA = atan2(a.second - pivot.second, a.first - pivot.first);
        double angleB = atan2(b.second - pivot.second, b.first - pivot.first);
        if (angleA == angleB) {
            double distA = (a.first - pivot.first) * (a.first - pivot.first) + (a.second - pivot.second) * (a.second - pivot.second);
            double distB = (b.first - pivot.first) * (b.first - pivot.first) + (b.second - pivot.second) * (b.second - pivot.second);
            return distA < distB;
        }
        return angleA < angleB;
    });
    
    // Build the hull
    vector<pair<int, int>> hull;
    hull.push_back(pivot);
    for (const auto &p : others) {
        // Remove points that would make a non-left turn
        while (hull.size() >= 2 &&
               crossProduct(hull[hull.size()-2], hull[hull.size()-1], p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    
    return hull;
}



int main()
{
    const int width = 1000;
    const int height = 1000;
    const int numberOfPoints = 5;
    const int sides = 4;
    int locatedPoints = 0;
    set<pair<int, int>> points;
    vector<pair<int, int>> pointsVector;
    srand(time(0));

    while (locatedPoints < numberOfPoints)
    {
        int x = rand() % width;
        int y = rand() % height;
        bool collinear = false;

        // Check collinearity with all pairs of existing points
        for (auto it1 = points.begin(); it1 != points.end() && !collinear; ++it1)
        {
            for (auto it2 = next(it1); it2 != points.end(); ++it2)
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