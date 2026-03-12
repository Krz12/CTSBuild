#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "engine/engine_library.hpp"
#include "audio/sound_library.hpp"
#include "engine/scene_tree.hpp"
#include "math/vector2.hpp"
#include "input/input_manager.hpp"
using namespace std;

int main() {
    scene_tree t;
    abstract_game_object & p = t.get_object(0);
    game_object g = *game_object::create(t, p);

    int r = chdir(get_executable_directory().c_str());
    if (r != 0) throw runtime_error("Chdir failed");

    window main_window(window::DEFAULT_SIZE, "CTSBuild");
    main_window.vsync(false);
    input_manager::init();
    sound_engine::init();
    settings::load_all_settings();
    //sound_engine::load_file("trump", "../assets/sound_trump.wav");
    //auto trump = sound_engine::play("trump", sound_category::sfx, false);

    
    while (main_window.is_open()) {
        runtime_data::update();
        double delta_time = runtime_data::delta_time();
        main_window.update();
        input_manager::update(delta_time);
        sound_engine::update(delta_time);
    }
    
    sound_engine::uninit();
    return 0;
}