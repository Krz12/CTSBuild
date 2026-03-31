#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "engine/engine_library.hpp"
#include "audio/sound_library.hpp"
#include "math/vector2.hpp"
#include "input/input_manager.hpp"
#include "game/main_menu_scene.hpp"
#include "engine/scene_manager.hpp"
using namespace std;
using namespace window_manager;

int main() {
    file_utils::init();
    /*scene_tree t;
    abstract_game_object & p = t.get_object(0);
    game_object g = *game_object::create(t, p);*/
    window_manager::init();
    main_window()->vsync(false);
    input_manager::init();
    sound_engine::init();
    settings::load_all_settings();
    //sound_engine::load_file("trump", file_utils::get_assets_path()+"sound_trump.wav");
    //auto trump = sound_engine::play("trump", sound_category::sfx, false);

    //Po wczytaniu całego silnika wczytujemy menu
    scene_manager::init();

    float quadVertices[] = {
    // pos    // uv
    0.0f, 0.0f,  0.0f, 0.0f,
    1.0f, 0.0f,  1.0f, 0.0f,
    1.0f, 1.0f,  1.0f, 1.0f,

    0.0f, 0.0f,  0.0f, 0.0f,
    1.0f, 1.0f,  1.0f, 1.0f,
    0.0f, 1.0f,  0.0f, 1.0f
    };
    
    while (main_window()->is_open()) {
        runtime_data::update();
        main_window()->update();
        input_manager::update();
        sound_engine::update();
        scene_manager::current_scene.get()->update();
        scene_manager::current_scene.get()->render();
    }
    
    sound_engine::uninit();
    return 0;
}