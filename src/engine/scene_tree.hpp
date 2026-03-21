#pragma once

#include <cstddef>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include "math/graph.hpp"
#include "math/tree.hpp"
#include "math/vector3.hpp"
#include "math/vector2.hpp"
using namespace std;

class abstract_game_object : virtual public tree_vertex {
    bool __active = true;

    public:
    
    virtual bool active() {
        return __active;
    }

    virtual void active(bool b) {
        __active = b;
    }
    
    abstract_game_object(int const& index, shared_ptr<vector<int>> const& __adj,
    shared_ptr<int> const& parent)
        : vertex(index, __adj),
    tree_vertex(index, __adj, parent) {}

    virtual void update() {}
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
            return true;
        };

        auto lf = [&to_update](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            to_update.push_back(u->index);
        };

        dfs(0, ef, lf);

        for (int i : to_update)
            get_object(i).update();
    }

    virtual ~scene_tree() override {}
};

class game_object : virtual public abstract_game_object {
    protected:
    scene_tree & __tree;

    public:

    scene_tree& tree() {
        return __tree;
    }

    game_object& parent_object() {
        int other = __tree.other_vertex(__tree.get_edge(parent()), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree.get_object(other));

        if (!ptr)
            throw runtime_error("Parent " + to_string(index) + " is not a game_object");
        return *ptr;
    }

    game_object const& parent_object() const {
        int other = __tree.other_vertex(__tree.get_edge(parent()), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree.get_object(other));

        if (!ptr)
            throw runtime_error("Parent " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    game_object& child_object(int adj_index) {
        if (index < 0 || index >= edges().size())
            throw runtime_error("Index " + to_string(adj_index) + " is out of bounds");

        int other = __tree.other_vertex(__tree.get_edge((*__adj)[adj_index]), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree.get_object(other));

        if (!ptr)
            throw runtime_error("Child " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    game_object const& child_object(int adj_index) const {
        if (index < 0 || index >= edges().size())
            throw runtime_error("Index " + to_string(adj_index) + " is out of bounds");

        int other = __tree.other_vertex(__tree.get_edge((*__adj)[adj_index]), index);
        game_object* ptr = dynamic_cast<game_object*>(&__tree.get_object(other));

        if (!ptr)
            throw runtime_error("Child " + to_string(other) + " is not a game_object");
        return *ptr;
    }

    //Do not instantiate directly, use game_object::create() instead
    game_object(scene_tree* tree)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),
        
        __tree(*tree) {}

    static shared_ptr<game_object> create(scene_tree & tree, abstract_game_object & parent) {
        shared_ptr<game_object> ptr = make_shared<game_object>(&tree);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent.index);
        return ptr;
    }
};

class game_object_3d : virtual public game_object {
    protected:
    vector3<double> __position;
    vector3<double> __velocity;
    vector3<double> __acceleration;
    vector3<double> __rotation;
    vector3<double> __angular_velocity;
    vector3<double> __angular_acceleration;
    vector3<double> __scale;

    public:

    game_object_3d(scene_tree* tree, vector3<double> position, vector3<double> rotation)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        game_object(tree), __position(position), __rotation(rotation), 
        __velocity(vector3<double>::ZERO), __angular_velocity(vector3<double>::ZERO),
        __acceleration(vector3<double>::ZERO), __angular_acceleration(vector3<double>::ZERO), __scale({1,1,1}) {}

    static shared_ptr<game_object_3d> create(scene_tree & tree, abstract_game_object & parent,
    vector3<double> position, vector3<double> rotation) {
        shared_ptr<game_object_3d> ptr = make_shared<game_object_3d>(&tree, position, rotation);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent.index);
        return ptr;
    }

    void position(vector3<double> new_pos) {
        __position = new_pos;
    }

    vector3<double> position() const {
        return __position;
    }

    void velocity(vector3<double> new_vel) {
        __velocity = new_vel;
    }

    vector3<double> velocity() const {
        return __velocity;
    }

    void acceleration(vector3<double> new_acc) {
        __acceleration = new_acc;
    }

    vector3<double> acceleration() const {
        return __acceleration;
    }

    void rotation(vector3<double> new_rot) {
        __rotation = new_rot;
    }

    vector3<double> rotation() const {
        return __rotation;
    }

    void angular_velocity(vector3<double> new_ang_vel) {
        __angular_velocity = new_ang_vel;
    }

    vector3<double> angular_velocity() const {
        return __angular_velocity;
    }

    void angular_acceleration(vector3<double> new_ang_acc) {
        __angular_acceleration = new_ang_acc;
    }

    vector3<double> angular_acceleration() const {
        return __angular_acceleration;
    }


    void update() override {
        __position += __velocity + 0.5 * __acceleration;
        __rotation += __angular_velocity + 0.5 * __angular_acceleration;
        __velocity += __acceleration;
        __angular_velocity += __angular_acceleration;
    }
};

class game_object_2d : virtual public game_object {
    protected:
    vector2<double> __position;
    vector2<double> __rotation;
    vector2<double> __scale;

    public:

    game_object_2d(scene_tree* tree, vector2<double> position, vector2<double> rotation)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        game_object(tree), __position(position), __rotation(rotation), __scale({1, 1}) {}

    static shared_ptr<game_object_2d> create(scene_tree & tree, abstract_game_object & parent,
    vector2<double> position, vector2<double> rotation) {
        shared_ptr<game_object_2d> ptr = make_shared<game_object_2d>(&tree, position, rotation);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent.index);
        return ptr;
    }

    void position(vector2<double> new_pos) {
        __position = new_pos;
    }

    vector2<double> position() const {
        return __position;
    }

    void rotation(vector2<double> new_rot) {
        __rotation = new_rot;
    }

    vector2<double> rotation() const {
        return __rotation;
    }

    void update() override {

    }
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