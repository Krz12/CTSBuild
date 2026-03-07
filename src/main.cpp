#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "graph.hpp"
#include "window.hpp"
#include "math/vector2.hpp"
#include "runtime_data.hpp"
#include "tree.hpp"
using namespace std;

int main() {
    window main_window(window::DEFAULT_SIZE, "CTSBuild");
    main_window.vsync(false);

    while (main_window.is_open()) {
        runtime_data::update();
        main_window.update();
    }

    return 0;
}