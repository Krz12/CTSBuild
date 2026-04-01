#include "systems/ui/ui_system.hpp"
#include "engine/window_manager.hpp"
#include "ui/ui_alligment.hpp"
#include "components/transform_2d.hpp"
#include "components/ui/ui_element.hpp"
#include "components/renderer_2d.hpp"

const double ui_system::scale()
{
    return __scale;
}

void ui_system::update(component_manager &cm) {
    window* w = window_manager::main_window();
    __mouse_click_captured = 0;
    __mouse_scroll_captured = 0;
    __scale = w->size().y / 1080.0;
    //Teraz przejdziemy po całym drzewie
    cm.for_each_depth_top_down<ui_element>([&](int depth){
    cm.for_each_on_depth<ui_element>(depth, [&](ui_element& t, entity_id e) {
        // operacje na komponencie
        entity_id parent = {cm.scene_tree().get_vertex(e.index).parent(), 0};
        transform_2d &__transform = cm.get_component<transform_2d>(e);
        transform_2d &__parent_transform = cm.get_component<transform_2d>(parent);
        ui_element &__ui_element_comp = cm.get_component<ui_element>(e);
        //renderer_2d &__renderer = cm.get_component<renderer_2d>(e);
        //Ustaw rendererowi rozmiar
        //if(!__renderer) throw("ui element tried to get it's renderer_2d but it doesn't exsist!");
        //__renderer->base_size = __ui_element_comp->design_size;

        vector2<double> parent_size;
        if (cm.has_component<ui_element>(parent)) {
            ui_element &ui_parent = cm.get_component<ui_element>(parent);
            parent_size = ui_parent.design_size;
        } else {
            parent_size = {(double)window_manager::main_window()->size().x, (double)window_manager::main_window()->size().y};
        }
        __transform.local.position((__ui_element_comp.design_offset+calculate_transform(e, parent, cm))*scale());

        if(cm.has_component<ui_element>(parent)) {
           __transform.global = __transform.calculate_global(__parent_transform.global);
        } else {
            __transform.global = __transform.local;
        }
        });
    });
}

vector2<double> ui_system::calculate_transform(const entity_id &object, const entity_id &parent, component_manager &cm)
{ // Zależnie od pivota, anchora, zwraca lewy górny róg tak aby było na miejscu
    const vector2<double> &parent_size = cm.get_component<ui_element>(parent).design_size;
    ui_element &ui_el = cm.get_component<ui_element>(object);
    vector2<double> anchor_point;
    switch (ui_el.anchor)
    {
    case ui_anchor::top_left:
        anchor_point = {0, 0};
        break;
    case ui_anchor::top_center:
        anchor_point = {parent_size.x / 2.0, 0};
        break;
    case ui_anchor::top_right:
        anchor_point = {parent_size.x, 0};
        break;
    case ui_anchor::center_left:
        anchor_point = {0, parent_size.y / 2.0};
        break;
    case ui_anchor::center:
        anchor_point = {parent_size.x / 2.0, parent_size.y / 2.0};
        break;
    case ui_anchor::center_right:
        anchor_point = {parent_size.x, parent_size.y / 2.0};
        break;
    case ui_anchor::bottom_left:
        anchor_point = {0, parent_size.y};
        break;
    case ui_anchor::bottom_center:
        anchor_point = {parent_size.x / 2.0, parent_size.y};
        break;
    case ui_anchor::bottom_right:
        anchor_point = {parent_size.x, parent_size.y};
        break;
    }

    vector2<double> pivot_offset;
    switch (ui_el.pivot)
    {
    case ui_pivot::top_left:
        pivot_offset = {0, 0};
        break;
    case ui_pivot::top_center:
        pivot_offset = {ui_el.design_size.x / 2.0, 0};
        break;
    case ui_pivot::top_right:
        pivot_offset = {ui_el.design_size.x, 0};
        break;
    case ui_pivot::center_left:
        pivot_offset = {0, ui_el.design_size.y / 2.0};
        break;
    case ui_pivot::center:
        pivot_offset = {ui_el.design_size.x / 2.0, ui_el.design_size.y / 2.0};
        break;
    case ui_pivot::center_right:
        pivot_offset = {ui_el.design_size.x, ui_el.design_size.y / 2.0};
        break;
    case ui_pivot::bottom_left:
        pivot_offset = {0, ui_el.design_size.y};
        break;
    case ui_pivot::bottom_center:
        pivot_offset = {ui_el.design_size.x / 2.0, ui_el.design_size.y};
        break;
    case ui_pivot::bottom_right:
        pivot_offset = {ui_el.design_size.x, ui_el.design_size.y};
        break;
    }

    // Pozycja lewego górnego rogu elementu: anchor_point - pivot_offset + design_offset
    vector2<double> top_left = anchor_point - pivot_offset + ui_el.design_offset;
    return top_left;
}