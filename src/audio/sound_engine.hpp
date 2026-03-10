#pragma once

#include <memory>
#include <string>
#include "sound.hpp"
#include "sound_categories.hpp"

namespace sound_engine {
    void init();
    void uninit();
    void load_file(const std::string& sound_name, const std::string& filepath);

    // Odtworzenie dźwięku o podanej nazwie (musi być wcześniej załadowana)
    // use3d = true => 3D sound (requires listener setup)
    std::shared_ptr<sound> play(const std::string& sound_name, sound_category category = sound_category::sfx, bool use3d = false);
    void update();
}