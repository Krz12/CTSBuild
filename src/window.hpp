#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vector2.hpp"
using namespace std;

class window {
    private:
    GLFWwindow* __api_window;

    public:
    window(vector2<int> size, string const& title) {
        __api_window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
        if (!glfwInit()) {
        //std::cerr << "Failed to initialize GLFW" << std::endl;
        }
    }

    const static vector2<int> DEFAULT_SIZE;

    bool fullscreen() const {
        return glfwGetWindowMonitor(__api_window) != nullptr;
    }

    void fullscreen(bool b) {
        if (!__api_window) throw ("API Window does not exist");
        
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if (b) {
            glfwSetWindowMonitor(__api_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        } else {
            int x = (mode->width - DEFAULT_SIZE.x) / 2;
            int y = (mode->height - DEFAULT_SIZE.y) / 2;
            glfwSetWindowMonitor(__api_window, nullptr, x, y, DEFAULT_SIZE.x, DEFAULT_SIZE.y, 0);
        }
    }

    bool const is_open() {
        return !glfwWindowShouldClose(__api_window);
    }

    bool const is_minimized() {
        return glfwGetWindowAttrib(__api_window, GLFW_ICONIFIED);
    }

    vector2<int> size() const {
        int w, h;
        glfwGetWindowSize(__api_window, &w, &h);
        return vector2<int>(w, h);
    }

    void resize(int w, int h) {
        glfwSetWindowSize(__api_window, w, h);
    }

    vector2<int> position() const {
        int x, y;
        glfwGetWindowPos(__api_window, &x, &y);
        return vector2<int>(x, y);
    }

    string title() const {
        return glfwGetWindowTitle(__api_window);
    }

    void update() {
        if (!is_open()) return;
        if (is_minimized()) return;

        
    }
};

const vector2<int> window::DEFAULT_SIZE(1280, 720);