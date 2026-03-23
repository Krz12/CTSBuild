#pragma once
#include <glad/glad.h>
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
#include "scene_tree.hpp"
using namespace std;

class scene {
    protected:
    shared_ptr<scene_tree> __tree = make_shared<scene_tree>();
    virtual void render_2d(const vector<int> &to_render) {
        glDisable(GL_DEPTH_TEST);
        for (int i : to_render)
        {
            abstract_game_object& obj = __tree->get_object(i);
            game_object_2d* obj_2d = dynamic_cast<game_object_2d*>(&obj);
            if(obj_2d) obj_2d->render();
        }
    }

    virtual void render_3d(const vector<int> &to_render) {
        glEnable(GL_DEPTH_TEST);
        for (int i : to_render)
        {
            abstract_game_object& obj = __tree->get_object(i);
            game_object_3d* obj_3d = dynamic_cast<game_object_3d*>(&obj);
            if(obj_3d) obj_3d->render();
        }
    }

    public:
    scene_tree & tree() {
        return *__tree;
    }

    scene() {}

    virtual void update() {
        __tree->update();
    }

    virtual void render() {
        //Zrób kolejka renderowania
        vector<int> to_render;

        auto ef = [&to_render, this](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            abstract_game_object obj = __tree->get_object(u->index);
            if (!obj.active()) return false;
            return true;
        };

        auto lf = [&to_render](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            to_render.push_back(u->index);
        };

        __tree->dfs(0, ef, lf);

        render_3d(to_render);
        render_2d(to_render);
    }

    virtual ~scene() {}
};