#pragma once
#include "engine/scene.hpp"
#include "engine/logger.hpp"
#include "ui/ui_elments/ui_panel.hpp"
#include "systems/testsystem.hpp"

using namespace std;

class main_menu_scene : public scene {
    public:
    main_menu_scene() {
        //auto panel = ui_panel::create();
        //panel->anchor = ui_anchor::center;
        //panel->design_size = vector2<double>(400, 100);
        entity_id test_ent = create_entity();
        manager.add_component<test_comp>(test_ent);
    }
};