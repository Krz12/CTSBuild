#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include "graph.hpp"
#include "scene_tree.hpp"
#include "window.hpp"
#include "math/vector2.hpp"
#include "runtime_data.hpp"
#include "tree.hpp"
#include "audio/sound_engine.hpp"
#include "audio/sound_categories.hpp"
#include "file_utils.hpp"
using namespace std;

int main() {
    chdir(getExecutableDir().c_str()); 
    tree t(graph(1), 0);
    t.add_vertex(0);
    t.remove_vertex(0);
    
    scene_tree t;
    game_object obj = game_object::create(t, 0);

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