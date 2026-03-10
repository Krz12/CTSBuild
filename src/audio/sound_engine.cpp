#define MINIAUDIO_IMPLEMENTATION
#include "sound_engine.hpp"
#include "sound3d.hpp"
#include "sound2d.hpp"
#include <stdexcept>
#include <unordered_map>
#include <filesystem>

using namespace std;

namespace sound_engine {
    static ma_engine g_engine;
    // Grupy dla poszczególnych kategorii (umożliwiają niezależną kontrolę głośności)
    static ma_sound_group g_group_ui;
    static ma_sound_group g_group_sfx;
    static ma_sound_group g_group_music;

    // Mapa: nazwa dźwięku -> ścieżka pliku
    static unordered_map<string, string> g_sound_paths;

    // Pomocnicza funkcja zwracająca grupę dla danej kategorii
    static ma_sound_group* get_group(sound_category cat) {
        switch (cat) {
            case sound_category::ui:    return &g_group_ui;
            case sound_category::sfx:   return &g_group_sfx;
            case sound_category::music: return &g_group_music;
            default: return nullptr;
        }
    }

    void init() {
        ma_result result;

        // Inicjalizacja silnika
        result = ma_engine_init(nullptr, &g_engine);
        if (result != MA_SUCCESS) {
            throw runtime_error("Failed to initialize sound engine!");
        }

        // Inicjalizacja grup
        result = ma_sound_group_init(&g_engine, 0, nullptr, &g_group_ui);
        if (result != MA_SUCCESS) throw runtime_error("Failed to create UI group");

        result = ma_sound_group_init(&g_engine, 0, nullptr, &g_group_sfx);
        if (result != MA_SUCCESS) throw runtime_error("Failed to create SFX group");

        result = ma_sound_group_init(&g_engine, 0, nullptr, &g_group_music);
        if (result != MA_SUCCESS) throw runtime_error("Failed to create music group");
    }

    void uninit() {
        ma_sound_group_uninit(&g_group_music);
        ma_sound_group_uninit(&g_group_sfx);
        ma_sound_group_uninit(&g_group_ui);
        ma_engine_uninit(&g_engine);
    }

    void load_file(const string& sound_name, const string& filepath) {
        filesystem::path path(filepath);
        if (!filesystem::exists(path) || !filesystem::is_regular_file(path)) {
            throw runtime_error("Sound file not found: " + filepath);
        }

        g_sound_paths[sound_name] = filepath;
    }

    shared_ptr<sound> play(const string& sound_name, sound_category category, bool use3d) {
        auto it = g_sound_paths.find(sound_name);
        if (it == g_sound_paths.end()) {
            throw ::runtime_error("Sound not loaded: " + sound_name);
        }

        ma_sound* raw_sound = new ma_sound();
        ma_uint32 flags = 0;
        if (!use3d) {
            // Sounds are spatialized by default; disable spatialization for 2D sounds.
            flags |= MA_SOUND_FLAG_NO_SPATIALIZATION;
        }

        ma_result result = ma_sound_init_from_file(&g_engine,
                                                  it->second.c_str(),
                                                  flags,
                                                  get_group(category),
                                                  nullptr,
                                                  raw_sound);
        if (result != MA_SUCCESS) {
            delete raw_sound;
            throw ::runtime_error("Failed to initialize sound: " + sound_name);
        }

        ::shared_ptr<sound> out;
        if (use3d) {
            out = ::make_shared<sound3d>(raw_sound, category);
        } else {
            out = ::make_shared<sound2d>(raw_sound, category);
        }

        out->play();
        return out;
    }

    
    void set_listener_position(float x, float y, float z) {
        ma_engine_listener_set_position(&g_engine, 0, x, y, z);
    }

    void set_listener_velocity(float x, float y, float z) {
        ma_engine_listener_set_velocity(&g_engine, 0, x, y, z);
    }

    void set_listener_orientation(float forward_x, float forward_y, float forward_z,
                                   float up_x, float up_y, float up_z) {
        // miniaudio in this version exposes listener direction and "world up" separately.
        ma_engine_listener_set_direction(&g_engine, 0, forward_x, forward_y, forward_z);
        ma_engine_listener_set_world_up(&g_engine, 0, up_x, up_y, up_z);
    }

    void update() {
        //set_listener_position
    }
}