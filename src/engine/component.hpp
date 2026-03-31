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

struct MappingEntry {
    int component_index = -1;
    int generation = -1;
    int depth = -1;
};

template<typename T>
class component_container {
public:
    component_container(tree &set_tree) : scene_tree(set_tree) {}

    int add(T component, entity_id entity) {
        int depth = scene_tree.get_vertex(entity.index).depth();
        if (depth >= (int)data.size()) data.resize(depth + 1);

        int index = (int)data[depth].size();
        data[depth].push_back(move(component));

        if (entity.index >= (int)entity_to_index.size())
            entity_to_index.resize(entity.index + 1);
        entity_to_index[entity.index] = {index, entity.generation, depth};

        if ((int)index_to_entity.size() <= depth) index_to_entity.resize(depth + 1);
        if ((int)index_to_entity[depth].size() <= index) index_to_entity[depth].resize(index + 1);
        index_to_entity[depth][index] = entity;

        return index;
    }

    void remove(entity_id entity) {
        if (entity.index >= (int)entity_to_index.size()) return;
        auto& entry = entity_to_index[entity.index];
        if (entry.generation != entity.generation) return; // nie istnieje

        int depth = scene_tree.get_vertex(entity.index).depth();
        int idx = entry.component_index;
        if (idx == -1) return;

        auto& vec = data[depth];
        int lastIdx = (int)vec.size() - 1;

        if (idx != lastIdx) {
            // Przenieś ostatni element na miejsce usuwanego
            vec[idx] = move(vec[lastIdx]);
            entity_id lastEntity = index_to_entity[depth][lastIdx];
            // Zaktualizuj mapowanie dla ostatniej encji
            if (lastEntity.index >= (int)entity_to_index.size())
                entity_to_index.resize(lastEntity.index + 1);
            entity_to_index[lastEntity.index] = {idx, lastEntity.generation, depth};
            index_to_entity[depth][idx] = lastEntity;
        }

        vec.pop_back();
        index_to_entity[depth].pop_back();

        entry.component_index = -1;
        // generation może pozostać, ale lepiej ustawić na -1 (oznacza wolny slot)
        entry.generation = -1;
    }

    //slop
    void move_to_depth(entity_id entity, int new_depth) {
        if (entity.index >= (int)entity_to_index.size()) return;
        auto& entry = entity_to_index[entity.index];
        if (entry.generation != entity.generation) return;
        int old_depth = entry.depth;
        if (old_depth == new_depth) return;
        int idx = entry.component_index;
        if (idx == -1) return;

        // Wyjmij komponent ze starego wektora
        auto& old_vec = data[old_depth];
        int lastIdx = (int)old_vec.size() - 1;
        T component = std::move(old_vec[idx]);

        // Swap & pop w starym wektorze
        if (idx != lastIdx) {
            old_vec[idx] = std::move(old_vec[lastIdx]);
            entity_id lastEntity = index_to_entity[old_depth][lastIdx];
            if (lastEntity.index >= (int)entity_to_index.size())
                entity_to_index.resize(lastEntity.index + 1);
            entity_to_index[lastEntity.index] = {idx, lastEntity.generation, old_depth};
            index_to_entity[old_depth][idx] = lastEntity;
        }
        old_vec.pop_back();
        index_to_entity[old_depth].pop_back();

        // Dodaj komponent na nową głębokość
        if (new_depth >= (int)data.size()) data.resize(new_depth + 1);
        int new_index = (int)data[new_depth].size();
        data[new_depth].push_back(std::move(component));

        if (new_depth >= (int)index_to_entity.size()) index_to_entity.resize(new_depth + 1);
        if (new_index >= (int)index_to_entity[new_depth].size()) index_to_entity[new_depth].resize(new_index + 1);
        index_to_entity[new_depth][new_index] = entity;

        // Aktualizuj mapowanie dla tej encji
        entry.component_index = new_index;
        entry.depth = new_depth;
    }

    T* get(entity_id entity) {
        if (entity.index >= (int)entity_to_index.size())
            return nullptr;
        auto& entry = entity_to_index[entity.index];
        if (entry.generation != entity.generation)
            return nullptr;      // to nie ta sama encja
        int idx = entry.component_index;
        if (idx == -1) return nullptr;
        int depth = scene_tree.get_vertex(entity.index).depth();
        if (depth >= (int)data.size() || idx >= (int)data[depth].size())
            return nullptr;
        return &data[depth][idx];
    }

    int max_depth() const {
        return static_cast<int>(data.size()) - 1;
    }

    void for_each_on_depth(int depth, function<void(T&, entity_id)> func) {
        if (depth >= static_cast<int>(data.size())) return;
        auto& vec = data[depth];
        for (size_t i = 0; i < vec.size(); ++i) {
            func(vec[i], index_to_entity[depth][i]);
        }
    }

    void for_each_depth_top_down(function<void(int)> func) {
        for(size_t i = 1; i < data.size(); ++i) {
            func(static_cast<int>(i));
        }
    }
    void for_each_depth_down_top(function<void(int)> func) {
        for(int i = static_cast<int>(data.size()) - 1; i >= 0; --i) {
            func(i);
        }
    }

private:
    vector<vector<T>> data;
    tree& scene_tree;
    vector<MappingEntry> entity_to_index;
    vector<vector<entity_id>> index_to_entity;
};

class IComponentContainer {
public:
    virtual ~IComponentContainer() = default;
    virtual void remove(entity_id entity) = 0;
    virtual void* get(entity_id entity) = 0;
    virtual void move_to_depth(entity_id entity, int new_depth) = 0;
};

template<typename T>
class ComponentContainerTyped : public IComponentContainer {
public:
    ComponentContainerTyped(tree& t) : impl(t) {}

    void add(T comp, entity_id entity) {
        impl.add(move(comp), entity);
    }

    void remove(entity_id entity) override {
        impl.remove(entity);
    }

    void* get(entity_id entity) override {
        return impl.get(entity);
    }

    void move_to_depth(entity_id entity, int new_depth) override {
        impl.move_to_depth(entity, new_depth);
    }

    component_container<T>& getImpl() {
        return impl;
    }

private:
    component_container<T> impl;
};

class component_manager {
public:
    component_manager(tree& t) : __scene_tree(t) {}

    template<typename T>
    void add_component(entity_id entity, T comp) {
        size_t typeIdx = getTypeIndex<T>();
        auto& container = getContainer<T>(typeIdx);
        container.add(move(comp), entity);
    }

    template<typename T> requires std::is_default_constructible_v<T>
    void add_component(entity_id entity) {
        size_t typeIdx = getTypeIndex<T>();
        auto& container = getContainer<T>(typeIdx);
        container.add(move(T{}), entity);
    }

    template<typename T>
    void remove_component(entity_id entity) {
        size_t typeIdx = getTypeIndex<T>();
        auto it = containers.find(typeIdx);
        if (it != containers.end()) {
            it->second->remove(entity);
        }
    }

    void move_components(entity_id entity, int new_depth) {
        for (auto& [typeIdx, container] : containers) {
            container->move_to_depth(entity, new_depth);
        }
    }

    template<typename T>
    T* get_component(entity_id entity) {
        size_t typeIdx = getTypeIndex<T>();
        auto it = containers.find(typeIdx);
        if (it == containers.end()) return nullptr;
        auto* typed = dynamic_cast<ComponentContainerTyped<T>*>(it->second.get());
        if (!typed) return nullptr;
        return static_cast<T*>(typed->get(entity));
    }

    template<typename T>
    void for_each_depth_top_down(function<void(int)> func) {
        size_t typeIdx = getTypeIndex<T>();
        auto it = containers.find(typeIdx);
        if (it == containers.end()) return;
        auto* typed = dynamic_cast<ComponentContainerTyped<T>*>(it->second.get());
        if (typed) typed->getImpl().for_each_depth_top_down(func);
    }

    template<typename T>
    void for_each_depth_down_top(function<void(int)> func) {
        size_t typeIdx = getTypeIndex<T>();
        auto it = containers.find(typeIdx);
        if (it == containers.end()) return;
        auto* typed = dynamic_cast<ComponentContainerTyped<T>*>(it->second.get());
        if (typed) typed->getImpl().for_each_depth_down_top(func);
    }

    template<typename T>
    void for_each_on_depth(int depth, function<void(T&, entity_id)> func) {
        size_t typeIdx = getTypeIndex<T>();
        auto it = containers.find(typeIdx);
        if (it == containers.end()) return;
        auto* typed = dynamic_cast<ComponentContainerTyped<T>*>(it->second.get());
        if (typed) typed->getImpl().for_each_on_depth(depth, func);
    }

    void remove_all_components(entity_id entity) {
        for (auto& [idx, container] : containers) {
            container->remove(entity);
        }
    }

    tree& scene_tree() {
        return __scene_tree;
    }

private:
    template<typename T>
    static size_t getTypeIndex() {
        static size_t idx = nextTypeIndex++;
        return idx;
    }

    template<typename T>
    ComponentContainerTyped<T>& getContainer(size_t typeIdx) {
        auto it = containers.find(typeIdx);
        if (it == containers.end()) {
            auto newContainer = make_unique<ComponentContainerTyped<T>>(__scene_tree);
            auto& ref = *newContainer;
            containers[typeIdx] = move(newContainer);
            return ref;
        }
        return *static_cast<ComponentContainerTyped<T>*>(it->second.get());
    }

    static inline size_t nextTypeIndex = 0;
    unordered_map<size_t, unique_ptr<IComponentContainer>> containers;
    tree& __scene_tree;
};