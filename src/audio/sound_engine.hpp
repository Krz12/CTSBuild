#include <memory>
#include "sound.hpp"
using namespace std;

namespace sound_engine {
    shared_ptr<sound> play();
    void load_file();
    void update();
}