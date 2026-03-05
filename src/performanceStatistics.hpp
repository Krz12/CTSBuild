#include <bits/stdc++.h>
using namespace std;
class performanceStatistics
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> time_frame_end = std::chrono::high_resolution_clock::now(),
    time_frame_start = time_frame_end;
    int frames = 0;
    float __delta_time = 1.0f;

public:
    performanceStatistics();
    void frame_start()
    {
        current_time = std::chrono::high_resolution_clock::now();
        __delta_time = std::chrono::duration<float>(current_time - last_time).count();
    }
    float delta_time()
    {
        return __delta_time;
    }
    ~performanceStatistics();
};