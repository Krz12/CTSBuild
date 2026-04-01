#pragma once
#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "systems/logic_system.hpp"
#include "systems/logic_system_register.hpp"
#include <iostream>

struct test_comp {
    int licznik = 0;
};

class testsystem : public logic_system {
private:
    inline static bool registered = []() {
        logic_system_register::register_system<testsystem>("testsystem");
        return true;
    }();
public:
    testsystem() = default;
    ~testsystem() = default;
    void update(component_manager &cm) override {
        cm.for_each_depth_top_down<test_comp>([&](int depth){
            cm.for_each_on_depth<test_comp>(depth, [&](test_comp& t, entity_id e) {
            std::cout << "licznik: " << t.licznik << " e: " << e.index << " depth: " << depth << "\n";
            t.licznik++;
            });
        });
    }
};