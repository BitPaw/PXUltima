#include "PXMatrix.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Engine/PXResource.h>

void PXAPI PXMatrix4x4FIdentity(PXMatrix4x4F PXREF matrix4x4F)
{
    for (PXSize y = 0; y < 4u; ++y)
    {
        for (PXSize x = 0; x < 4u; ++x)
        {
            matrix4x4F->Data4[x + y * 4u] = y == x;
        }
    }
}

void PXAPI PXMatrix4x4FResetAxisW(PXMatrix4x4F PXREF matrix4x4F)
{
    // ---0
    // ---0
    // ---0
    // 0001

    matrix4x4F->Data4[WAxisX] = 0.0f;
    matrix4x4F->Data4[WAxisY] = 0.0f;
    matrix4x4F->Data4[WAxisZ] = 0.0f;
    matrix4x4F->Data4[TransformX] = 0.0f;
    matrix4x4F->Data4[TransformY] = 0.0f;
    matrix4x4F->Data4[TransformZ] = 0.0f;
    matrix4x4F->Data4[TransformW] = 1.0f;
}

void PXAPI PXMatrix4x4FRotationAxisSet(PXMatrix4x4F PXREF matrix4x4F, const PXI8U axisA, const PXI8U axisB, const PXF32 angle)
{
    float s;//sinf(angle);
    float c;//cosf(angle);  

    PXMathSinCosRADF32(angle, &s, &c);  

    matrix4x4F->DataXY[axisA][axisA] = c;
    matrix4x4F->DataXY[axisA][axisB] = -s;
    matrix4x4F->DataXY[axisB][axisA] = s;
    matrix4x4F->DataXY[axisB][axisB] = c;
}

void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position)
{
    position->X = matrix->Data4[TransformX];
    position->Y = matrix->Data4[TransformY];
    position->Z = matrix->Data4[TransformZ];
}

void PXAPI PXMatrix4x4FPositionSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position)
{
    matrix4x4F->Data4[TransformX] = position->X;
    matrix4x4F->Data4[TransformY] = position->Y;
    matrix4x4F->Data4[TransformZ] = position->Z;
}

void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F PXREF matrix, PXVector3F32 PXREF position)
{
    // Pitch
    position->Y = -PXMathArcusSinus(matrix->Data4[ZAxisX]); // 31

    if(matrix->Data4[ZAxisX] == 1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathArcusTangens2(-matrix->Data4[XAxisY], -matrix->Data4[XAxisZ]); // -12 -13
    }
    else if(matrix->Data4[ZAxisX] == -1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathArcusTangens2(matrix->Data4[YAxisX], matrix->Data4[XAxisX]); // 12 13
    }
    else
    {
        // Yaw
        position->X = PXMathArcusTangens2(matrix->Data4[YAxisX], matrix->Data4[XAxisX]); // 21 11

        // Roll
        position->Z = PXMathArcusTangens2(matrix->Data4[ZAxisY], matrix->Data4[ZAxisZ]); // 32 33
    }
}

void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position)
{
    PXMatrix4x4F pxMatrix4x4F;

    PXMatrix4x4FRotationMatrixGenerate(&pxMatrix4x4F, position);

    for(PXSize y = 0; y < 3; ++y)
    {
        for(PXSize x = 0; x < 3; ++x)
        {
            PXSize index = y * 4 + x;

            matrix4x4F->Data4[index] = pxMatrix4x4F.Data4[index];
        }
    }
}

void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF position)
{
    PXMatrix4x4F xRotation;
    PXMatrix4x4F yRotation;
    PXMatrix4x4F zRotation;

    PXMatrix4x4FIdentity(&xRotation);
    PXMatrix4x4FIdentity(&yRotation);
    PXMatrix4x4FIdentity(&zRotation);

    //-----<X ROT>-----
    {
        const PXF32 cosResult = PXMathCosinusRADF32(position->X);
        const PXF32 sinResult = PXMathSinusRADF32(position->X);

        xRotation.Data4[5] = cosResult;
        xRotation.Data4[6] = sinResult;
        xRotation.Data4[9] = -sinResult;
        xRotation.Data4[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const PXF32 cosResult = PXMathCosinusRADF32(-position->Y);
        const PXF32 sinResult = PXMathSinusRADF32(-position->Y);

        yRotation.Data4[0] = cosResult;
        yRotation.Data4[2] = -sinResult;
        yRotation.Data4[8] = sinResult;
        yRotation.Data4[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const PXF32 cosResult = PXMathCosinusRADF32(position->Z);
        const PXF32 sinResult = PXMathSinusRADF32(position->Z);

        zRotation.Data4[0] = cosResult;
        zRotation.Data4[1] = -sinResult;
        zRotation.Data4[4] = sinResult;
        zRotation.Data4[5] = cosResult;
    }

    // Gimble (result = xRotation * yRotation * zRotation;)
    {

        PXMatrix4x4FIdentity(matrix4x4F);

        PXMatrix4x4FMultiply(matrix4x4F, &zRotation);
        PXMatrix4x4FMultiply(matrix4x4F, &yRotation);
        PXMatrix4x4FMultiply(matrix4x4F, &xRotation);

        /*
                PXMatrix4x4F tempRotation;
        PXMatrix4x4FIdentity(&tempRotation);

        PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
        PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
        PXMatrix4x4FMultiply(&tempRotation, &xRotation, matrix4x4F);

        */
    }
}

void PXAPI PXMatrix4x4FMultiply(PXMatrix4x4F PXREF matrixA, const PXMatrix4x4F PXREF matrixB)
{
    PXF32 a = matrixA->Data4[0];
    PXF32 b = matrixA->Data4[1];
    PXF32 c = matrixA->Data4[2];
    PXF32 d = matrixA->Data4[3];
    PXF32 e = matrixA->Data4[4];
    PXF32 f = matrixA->Data4[5];
    PXF32 g = matrixA->Data4[6];
    PXF32 h = matrixA->Data4[7];
    PXF32 i = matrixA->Data4[8];
    PXF32 j = matrixA->Data4[9];
    PXF32 k = matrixA->Data4[10];
    PXF32 l = matrixA->Data4[11];
    PXF32 m = matrixA->Data4[12];
    PXF32 n = matrixA->Data4[13];
    PXF32 o = matrixA->Data4[14];
    PXF32 p = matrixA->Data4[15];

    PXF32 A = matrixB->Data4[0];
    PXF32 B = matrixB->Data4[1];
    PXF32 C = matrixB->Data4[2];
    PXF32 D = matrixB->Data4[3];
    PXF32 E = matrixB->Data4[4];
    PXF32 F = matrixB->Data4[5];
    PXF32 G = matrixB->Data4[6];
    PXF32 H = matrixB->Data4[7];
    PXF32 I = matrixB->Data4[8];
    PXF32 J = matrixB->Data4[9];
    PXF32 K = matrixB->Data4[10];
    PXF32 L = matrixB->Data4[11];
    PXF32 M = matrixB->Data4[12];
    PXF32 N = matrixB->Data4[13];
    PXF32 O = matrixB->Data4[14];
    PXF32 P = matrixB->Data4[15];

    matrixA->Data4[0] = a * A + b * E + c * I + d * M;
    matrixA->Data4[1] = e * A + f * E + g * I + h * M;
    matrixA->Data4[2] = i * A + j * E + k * I + l * M;
    matrixA->Data4[3] = m * A + n * E + o * I + p * M;

    matrixA->Data4[4] = a * B + b * F + c * J + d * N;
    matrixA->Data4[5] = e * B + f * F + g * J + h * N;
    matrixA->Data4[6] = i * B + j * F + k * J + l * N;
    matrixA->Data4[7] = m * B + n * F + o * J + p * N;

    matrixA->Data4[8] = a * C + b * G + c * K + d * O;
    matrixA->Data4[9] = e * C + f * G + g * K + h * O;
    matrixA->Data4[10] = i * C + j * G + k * K + l * O;
    matrixA->Data4[11] = m * C + n * G + o * K + p * O;

    matrixA->Data4[12] = a * D + b * H + c * L + d * P;
    matrixA->Data4[13] = e * D + f * H + g * L + h * P;
    matrixA->Data4[14] = i * D + j * H + k * L + l * P;
    matrixA->Data4[15] = m * D + n * H + o * L + p * P;
}

void PXAPI PXMatrix4x4FMultiplyV4F(const PXMatrix4x4F PXREF matrix, PXVector4F32 PXREF pxVector4F32)
{
    const PXVector4F32 copy = *pxVector4F32;

    pxVector4F32->X = matrix->DataXY[0][0] * copy.X + matrix->DataXY[0][1] * copy.Y + matrix->DataXY[0][2] * copy.Z + matrix->DataXY[0][3] * copy.W;
    pxVector4F32->Y = matrix->DataXY[1][0] * copy.X + matrix->DataXY[1][1] * copy.Y + matrix->DataXY[1][2] * copy.Z + matrix->DataXY[1][3] * copy.W;
    pxVector4F32->Z = matrix->DataXY[2][0] * copy.X + matrix->DataXY[2][1] * copy.Y + matrix->DataXY[2][2] * copy.Z + matrix->DataXY[2][3] * copy.W;
    pxVector4F32->W = matrix->DataXY[3][0] * copy.X + matrix->DataXY[3][1] * copy.Y + matrix->DataXY[3][2] * copy.Z + matrix->DataXY[3][3] * copy.W;
}

void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF vector3F)
{
    PXMatrix4x4F matrixRotation;

    PXMatrix4x4FRotationMatrixGenerate(&matrixRotation, vector3F);

    PXMatrix4x4FMultiply(matrix4x4F, &matrixRotation);
}

void PXAPI PXMatrix4x4FCopy(const PXMatrix4x4F PXREF matrixA, PXMatrix4x4F PXREF matrixResult)
{
    PXCopy(PXMatrix4x4F, matrixA, matrixResult);
}

void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F PXREF matrixData, const PXVector3F32 PXREF vector3F)
{
    matrixData->Data4[TransformX] += vector3F->X;
    matrixData->Data4[TransformY] += vector3F->Y;
    matrixData->Data4[TransformZ] += vector3F->Z;
}

void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y)
{
    matrixData->Data4[TransformX] += x;
    matrixData->Data4[TransformY] += y;
}

void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y)
{
    PXMatrix4x4FMoveXY(matrixData, x * (1- matrixData->Data4[ScaleX]), y * (1 - matrixData->Data4[ScaleY]));
}

void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F PXREF matrixData, const PXF32 x, const PXF32 y, const PXF32 z)
{
    matrixData->Data4[TransformX] += x;
    matrixData->Data4[TransformY] += y;
    matrixData->Data4[TransformZ] += z;
}

void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 scalar)
{
    pxMatrix4x4F->Data4[ScaleX] *= scalar;
    pxMatrix4x4F->Data4[ScaleY] *= scalar;
    pxMatrix4x4F->Data4[ScaleZ] *= scalar;
}

/*
void PXAPI PXMatrix4x4FScaleByMargin(PXMatrix4x4F PXREF pxMatrix4x4F, const PXRectangleOffset PXREF pxMargin)
{
#if 1

    const PXRectangleOffset offset =
    {
        1 - pxMargin->Left,
        1 - pxMargin->Top,
        1 - pxMargin->Right,
        1 - pxMargin->Bottom
    };

    const PXF32 offsetTotalWidth = (pxMargin->Left + pxMargin->Right) / 2.0f;
    const PXF32 offsetTotalHeight = (pxMargin->Top + pxMargin->Bottom) / 2.0f;

    pxMatrix4x4F->Data[ScaleX] *= offsetTotalWidth;
    pxMatrix4x4F->Data[ScaleY] *= offsetTotalHeight;

    //pxMatrix4x4F->Data[TransformX] += offset.Left + offset.Right;
    //pxMatrix4x4F->Data[TransformY] -= offset.Top + offset.Bottom;



#else
    const PXF32 moveX = 1 - pxMargin->Right;
    const PXF32 shrinkX = 1 - pxMargin->Left;
    const PXF32 moveY = 1 - pxMargin->Top;
    const PXF32 shrinkY = 1 - pxMargin->Bottom;

    const PXF32 totalWidth = 1 - (moveX + shrinkX);
    const PXF32 totalHeight = 1 - (moveY + shrinkY);

    pxMatrix4x4F->Data[TransformX] += (moveX - shrinkX);
    pxMatrix4x4F->Data[TransformY] -= (moveY - shrinkY);
    //matrixResult->Data[TransformZ] = 1;

    pxMatrix4x4F->Data[ScaleX] *= totalWidth;
    pxMatrix4x4F->Data[ScaleY] *= totalHeight;
    //pxMatrix4x4F->Data[ScaleZ] = 1.0f;
#endif // 0



}
*/

void PXAPI PXMatrix4x4FScaleByXY(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 x, const PXF32 y)
{
    pxMatrix4x4F->Data4[ScaleX] *= x;
    pxMatrix4x4F->Data4[ScaleY] *= y;
}

void PXAPI PXMatrix4x4FScaleSet(PXMatrix4x4F PXREF pxMatrix4x4F, const PXVector3F32 PXREF pxVector3F32)
{
    pxMatrix4x4F->Data4[ScaleX] = pxVector3F32->X;
    pxMatrix4x4F->Data4[ScaleY] = pxVector3F32->Y;
    pxMatrix4x4F->Data4[ScaleZ] = pxVector3F32->Z;
}

void PXAPI PXMatrix4x4FScaleSetXY(PXMatrix4x4F PXREF pxMatrix4x4F, const PXF32 x, const PXF32 y)
{
    pxMatrix4x4F->Data4[ScaleX] = x;
    pxMatrix4x4F->Data4[ScaleY] = y;
}

void PXAPI PXMatrix4x4FScaleSetXYZ(PXMatrix4x4F PXREF xpMatrix4x4F, const PXF32 x, const PXF32 y, const PXF32 z)
{
    const PXVector3F32 PXVector3F32 = {x, y, z};

    PXMatrix4x4FScaleSet(xpMatrix4x4F, &PXVector3F32);
}

void PXAPI PXMatrix4x4FScaleGet(const PXMatrix4x4F PXREF matrixResult, PXVector3F32 PXREF pxVector3F32)
{
    pxVector3F32->X = matrixResult->Data4[ScaleX];
    pxVector3F32->Y = matrixResult->Data4[ScaleY];
    pxVector3F32->Z = matrixResult->Data4[ScaleZ];
}

void PXAPI PXMatrix4x4FScaleGetXYZ(const PXMatrix4x4F PXREF matrixResult, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z)
{
    *x = matrixResult->Data4[ScaleX];
    *y = matrixResult->Data4[ScaleY];
    *z = matrixResult->Data4[ScaleZ];
}

void PXAPI PXMatrix4x4FOrthographic(PXMatrix4x4F PXREF matrix4x4F, const PXF32 left, const PXF32 right, const PXF32 bottom, const PXF32 top, const PXF32 nearPlane, const PXF32 farPlane)
{
    PXMatrix4x4FIdentity(matrix4x4F);

    matrix4x4F->Data4[ScaleX] = (2.0f) / (right - left);
    matrix4x4F->Data4[ScaleY] = (2.0f) / (top - bottom);
    matrix4x4F->Data4[ScaleZ] = -(2.0f) / (farPlane - nearPlane);
    matrix4x4F->Data4[TransformX] = -(right + left) / (right - left);
    matrix4x4F->Data4[TransformY] = -(top + bottom) / (top - bottom);
    matrix4x4F->Data4[TransformZ] = -(farPlane + nearPlane) / (farPlane - nearPlane);
}

void PXAPI PXMatrix4x4FPerspective(PXMatrix4x4F PXREF matrix4x4F, const PXF32 fielfOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane)
{
    const PXF32 difference = farPlane - nearPlane;
    const PXF32 fielfOfViewRadians = PXMathDegreeToRadians(fielfOfView);
    const PXF32 tanHalfFovy = PXMathTangensRADF32(fielfOfViewRadians / 2.0f);

    PXMatrix4x4FIdentity(matrix4x4F);

    matrix4x4F->Data4[ScaleX] = (1) / (aspectRatio * tanHalfFovy);
    matrix4x4F->Data4[ScaleY] = (1) / (tanHalfFovy);
    matrix4x4F->Data4[ScaleZ] = -(farPlane + nearPlane) / difference;
    matrix4x4F->Data4[11] = -(1);
    matrix4x4F->Data4[TransformZ] = -((2) * farPlane * nearPlane) / difference;
}

PXBool PXAPI PXMatrix4x4FInverse(PXMatrix4x4F PXREF matrix4x4F)
{
    PXMatrix4x4F temp;
    double det = 0;

    temp.Data4[0] = matrix4x4F->Data4[5] * matrix4x4F->Data4[10] * matrix4x4F->Data4[15] - matrix4x4F->Data4[5] * matrix4x4F->Data4[11] * matrix4x4F->Data4[14] - matrix4x4F->Data4[9] * matrix4x4F->Data4[6] * matrix4x4F->Data4[15] + matrix4x4F->Data4[9] * matrix4x4F->Data4[7] * matrix4x4F->Data4[14] + matrix4x4F->Data4[13] * matrix4x4F->Data4[6] * matrix4x4F->Data4[11] - matrix4x4F->Data4[13] * matrix4x4F->Data4[7] * matrix4x4F->Data4[10];
    temp.Data4[4] = -matrix4x4F->Data4[4] * matrix4x4F->Data4[10] * matrix4x4F->Data4[15] + matrix4x4F->Data4[4] * matrix4x4F->Data4[11] * matrix4x4F->Data4[14] + matrix4x4F->Data4[8] * matrix4x4F->Data4[6] * matrix4x4F->Data4[15] - matrix4x4F->Data4[8] * matrix4x4F->Data4[7] * matrix4x4F->Data4[14] - matrix4x4F->Data4[12] * matrix4x4F->Data4[6] * matrix4x4F->Data4[11] + matrix4x4F->Data4[12] * matrix4x4F->Data4[7] * matrix4x4F->Data4[10];
    temp.Data4[8] = matrix4x4F->Data4[4] * matrix4x4F->Data4[9] * matrix4x4F->Data4[15] - matrix4x4F->Data4[4] * matrix4x4F->Data4[11] * matrix4x4F->Data4[13] - matrix4x4F->Data4[8] * matrix4x4F->Data4[5] * matrix4x4F->Data4[15] + matrix4x4F->Data4[8] * matrix4x4F->Data4[7] * matrix4x4F->Data4[13] + matrix4x4F->Data4[12] * matrix4x4F->Data4[5] * matrix4x4F->Data4[11] - matrix4x4F->Data4[12] * matrix4x4F->Data4[7] * matrix4x4F->Data4[9];
    temp.Data4[12] = -matrix4x4F->Data4[4] * matrix4x4F->Data4[9] * matrix4x4F->Data4[14] + matrix4x4F->Data4[4] * matrix4x4F->Data4[10] * matrix4x4F->Data4[13] + matrix4x4F->Data4[8] * matrix4x4F->Data4[5] * matrix4x4F->Data4[14] - matrix4x4F->Data4[8] * matrix4x4F->Data4[6] * matrix4x4F->Data4[13] - matrix4x4F->Data4[12] * matrix4x4F->Data4[5] * matrix4x4F->Data4[10] + matrix4x4F->Data4[12] * matrix4x4F->Data4[6] * matrix4x4F->Data4[9];
    temp.Data4[1] = -matrix4x4F->Data4[1] * matrix4x4F->Data4[10] * matrix4x4F->Data4[15] + matrix4x4F->Data4[1] * matrix4x4F->Data4[11] * matrix4x4F->Data4[14] + matrix4x4F->Data4[9] * matrix4x4F->Data4[2] * matrix4x4F->Data4[15] - matrix4x4F->Data4[9] * matrix4x4F->Data4[3] * matrix4x4F->Data4[14] - matrix4x4F->Data4[13] * matrix4x4F->Data4[2] * matrix4x4F->Data4[11] + matrix4x4F->Data4[13] * matrix4x4F->Data4[3] * matrix4x4F->Data4[10];
    temp.Data4[5] = matrix4x4F->Data4[0] * matrix4x4F->Data4[10] * matrix4x4F->Data4[15] - matrix4x4F->Data4[0] * matrix4x4F->Data4[11] * matrix4x4F->Data4[14] - matrix4x4F->Data4[8] * matrix4x4F->Data4[2] * matrix4x4F->Data4[15] + matrix4x4F->Data4[8] * matrix4x4F->Data4[3] * matrix4x4F->Data4[14] + matrix4x4F->Data4[12] * matrix4x4F->Data4[2] * matrix4x4F->Data4[11] - matrix4x4F->Data4[12] * matrix4x4F->Data4[3] * matrix4x4F->Data4[10];
    temp.Data4[9] = -matrix4x4F->Data4[0] * matrix4x4F->Data4[9] * matrix4x4F->Data4[15] + matrix4x4F->Data4[0] * matrix4x4F->Data4[11] * matrix4x4F->Data4[13] + matrix4x4F->Data4[8] * matrix4x4F->Data4[1] * matrix4x4F->Data4[15] - matrix4x4F->Data4[8] * matrix4x4F->Data4[3] * matrix4x4F->Data4[13] - matrix4x4F->Data4[12] * matrix4x4F->Data4[1] * matrix4x4F->Data4[11] + matrix4x4F->Data4[12] * matrix4x4F->Data4[3] * matrix4x4F->Data4[9];
    temp.Data4[13] = matrix4x4F->Data4[0] * matrix4x4F->Data4[9] * matrix4x4F->Data4[14] - matrix4x4F->Data4[0] * matrix4x4F->Data4[10] * matrix4x4F->Data4[13] - matrix4x4F->Data4[8] * matrix4x4F->Data4[1] * matrix4x4F->Data4[14] + matrix4x4F->Data4[8] * matrix4x4F->Data4[2] * matrix4x4F->Data4[13] + matrix4x4F->Data4[12] * matrix4x4F->Data4[1] * matrix4x4F->Data4[10] - matrix4x4F->Data4[12] * matrix4x4F->Data4[2] * matrix4x4F->Data4[9];
    temp.Data4[2] = matrix4x4F->Data4[1] * matrix4x4F->Data4[6] * matrix4x4F->Data4[15] - matrix4x4F->Data4[1] * matrix4x4F->Data4[7] * matrix4x4F->Data4[14] - matrix4x4F->Data4[5] * matrix4x4F->Data4[2] * matrix4x4F->Data4[15] + matrix4x4F->Data4[5] * matrix4x4F->Data4[3] * matrix4x4F->Data4[14] + matrix4x4F->Data4[13] * matrix4x4F->Data4[2] * matrix4x4F->Data4[7] - matrix4x4F->Data4[13] * matrix4x4F->Data4[3] * matrix4x4F->Data4[6];
    temp.Data4[6] = -matrix4x4F->Data4[0] * matrix4x4F->Data4[6] * matrix4x4F->Data4[15] + matrix4x4F->Data4[0] * matrix4x4F->Data4[7] * matrix4x4F->Data4[14] + matrix4x4F->Data4[4] * matrix4x4F->Data4[2] * matrix4x4F->Data4[15] - matrix4x4F->Data4[4] * matrix4x4F->Data4[3] * matrix4x4F->Data4[14] - matrix4x4F->Data4[12] * matrix4x4F->Data4[2] * matrix4x4F->Data4[7] + matrix4x4F->Data4[12] * matrix4x4F->Data4[3] * matrix4x4F->Data4[6];
    temp.Data4[10] = matrix4x4F->Data4[0] * matrix4x4F->Data4[5] * matrix4x4F->Data4[15] - matrix4x4F->Data4[0] * matrix4x4F->Data4[7] * matrix4x4F->Data4[13] - matrix4x4F->Data4[4] * matrix4x4F->Data4[1] * matrix4x4F->Data4[15] + matrix4x4F->Data4[4] * matrix4x4F->Data4[3] * matrix4x4F->Data4[13] + matrix4x4F->Data4[12] * matrix4x4F->Data4[1] * matrix4x4F->Data4[7] - matrix4x4F->Data4[12] * matrix4x4F->Data4[3] * matrix4x4F->Data4[5];
    temp.Data4[14] = -matrix4x4F->Data4[0] * matrix4x4F->Data4[5] * matrix4x4F->Data4[14] + matrix4x4F->Data4[0] * matrix4x4F->Data4[6] * matrix4x4F->Data4[13] + matrix4x4F->Data4[4] * matrix4x4F->Data4[1] * matrix4x4F->Data4[14] - matrix4x4F->Data4[4] * matrix4x4F->Data4[2] * matrix4x4F->Data4[13] - matrix4x4F->Data4[12] * matrix4x4F->Data4[1] * matrix4x4F->Data4[6] + matrix4x4F->Data4[12] * matrix4x4F->Data4[2] * matrix4x4F->Data4[5];
    temp.Data4[3] = -matrix4x4F->Data4[1] * matrix4x4F->Data4[6] * matrix4x4F->Data4[11] + matrix4x4F->Data4[1] * matrix4x4F->Data4[7] * matrix4x4F->Data4[10] + matrix4x4F->Data4[5] * matrix4x4F->Data4[2] * matrix4x4F->Data4[11] - matrix4x4F->Data4[5] * matrix4x4F->Data4[3] * matrix4x4F->Data4[10] - matrix4x4F->Data4[9] * matrix4x4F->Data4[2] * matrix4x4F->Data4[7] + matrix4x4F->Data4[9] * matrix4x4F->Data4[3] * matrix4x4F->Data4[6];
    temp.Data4[7] = matrix4x4F->Data4[0] * matrix4x4F->Data4[6] * matrix4x4F->Data4[11] - matrix4x4F->Data4[0] * matrix4x4F->Data4[7] * matrix4x4F->Data4[10] - matrix4x4F->Data4[4] * matrix4x4F->Data4[2] * matrix4x4F->Data4[11] + matrix4x4F->Data4[4] * matrix4x4F->Data4[3] * matrix4x4F->Data4[10] + matrix4x4F->Data4[8] * matrix4x4F->Data4[2] * matrix4x4F->Data4[7] - matrix4x4F->Data4[8] * matrix4x4F->Data4[3] * matrix4x4F->Data4[6];
    temp.Data4[11] = -matrix4x4F->Data4[0] * matrix4x4F->Data4[5] * matrix4x4F->Data4[11] + matrix4x4F->Data4[0] * matrix4x4F->Data4[7] * matrix4x4F->Data4[9] + matrix4x4F->Data4[4] * matrix4x4F->Data4[1] * matrix4x4F->Data4[11] - matrix4x4F->Data4[4] * matrix4x4F->Data4[3] * matrix4x4F->Data4[9] - matrix4x4F->Data4[8] * matrix4x4F->Data4[1] * matrix4x4F->Data4[7] + matrix4x4F->Data4[8] * matrix4x4F->Data4[3] * matrix4x4F->Data4[5];
    temp.Data4[15] = matrix4x4F->Data4[0] * matrix4x4F->Data4[5] * matrix4x4F->Data4[10] - matrix4x4F->Data4[0] * matrix4x4F->Data4[6] * matrix4x4F->Data4[9] - matrix4x4F->Data4[4] * matrix4x4F->Data4[1] * matrix4x4F->Data4[10] + matrix4x4F->Data4[4] * matrix4x4F->Data4[2] * matrix4x4F->Data4[9] + matrix4x4F->Data4[8] * matrix4x4F->Data4[1] * matrix4x4F->Data4[6] - matrix4x4F->Data4[8] * matrix4x4F->Data4[2] * matrix4x4F->Data4[5];

    det = matrix4x4F->Data4[0] * temp.Data4[0] + matrix4x4F->Data4[1] * temp.Data4[4] + matrix4x4F->Data4[2] * temp.Data4[8] + matrix4x4F->Data4[3] * temp.Data4[12];

    if (det == 0)
        return PXNo;

    det = 1.0 / det;

    for (PXSize i = 0; i < 16u; ++i)
    {
        matrix4x4F->Data4[i] = temp.Data4[i] * det;
    }

    return PXYes;
}

void PXAPI PXMatrix4x4FTranpose(PXMatrix4x4F PXREF matrix4x4F)
{
    //NumberType a = this->Data[0];
    const PXF32 b = matrix4x4F->Data4[1];
    const PXF32 c = matrix4x4F->Data4[2];
    const PXF32 d = matrix4x4F->Data4[3];

    const PXF32 e = matrix4x4F->Data4[4];
    //NumberType f = this->Data[5];
    const PXF32 g = matrix4x4F->Data4[6];
    const PXF32 h = matrix4x4F->Data4[7];

    const PXF32 i = matrix4x4F->Data4[8];
    const PXF32 j = matrix4x4F->Data4[9];
    //NumberType k = this->Data[10];
    const PXF32 l = matrix4x4F->Data4[11];

    const PXF32 m = matrix4x4F->Data4[12];
    const PXF32 n = matrix4x4F->Data4[13];
    const PXF32 o = matrix4x4F->Data4[14];
    //NumberType p = this->Data[15];

    //this->Data[0] = a;
    matrix4x4F->Data4[1] = e;
    matrix4x4F->Data4[2] = i;
    matrix4x4F->Data4[3] = m;

    matrix4x4F->Data4[4] = b;
    //this->Data[5] = f;
    matrix4x4F->Data4[6] = j;
    matrix4x4F->Data4[7] = n;

    matrix4x4F->Data4[8] = c;
    matrix4x4F->Data4[9] = g;
    //this->Data[10] = k;
    matrix4x4F->Data4[11] = o;

    matrix4x4F->Data4[12] = d;
    matrix4x4F->Data4[13] = h;
    matrix4x4F->Data4[14] = l;
    //this->Data[15] = p;
}

void PXAPI PXMatrix4x4FLookAt(PXMatrix4x4F PXREF matrix4x4F, const PXVector3F32 PXREF eye, const PXVector3F32 PXREF center, const PXVector3F32 PXREF up)
{
    PXVector3F32 centereye = *center;
    PXVector3F32 frontUpCross;
    PXVector3F32 u;

    PXVector3F32Substract(&centereye, eye);
    PXVector3F32Normalize(&centereye);
    PXVector3F32CrossProduct(&frontUpCross, up, &centereye);

    PXVector3F32Normalize(&frontUpCross);
    PXVector3F32CrossProduct(&u, &centereye, &frontUpCross);

    matrix4x4F->Data4[XAxisX] = frontUpCross.X;
    matrix4x4F->Data4[XAxisY] = frontUpCross.Y;
    matrix4x4F->Data4[XAxisZ] = frontUpCross.Z;
    matrix4x4F->Data4[XAxisW] = 0;

    matrix4x4F->Data4[YAxisX] = u.X;
    matrix4x4F->Data4[YAxisY] = u.Y;
    matrix4x4F->Data4[YAxisZ] = u.Z;
    matrix4x4F->Data4[YAxisW] = 0;

    matrix4x4F->Data4[ZAxisX] = -centereye.X;
    matrix4x4F->Data4[ZAxisY] = -centereye.Y;
    matrix4x4F->Data4[ZAxisZ] = -centereye.Z;
    matrix4x4F->Data4[ZAxisW] = 0;

    matrix4x4F->Data4[WAxisX] = 0;
    matrix4x4F->Data4[WAxisY] = 0;
    matrix4x4F->Data4[WAxisZ] = 0;
    matrix4x4F->Data4[WAxisW] = 0;

    matrix4x4F->Data4[TransformX] = -PXVector3F32DotProduct(&frontUpCross, eye);
    matrix4x4F->Data4[TransformY] = -PXVector3F32DotProduct(&u, eye);
    matrix4x4F->Data4[TransformZ] = PXVector3F32DotProduct(&centereye, eye);
    matrix4x4F->Data4[TransformW] = 1;
}

/*
    sprintf
    (
        buffer,
        "||-------------------------------||\n"
        "||%7.2f|%7.2f|%7.2f|%7.2f||\n"
        "||%7.2f|%7.2f|%7.2f|%7.2f||\n"
        "||%7.2f|%7.2f|%7.2f|%7.2f||\n"
        "||%7.2f|%7.2f|%7.2f|%7.2f||\n"
        "||-------------------------------||\n",
        matrix4x4F->Data[0],
        matrix4x4F->Data[1],
        matrix4x4F->Data[2],
        matrix4x4F->Data[3],

        matrix4x4F->Data[4],
        matrix4x4F->Data[5],
        matrix4x4F->Data[6],
        matrix4x4F->Data[7],

        matrix4x4F->Data[8],
        matrix4x4F->Data[9],
        matrix4x4F->Data[10],
        matrix4x4F->Data[11],

        matrix4x4F->Data[12],
        matrix4x4F->Data[13],
        matrix4x4F->Data[14],
        matrix4x4F->Data[15]
    );
*/
