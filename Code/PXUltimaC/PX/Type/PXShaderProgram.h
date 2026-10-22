#pragma once

#ifndef PXShaderProgramIncluded
#define PXShaderProgramIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXShaderCompute.h>
#include <PX/Type/PXShaderGeometry.h>
#include <PX/Type/PXShaderPixelFragment.h>
#include <PX/Type/PXShaderTessellationControlHull.h>
#include <PX/Type/PXShaderTessellationEvaluationDomain.h>
#include <PX/Type/PXShaderVertex.h>
#include "PXShaderUniform.h"

typedef struct PXShaderProgram_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;// ProgramID;
    };

    PXShaderCompute* Compute;
    PXShaderGeometry* Geometry;
    PXShaderPixelFragment* PixelFragment;
    PXShaderTessellationControlHull* TessellationControlHull;
    PXShaderTessellationEvaluationDomain* TessellationEvaluationDomain;
    PXShaderVertex* Vertex;

    PXSize NumberOfUniforms;
    PXShaderUniform* UniformList;

    PXSize NumberOfAttributes;
    PXShaderVariable* AttributeList;
}
PXShaderProgram;

typedef struct PXShaderProgramCreateInfo_
{
    PXECSCreateInfo Info;
    
    PXShaderCompute* Compute;
    PXShaderGeometry* Geometry;
    PXShaderPixelFragment* PixelFragment;
    PXShaderTessellationControlHull* TessellationControlHull;
    PXShaderTessellationEvaluationDomain* TessellationEvaluationDomain;
    PXShaderVertex* Vertex;
}
PXShaderProgramCreateInfo;

PXInternal void PXAPI PXShaderProgramRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXShaderProgramCreate(PXShaderProgram** pxShaderProgramREF, PXShaderProgramCreateInfo PXREF pxShaderProgramCreateInfo);
PXPublic PXResult PXAPI PXShaderProgramRelease(PXShaderProgram PXREF pxShaderProgram);

PXPublic PXSize PXAPI PXShaderProgramAmountOfShader(const PXShaderProgram PXREF pxShaderProgram);

#endif