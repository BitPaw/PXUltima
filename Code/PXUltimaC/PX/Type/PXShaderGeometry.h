#pragma once

#include <PX/Type/PXShader.h>

typedef struct PXShaderGeometry_
{
    PXShader ShaderBase;  

    union
    {
        PXI32U OpenGLID;
    };
}
PXShaderGeometry;