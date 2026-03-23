#include <algorithm>
#include <cmath>

class color {
public:
    float r, g, b, a;  // od 0 do 1

    color() : r(1), g(1), b(1), a(1) {}                // domyślnie biały
    color(float gray, float alpha = 1.0f) : r(gray), g(gray), b(gray), a(alpha) {}
    color(float red, float green, float blue, float alpha = 1.0f)
        : r(red), g(green), b(blue), a(alpha) {}

    // Konwersja z/w unsigned char (0-255)
    static color fromBytes(unsigned char red, unsigned char green,
                           unsigned char blue, unsigned char alpha = 255) {
        return color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
    }
    void toBytes(unsigned char& red, unsigned char& green,
                 unsigned char& blue, unsigned char& alpha) const {
        red   = static_cast<unsigned char>(std::clamp(r, 0.0f, 1.0f) * 255);
        green = static_cast<unsigned char>(std::clamp(g, 0.0f, 1.0f) * 255);
        blue  = static_cast<unsigned char>(std::clamp(b, 0.0f, 1.0f) * 255);
        alpha = static_cast<unsigned char>(std::clamp(a, 0.0f, 1.0f) * 255);
    }

    // Dostęp w stylu tablicowym (przydatne przy przekazywaniu do OpenGL)
    float* data() { return &r; }
    const float* data() const { return &r; }

    // Operatory arytmetyczne (składanie kolorów)
    color operator+(const color& other) const {
        return color(r + other.r, g + other.g, b + other.b, a + other.a);
    }
    color operator-(const color& other) const {
        return color(r - other.r, g - other.g, b - other.b, a - other.a);
    }
    color operator*(float scalar) const {
        return color(r * scalar, g * scalar, b * scalar, a * scalar);
    }
    color operator*(const color& other) const { // składowe mnożenie
        return color(r * other.r, g * other.g, b * other.b, a * other.a);
    }
    color& operator+=(const color& other) { *this = *this + other; return *this; }
    color& operator-=(const color& other) { *this = *this - other; return *this; }
    color& operator*=(float scalar) { *this = *this * scalar; return *this; }

    // Modyfikacja składowych
    color& clamp() {
        r = std::clamp(r, 0.0f, 1.0f);
        g = std::clamp(g, 0.0f, 1.0f);
        b = std::clamp(b, 0.0f, 1.0f);
        a = std::clamp(a, 0.0f, 1.0f);
        return *this;
    }

    // Mieszanie (alpha blending)
    color blend(const color& background) const {
        // Standardowa formuła: wynik = foreground * alpha_fg + background * (1 - alpha_fg)
        float out_a = a + background.a * (1 - a);
        if (out_a == 0) return color(0,0,0,0);
        color result;
        result.r = (r * a + background.r * background.a * (1 - a)) / out_a;
        result.g = (g * a + background.g * background.a * (1 - a)) / out_a;
        result.b = (b * a + background.b * background.a * (1 - a)) / out_a;
        result.a = out_a;
        return result;
    }

    // Predefiniowane kolory (jak w CSS)
    static color white()   { return color(1,1,1,1); }
    static color black()   { return color(0,0,0,1); }
    static color red()     { return color(1,0,0,1); }
    static color green()   { return color(0,1,0,1); }
    static color blue()    { return color(0,0,1,1); }
    static color yellow()  { return color(1,1,0,1); }
    static color magenta() { return color(1,0,1,1); }
    static color cyan()    { return color(0,1,1,1); }
    static color transparent() { return color(0,0,0,0); }
};