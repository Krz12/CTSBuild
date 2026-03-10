#pragma once

#include <cstddef>
#include <future>
#include <memory>
#include <stdexcept>
#include "tree.hpp"
using namespace std;

class abstract_game_object : virtual public tree_vertex {
    public:
    abstract_game_object(int const& index, shared_ptr<vector<int>> const& __adj,
    shared_ptr<int> const& parent)
        : tree_vertex(index, __adj, parent) {}
    abstract_game_object() : tree_vertex(-1, nullptr, nullptr) {}

    abstract_game_object& parent_object();
};

class scene_tree : virtual public abstract_tree {
    protected:

    //not recommended for use, instantiate game_object or its derived classes instead
    void create_vertex(int index, shared_ptr<abstract_game_object> & ptr) {
        ptr = make_shared<abstract_game_object>(index, __adj[index], __parent[index]);
    }

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) override {
        ptr = make_shared<edge>(index, u, v);
    }

    abstract_game_object& get_vertex(int index) override {
        abstract_tree::get_vertex(index);
        abstract_game_object* ptr = dynamic_cast<abstract_game_object*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not an abstract_game_object");
        return *ptr;
    }

    abstract_game_object const& get_vertex(int index) const override {
        abstract_tree::get_vertex(index);
        abstract_game_object* ptr = dynamic_cast<abstract_game_object*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not a abstract_game_object");
        return *ptr;
    }

    public:
    void add_root_object();

    abstract_game_object& get_object(int index) {
        return get_vertex(index);
    }

    abstract_game_object const& get_object(int index) const {
        return get_vertex(index);
    }

    abstract_game_object& operator[](int index) override {
        return get_vertex(index);
    }

    abstract_game_object const& operator[](int index) const override {
        return get_vertex(index);
    }

    scene_tree() {
        __root = 0;
        add_root_object();
    }

    int next_index() {
        return next_vertex_index();
    }

    shared_ptr<vector<int>> const& next_adj() {
        return __adj[next_vertex_index()];
    }

    shared_ptr<int> const& parent_ptr(int index) {
        get_vertex(index);
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

    virtual ~scene_tree() override {}
};

class game_object : virtual public abstract_game_object {
    protected:
    shared_ptr<scene_tree> __tree;
    bool __active = true;

    public:

    scene_tree& tree() {
        return *__tree;
    }

    game_object& parent_object() {
        int other = __tree->other_vertex(__tree->get_edge(parent()), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree->get_object(other));

        if (!ptr)
            throw runtime_error("Parent " + to_string(index) + " is not a game_object");
        return *ptr;
    }

    game_object const& parent_object() const {
        int other = __tree->other_vertex(__tree->get_edge(parent()), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree->get_object(other));

        if (!ptr)
            throw runtime_error("Parent " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    game_object& child_object(int adj_index) {
        if (index < 0 || index >= edges().size())
            throw runtime_error("Index " + to_string(adj_index) + " is out of bounds");

        int other = __tree->other_vertex(__tree->get_edge((*__adj)[adj_index]), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree->get_object(other));

        if (!ptr)
            throw runtime_error("Child " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    game_object const& child_object(int adj_index) const {
        if (index < 0 || index >= edges().size())
            throw runtime_error("Index " + to_string(adj_index) + " is out of bounds");

        int other = __tree->other_vertex(__tree->get_edge((*__adj)[adj_index]), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree->get_object(other));

        if (!ptr)
            throw runtime_error("Child " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    bool active() {
        return __active;
    }

    //Do not instantiate directly, use game_object::create() instead
    game_object(scene_tree* tree)
        : abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())) {}
    game_object() : game_object(nullptr) {}

    static shared_ptr<game_object> create(scene_tree & tree, int parent) {
        shared_ptr<game_object> ptr = make_shared<game_object>(&tree);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent);
        return ptr;
    }
};

class root_object : virtual public game_object {
    public:

    //do not instantiate directly
    root_object(scene_tree* tree)
    : abstract_game_object(tree->next_index(), tree->next_adj(),
    tree->parent_ptr(tree->next_index())) {}
};

void scene_tree::add_root_object() {
    if (size() != 0)
        throw runtime_error("Root object already exists");
    
    shared_ptr<root_object> ptr = make_shared<root_object>(this);
    int index = add_edgeless_vertex().index;
    __vertices[index] = ptr;
}