#include "PXVector.h"

#include <Math/PXMath.h>

float PXAPI PXVector2FDistanceTo(PXVector2F* const vectorA, PXVector2F* const vectorB)
{
    // d = Root((x2?x1)^2 + (y2?y1)^2)
    return PXMathSquareRoot(PXMathPowerOfTwo(vectorB->X - vectorA->X) + PXMathPowerOfTwo(vectorB->Y - vectorA->X));
}

float PXAPI PXVector3FLength(const PXVector3F* const vector)
{
    const float sum = vector->X * vector->X + vector->Y * vector->Y + vector->Z * vector->Z; // sum = x * x + y * y + z * z
    const float sqroot = PXMathSquareRoot(sum);

    return sqroot;
}

void PXAPI PXVector3FCrossProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult)
{
    vectorResult->X = vectorA->Y * vectorB->Z - vectorA->Z * vectorB->Y;
    vectorResult->Y = vectorA->Z * vectorB->X - vectorA->X * vectorB->Z;
    vectorResult->Z = vectorA->X * vectorB->Y - vectorA->Y * vectorB->X;
}

float PXAPI PXVector2FDotProduct(const PXVector2F* const vectorA, const PXVector2F* const vectorB)
{
    // a · b = ax × bx + ay × by
    return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y;
}

float PXAPI PXVector3FDotProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB)
{
    return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y + vectorA->Z * vectorB->Z;
}

void PXAPI PXVector3FSet(PXVector3F* const pxVector3F, const PXVector3F* const pxVector3FSource)
{
    pxVector3F->X = pxVector3FSource->X;
    pxVector3F->Y = pxVector3FSource->Y;
    pxVector3F->Z = pxVector3FSource->Z;
}

void PXAPI PXVector2FSet(PXVector2F* const vector, const PXVector2F* const vectorSet)
{
    vector->X = vectorSet->X;
    vector->Y = vectorSet->Y;
}

void PXAPI PXVector2FSetXY(PXVector2F* const vector, const float x, const float y)
{
    const PXVector2F pxVectorData = { x, y };

    PXVector2FSet(vector, &pxVectorData);
}

void PXAPI PXVector2FAdd(PXVector2F* const vector, const PXVector2F* const pxVectorData)
{
    vector->X += pxVectorData->X;
    vector->Y += pxVectorData->Y;
}

void PXAPI PXVector2FAddXY(PXVector2F* const vector, const float x, const float y)
{
    const PXVector2F pxVectorData = { x, y };

    PXVector2FAdd(vector, &pxVectorData);
}

void PXAPI PXVector2FMultiply(PXVector2F* const vector, const PXVector2F* const pxVectorData)
{
    vector->X *= pxVectorData->X;
    vector->Y *= pxVectorData->Y;
}

void PXAPI PXVector2FMultiplyXY(PXVector2F* const vector, const float x, const float y)
{
    const PXVector2F pxVectorData = { x, y };

    PXVector2FMultiply(vector, &pxVectorData);
}

void PXAPI PXVector3FSetXYZ(PXVector3F* const vector, const float x, const float y, const float z)
{
    vector->X = x;
    vector->Y = y;
    vector->Z = z;
}

void PXAPI PXVector3FAbsolute(PXVector3F* const pxVector3F)
{
    pxVector3F->X = PXMathAbsolute(pxVector3F->X);
    pxVector3F->Y = PXMathAbsolute(pxVector3F->Y);
    pxVector3F->Z = PXMathAbsolute(pxVector3F->Z);
}

void PXAPI PXVector4FSetXYZW(PXVector4F* const vector, const float x, const float y, const float z, const float w)
{
    vector->X = x;
    vector->Y = y;
    vector->Z = z;
    vector->W = w;
}

void PXAPI PXVector3FSubstract(PXVector3F* const pxVector3F, const PXVector3F* const pxVector3FInput)
{
    pxVector3F->X -= pxVector3FInput->X;
    pxVector3F->Y -= pxVector3FInput->Y;
    pxVector3F->Z -= pxVector3FInput->Z;
}

void PXAPI PXVector3FMultiply(PXVector3F* const vector, PXVector3F* const vectorResult)
{
    vectorResult->X *= vector->X;
    vectorResult->Y *= vector->Y;
    vectorResult->Z *= vector->Z;
}

void PXAPI PXVector3FMultiplyXYZ(PXVector3F* const pxVector3F, const float x, const float y, const float z)
{
    pxVector3F->X *= x;
    pxVector3F->Y *= y;
    pxVector3F->Z *= z;
}

void PXAPI PXVector3FMultiplyS(PXVector3F* const vector, const float scalar)
{
    vector->X *= scalar;
    vector->Y *= scalar;
    vector->Z *= scalar;
}

void PXAPI PXVector4FMultiplyXYZW(PXVector4F* const pxVector4F, const float x, const float y, const float z, const float w)
{
    pxVector4F->X *= x;
    pxVector4F->Y *= y;
    pxVector4F->Z *= z;
    pxVector4F->W *= w;
}

void PXAPI PXVector3FDivide(PXVector3F* const pxVector3F, const PXVector3F* const vectorB)
{
    pxVector3F->X /= vectorB->X;
    pxVector3F->Y /= vectorB->Y;
    pxVector3F->Z /= vectorB->Z;
}

void PXAPI PXVector3FDivideS(PXVector3F* const pxVector3F, const float scalar)
{
    pxVector3F->X /= scalar;
    pxVector3F->Y /= scalar;
    pxVector3F->Z /= scalar;
}

void PXAPI PXVector3FNormalize(PXVector3F* const pxVector3F)
{
    const float length = PXVector3FLength(pxVector3F);

    if (length)
    {
        pxVector3F->X /= length;
        pxVector3F->Y /= length;
        pxVector3F->Z /= length;
    }
}

void PXAPI PXVector3FAdd(PXVector3F* const pxVector3F, const PXVector3F* const vectorB)
{
    pxVector3F->X += vectorB->X;
    pxVector3F->Y += vectorB->Y;
    pxVector3F->Z += vectorB->Z;
}

void PXAPI PXVector3FAddXYZ(PXVector3F* const pxVector3F, const float x, const float y, const float z)
{
    pxVector3F->X += x;
    pxVector3F->Y += y;
    pxVector3F->Z += z;
}

void PXAPI PXVector4FAddXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult)
{
    vectorResult->X = vector->X + x;
    vectorResult->Y = vector->Y + y;
    vectorResult->Z = vector->Z + z;
    vectorResult->W = vector->W + w;
}

void PXAPI PXVector3FInterpolate(PXVector3F* const vectorA, const PXVector3F* const vectorB, float speed)
{
    // A * t + (1 - t) * B

    PXVector3F deltaA = { 0,0,0 };
    PXVector3F deltaB = { 0,0,0 };

    PXVector3FAdd(&deltaA, vectorA);
    PXVector3FAdd(&deltaB, vectorB);
    PXVector3FMultiplyS(&deltaA, 1 - speed);
    PXVector3FMultiplyS(&deltaB, speed);

    PXVector3FSetXYZ(vectorA, 0, 0, 0);
    PXVector3FAdd(vectorA, &deltaA);
    PXVector3FAdd(vectorA, &deltaB);
}
