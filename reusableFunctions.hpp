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
vector<pair<int, int>> grahamScan(vector<pair<int, int>> points);
vector<pair<int, int>> checkPointsForPolygonH(int n, int h, vector<pair<int, int>> points);
vector<pair<int, int>> randomPointGenerator(int n, int x, int y);
void threadFunctionEmptySet(mutex *mtx, int n, int h, int x, int y, vector<pair<int, int>> *emptySet, bool *found, long long *iterations);

