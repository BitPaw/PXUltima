#pragma once

#include <PX/Type/PXShader.h>

typedef struct PXShaderTessellationControlHull_
{
    PXShader ShaderBase;

    union
    {
        PXI32U OpenGLID;

    };
}
PXShaderTessellationControlHull;