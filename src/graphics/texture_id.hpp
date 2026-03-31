#pragma once
#include "GL/gl.h"
class texture_id
{
private:
public:
    GLuint opengl_texture_id = 0;
    texture_id();
    ~texture_id() {
        if(opengl_texture_id != 0) {
            glDeleteTextures(1, &opengl_texture_id);
        }
    };
};