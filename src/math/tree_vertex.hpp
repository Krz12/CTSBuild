#pragma once
#include <memory>
#include "math/vertex.hpp"

class tree_vertex : virtual public vertex {
    protected:
    const shared_ptr<int> __parent; //index of edge, not vertex

    public:
    inline virtual int parent() const {
        return *__parent;
    }

    tree_vertex(int const& index, shared_ptr<vector<int>> const& __adj, shared_ptr<int> const& parent)
        : vertex(index, __adj), __parent(parent) {}
};