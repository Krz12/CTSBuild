#pragma once

#include "sound.hpp"

class sound2d : public sound
{
public:
    sound2d(ma_sound* s, sound_category cat);
    ~sound2d() = default;

    void play() override;
    void stop() override;
    void pause() override;
    void set_volume(float volume) override;
    void loop(bool b) override;
    bool is_playing() override;
    sound_category category() const override;
};
