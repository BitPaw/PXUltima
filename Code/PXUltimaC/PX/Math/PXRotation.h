#pragma once

#ifndef PXRotationIncluded
#define PXRotationIncluded

#include <PX/Media/PXType.h>


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
PXEulerAngleF32;



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




#define PXRotorNormalDimension 6

// A generic Quaternion that is not limited by any amount of dimensions
typedef struct PXRotor_
{
    // How many dimensions we can rotate with
    // 1D = Invalid
    // 2D =  1x (XY)
    // 3D =  3x (XY, XZ; YZ)
    // 4D =  6x (XY, XZ, XW, YZ, YW, ZW)
    // 5D = 10x (XY, XZ, XW, YZ, YW, ZW, YV, ZV, WV, XV)
    // nD = (n(n-1))/2
    PXI32U Dimension;
    PXF32 Scalar;  // cos(?)

    union
    {
        PXF32* PlaneAngleList;    // sin(?) · plane components (size = n(n-1)/2)
        PXF32 PlaneAngles[PXRotorNormalDimension];
    };
}
PXRotor;



typedef struct PXMatrix4x4F_ PXMatrix4x4F;


PXPublic PXI8U PXAPI PXRotorAnglesCount(const PXI8U amountOfDimensions);

PXPublic void PXAPI PXRotorFromEuler(PXRotor PXREF pxRotor, PXEulerAngleF32 PXREF pxEulerAngleF32);
PXPublic void PXAPI PXRotorInterpolate(PXRotor PXREF pxRotorCurrent, const PXRotor* pxRotorTarget, const PXF32 t);

PXPublic void PXAPI PXRotorToMatrix4x4(PXRotor PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4);





PXPublic void PXAPI PXQuaternionToMatrix4x4(const PXQuaternionF32* quaternionF32, PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXQuaternionFromEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32);
PXPublic void PXAPI PXQuaternionFromEulerDEG(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32);

PXPublic void PXAPI PXQuaternionToEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32);

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