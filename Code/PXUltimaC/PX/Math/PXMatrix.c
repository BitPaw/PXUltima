#include "PXMatrix.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Engine/PXResource.h>

void PXAPI PXMatrix4x4FIdentity(PXMatrix4x4F* const matrix4x4F)
{
    for (PXSize y = 0; y < 4u; ++y)
    {
        for (PXSize x = 0; x < 4u; ++x)
        {
            matrix4x4F->Data[x + y * 4u] = y == x;
        }
    }
}

void PXAPI PXMatrix4x4FResetAxisW(PXMatrix4x4F* const matrix4x4F)
{
    // ---0
    // ---0
    // ---0
    // 0001

    matrix4x4F->Data[WAxisX] = 0.0f;
    matrix4x4F->Data[WAxisY] = 0.0f;
    matrix4x4F->Data[WAxisZ] = 0.0f;
    matrix4x4F->Data[TransformX] = 0.0f;
    matrix4x4F->Data[TransformY] = 0.0f;
    matrix4x4F->Data[TransformZ] = 0.0f;
    matrix4x4F->Data[TransformW] = 1.0f;
}

void PXAPI PXMatrix4x4FRotationAxisSet(PXMatrix4x4F* const matrix4x4F, const PXI8U axisA, const PXI8U axisB, const PXF32 angle)
{
    float s;//sinf(angle);
    float c;//cosf(angle);  

    PXMathSinCosRADF32(angle, &s, &c);  

    matrix4x4F->DataXY[axisA][axisA] = c;
    matrix4x4F->DataXY[axisA][axisB] = -s;
    matrix4x4F->DataXY[axisB][axisA] = s;
    matrix4x4F->DataXY[axisB][axisB] = c;
}

void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F* const matrix, PXVector3F32* const position)
{
    position->X = matrix->Data[TransformX];
    position->Y = matrix->Data[TransformY];
    position->Z = matrix->Data[TransformZ];
}

void PXAPI PXMatrix4x4FPositionSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F32* const position)
{
    matrix4x4F->Data[TransformX] = position->X;
    matrix4x4F->Data[TransformY] = position->Y;
    matrix4x4F->Data[TransformZ] = position->Z;
}

void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F* const matrix, PXVector3F32* const position)
{
    // Pitch
    position->Y = -PXMathArcusSinus(matrix->Data[ZAxisX]); // 31

    if(matrix->Data[ZAxisX] == 1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathArcusTangens2(-matrix->Data[XAxisY], -matrix->Data[XAxisZ]); // -12 -13
    }
    else if(matrix->Data[ZAxisX] == -1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathArcusTangens2(matrix->Data[YAxisX], matrix->Data[XAxisX]); // 12 13
    }
    else
    {
        // Yaw
        position->X = PXMathArcusTangens2(matrix->Data[YAxisX], matrix->Data[XAxisX]); // 21 11

        // Roll
        position->Z = PXMathArcusTangens2(matrix->Data[ZAxisY], matrix->Data[ZAxisZ]); // 32 33
    }
}

void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F32* const position)
{
    PXMatrix4x4F pxMatrix4x4F;

    PXMatrix4x4FRotationMatrixGenerate(&pxMatrix4x4F, position);

    for(PXSize y = 0; y < 3; ++y)
    {
        for(PXSize x = 0; x < 3; ++x)
        {
            PXSize index = y * 4 + x;

            matrix4x4F->Data[index] = pxMatrix4x4F.Data[index];
        }
    }
}

void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F* const matrix4x4F, const PXVector3F32* const position)
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

        xRotation.Data[5] = cosResult;
        xRotation.Data[6] = sinResult;
        xRotation.Data[9] = -sinResult;
        xRotation.Data[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const PXF32 cosResult = PXMathCosinusRADF32(-position->Y);
        const PXF32 sinResult = PXMathSinusRADF32(-position->Y);

        yRotation.Data[0] = cosResult;
        yRotation.Data[2] = -sinResult;
        yRotation.Data[8] = sinResult;
        yRotation.Data[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const PXF32 cosResult = PXMathCosinusRADF32(position->Z);
        const PXF32 sinResult = PXMathSinusRADF32(position->Z);

        zRotation.Data[0] = cosResult;
        zRotation.Data[1] = -sinResult;
        zRotation.Data[4] = sinResult;
        zRotation.Data[5] = cosResult;
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

void PXAPI PXMatrix4x4FMultiply(PXMatrix4x4F* const matrixA, const PXMatrix4x4F* const matrixB)
{
    PXF32 a = matrixA->Data[0];
    PXF32 b = matrixA->Data[1];
    PXF32 c = matrixA->Data[2];
    PXF32 d = matrixA->Data[3];
    PXF32 e = matrixA->Data[4];
    PXF32 f = matrixA->Data[5];
    PXF32 g = matrixA->Data[6];
    PXF32 h = matrixA->Data[7];
    PXF32 i = matrixA->Data[8];
    PXF32 j = matrixA->Data[9];
    PXF32 k = matrixA->Data[10];
    PXF32 l = matrixA->Data[11];
    PXF32 m = matrixA->Data[12];
    PXF32 n = matrixA->Data[13];
    PXF32 o = matrixA->Data[14];
    PXF32 p = matrixA->Data[15];

    PXF32 A = matrixB->Data[0];
    PXF32 B = matrixB->Data[1];
    PXF32 C = matrixB->Data[2];
    PXF32 D = matrixB->Data[3];
    PXF32 E = matrixB->Data[4];
    PXF32 F = matrixB->Data[5];
    PXF32 G = matrixB->Data[6];
    PXF32 H = matrixB->Data[7];
    PXF32 I = matrixB->Data[8];
    PXF32 J = matrixB->Data[9];
    PXF32 K = matrixB->Data[10];
    PXF32 L = matrixB->Data[11];
    PXF32 M = matrixB->Data[12];
    PXF32 N = matrixB->Data[13];
    PXF32 O = matrixB->Data[14];
    PXF32 P = matrixB->Data[15];

    matrixA->Data[0] = a * A + b * E + c * I + d * M;
    matrixA->Data[1] = e * A + f * E + g * I + h * M;
    matrixA->Data[2] = i * A + j * E + k * I + l * M;
    matrixA->Data[3] = m * A + n * E + o * I + p * M;

    matrixA->Data[4] = a * B + b * F + c * J + d * N;
    matrixA->Data[5] = e * B + f * F + g * J + h * N;
    matrixA->Data[6] = i * B + j * F + k * J + l * N;
    matrixA->Data[7] = m * B + n * F + o * J + p * N;

    matrixA->Data[8] = a * C + b * G + c * K + d * O;
    matrixA->Data[9] = e * C + f * G + g * K + h * O;
    matrixA->Data[10] = i * C + j * G + k * K + l * O;
    matrixA->Data[11] = m * C + n * G + o * K + p * O;

    matrixA->Data[12] = a * D + b * H + c * L + d * P;
    matrixA->Data[13] = e * D + f * H + g * L + h * P;
    matrixA->Data[14] = i * D + j * H + k * L + l * P;
    matrixA->Data[15] = m * D + n * H + o * L + p * P;
}

void PXAPI PXMatrix4x4FMultiplyV4F(const PXMatrix4x4F* const matrix, PXVector4F32* const pxVector4F32)
{
    const PXVector4F32 copy = *pxVector4F32;

    pxVector4F32->X = matrix->DataXY[0][0] * copy.X + matrix->DataXY[0][1] * copy.Y + matrix->DataXY[0][2] * copy.Z + matrix->DataXY[0][3] * copy.W;
    pxVector4F32->Y = matrix->DataXY[1][0] * copy.X + matrix->DataXY[1][1] * copy.Y + matrix->DataXY[1][2] * copy.Z + matrix->DataXY[1][3] * copy.W;
    pxVector4F32->Z = matrix->DataXY[2][0] * copy.X + matrix->DataXY[2][1] * copy.Y + matrix->DataXY[2][2] * copy.Z + matrix->DataXY[2][3] * copy.W;
    pxVector4F32->W = matrix->DataXY[3][0] * copy.X + matrix->DataXY[3][1] * copy.Y + matrix->DataXY[3][2] * copy.Z + matrix->DataXY[3][3] * copy.W;
}

void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F* const matrix4x4F, const PXVector3F32* const vector3F)
{
    PXMatrix4x4F matrixRotation;

    PXMatrix4x4FRotationMatrixGenerate(&matrixRotation, vector3F);

    PXMatrix4x4FMultiply(matrix4x4F, &matrixRotation);
}

void PXAPI PXMatrix4x4FCopy(const PXMatrix4x4F* const matrixA, PXMatrix4x4F* const matrixResult)
{
    PXCopy(PXMatrix4x4F, matrixA, matrixResult);
}

void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F* const matrixData, const PXVector3F32* const vector3F)
{
    matrixData->Data[TransformX] += vector3F->X;
    matrixData->Data[TransformY] += vector3F->Y;
    matrixData->Data[TransformZ] += vector3F->Z;
}

void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F* const matrixData, const PXF32 x, const PXF32 y)
{
    matrixData->Data[TransformX] += x;
    matrixData->Data[TransformY] += y;
}

void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F* const matrixData, const PXF32 x, const PXF32 y)
{
    PXMatrix4x4FMoveXY(matrixData, x * (1- matrixData->Data[ScaleX]), y * (1 - matrixData->Data[ScaleY]));
}

void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F* const matrixData, const PXF32 x, const PXF32 y, const PXF32 z)
{
    matrixData->Data[TransformX] += x;
    matrixData->Data[TransformY] += y;
    matrixData->Data[TransformZ] += z;
}

void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F* const pxMatrix4x4F, const PXF32 scalar)
{
    pxMatrix4x4F->Data[ScaleX] *= scalar;
    pxMatrix4x4F->Data[ScaleY] *= scalar;
    pxMatrix4x4F->Data[ScaleZ] *= scalar;
}

/*
void PXAPI PXMatrix4x4FScaleByMargin(PXMatrix4x4F* const pxMatrix4x4F, const PXRectangleOffset* const pxMargin)
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

void PXAPI PXMatrix4x4FScaleByXY(PXMatrix4x4F* const pxMatrix4x4F, const PXF32 x, const PXF32 y)
{
    pxMatrix4x4F->Data[ScaleX] *= x;
    pxMatrix4x4F->Data[ScaleY] *= y;
}

void PXAPI PXMatrix4x4FScaleSet(PXMatrix4x4F* const pxMatrix4x4F, const PXVector3F32* const pxVector3F32)
{
    pxMatrix4x4F->Data[ScaleX] = pxVector3F32->X;
    pxMatrix4x4F->Data[ScaleY] = pxVector3F32->Y;
    pxMatrix4x4F->Data[ScaleZ] = pxVector3F32->Z;
}

void PXAPI PXMatrix4x4FScaleSetXY(PXMatrix4x4F* const pxMatrix4x4F, const PXF32 x, const PXF32 y)
{
    pxMatrix4x4F->Data[ScaleX] = x;
    pxMatrix4x4F->Data[ScaleY] = y;
}

void PXAPI PXMatrix4x4FScaleSetXYZ(PXMatrix4x4F* const xpMatrix4x4F, const PXF32 x, const PXF32 y, const PXF32 z)
{
    const PXVector3F32 PXVector3F32 = {x, y, z};

    PXMatrix4x4FScaleSet(xpMatrix4x4F, &PXVector3F32);
}

void PXAPI PXMatrix4x4FScaleGet(const PXMatrix4x4F* const matrixResult, PXVector3F32* const pxVector3F32)
{
    pxVector3F32->X = matrixResult->Data[ScaleX];
    pxVector3F32->Y = matrixResult->Data[ScaleY];
    pxVector3F32->Z = matrixResult->Data[ScaleZ];
}

void PXAPI PXMatrix4x4FScaleGetXYZ(const PXMatrix4x4F* const matrixResult, PXF32* const x, PXF32* const y, PXF32* const z)
{
    *x = matrixResult->Data[ScaleX];
    *y = matrixResult->Data[ScaleY];
    *z = matrixResult->Data[ScaleZ];
}

void PXAPI PXMatrix4x4FOrthographic(PXMatrix4x4F* const matrix4x4F, const PXF32 left, const PXF32 right, const PXF32 bottom, const PXF32 top, const PXF32 nearPlane, const PXF32 farPlane)
{
    PXMatrix4x4FIdentity(matrix4x4F);

    matrix4x4F->Data[ScaleX] = (2.0f) / (right - left);
    matrix4x4F->Data[ScaleY] = (2.0f) / (top - bottom);
    matrix4x4F->Data[ScaleZ] = -(2.0f) / (farPlane - nearPlane);
    matrix4x4F->Data[TransformX] = -(right + left) / (right - left);
    matrix4x4F->Data[TransformY] = -(top + bottom) / (top - bottom);
    matrix4x4F->Data[TransformZ] = -(farPlane + nearPlane) / (farPlane - nearPlane);
}

void PXAPI PXMatrix4x4FPerspective(PXMatrix4x4F* const matrix4x4F, const PXF32 fielfOfView, const PXF32 aspectRatio, const PXF32 nearPlane, const PXF32 farPlane)
{
    const PXF32 difference = farPlane - nearPlane;
    const PXF32 fielfOfViewRadians = PXMathDegreeToRadians(fielfOfView);
    const PXF32 tanHalfFovy = PXMathTangensRADF32(fielfOfViewRadians / 2.0f);

    PXMatrix4x4FIdentity(matrix4x4F);

    matrix4x4F->Data[ScaleX] = (1) / (aspectRatio * tanHalfFovy);
    matrix4x4F->Data[ScaleY] = (1) / (tanHalfFovy);
    matrix4x4F->Data[ScaleZ] = -(farPlane + nearPlane) / difference;
    matrix4x4F->Data[11] = -(1);
    matrix4x4F->Data[TransformZ] = -((2) * farPlane * nearPlane) / difference;
}

PXBool PXAPI PXMatrix4x4FInverse(PXMatrix4x4F* const matrix4x4F)
{
    PXMatrix4x4F temp;
    double det = 0;

    temp.Data[0] = matrix4x4F->Data[5] * matrix4x4F->Data[10] * matrix4x4F->Data[15] - matrix4x4F->Data[5] * matrix4x4F->Data[11] * matrix4x4F->Data[14] - matrix4x4F->Data[9] * matrix4x4F->Data[6] * matrix4x4F->Data[15] + matrix4x4F->Data[9] * matrix4x4F->Data[7] * matrix4x4F->Data[14] + matrix4x4F->Data[13] * matrix4x4F->Data[6] * matrix4x4F->Data[11] - matrix4x4F->Data[13] * matrix4x4F->Data[7] * matrix4x4F->Data[10];
    temp.Data[4] = -matrix4x4F->Data[4] * matrix4x4F->Data[10] * matrix4x4F->Data[15] + matrix4x4F->Data[4] * matrix4x4F->Data[11] * matrix4x4F->Data[14] + matrix4x4F->Data[8] * matrix4x4F->Data[6] * matrix4x4F->Data[15] - matrix4x4F->Data[8] * matrix4x4F->Data[7] * matrix4x4F->Data[14] - matrix4x4F->Data[12] * matrix4x4F->Data[6] * matrix4x4F->Data[11] + matrix4x4F->Data[12] * matrix4x4F->Data[7] * matrix4x4F->Data[10];
    temp.Data[8] = matrix4x4F->Data[4] * matrix4x4F->Data[9] * matrix4x4F->Data[15] - matrix4x4F->Data[4] * matrix4x4F->Data[11] * matrix4x4F->Data[13] - matrix4x4F->Data[8] * matrix4x4F->Data[5] * matrix4x4F->Data[15] + matrix4x4F->Data[8] * matrix4x4F->Data[7] * matrix4x4F->Data[13] + matrix4x4F->Data[12] * matrix4x4F->Data[5] * matrix4x4F->Data[11] - matrix4x4F->Data[12] * matrix4x4F->Data[7] * matrix4x4F->Data[9];
    temp.Data[12] = -matrix4x4F->Data[4] * matrix4x4F->Data[9] * matrix4x4F->Data[14] + matrix4x4F->Data[4] * matrix4x4F->Data[10] * matrix4x4F->Data[13] + matrix4x4F->Data[8] * matrix4x4F->Data[5] * matrix4x4F->Data[14] - matrix4x4F->Data[8] * matrix4x4F->Data[6] * matrix4x4F->Data[13] - matrix4x4F->Data[12] * matrix4x4F->Data[5] * matrix4x4F->Data[10] + matrix4x4F->Data[12] * matrix4x4F->Data[6] * matrix4x4F->Data[9];
    temp.Data[1] = -matrix4x4F->Data[1] * matrix4x4F->Data[10] * matrix4x4F->Data[15] + matrix4x4F->Data[1] * matrix4x4F->Data[11] * matrix4x4F->Data[14] + matrix4x4F->Data[9] * matrix4x4F->Data[2] * matrix4x4F->Data[15] - matrix4x4F->Data[9] * matrix4x4F->Data[3] * matrix4x4F->Data[14] - matrix4x4F->Data[13] * matrix4x4F->Data[2] * matrix4x4F->Data[11] + matrix4x4F->Data[13] * matrix4x4F->Data[3] * matrix4x4F->Data[10];
    temp.Data[5] = matrix4x4F->Data[0] * matrix4x4F->Data[10] * matrix4x4F->Data[15] - matrix4x4F->Data[0] * matrix4x4F->Data[11] * matrix4x4F->Data[14] - matrix4x4F->Data[8] * matrix4x4F->Data[2] * matrix4x4F->Data[15] + matrix4x4F->Data[8] * matrix4x4F->Data[3] * matrix4x4F->Data[14] + matrix4x4F->Data[12] * matrix4x4F->Data[2] * matrix4x4F->Data[11] - matrix4x4F->Data[12] * matrix4x4F->Data[3] * matrix4x4F->Data[10];
    temp.Data[9] = -matrix4x4F->Data[0] * matrix4x4F->Data[9] * matrix4x4F->Data[15] + matrix4x4F->Data[0] * matrix4x4F->Data[11] * matrix4x4F->Data[13] + matrix4x4F->Data[8] * matrix4x4F->Data[1] * matrix4x4F->Data[15] - matrix4x4F->Data[8] * matrix4x4F->Data[3] * matrix4x4F->Data[13] - matrix4x4F->Data[12] * matrix4x4F->Data[1] * matrix4x4F->Data[11] + matrix4x4F->Data[12] * matrix4x4F->Data[3] * matrix4x4F->Data[9];
    temp.Data[13] = matrix4x4F->Data[0] * matrix4x4F->Data[9] * matrix4x4F->Data[14] - matrix4x4F->Data[0] * matrix4x4F->Data[10] * matrix4x4F->Data[13] - matrix4x4F->Data[8] * matrix4x4F->Data[1] * matrix4x4F->Data[14] + matrix4x4F->Data[8] * matrix4x4F->Data[2] * matrix4x4F->Data[13] + matrix4x4F->Data[12] * matrix4x4F->Data[1] * matrix4x4F->Data[10] - matrix4x4F->Data[12] * matrix4x4F->Data[2] * matrix4x4F->Data[9];
    temp.Data[2] = matrix4x4F->Data[1] * matrix4x4F->Data[6] * matrix4x4F->Data[15] - matrix4x4F->Data[1] * matrix4x4F->Data[7] * matrix4x4F->Data[14] - matrix4x4F->Data[5] * matrix4x4F->Data[2] * matrix4x4F->Data[15] + matrix4x4F->Data[5] * matrix4x4F->Data[3] * matrix4x4F->Data[14] + matrix4x4F->Data[13] * matrix4x4F->Data[2] * matrix4x4F->Data[7] - matrix4x4F->Data[13] * matrix4x4F->Data[3] * matrix4x4F->Data[6];
    temp.Data[6] = -matrix4x4F->Data[0] * matrix4x4F->Data[6] * matrix4x4F->Data[15] + matrix4x4F->Data[0] * matrix4x4F->Data[7] * matrix4x4F->Data[14] + matrix4x4F->Data[4] * matrix4x4F->Data[2] * matrix4x4F->Data[15] - matrix4x4F->Data[4] * matrix4x4F->Data[3] * matrix4x4F->Data[14] - matrix4x4F->Data[12] * matrix4x4F->Data[2] * matrix4x4F->Data[7] + matrix4x4F->Data[12] * matrix4x4F->Data[3] * matrix4x4F->Data[6];
    temp.Data[10] = matrix4x4F->Data[0] * matrix4x4F->Data[5] * matrix4x4F->Data[15] - matrix4x4F->Data[0] * matrix4x4F->Data[7] * matrix4x4F->Data[13] - matrix4x4F->Data[4] * matrix4x4F->Data[1] * matrix4x4F->Data[15] + matrix4x4F->Data[4] * matrix4x4F->Data[3] * matrix4x4F->Data[13] + matrix4x4F->Data[12] * matrix4x4F->Data[1] * matrix4x4F->Data[7] - matrix4x4F->Data[12] * matrix4x4F->Data[3] * matrix4x4F->Data[5];
    temp.Data[14] = -matrix4x4F->Data[0] * matrix4x4F->Data[5] * matrix4x4F->Data[14] + matrix4x4F->Data[0] * matrix4x4F->Data[6] * matrix4x4F->Data[13] + matrix4x4F->Data[4] * matrix4x4F->Data[1] * matrix4x4F->Data[14] - matrix4x4F->Data[4] * matrix4x4F->Data[2] * matrix4x4F->Data[13] - matrix4x4F->Data[12] * matrix4x4F->Data[1] * matrix4x4F->Data[6] + matrix4x4F->Data[12] * matrix4x4F->Data[2] * matrix4x4F->Data[5];
    temp.Data[3] = -matrix4x4F->Data[1] * matrix4x4F->Data[6] * matrix4x4F->Data[11] + matrix4x4F->Data[1] * matrix4x4F->Data[7] * matrix4x4F->Data[10] + matrix4x4F->Data[5] * matrix4x4F->Data[2] * matrix4x4F->Data[11] - matrix4x4F->Data[5] * matrix4x4F->Data[3] * matrix4x4F->Data[10] - matrix4x4F->Data[9] * matrix4x4F->Data[2] * matrix4x4F->Data[7] + matrix4x4F->Data[9] * matrix4x4F->Data[3] * matrix4x4F->Data[6];
    temp.Data[7] = matrix4x4F->Data[0] * matrix4x4F->Data[6] * matrix4x4F->Data[11] - matrix4x4F->Data[0] * matrix4x4F->Data[7] * matrix4x4F->Data[10] - matrix4x4F->Data[4] * matrix4x4F->Data[2] * matrix4x4F->Data[11] + matrix4x4F->Data[4] * matrix4x4F->Data[3] * matrix4x4F->Data[10] + matrix4x4F->Data[8] * matrix4x4F->Data[2] * matrix4x4F->Data[7] - matrix4x4F->Data[8] * matrix4x4F->Data[3] * matrix4x4F->Data[6];
    temp.Data[11] = -matrix4x4F->Data[0] * matrix4x4F->Data[5] * matrix4x4F->Data[11] + matrix4x4F->Data[0] * matrix4x4F->Data[7] * matrix4x4F->Data[9] + matrix4x4F->Data[4] * matrix4x4F->Data[1] * matrix4x4F->Data[11] - matrix4x4F->Data[4] * matrix4x4F->Data[3] * matrix4x4F->Data[9] - matrix4x4F->Data[8] * matrix4x4F->Data[1] * matrix4x4F->Data[7] + matrix4x4F->Data[8] * matrix4x4F->Data[3] * matrix4x4F->Data[5];
    temp.Data[15] = matrix4x4F->Data[0] * matrix4x4F->Data[5] * matrix4x4F->Data[10] - matrix4x4F->Data[0] * matrix4x4F->Data[6] * matrix4x4F->Data[9] - matrix4x4F->Data[4] * matrix4x4F->Data[1] * matrix4x4F->Data[10] + matrix4x4F->Data[4] * matrix4x4F->Data[2] * matrix4x4F->Data[9] + matrix4x4F->Data[8] * matrix4x4F->Data[1] * matrix4x4F->Data[6] - matrix4x4F->Data[8] * matrix4x4F->Data[2] * matrix4x4F->Data[5];

    det = matrix4x4F->Data[0] * temp.Data[0] + matrix4x4F->Data[1] * temp.Data[4] + matrix4x4F->Data[2] * temp.Data[8] + matrix4x4F->Data[3] * temp.Data[12];

    if (det == 0)
        return PXNo;

    det = 1.0 / det;

    for (PXSize i = 0; i < 16u; ++i)
    {
        matrix4x4F->Data[i] = temp.Data[i] * det;
    }

    return PXYes;
}

void PXAPI PXMatrix4x4FTranpose(PXMatrix4x4F* const matrix4x4F)
{
    //NumberType a = this->Data[0];
    const PXF32 b = matrix4x4F->Data[1];
    const PXF32 c = matrix4x4F->Data[2];
    const PXF32 d = matrix4x4F->Data[3];

    const PXF32 e = matrix4x4F->Data[4];
    //NumberType f = this->Data[5];
    const PXF32 g = matrix4x4F->Data[6];
    const PXF32 h = matrix4x4F->Data[7];

    const PXF32 i = matrix4x4F->Data[8];
    const PXF32 j = matrix4x4F->Data[9];
    //NumberType k = this->Data[10];
    const PXF32 l = matrix4x4F->Data[11];

    const PXF32 m = matrix4x4F->Data[12];
    const PXF32 n = matrix4x4F->Data[13];
    const PXF32 o = matrix4x4F->Data[14];
    //NumberType p = this->Data[15];

    //this->Data[0] = a;
    matrix4x4F->Data[1] = e;
    matrix4x4F->Data[2] = i;
    matrix4x4F->Data[3] = m;

    matrix4x4F->Data[4] = b;
    //this->Data[5] = f;
    matrix4x4F->Data[6] = j;
    matrix4x4F->Data[7] = n;

    matrix4x4F->Data[8] = c;
    matrix4x4F->Data[9] = g;
    //this->Data[10] = k;
    matrix4x4F->Data[11] = o;

    matrix4x4F->Data[12] = d;
    matrix4x4F->Data[13] = h;
    matrix4x4F->Data[14] = l;
    //this->Data[15] = p;
}

void PXAPI PXMatrix4x4FLookAt(PXMatrix4x4F* const matrix4x4F, const PXVector3F32* const eye, const PXVector3F32* const center, const PXVector3F32* const up)
{
    PXVector3F32 centereye = *center;
    PXVector3F32 frontUpCross;
    PXVector3F32 u;

    PXVector3F32Substract(&centereye, eye);
    PXVector3F32Normalize(&centereye);
    PXVector3F32CrossProduct(&frontUpCross, up, &centereye);

    PXVector3F32Normalize(&frontUpCross);
    PXVector3F32CrossProduct(&u, &centereye, &frontUpCross);

    matrix4x4F->Data[XAxisX] = frontUpCross.X;
    matrix4x4F->Data[XAxisY] = frontUpCross.Y;
    matrix4x4F->Data[XAxisZ] = frontUpCross.Z;
    matrix4x4F->Data[XAxisW] = 0;

    matrix4x4F->Data[YAxisX] = u.X;
    matrix4x4F->Data[YAxisY] = u.Y;
    matrix4x4F->Data[YAxisZ] = u.Z;
    matrix4x4F->Data[YAxisW] = 0;

    matrix4x4F->Data[ZAxisX] = -centereye.X;
    matrix4x4F->Data[ZAxisY] = -centereye.Y;
    matrix4x4F->Data[ZAxisZ] = -centereye.Z;
    matrix4x4F->Data[ZAxisW] = 0;

    matrix4x4F->Data[WAxisX] = 0;
    matrix4x4F->Data[WAxisY] = 0;
    matrix4x4F->Data[WAxisZ] = 0;
    matrix4x4F->Data[WAxisW] = 0;

    matrix4x4F->Data[TransformX] = -PXVector3F32DotProduct(&frontUpCross, eye);
    matrix4x4F->Data[TransformY] = -PXVector3F32DotProduct(&u, eye);
    matrix4x4F->Data[TransformZ] = PXVector3F32DotProduct(&centereye, eye);
    matrix4x4F->Data[TransformW] = 1;
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
