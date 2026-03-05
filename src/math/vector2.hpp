#pragma once

#include <cmath>
using namespace std;

template <typename T>
class vector2 {
    private:
    double __magnitude = 0;
    double __angle = 0;

    T __x_cache = 0, __y_cache = 0;

    bool check_cache() {
        if (x != __x_cache || y != __y_cache) {
            __magnitude = -INFINITY;
            __angle = -INFINITY;
            __x_cache = x;
            __y_cache = y;
            return false;
        }
        return true;
    }

    public:
    double magnitude_squared() {
        return x * x + y * y;
    }

    public:
    T x, y;

    vector2(T x, T y) : x(x), y(y) {

    }

    double magnitude() {
        check_cache();
        if (__magnitude == -INFINITY) 
            __magnitude = sqrt(magnitude_squared());
        return __magnitude;
    }
};