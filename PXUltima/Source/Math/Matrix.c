#include "Matrix.h"

#include <Math/Math.h>

void PXMatrix4x4FReset(PXMatrix4x4F* const matrix4x4F)
{
	for (size_t y = 0; y < 4u; ++y)
	{
		for (size_t x = 0; x < 4u; ++x)
		{
			matrix4x4F->Data[x + y * 4u] = y == x;
		}
	}
}

void PXMatrix4x4FResetAxisW(PXMatrix4x4F* const matrix4x4F)
{
	// ---0
	// ---0
	// ---0
	// 0001

	matrix4x4F->Data[3] = 0;
	matrix4x4F->Data[7] = 0;
	matrix4x4F->Data[11] = 0;
	matrix4x4F->Data[12] = 0;
	matrix4x4F->Data[13] = 0;
	matrix4x4F->Data[14] = 0;
	matrix4x4F->Data[15] = 1;
}

void PXMatrix4x4FMultiply(const PXMatrix4x4F* matrixA, const PXMatrix4x4F* matrixB, PXMatrix4x4F* const matrixResult)
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

void PXMatrix4x4FRotate(const PXMatrix4x4F* matrixA, const float x, const float y, const float z, PXMatrix4x4F* const matrixResult)
{
	PXMatrix4x4F xRotation;
	PXMatrix4x4F yRotation;
	PXMatrix4x4F zRotation;

	PXMatrix4x4FReset(&xRotation);
	PXMatrix4x4FReset(&yRotation);
	PXMatrix4x4FReset(&zRotation);

	//-----<X ROT>-----
	{
		float cosResult = MathCosinus(x);
		float sinResult = MathSinus(x);

		xRotation.Data[5] = cosResult;
		xRotation.Data[6] = sinResult;
		xRotation.Data[9] = -sinResult;
		xRotation.Data[10] = cosResult;

	}

	//-----<X ROT>-----
	{
		float cosResult = MathCosinus(y);
		float sinResult = MathSinus(y);

		yRotation.Data[0] = cosResult;
		yRotation.Data[2] = -sinResult;
		yRotation.Data[8] = sinResult;
		yRotation.Data[10] = cosResult;
	}

	//-----<X ROT>-----
	{
		float cosResult = MathCosinus(z);
		float sinResult = MathSinus(z);

		zRotation.Data[0] = cosResult;
		zRotation.Data[1] = -sinResult;
		zRotation.Data[4] = sinResult;
		zRotation.Data[5] = cosResult;
	}


	// Gimble (result = xRotation * yRotation * zRotation;)
	{
		PXMatrix4x4F tempRotation;
		PXMatrix4x4FReset(&tempRotation);

		PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
		PXMatrix4x4FMultiply(&tempRotation, &xRotation, matrixResult);
	}

	//PXMatrix4x4FMultiply(xRotation);
}

void PXMatrix4x4FMoveXY(const PXMatrix4x4F* const matrixA, const float x, const float y, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[TransformX] = matrixA->Data[TransformX] + x;
	matrixResult->Data[TransformY] = matrixA->Data[TransformY] + y;
}

void PXMatrix4x4FMoveXYZ(const PXMatrix4x4F* const matrixA, const float x, const float y, const float z, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[TransformX] = matrixA->Data[TransformX] + x;
	matrixResult->Data[TransformY] = matrixA->Data[TransformY] + y;
	matrixResult->Data[TransformZ] = matrixA->Data[TransformZ] + z;
}

void PXMatrix4x4FScaleBy(const PXMatrix4x4F* const matrixA, const float scalar, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[ScaleX] = matrixA->Data[ScaleX] + scalar;
	matrixResult->Data[ScaleZ] = matrixA->Data[ScaleZ] + scalar;
	matrixResult->Data[ScaleW] = matrixA->Data[ScaleW] + scalar;
}

void PXMatrix4x4FScaleSet(const float x, const float y, const float z, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[ScaleX] = x;
	matrixResult->Data[ScaleZ] = y;
	matrixResult->Data[ScaleW] = z;
}
