#pragma once

#include <chrono>
#include <iostream>
using namespace std;

namespace runtime_data {
    int fps();
    void update();
    double delta_time();
}