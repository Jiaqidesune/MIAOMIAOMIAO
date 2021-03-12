#pragma once

#include "Shader.h"

#include <vector>

namespace Rendering
{
    class Program
    {
    public:
        Program(const std::vector<Shader>& shaders);
        ~Program();

        void Bind();
        void UnBind();
        GLuint Object();

    private:
        GLuint m_Object;
    };
}