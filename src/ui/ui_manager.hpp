#pragma once
#include "math/vector2.hpp"
#include "engine/window.hpp"

namespace ui_manager {
    const double scale();
    const bool  mouse_click_captured(),
                mouse_scroll_captured();
    const bool occupy_click(vector2<int> corner1, vector2<int> corner2);
    const bool occupy_scroll(vector2<int> corner1, vector2<int> corner2);
    void update(const window &w);
};