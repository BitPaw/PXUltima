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

void PXAPI PXVector2FConstruct(PXVector2F* const vector2F)
{
	vector2F->X = 0;
	vector2F->Y = 0;
}

void PXAPI PXVector3FConstruct(PXVector3F* const vector3F)
{
	vector3F->X = 0;
	vector3F->Y = 0;
	vector3F->Z = 0;
}

void PXAPI PXVector4FConstruct(PXVector4F* const vector4F)
{
	vector4F->X = 0;
	vector4F->Y = 0;
	vector4F->Z = 0;
	vector4F->W = 0;
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

void PXAPI PXVector4FMultiplyXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult)
{
	vectorResult->X = vector->X * x;
	vectorResult->Y = vector->Y * y;
	vectorResult->Z = vector->Z * z;
	vectorResult->W = vector->W * w;
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

void PXAPI PXVector3FInterpolate(const PXVector3F* const vectorA, const PXVector3F* const vectorB, const float speed, PXVector3F* const vectorResult)
{
	// A * t + (1 - t) * B

	PXVector3F deltaA;
	PXVector3F deltaB;

	PXVector3FAdd(&deltaA, vectorA);
	PXVector3FAdd(&deltaB, vectorB);
	PXVector3FMultiplyS(&deltaA, 1 - speed);
	PXVector3FMultiplyS(&deltaB, speed);

	PXVector3FSetXYZ(vectorResult, 0, 0, 0);
	PXVector3FAdd(vectorResult, &deltaA);
	PXVector3FAdd(vectorResult, &deltaB);
}