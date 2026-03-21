#pragma once
#include "math/tree_vertex.hpp"
#include "math/vector2.hpp"

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