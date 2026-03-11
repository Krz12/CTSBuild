#pragma once

#include "sound.hpp"

class sound3d : public sound
{
public:
    sound3d(ma_sound* s, sound_category cat);
    ~sound3d() = default;

    void play() override;
    void stop() override;
    void pause() override;
    void volume(float volume) override;
    float volume() override;
    void loop(bool b) override;
    bool is_playing() override;
    sound_category category() const override;

    // Metody charakterystyczne dla dźwięku 3D
    void set_position(float x, float y, float z);
    void set_velocity(float x, float y, float z);
    // Można dodać set_direction, set_cone itp.
};