#pragma once

#ifndef PXModelIncluded
#define PXModelIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>


typedef struct PXModelCreateInfo_
{
    PXShaderProgram* ShaderProgramReference;
    PXF32 Scale;

    PXI32U HitBoxBehaviour;

    // If not loaded by a file, this data shal be used
    PXVertexBuffer VertexBuffer;
    PXIndexBuffer IndexBuffer;

    PXModelForm Form;
}
PXModelCreateInfo;


// A Model is a renderable entity.
// Has a mesh as a base structure
// Matrix for where it is
// Render information
typedef struct PXModel_ PXModel;

PXPublic PXResult PXAPI PXModelRegisterToECS();

PXPublic void PXAPI PXModelConstruct(PXModel PXREF pxModel);

#endif