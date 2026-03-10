#pragma once
#include <string>
#include <unistd.h>
#include <limits.h>
#include <filesystem>
using namespace std;

inline string get_executable_directory() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    string path = string(result, (count > 0) ? count : 0);
    return filesystem::path(path).parent_path().string();
}