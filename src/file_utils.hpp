#pragma once
#include <string>
#include <unistd.h>
#include <limits.h>
#include <filesystem>

inline std::string getExecutableDir() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    std::string path = std::string(result, (count > 0) ? count : 0);
    return std::filesystem::path(path).parent_path().string();
}