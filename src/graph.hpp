#pragma once

#include "abstract_graph.hpp"
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

class graph : virtual public abstract_graph {
    protected:
    virtual void create_vertex(int index, unique_ptr<vertex> & ptr) override {
        ptr = make_unique<vertex>(index, __adj[index]);
    }

    virtual void create_edge(int index, int u, int v, unique_ptr<edge> & ptr) override {
        ptr = make_unique<edge>(index, u, v);
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

    void remove_edge(int index) override {
        abstract_graph::remove_edge(index);
    }

    void remove_vertex(int index) override {
        abstract_graph::remove_vertex(index);
    }
};