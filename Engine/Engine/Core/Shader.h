#pragma once

#include <string>

#include <GL/glew.h>

namespace Rendering
{
    class Shader
    {
    public:
        Shader(const std::string& filePath, GLuint shaderType);
        GLuint Object() const;
    private:
        GLuint m_Object;
    };
}