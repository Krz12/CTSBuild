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
    
    const static vector2<double> ZERO;

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

    vector2<double> rotate(double rotation) const {
        vector2<double> result;
        double rad = rotation * M_PI / 180.0;
        double cos_a = cos(rad);
        double sin_a = sin(rad);
        result.x = x * cos_a - y * sin_a;
        result.y = x * sin_a + y * cos_a;
        return result;
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

    vector2<T> operator+(vector2<T> const& other) const {
        vector2<T> result = *this;
        result += other;
        return result;
    }

    vector2<T> operator-(vector2<T> const& other) const {
        vector2<T> result = *this;
        result -= other;
        return result;
    }

    vector2<T> operator*(vector2<T> const& other) const {
        vector2<T> result = *this;
        result *= other;
        return result;
    }

    vector2<T> operator/(vector2<T> const& other) const {
        vector2<T> result = *this;
        result /= other;
        return result;
    }

    //vector, double operations
    vector2<T>& operator*=(T const& scalar) {
        x *= scalar;
        y *= scalar;
        __x_cache = x;
        __y_cache = y;
        __magnitude *= scalar;
        return *this;
    }

    friend vector2<T>& operator*=(T const& scalar, vector2<T> & v) {
        return v *= scalar;
    }

    vector2<T>& operator/=(T const& scalar) {
        x /= scalar;
        y /= scalar;
        __x_cache = x;
        __y_cache = y;
        __magnitude /= scalar;
        return *this;
    }

    friend vector2<T>& operator/=(T const& scalar, vector2<T> & v) {
        return v /= scalar;
    }

    vector2<T> operator*(T const& scalar) const {
        vector2<T> v(*this);
        v *= scalar;
        return v;
    }

    friend vector2<T> operator*(T const& scalar, vector2<T> const& v) {
        return v * scalar;
    }

    vector2<T> operator/(T const& scalar) const {
        vector2<T> v(*this);
        v /= scalar;
        return v;
    }

    friend vector2<T> operator/(T const& scalar, vector2<T> const& v) {
        return v / scalar;
    }
};