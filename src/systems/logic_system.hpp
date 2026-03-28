#pragma once
#include "engine/component.hpp"
#include <unordered_set>
#include <memory>

class logic_system {
    public:
    logic_system();
    virtual ~logic_system();
    virtual void update(component_manager &cm);
};