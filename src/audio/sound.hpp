#include "miniaudio.h"
#include "sound_categories.hpp"
#include <memory>
class sound : public std::enable_shared_from_this<sound>
{
protected:
    ma_sound __api_sound;
    sound_category __category;
public:
    sound(ma_sound load_sound, sound_category load_category)
    {
        __api_sound = load_sound;
        __category = load_category;
    }
    virtual void play() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void set_volume(float volume) = 0; // 0.0 - 1.0
    virtual void loop(bool b) = 0;
    virtual bool is_playing() = 0;
    virtual sound_category category() = 0;
    
    virtual ~sound() {}
};