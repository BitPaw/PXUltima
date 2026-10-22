#include "PXEulerAngle.h"

#include <PX/Math/PXMath.h>

#include <intrin.h>

void PXAPI PXEulerAngle3F32ToMatrix4x4(const PXEulerAngle3F32 PXREF pxEulerAngle3F32, PXMatrix4x4F PXREF pxMatrix4x4)
{ 
    PXMathSinCosRADF32Result pxMathSinCosRADF32Result;
    pxMathSinCosRADF32Result.InputAngles[0] = pxEulerAngle3F32->X;
    pxMathSinCosRADF32Result.InputAngles[1] = pxEulerAngle3F32->Y;
    pxMathSinCosRADF32Result.InputAngles[2] = pxEulerAngle3F32->Z;
    pxMathSinCosRADF32Result.InputAngles[3] = 0.0f;

    PXMathSinCosRADF32x4(&pxMathSinCosRADF32Result);

    PXF32 sx = pxMathSinCosRADF32Result.DataSin[0];
    PXF32 sy = pxMathSinCosRADF32Result.DataSin[1];
    PXF32 sz = pxMathSinCosRADF32Result.DataSin[2];

    PXF32 cx = pxMathSinCosRADF32Result.DataCos[0];
    PXF32 cy = pxMathSinCosRADF32Result.DataCos[1];
    PXF32 cz = pxMathSinCosRADF32Result.DataCos[2];

    pxMatrix4x4->Column.X.Right = cy * cz + sy * sx * sz;
    pxMatrix4x4->Column.Y.Right = cz * sy * sx - cy * sz;
    pxMatrix4x4->Column.Z.Right = cx * sy;
    pxMatrix4x4->Column.W.Right = 0.0f;

    pxMatrix4x4->Column.X.Up = cx * sz;
    pxMatrix4x4->Column.Y.Up = cx * cz;
    pxMatrix4x4->Column.Z.Up = -sx;
    pxMatrix4x4->Column.W.Up = 0.0f;

    pxMatrix4x4->Column.X.Forward = cy * sx * sz - cz * sy;
    pxMatrix4x4->Column.Y.Forward = cy * cz * sx + sy * sz;
    pxMatrix4x4->Column.Z.Forward = cy * cx;
    pxMatrix4x4->Column.W.Forward = 0.0f;

    pxMatrix4x4->Column.X.Translation = 0.0f;
    pxMatrix4x4->Column.Y.Translation = 0.0f;
    pxMatrix4x4->Column.Z.Translation = 0.0f;
    pxMatrix4x4->Column.W.Translation = 1.0f;
}

void PXAPI PXEulerAngle4F32ToMatrix4x4(const PXEulerAngle4F32 PXREF pxEulerAngle4F32, PXMatrix4x4F PXREF pxMatrix4x4)
{
    PXMathSinCosRADF32Result pxMathSinCosRADF32Result;
    pxMathSinCosRADF32Result.InputAngles[0] = pxEulerAngle4F32->X;
    pxMathSinCosRADF32Result.InputAngles[1] = pxEulerAngle4F32->Y;
    pxMathSinCosRADF32Result.InputAngles[2] = pxEulerAngle4F32->Z;
    pxMathSinCosRADF32Result.InputAngles[3] = pxEulerAngle4F32->W;

    PXMathSinCosRADF32x4(&pxMathSinCosRADF32Result);

    PXF32 sx = pxMathSinCosRADF32Result.DataSin[0];
    PXF32 sy = pxMathSinCosRADF32Result.DataSin[1];
    PXF32 sz = pxMathSinCosRADF32Result.DataSin[2];
    PXF32 sw = pxMathSinCosRADF32Result.DataSin[3];

    PXF32 cx = pxMathSinCosRADF32Result.DataCos[0];
    PXF32 cy = pxMathSinCosRADF32Result.DataCos[1];
    PXF32 cz = pxMathSinCosRADF32Result.DataCos[2];
    PXF32 cw = pxMathSinCosRADF32Result.DataCos[3];

    PXF32 r00 = cy * cz + sy * sx * sz;
    PXF32 r01 = cz * sy * sx - cy * sz;
    PXF32 r02 = cx * sy;

    PXF32 r10 = cx * sz;
    PXF32 r11 = cx * cz;
    PXF32 r12 = -sx;

    PXF32 r20 = cy * sx * sz - cz * sy;
    PXF32 r21 = cy * cz * sx + sy * sz;
    PXF32 r22 = cy * cx;

    pxMatrix4x4->Column.X.A = r00 * cw;
    pxMatrix4x4->Column.X.B = r01;
    pxMatrix4x4->Column.X.C = r02;
    pxMatrix4x4->Column.X.D = r00 * sw;

    pxMatrix4x4->Column.Y.A = r10 * cw;
    pxMatrix4x4->Column.Y.B = r11;
    pxMatrix4x4->Column.Y.C = r12;
    pxMatrix4x4->Column.Y.D = r10 * sw;

    pxMatrix4x4->Column.Z.A = r20 * cw;
    pxMatrix4x4->Column.Z.B = r21;
    pxMatrix4x4->Column.Z.C = r22;
    pxMatrix4x4->Column.Z.D = r20 * sw;

    pxMatrix4x4->Column.W.A = -sw;
    pxMatrix4x4->Column.W.B = 0.0f;
    pxMatrix4x4->Column.W.C = 0.0f;
    pxMatrix4x4->Column.W.D = cw;
}