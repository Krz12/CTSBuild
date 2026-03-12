#pragma once

#include "key.hpp"

enum action_type
{
    move_forward,
    move_backwards,
    move_left,
    move_right,
    zoom_in,
    zoom_out,
    last_action
};

class action
{
public:
    action() = default;
    ~action() = default;

    void set_trigger(key k)
    {
        trigger = k;
    }

    key get_trigger() const { return trigger; }

private:
    key trigger = key::NONE;
};