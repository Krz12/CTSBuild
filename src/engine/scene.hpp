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
#include "engine/component.hpp"
#include "engine/entity.hpp"
#include "systems/logic_system.hpp"
#include "systems/testsystem.hpp"
#include "systems/logic_system_register.hpp"

using namespace std;

class scene {
private: 
    unordered_set<unique_ptr<logic_system>> __logic_systems;
protected:
    shared_ptr<tree> __tree = make_shared<tree>();
    vector<int> generations;
    
    virtual void render_2d(const vector<int> &to_render) {
        glDisable(GL_DEPTH_TEST);
    }
    
    virtual void render_3d(const vector<int> &to_render) {
        glEnable(GL_DEPTH_TEST);
    }
    
public:
    component_manager manager{*__tree};
    scene() {
        auto factories = logic_system_register::get_factories();
        for (const auto& [name, factory] : factories) {
            __logic_systems.insert(factory());
        }
    }

    tree& get_tree() {
        return *__tree;
    }

    entity_id create_entity(entity_id parent = {0, 0}) {
        int idx = __tree->add_vertex(parent.index).index;
        if (idx >= generations.size()) generations.resize(idx + 1, 0);
        entity_id eid{idx, generations[idx]};
        return eid;
    }

    void remove_entity(entity_id id) {
        if(id.index == 0) return;
        vector<int> to_remove = __tree->get_descendants(id.index);
        for (int idx : to_remove) {
            entity_id eid{idx, generations[idx]};
            manager.remove_all_components(eid);
            __tree->remove_vertex(idx);
            generations[idx]++;
        }
    }

    //slop
    void set_parent(entity_id entity, entity_id new_parent) {
        if (entity.index == new_parent.index) return;
        __tree->set_parent(entity.index, new_parent.index);

        // Pobierz wszystkie węzły w poddrzewie (włącznie z entity)
        vector<int> subtree = __tree->get_descendants(entity.index);
        for (int idx : subtree) {
            entity_id eid{idx, generations[idx]};
            int new_depth = __tree->get_vertex(idx).depth();
            manager.move_components(eid, new_depth);
        }
    }

    virtual void update() {
        for(auto &ls : __logic_systems) {
            ls->update(manager);
        }
    }

    virtual void render() {
        vector<entity_id> to_render_3d;
        vector<entity_id> to_render_2d;
        auto enter_func = [&](shared_ptr<vertex> const& u,
                            shared_ptr<edge> const& last,
                            vector<dfs_state> const& state,
                            vector<int> const& current_edge) -> bool {
            entity_id eid{u->index, generations[u->index]};
            /*if (manager.get_component<renderer_2d>(eid)) {
                to_render_2d.push_back(eid);
            }*/
            return true;
        };

        auto leave_func = [&](shared_ptr<vertex> const& u,
                            shared_ptr<edge> const& last,
                            vector<dfs_state> const& state,
                            vector<int> const& current_edge) {
            entity_id eid{u->index, generations[u->index]};
            /*if (manager.get_component<renderer_3d>(eid)) {
                to_render_3d.push_back(eid);
            }*/
        };

        __tree->dfs(0, enter_func, leave_func);

        // Wywołaj funkcje renderowania z odpowiednimi listami
        //render_3d(to_render_3d);   // kolejność post-order (dzieci przed rodzicem)
        //render_2d(to_render_2d);   // kolejność pre-order (rodzic przed dziećmi)
        
    }

    virtual ~scene() {}
};