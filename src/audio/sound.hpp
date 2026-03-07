#include "miniaudio.h"
#include "sound_categories.hpp"
#include <memory>
class sound : public std::enable_shared_from_this<sound>
{
private:
    ma_sound* api_sound;
    sound_category category;
public:
    sound(const ma_sound &load_sound, sound_category category)
    {
        api_sound
    }
    ~sound();
};