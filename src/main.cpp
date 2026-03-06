#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.hpp"
#include "math/vector2.hpp"
#include "runtime_data.hpp"
#include <chrono>
#include "graph.hpp"
#include "tree.hpp"
using namespace std;

int main() {
    window main_window(window::DEFAULT_SIZE, "CTSBuild");
    
    while (main_window.is_open()) {
        runtime_data::update();
        main_window.update();
    }

    return 0;
}