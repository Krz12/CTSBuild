#include "settings.hpp"
#include "input/key.hpp"

#include <fstream>
#include <optional>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include "engine/logger.hpp"
#include "input/input_manager.hpp"
#include <stdexcept>
#include "engine/file_utils.hpp"
#include "logger.hpp"
#include "audio/sound_categories.hpp"

using nlohmann::json;
using namespace std;

namespace settings
{

    const string path = file_utils::get_player_data_path() + "settings.json";
static const unordered_map<string, action_type> action_from_string = {
    {"move_forward", action_type::move_forward},
    {"move_backwards", action_type::move_backwards},
    {"move_left", action_type::move_left},
    {"move_right", action_type::move_right},
    {"zoom_in", action_type::zoom_in},
    {"zoom_out", action_type::zoom_out},
};

static const unordered_map<action_type, string> action_to_string = {
    {action_type::move_forward, "move_forward"},
    {action_type::move_backwards, "move_backwards"},
    {action_type::move_left, "move_left"},
    {action_type::move_right, "move_right"},
    {action_type::zoom_in, "zoom_in"},
    {action_type::zoom_out, "zoom_out"},
};

static const unordered_map<sound_category, string> sound_to_string = {
    {sound_category::music, "music"},
    {sound_category::sfx, "sfx"},
    {sound_category::ui, "ui"},
};

static const unordered_map<string, sound_category> sound_from_string = {
    {"music", sound_category::music},
    {"sfx", sound_category::sfx, },
    {"ui", sound_category::ui},
};

static key key_from_string(const string &str)
{
    if (str.size() == 1) {
        char c = str[0];
        if (c >= 'A' && c <= 'Z') return static_cast<key>(static_cast<size_t>(key::A) + (c - 'A'));
        if (c >= 'a' && c <= 'z') return static_cast<key>(static_cast<size_t>(key::A) + (c - 'a'));
        if (c >= '0' && c <= '9') return static_cast<key>(static_cast<size_t>(key::D0) + (c - '0'));
    }

    static const unordered_map<string, key> map = {
        {"SPACE", key::SPACE},
        {"ESCAPE", key::ESCAPE},
        {"LEFT", key::LEFT},
        {"RIGHT", key::RIGHT},
        {"UP", key::UP},
        {"DOWN", key::DOWN},
        {"LEFT_SHIFT", key::LEFT_SHIFT},
        {"RIGHT_SHIFT", key::RIGHT_SHIFT},
        {"LEFT_CONTROL", key::LEFT_CONTROL},
        {"RIGHT_CONTROL", key::RIGHT_CONTROL},
        {"MOUSE_LEFT", key::MOUSE_LEFT},
        {"MOUSE_RIGHT", key::MOUSE_RIGHT},
        {"MOUSE_MIDDLE", key::MOUSE_MIDDLE},
    };

    auto it = map.find(str);
    if (it != map.end()) return it->second;
    logger::log("Couldn't bind unknown action: " + str + "\n");
    return key::NONE;
}

static string key_to_string(key k)
{
    if (k >= key::A && k <= key::Z) return string(1, static_cast<char>('A' + (static_cast<int>(k) - static_cast<int>(key::A))));
    if (k >= key::D0 && k <= key::D9) return string(1, static_cast<char>('0' + (static_cast<int>(k) - static_cast<int>(key::D0))));

    switch (k) {
        case key::SPACE: return "SPACE";
        case key::ESCAPE: return "ESCAPE";
        case key::LEFT: return "LEFT";
        case key::RIGHT: return "RIGHT";
        case key::UP: return "UP";
        case key::DOWN: return "DOWN";
        case key::LEFT_SHIFT: return "LEFT_SHIFT";
        case key::RIGHT_SHIFT: return "RIGHT_SHIFT";
        case key::LEFT_CONTROL: return "LEFT_CONTROL";
        case key::RIGHT_CONTROL: return "RIGHT_CONTROL";
        case key::MOUSE_LEFT: return "MOUSE_LEFT";
        case key::MOUSE_RIGHT: return "MOUSE_RIGHT";
        case key::MOUSE_MIDDLE: return "MOUSE_MIDDLE";
        default: break;
    }

    return to_string(static_cast<int>(k));
}

bool create_default_mappings()
{
    logger::log("Creating default mappings!");
    input_manager::map_action(action_type::move_forward, key::W);
    input_manager::map_action(action_type::move_backwards, key::S);
    input_manager::map_action(action_type::move_left, key::A);
    input_manager::map_action(action_type::move_right, key::D);
    return save_action_mappings();
}

bool load_action_mappings()
{
    if(!input_manager::is_initialized()) throw runtime_error("Tried loading action mappings but input manager is not initialized!");
    ifstream in(path);
    if (!in.is_open())
    {
        return create_default_mappings();
    }

    json j;
    try {
        in >> j;
    } catch (const json::exception &e) {
        logger::logError("Failed to parse settings.json: " + string(e.what()));
        return create_default_mappings();
    }

    if (!j.is_object() || !j.contains("actions"))
    {
        return create_default_mappings();
    }
    const auto &actions = j["actions"];
    if (!actions.is_object()) return false;

    for (auto &[action_name, value] : actions.items()) {
        auto ait = action_from_string.find(action_name);
        if (ait == action_from_string.end()) continue;

        action_type at = ait->second;
        key k = key_from_string(value.get<string>());
        input_manager::map_action(at, k);
    }

    return true;
}

bool save_action_mappings()
{
    json j;
    json actions = json::object();

    for (const auto &p : action_to_string) {
        const auto &name = p.second;
        action_type at = p.first;
        key k = input_manager::get_action_mapping(at);
        actions[name] = key_to_string(k);
    }

    j["actions"] = move(actions);

    ofstream out(path);
    if (!out.is_open()) return false;

    out << j.dump(2) << "\n";
    return true;
}

void save_sound_settings()
{
    json j;
    json sound = json::object();

    for (const auto &p : action_to_string) {
        const auto &name = p.second;
        action_type at = p.first;
        key k = input_manager::get_action_mapping(at);
        sound[name] = key_to_string(k);
    }

    j["sound"] = move(sound);

    ofstream out(path);
    if (!out.is_open()) return;

    out << j.dump(2) << "\n";
}

void load_all_settings()
{
    load_action_mappings();
}

}