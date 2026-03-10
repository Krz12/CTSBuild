#pragma once

#include "miniaudio.h"
#include "sound_categories.hpp"
#include <memory>

class sound : public std::enable_shared_from_this<sound>
{
protected:
    struct SoundDeleter {
        void operator()(ma_sound* s) const {
            if (s) ma_sound_uninit(s);
        }
    };
    std::unique_ptr<ma_sound, SoundDeleter> m_sound;
    sound_category m_category;

    // Pause/resume helper state
    bool m_paused = false;
    ma_uint64 m_pausedCursor = 0;

    void pause_internal() {
        if (!m_sound || m_paused) return;
        ma_sound_get_cursor_in_pcm_frames(m_sound.get(), &m_pausedCursor);
        ma_sound_stop(m_sound.get());
        m_paused = true;
    }

    void resume_internal() {
        if (!m_sound || !m_paused) return;
        ma_sound_seek_to_pcm_frame(m_sound.get(), m_pausedCursor);
        ma_sound_start(m_sound.get());
        m_paused = false;
    }

    void stop_internal() {
        if (!m_sound) return;
        ma_sound_stop(m_sound.get());
        m_paused = false;
        m_pausedCursor = 0;
    }

public:
    sound(ma_sound* s, sound_category cat) : m_sound(s), m_category(cat) {}
    virtual ~sound() = default;

    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void set_volume(float volume) = 0;      // 0.0 – 1.0
    virtual void loop(bool b) = 0;
    virtual bool is_playing() = 0;
    virtual sound_category category() const = 0;
};