#pragma once

#include "Shader.h"


class DebugShader : public Shader
{
    public:
        DebugShader();
        virtual ~DebugShader();

        virtual void digestModel(DrawableModel* model);

};
