/*#include "ui/ui_alligment.hpp"
#include "math/vector2.hpp"
#include "engine/window_manager.hpp"
//#include "ui/ui_manager.hpp"

class ui_element : public virtual game_object_2d {
    private:
    vector2<double> calculate_transform(const vector2<double> &parent_size) { //Zależnie od pivota, anchora, zwraca lewy górny róg tak aby było na miejscu
        vector2<double> anchor_point;
        switch (anchor) {
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
        switch (pivot) {
            case ui_pivot::top_left:
                pivot_offset = {0, 0};
                break;
            case ui_pivot::top_center:
                pivot_offset = {design_size.x / 2.0, 0};
                break;
            case ui_pivot::top_right:
                pivot_offset = {design_size.x, 0};
                break;
            case ui_pivot::center_left:
                pivot_offset = {0, design_size.y / 2.0};
                break;
            case ui_pivot::center:
                pivot_offset = {design_size.x / 2.0, design_size.y / 2.0};
                break;
            case ui_pivot::center_right:
                pivot_offset = {design_size.x, design_size.y / 2.0};
                break;
            case ui_pivot::bottom_left:
                pivot_offset = {0, design_size.y};
                break;
            case ui_pivot::bottom_center:
                pivot_offset = {design_size.x / 2.0, design_size.y};
                break;
            case ui_pivot::bottom_right:
                pivot_offset = {design_size.x, design_size.y};
                break;
        }

        // Pozycja lewego górnego rogu elementu: anchor_point - pivot_offset + design_offset
        vector2<double> top_left = anchor_point - pivot_offset + design_offset;
        return top_left;
    }
    protected:
    double top, left, bottom, right;
    public:
        ui_element(scene_tree* tree, vector2<double> position = {0, 0})
        : vertex(tree->next_index(), tree->next_adj()),
          tree_vertex(tree->next_index(), tree->next_adj(),
          tree->parent_ptr(tree->next_index())),
          abstract_game_object(tree->next_index(), tree->next_adj(),
          tree->parent_ptr(tree->next_index())),
          game_object(tree),
          game_object_2d(tree, position),
          anchor(ui_anchor::top_left),
          pivot(ui_pivot::top_left),
          design_offset(0, 0),
          design_size(100, 100),
          does_capture_mouse_click(false),
          does_capture_mouse_scroll(false)
    {}
    ui_anchor anchor;
    ui_pivot pivot;
    vector2<double> design_offset;
    vector2<double> design_size;
    bool does_capture_mouse_click, does_capture_mouse_scroll;
    virtual void update_enter() override
    {
        //game_object_2d::update_enter();
        game_object& parent = parent_object();
        ui_element* ui_parent = dynamic_cast<ui_element*>(&parent);
        //Oblicz transform względem parenta (transform jest przeskalowany)
        vector2<double> parent_size;
        if (ui_parent) {
            parent_size = ui_parent->design_size;
        } else {
            parent_size = {(double)window_manager::main_window()->size().x, (double)window_manager::main_window()->size().y};
        }
        __transform.position((design_offset+calculate_transform(parent_size))*ui_manager::scale());

        if(ui_parent) {
           __global_transform = transform_2d::calculate_global(ui_parent->global_transform(), __transform);
        } else {
            __global_transform = __transform;
        }
    }
};
*/