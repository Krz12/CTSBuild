#pragma once
#include <string>
#include <vector>
#include "audio/sound_categories.hpp"

namespace settings
{
    bool load_action_mappings();
    bool save_action_mappings();
    void load_all_settings();
    void load_sound_settings();
}