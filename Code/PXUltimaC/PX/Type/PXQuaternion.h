#pragma once

#pragma once

#ifndef PXQuaternionIncluded
#define PXQuaternionIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXEulerAngle.h>
#include <PX/Math/PXMatrix.h>

// 4D Object for more acurate 3D rotation to prevent gimblelock.
typedef struct PXQuaternionF32_
{
    union
    {
        struct
        {
            PXF32 Lambda;
            PXF32 I;
            PXF32 J;
            PXF32 K;
        };

        struct
        {
            PXF32 W;
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
        };

        PXF32 Data[4];
    };
}
PXQuaternionF32;

PXPublic void PXAPI PXQuaternionToMatrix4x4(const PXQuaternionF32 PXREF quaternionF32, PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXQuaternionFromEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngle3F32 PXREF pxEulerAngleF32);
PXPublic void PXAPI PXQuaternionFromEulerDEG(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngle3F32 PXREF pxEulerAngleF32);

PXPublic void PXAPI PXQuaternionToEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngle3F32 PXREF pxEulerAngleF32);

PXPublic PXF32 PXAPI PXQuaternionDot(const PXQuaternionF32 PXREF a, const PXQuaternionF32 PXREF b);
PXPublic void PXAPI PXQuaternionNormal(PXQuaternionF32 PXREF pxQuaternionF32);
PXPublic void PXAPI PXQuaternionConjugate(PXQuaternionF32 PXREF pxQuaternionF32);

PXPublic void PXAPI PXQuaternionLerp
(
    PXQuaternionF32 PXREF a,
    const PXQuaternionF32 PXREF b,
    const PXF32 t
);

#endif