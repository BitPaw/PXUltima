#pragma once

#ifndef PXRotationIncluded
#define PXRotationIncluded

#include <PX/Type/PXType.h>
#include <PX/Math/PXVector.h>
#include <PX/Type/PXEulerAngle.h>
#include <PX/Math/PXMatrix.h>


#define PXRotor2DDimensions 2
#define PXRotor3DDimensions 3
#define PXRotor4DDimensions 6+1 // +1 Not nessesary
#define PXRotorNormalDimension 6


// For 2D
#define PXRotorAxisXY 0
#define PXRotorAxisYZ 1

// For +3D
#define PXRotorAxisZX 2 // 

// For +4D
#define PXRotorAxisXW 3
#define PXRotorAxisYW 4
#define PXRotorAxisZW 5
#define PXRotorAxisXYZW 6// Extra plane, used as overflow for stacked 4D operations


// 2D =  1x (XY)
// 3D =  3x (XY, XZ; YZ)
// 4D =  6x (XY, XZ, XW, YZ, YW, ZW)
// 5D = 10x (XY, XZ, XW, YZ, YW, ZW, YV, ZV, WV, XV)


// A generic Quaternion that is not limited by any amount of dimensions
typedef struct PXRotorF32_
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
PXRotorF32;

typedef struct PXRotor2DF32_
{
    PXF32 Scalar;
    PXF32 XY;
    PXF32 YZ;
}
PXRotor2DF32;

typedef struct PXRotor3DF32_
{
    PXF32 Scalar;
    PXF32 XY;
    PXF32 YZ;
    PXF32 ZX;
}
PXRotor3DF32;

typedef struct PXRotor4DF32_
{
    PXF32 Scalar;
    PXF32 XY;
    PXF32 YZ;
    PXF32 ZX;
    PXF32 XW;
    PXF32 YW;
    PXF32 ZW;
    PXF32 XYZW; // Extra plane, used as overflow for stacked 4D operations
}
PXRotor4DF32;

PXPublic PXI8U PXAPI PXRotorAnglesCount(const PXI8U amountOfDimensions);

PXPublic void PXAPI PXRotorFromEuler(PXRotorF32 PXREF pxRotor, PXEulerAngle3F32 PXREF pxEulerAngleF32);
PXPublic void PXAPI PXRotorInterpolate(PXRotorF32 PXREF pxRotorCurrent, const PXRotorF32* pxRotorTarget, const PXF32 t);

PXPublic void PXAPI PXRotorNDF32ToMatrix4x4F32
(
    PXVec2I8U* rotorPlanes,
    PXF32 scalar,
    PXSize planeAngleAmount,
    PXF32 PXREF planeAngleList,
    PXMatrix4x4F PXREF pxMatrix4x4
);


//---------------------------------------------------------
PXPublic void PXAPI PXRotor3DF32ToMatrix4x4F32(PXRotor3DF32 PXREF pxRotor3D, PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXRotor3DF32RotateVector(const PXRotor3DF32 PXREF pxRotor3D, const PXVector4F32 PXREF vec, PXVector4F32 PXREF result);
PXPublic void PXAPI PXRotor3DF32Identity(PXRotor3DF32 PXREF pxRotor3D);
PXPublic void PXAPI PXRotor3DF32Set(PXRotor3DF32 PXREF pxRotor3DA, const PXRotor3DF32 PXREF pxRotor3DB);
PXPublic void PXAPI PXRotor3DF32Multiply(PXRotor3DF32 PXREF pxRotor3D, const PXRotor3DF32 PXREF pxRotor3DB);
PXPublic PXF32 PXAPI PXRotor3DF32Length(const PXRotor3DF32 PXREF pxRotor3D);
PXPublic PXF32 PXAPI PXRotor3DF32LengthSquared(const PXRotor3DF32 PXREF pxRotor3D);
PXPublic void PXAPI PXRotor3DF32Normalize(PXRotor3DF32 PXREF pxRotor3D);
PXPublic void PXAPI PXRotor3DFromAngleAndPlane(PXRotor3DF32 PXREF pxRotor3D, PXF32 angleRadians, PXF32 planeXY, PXF32 planeYZ, PXF32 planeZX);
//---------------------------------------------------------



//---------------------------------------------------------
// Rotor 4D
//---------------------------------------------------------
PXPublic void PXAPI PXRotor4DF32ToMatrix4x4F32(PXRotor4DF32 PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4);
//PXPublic void PXAPI PXRotor4DF32ToMatrix4x4F32_2(PXRotor4DF32 PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXRotor4DF32RotateVector(const PXRotor4DF32 PXREF rotor, const PXVector4F32 PXREF vec, PXVector4F32 PXREF result);
PXPublic void PXAPI PXRotor4DF32Identity(PXRotor4DF32 PXREF rotor);
PXPublic void PXAPI PXRotor4DF32Set(PXRotor4DF32 PXREF rotor, const PXRotor4DF32 PXREF rotorB);
PXPublic void PXAPI PXRotor4DF32Multiply(PXRotor4DF32 PXREF rotor, const PXRotor4DF32 PXREF rotorB);
PXPublic PXF32 PXAPI PXRotor4DF32Length(const PXRotor4DF32 PXREF pxRotor4DF32);
PXPublic PXF32 PXAPI PXRotor4DF32LengthSquared(const PXRotor4DF32 PXREF pxRotor4DF32);
PXPublic void PXAPI PXRotor4DF32Normalize(PXRotor4DF32 PXREF pxRotor4DF32);
PXPublic void PXAPI PXRotor4DFromAngleAndPlane(PXRotor4DF32 PXREF pxRotor4D, PXF32 angleRadians, PXF32 planeXY, PXF32 planeYZ, PXF32 planeZX);
//---------------------------------------------------------

#endif