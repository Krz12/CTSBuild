#include "ui/ui_element.hpp"
#include "engine/color.hpp"

class ui_panel : public ui_element {
    public:
    color foreground_color;
    virtual void render() override {
        //Ai slop gówno narazie
        // Pobierz globalną pozycję w pikselach (uwzględniającą skalę UI)
        vector2<double> pos = __global_transform.position();
        // Rozmiar w pikselach
        vector2<double> size = design_size * ui_manager::scale();

        // Jeśli kolor ma przezroczystość, włącz blendowanie
        if (foreground_color.a < 1.0f) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        // Rysuj prostokąt (np. za pomocą ui_manager lub bezpośrednio OpenGL)
        // Zakładamy, że mamy funkcję do rysowania wypełnionego prostokąta.
        // Może to być np. ui_manager::draw_rect(pos.x, pos.y, size.x, size.y, foreground_color);
        // lub bezpośrednio OpenGL:
        glColor4f(foreground_color.r, foreground_color.g, foreground_color.b, foreground_color.a);
        glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + size.x, pos.y);
        glVertex2f(pos.x + size.x, pos.y + size.y);
        glVertex2f(pos.x, pos.y + size.y);
        glEnd();

        if (foreground_color.a < 1.0f) {
            glDisable(GL_BLEND);
        }
    }
};