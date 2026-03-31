#pragma once
#include "math/vector2.hpp"
#include "engine/window.hpp"
#include "systems/logic_system.hpp"
#include "systems/logic_system_register.hpp"

class ui_system : public logic_system {
    private:
    inline static bool registered = []() {
        logic_system_register::register_system<ui_system>("ui_system");
        return true;
    }();
    protected:
    double __scale = 1.0;
    bool  __mouse_click_captured = 0,
          __mouse_scroll_captured = 0;
    public:
    const double scale();
    const bool  mouse_click_captured(),
                mouse_scroll_captured();
    const bool occupy_click(vector2<int> corner1, vector2<int> corner2);
    const bool occupy_scroll(vector2<int> corner1, vector2<int> corner2);
    void update(component_manager &cm) override;
    static vector2<double> calculate_transform(const entity_id &object, const entity_id &parent, component_manager &cm);
};