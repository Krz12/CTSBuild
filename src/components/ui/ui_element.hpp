#include "ui/ui_alligment.hpp"
#include "math/vector2.hpp"
struct ui_element
{
    ui_anchor anchor;
    ui_pivot pivot;
    vector2<double> design_offset;
    vector2<double> design_size;
    bool does_capture_mouse_click, does_capture_mouse_scroll;
};