#include "ShadowMapShader.h"

ShadowMapShader::ShadowMapShader() : Shader("Shaders/shadowMap.vert","Shaders/shadowMap.frag")
{
    //ctor
}

ShadowMapShader::~ShadowMapShader()
{
    // Destruction of shader
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}

void ShadowMapShader::digestModel(DrawableModel* model) {
}
