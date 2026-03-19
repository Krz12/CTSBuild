#pragma once
#include <string>
#include <unistd.h>
#include <limits.h>
#include <filesystem>
using namespace std;

namespace file_utils {

inline string get_executable_directory() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    string path = string(result, (count > 0) ? count : 0);
    return filesystem::path(path).parent_path().string() + "/../";
}

inline string get_assets_path()
{
    return "assets/";
}

inline string get_player_data_path()
{
    return "player_data/";
}

inline void create_folders()
{
    filesystem::path base = filesystem::path(get_executable_directory());
    filesystem::create_directories(base / "assets");
    filesystem::create_directories(base / "player_data");
}

inline void init()
{
    int r = chdir(get_executable_directory().c_str());
    if (r != 0) throw runtime_error("Chdir failed");
    create_folders();
}

}