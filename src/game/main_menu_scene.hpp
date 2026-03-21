#pragma once
#include "engine/scene.hpp"
#include "ui/ui_manager.hpp"
#include "engine/logger.hpp"

class main_menu_scene : public scene {
    public:
    ui_manager um;
    main_menu_scene() {
        
    }
};