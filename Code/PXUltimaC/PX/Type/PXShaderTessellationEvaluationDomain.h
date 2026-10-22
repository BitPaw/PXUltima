#pragma once

#include <PX/Type/PXShader.h>

typedef struct PXShaderTessellationEvaluationDomain_
{
    PXShader ShaderBase;

    union
    {
        PXI32U OpenGLID;
    };
}
PXShaderTessellationEvaluationDomain;