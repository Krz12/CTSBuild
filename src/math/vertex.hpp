#pragma once
#include <memory>
#include <stdexcept>
#include <vector>

using namespace std;

class vertex {
    public:
    const int index;

    protected:
    shared_ptr<vector<int>> __adj;

    public:
    inline virtual vector<int> const& edges() const {
        return *__adj;
    }

    vertex(int index, shared_ptr<vector<int>> const& adj)
        : index(index), __adj(adj) {}
};