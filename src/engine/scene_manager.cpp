#include "scene_manager.hpp"
#include "game/main_menu_scene.hpp"

namespace scene_manager {
    void init()
    {
        current_scene = std::make_unique<main_menu_scene>();
    }
}