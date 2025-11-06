#include "PXRotation.h"

#include <PX/Math/PXMath.h>
#include <PX/Math/PXMatrix.h>

PXI8U PXAPI PXRotorAnglesCount(const PXI8U amountOfDimensions)
{
    return amountOfDimensions * (amountOfDimensions - 1) / 2;
}

void PXAPI PXRotorFromEuler(PXRotor PXREF pxRotor, PXEulerAngleF32 PXREF pxEulerAngleF32)
{
    PXF32 angle = (pxEulerAngleF32->Pitch + pxEulerAngleF32->Yaw + pxEulerAngleF32->Roll) / 3.0f;

    pxRotor->Dimension = 3;
    pxRotor->Scalar = PXMathCosinusRADF32(angle);

    pxRotor->PlaneAngles[0] = PXMathSinusRADF32(angle); // XY
    pxRotor->PlaneAngles[1] = 0.0f; // YZ
    pxRotor->PlaneAngles[2] = 0.0f; // ZX
}

void PXAPI PXRotorInterpolate(PXRotor PXREF pxRotorCurrent, const PXRotor* pxRotorTarget, const PXF32 t)
{
    pxRotorCurrent->Scalar = PXMathLerpF32H(pxRotorCurrent->Scalar, pxRotorTarget->Scalar, t);

    for(PXI32U i = 0; i < pxRotorCurrent->Dimension; i++)
    {
        pxRotorCurrent->PlaneAngles[i] = PXMathLerpF32H
        (
            pxRotorCurrent->PlaneAngles[i],
            pxRotorTarget->PlaneAngles[i],
            t
        );
    }
}


// 2D =  1x (XY)
// 3D =  3x (XY, XZ; YZ)
// 4D =  6x (XY, XZ, XW, YZ, YW, ZW)
// 5D = 10x (XY, XZ, XW, YZ, YW, ZW, YV, ZV, WV, XV)

#define PXRotorPlaneX 0
#define PXRotorPlaneY 1
#define PXRotorPlaneZ 2
#define PXRotorPlaneW 3
#define PXRotorPlaneV 4

const PXVec2I8U PXRotorPlanes2D[] =
{
    {PXRotorPlaneX,PXRotorPlaneY},
};
const PXVec2I8U PXRotorPlanes3D[] =
{
    {PXRotorPlaneX,PXRotorPlaneY},
    {PXRotorPlaneX,PXRotorPlaneZ},
    {PXRotorPlaneY,PXRotorPlaneZ}
};
const PXVec2I8U PXRotorPlanes4D[] =
{
    {PXRotorPlaneX,PXRotorPlaneY},
    {PXRotorPlaneX,PXRotorPlaneZ},
    {PXRotorPlaneX,PXRotorPlaneW},
    {PXRotorPlaneY,PXRotorPlaneZ},
    {PXRotorPlaneY,PXRotorPlaneW},
    {PXRotorPlaneZ,PXRotorPlaneW}
};
const PXVec2I8U* PXRotorPlanes[] =
{
    PXRotorPlanes2D,
    PXRotorPlanes3D,
    PXRotorPlanes4D
};

void PXAPI PXRotorToMatrix4x4(PXRotor PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4)
{   
    PXVec2I8U* planes = PXRotorPlanes[pxRotor->Dimension];

    PXMatrix4x4FIdentity(pxMatrix4x4);

    for(PXI8U i = 0; i < pxRotor->Dimension; ++i)
    {
        int a = planes->X;
        int b = planes->Y;

        float c = pxRotor->Scalar;
        float s = pxRotor->PlaneAngles[i];
        float temp_a = pxMatrix4x4->DataXY[a][a] * c - pxMatrix4x4->DataXY[a][b] * s;
        float temp_b = pxMatrix4x4->DataXY[b][a] * s + pxMatrix4x4->DataXY[b][b] * c;
        pxMatrix4x4->DataXY[a][a] = temp_a;
        pxMatrix4x4->DataXY[b][b] = temp_b;
        pxMatrix4x4->DataXY[a][b] = -s;
        pxMatrix4x4->DataXY[b][a] = s;
    }
}

void PXAPI PXQuaternionToMatrix4x4(const PXQuaternionF32* quaternionF32, PXMatrix4x4F PXREF pxMatrix4x4)
{
    float w = quaternionF32->W;
    float x = quaternionF32->X;
    float y = quaternionF32->Y;
    float z = quaternionF32->Z;

    PXMatrix4x4FIdentity(pxMatrix4x4);

    pxMatrix4x4->DataXY[0][0] = 1 - 2 * y * y - 2 * z * z;
    pxMatrix4x4->DataXY[0][1] = 2 * x * y - 2 * z * w;
    pxMatrix4x4->DataXY[0][2] = 2 * x * z + 2 * y * w;

    pxMatrix4x4->DataXY[1][0] = 2 * x * y + 2 * z * w;
    pxMatrix4x4->DataXY[1][1] = 1 - 2 * x * x - 2 * z * z;
    pxMatrix4x4->DataXY[1][2] = 2 * y * z - 2 * x * w;

    pxMatrix4x4->DataXY[2][0] = 2 * x * z - 2 * y * w;
    pxMatrix4x4->DataXY[2][1] = 2 * y * z + 2 * x * w;
    pxMatrix4x4->DataXY[2][2] = 1 - 2 * x * x - 2 * y * y;
}

void PXAPI PXQuaternionFromEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32)
{
    PXEulerAngleF32 halfAngle;
    halfAngle.Yaw = pxEulerAngleF32->Yaw * 0.5f;
    halfAngle.Pitch = pxEulerAngleF32->Pitch * 0.5f;
    halfAngle.Roll = pxEulerAngleF32->Roll * 0.5f;


    struct
    {
        union
        {
            struct
            {
                PXF32 sy; // SIN
                PXF32 cy; // COS
             
                PXF32 sp; // SIN
                PXF32 cp; // COS
           
                PXF32 sr; // SIN
                PXF32 cr; // COS
            };

            PXF32 Data[6];
        };
    }
    data;

    PXMathSinCosRADF32V(data.Data, 6);

    pxQuaternionF32->W = data.cr * data.cp * data.cy + data.sr * data.sp * data.sy; // w
    pxQuaternionF32->X = data.sr * data.cp * data.cy - data.cr * data.sp * data.sy; // x
    pxQuaternionF32->Y = data.cr * data.sp * data.cy + data.sr * data.cp * data.sy; // y
    pxQuaternionF32->Z = data.cr * data.cp * data.sy - data.sr * data.sp * data.cy; // z
}

void PXAPI PXQuaternionFromEulerDEG(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32)
{
    PXEulerAngleF32 halfAngle = *pxEulerAngleF32;
    halfAngle.Yaw = PXMathDegreeToRadians(halfAngle.Yaw);
    halfAngle.Pitch = PXMathDegreeToRadians(halfAngle.Pitch);
    halfAngle.Roll = PXMathDegreeToRadians(halfAngle.Roll);

    PXQuaternionFromEulerRAD(pxQuaternionF32, &halfAngle);
}

void PXAPI PXQuaternionToEulerRAD(PXQuaternionF32 PXREF pxQuaternionF32, PXEulerAngleF32 PXREF pxEulerAngleF32)
{
    float w = pxQuaternionF32->W;
    float x = pxQuaternionF32->X;
    float y = pxQuaternionF32->Y;
    float z = pxQuaternionF32->Z;

    // Roll (X-axis rotation)
    float sinr_cosp = 2 * (w * x + y * z);
    float cosr_cosp = 1 - 2 * (x * x + y * y);
    pxEulerAngleF32->Roll = PXMathArcusTangens2(sinr_cosp, cosr_cosp);    

    // Pitch (Y-axis rotation)
    float sinp = 2 * (w * y - z * x);
    if(PXMathAbsoluteF(sinp) >= 1)
        pxEulerAngleF32->Pitch = PXMathCopysignf(3.14 / 2, sinp); // use 90° if out of range
    else
        pxEulerAngleF32->Pitch = PXMathArcusSinus(sinp);

    // Yaw (Z-axis rotation)
    float siny_cosp = 2 * (w * z + x * y);
    float cosy_cosp = 1 - 2 * (y * y + z * z);

    pxEulerAngleF32->Yaw = PXMathArcusTangens2(siny_cosp, cosy_cosp);
}

PXF32 PXAPI PXQuaternionDot(const PXQuaternionF32 PXREF a, const PXQuaternionF32 PXREF b)
{
    return a->W * b->W + a->X * b->X + a->Y * b->Y + a->Z * b->Z;
}

void PXAPI PXQuaternionNormal(PXQuaternionF32 PXREF pxQuaternionF32)
{
    float dot = PXQuaternionDot(pxQuaternionF32, pxQuaternionF32);
    float mag = PXMathRootSquareF32(dot);

    pxQuaternionF32->W /= mag;
    pxQuaternionF32->X /= mag;
    pxQuaternionF32->Y /= mag;
    pxQuaternionF32->Z /= mag;
}

void PXAPI PXQuaternionConjugate(PXQuaternionF32 PXREF pxQuaternionF32)
{
    pxQuaternionF32->W = +pxQuaternionF32->W;
    pxQuaternionF32->X = -pxQuaternionF32->X;
    pxQuaternionF32->Y = -pxQuaternionF32->Y;
    pxQuaternionF32->Z = -pxQuaternionF32->Z;
}

void PXAPI PXQuaternionLerp
(
    PXQuaternionF32 PXREF a,
    const PXQuaternionF32 PXREF b, 
    const PXF32 t
)
{
    PXQuaternionF32 res;
    PXF32 dot = PXQuaternionDot(a, b);

    // If dot < 0, negate one quaternion to take the shortest path
    PXQuaternionF32 q2copy = *b;
    if(dot < 0.0f)
    {
        dot = -dot;

        q2copy.W *= -1;
        q2copy.X *= -1;
        q2copy.Y *= -1;
        q2copy.Z *= -1;
    }

    if(dot > 0.9995f)
    {
        // Linear interpolation for very close quaternions
        for(int i = 0; i < 4; ++i)
        {
            res.Data[i] = PXMathLerpF32H(a->Data[i], q2copy.Data[i], t);
        }
    }
    else
    { 
        float theta_0 = PXMathArcusCosinusF32(dot);
        float theta = theta_0 * t;
        float sin_theta = PXMathSinusRADF32(theta);
        float sin_theta_0 = PXMathSinusRADF32(theta_0);        

        float s0 = PXMathCosinusRADF32(theta) - dot * sin_theta / sin_theta_0;
        float s1 = sin_theta / sin_theta_0;

        for(int i = 0; i < 4; ++i)
        {
            res.Data[i] = s0 * a->Data[i] + s1 * q2copy.Data[i];
        }
    }

    // Optional: normalize result
    PXQuaternionNormal(&res);

    // Store
    *a = res;
}
