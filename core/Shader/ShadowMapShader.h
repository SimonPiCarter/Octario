#ifndef SHADOWMAPSHADER_H
#define SHADOWMAPSHADER_H

#include "Shader.h"


class ShadowMapShader : public Shader
{
    public:

    ShadowMapShader();
    virtual ~ShadowMapShader();

    virtual void digestModel(DrawableModel* model);
};

#endif // SHADOWMAPSHADER_H
