#pragma once
#include <memory>
#include "engine/scene.hpp"

namespace scene_manager {
    inline std::unique_ptr<scene> current_scene;
    void init();
};