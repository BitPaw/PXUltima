#pragma once

#include <PX/Type/PXType.h>
#include <PX/Math/PXMatrix.h>

// Classic angles of rotation
// Problematic in calculation when axis allign
typedef struct PXEulerAngle3F32_
{
    union
    {
        struct
        {
            PXF32 Yaw;
            PXF32 Pitch;
            PXF32 Roll;
        };

        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
        };

        PXF32 Data[3];
    };
}
PXEulerAngle3F32;

typedef struct PXEulerAngle4F32_
{
    union
    {
        struct
        {
            PXF32 Yaw;
            PXF32 Pitch;
            PXF32 Roll;
            PXF32 Skew;
        };

        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
            PXF32 W;
        };

        PXF32 Data[4];
    };
}
PXEulerAngle4F32;

PXPublic void PXAPI PXEulerAngle3F32ToMatrix4x4(const PXEulerAngle3F32 PXREF pxEulerAngle3F32, PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXEulerAngle4F32ToMatrix4x4(const PXEulerAngle4F32 PXREF pxEulerAngle3F32, PXMatrix4x4F PXREF pxMatrix4x4);