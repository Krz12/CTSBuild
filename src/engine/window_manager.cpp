#include "engine/window_manager.hpp"

namespace window_manager {
    window* __main_window = nullptr;
    void init() {
        __main_window =  new window(window::DEFAULT_SIZE, "CTSBuild");
    }
    window* main_window() {
        return __main_window;
    }
}