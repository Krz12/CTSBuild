#include <bits/stdc++.h>
using namespace std;
class performanceStatistics
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now(),
    currentTime = lastTime;

public:
    performanceStatistics();
    ~performanceStatistics();
};