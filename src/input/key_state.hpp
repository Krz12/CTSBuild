#pragma once
#include "key.hpp"
#include <memory>

namespace input_manager { void init(); void update(); }

class key_state
{
    // Only input_manager may write to key_state internals.
    friend void input_manager::init();
    friend void input_manager::update();

private:
    bool __pressed = false;
    bool __held = false;
    float __held_time = 0.0f;
    bool __down = false;
    bool __up = false;
    
    public:
    key_state() {}
    ~key_state() {}
    bool pressed() const { return __pressed; }
    bool held() const { return __held; }
    float held_time() const { return __held_time; }
    bool down() const { return __down; }
    bool up() const { return __up; }
};