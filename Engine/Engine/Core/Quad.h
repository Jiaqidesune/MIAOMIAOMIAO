#pragma once

#include <GL/glew.h>

namespace Rendering
{
    class Program;

    class Quad
    {
    public:
        Quad();

        void Draw(Program* program);
    private:
        GLuint vao;
        GLuint vbo;
    };
}