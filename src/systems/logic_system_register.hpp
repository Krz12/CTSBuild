#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include "logic_system.hpp"

using namespace std;

namespace logic_system_register {
    using Factory = function<unique_ptr<logic_system>()>;

    inline unordered_map<string, Factory>& factories() {
        static unordered_map<string, Factory> map;
        return map;
    }

    template<typename T>
    void register_system(const string& name) {
        factories()[name] = []() -> unique_ptr<logic_system> {
            return make_unique<T>();
        };
    }

    inline const unordered_map<string, Factory>& get_factories() {
        return factories();
    }

    inline Factory get_factory(const string& name) {
        auto& map = factories();
        auto it = map.find(name);
        return it != map.end() ? it->second : Factory{};
    }
}