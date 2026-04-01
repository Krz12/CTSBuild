#pragma once
#include <glad/glad.h>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "math/graph.hpp"
#include "math/vector3.hpp"
#include "math/tree.hpp"
#include "engine/entity.hpp"
#include <typeindex>

namespace std {
    template<>
    struct hash<entity_id> {
        size_t operator()(const entity_id& eid) const noexcept {
            return ((size_t)eid.index << 32) | eid.generation;
        }
    };
}

class component_container_interface {
    public:
    virtual ~component_container_interface() = default;
    virtual void add_component(const entity_id &eid) = 0;
    virtual void remove_component(const entity_id &eid) = 0;
    virtual void* get_component(const entity_id &eid) = 0;
    virtual void update_entity_depth(const entity_id &eid) = 0;
    virtual const bool has_component(const entity_id &eid) = 0;
    
};

template <typename T>
class component_container : public component_container_interface {
    private:
    unordered_map<entity_id, int> entity_component_index;
    unordered_map<entity_id, int> entity_depth;
    vector<vector<T>> components;
    vector<vector<entity_id>> entities; //depth,index

    tree &scene_tree;
    public:
    component_container(tree &set_tree) : scene_tree(set_tree) {}
    
    void add_component(const entity_id &eid) override {
        if(has_component(eid)) return;
        int depth = scene_tree.get_vertex(eid.index).depth();
        cerr << "depth: " << depth << "\n";
        if(depth >= components.size()) {
            components.resize(depth+1);
            entities.resize(depth+1);
        }
        components[depth].push_back(T());
        entities[depth].push_back(eid);
        entity_component_index[eid] = (int)components[depth].size()-1;
        entity_depth[eid] = depth;
    }

    void remove_component(const entity_id &eid) override {
        if(!has_component(eid)) return;
        int depth = entity_depth[eid];
        int idx = entity_component_index[eid];
        if(idx != components[depth].size()-1) {
            swap(components[depth][idx], components[depth][components[depth].size()-1]);
            swap(entities[depth][idx], entities[depth][entities[depth].size()-1]);
            entity_component_index[entities[depth][idx]] = idx;
        }
        components[depth].pop_back();
        entities[depth].pop_back();
        entity_component_index.erase(eid);
        entity_depth.erase(eid);
    }

    void update_entity_depth(const entity_id &eid) override {
        if(!has_component(eid)) return;
        int new_depth = scene_tree.get_vertex(eid.index).depth();
        if(entity_depth[eid] == new_depth) return;
        T temp = move(components[entity_depth[eid]][entity_component_index[eid]]);
        remove_component(eid);
        entity_depth[eid] = new_depth;
        add_component(eid);
        components[new_depth][entity_component_index[eid]] = move(temp);
    }

    void* get_component(const entity_id &eid) override {
        if (!has_component(eid)) return nullptr;
        return static_cast<void*>(&components[entity_depth[eid]][entity_component_index[eid]]);
    }

    const bool has_component(const entity_id &eid) override {
        return (entity_component_index.find(eid) == entity_component_index.end())? 0 : 1;
    }
    
    void for_each_on_depth(int depth, function<void(T&, entity_id)> func) {
        if (depth >= (int)components.size()) return;
        auto& vec = components[depth];
        auto& ent = entities[depth];
        for (size_t i = 0; i < vec.size(); ++i) {
            func(vec[i], ent[i]);
        }
    }

    void for_each_depth_top_down(function<void(int depth)> func) {
        for(size_t d = 0; d < components.size(); d++) {
            func(d);
        }
    }

    void for_each_depth_down_top(function<void(T&, entity_id, int depth)> func) {
        for(size_t d = components.size()-1; d > 0; --d) {
            func(d);
        }
    }

};

class component_manager {
    private:
    tree &__scene_tree;
    unordered_map<type_index, unique_ptr<component_container_interface>> containers;
    template <typename T>
    component_container<T>& get_container() {
        auto it = containers.find(type_index(typeid(T)));
        if (it == containers.end()) {
            auto new_container = make_unique<component_container<T>>(__scene_tree);
            auto& ref = *new_container;
            containers[type_index(typeid(T))] = move(new_container);
            return ref;
        }
        return static_cast<component_container<T>&>(*it->second);
    }

    public:
    component_manager(tree &set_tree) : __scene_tree(set_tree) {}

    const tree &scene_tree() {
        return __scene_tree;
    }
    
    template <typename T>
    void add_component(const entity_id &eid) {
        get_container<T>().add_component(eid);
    }

    template <typename T>
    void remove_component(const entity_id &eid) {
        get_container<T>().remove_component(eid);
    }

    template <typename T>
    T& get_component(const entity_id &eid) {
        auto* ptr = static_cast<T*>(get_container<T>().get_component(eid));
        if(!ptr) throw("Tried to get component which entity doesn't have");
        return *ptr;
    }

    template <typename T>
    const bool has_component(const entity_id &eid) {
        return get_container<T>().has_component(eid);
    }

    //będzie trzeba wywołać po zmianie parenta
    void update_entity_depth(const entity_id &eid) {
        //zrobimy dla każdego komponentu, można ewentualnie dla każdego trzymać liste komponentów
        for(auto& p : containers) {
            p.second.get()->update_entity_depth(eid);
        }
    }

    void remove_all_components(const entity_id &eid) {
        for(auto& p : containers) {
            p.second.get()->remove_component(eid);
        }
    }

    template<typename T>
    void for_each_on_depth(int depth, function<void(T&, entity_id)> func) {
        get_container<T>().for_each_on_depth(depth, func);
    }

    template<typename T>
    void for_each_depth_top_down(function<void(int depth)> func) {
        get_container<T>().for_each_depth_top_down(func);
    }

    template<typename T>
    void for_each_depth_down_top(function<void(int depth)> func) {
        get_container<T>().for_each_depth_down_top(func);
    }
};