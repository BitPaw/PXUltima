#include "PXMatrix.h"

#include <Math/PXMath.h>
#include <OS/Memory/PXMemory.h>
#include <Media/PXResource.h>

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

void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F* const matrix, PXVector3F* const position)
{
    position->X = matrix->Data[TransformX];
    position->Y = matrix->Data[TransformY];
    position->Z = matrix->Data[TransformZ];
}

void PXAPI PXMatrix4x4FPositionSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const position)
{
    matrix4x4F->Data[TransformX] = position->X;
    matrix4x4F->Data[TransformY] = position->Y;
    matrix4x4F->Data[TransformZ] = position->Z;
}

void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F* const matrix, PXVector3F* const position)
{
    // Pitch
    position->Y = -PXMathSinusA(matrix->Data[ZAxisX]); // 31

    if(matrix->Data[ZAxisX] == 1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathTangensArc2(-matrix->Data[XAxisY], -matrix->Data[XAxisZ]); // -12 -13
    }
    else if(matrix->Data[ZAxisX] == -1.0f)
    {
        // Yaw
        position->X = 0;

        // Roll
        position->Z = PXMathTangensArc2(matrix->Data[YAxisX], matrix->Data[XAxisX]); // 12 13
    }
    else
    {
        // Yaw
        position->X = PXMathTangensArc2(matrix->Data[YAxisX], matrix->Data[XAxisX]); // 21 11

        // Roll
        position->Z = PXMathTangensArc2(matrix->Data[ZAxisY], matrix->Data[ZAxisZ]); // 32 33
    }
}

void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const position)
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

void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const position)
{
    PXMatrix4x4F xRotation;
    PXMatrix4x4F yRotation;
    PXMatrix4x4F zRotation;

    PXMatrix4x4FIdentity(&xRotation);
    PXMatrix4x4FIdentity(&yRotation);
    PXMatrix4x4FIdentity(&zRotation);

    //-----<X ROT>-----
    {
        const float cosResult = PXMathCosinus(position->X);
        const float sinResult = PXMathSinus(position->X);

        xRotation.Data[5] = cosResult;
        xRotation.Data[6] = sinResult;
        xRotation.Data[9] = -sinResult;
        xRotation.Data[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const float cosResult = PXMathCosinus(-position->Y);
        const float sinResult = PXMathSinus(-position->Y);

        yRotation.Data[0] = cosResult;
        yRotation.Data[2] = -sinResult;
        yRotation.Data[8] = sinResult;
        yRotation.Data[10] = cosResult;
    }

    //-----<X ROT>-----
    {
        const float cosResult = PXMathCosinus(position->Z);
        const float sinResult = PXMathSinus(position->Z);

        zRotation.Data[0] = cosResult;
        zRotation.Data[1] = -sinResult;
        zRotation.Data[4] = sinResult;
        zRotation.Data[5] = cosResult;
    }

    // Gimble (result = xRotation * yRotation * zRotation;)
    {

        PXMatrix4x4FIdentity(matrix4x4F);

        PXMatrix4x4FMultiply(matrix4x4F, &zRotation, matrix4x4F);
        PXMatrix4x4FMultiply(matrix4x4F, &yRotation, matrix4x4F);
        PXMatrix4x4FMultiply(matrix4x4F, &xRotation, matrix4x4F);

        /*
                PXMatrix4x4F tempRotation;
        PXMatrix4x4FIdentity(&tempRotation);

        PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
        PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
        PXMatrix4x4FMultiply(&tempRotation, &xRotation, matrix4x4F);

        */
    }
}

void PXAPI PXMatrix4x4FMultiply(const PXMatrix4x4F* matrixA, const PXMatrix4x4F* matrixB, PXMatrix4x4F* const matrixResult)
{
    float a = matrixA->Data[0];
    float b = matrixA->Data[1];
    float c = matrixA->Data[2];
    float d = matrixA->Data[3];
    float e = matrixA->Data[4];
    float f = matrixA->Data[5];
    float g = matrixA->Data[6];
    float h = matrixA->Data[7];
    float i = matrixA->Data[8];
    float j = matrixA->Data[9];
    float k = matrixA->Data[10];
    float l = matrixA->Data[11];
    float m = matrixA->Data[12];
    float n = matrixA->Data[13];
    float o = matrixA->Data[14];
    float p = matrixA->Data[15];

    float A = matrixB->Data[0];
    float B = matrixB->Data[1];
    float C = matrixB->Data[2];
    float D = matrixB->Data[3];
    float E = matrixB->Data[4];
    float F = matrixB->Data[5];
    float G = matrixB->Data[6];
    float H = matrixB->Data[7];
    float I = matrixB->Data[8];
    float J = matrixB->Data[9];
    float K = matrixB->Data[10];
    float L = matrixB->Data[11];
    float M = matrixB->Data[12];
    float N = matrixB->Data[13];
    float O = matrixB->Data[14];
    float P = matrixB->Data[15];

    matrixResult->Data[0] = a * A + b * E + c * I + d * M;
    matrixResult->Data[1] = e * A + f * E + g * I + h * M;
    matrixResult->Data[2] = i * A + j * E + k * I + l * M;
    matrixResult->Data[3] = m * A + n * E + o * I + p * M;

    matrixResult->Data[4] = a * B + b * F + c * J + d * N;
    matrixResult->Data[5] = e * B + f * F + g * J + h * N;
    matrixResult->Data[6] = i * B + j * F + k * J + l * N;
    matrixResult->Data[7] = m * B + n * F + o * J + p * N;

    matrixResult->Data[8] = a * C + b * G + c * K + d * O;
    matrixResult->Data[9] = e * C + f * G + g * K + h * O;
    matrixResult->Data[10] = i * C + j * G + k * K + l * O;
    matrixResult->Data[11] = m * C + n * G + o * K + p * O;

    matrixResult->Data[12] = a * D + b * H + c * L + d * P;
    matrixResult->Data[13] = e * D + f * H + g * L + h * P;
    matrixResult->Data[14] = i * D + j * H + k * L + l * P;
    matrixResult->Data[15] = m * D + n * H + o * L + p * P;
}

void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const vector3F)
{
    PXMatrix4x4F matrixRotation;

    PXMatrix4x4FRotationMatrixGenerate(&matrixRotation, vector3F);

    PXMatrix4x4FMultiply(matrix4x4F, &matrixRotation, matrix4x4F);
}

void PXAPI PXMatrix4x4FCopy(const PXMatrix4x4F* const matrixA, PXMatrix4x4F* const matrixResult)
{
    PXCopy(PXMatrix4x4F, matrixA, matrixResult);
}

void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F* const matrixData, const PXVector3F* const vector3F)
{
    matrixData->Data[TransformX] += vector3F->X;
    matrixData->Data[TransformY] += vector3F->Y;
    matrixData->Data[TransformZ] += vector3F->Z;
}

void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F* const matrixData, const float x, const float y)
{
    matrixData->Data[TransformX] += x;
    matrixData->Data[TransformY] += y;
}

void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F* const matrixData, const float x, const float y)
{
    PXMatrix4x4FMoveXY(matrixData, x * (1- matrixData->Data[ScaleX]), y * (1 - matrixData->Data[ScaleY]));
}

void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F* const matrixData, const float x, const float y, const float z)
{
    matrixData->Data[TransformX] += x;
    matrixData->Data[TransformY] += y;
    matrixData->Data[TransformZ] += z;
}

void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F* const pxMatrix4x4F, const float scalar)
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

    const float offsetTotalWidth = (pxMargin->Left + pxMargin->Right) / 2.0f;
    const float offsetTotalHeight = (pxMargin->Top + pxMargin->Bottom) / 2.0f;

    pxMatrix4x4F->Data[ScaleX] *= offsetTotalWidth;
    pxMatrix4x4F->Data[ScaleY] *= offsetTotalHeight;

    //pxMatrix4x4F->Data[TransformX] += offset.Left + offset.Right;
    //pxMatrix4x4F->Data[TransformY] -= offset.Top + offset.Bottom;



#else
    const float moveX = 1 - pxMargin->Right;
    const float shrinkX = 1 - pxMargin->Left;
    const float moveY = 1 - pxMargin->Top;
    const float shrinkY = 1 - pxMargin->Bottom;

    const float totalWidth = 1 - (moveX + shrinkX);
    const float totalHeight = 1 - (moveY + shrinkY);

    pxMatrix4x4F->Data[TransformX] += (moveX - shrinkX);
    pxMatrix4x4F->Data[TransformY] -= (moveY - shrinkY);
    //matrixResult->Data[TransformZ] = 1;

    pxMatrix4x4F->Data[ScaleX] *= totalWidth;
    pxMatrix4x4F->Data[ScaleY] *= totalHeight;
    //pxMatrix4x4F->Data[ScaleZ] = 1.0f;
#endif // 0



}
*/

void PXAPI PXMatrix4x4FScaleByXY(PXMatrix4x4F* const pxMatrix4x4F, const float x, const float y)
{
    pxMatrix4x4F->Data[ScaleX] *= x;
    pxMatrix4x4F->Data[ScaleY] *= y;
}

void PXAPI PXMatrix4x4FScaleSet(PXMatrix4x4F* const pxMatrix4x4F, const PXVector3F* const pxVector3F)
{
    pxMatrix4x4F->Data[ScaleX] = pxVector3F->X;
    pxMatrix4x4F->Data[ScaleY] = pxVector3F->Y;
    pxMatrix4x4F->Data[ScaleZ] = pxVector3F->Z;
}

void PXAPI PXMatrix4x4FScaleSetXY(PXMatrix4x4F* const pxMatrix4x4F, const float x, const float y)
{
    pxMatrix4x4F->Data[ScaleX] = x;
    pxMatrix4x4F->Data[ScaleY] = y;
}

void PXAPI PXMatrix4x4FScaleSetXYZ(PXMatrix4x4F* const xpMatrix4x4F, const float x, const float y, const float z)
{
    const PXVector3F pxVector3F = {x, y, z};

    PXMatrix4x4FScaleSet(xpMatrix4x4F, &pxVector3F);
}

void PXAPI PXMatrix4x4FScaleGet(const PXMatrix4x4F* const matrixResult, PXVector3F* const pxVector3F)
{
    pxVector3F->X = matrixResult->Data[ScaleX];
    pxVector3F->Y = matrixResult->Data[ScaleY];
    pxVector3F->Z = matrixResult->Data[ScaleZ];
}

void PXAPI PXMatrix4x4FScaleGetXYZ(const PXMatrix4x4F* const matrixResult, float* const x, float* const y, float* const z)
{
    *x = matrixResult->Data[ScaleX];
    *y = matrixResult->Data[ScaleY];
    *z = matrixResult->Data[ScaleZ];
}

void PXAPI PXMatrix4x4FOrthographic(PXMatrix4x4F* const matrix4x4F, const float left, const float right, const float bottom, const float top, const float nearPlane, const float farPlane)
{
    PXMatrix4x4FIdentity(matrix4x4F);

    matrix4x4F->Data[ScaleX] = (2.0f) / (right - left);
    matrix4x4F->Data[ScaleY] = (2.0f) / (top - bottom);
    matrix4x4F->Data[ScaleZ] = -(2.0f) / (farPlane - nearPlane);
    matrix4x4F->Data[TransformX] = -(right + left) / (right - left);
    matrix4x4F->Data[TransformY] = -(top + bottom) / (top - bottom);
    matrix4x4F->Data[TransformZ] = -(farPlane + nearPlane) / (farPlane - nearPlane);
}

void PXAPI PXMatrix4x4FPerspective(PXMatrix4x4F* const matrix4x4F, const float fielfOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
    const float difference = farPlane - nearPlane;
    const float fielfOfViewRadians = PXMathDegreeToRadians(fielfOfView);
    const float tanHalfFovy = PXMathTangens(fielfOfViewRadians / 2.0f);

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
    const float b = matrix4x4F->Data[1];
    const float c = matrix4x4F->Data[2];
    const float d = matrix4x4F->Data[3];

    const float e = matrix4x4F->Data[4];
    //NumberType f = this->Data[5];
    const float g = matrix4x4F->Data[6];
    const float h = matrix4x4F->Data[7];

    const float i = matrix4x4F->Data[8];
    const float j = matrix4x4F->Data[9];
    //NumberType k = this->Data[10];
    const float l = matrix4x4F->Data[11];

    const float m = matrix4x4F->Data[12];
    const float n = matrix4x4F->Data[13];
    const float o = matrix4x4F->Data[14];
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

void PXAPI PXMatrix4x4FLookAt(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const eye, const PXVector3F* const center, const PXVector3F* const up)
{
    PXVector3F centereye = *center;
    PXVector3F frontUpCross;
    PXVector3F u;

    PXVector3FSubstract(&centereye, eye);
    PXVector3FNormalize(&centereye);
    PXVector3FCrossProduct(&centereye, up, &frontUpCross);

    PXVector3FNormalize(&frontUpCross);
    PXVector3FCrossProduct(&frontUpCross, &centereye, &u);

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

    matrix4x4F->Data[TransformX] = -PXVector3FDotProduct(&frontUpCross, eye);
    matrix4x4F->Data[TransformY] = -PXVector3FDotProduct(&u, eye);
    matrix4x4F->Data[TransformZ] = PXVector3FDotProduct(&centereye, eye);
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
