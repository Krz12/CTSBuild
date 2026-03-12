#pragma once

#include <cstddef>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include "math/graph.hpp"
#include "math/tree.hpp"
#include "math/vector3.hpp"
#include "engine/scene_tree.hpp"
#include "math/graph.hpp"
using namespace std;

class scene {
    protected:
    shared_ptr<scene_tree> __tree = make_shared<scene_tree>();

    public:
    scene_tree & tree() {
        return *__tree;
    }

    scene() {}

    virtual void update() {
        __tree->update();
    }

    virtual ~scene() {}
};