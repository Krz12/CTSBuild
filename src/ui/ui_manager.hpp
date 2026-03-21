#pragma once
#include "ui/ui_anchor.hpp"
#include "ui/ui_element.hpp"
#include "math/vector2.hpp"
#include "engine/window.hpp"

class ui_manager {
    private: 
    double __scale;
    bool  __mouse_click_captured,
          __mouse_scroll_captured;
    public:
    const double scale();
    const bool  mouse_click_captured(),
                mouse_scroll_captured();
    const bool occupy_click(vector2<int> corner1, vector2<int> corner2);
    const bool occupy_scroll(vector2<int> corner1, vector2<int> corner2);
    void update(const window &w);
};