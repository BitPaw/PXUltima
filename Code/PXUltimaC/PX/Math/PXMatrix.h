#pragma once

#ifndef PXMatrixIncluded
#define PXMatrixIncluded

#include <PX/Media/PXType.h>
#include <PX/Math/PXVector.h>

#define XAxisX 0
#define XAxisY 4
#define XAxisZ 8
#define XAxisW 12

#define YAxisX 1
#define YAxisY 5
#define YAxisZ 9
#define YAxisW 13

#define ZAxisX 2
#define ZAxisY 6
#define ZAxisZ 10
#define ZAxisW 14

#define WAxisX 3
#define WAxisY 7
#define WAxisZ 11
#define WAxisW 15

#define ScaleX 0
#define ScaleY 5
#define ScaleZ 10
#define ScaleW 15

#define TransformX 12
#define TransformY 13
#define TransformZ 14
#define TransformW 15

/*
    +----+----+----+----+
    | XA | YA | ZA | Or |
    +----+----+----+----+
    |  0 |  1 |  2 |  3 |
    |  4 |  5 |  6 |  7 |
    |  8 |  9 | 10 | 11 |
    | 12 | 13 | 14 | 15 |
    +----+----+----+----+
*/
typedef struct PXMatrix4x4F_
{
    union
    {
        struct
        {
            PXF32 XX;
            PXF32 XY;
            PXF32 XZ;
            PXF32 XW;
            PXF32 YX;
            PXF32 YY;
            PXF32 YZ;
            PXF32 YW;
            PXF32 ZX;
            PXF32 ZY;
            PXF32 ZZ;
            PXF32 ZW;
            PXF32 WX;
            PXF32 WY;
            PXF32 WZ;
            PXF32 WW;
        };

        PXF32 Data[16];

        PXF32 DataXY[4][4];
    };
}
PXMatrix4x4F;

PXPublic void PXAPI PXMatrix4x4FIdentity(PXMatrix4x4F PXREF matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FResetAxisW(PXMatrix4x4F PXREF matrix4x4F);

PXPublic void PXAPI PXMatrix4x4FRotationAxisSet(PXMatrix4x4F PXREF matrix4x4F, const PXI8U axisA, const PXI8U axisB, const PXF32 angle);

PXPublic void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FPositionSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);

PXPublic void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);
PXPublic void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position);

PXPublic void PXAPI PXMatrix4x4FMultiply(PXMatrix4x4F PXREF matrixA, const PXMatrix4x4F PXREF matrixB);

PXPublic void PXAPI PXMatrix4x4FMultiplyV4F(const PXMatrix4x4F PXREF matrix, PXVector4F32 PXREF pxVector4F32);

PXPublic void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF vector3F);

PXPublic void PXAPI PXMatrix4x4FCopy(const PXMatrix4x4F PXREF matrixA, PXMatrix4x4F PXREF matrixResult);

PXPublic void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F PXREF matrixData, const PXVector3F32 PXREF vector3F);
PXPublic void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F PXREF matrixA, const PXF32 scalar);

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
