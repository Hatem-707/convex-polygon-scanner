#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

using namespace std;

vector<pair<int, int>> parse(string line){
    vector<pair<int, int>> output;
    string buffer;
    string buffer2;
    int first, second;
    for (const char& ch: line){
        if (ch != '|'){
            buffer+=ch;
        }else{
            for (const char& ch2: buffer){
                if (ch2 != ' '){
                    buffer2+=ch2;
                }else{
                    first = stoi(buffer2);
                    buffer2.clear();
                }
            }
            second = stoi(buffer2);
            output.push_back({first, second});
            buffer2.clear();
            buffer.clear();
        }
    }
    return output;
}

int main(){

    ifstream formatedSets;
    ofstream extendedSets;
    string line;
    int edge = 100;
    formatedSets.open("16points.txt");
    extendedSets.open("extensionData.txt", ios::app);
    vector<vector<pair<int, int>>> result;
    while (getline(formatedSets, line))
    {
        vector<pair<int, int>> pointSet = parse(line);
        vector<pair<int, int>> reflectedX, reflectedY, reflectedDiagonally, rotated1, rotated2, rotated3;
        for (const pair<int, int>& point: pointSet){
            int x = point.first, y = point.second;
            reflectedX.push_back({edge-x,y});
            reflectedY.push_back({x,edge-y});
            reflectedDiagonally.push_back({y,x});
            rotated1.push_back({y,edge-x});
            rotated2.push_back({edge-x, edge-y});
            rotated3.push_back({edge-y,x});
        }
        result.push_back(pointSet);
        result.push_back(reflectedX);
        result.push_back(reflectedY);
        result.push_back(reflectedDiagonally);
        result.push_back(rotated1);
        result.push_back(rotated2);
        result.push_back(rotated3);
    }

    for (vector<pair<int, int>>& points: result){
        for(pair<int, int>&point: points){
            extendedSets<<point.first<<" "<<point.second<<"|";
        }
        extendedSets<<endl;
    }
    
    

}