#pragma once

#ifndef PXTransformIncluded
#define PXTransformIncluded

#include <PX/Math/PXVector.h>
#include <PX/Math/PXRotation.h>

typedef struct PXTransform3D
{
    PXRotor3DF32 Rotor;
    PXVector3F32 Position;
    PXVector3F32 Scale;
}
PXTransform3D;

typedef struct PXTransform4D
{
    PXRotor4DF32 Rotor;
    PXVector4F32 Position;
    PXVector4F32 Scale;
}
PXTransform4D;

PXPublic void PXAPI PXTransform3DInit(PXTransform3D PXREF pxTransform3D);
PXPublic void PXAPI PXTransform4DInit(PXTransform4D PXREF pxTransform4D);

PXPublic void PXAPI PXTransform4DToMatrix(const PXTransform4D PXREF pxTransform4D, PXMatrix4x4F PXREF pxMatrix4x4F);

#endif