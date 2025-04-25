#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include "raylib.h"

using namespace std;

int main(){
    vector<pair<int, int>> emptySet = {
        {74, 99}, {3, 11}, {23, 39}, {46, 29}, {84, 70}, {68, 81},
        {16, 88}, {80, 68}, {54, 25}, {94, 1}, {58, 63}, {15, 25},
        {68, 80}, {0, 97}, {20, 87}, {81, 69}
    };

    int screenHeight = 1000;
    int screenWidth = 1000;
    int limit = 100;
    int scale = screenHeight/limit;
    InitWindow(screenWidth, screenHeight, "General Position Points");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (const auto &p : emptySet)
        {
            DrawCircle(p.first * scale, p.second * scale, 5, BLACK);
        }
        DrawCircleLines(emptySet[emptySet.size()-1].first*scale, emptySet[emptySet.size()-1].second*scale, 15, RED);
        EndDrawing();
    }
    


}