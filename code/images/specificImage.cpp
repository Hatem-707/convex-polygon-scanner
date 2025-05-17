#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

int main()
{
    int numbers[] = {
        0, 61, 0, 61, 48, 82, 48, 82, 50, 47, 50, 47, 74, 85, 74, 85, 74, 82,
        74, 82, 7, 3, 7, 3, 68, 78, 68, 78, 50, 35, 50, 35, 52, 22, 52, 22,
        48, 36, 48, 36, 26, 15, 26, 15, 15, 36, 15, 36, 63, 14, 63, 14, 33, 93,
        33, 93, 40, 51, 40, 51, 42, 41, 42, 41, 52, 34, 52, 34, 42, 46, 42, 46,
        53, 74, 53, 74, 34, 48, 34, 48, 95, 60, 95, 60, 83, 40, 83, 40, 17, 7,
        17, 7, 56, 74, 56, 74, 29, 23, 29, 23, 78, 13, 78, 13, 30, 28, 30, 28,
        28, 47, 28, 47, 71, 6, 71, 6, 36, 25, 36, 25, 11, 6, 11, 6, 99, 4,
        99, 4};

    int binaryArray[] = {
        0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
        0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0};

    CImg<unsigned char> image(1000, 1000, 1, 3, 0);
    for (int i = 0; i < 32; ++i)
    {
        pair<int, int> anchor = {numbers[4 * i], numbers[4 * i + 1]};
        unsigned char colorFirst;
        unsigned char colorThird;
        if (binaryArray[i] == 1)
        {
            colorFirst = static_cast<unsigned char>(0);
            colorThird = static_cast<unsigned char>(255);
        }
        else
        {
            colorFirst = static_cast<unsigned char>(255);
            colorThird = static_cast<unsigned char>(0);
        }

        for (int j = 0; j < 10; ++j)
        {
            for (int k = 0; k < 10; ++k)
            {
                image(anchor.first * 10 + j, anchor.second * 10 + k, 0, 0) = static_cast<unsigned char>(0);
                image(anchor.first * 10 + j, anchor.second * 10 + k, 0, 1) = colorFirst;
                image(anchor.first * 10 + j, anchor.second * 10 + k, 0, 2) = colorThird;
            }
        }
    }
    image.save_png("ilustrated.png");
}