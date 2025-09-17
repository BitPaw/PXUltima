#include "PXVector.h"

#include <PX/Math/PXMath.h>

PXF32 PXAPI PXVector2F32DistanceTo(PXVector2F32* const vectorA, PXVector2F32* const vectorB)
{
    // d = Root((x2?x1)^2 + (y2?y1)^2)
    return PXMathRootSquareF64(PXMathPowerOfTwo(vectorB->X - vectorA->X) + PXMathPowerOfTwo(vectorB->Y - vectorA->X));
}

PXF32 PXAPI PXVector3F32Length(const PXVector3F32* const vector)
{
    const PXF32 sum = vector->X * vector->X + vector->Y * vector->Y + vector->Z * vector->Z; // sum = x * x + y * y + z * z
    const PXF32 sqroot = PXMathRootSquareF64(sum);

    return sqroot;
}

void PXAPI PXVector3F32CrossProduct(PXVector3F32* const vectorResult, const PXVector3F32* const vectorA, const PXVector3F32* const vectorB)
{
    vectorResult->X = vectorA->Y * vectorB->Z - vectorA->Z * vectorB->Y;
    vectorResult->Y = vectorA->Z * vectorB->X - vectorA->X * vectorB->Z;
    vectorResult->Z = vectorA->X * vectorB->Y - vectorA->Y * vectorB->X;
}

PXF32 PXAPI PXVector2F32DotProduct(const PXVector2F32* const vectorA, const PXVector2F32* const vectorB)
{
    // a · b = ax × bx + ay × by
    return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y;
}

PXF32 PXAPI PXVector3F32DotProduct(const PXVector3F32* const vectorA, const PXVector3F32* const vectorB)
{
    return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y + vectorA->Z * vectorB->Z;
}

void PXAPI PXVector3F32Set(PXVector3F32* const pxVector3F32, const PXVector3F32* const pxVector3F32Source)
{
    pxVector3F32->X = pxVector3F32Source->X;
    pxVector3F32->Y = pxVector3F32Source->Y;
    pxVector3F32->Z = pxVector3F32Source->Z;
}

void PXAPI PXVector2F32Set(PXVector2F32* const vector, const PXVector2F32* const vectorSet)
{
    vector->X = vectorSet->X;
    vector->Y = vectorSet->Y;
}

void PXAPI PXVector2F32SetXY(PXVector2F32* const vector, const PXF32 x, const PXF32 y)
{
    const PXVector2F32 pxVectorData = { x, y };

    PXVector2F32Set(vector, &pxVectorData);
}

void PXAPI PXVector2F32Add(PXVector2F32* const vector, const PXVector2F32* const pxVectorData)
{
    vector->X += pxVectorData->X;
    vector->Y += pxVectorData->Y;
}

void PXAPI PXVector2F32AddXY(PXVector2F32* const vector, const PXF32 x, const PXF32 y)
{
    const PXVector2F32 pxVectorData = { x, y };

    PXVector2F32Add(vector, &pxVectorData);
}

void PXAPI PXVector2F32Multiply(PXVector2F32* const vector, const PXVector2F32* const pxVectorData)
{
    vector->X *= pxVectorData->X;
    vector->Y *= pxVectorData->Y;
}

void PXAPI PXVector2F32MultiplyXY(PXVector2F32* const vector, const PXF32 x, const PXF32 y)
{
    const PXVector2F32 pxVectorData = { x, y };

    PXVector2F32Multiply(vector, &pxVectorData);
}

void PXAPI PXVector3F32SetXYZ(PXVector3F32* const vector, const PXF32 x, const PXF32 y, const PXF32 z)
{
    vector->X = x;
    vector->Y = y;
    vector->Z = z;
}

void PXAPI PXVector3F32Absolute(PXVector3F32* const pxVector3F32)
{
    pxVector3F32->X = PXMathAbsolute(pxVector3F32->X);
    pxVector3F32->Y = PXMathAbsolute(pxVector3F32->Y);
    pxVector3F32->Z = PXMathAbsolute(pxVector3F32->Z);
}

void PXAPI PXVector4F32SetXYZW(PXVector4F32* const vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w)
{
    vector->X = x;
    vector->Y = y;
    vector->Z = z;
    vector->W = w;
}

void PXAPI PXVector3F32Substract(PXVector3F32* const pxVector3F32, const PXVector3F32* const pxVector3F32Input)
{
    pxVector3F32->X -= pxVector3F32Input->X;
    pxVector3F32->Y -= pxVector3F32Input->Y;
    pxVector3F32->Z -= pxVector3F32Input->Z;
}

void PXAPI PXVector3F32Multiply(PXVector3F32* const vector, PXVector3F32* const vectorResult)
{
    vectorResult->X *= vector->X;
    vectorResult->Y *= vector->Y;
    vectorResult->Z *= vector->Z;
}

void PXAPI PXVector3F32MultiplyXYZ(PXVector3F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z)
{
    pxVector3F32->X *= x;
    pxVector3F32->Y *= y;
    pxVector3F32->Z *= z;
}

void PXAPI PXVector3F32MultiplyS(PXVector3F32* const vector, const PXF32 scalar)
{
    vector->X *= scalar;
    vector->Y *= scalar;
    vector->Z *= scalar;
}

void PXAPI PXVector4F32MultiplyXYZW(PXVector4F32* const PXVector4F32, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w)
{
    PXVector4F32->X *= x;
    PXVector4F32->Y *= y;
    PXVector4F32->Z *= z;
    PXVector4F32->W *= w;
}

void PXAPI PXVector3F32Divide(PXVector3F32* const pxVector3F32, const PXVector3F32* const vectorB)
{
    pxVector3F32->X /= vectorB->X;
    pxVector3F32->Y /= vectorB->Y;
    pxVector3F32->Z /= vectorB->Z;
}

void PXAPI PXVector3F32DivideS(PXVector3F32* const pxVector3F32, const PXF32 scalar)
{
    pxVector3F32->X /= scalar;
    pxVector3F32->Y /= scalar;
    pxVector3F32->Z /= scalar;
}

void PXAPI PXVector3F32Normalize(PXVector3F32* const pxVector3F32)
{
    const PXF32 length = PXVector3F32Length(pxVector3F32);

    if (length)
    {
        pxVector3F32->X /= length;
        pxVector3F32->Y /= length;
        pxVector3F32->Z /= length;
    }
}

void PXAPI PXVector3F32Add(PXVector3F32* const pxVector3F32, const PXVector3F32* const vectorB)
{
    pxVector3F32->X += vectorB->X;
    pxVector3F32->Y += vectorB->Y;
    pxVector3F32->Z += vectorB->Z;
}

void PXAPI PXVector3F32AddXYZ(PXVector3F32* const pxVector3F32, const PXF32 x, const PXF32 y, const PXF32 z)
{
    pxVector3F32->X += x;
    pxVector3F32->Y += y;
    pxVector3F32->Z += z;
}

void PXAPI PXVector4F32AddXYZW(const PXVector4F32* const vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w, PXVector4F32* const vectorResult)
{
    vectorResult->X = vector->X + x;
    vectorResult->Y = vector->Y + y;
    vectorResult->Z = vector->Z + z;
    vectorResult->W = vector->W + w;
}

void PXAPI PXVector3F32Interpolate(PXVector3F32* const vectorA, const PXVector3F32* const vectorB, PXF32 speed)
{
    // A * t + (1 - t) * B

    PXVector3F32 deltaA = { 0,0,0 };
    PXVector3F32 deltaB = { 0,0,0 };

    PXVector3F32Add(&deltaA, vectorA);
    PXVector3F32Add(&deltaB, vectorB);
    PXVector3F32MultiplyS(&deltaA, 1 - speed);
    PXVector3F32MultiplyS(&deltaB, speed);

    PXVector3F32SetXYZ(vectorA, 0, 0, 0);
    PXVector3F32Add(vectorA, &deltaA);
    PXVector3F32Add(vectorA, &deltaB);
}

void PXAPI PXVector4Demote(PXVector4F32* const pxVectorIn, PXVector3F32* pxVectorOut, const PXF32 distance)
{
    const float w = 1.0f / (distance - pxVectorIn->W);

    pxVectorOut->X = pxVectorIn->X * w;
    pxVectorOut->Y = pxVectorIn->Y * w;
    pxVectorOut->Z = pxVectorIn->Z * w;
}

void PXAPI PXVector3Demote(PXVector3F32* const pxVectorIn, PXVector2F32* pxVectorOut, const PXF32 distance)
{
    const float z = 1.0f / (distance - pxVectorIn->Z);

    pxVectorOut->X = pxVectorIn->X * z;
    pxVectorOut->Y = pxVectorIn->Y * z;
}

void PXAPI PXVector3FNormal
(
    PXVector3F32* const res,
    const PXVector3F32* const a,
    const PXVector3F32* const b, 
    const PXVector3F32* const c
)
{
    PXVector3F32 u = { b->X - a->X, b->Y - a->Y, b->Z - a->Z };
    PXVector3F32 v = { c->X - a->X, c->Y - a->Y, c->Z - a->Z };

    res->X = u.Y * v.Z - u.Z * v.Y;
    res->Y = u.Z * v.X - u.X * v.Z;
    res->Z = u.X * v.Y - u.Y * v.X;

    PXVector3F32Normalize(res);
}

void PXAPI PXVector4FNormal
(
    PXVector3F32* const res,
    const PXVector4F32* const a,
    const PXVector4F32* const b,
    const PXVector4F32* const c,
    const PXF32 distance
)
{
    PXVector3F32 data[3];

    PXVector4Demote(a, &data[0], distance);
    PXVector4Demote(b, &data[1], distance);
    PXVector4Demote(c, &data[2], distance);

    PXVector3FNormal(res, &data[0], &data[1], &data[2]);
}
