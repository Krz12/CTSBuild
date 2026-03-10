#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "engine/engine_library.hpp"
#include "audio/sound_library.hpp"
#include "math/vector2.hpp"
using namespace std;

int main() {
    int r = chdir(get_executable_directory().c_str());
    if (r != 0) throw runtime_error("Chdir failed");
    //scene_tree t;
    //game_object obj = game_object::create(t, 0);

    window main_window(window::DEFAULT_SIZE, "CTSBuild");
    main_window.vsync(false);
    sound_engine::init();
    sound_engine::load_file("trump", "../assets/sound_trump.wav");
    
    auto trump = sound_engine::play("trump", sound_category::sfx, false);
    
    while (main_window.is_open()) {
        runtime_data::update();
        main_window.update();
    }
    
    sound_engine::uninit();
    return 0;
}