#include "DebugShader.h"

DebugShader::DebugShader() : Shader("Shaders/debugColor.vert","Shaders/debugColor.frag")
{
    //ctor
}

DebugShader::~DebugShader()
{
    // Destruction of shader
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

void DebugShader::digestModel(DrawableModel* model) {
}
