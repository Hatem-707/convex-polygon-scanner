#include <iostream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <mutex>
#include <numeric>
#include "fstream"
#include "format"
#include "reusableFunctions.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    int n = 12;
    int h = 6;
    const int MIN_COORD = 7;
    const int MAX_COORD = 11; // Increased to 100
    const int SAMPLES = 20;
    const long long TIMEOUT = 500000; // Timeout per coordinate pair
    
    fstream outputFile;
    outputFile.open("heatmap_data.txt", ios::app); // Changed to write mode
    
    srand(time(0));
    int samples_collected = 0;
    
    while (samples_collected < SAMPLES)
    {
        long long x = MIN_COORD + (rand() % (MAX_COORD - MIN_COORD + 1));
        long long y = MIN_COORD + (rand() % (MAX_COORD - MIN_COORD + 1));
        
        
        // Write results to file
        outputFile << x << " " << y << " " << TIMEOUT << "\n";
        
        samples_collected++;
        outputFile.flush(); // Ensure data is written even if program is interrupted
    }
    
    outputFile.close();
    return 0;
}