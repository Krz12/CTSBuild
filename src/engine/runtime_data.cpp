#include <chrono>
#include <iostream>
using namespace std;

#define clock chrono::high_resolution_clock

namespace runtime_data {
    chrono::time_point<clock> __last_time = clock::now(),
    __current_time = clock::now(),
    __last_time_fps = clock::now();

    double delta_time_fps = 1.0f;
    int frames = 0, __fps = 0;
    double __delta_time = 1.0f;

    int fps()
    {
        return __fps;
    }
    void update()
    {
        __current_time = clock::now();
        __delta_time = chrono::duration<double>(__current_time - __last_time).count();
        __last_time = __current_time;
        delta_time_fps = chrono::duration<double>(__current_time - __last_time_fps).count();
        frames++;
        if (delta_time_fps >= 1.0f) {
            __fps = frames;
            frames = 0;
            __last_time_fps = __current_time;
            cerr << "FPS: " << fps() << "\n";
        }
    }
    double delta_time()
    {
        return __delta_time;
    }
}