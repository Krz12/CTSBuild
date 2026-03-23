#pragma once

#include <cmath>
using namespace std;

template <typename T>
class vector3 {
    private:
    double __magnitude = 0;
    double __angle = 0;

    T __x_cache = 0, __y_cache = 0, __z_cache = 0;

    bool check_cache() {
        if (x != __x_cache || y != __y_cache || z != __z_cache) {
            __magnitude = -INFINITY;
            __angle = -INFINITY;
            __x_cache = x;
            __y_cache = y;
            __z_cache = z;
            return false;
        }
        return true;
    }

    public:
    T x, y, z;

    vector3(T x, T y, T z) : x(x), y(y), z(z) {
        
    }

    const static vector3<double> ZERO;

    double magnitude_squared() {
        return x * x + y * y + z * z;
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
            __angle = atan2(y, x); //idk tbh
        return __angle;
    }

    double dot(vector3<T> const& other) {
        return x * other.x + y * other.y + z * other.z;
    }

    void normalize() {
        double inverse_magnitude = 1 / magnitude();
        x *= inverse_magnitude;
        y *= inverse_magnitude;
        z *= inverse_magnitude;
        __x_cache = x;
        __y_cache = y;
        __z_cache = z;
        __magnitude = 1;
    }

    //Slop funckja
    vector3<double> rotate(vector3<double> rotation) const {
        double rx = rotation.x * M_PI / 180.0;
        double ry = rotation.y * M_PI / 180.0;
        double rz = rotation.z * M_PI / 180.0;
        
        vector3<double> result(*this);
        
        double cos_x = cos(rx);
        double sin_x = sin(rx);
        result = vector3<double>(
            result.x,
            result.y * cos_x - result.z * sin_x,
            result.y * sin_x + result.z * cos_x
        );
        
        double cos_y = cos(ry);
        double sin_y = sin(ry);
        result = vector3<double>(
            result.x * cos_y + result.z * sin_y,
            result.y,
            -result.x * sin_y + result.z * cos_y
        );
        
        double cos_z = cos(rz);
        double sin_z = sin(rz);
        result = vector3<double>(
            result.x * cos_z - result.y * sin_z,
            result.x * sin_z + result.y * cos_z,
            result.z
        );
        return result;
    }

    //vector, vector operations
    vector3<T>& operator+=(vector3<T> const& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vector3<T>& operator-=(vector3<T> const& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vector3<T>& operator*=(vector3<T> const& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    vector3<T>& operator/=(vector3<T> const& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    vector3<T> operator+(vector3<T> const& other) const {
        vector3<T> result = *this;
        result += other;
        return result;
    }

    vector3<T> operator-(vector3<T> const& other) const {
        vector3<T> result = *this;
        result -= other;
        return result;
    }

    vector3<T> operator*(vector3<T> const& other) const {
        vector3<T> result = *this;
        result *= other;
        return result;
    }

    vector3<T> operator/(vector3<T> const& other) const {
        vector3<T> result = *this;
        result /= other;
        return result;
    }

    //vector, double operations
    vector3<T>& operator*=(T const& scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        __x_cache = x;
        __y_cache = y;
        __z_cache = z;
        __magnitude *= scalar;
        return *this;
    }

    friend vector3<T>& operator*=(T const& scalar, vector3<T> & v) {
        return v *= scalar;
    }

    vector3<T>& operator/=(T const& scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        __x_cache = x;
        __y_cache = y;
        __z_cache = z;
        __magnitude /= scalar;
        return *this;
    }

    friend vector3<T>& operator/=(T const& scalar, vector3<T> & v) {
        return v /= scalar;
    }

    vector3<T> operator*(T const& scalar) const {
        vector3<T> v(*this);
        v *= scalar;
        return v;
    }

    friend vector3<T> operator*(T const& scalar, vector3<T> const& v) {
        return v * scalar;
    }

    vector3<T> operator/(T const& scalar) const {
        vector3<T> v(*this);
        v /= scalar;
        return v;
    }

    friend vector3<T> operator/(T const& scalar, vector3<T> const& v) {
        return v / scalar;
    }
};