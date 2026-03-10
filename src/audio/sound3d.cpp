#include "sound3d.hpp"

sound3d::sound3d(ma_sound* s, sound_category cat)
    : sound(s, cat)
{}

void sound3d::play() {
    if (m_paused) {
        resume_internal();
    } else {
        ma_sound_start(m_sound.get());
    }
}

void sound3d::stop() {
    stop_internal();
}

void sound3d::pause() {
    pause_internal();
}

void sound3d::set_volume(float volume) {
    ma_sound_set_volume(m_sound.get(), volume);
}

void sound3d::loop(bool b) {
    ma_sound_set_looping(m_sound.get(), b ? MA_TRUE : MA_FALSE);
}

bool sound3d::is_playing() {
    return ma_sound_is_playing(m_sound.get()) == MA_TRUE;
}

sound_category sound3d::category() const {
    return m_category;
}

void sound3d::set_position(float x, float y, float z) {
    ma_sound_set_position(m_sound.get(), x, y, z);
}

void sound3d::set_velocity(float x, float y, float z) {
    ma_sound_set_velocity(m_sound.get(), x, y, z);
}