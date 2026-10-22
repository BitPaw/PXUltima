#pragma once

#ifndef PXMatrixIncluded
#define PXMatrixIncluded

#include <PX/Type/PXType.h>
#include <PX/Math/PXVector.h>

#define ScaleX   0 
#define ScaleY   5 
#define ScaleZ   10
#define ScaleW   15

typedef struct PXMatrix4x4FColumn_
{
    union
    {
        struct
        {
            PXF32 Right;
            PXF32 Up;
            PXF32 Forward;
            PXF32 Translation;
        };

        struct
        {
            PXF32 A;
            PXF32 B;
            PXF32 C;
            PXF32 D;
        };
    };   
}
PXMatrix4x4FColumn;

typedef struct PXMatrix4x4F_
{
    union
    {
        // Only DirectX uses this format.
        // OpenGL and Vulcan do use the coloum major format.
        // Axis are oriented horizontally in a row.
        // [Index  0,  1,  2,  3] Row 0 : Right.X,  Right.Y     Right.Z     0.0
        // [Index  4,  5,  6,  7] Row 1 : Up.X      Up.Y        Up.Z        0.0
        // [Index  8,  9, 10, 11] Row 2 : Forward.X Forward.Y   Forward.Z   0.0
        // [Index 12, 13, 14, 15] Row 3 : Trans.X   Trans.Y     Trans.Z     1.0
        struct
        {
            PXVector4F32 Right;
            PXVector4F32 Up;
            PXVector4F32 Forward;
            PXVector4F32 Translation;
        }
        Row;

        // Column Major Format: Used by OpenGL and Vulcan.
        // Axis are stacked vertically.
        // [Index  0, 4,  8, 12] Col 0 : Right.X    Up.X    Forward.X   Trans.X
        // [Index  1, 5,  9, 13] Col 1 : Right.Y    Up.Y    Forward.Y   Trans.Y
        // [Index  2, 6, 10, 14] Col 2 : Right.Z    Up.Z    Forward.Z   Trans.Z
        // [Index  3, 7, 11, 15] Col 3 : Right.W    Up.W    Forward.W   Trans.W
        struct
        {
            PXMatrix4x4FColumn X;
            PXMatrix4x4FColumn Y;
            PXMatrix4x4FColumn Z;
            PXMatrix4x4FColumn W;
        }
        Column;

        PXF32 Data[16];

        PXF32 DataXY[4][4];
    };
}
PXMatrix4x4F;

PXPublic void PXAPI PXMatrix4x4FIdentity(PXMatrix4x4F PXREF matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FResetAxisW(PXMatrix4x4F PXREF matrix4x4F);

PXPublic void PXAPI PXMatrix4x4FRotationAxisSet(PXMatrix4x4F PXREF matrix4x4F, const PXI8U axisA, const PXI8U axisB, const PXF32 angle);

PXPublic void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FPosition3DSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FPosition4DSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector4F32 PXREF position);


PXPublic void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);

PXPublic void PXAPI PXMatrix4x4FMultiply(PXMatrix4x4F PXREF matrixA, const PXMatrix4x4F PXREF matrixB);

PXPublic void PXAPI PXMatrix4x4FMultiplyV4F(const PXMatrix4x4F PXREF matrix, PXVector4F32 PXREF pxVector4F32);

PXPublic void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF vector3F);

PXPublic void PXAPI PXMatrix4x4FCopy(PXMatrix4x4F PXREF pxMatrix, const PXMatrix4x4F PXREF matrixSource);

PXPublic void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F PXREF matrixData, const PXVector3F32 PXREF vector);
PXPublic void PXAPI PXMatrix4x4FMove4F(PXMatrix4x4F PXREF matrixData, const PXVector4F32 PXREF vector);
PXPublic void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F PXREF matrixA, const PXF32 scalar);

PXPublic void PXAPI PXMatrix4x4FScale4D(PXMatrix4x4F PXREF pxMatrix, const PXVector4F32 PXREF pxVector4F32);


//PXPublic void PXAPI PXMatrix4x4FScaleByMargin(PXMatrix4x4F PXREF pxMatrix4x4F, const PXRectangleOffset PXREF pxMargin);
PXPublic void PXAPI PXMatrix4x4FScaleByXY(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 x, const PXF32 y);

PXPublic void PXAPI PXMatrix4x4FScaleSet(PXMatrix4x4F PXREF pxMatrix4x4F, const PXVector3F32 PXREF pxVector3F32);
PXPublic void PXAPI PXMatrix4x4FScaleSetXY(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXMatrix4x4FScaleSetXYZ(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXMatrix4x4FScaleGet(const PXMatrix4x4F PXREF matrixResult, PXVector3F32 PXREF pxVector3F32);
PXPublic void PXAPI PXMatrix4x4FScaleGetXYZ(const PXMatrix4x4F PXREF matrixResult, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z);

PXPublic void PXAPI PXMatrix4x4FOrthographic(PXMatrix4x4F PXREF matrix4x4F, const PXF32 left, const PXF32 right, const PXF32 bottom, const PXF32 top, const PXF32 nearPlane, const PXF32 farPlane);
PXPublic void PXAPI PXMatrix4x4FPerspective(PXMatrix4x4F PXREF matrix4x4F, const PXF32 fielfOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane);

PXPublic PXBool PXAPI PXMatrix4x4FInverse(PXMatrix4x4F PXREF matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FTranpose(PXMatrix4x4F PXREF matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FLookAt(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF eye, const PXVector3F32 PXREF  center, const PXVector3F32 PXREF up);

#endif
