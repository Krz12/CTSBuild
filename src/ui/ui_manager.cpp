#include "ui/ui_manager.hpp"

const double ui_manager::scale()
{
    return __scale;
}

void ui_manager::update(const window &w)
{
    __mouse_click_captured = 0;
    __mouse_scroll_captured = 0;
    __scale = max(w.size().y, 1) / 1080.0;
}