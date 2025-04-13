#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include "format"
#include "fstream"
#include "reusableFunctions.hpp"

using namespace std;


void increamentalThread(pair<int, int> point, vector<pair<int, int>>* emptySet, int h, int * valid, mutex* mtx){
    if (examineNewPoint(point, *emptySet, h, (*emptySet).size())){
        lock_guard<mutex> lock(*mtx);
        (*valid)++;
        (*emptySet).push_back(point);
    }
}

int main(int argc, const char *argv[])
{
    int n = 16;
    int h = 7;
    int x = 400;
    int y = 400;
    srand(time(0));
    string saveFile = "17points.txt";

    int toBoTested = 1;
    int success = 0;
    while (toBoTested--)
    {
        long long iterations = 0;
        vector<pair<int, int>> emptySet = {
            {288, 200}, {208, 344}, {112, 48}, {288, 328}, {304, 344}, {240, 168}, {280, 200}, {72, 136},
            {336, 112}, {256, 136}, {104, 0}, {392, 240}, {232, 176}, {304, 160}, {0, 104}, {120, 16},
            {360, 216}, {0, 392}, {32, 384}, {40, 120}, {80, 144}, {136, 360}, {144, 72}
        };
        
        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        std::cout << endl;

        string output = format("Number of iterations for generation: {}\n", iterations);
        std::cout << output;

        long long increments = 0;
        int valid = 0;
        int toBeAdded = 32-emptySet.size();
        for (int added = 0; added < toBeAdded; ++added)
        {
            mutex innerMtx;
            vector<pair<int, int>> batch;
            if (added > valid){
                cout<<"Maximum set length: "<<emptySet.size()<<endl;
                break;
            }
            for (int i = 0; i < x; ++i)
            {
                for (int j = 0; j < y; ++j)
                {
                    if(batch.size() == 14){
                        int old = valid; 
                        vector<thread> innerThreads;
                        for (int m=0; m<14; ++m){
                            innerThreads.emplace_back(increamentalThread, batch[m], &emptySet, h, &valid, &innerMtx);
                        }
                        for (auto& t : innerThreads) {
                            t.join();
                        }
                        increments+=14;
                        batch.clear();
                        while (old+1<valid){
                            emptySet.pop_back();
                            valid--;
                        }
                    }else
                        batch.push_back({i, j});
                    
                    if (!(valid < toBeAdded))
                        break;
                }
                if (!(valid < toBeAdded))
                    break;
            }
        }
        string output2 = format("Number of iterations for extending: {} to set size {}\n", increments, emptySet.size());
        std::cout << output2;
        for (pair<int, int> point : emptySet)
        {
            std::cout << point.first << " " << point.second << "|";
        }
        std::cout << endl;
        if (!(valid < toBeAdded))
        {
            for (pair<int, int> point : emptySet)
            {
                std::cout << point.first << " " << point.second << "|";
            }
            std::cout << endl;
            vector<pair<int, int>> confirmationHull = checkPointsForPolygonH(emptySet.size(), h, emptySet);
            if (confirmationHull.size() == 1)
            {
                success++;
                std::cout << "Confirmed!!!\n";
                ofstream file;
                file.open(saveFile, ios::app);
                for (pair<int, int> point : emptySet)
                {
                    file << point.first << " " << point.second << "|";
                }
                file<<endl;
                file.close();
            }
            else
                std::cout << "Mistake!!!\n";
        }
        else
            std::cout << "couldn't find set of size 32 \n";
        std::cout << "run: " << toBoTested << "\n";
        std::cout << "successes: " << success << "\n";
    }
    return 0;
}