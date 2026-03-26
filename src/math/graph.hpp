#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;
#define pii pair<int, int>

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
    //vertices and edges themselves, i < size() can be nullptr
    vector<shared_ptr<vertex>> __vertices;
    vector<shared_ptr<edge>> __edges;

    //list of used vertex indices
    vector<int> __used_vertices;
    //position of a vertex in __used_vertices
    vector<int> __used_vertices_index;
    //list of used edge indices
    vector<int> __used_edges;
    //position of an edge in __used_edges
    vector<int> __used_edges_index;

    //adjacent edges for each vertex
    vector<shared_ptr<vector<int>>> __adj;
    //holds the indices of an edge in __adj for both u and v
    vector<pii> __adj_index;

    //containers with all unused indices < size()
    queue<int> __free_vertices;
    queue<int> __free_edges;

    void check_vertex_index(int index) const {
        if (index < 0 || index >= size())
            throw runtime_error("Index " + to_string(index) + " is out of bounds");
        if (!__vertices[index])
            throw runtime_error("Vertex of index " + to_string(index) + " does not currently exist");
    }

    void check_edge_index(int index) const {
        if (index < 0 || index >= __edges.size())
            throw runtime_error("Index " + to_string(index) + " is out of bounds");
        if (!__edges[index])
            throw runtime_error("Edge of index " + to_string(index) + " does not currently exist");
    }

    public:
    //NOT the vertex count, i < size() may be unused
    virtual size_t size() const {
        return __vertices.size();
    }

    //Index must be used and in bounds, otherwise an error will be thrown
    virtual vertex& get_vertex(int index) {
        check_vertex_index(index);
        return *__vertices[index];
    }

    virtual vertex const& get_vertex(int index) const {
        check_vertex_index(index);
        return *__vertices[index];
    }

    //Index must be used and in bounds, otherwise an error will be thrown
    virtual edge& get_edge(int index) {
        check_edge_index(index);
        return *__edges[index];
    }

    virtual edge const& get_edge(int index) const {
        check_edge_index(index);
        return *__edges[index];
    }

    //Returns the index of the vertex at the other side of an edge
    static int other_vertex(edge const& e, int index) {
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

    virtual int next_vertex_index() {
        if (__free_vertices.empty()) {
            __free_vertices.push(size());
            __adj.push_back(make_shared<vector<int>>());
            __vertices.push_back(nullptr);
            __used_vertices_index.push_back(-1);
        }

        return __free_vertices.front();
    }

    virtual int next_edge_index() {
        if (__free_edges.empty()) {
            __free_edges.push(__edges.size());
            __edges.push_back(nullptr);
            __adj_index.push_back({-1, -1});
            __used_edges_index.push_back(-1);
        }

        return __free_edges.front();
    }

    //ALGORITHMS

    /*
    -dfs_enter should return whether the dfs should visit the given vertex
    -current_edge represents the last edge entered from the given vertex
    -newly visited nodes will always be in the ENTERING state
    -vertices/edges should not be removed during traversal
    -pointers to previous edges are null for the vertex

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

                shared_ptr<vertex> const& v_ptr = __vertices[index];
                shared_ptr<edge> const& e_ptr = last == -1 ? nullptr : __edges[last];

                //enter function
                bool enter = std::forward<dfs_enter>(enter_function)
                (v_ptr, e_ptr, state, current_edge);

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

                shared_ptr<vertex> const& v_ptr = __vertices[index];
                shared_ptr<edge> const& e_ptr = last == -1 ? nullptr : __edges[last];

                //leave function
                std::forward<dfs_leave>(leave_function)
                (v_ptr, e_ptr, state, current_edge);

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
        -pointers to previous edges are null for the vertex

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

            shared_ptr<vertex> const& v_ptr = __vertices[index];
            shared_ptr<edge> const& e_ptr = last == -1 ? nullptr : __edges[last];

            //enter function
            bool enter = forward<bfs_enter>(enter_function)
            (v_ptr, e_ptr, state);
            if (!enter) continue;

            for (int i : get_vertex(index).edges()) {
                int x = other_vertex(get_edge(i), index);

                if (state[x] != bfs_state::UNVISITED) continue;
                state[x] = bfs_state::VISITED;

                q.push({x, i});
            }
        }
    }

    virtual vector<int> dfs_order(int start) {
        vector<int> order;

        auto ef = [this, &order](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {
            order.push_back(v_ptr->index);
            return true;
        };

        auto lf = [](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<dfs_state> const& state,
        vector<int> const& d) {};

        dfs(start, ef, lf);

        return order;
    }

    virtual vector<int> bfs_order(int start) {
        vector<int> order;

        auto ef = [this, &order](shared_ptr<vertex> const& v_ptr,
        shared_ptr<edge> const& e_ptr, vector<bfs_state> const& state) {
            order.push_back(v_ptr->index);
            return true;
        };

        bfs(start, ef);

        return order;
    }

    protected:
    virtual void create_vertex(int index, shared_ptr<vertex> & ptr) = 0;

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) = 0;

    virtual void fix_adj(int v, int i) {
        vector<int>& a = *__adj[v];
        swap(a[i], a[a.size() - 1]);
        a.pop_back();

        if (i == a.size()) return;
        
        edge& k = get_edge(a[i]);
        if (v == k.u) __adj_index[a[i]].first = i;
        else __adj_index[a[i]].second = i;
    }

    //Adds new a new vertex, if an unused index is available it will be used
    virtual vertex& add_vertex() {
        int index = next_vertex_index();
        __free_vertices.pop();

        create_vertex(index, __vertices[index]);

        __used_vertices_index[index] = __used_vertices.size();
        __used_vertices.push_back(index);

        return get_vertex(index);
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
        check_edge_index(index);

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
        check_vertex_index(index);
        vector<int> adj = *__adj[index];

        for (int i : adj)
            remove_edge(i);
        
        shared_ptr<vector<int>> ptr = make_shared<vector<int>>();
        __adj[index].swap(ptr);
        __adj[index] = make_shared<vector<int>>();
        
        __free_vertices.push(index);
        __vertices[index] = nullptr;

        index = __used_vertices_index[index];
        vector_remove(__used_vertices, __used_vertices_index, index);
    }

    virtual ~abstract_graph() {}
};

class graph : virtual public abstract_graph {
    protected:
    virtual void create_vertex(int index, shared_ptr<vertex> & ptr) override {
        ptr = make_shared<vertex>(index, __adj[index]);
    }

    virtual void create_edge(int index, int u, int v, shared_ptr<edge> & ptr) override {
        ptr = make_shared<edge>(index, u, v);
    }

    public:
    //Indices are reusable
    graph(size_t const& size) {
        __vertices.reserve(size);
        for (int i = 0; i < size; i++)
            add_vertex();
    }

    //Indices are reusable
    graph() {
        graph(0);
    }

    virtual vertex& add_vertex() override {
        return abstract_graph::add_vertex();
    }

    virtual edge& add_edge(int u, int v) override {
        return abstract_graph::add_edge(u, v);
    }

    virtual void remove_edge(int index) override {
        abstract_graph::remove_edge(index);
    }

    virtual void remove_vertex(int index) override {
        abstract_graph::remove_vertex(index);
    }

    virtual ~graph() override {}
};