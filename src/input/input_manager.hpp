#pragma once
#include "key_state.hpp"

namespace input_manager
{
    void init();
    void update();
    const key_state &get_key_state(const key &k);
}