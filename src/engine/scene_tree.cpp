#include "engine/scene_tree.hpp"

void scene_tree::add_root_object() {
    if (size() != 0)
        throw runtime_error("Root object already exists");
    
    shared_ptr<root_object> ptr = make_shared<root_object>(this);
    int index = add_edgeless_vertex().index;
    
    __vertices[index] = ptr;
}