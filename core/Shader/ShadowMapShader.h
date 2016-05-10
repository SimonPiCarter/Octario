#pragma once

#include "Shader.h"


class ShadowMapShader : public Shader
{
    public:

    ShadowMapShader();
    virtual ~ShadowMapShader();

    virtual void digestModel(DrawableModel* model);
};
