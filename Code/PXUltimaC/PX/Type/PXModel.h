#pragma once

#ifndef PXModelIncluded
#define PXModelIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXMesh.h>
#include <PX/Type/PXShader.h>
#include <PX/Math/PXTransform.h>

// Mesh with instance data, position and rotation, ect..
typedef struct PXModel_ PXModel;

typedef struct PXModelCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize MeshListAmount;
    PXMesh* MeshList;
}
PXModelCreateInfo;

PXPublic void PXAPI PXModelRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXModelCreate(PXModel** pxModelREF, PXModelCreateInfo PXREF pxModelCreateInfo);
PXPublic PXResult PXAPI PXModelRelease(PXModel PXREF pxModel);

// mesh
PXPublic PXResult PXAPI PXModelMeshAmountPrepare(PXModel PXREF pxModel, const PXSize amount);
PXPublic PXSize PXAPI PXModelMeshAmount(const PXModel PXREF pxModel);
PXPublic PXMesh* PXAPI PXModelMeshGet(const PXModel PXREF pxModel, const PXSize index);
PXPublic PXResult PXAPI PXModelMeshSet(PXModel PXREF pxModel, PXMesh PXREF pxMesh, const PXSize index);

PXPublic PXResult PXAPI PXModelMaterialSet(PXModel PXREF pxModel, PXMaterial PXREF pxMaterial, const PXSize meshIndex, const PXSize segmentIndex);
PXPublic PXMaterial* PXAPI PXModelMaterialGetViaIndex(const PXModel PXREF pxModel, const PXSize mateialIndex);
PXPublic PXMaterial* PXAPI PXModelMaterialGetForMeshIndex(const PXModel PXREF pxModel, const PXSize meshIndex, const PXSize segmentIndex);
PXPublic PXSize PXAPI PXModelMaterialAmount(const PXModel PXREF pxModel);

PXPublic PXResult PXAPI PXModelShow(PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXModelHide(PXModel PXREF pxModel);

PXPublic PXTransform4D* PXAPI PXModelTransformGet(const PXModel PXREF pxModel);

#endif