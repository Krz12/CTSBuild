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

    virtual void update_enter() {}
    virtual void update_leave() {}
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
        vector<pair<int, bool>> to_update;

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

class transform_3d {
    protected:
    vector3<double> __position;
    vector3<double> __velocity;
    vector3<double> __acceleration;
    vector3<double> __rotation;
    vector3<double> __angular_velocity;
    vector3<double> __angular_acceleration;
    vector3<double> __scale;
    public:
    transform_3d(vector3<double> position) :
        __position(position),
        __velocity(vector3<double>::ZERO),
        __acceleration(vector3<double>::ZERO),
        __rotation(vector3<double>::ZERO),
        __angular_velocity(vector3<double>::ZERO),
        __angular_acceleration(vector3<double>::ZERO),
        __scale({1,1,1}) {}

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

    void scale(vector3<double> new_scale) {
        __scale = new_scale;
    }

    vector3<double> scale() const {
        return __scale;
    }

    transform_3d operator+(const transform_3d& other) const {
        transform_3d result(vector3<double>::ZERO);
        result.__position = this->__position + other.__position;
        result.__rotation = this->__rotation + other.__rotation;
        result.__scale = vector3<double>(
            this->__scale.x * other.__scale.x,
            this->__scale.y * other.__scale.y,
            this->__scale.z * other.__scale.z
        );
        result.__velocity = this->__velocity + other.__velocity;
        result.__acceleration = this->__acceleration + other.__acceleration;
        result.__angular_velocity = this->__angular_velocity + other.__angular_velocity; //nwm czy to dobrze
        result.__angular_acceleration = this->__angular_acceleration + other.__angular_acceleration;
        
        return result;
    }

};

class game_object_3d : virtual public game_object {
    protected:
    transform_3d __global_transform;
    transform_3d __transform;
    public:

    game_object_3d(scene_tree* tree, vector3<double> position)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        game_object(tree), __transform(position), __global_transform(vector3<double>::ZERO) {}

    static shared_ptr<game_object_3d> create(scene_tree & tree, abstract_game_object & parent,
    vector3<double> position, vector3<double> rotation) {
        shared_ptr<game_object_3d> ptr = make_shared<game_object_3d>(&tree, position, rotation);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent.index);
        return ptr;
    }

    const transform_3d& global_transform() const {
        return __global_transform;
    }
    transform_3d& transform() {
        return __transform;
    }

    void update() override {
        __transform.position(__transform.position()+ __transform.velocity() + 0.5 * __transform.acceleration());
        __transform.rotation(__transform.rotation() + __transform.angular_velocity() + 0.5 * __transform.angular_acceleration());
        __transform.velocity(__transform.velocity() + __transform.acceleration());
        __transform.angular_velocity(__transform.angular_velocity() + __transform.angular_acceleration());

        game_object& parent = parent_object();
        game_object_3d* parent_3d = dynamic_cast<game_object_3d*>(&parent); //slop żeby mieć parenta 3d (można dodac jeszcze z 2d ale po co)
        if(parent_3d) {
           __global_transform = parent_3d->global_transform() + __transform;
        } else {
            __global_transform = __transform;
        }

    }
};

class transform_2d {
protected:
    vector2<double> __position;
    double __rotation;
    vector2<double> __scale;
    
public:
    transform_2d(vector2<double> position = vector2<double>::ZERO) :
        __position(position),
        __rotation(0),
        __scale({1, 1}) {}
    
    void position(vector2<double> new_pos) { __position = new_pos; }
    vector2<double> position() const { return __position; }
    
    void rotation(double new_rotation) { __rotation = new_rotation; }
    double rotation() const { return __rotation; }
    
    void scale(vector2<double> new_scale) { __scale = new_scale; }
    vector2<double> scale() const { return __scale; }
    
    transform_2d operator+(const transform_2d& other) const {
        transform_2d result;
        result.__position = this->__position + other.__position;
        result.__rotation = this->__rotation + other.__rotation;
        result.__scale = vector2<double>(
            this->__scale.x * other.__scale.x,
            this->__scale.y * other.__scale.y
        );
        return result;
    }
};

class game_object_2d : virtual public game_object {
    protected:
    transform_2d __global_transform;
    transform_2d __transform;
    public:

    game_object_2d(scene_tree* tree, vector2<double> position)
        : vertex(tree->next_index(), tree->next_adj()),

        tree_vertex(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        abstract_game_object(tree->next_index(), tree->next_adj(),
        tree->parent_ptr(tree->next_index())),

        game_object(tree), __transform(position), __global_transform(vector2<double>::ZERO) {}

    static shared_ptr<game_object_2d> create(scene_tree & tree, abstract_game_object & parent,
    vector3<double> position, vector3<double> rotation) {
        shared_ptr<game_object_2d> ptr = make_shared<game_object_2d>(&tree, position, rotation);
        shared_ptr<abstract_game_object> abs_ptr = dynamic_pointer_cast<abstract_game_object>(ptr);
        tree.add_object(abs_ptr, parent.index);
        return ptr;
    }

    const transform_2d& global_transform() const {
        return __global_transform;
    }
    transform_2d& transform() {
        return __transform;
    }

    void update() override {
        game_object& parent = parent_object();
        game_object_2d* parent_2d = dynamic_cast<game_object_2d*>(&parent); //slop żeby mieć parenta 3d (można dodac jeszcze z 2d ale po co)
        if(parent_2d) {
           __global_transform = parent_2d->global_transform() + __transform;
        } else {
            __global_transform = __transform;
        }

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