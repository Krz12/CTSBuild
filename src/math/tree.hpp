#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include "graph.hpp"
#include <iostream>
using namespace std;

class tree_vertex : virtual public vertex {
    protected:
    const shared_ptr<int> __parent_vertex;
    const shared_ptr<int> __parent_edge;
    const shared_ptr<int> __depth;

    public:
    inline virtual int parent() const {
        return *__parent_vertex;
    }

    inline virtual int parent_edge() const {
        return *__parent_edge;
    }

    inline virtual int depth() const {
        return *__depth;
    }

    tree_vertex(int const& index, shared_ptr<vector<int>> const& __adj,
        shared_ptr<int> const& parent_edge,
        shared_ptr<int> const& parent_vertex,
        shared_ptr<int> depth)
        : vertex(index, __adj),
        __parent_edge(parent_edge),
        __parent_vertex(parent_vertex),
        __depth(depth) {}
};

class abstract_tree : virtual public abstract_graph {
    protected:
    vector<shared_ptr<int>> __parent_edge, __parent_vertex, __depth;
    int __root;

    virtual void create_vertex(int index, shared_ptr<vertex> & ptr) override {
        ptr = make_shared<tree_vertex>(index,
            __adj[index],
            __parent_edge[index],
            __parent_vertex[index],
            __depth[index]);
    }

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) override {
        ptr = make_shared<edge>(index, u, v);
    }

    virtual int next_vertex_index() override {
        int index = abstract_graph::next_vertex_index();

        while (__parent_edge.size() <= index)
            __parent_edge.push_back(make_shared<int>(-1));
        while (__parent_vertex.size() <= index)
            __parent_vertex.push_back(make_shared<int>(-1));
        while (__depth.size() <= index)
            __depth.push_back(make_shared<int>(-1));

        return index;
    }

    virtual tree_vertex& add_edgeless_vertex() {
        int index = abstract_graph::add_vertex().index;
        *__depth[index] = 0;
        return get_vertex(index);
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

    virtual vector<int> get_descendants(int index) {
        int parent = get_vertex(index).parent();
        vector<int> descendants;
        
        auto ef = [this, &descendants, &parent](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {
            if (v_ptr->index == parent) return false;
            descendants.push_back(v_ptr->index);
            return true;
        };

        auto lf = [](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {};

        dfs(index, ef, lf);

        return descendants;
    }

    virtual void sort_edges(int index, vector<int> const& permutation) override {
        abstract_graph::sort_edges(index, permutation);
    }

    protected:
    virtual void set_parent(int index, int parent) {
        get_vertex(parent);

        int parent_e = get_vertex(index).parent_edge();
        remove_edge(parent_e);

        *__parent_edge[index] = add_edge(parent, index).index;
        *__parent_vertex[index] = parent;
        vector<int> indices = get_descendants(index);

        for (int i : indices) {
            int p = get_vertex(i).parent_edge();
            p = other_vertex(get_edge(p), i);
            *__depth[i] = *__depth[p] + 1;
        }
    }

    //Removes all descendants as well, set new parent first if not intended
    virtual void remove_vertex(int index) override {
        int parent = get_vertex(index).parent_edge();
        remove_edge(parent);

        vector<int> indices = get_descendants(index);

        for (int i : indices) {
            tree_vertex const& u = get_vertex(i);
            if (u.parent_edge() == -1) continue;
            remove_edge(u.parent_edge());
        }

        for (int i : indices)
            abstract_graph::remove_vertex(i);
    }

    virtual tree_vertex& add_vertex(int parent) {
        get_vertex(parent);
        if (!has_vertex(parent))
            throw runtime_error("Parent does not exist");

        int index = next_vertex_index();
        abstract_graph::add_vertex();

        edge& e = add_edge(parent, index);
        *__parent_edge[index] = e.index;
        *__parent_vertex[index] = parent;
        *__depth[index] = *__depth[parent] + 1;

        return get_vertex(index);
    }

    public:

    virtual ~abstract_tree() override {}
};

class tree : virtual public abstract_tree {
    public:
    /*
        -g is the reference graph the tree will be built on,
        it must be normalized, have exactly one component, and be acyclic
    */
    tree(abstract_graph const& g, int const& root) {
        __root = root;
        if (g.vertices().size() != g.size())
            throw runtime_error("Graph is not normalized");

        for (int i = 0; i < g.size(); i++)
            if (g.vertices()[i] >= g.size())
                throw runtime_error("Graph is not normalized");

        if (g.vertices().size() != g.edges().size() + 1)
            throw runtime_error("Graph is not structured like a tree");

        int count = 0;
        
        auto count_f = [&count]
        (shared_ptr<vertex> const& v_ptr, shared_ptr<edge> const& e_ptr,
        vector<bfs_state> const& state) {
            ++count;
            return true;
        };

        g.bfs(g.vertices()[0], count_f);

        if (count != g.vertices().size())
            throw runtime_error("Graph is not structured like a tree");

        for (int i = 0; i < g.vertices().size(); i++)
            add_edgeless_vertex();

        auto set_parent = [this](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {
            if (!e_ptr) return true;

            edge& e = add_edge(other_vertex(*e_ptr, v_ptr->index), v_ptr->index);
            *__parent_edge[v_ptr->index] = e.index;

            int pd = get_vertex(other_vertex(*e_ptr, v_ptr->index)).depth();
            *__depth[v_ptr->index] = pd;

            return true;
        };

        auto nothing = [](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {};

        g.dfs(root, set_parent, nothing);
    }

    //Root = 0
    tree() {
        __root = 0;
        add_edgeless_vertex();
    }

    tree_vertex& add_vertex(int parent) override {
        return abstract_tree::add_vertex(parent);
    }

    void set_parent(int index, int parent) override {
        abstract_tree::set_parent(index, parent);
    }

    void remove_vertex(int index) override {
        abstract_tree::remove_vertex(index);
    }

    virtual ~tree() override {}
};