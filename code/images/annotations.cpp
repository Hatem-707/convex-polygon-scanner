#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

int main() {

    vector<string> fileNames = {"true", "false"}; 
    for (int i = 0; i < 1400; ++i) {
        ofstream trFile;
        ofstream flFile;
        trFile.open(fileNames[0]+to_string(i)+".txt", ios::out);
        flFile.open(fileNames[1]+to_string(i)+".txt", ios::out);
        trFile<<"0 0.5 0.5 1 1";
        flFile<<"1 0.5 0.5 1 1";
        flFile.close();
        trFile.close();
    }

}