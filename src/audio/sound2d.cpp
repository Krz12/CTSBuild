#include "sound2d.hpp"

sound2d::sound2d(ma_sound* s, sound_category cat)
    : sound(s, cat)
{}

void sound2d::play() {
    if (m_paused) {
        resume_internal();
    } else {
        ma_sound_start(m_sound.get());
    }
}

void sound2d::stop() {
    stop_internal();
}

void sound2d::pause() {
    pause_internal();
}

void sound2d::set_volume(float volume) {
    ma_sound_set_volume(m_sound.get(), volume);
}

void sound2d::loop(bool b) {
    ma_sound_set_looping(m_sound.get(), b ? MA_TRUE : MA_FALSE);
}

bool sound2d::is_playing() {
    return ma_sound_is_playing(m_sound.get()) == MA_TRUE;
}

sound_category sound2d::category() const {
    return m_category;
}
