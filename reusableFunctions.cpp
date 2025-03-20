#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include "thread"
#include "mutex"
#include "reusableFunctions.hpp"

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

float crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
    // formula: (b-a)x(c-a), (ux*vy - uy*vx)
    pair<int, int> vec1 = {b.first - a.first, b.second - a.second};
    pair<int, int> vec2 = {c.first - a.first, c.second - a.second};
    return (vec1.first * vec2.second) - (vec1.second * vec2.first);
}

vector<pair<int, int>> grahamScan(vector<pair<int, int>> points)
{
    pair<int, int> minPoint = {INT32_MIN, INT32_MAX};
    for (pair<int, int> point : points)
    {
        if (point.second < minPoint.second || (point.second == minPoint.second && point.first > minPoint.first))
        {
            minPoint = point;
        }
    }

    vector<pair<int, int>> others;

    for (pair<int, int> point : points)
    {
        if (point != minPoint)
            others.push_back(point);
    }

    sort(others.begin(), others.end(), [minPoint](pair<int, int> a, pair<int, int> b)
         {
        int orientation = (b.second-minPoint.second)*(minPoint.first-a.first) - (a.second - minPoint.second)*(minPoint.first - b.first);
        if (orientation == 0){
            return (a.first-minPoint.first)*(a.first-minPoint.first) + (a.second - minPoint.second)*(a.second - minPoint.second) < 
                    (b.first-minPoint.first)*(b.first-minPoint.first) + (b.second - minPoint.second)*(b.second - minPoint.second);         
        }
        return orientation<0; });
    vector<pair<int, int>> hull;
    hull.push_back(minPoint);
    hull.push_back(others[0]);
    for (int i = 1; i < others.size(); i++)
    {
        if (crossProduct(hull[hull.size() - 2], hull[hull.size() - 1], others[i]) >= 0)
        {
            hull.push_back(others[i]);
        }
        else
        {
            while (crossProduct(hull[hull.size() - 2], hull[hull.size() - 1], others[i]) < 0)
            {
                hull.pop_back();
            }
            hull.push_back(others[i]);
        }
    }

    return hull;
}

vector<pair<int, int>> checkPointsForPolygonH(int n, int h, vector<pair<int, int>> points)
{
    vector<bool> pointsToCheck(n, false);
    std::fill(pointsToCheck.begin(), pointsToCheck.begin() + h, true);
    vector<pair<int, int>> currentPolygon;
    do
    {
        currentPolygon.clear();
        for (int i = 0; i < n; i++)
        {
            if (pointsToCheck[i])
                currentPolygon.push_back(points[i]);
        }
        vector<pair<int, int>> hull = grahamScan(currentPolygon);
        if (hull.size() == h)
            return hull;
    } while (prev_permutation(pointsToCheck.begin(), pointsToCheck.end()));

    return vector<pair<int, int>>(1, {-1, -1});
};

vector<pair<int, int>> randomPointGenerator(int n, int x, int y)
{
    vector<pair<int, int>> pointsVector;
    set<pair<int, int>> locatedPoints;
    while (locatedPoints.size() < n)
    {
        int right = rand() % x;
        int down = rand() % y;
        bool valid = false;
        if (locatedPoints.count((pair<int, int>){right, down}) == 0)
        {
            valid = true;
            for (const auto &point1 : locatedPoints)
            {
                for (const auto &point2 : locatedPoints)
                {
                    if (crossProduct((pair<int, int>){right, down}, point1, point2) == 0 && point1 != point2)
                    {
                        valid = false;
                        break;
                    }
                }
                if (!valid)
                        break;
            }
        }
        if (valid)
        {
            locatedPoints.emplace((pair<int, int>){right, down});
            pointsVector.push_back((pair<int, int>){right, down});
        }
    }
    return pointsVector;
}

void threadFunctionEmptySet(mutex *mtx, int n, int h, int x, int y, vector<pair<int, int>> *emptySet, bool *found, long long *iterations)
{

    while (!(*found))
    {
        vector<pair<int, int>> newPoints = randomPointGenerator(n, x, y);
        if (!(*found))
        {
            vector<pair<int, int>> polygonH = checkPointsForPolygonH(n, h, newPoints);
            if (polygonH.size() == 1)
            {
                const lock_guard<mutex> lock(*mtx);
                (*iterations)++;
                *found = true;
                *emptySet = newPoints;
                return;
            }
            const lock_guard<mutex> lock(*mtx);
            (*iterations)++;
        }
    }
}