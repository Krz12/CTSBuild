#pragma once

#include <string>
#include <vector>

#include "input/action.hpp"

namespace settings
{
    bool load_action_mappings();
    bool save_action_mappings();
    void load_all_settings();
}
