#pragma once

#include <cstddef>
#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <type_traits>
using namespace std;

#define pii pair<int, int>
#include <memory>
#include <vector>
using namespace std;

class vertex {
    public:
    const int index;

    protected:
    shared_ptr<vector<int>> __adj;

    public:
    virtual vector<int> const& edges() const {
        return *__adj;
    }

    vertex(int index, shared_ptr<vector<int>> const& adj)
        : index(index), __adj(adj) {}
    vertex() : index(-1), __adj(nullptr) {}
};

class edge {
    protected:
    //Intended to make the class castable
    virtual void nothing() {}

    public:
    const int index;
    const int u, v;

    edge(int index, int u, int v)
        : index(index), u(u), v(v) {}
};

enum class bfs_state : char {
    NONE = 0,
    UNVISITED = 1,
    VISITED = 2
};

enum class dfs_state : char {
    NONE = 0,
    UNVISITED = 1,
    ENTERING = 2,
    ENTERED = 3,
    LEAVING = 4,
    LEFT = 5
};

class abstract_graph {
    protected:
    vector<unique_ptr<vertex>> __vertices;
    vector<unique_ptr<edge>> __edges;

    vector<int> __used_vertices;
    vector<int> __used_vertices_index;
    vector<int> __used_edges;
    vector<int> __used_edges_index;

    vector<shared_ptr<vector<int>>> __adj;
    vector<pii> __adj_index;

    queue<int> __free_vertices;
    queue<int> __free_edges;

    virtual void create_vertex(int index, unique_ptr<vertex> & ptr);

    virtual void create_edge(int index, int u, int v, unique_ptr<edge> & ptr);

    virtual void fix_adj(int v, int i) {
        vector<int>& a = *__adj[v];
        swap(a[i], a[a.size() - 1]);
        a.pop_back();

        if (i == a.size()) return;
        
        edge& k = get_edge(a[i]);
        if (v == k.u) __adj_index[a[i]].first = i;
        else __adj_index[a[i]].second = i;
    }

    virtual int next_vertex_index() {
        if (__free_vertices.empty()) {
            __adj.push_back(nullptr);
            __vertices.push_back(nullptr);
            __free_vertices.push(size());
            __used_vertices_index.push_back(-1);
        }

        return __free_vertices.front();
    }

    virtual int next_edge_index() {
        if (__free_edges.empty()) {
            __edges.push_back(nullptr);
            __free_edges.push(__edges.size());
            __adj_index.push_back({-1, -1});
            __used_edges_index.push_back(-1);
        }

        return __free_edges.front();
    }

    /*
    Removes an element from a vector in O(1) time by swapping
    it with another element whose index is then updated
    */
    static void vector_remove(vector<int> & a, vector<int> & inverse, int i) {
        swap(a[i], a[a.size() - 1]);
        a.pop_back();

        if (i == a.size()) return;
        
        inverse[a[i]] = i;
    }

    //O(1) time
    virtual void remove_edge(int index) {
        edge& k = get_edge(index);
        fix_adj(k.u, __adj_index[k.u].first);
        fix_adj(k.v, __adj_index[k.u].second);
        __free_edges.push(index);
        __edges[index] = nullptr;

        index = __used_edges_index[index];
        vector_remove(__used_edges, __used_edges_index, index);
    }

    //O(1) time
    virtual void remove_vertex(int index) {
        vector<int> adj = *__adj[index];

        for (int i : adj)
            remove_edge(i);
        
        __free_vertices.push(index);
        __vertices[index] = nullptr;

        index = __used_vertices_index[index];
        vector_remove(__used_vertices, __used_vertices_index, index);
    }

    public:
    //NOT the vertex count, i < size() may be unused
    virtual size_t size() const {
        return __vertices.size();
    }

    //Index must be used and in bounds, otherwise an error will be thrown
    virtual vertex& get_vertex(int index) {
        if (index < 0 || index >= size())
            throw runtime_error("Index " + to_string(index) + " is out of bounds");

        if (!__vertices[index])
            throw runtime_error("Vertex of index " + to_string(index) + " does not currently exist");

        return *__vertices[index];
    }

    //Adds new a new vertex, if an unused index is available it will be used
    virtual vertex& add_vertex() {
        int index = next_vertex_index();
        __free_vertices.pop();

        __adj[index] = make_shared<vector<int>>();
        create_vertex(index, __vertices[index]);

        __used_vertices_index[index] = __used_vertices.size();
        __used_vertices.push_back(index);

        return get_vertex(index);
    }

    virtual vertex const& get_vertex(int index) const {
        return get_vertex(index);
    }

    //Index must be used and in bounds, otherwise an error will be thrown
    virtual edge& get_edge(int index) {
        if (index < 0 || index >= __edges.size())
            throw runtime_error("Index " + to_string(index) + " is out of bounds");

        if (!__edges[index])
            throw runtime_error("Edge of index " + to_string(index) + " does not currently exist");

        return *__edges[index];
    }

    virtual edge const& get_edge(int index) const {
        return get_edge(index);
    }

    //Adds new a new edge, if an unused index is available it will be used
    virtual edge& add_edge(int u, int v) {
        get_vertex(u);
        get_vertex(v);

        pii pos = {__adj[u]->size(), __adj[v]->size()};

        int index = next_edge_index();

        index = __free_edges.front();
        __free_edges.pop();

        create_edge(index, u, v, __edges[index]);
        __adj_index[index] = pos;

        __adj[u]->push_back(index);
        __adj[v]->push_back(index);

        __used_edges_index[index] = __used_edges.size();
        __used_edges.push_back(index);

        return get_edge(index);
    }

    //Returns the index of the vertex at the other side of an edge
    virtual int other_vertex(edge const& e, int index) const {
        if (e.u != index) return e.u;
        return e.v;
    }

    //A full list of used vertex indices, may not be ordered if anything was removed
    virtual vector<int> const& vertices() const {
        return __used_vertices;
    }

    //A full list of used edge indices, may not be ordered if any edges were removed
    virtual vector<int> const& edges() const {
        return __used_edges;
    }

    //Whether the index is tied to a valid vertex
    virtual bool has_vertex(int index) const {
        if (index < 0 || index >= size()) return false;
        if (!__vertices[index]) return false;
        return true;
    }

    //Whether the index is tied to a valid edge
    virtual bool has_edge(int index) const {
        if (index < 0 || index >= __edges.size()) return false;
        if (!__edges[index]) return false;
        return true;
    }

    //Alternative to get_vertex()
    virtual vertex& operator[](int index) {
        return get_vertex(index);
    }

    virtual vertex const& operator[](int index) const {
        return get_vertex(index);
    }

    /*
    Returns a new graph with the same structure and normalized indices.
    Original order is not necessarily restored.
    Time = O(size())
    */
    virtual abstract_graph normalize() const;

    //ALGORITHMS

    /*
    -dfs_enter should return whether the dfs should visit the given vertex
    -current_edge represents the last edge entered from the given vertex
    -newly visited nodes will always be in the ENTERING state
    -vertices/edges should not be removed during traversal

    memory = O(size()) = O(vertex_count) + O(__free_vertices)
    time = O(size() + edge_count)
    */
    template<typename dfs_enter, typename dfs_leave>
    void dfs(int start, dfs_enter&& enter_function, dfs_leave&& leave_function) const {
        vertex const& r = get_vertex(start);
        vector<dfs_state> state(size(), dfs_state::NONE);
        vector<int> current_edge(size(), -1);
        
        for (int i : vertices())
            state[i] = dfs_state::UNVISITED;

        //first = index, second = edge used to enter
        stack<pii> s;
        s.push({r.index, -1});

        while (!s.empty()) {
            int index = s.top().first;
            int last = s.top().second;
            s.pop();

            if (current_edge[index] == -1) {
                state[index] = dfs_state::ENTERING;

                //enter function
                bool enter = std::forward<dfs_enter>(enter_function)
                (index, last, state, current_edge);

                if (!enter) {
                    state[index] = dfs_state::LEFT;
                    continue;
                }

                state[index] = dfs_state::ENTERED;
            }

            current_edge[index]++;

            if (current_edge[index] == get_vertex(index).edges().size()) {
                current_edge[index] = -1;
                state[index] = dfs_state::LEAVING;

                //leave function
                std::forward<dfs_leave>(leave_function)
                (index, last, state, current_edge);

                state[index] = dfs_state::LEFT;
                continue;
            }
            
            s.push({index, last});
            int next = get_vertex(index).edges()[current_edge[index]];
            int x = other_vertex(get_edge(next), index);

            if (state[x] == dfs_state::UNVISITED)
                s.push({x, next});
        }
    }

    /*
        -bfs_enter should return whether the bfs should visit the given vertex
        -newly visited nodes will always be in the VISITED state
        -vertices/edges should not be removed during traversal

        memory = O(size()) = O(vertex_count) + O(__free_vertices)
        time = O(size() + edge_count)
    */
    template<typename bfs_enter>
    void bfs(int start, bfs_enter&& enter_function) const {
        vertex const& r = get_vertex(start);
        vector<bfs_state> state(size(), bfs_state::NONE);
        
        for (int i : vertices())
            state[i] = bfs_state::UNVISITED;

        //first = index, second = edge used to enter
        queue<pii> q;
        q.push({r.index, -1});
        state[r.index] = bfs_state::VISITED;

        while (!q.empty()) {
            int index = q.front().first;
            int last = q.front().second;
            q.pop();

            //enter function
            bool enter = forward<bfs_enter>(enter_function)(index, last, state);
            if (!enter) continue;

            for (int i : get_vertex(index).edges()) {
                int x = other_vertex(get_edge(i), index);

                if (state[x] != bfs_state::UNVISITED) continue;
                state[x] = bfs_state::VISITED;

                q.push({x, i});
            }
        }
    }
};