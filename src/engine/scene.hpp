#pragma once
#include <glad/glad.h>
#include <cstddef>
#include <future>
#include <memory>
#include <stdexcept>
#include <string>
#include "math/graph.hpp"
#include "math/vector3.hpp"
#include "math/graph.hpp"
#include "math/tree.hpp"
#include "unordered_set"
#include "unordered_map"
using namespace std;

struct entity_id {
    entity_id(int set_idx) : index(set_idx) {}
    uint32_t index;
    uint32_t generation;
};

template<typename T>
class component_container {
public:
    int add(T component, entity_id entity, tree &scene_tree) {
        int depth; //ustaw na głębokość entity w drzewie
        if (depth >= data.size()) data.resize(depth + 1);
        
        int index = data[depth].size();
        data[depth].push_back(std::move(component));
        
        entity_to_depth[entity] = depth;
        entity_to_index[entity] = index;
        if (index_to_entity.size() <= depth) index_to_entity.resize(depth + 1);
        if (index_to_entity[depth].size() <= index) index_to_entity[depth].resize(index + 1);
        index_to_entity[depth][index] = entity;
        
        return index;
    }
    
    void remove(entity_id entity) {
        auto itDepth = entity_to_depth.find(entity);
        if (itDepth == entity_to_depth.end()) return;
        int depth = itDepth->second;
        auto itIdx = entity_to_index.find(entity);
        int idx = itIdx->second;
        
        auto& vec = data[depth];
        int lastIdx = static_cast<int>(vec.size() - 1);
        
        if (idx != lastIdx) {
            //przepisujemy ostatni element na puste miejsce
            vec[idx] = move(vec[lastIdx]);
            entity_id lastEntity = index_to_entity[depth][lastIdx];
            entity_to_depth[lastEntity] = depth;
            entity_to_index[lastEntity] = idx;
            index_to_entity[depth][idx] = lastEntity;
        }
        
        vec.pop_back();
        index_to_entity[depth].pop_back();
        
        entity_to_depth.erase(entity);
        entity_to_index.erase(entity);
    }
    
    T* get(entity_id entity) {
        auto itDepth = entity_to_depth.find(entity);
        if (itDepth == entity_to_depth.end()) return nullptr;
        int depth = itDepth->second;
        auto itIdx = entity_to_index.find(entity);
        int idx = itIdx->second;
        if (depth >= data.size() || idx >= data[depth].size()) return nullptr;
        return &data[depth][idx];
    }

    int max_depth() const {
        return data.size()-1;
    }
    
    void for_each_on_depth(int depth, function<void(T&, entity_id)> func) {
        if (depth >= data.size()) return;
        auto& vec = data[depth];
        for (int i = 0; i < vec.size(); ++i) {
            func(vec[i], index_to_entity[depth][i]);
        }
    }
    
private:
    vector<vector<T>> data;
    unordered_map<entity_id, int> entity_to_depth;
    unordered_map<entity_id, uint32_t> entity_to_index;
    vector<vector<entity_id>> index_to_entity;
};

class scene {
    protected:
    shared_ptr<tree> __tree = make_shared<tree>();
    vector<entity_id> __entities;
    virtual void render_2d(const vector<int> &to_render) {
        glDisable(GL_DEPTH_TEST);
        
    }

    virtual void render_3d(const vector<int> &to_render) {
        glEnable(GL_DEPTH_TEST);
        
    }

    public:
    scene() {}
    tree &get_tree() {
        return *__tree;
    }

    entity_id create_entity(int parent = 0) {
        __entities.push_back(entity_id(__tree->add_vertex(parent).index));
        return __entities.back();
    }

    void remove_entity(entity_id id) {
        //trzeba usunąć dzieci
        vector<int> to_remove;
        
        auto ef = [&to_remove, this](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            return true;
        };

        auto lf = [&to_remove](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            to_remove.push_back(u->index);
        };

        __tree->dfs(0, ef, lf);
    }

    virtual void update() {
        
    }

    virtual void render() {
        //Zrób kolejka renderowania
        vector<int> to_render;

        auto ef = [&to_render, this](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            //abstract_game_object obj = __tree->get_object(u->index);
            if (!obj.active()) return false;
            return true;
        };

        auto lf = [&to_render](shared_ptr<vertex> const& u,
        shared_ptr<edge> const& last, vector<dfs_state> const& state,
        vector<int> const& current_edge) {
            to_render.push_back(u->index);
        };

        __tree->dfs(0, ef, lf);

        render_3d(to_render);
        render_2d(to_render);
    }

    virtual ~scene() {}
};