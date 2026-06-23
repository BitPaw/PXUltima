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

#endif