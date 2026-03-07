#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "graph.hpp"
using namespace std;

class tree : virtual public abstract_graph {
    public:
    
    class tree_vertex : virtual public vertex {
        protected:
        shared_ptr<int> __parent; //index of edge, not vertex

        public:
        virtual int parent() {
            return *__parent;
        }

        tree_vertex(int const& index, shared_ptr<vector<int>> __adj, shared_ptr<int> const& parent)
            : vertex(index, __adj), __parent(parent) {}
    };

    protected:
    vector<shared_ptr<int>> __parent;
    int __root;

    virtual void create_vertex(int index, shared_ptr<vertex> & ptr) override {
        ptr = make_shared<tree_vertex>(index, __adj[index], __parent[index]);
    }

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) override {
        ptr = make_shared<edge>(index, u, v);
    }

    virtual void add_edgeless_vertex() {
        int index = next_vertex_index();
        while (__parent.size() <= index)
            __parent.push_back(nullptr);

        __parent[index] = nullptr;
        abstract_graph::add_vertex();
    }

    public:
    virtual tree_vertex& get_vertex(int index) override {
        abstract_graph::get_vertex(index);
        tree_vertex* ptr = dynamic_cast<tree_vertex*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not a tree_vertex");
        return *ptr;
    }
    
    virtual tree_vertex& operator[](int index) override {
        return get_vertex(index);
    }

    virtual tree_vertex const& get_vertex(int index) const override {
        abstract_graph::get_vertex(index);
        tree_vertex* ptr = dynamic_cast<tree_vertex*>(__vertices[index].get());
        if (!ptr)
            throw runtime_error("Vertex " + to_string(index) + " is not a tree_vertex");
        return *ptr;
    }
    
    virtual tree_vertex const& operator[](int index) const override {
        return get_vertex(index);
    }

    virtual tree_vertex& add_vertex(int parent) {
        get_vertex(parent);
        if (!has_vertex(parent))
            throw runtime_error("Parent does not exist");

        int index = next_vertex_index();
        while (__parent.size() <= index)
            __parent.push_back(nullptr);

        __parent[index] = make_shared<int>(0);
        abstract_graph::add_vertex();

        edge& e = add_edge(parent, index);
        *__parent[index] = e.index;

        return get_vertex(index);
    }

    /*
        -g is the reference graph the tree will be built on,
        it must be normalized, have exactly one component, and be acyclic
    */
    tree(abstract_graph const& g, int const& root) : __root(root) {
        if (g.vertices().size() != g.size())
            throw runtime_error("Graph is not normalized");

        for (int i = 0; i < size(); i++)
            if (g.vertices()[i] >= size())
                throw runtime_error("Graph is not normalized");

        if (g.vertices().size() != g.edges().size() - 1)
            throw runtime_error("Graph is not structured like a tree");
        
        int count = 0;
        
        auto count_f = [&count](int index, int last,
        vector<bfs_state> const& state) {
            ++count;
            return true;
        };

        g.bfs(g.vertices()[0], count_f);

        if (count != g.vertices().size())
            throw runtime_error("Graph is not structured like a tree");

        for (int i = 0; i < g.vertices().size(); i++)
            add_edgeless_vertex();
        
        auto set_parent = [this](int index, int last,
        vector<dfs_state> const& state, vector<int> const& d) {
            edge& e = add_edge(last, index);
            *__parent[index] = e.index;
            return true;
        };

        auto nothing = [](int a, int b,
        vector<dfs_state> const& c, vector<int> const& d) {};

        g.dfs(root, set_parent, nothing);
    }

    //Root = 0
    tree() : __root(0) {
        add_edgeless_vertex();
    }

    virtual void set_parent(int index, int parent) {
        get_vertex(parent);
        if (!has_edge(parent))
            throw runtime_error("Parent does not exist");

        remove_edge(get_vertex(index).parent());
        edge& e = add_edge(parent, index);
        *__parent[index] = e.index;
    }

    //Removes all descendants as well
    virtual void remove_vertex(int index) override {
        vector<int> indices, edge_i;
        int parent = get_vertex(index).parent();

        auto collect = [&parent, &indices, this](int i, int last,
        vector<bfs_state> const& state) {
            if (last == parent) return false;
            indices.push_back(i);
            return true;
        };

        bfs(index, collect);

        for (int i : indices)
            remove_edge(get_vertex(i).parent());

        for (int i : indices)
            abstract_graph::remove_vertex(i);
    }

    virtual ~tree() override {}
};