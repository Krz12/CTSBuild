#include "ui/ui_element.hpp"
#include "engine/color.hpp"
#include "engine/scene_manager.hpp"

class ui_panel : public ui_element {
    public:
    color foreground_color;
    virtual void render() override;
};