#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "window.hpp"
#include "vector2.hpp"
using namespace std;

int main() {
    window w(window::DEFAULT_SIZE, "TUFF");

    while (w.is_open()) {
        w.update();
    }

    return 0;
}