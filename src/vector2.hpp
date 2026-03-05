#pragma once

#include <bits/stdc++.h>
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

    private:
    void compute_magnitude() {
        __magnitude = sqrt(magnitude_squared());
    }

    public:
    T x, y;

    vector2(T x, T y) : x(x), y(y) {

    }

    double magnitude() {
        check_cache();
        if (__magnitude == -INFINITY) compute_magnitude();
        return __magnitude;
    }

    double ratio_xy() {
        return (double)x / (double)y;
    }

    double ratio_yx() {
        return (double)y / (double)x;
    }
};