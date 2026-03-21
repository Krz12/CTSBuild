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

class game_object_2d : abstract_game_object
{
private:
    
public:
    vector2<float> position,
    scale;
};