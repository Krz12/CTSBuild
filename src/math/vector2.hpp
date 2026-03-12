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
    T x, y;

    vector2(T x, T y) : x(x), y(y) {
        
    }

    vector2() : x(0), y(0) {

    }

    double magnitude_squared() {
        return x * x + y * y;
    }

    double magnitude() {
        check_cache();
        if (__magnitude == -INFINITY) 
            __magnitude = sqrt(magnitude_squared());
        return __magnitude;
    }

    double angle() {
        check_cache();
        if (__angle == -INFINITY)
            __angle = atan2(y, x);
        return __angle;
    }

    double dot(vector2<T> const& other) {
        return x * other.x + y * other.y;
    }

    void normalize() {
        double inverse_magnitude = 1 / magnitude();
        x *= inverse_magnitude;
        y *= inverse_magnitude;
        __x_cache = x;
        __y_cache = y;
        __magnitude = 1;
    }

    //vector, vector operations
    vector2<T>& operator+=(vector2<T> const& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    vector2<T>& operator-=(vector2<T> const& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    vector2<T>& operator*=(vector2<T> const& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    vector2<T>& operator/=(vector2<T> const& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    vector2<T> operator+(vector2<T> const& other) {
        vector2<T> result = *this;
        result += other;
        return result;
    }

    vector2<T> operator-(vector2<T> const& other) {
        vector2<T> result = *this;
        result -= other;
        return result;
    }

    vector2<T> operator*(vector2<T> const& other) {
        vector2<T> result = *this;
        result *= other;
        return result;
    }

    vector2<T> operator/(vector2<T> const& other) {
        vector2<T> result = *this;
        result /= other;
        return result;
    }

    //vector, double operations
    vector2<T>& operator*=(double const& scalar) {
        x *= scalar;
        y *= scalar;
        __x_cache = x;
        __y_cache = y;
        __magnitude *= scalar;
        return *this;
    }

    friend vector2<T>& operator*=(double const& scalar, vector2<T> const& v) {
        return v *= scalar;
    }

    vector2<T>& operator/=(double const& scalar) {
        x /= scalar;
        y /= scalar;
        __x_cache = x;
        __y_cache = y;
        __magnitude /= scalar;
        return *this;
    }

    friend vector2<T>& operator/=(double const& scalar, vector2<T> const& v) {
        return v /= scalar;
    }

    vector2<T> operator*(double const& scalar) {
        vector2<T> v(*this);
        v *= scalar;
        return v;
    }

    friend vector2<T>& operator*(double const& scalar, vector2<T> const& v) {
        return v * scalar;
    }

    vector2<T> operator/(double const& scalar) {
        vector2<T> v(*this);
        v /= scalar;
        return v;
    }

    friend vector2<T>& operator/(double const& scalar, vector2<T> const& v) {
        return v / scalar;
    }
};