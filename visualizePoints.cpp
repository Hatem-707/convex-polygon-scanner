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
        {80, 160}, {132, 126}, {28, 158}, {194, 88}, {134, 136}, {190, 162},
        {50, 158}, {48, 42}, {78, 56}, {86, 102}, {0, 174}, {98, 166},
        {140, 160}, {100, 186}, {172, 64}, {98, 180}, {172, 50}, {0, 4},
        {2, 6}, {4, 172}, {52, 156}, {56, 154}, {88, 102}, {94, 100},
        {170, 60}, {174, 42}, {176, 36}, {81, 159}
    };

    int screenHeight = 1000;
    int screenWidth = 1000;
    int limit = 200;
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