#pragma once

#ifndef PXModelIncluded
#define PXModelIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>


// A Model is a renderable entity.
// Has a mesh as a base structure
// Matrix for where it is
// Render information
typedef struct PXModel_ PXModel;

typedef struct PXModelCreateInfo_
{
    PXECSCreateInfo Info;

    PXShaderProgram* ShaderProgramReference;
    PXF32 Scale;

    PXI32U HitBoxBehaviour;

    // If not loaded by a file, this data shal be used
    PXVertexBuffer VertexBuffer;
    PXIndexBuffer IndexBuffer;

    PXModelForm Form;
}
PXModelCreateInfo;

PXPublic PXResult PXAPI PXModelRegisterToECS();
PXPublic PXResult PXAPI PXModelCreate(PXModel** pxModelREF, PXModelCreateInfo PXREF pxModelCreateInfo);
PXPublic PXResult PXAPI PXModelRelease(PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXModelDraw(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo);
PXPublic PXResult PXAPI PXModelDrawGL(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo);

#endif