#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

#define pii pair<int, int>

class graph {
    public:
    class edge {
        public:
        const int index;
        const int u, v;

        edge(int const& index, int const& u, int const& v)
            : index(index), u(u), v(v) {}
    };

    class vertex {
        public:
        const int index;

        protected:
        shared_ptr<vector<int>> __adj;

        public:
        vector<int> const& edges() const {
            return *__adj;
        }

        vertex(int const& index, shared_ptr<vector<int>> const& adj)
            : index(index), __adj(adj) {}
    };

    protected:
    vector<unique_ptr<vertex>> __vertices;
    vector<unique_ptr<edge>> __edges;
    vector<shared_ptr<vector<int>>> __adj;
    vector<pii> __adj_index;
    queue<int> __free_vertices;
    queue<int> __free_edges;

    void create_vertex(int const& index, unique_ptr<vertex> & ptr) {
        ptr = make_unique<vertex>(index, __adj[index]);
    }

    void create_edge(int const& index, int const& u, int const& v, unique_ptr<edge> & ptr) {
        ptr = make_unique<edge>(index, u, v);
    }

    public:
    //CONSTRUCTOR AND MODIFYING FUNCTIONS

    //NOT the vertex count, i < size() may be unused
    size_t size() const {
        return __vertices.size();
    }

    vertex& get_vertex(int const& index) {
        if (index < 0 || index >= size())
            throw runtime_error("Index " + to_string(index) + "is out of bounds");

        if (!__vertices[index])
            throw runtime_error("Vertex of index " + to_string(index) + " does not currently exist");

        return *__vertices[index];
    }

    vertex& add_vertex() {
        int index = 0;

        if (__free_vertices.empty()) {
            index = size();

            __adj.push_back(nullptr);
            __vertices.push_back(nullptr);
            __free_vertices.push(index);
        }

        index = __free_vertices.front();
        __free_vertices.pop();

        __adj[index] = make_shared<vector<int>>();
        create_vertex(index, __vertices[index]);

        return get_vertex(index);
    }

    edge& get_edge(int const& index) {
        if (index < 0 || index >= __edges.size())
            throw runtime_error("Index " + to_string(index) + "is out of bounds");

        if (!__edges[index])
            throw runtime_error("Edge of index " + to_string(index) + " does not currently exist");

        return *__edges[index];
    }

    edge& add_edge(int const& u, int const& v) {
        get_vertex(u);
        get_vertex(v);

        pii pos = {__adj[u]->size(), __adj[v]->size()};
        int index = 0;

        if (__free_edges.empty()) {
            index = __edges.size();

            __edges.push_back(nullptr);
            __free_edges.push(index);
            __adj_index.push_back({-1, -1});
        }

        index = __free_edges.front();
        __free_edges.pop();

        create_edge(index, u, v, __edges[index]);
        __adj_index[index] = pos;

        __adj[u]->push_back(index);
        __adj[v]->push_back(index);

        return get_edge(index);
    }

    //CONSTRUCTOR
    graph(size_t const& size) {
        __vertices.reserve(size);
        for (int i = 0; i < size; i++)
            add_vertex();
    }

    bool has_vertex(int const& index) {
        if (index < 0 || index >= size()) return false;
        if (!__vertices[index]) return false;
        return true;
    }

    bool has_edge(int const& index) {
        if (index < 0 || index >= __edges.size()) return false;
        if (!__edges[index]) return false;
        return true;
    }

    //Alternative to get_vertex()
    vertex& operator[](int const& index) {
        return get_vertex(index);
    }

    protected:
    void fix_adj(int const& v, int const& i) {
        vector<int>& a = *__adj[v];
        swap(a[i], a[a.size() - 1]);
        a.pop_back();

        if (i == a.size()) return;
        
        edge& k = get_edge(a[i]);
        if (v == k.u) __adj_index[a[i]].first = i;
        else __adj_index[a[i]].second = i;
    }

    public:
    void remove_edge(int index) {
        edge& k = get_edge(index);
        fix_adj(k.u, __adj_index[k.u].first);
        fix_adj(k.v, __adj_index[k.u].second);
        __free_edges.push(index);
        __edges[index] = nullptr;
    }

    void remove_vertex(int index) {
        vector<int> adj = *__adj[index];
        
        __free_vertices.push(index);
        __vertices[index] = nullptr;
    }

    //ALGORITHMS
    private:

    public:
    enum class dfs_state : char {
        NONE = 0,
        UNVISITED = 1,
        ENTERING = 2,
        ENTERED = 3,
        LEAVING = 4,
        VISITED = 5
    };

    //memory = O(size()) = O(vertex_cout) + O(__free_vertices), using it anyway
    void dfs(int start) {
        vertex& s = get_vertex(start);
        vector<dfs_state> vertex_state(size(), dfs_state::NONE);
        
    }
};