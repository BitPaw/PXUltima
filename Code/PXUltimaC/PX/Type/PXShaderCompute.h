#pragma once
#include <PX/Type/PXShader.h>

typedef struct PXShaderCompute_
{
    PXShader ShaderBase;

    union
    {
        PXI32U OpenGLID;
    };
}
PXShaderCompute;