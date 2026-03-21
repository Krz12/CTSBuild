#pragma once

#include <cstddef>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include "math/graph.hpp"
#include "math/tree.hpp"
#include "math/vector3.hpp"
#include "engine/game_object.hpp"
using namespace std;

class scene_tree : virtual public abstract_tree {
    protected:

    //not recommended for use, instantiate game_object or its derived classes instead
    void create_vertex(int index, shared_ptr<abstract_game_object> & ptr) {
        ptr = make_shared<abstract_game_object>(index, __adj[index], __parent[index]);
    }

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) override {
        ptr = make_shared<edge>(index, u, v);
    }

    public:
    void add_root_object();

    abstract_game_object& get_object(int index) {
        abstract_tree::get_vertex(index);
        abstract_game_object* ptr = dynamic_cast<abstract_game_object*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not an abstract_game_object");
        return *ptr;
    }

    abstract_game_object const& get_object(int index) const {
        abstract_tree::get_vertex(index);
        abstract_game_object* ptr = dynamic_cast<abstract_game_object*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not a abstract_game_object");
        return *ptr;
    }

    abstract_game_object& operator[](int index) override {
        return get_object(index);
    }

    abstract_game_object const& operator[](int index) const override {
        return get_object(index);
    }

    scene_tree() {
        __root = 0;
        add_root_object();
    }

    int next_index() {
        return next_vertex_index();
    }

    shared_ptr<vector<int>> const& next_adj() {
        return __adj[next_index()];
    }

    shared_ptr<int> const& parent_ptr(int index) {
        if (index < 0 || __parent.size() <= index)
            throw runtime_error("Index " + to_string(index) + " is out of bounds");
        return __parent[index];
    }

    //do not use directly, instantiate a game_object instead
    void add_object(shared_ptr<abstract_game_object> & v, int parent) {
        int index =  add_vertex(parent).index;
        __vertices[index] = v;
    }

    void remove_object(int index) {
        abstract_tree::remove_vertex(index);
    }

    virtual void update() {
        vector<int> to_update;

        auto ef = [&to_update, this](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            abstract_game_object obj = get_object(u->index);

            if (!obj.active()) return false;

            to_update.push_back(obj.index);
            return true;
        };
        auto lf = [](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {};
        dfs(0, ef, lf);

        for (int i : to_update)
            get_object(i).update();
    }

    virtual ~scene_tree() override {}
};

class root_object : virtual public game_object {
    public:

    //do not instantiate directly
    root_object(scene_tree* tree)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        game_object(tree) {}
};