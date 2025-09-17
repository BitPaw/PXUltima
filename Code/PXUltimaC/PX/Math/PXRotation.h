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
            float Yaw;
            float Pitch;
            float Roll;
        };

        struct
        {
            float X;
            float Y;
            float Z;
        };

        float Data[3];
    };
}
PXEulerAngleF32;



typedef struct PXEulerAngle4F32_
{
    union
    {
        struct
        {
            float Yaw;
            float Pitch;
            float Roll;
            float Skew;
        };

        struct
        {
            float X;
            float Y;
            float Z;
            float W;
        };

        float Data[4];
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
            float Lambda;
            float I;
            float J;
            float K;
        };

        struct
        {
            float W;
            float X;
            float Y;
            float Z;
        };

        float Data[4];
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
    float Scalar;  // cos(?)

    union
    {
        float* PlaneAngleList;    // sin(?) · plane components (size = n(n-1)/2)
        float PlaneAngles[PXRotorNormalDimension];
    };
}
PXRotor;



typedef struct PXMatrix4x4F_ PXMatrix4x4F;


PXPublic PXI8U PXAPI PXRotorAnglesCount(const PXI8U amountOfDimensions);

PXPublic void PXAPI PXRotorFromEuler(PXRotor* const pxRotor, PXEulerAngleF32* const pxEulerAngleF32);
PXPublic void PXAPI PXRotorInterpolate(PXRotor* const pxRotorCurrent, const PXRotor* pxRotorTarget, const PXF32 t);

PXPublic void PXAPI PXRotorToMatrix4x4(PXRotor* const pxRotor, PXMatrix4x4F* const pxMatrix4x4);





PXPublic void PXAPI PXQuaternionToMatrix4x4(const PXQuaternionF32* quaternionF32, PXMatrix4x4F* const pxMatrix4x4);
PXPublic void PXAPI PXQuaternionFromEulerRAD(PXQuaternionF32* const pxQuaternionF32, PXEulerAngleF32* const pxEulerAngleF32);
PXPublic void PXAPI PXQuaternionFromEulerDEG(PXQuaternionF32* const pxQuaternionF32, PXEulerAngleF32* const pxEulerAngleF32);

PXPublic void PXAPI PXQuaternionToEulerRAD(PXQuaternionF32* const pxQuaternionF32, PXEulerAngleF32* const pxEulerAngleF32);

PXPublic PXF32 PXAPI PXQuaternionDot(const PXQuaternionF32* const a, const PXQuaternionF32* const b);
PXPublic void PXAPI PXQuaternionNormal(PXQuaternionF32* const pxQuaternionF32);
PXPublic void PXAPI PXQuaternionConjugate(PXQuaternionF32* const pxQuaternionF32);

PXPublic void PXAPI PXQuaternionLerp
(
    PXQuaternionF32* const a, 
    const PXQuaternionF32* const b,
    const t
);





#endif