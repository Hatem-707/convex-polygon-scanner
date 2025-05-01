#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include "reusableFunctions.hpp"
#include "CImg.h"

using namespace std;
using namespace cimg_library;

vector<pair<int, int>> parse(string line) {
    vector<pair<int, int>> output;
    string buffer;
    string buffer2;
    int first, second;
    for (const char& ch : line) {
        if (ch != '|') {
            buffer += ch;
        }
        else {
            for (const char& ch2 : buffer) {
                if (ch2 != ' ') {
                    buffer2 += ch2;
                }
                else {
                    first = stoi(buffer2);
                    buffer2.clear();
                }
            }
            second = stoi(buffer2);
            output.push_back({ first, second });
            buffer2.clear();
            buffer.clear();
        }
    }
    return output;
}

int main() {

    vector<string> files = { "/home/hatem/cpp_vscode/algorithgmProject/code/extensionData.txt", 
        "/home/hatem/cpp_vscode/algorithgmProject/code/falseData.txt" };
    vector<string> output = { "true", "false" };
    

    for (int i = 0; i < 2; ++i) {
        ifstream inputFile;
        inputFile.open(files[i]);
        string line;
        int j = 0;
        while (getline(inputFile, line))
        {
            CImg<unsigned char> image(606, 606, 1, 1, 0);
            vector<pair<int, int>> points = parse(line);
            for (pair<int, int> point : points) {
                int x = point.first * 6;
                int y = point.second * 6;
                for (int m = 0; m < 6; ++m) {
                    for (int n = 0; n < 6; ++n) {
                        image(x + m, y + n, 0, 0) = static_cast<unsigned char>(255);
                    }
                }
            }
            string temp = output[i]+to_string(j)+".png";
            const char* saveLocation = temp.c_str();
            image.save_png(saveLocation);
            ++j;
        }
    }
}
