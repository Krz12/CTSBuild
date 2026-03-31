#pragma once
#include <functional>
#include <GL/gl.h>
#include "engine/entity.hpp"
#include "engine/component.hpp"
#include "graphics/texture_id.hpp"
#include "components/transform_2d.hpp"

using namespace std;
using texture_generator = function<texture_id(const entity_id&, component_manager&)>;

class renderer_2d
{
public:
    texture_generator generator;
    texture_id __texture_id;
    vector2<double> base_size = {100, 100};
    bool dirty = true;
    renderer_2d() = default;
    explicit renderer_2d(texture_generator gen) : generator(move(gen)), dirty(true) {}
    //noexcept - mówimy że nie będzie wyjątków a jak będzie to odrazu program wywali
    renderer_2d(renderer_2d&& other) noexcept
        : generator(move(other.generator))
        , __texture_id(other.__texture_id)
        , dirty(other.dirty) {
        other.__texture_id.opengl_texture_id = 0; // zapobiega podwójnemu usunięciu
    }
    //Ustawia render na jakis inny więc trzeba usunąć teksturę
    renderer_2d& operator=(renderer_2d&& other) noexcept {
        if (this != &other) {
            if (__texture_id.opengl_texture_id != 0) glDeleteTextures(1, &__texture_id.opengl_texture_id);
            generator = move(other.generator);
            __texture_id.opengl_texture_id = other.__texture_id.opengl_texture_id;
            dirty = other.dirty;
            other.__texture_id.opengl_texture_id = 0;
        }
        return *this;
    }

    renderer_2d(const renderer_2d&) = delete;
    renderer_2d& operator=(const renderer_2d&) = delete;

    ~renderer_2d() {
        if (__texture_id.opengl_texture_id != 0) glDeleteTextures(1, &__texture_id.opengl_texture_id);
    }
};