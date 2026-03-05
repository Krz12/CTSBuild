#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>
#include "window.hpp"
#include "math/vector2.hpp"
using namespace std;

int main() {
    window main_window(window::DEFAULT_SIZE, "CTSBuild");

    while (main_window.is_open()) {
        main_window.update();
    }

    return 0;
}