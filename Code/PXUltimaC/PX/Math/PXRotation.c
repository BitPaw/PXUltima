#include "PXRotation.h"

#include <PX/Math/PXMath.h>
#include <PX/Math/PXMatrix.h>
#include <math.h>

PXI8U PXAPI PXRotorAnglesCount(const PXI8U amountOfDimensions)
{
    return amountOfDimensions * (amountOfDimensions - 1) / 2;
}

void PXAPI PXRotorFromEuler(PXRotorF32 PXREF pxRotor, PXEulerAngle3F32 PXREF pxEulerAngleF32)
{
    PXF32 angle = (pxEulerAngleF32->Pitch + pxEulerAngleF32->Yaw + pxEulerAngleF32->Roll) / 3.0f;

    pxRotor->Dimension = 3;
    pxRotor->Scalar = PXMathCosinusRADF32(angle);

    pxRotor->PlaneAngles[0] = PXMathSinusRADF32(angle); // XY
    pxRotor->PlaneAngles[1] = 0.0f; // YZ
    pxRotor->PlaneAngles[2] = 0.0f; // ZX
}

void PXAPI PXRotorInterpolate(PXRotorF32 PXREF pxRotorCurrent, const PXRotorF32* pxRotorTarget, const PXF32 t)
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

void PXAPI PXRotorNDF32ToMatrix4x4F32
(
    PXVec2I8U* rotorPlanes,
    PXF32 scalar,
    PXSize planeAngleAmount,
    PXF32 PXREF planeAngleList,
    PXMatrix4x4F PXREF pxMatrix4x4
)
{
    PXMatrix4x4FIdentity(pxMatrix4x4);

    for(PXI8U i = 0; i < planeAngleAmount; ++i)
    {
        int a = rotorPlanes->X;
        int b = rotorPlanes->Y;

        PXF32 c = scalar;
        PXF32 s = planeAngleList[i];
        PXF32 temp_a = pxMatrix4x4->DataXY[a][a] * c - pxMatrix4x4->DataXY[a][b] * s;
        PXF32 temp_b = pxMatrix4x4->DataXY[b][a] * s + pxMatrix4x4->DataXY[b][b] * c;
        pxMatrix4x4->DataXY[a][a] = temp_a;
        pxMatrix4x4->DataXY[b][b] = temp_b;
        pxMatrix4x4->DataXY[a][b] = -s;
        pxMatrix4x4->DataXY[b][a] = s;
    }
}

void PXAPI PXRotor3DF32ToMatrix4x4F32(PXRotor3DF32 PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4)
{
   // PXRotorNDF32ToMatrix4x4F32(PXRotorPlanes3D, pxRotor->Scalar, PXRotor3DDimensions, pxRotor->PlaneAngles, pxMatrix4x4);
}

void PXAPI PXRotor3DF32RotateVector(const PXRotor3DF32 PXREF pxRotor3D, const PXVector4F32 PXREF vec, PXVector4F32 PXREF result)
{

}

void PXAPI PXRotor3DF32Identity(PXRotor3DF32 PXREF pxRotor3D)
{

}

void PXAPI PXRotor3DF32Set(PXRotor3DF32 PXREF pxRotor3DA, const PXRotor3DF32 PXREF pxRotor3DB)
{
    pxRotor3DA->Scalar = pxRotor3DB->Scalar;
    pxRotor3DA->XY = pxRotor3DB->XY;
    pxRotor3DA->YZ = pxRotor3DB->YZ;
    pxRotor3DA->ZX = pxRotor3DB->ZX;
}

void PXAPI PXRotor3DF32Multiply(PXRotor3DF32 PXREF pxRotor3D, const PXRotor3DF32 PXREF pxRotor3DB)
{
    pxRotor3D->Scalar += pxRotor3DB->Scalar;
    pxRotor3D->XY += pxRotor3DB->XY;
    pxRotor3D->YZ += pxRotor3DB->YZ;
    pxRotor3D->ZX += pxRotor3DB->ZX;
}

PXF32 PXAPI PXRotor3DF32Length(const PXRotor3DF32 PXREF pxRotor3D)
{
    if(!pxRotor3D)
    {
        return 1.0f;
    }

    PXF32 x = PXRotor3DF32LengthSquared(pxRotor3D);

    return PXMathRootSquareF32(x);
}

PXF32 PXAPI PXRotor3DF32LengthSquared(const PXRotor3DF32 PXREF pxRotor3D)
{
    return
        (pxRotor3D->Scalar * pxRotor3D->Scalar) +
        (pxRotor3D->XY * pxRotor3D->XY) +
        (pxRotor3D->YZ * pxRotor3D->YZ) +
        (pxRotor3D->ZX * pxRotor3D->ZX);
}

void PXAPI PXRotor3DF32Normalize(PXRotor3DF32 PXREF pxRotor3D)
{
    // Calculate Length L first
    PXF32 length = PXRotor3DF32Length(pxRotor3D);

    if(length < 1.0e-6f) // Check against small epsilon for zero rotor
    {
        // Identity rotation, set output to zero vector
        pxRotor3D->Scalar = 0.0f;
        pxRotor3D->XY = 0.0f;
        pxRotor3D->YZ = 0.0f;
        pxRotor3D->ZX = 0.0f;
        return length; // Returns 0.0f length
    }

    // Normalize by dividing all components by the length L
    pxRotor3D->Scalar /= length;
    pxRotor3D->XY /= length;
    pxRotor3D->YZ /= length;
    pxRotor3D->ZX /= length;

    return length; // Return the original length
}

void PXAPI PXRotor3DFromAngleAndPlane(PXRotor3DF32 PXREF pxRotor3D, PXF32 angleRadians, PXF32 planeXY, PXF32 planeYZ, PXF32 planeZX)
{

}

void PXAPI PXRotor4DF32ToMatrix4x4F32(PXRotor4DF32 PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix4x4)
{

    // 1. Define the pure unrotated 4D unit basis axes
    PXVector4F32 baseX = { 1.0f, 0.0f, 0.0f, 0.0f };
    PXVector4F32 baseY = { 0.0f, 1.0f, 0.0f, 0.0f };
    PXVector4F32 baseZ = { 0.0f, 0.0f, 1.0f, 0.0f };
    PXVector4F32 baseW = { 0.0f, 0.0f, 0.0f, 1.0f };

    // 2. Rotate all four 4D basis axes via the sandwich product
    PXVector4F32 rotX, rotY, rotZ, rotW;
    PXRotor4DF32RotateVector(pxRotor, &baseX, &rotX);
    PXRotor4DF32RotateVector(pxRotor, &baseY, &rotY);
    PXRotor4DF32RotateVector(pxRotor, &baseZ, &rotZ);
    PXRotor4DF32RotateVector(pxRotor, &baseW, &rotW);

    // 3. Map the rotated axes directly into your 4x4 matrix data layout indices.
    // If your matrix data is an array of 16 floats, map them directly to their slot definitions:

    // Column 0: Rotated X-basis
    pxMatrix4x4->Column.X.Right = rotX.X;
    pxMatrix4x4->Column.Y.Right = rotX.Y;
    pxMatrix4x4->Column.Z.Right = rotX.Z;
    pxMatrix4x4->Column.W.Right = rotX.W;

    // Column 1: Rotated Y-basis
    pxMatrix4x4->Column.X.Up = rotY.X;
    pxMatrix4x4->Column.Y.Up = rotY.Y;
    pxMatrix4x4->Column.Z.Up = rotY.Z;
    pxMatrix4x4->Column.W.Up = rotY.W;

    // Column 2: Rotated Z-basis
    pxMatrix4x4->Column.X.Forward = rotZ.X;
    pxMatrix4x4->Column.Y.Forward = rotZ.Y;
    pxMatrix4x4->Column.Z.Forward = rotZ.Z;
    pxMatrix4x4->Column.W.Forward = rotZ.W;

    // Column 3: Rotated W-basis
    pxMatrix4x4->Column.X.Translation = rotW.X;
    pxMatrix4x4->Column.Y.Translation = rotW.Y;
    pxMatrix4x4->Column.Z.Translation = rotW.Z;
    pxMatrix4x4->Column.W.Translation = rotW.W;

    PXRotor4DF32Normalize(pxMatrix4x4);
}
/*
void PXAPI PXRotor4DF32ToMatrix4x4F32_2(PXRotor4DF32 PXREF pxRotor, PXMatrix4x4F PXREF pxMatrix)
{
    float s = pxRotor->Scalar;
    float xy = pxRotor->PlaneAngles[PXRotorAxisXY];
    float yz = pxRotor->PlaneAngles[PXRotorAxisYZ];
    float zx = pxRotor->PlaneAngles[PXRotorAxisZX];
    float xw = pxRotor->PlaneAngles[PXRotorAxisXW];
    float yw = pxRotor->PlaneAngles[PXRotorAxisYW];
    float zw = pxRotor->PlaneAngles[PXRotorAxisZW];

    // Pre?compute some repeated terms
    float s2 = s * s;
    float xy2 = xy * xy, yz2 = yz * yz, zx2 = zx * zx;
    float xw2 = xw * xw, yw2 = yw * yw, zw2 = zw * zw;

    // Column 0 – rotated X basis
    pxMatrix->Data[0] = s2 + xy2 - yz2 - zx2 - xw2 - yw2 - zw2;
    pxMatrix->Data[1] = 2.0f * (xy * yz - zx * xw);
    pxMatrix->Data[2] = 2.0f * (xy * zx + yz * xw);
    pxMatrix->Data[3] = 2.0f * (s * xw + yz * yw - s * zx);

    // Column 1 – rotated Y basis 
    pxMatrix->Data[4] = 2.0f * (xy * yz + zx * xw);
    pxMatrix->Data[5] = s2 - xy2 + yz2 - zx2 - xw2 - yw2 - zw2;
    pxMatrix->Data[6] = 2.0f * (yz * zx - xy * xw);
    pxMatrix->Data[7] = 2.0f * (s * yw - xy * zw - s * yz);

    // Column 2 – rotated Z basis
    pxMatrix->Data[8] = 2.0f * (xy * zx - yz * xw);
    pxMatrix->Data[9] = 2.0f * (yz * zx + xy * xw);
    pxMatrix->Data[10] = s2 - xy2 - yz2 + zx2 - xw2 - yw2 - zw2;
    pxMatrix->Data[11] = 2.0f * (s * zw + xy * yw - s * zx);

    // Column 3 – rotated W basis
    pxMatrix->Data[12] = 2.0f * (s * xw - yz * yw + xy * zw);
    pxMatrix->Data[13] = 2.0f * (s * yw + xy * zw - yz * xw);
    pxMatrix->Data[14] = 2.0f * (s * zw - xy * yw + yz * xw);
    pxMatrix->Data[15] = s2 - xy2 - yz2 - zx2 + xw2 + yw2 + zw2;
}
*/
/*
 const PXRotor4DF32 PXREF pxRotor = &pxTransform4D->Rotor;
    PXVector4F32* eye = &pxTransform4D->Position;

    float s = pxRotor->Scalar;
    float xy = pxRotor->PlaneAngles[PXRotorAxisXY]; // Roll
    float yz = pxRotor->PlaneAngles[PXRotorAxisYZ]; // Pitch
    float zx = pxRotor->PlaneAngles[PXRotorAxisZX]; // Yaw

    // Enforce normalization scaling to prevent squashing/distortion
    float lengthSq = s * s + xy * xy + yz * yz + zx * zx;
    float num = (lengthSq > 1e-6f) ? (2.0f / lengthSq) : 2.0f;

    // Precalculate standard 3D rotor matrix coefficients
    float sxy = s * xy;   float syz = s * yz;   float szx = s * zx;
    float xx = xy * xy;   float yy = yz * yz;   float zz = zx * zx;
    float xyyz = xy * yz; float xyzx = xy * zx; float yzzx = yz * zx;

    // --- Column 1: Right Vector (X Basis) ---
    pxMatrix->Data[XAxisX] = 1.0f - num * (yy + zz);
    pxMatrix->Data[XAxisY] = num * (xyyz - szx);
    pxMatrix->Data[XAxisZ] = num * (xyzx + syz);
    pxMatrix->Data[XAxisW] = 0.0f;

    // --- Column 2: Up Vector (Y Basis) ---
    pxMatrix->Data[YAxisX] = num * (xyyz + szx);
    pxMatrix->Data[YAxisY] = 1.0f - num * (xx + zz);
    pxMatrix->Data[YAxisZ] = num * (yzzx - sxy);
    pxMatrix->Data[YAxisW] = 0.0f;

    // --- Column 3: Forward Vector (Z Basis - Face down -Z for Right-Handed) ---
    pxMatrix->Data[ZAxisX] = -num * (xyzx - syz);
    pxMatrix->Data[ZAxisY] = -num * (yzzx + sxy);
    pxMatrix->Data[ZAxisZ] = -(1.0f - num * (xx + yy));
    pxMatrix->Data[ZAxisW] = 0.0f;

    // --- Column 4: Base Structural Element ---
    pxMatrix->Data[WAxisX] = 0.0f;
    pxMatrix->Data[WAxisY] = 0.0f;
    pxMatrix->Data[WAxisZ] = 0.0f;
    pxMatrix->Data[WAxisW] = 1.0f;

    // --- Right-Handed View Space Position Translation ---
    // The Z vector multiplier is positive to balance the inverted forward column!
    pxMatrix->Data[TransformX] = -eye->X;
    pxMatrix->Data[TransformY] = -eye->Y;
    pxMatrix->Data[TransformZ] = -eye->Z;
    pxMatrix->Data[TransformX] = -(pxMatrix->Data[XAxisX] * eye->X + pxMatrix->Data[XAxisY] * eye->Y + pxMatrix->Data[XAxisZ] * eye->Z);
    pxMatrix->Data[TransformY] = -(pxMatrix->Data[YAxisX] * eye->X + pxMatrix->Data[YAxisY] * eye->Y + pxMatrix->Data[YAxisZ] * eye->Z);
    pxMatrix->Data[TransformZ] = (pxMatrix->Data[ZAxisX] * eye->X + pxMatrix->Data[ZAxisY] * eye->Y + pxMatrix->Data[ZAxisZ] * eye->Z);
    //pxMatrix->Data[TransformX] = -(pxMatrix->Data[XAxisX] * eye->X + pxMatrix->Data[XAxisY] * eye->Y + pxMatrix->Data[XAxisZ] * eye->Z);
    //pxMatrix->Data[TransformY] = -(pxMatrix->Data[YAxisX] * eye->X + pxMatrix->Data[YAxisY] * eye->Y + pxMatrix->Data[YAxisZ] * eye->Z);
    //pxMatrix->Data[TransformZ] = (pxMatrix->Data[ZAxisX] * eye->X + pxMatrix->Data[ZAxisY] * eye->Y + pxMatrix->Data[ZAxisZ] * eye->Z);
    pxMatrix->Data[TransformW] = 1.0f;
*/

void PXAPI PXRotor4DF32RotateVector(const PXRotor4DF32 PXREF pxRotor, const PXVector4F32 PXREF vec, PXVector4F32 PXREF result)
{
    if(!(pxRotor && vec && result)) 
        return;

    // Cache local inputs to ensure stability
    PXF32 vX = vec->X;
    PXF32 vY = vec->Y;
    PXF32 vZ = vec->Z;
    PXF32 vW = vec->W;

    PXF32 s = pxRotor->Scalar;
    PXF32 xy = pxRotor->XY;
    PXF32 yz = pxRotor->YZ;
    PXF32 zx = pxRotor->ZX;
    PXF32 xw = pxRotor->XW;
    PXF32 yw = pxRotor->YW;
    PXF32 zw = pxRotor->ZW;
    PXF32 xyzw = pxRotor->XYZW;

    // 1. Intermediate geometric product: temp = rotor * vec
    PXF32 t_X = s * vX + xy * vY - zx * vZ - xw * vW;
    PXF32 t_Y = -xy * vX + s * vY + yz * vZ - yw * vW;
    PXF32 t_Z = zx * vX - yz * vY + s * vZ - zw * vW;
    PXF32 t_W = xw * vX + yw * vY + zw * vZ + s * vW;

    PXF32 t_XYZ = -yz * vX - zx * vY - xy * vZ + xyzw * vW;
    PXF32 t_XYW = -yw * vX + xw * vY + xyzw * vZ - xy * vW;
    PXF32 t_YZW = xyzw * vX - zw * vY + yw * vZ - yz * vW;
    PXF32 t_ZXW = -zw * vX - xyzw * vY + xw * vZ + zx * vW;

    // 2. Final sandwich product: temp * reverse(rotor)
    // Local stack values prevent memory overwriting bugs
    PXF32 r_X = t_X * s + t_Y * xy - t_Z * zx - t_W * xw - t_XYZ * yz - t_XYW * yw - t_ZXW * zw + t_YZW * xyzw;
    PXF32 r_Y = -t_X * xy + t_Y * s + t_Z * yz - t_W * yw + t_XYZ * zx + t_XYW * xw - t_YZW * zw - t_ZXW * xyzw;
    PXF32 r_Z = t_X * zx - t_Y * yz + t_Z * s - t_W * zw - t_XYZ * xy + t_YZW * yw - t_ZXW * xw + t_XYW * xyzw;
    PXF32 r_W = t_X * xw + t_Y * yw + t_Z * zw + t_W * s - t_XYW * xy - t_YZW * yz - t_ZXW * zx - t_XYZ * xyzw;

    // 3. Commit cleanly to memory layout
    result->X = r_X;
    result->Y = r_Y;
    result->Z = r_Z;
    result->W = r_W;
}

void PXAPI PXRotor4DF32Identity(PXRotor4DF32 PXREF rotor)
{
    if(!rotor)
    {
        return;
    }

    rotor->Scalar = 1.0f;
    rotor->XY = 0.0f;
    rotor->YZ = 0.0f;
    rotor->ZX = 0.0f;
    rotor->XW = 0.0f;
    rotor->YW = 0.0f;
    rotor->ZW = 0.0f;
    rotor->XYZW = 0.0f;
}

void PXAPI PXRotor4DF32Set(PXRotor4DF32 PXREF rotor, const PXRotor4DF32 PXREF rotorB)
{
    if(!(rotor && rotorB))
    {
        return;
    }

    rotor->Scalar = rotorB->Scalar;
    rotor->XY = rotorB->XY;
    rotor->YZ = rotorB->YZ;
    rotor->ZX = rotorB->ZX;
    rotor->XW = rotorB->XW;
    rotor->YW = rotorB->YW;
    rotor->ZW = rotorB->ZW;
    rotor->XYZW = rotorB->XYZW;
}

void PXAPI PXRotor4DF32Multiply(PXRotor4DF32 PXREF rotor, const PXRotor4DF32 PXREF rotorB)
{
    if(!(rotor && rotorB))
        return;

    PXRotor4DF32 result;

    PXF32 a_s = rotor->Scalar;
    PXF32 a_xy = rotor->XY;
    PXF32 a_yz = rotor->YZ;
    PXF32 a_zx = rotor->ZX;
    PXF32 a_xw = rotor->XW;
    PXF32 a_yw = rotor->YW;
    PXF32 a_zw = rotor->ZW;
    PXF32 a_xyzw = rotor->XYZW;

    PXF32 b_s = rotorB->Scalar;
    PXF32 b_xy = rotorB->XY;
    PXF32 b_yz = rotorB->YZ;
    PXF32 b_zx = rotorB->ZX;
    PXF32 b_xw = rotorB->XW;
    PXF32 b_yw = rotorB->YW;
    PXF32 b_zw = rotorB->ZW;
    PXF32 b_xyzw = rotorB->XYZW;

#if 0
    result.Scalar = a_s * b_s - a_xy * b_xy - a_yz * b_yz - a_zx * b_zx - a_xw * b_xw - a_yw * b_yw - a_zw * b_zw - a_xyzw * b_xyzw; // FIX: sign on xyzw
    result.PlaneAngles[PXRotorAxisXY] = a_s * b_xy + a_xy * b_s + a_yz * b_zx - a_zx * b_yz - a_xw * b_yw + a_yw * b_xw + a_zw * b_xyzw - a_xyzw * b_zw; // FIX: signs aligned to basis
    result.PlaneAngles[PXRotorAxisYZ] = a_s * b_yz - a_xy * b_zx + a_yz * b_s + a_zx * b_xy - a_yw * b_zw + a_zw * b_yw + a_xw * b_xyzw - a_xyzw * b_xw; // FIX: corrected cross planes
    result.PlaneAngles[PXRotorAxisZX] = a_s * b_zx + a_xy * b_yz - a_yz * b_xy + a_zx * b_s - a_zw * b_xw + a_xw * b_zw + a_yw * b_xyzw + a_xyzw * b_yw; // FIX: FIXED bleeding terms here!
    result.PlaneAngles[PXRotorAxisXW] = a_s * b_xw + a_xy * b_yw - a_yz * b_xyzw - a_zx * b_zw + a_xw * b_s - a_yw * b_xy + a_zw * b_zx - a_xyzw * b_yz;
    result.PlaneAngles[PXRotorAxisYW] = a_s * b_yw - a_xy * b_xw + a_yz * b_zw - a_zx * b_xyzw + a_xw * b_xy + a_yw * b_s - a_zw * b_yz - a_xyzw * b_zx;
    result.PlaneAngles[PXRotorAxisZW] = a_s * b_zw - a_xy * b_xyzw - a_yz * b_yw + a_zx * b_xw + a_xw * b_zx + a_yw * b_yz + a_zw * b_s - a_xyzw * b_xy;
    result.PlaneAngles[PXRotorAxisXYZW] = a_s * b_xyzw + a_xy * b_zw + a_yz * b_xw + a_zx * b_yw + a_xw * b_yz + a_yw * b_zx + a_zw * b_xy + a_xyzw * b_s;
#elif 0
    result.Scalar = a_s * b_s - a_xy * b_xy - a_yz * b_yz - a_zx * b_zx - a_xw * b_xw - a_yw * b_yw - a_zw * b_zw + a_xyzw * b_xyzw;

    result.PlaneAngles[PXRotorAxisXY] = a_s * b_xy + a_xy * b_s - a_yz * b_zx + a_zx * b_yz + a_xw * b_yw - a_yw * b_xw - a_zw * b_xyzw - a_xyzw * b_zw;
    result.PlaneAngles[PXRotorAxisYZ] = a_s * b_yz + a_yz * b_s - a_zx * b_xy + a_xy * b_zx + a_yw * b_zw - a_zw * b_yw - a_xw * b_xyzw - a_xyzw * b_xw;
    result.PlaneAngles[PXRotorAxisZX] = a_s * b_zx + a_zx * b_s - a_xy * b_yz + a_yz * b_xy + a_zw * b_xw - a_xw * b_zw - a_yw * b_xyzw - a_xyzw * b_yw;
    result.PlaneAngles[PXRotorAxisXW] = a_s * b_xw + a_xw * b_s - a_xy * b_yw + a_yw * b_xy + a_zx * b_zw - a_zw * b_zx + a_yz * b_xyzw + a_xyzw * b_yz;
    result.PlaneAngles[PXRotorAxisYW] = a_s * b_yw + a_yw * b_s - a_yz * b_zw + a_zw * b_yz + a_xy * b_xw - a_xw * b_xy + a_zx * b_xyzw + a_xyzw * b_zx;
    result.PlaneAngles[PXRotorAxisZW] = a_s * b_zw + a_zw * b_s - a_zx * b_xw + a_xw * b_zx + a_yz * b_yw - a_yw * b_yz + a_xy * b_xyzw + a_xyzw * b_xy;
    result.PlaneAngles[PXRotorAxisXYZW] = a_s * b_xyzw + a_xyzw * b_s + a_xy * b_zw + a_zw * b_xy + a_yz * b_xw + a_xw * b_yz + a_zx * b_yw + a_yw * b_zx;
#elif 0
    // Temporary variables allow for in-place multiplication (e.g., PXRotor4DMultiply(&r1, &r1, &r2))
    PXF32 s = rotor->Scalar;
    PXF32 p0 = rotor->XY; 
    PXF32 p1 = rotor->YZ;
    PXF32 p2 = rotor->ZX;
    PXF32 p3 = rotor->XW;
    PXF32 p4 = rotor->YW;
    PXF32 p5 = rotor->ZW;
    PXF32 p6 = rotor->XYZW; // Pseudoscalar

    PXF32 bs = rotorB->Scalar;
    PXF32 bp0 = rotorB->XY;
    PXF32 bp1 = rotorB->YZ;
    PXF32 bp2 = rotorB->ZX;
    PXF32 bp3 = rotorB->XW;
    PXF32 bp4 = rotorB->YW;
    PXF32 bp5 = rotorB->ZW;
    PXF32 bp6 = rotorB->XYZW; // Pseudoscalar

    result.Scalar = s * bs - p0 * bp0 - p1 * bp1 - p2 * bp2 - p3 * bp3 - p4 * bp4 - p5 * bp5 + p6 * bp6;
    result.XY = s * bp0 + p0 * bs - p1 * bp3 - p2 * bp4 + p3 * bp1 + p4 * bp2 - p5 * bp6 - p6 * bp5;
    result.YZ = s * bp1 + p0 * bp3 + p1 * bs - p2 * bp5 - p3 * bp0 + p4 * bp6 + p5 * bp2 + p6 * bp4;
    result.ZX = s * bp2 + p0 * bp4 + p1 * bp5 + p2 * bs - p3 * bp6 - p4 * bp0 - p5 * bp1 - p6 * bp3;
    result.XW = s * bp3 - p0 * bp1 + p1 * bp0 - p2 * bp6 + p3 * bs - p4 * bp5 + p5 * bp4 - p6 * bp2;
    result.YW = s * bp4 - p0 * bp2 + p1 * bp6 + p2 * bp0 + p3 * bp5 + p4 * bs - p5 * bp3 + p6 * bp1;
    result.ZW = s * bp5 - p0 * bp6 - p1 * bp2 + p2 * bp1 - p3 * bp4 + p4 * bp3 + p5 * bs - p6 * bp0;
    result.XYZW = s * bp6 + p0 * bp5 - p1 * bp4 + p2 * bp3 + p3 * bp2 - p4 * bp1 + p5 * bp0 + p6 * bs;
#else

    result.Scalar += rotorB->Scalar;
    result.XY += rotorB->XY;
    result.YZ += rotorB->YZ;
    result.ZX += rotorB->ZX;
    result.XW += rotorB->XW;
    result.YW += rotorB->YW;
    result.ZW += rotorB->ZW;
    result.XYZW += rotorB->XYZW;

#endif

    PXRotor4DF32Set(rotor, &result);
}

PXF32 PXAPI PXRotor4DF32Length(const PXRotor4DF32 PXREF pxRotor4DF32)
{
    if(!pxRotor4DF32)
    {
        return 1.0f;
    }

    PXF32 lengthSq = PXRotor4DF32LengthSquared(pxRotor4DF32);

    return PXMathRootSquareF32(lengthSq);
}

PXF32 PXAPI PXRotor4DF32LengthSquared(const PXRotor4DF32 PXREF pxRotor4DF32)
{
    if(!pxRotor4DF32)
    {
        return 1.0f;
    }

    return
        (pxRotor4DF32->Scalar * pxRotor4DF32->Scalar) +
        (pxRotor4DF32->XY * pxRotor4DF32->XY) +
        (pxRotor4DF32->YZ * pxRotor4DF32->YZ) +
        (pxRotor4DF32->ZX * pxRotor4DF32->ZX) +
        (pxRotor4DF32->XW * pxRotor4DF32->XW) +
        (pxRotor4DF32->YW * pxRotor4DF32->YW) +
        (pxRotor4DF32->ZW * pxRotor4DF32->ZW) +
        (pxRotor4DF32->XYZW * pxRotor4DF32->XYZW);
}

void PXAPI PXRotor4DF32Normalize(PXRotor4DF32 PXREF pxRotor4DF32)
{
    const PXF32 lengthSq = PXRotor4DF32Length(pxRotor4DF32);
    const PXBool doCorrection = lengthSq != 1.0f;

    if(!doCorrection)
    {
        return;
    }

    // If the length drifts away from 1.0, scale it back to unit length
    const PXF32 invLength = PXMathRootSquareInverseF32(lengthSq);
    pxRotor4DF32->Scalar *= invLength;
    pxRotor4DF32->XY *= invLength;
    pxRotor4DF32->YZ *= invLength;
    pxRotor4DF32->ZX *= invLength;
    pxRotor4DF32->XW *= invLength;
    pxRotor4DF32->YW *= invLength;
    pxRotor4DF32->ZW *= invLength;
    pxRotor4DF32->XYZW *= invLength;
}

void PXAPI PXRotor4DFromAngleAndPlane(PXRotor4DF32 PXREF pxRotor4D, PXF32 angleRadians, PXF32 planeXY, PXF32 planeYZ, PXF32 planeZX)
{
    const PXF32 halfAngle = angleRadians / 2.0f;
    const PXF32 sinHalf = PXMathSinusRADF32(halfAngle);

    pxRotor4D->Scalar = PXMathCosinusRADF32(halfAngle);

    // In Geometric Algebra, the rotor formula uses a minus sign before the bivector.
    // We apply it directly to the components here.
    pxRotor4D->XY = -planeXY * sinHalf; // XY
    pxRotor4D->YZ = -planeYZ * sinHalf; // YZ
    pxRotor4D->ZX = -planeZX * sinHalf; // ZX
    pxRotor4D->XW = 0;
    pxRotor4D->YW = 0;
    pxRotor4D->ZW = 0;
    pxRotor4D->XYZW = 0;

    PXRotor4DF32Normalize(pxRotor4D);
}