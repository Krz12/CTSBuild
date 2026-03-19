#pragma once
#include "key_state.hpp"
#include "mouse_state.hpp"
#include "action_type.hpp"

namespace input_manager
{
    void init();
    void update(double delta_time);
    const key_state &get_key_state(const key &k);
    const key_state &get_action_state(const action_type &a);
    const mouse_state &get_mouse_state();
    const key get_action_mapping(const action_type &a);
    void map_action(const action_type &a, const key &k);
    const bool is_initialized();
}