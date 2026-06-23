#pragma once

#ifndef PXModelIncluded
#define PXModelIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXMesh.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXWindow.h>


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

    PXSize MeshAmount;
}
PXModelCreateInfo;

PXPublic void PXAPI PXModelRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXModelCreate(PXModel** pxModelREF, PXModelCreateInfo PXREF pxModelCreateInfo);
PXPublic PXResult PXAPI PXModelRelease(PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXModelDraw(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo);
PXPublic PXResult PXAPI PXModelDrawGL(PXModel PXREF pxModel, PXDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXModelMeshAmountSet(PXModel PXREF pxModel, const PXSize amount);
PXPublic PXSize PXAPI PXModelMeshAmountGet(const PXModel PXREF pxModel);
PXPublic PXMesh** PXAPI PXModelMeshGet(const PXModel PXREF pxModel, const PXSize index);

#endif