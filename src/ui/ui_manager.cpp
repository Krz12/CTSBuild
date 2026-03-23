#include "ui/ui_manager.hpp"

namespace ui_manager
{
    double __scale = 1.0;
    bool  __mouse_click_captured = 0,
          __mouse_scroll_captured = 0;

    const double scale()
    {
        return __scale;
    }

    void update(const window &w)
    {
        __mouse_click_captured = 0;
        __mouse_scroll_captured = 0;
        __scale = w.size().y / 1080.0;
    }
} 
