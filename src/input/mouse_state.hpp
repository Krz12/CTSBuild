#pragma once

#include <GLFW/glfw3.h>
#include "math/vector2.hpp"

namespace input_manager { void init(); void update(); }

class mouse_state
{
    // Only input_manager may write to mouse_state internals.
    friend void input_manager::init();
    friend void input_manager::update();

private:
    vector2<int> position;
    vector2<int> position_delta;

    void update()
    {
        GLFWwindow* win = glfwGetCurrentContext();
        if (!win) return;

        double x, y;
        glfwGetCursorPos(win, &x, &y);

        vector2<int> new_pos{static_cast<int>(x), static_cast<int>(y)};
        position_delta = new_pos - position;
        position = new_pos;
    }

public:
    mouse_state() = default;
    ~mouse_state() = default;

    vector2<int> get_position() const { return position; }
    vector2<int> get_position_delta() const { return position_delta; }
};