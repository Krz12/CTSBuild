#include <memory>
#include "sound.hpp"
using namespace std;

namespace sound_engine {
    shared_ptr<sound> play();
    void load_file();
    void pause(); //zostanie w pamięci!
    void resume();
    void stop();
    void update();
}