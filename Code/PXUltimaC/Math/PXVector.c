#include "PXVector.h"

#include <Math/PXMath.h>

void PXVector2FDistanceTo(PXVector2F* const vectorA, PXVector2F* const vectorB)
{
	// d = Root((x2?x1)^2 + (y2?y1)^2)
	return PXMathSquareRoot(PXMathPowerOfTwo(vectorB->X - vectorA->X) + PXMathPowerOfTwo(vectorB->Y - vectorA->X));
}

float PXVector3FLength(const PXVector3F* const vector)
{
	const float sum = vector->X * vector->X + vector->Y * vector->Y + vector->Z * vector->Z; // sum = x * x + y * y + z * z
	const float sqroot = PXMathSquareRoot(sum);

	return sqroot;
}

void PXVector3FCrossProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult)
{
	vectorResult->X = vectorA->Y * vectorB->Z - vectorA->Z * vectorB->Y;
	vectorResult->Y = vectorA->Z * vectorB->X - vectorA->X * vectorB->Z;
	vectorResult->Z = vectorA->X * vectorB->Y - vectorA->Y * vectorB->X;
}

float PXVector2FDotProduct(const PXVector2F* const vectorA, const PXVector2F* const vectorB)
{
	// a · b = ax × bx + ay × by
	return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y;
}

float PXVector3FDotProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB)
{
	return vectorA->X * vectorB->X + vectorA->Y * vectorB->Y + vectorA->Z * vectorB->Z;
}

void PXVector2FConstruct(PXVector2F* const vector2F)
{
	vector2F->X = 0;
	vector2F->Y = 0;
}

void PXVector3FConstruct(PXVector3F* const vector3F)
{
	vector3F->X = 0;
	vector3F->Y = 0;
	vector3F->Z = 0;
}

void PXVector4FConstruct(PXVector4F* const vector4F)
{
	vector4F->X = 0;
	vector4F->Y = 0;
	vector4F->Z = 0;
	vector4F->W = 0;
}

void PXVector2FSet(const PXVector2F* const vector, PXVector2F* const vectorSet, PXVector2F* const vectorResult)
{
	
}

void PXVector2FSetXY(const PXVector2F* const vector, const float x, const float y, PXVector2F* const vectorResult)
{
	vectorResult->X = vector->X - x;
	vectorResult->Y = vector->Y - y;
}

void PXVector3FSetXYZ(const PXVector3F* const vector, const float x, const float y, const float z, PXVector3F* const vectorResult)
{
	vectorResult->X = vector->X - x;
	vectorResult->Y = vector->Y - y;
	vectorResult->Z = vector->Z - z;
}

void PXVector4FSetXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult)
{
	vectorResult->X = vector->X - x;
	vectorResult->Y = vector->Y - y;
	vectorResult->Z = vector->Z - z;
	vectorResult->W = vector->W - w;
}

void PXVector3FSubstract(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult)
{
	vectorResult->X = vectorA->X - vectorB->X;
	vectorResult->Y = vectorA->Y - vectorB->Y;
	vectorResult->Z = vectorA->Z - vectorB->Z;
}

void PXVector2FMultiplyXY(const PXVector2F* const vector, const float x, const float y, PXVector2F* const vectorResult)
{
	vectorResult->X = vector->X * x;
	vectorResult->Y = vector->Y * y;
}

void PXVector3FMultiply(const PXVector3F* const vector, PXVector3F* const vectorResult)
{
	vectorResult->X *= vector->X;
	vectorResult->Y *= vector->Y;
	vectorResult->Z *= vector->Z;
}

void PXVector3FMultiplyXYZ(const PXVector3F* const vector, const float x, const float y, const float z, PXVector3F* const vectorResult)
{
	vectorResult->X = vector->X * x;
	vectorResult->Y = vector->Y * y;
	vectorResult->Z = vector->Z * z;
}

void PXVector3FMultiplyS(const PXVector3F* const vector, const float scalar, PXVector3F* const vectorResult)
{
	vectorResult->X = vector->X * scalar;
	vectorResult->Y = vector->Y * scalar;
	vectorResult->Z = vector->Z * scalar;
}

void PXVector4FMultiplyXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult)
{
	vectorResult->X = vector->X * x;
	vectorResult->Y = vector->Y * y;
	vectorResult->Z = vector->Z * z;
	vectorResult->W = vector->W * w;
}

void PXVector3FDivide(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult)
{
	vectorResult->X = vectorA->X / vectorB->X;
	vectorResult->Y = vectorA->Y / vectorB->Y;
	vectorResult->Z = vectorA->Z / vectorB->Z;
}

void PXVector3FDivideS(const PXVector3F* const vectorA, const float scalar, PXVector3F* const vectorResult)
{
	vectorResult->X = vectorA->X / scalar;
	vectorResult->Y = vectorA->Y / scalar;
	vectorResult->Z = vectorA->Z / scalar;
}

void PXVector3FNormalize(const PXVector3F* const vector, PXVector3F* const vectorResult)
{
	const float length = PXVector3FLength(vector);

	if (length)
	{
		vectorResult->X = vector->X / length;
		vectorResult->Y = vector->Y / length;
		vectorResult->Z = vector->Z / length;
	}
}

void PXVector2FAddXY(const PXVector2F* const vector, const float x, const float y, PXVector2F* const vectorResult)
{
	vectorResult->X = vector->X + x;
	vectorResult->Y = vector->Y + y;
}

void PXVector3FAdd(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult)
{
	vectorResult->X = vectorA->X + vectorB->X;
	vectorResult->Y = vectorA->Y + vectorB->Y;
	vectorResult->Z = vectorA->Z + vectorB->Z;
}

void PXVector3FAddXYZ(const PXVector3F* const vector, const float x, const float y, const float z, PXVector3F* const vectorResult)
{
	vectorResult->X = vector->X + x;
	vectorResult->Y = vector->Y + y;
	vectorResult->Z = vector->Z + z;
}

void PXVector4FAddXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult)
{
	vectorResult->X = vector->X + x;
	vectorResult->Y = vector->Y + y;
	vectorResult->Z = vector->Z + z;
	vectorResult->W = vector->W + w;
}

void PXVector3FInterpolate(const PXVector3F* const vectorA, const PXVector3F* const vectorB, const float speed, PXVector3F* const vectorResult)
{
	// A * t + (1 - t) * B

	PXVector3F deltaA;
	PXVector3F deltaB;

	PXVector3FMultiplyS(vectorA, 1 - speed, &deltaA);
	PXVector3FMultiplyS(vectorA, speed, &deltaB);
	PXVector3FAdd(&deltaA, &deltaB, vectorResult);
}