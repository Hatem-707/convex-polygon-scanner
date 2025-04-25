#pragma once
#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include "thread"
#include "mutex"

using namespace std;

long long factorial(int x);
float crossProduct(pair<int, int> a, pair<int, int> b, pair<int, int> c);
float crossProduct(pair<long long, long long> a, pair<long long, long long> b, pair<long long, long long> c);
vector<pair<long long, long long>> grahamScan(vector<pair<long long, long long>> points);
vector<pair<long long, long long>> checkPointsForPolygonH(int n, int h, vector<pair<long long, long long>> points);
vector<pair<long long, long long>> randomPointGenerator(int n, long long x, long long y);
void threadFunctionEmptySet(mutex *mtx, int n, int h, long long x, long long y, vector<pair<long long, long long>> *emptySet, bool *found, long long *iterations);
vector<pair<int, int>> grahamScan(vector<pair<int, int>> points);
vector<pair<int, int>> checkPointsForPolygonH(int n, int h, vector<pair<int, int>> points);
vector<pair<int, int>> randomPointGenerator(int n, int x, int y);
void threadFunctionEmptySet(mutex *mtx, int n, int h, int x, int y, vector<pair<int, int>> *emptySet, bool *found, long long *iterations);
void threadFunctionEmptySetLimited(mutex *mtx, int n, int h, long long x, long long y, vector<pair<long long, long long>> *emptySet, bool *found, long long *iterations, long long limit);
bool examineNewPoint(pair<int, int> newPoint, vector<pair<int, int>> previousSet, int targetPoly, int setSize);

