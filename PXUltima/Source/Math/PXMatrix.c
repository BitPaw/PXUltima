#include "PXMatrix.h"

#include <Math/PXMath.h>
#include <OS/Memory/PXMemory.h>

void PXMatrix4x4FIdentity(PXMatrix4x4F* const matrix4x4F)
{
	for (PXSize y = 0; y < 4u; ++y)
	{
		for (PXSize x = 0; x < 4u; ++x)
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

void PXMatrix4x4FPosition(const PXMatrix4x4F* const matrix, PXVector3F* const position)
{
	position->X = matrix->Data[TransformX];
	position->Y = matrix->Data[TransformY];
	position->Z = matrix->Data[TransformZ];
}

void PXMatrix4x4FMoveTo(PXMatrix4x4F* const matrixInput, const PXVector3F* const position, PXMatrix4x4F* const matrixResult)
{
    MemoryCopy(matrixInput, sizeof(PXMatrix4x4F), matrixResult, sizeof(PXMatrix4x4F));

	matrixResult->Data[TransformX] = position->X;
	matrixResult->Data[TransformY] = position->Y;
	matrixResult->Data[TransformZ] = position->Z;
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

	PXMatrix4x4FIdentity(&xRotation);
	PXMatrix4x4FIdentity(&yRotation);
	PXMatrix4x4FIdentity(&zRotation);

	//-----<X ROT>-----
	{
		const float cosResult = MathCosinus(x);
		const float sinResult = MathSinus(x);

		xRotation.Data[5] = cosResult;
		xRotation.Data[6] = sinResult;
		xRotation.Data[9] = -sinResult;
		xRotation.Data[10] = cosResult;

	}

	//-----<X ROT>-----
	{
		const float cosResult = MathCosinus(-y);
		const float sinResult = MathSinus(-y);

		yRotation.Data[0] = cosResult;
		yRotation.Data[2] = -sinResult;
		yRotation.Data[8] = sinResult;
		yRotation.Data[10] = cosResult;
	}

	//-----<X ROT>-----
	{
		const float cosResult = MathCosinus(z);
		const float sinResult = MathSinus(z);

		zRotation.Data[0] = cosResult;
		zRotation.Data[1] = -sinResult;
		zRotation.Data[4] = sinResult;
		zRotation.Data[5] = cosResult;
	}


	// Gimble (result = xRotation * yRotation * zRotation;)
	{
		PXMatrix4x4F tempRotation;
		PXMatrix4x4FIdentity(&tempRotation);

		PXMatrix4x4FMultiply(&yRotation, &zRotation, &tempRotation);
		PXMatrix4x4FMultiply(&tempRotation, &xRotation, matrixResult);
	}
}

void PXMatrix4x4FCopy(const PXMatrix4x4F* const matrixA, const PXMatrix4x4F* const matrixResult)
{
	const PXSize sizeOfMatrix = sizeof(PXMatrix4x4F);

	MemoryCopy(matrixA, sizeOfMatrix, matrixResult, sizeOfMatrix);
}

void PXMatrix4x4FMove3F(const PXMatrix4x4F* const matrixA, const PXVector3F* const vector3F, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[TransformX] = matrixA->Data[TransformX] + vector3F->X;
	matrixResult->Data[TransformY] = matrixA->Data[TransformY] + vector3F->Y;
	matrixResult->Data[TransformZ] = matrixA->Data[TransformZ] + vector3F->Z;
}

void PXMatrix4x4FMoveXY(const PXMatrix4x4F* const matrixA, const float x, const float y, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[TransformX] = matrixA->Data[TransformX] + x;
	matrixResult->Data[TransformY] = matrixA->Data[TransformY] + y;
}

void PXMatrix4x4FMoveToScaleXY(const PXMatrix4x4F* const matrixA, const float x, const float y, PXMatrix4x4F* const matrixResult)
{
	PXMatrix4x4FMoveXY(matrixA, x * (1-matrixA->Data[ScaleX]), y * (1 - matrixA->Data[ScaleY]), matrixResult);
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
	matrixResult->Data[ScaleY] = matrixA->Data[ScaleY] + scalar;
	matrixResult->Data[ScaleZ] = matrixA->Data[ScaleZ] + scalar;
}

void PXMatrix4x4FScaleSet(const float x, const float y, const float z, PXMatrix4x4F* const matrixResult)
{
	matrixResult->Data[ScaleX] = x;
	matrixResult->Data[ScaleY] = y;
	matrixResult->Data[ScaleZ] = z;
}

void PXMatrix4x4FScaleGet(const PXMatrix4x4F* const matrixResult, float* const x, float* const y, float* const z)
{
	*x = matrixResult->Data[ScaleX];
	*y = matrixResult->Data[ScaleY];
	*z = matrixResult->Data[ScaleZ];
}

void PXMatrix4x4FOrthographic(PXMatrix4x4F* const matrix4x4F, const float left, const float right, const float bottom, const float top, const float nearPlane, const float farPlane)
{
	PXMatrix4x4FIdentity(matrix4x4F);

	matrix4x4F->Data[ScaleX] = (2.0f) / (right - left);
	matrix4x4F->Data[ScaleY] = (2.0f) / (top - bottom);
	matrix4x4F->Data[ScaleZ] = -(2.0f) / (farPlane - nearPlane);
	matrix4x4F->Data[TransformX] = -(right + left) / (right - left);
	matrix4x4F->Data[TransformY] = -(top + bottom) / (top - bottom);
	matrix4x4F->Data[TransformZ] = -(farPlane + nearPlane) / (farPlane - nearPlane);
}

void PXMatrix4x4FPerspective(PXMatrix4x4F* const matrix4x4F, const float fielfOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
	const float difference = farPlane - nearPlane;
	const float fielfOfViewRadians = MathDegreeToRadians(fielfOfView);
	const float tanHalfFovy = MathTangens(fielfOfViewRadians / 2.0f);

	PXMatrix4x4FIdentity(matrix4x4F);

	matrix4x4F->Data[ScaleX] = (1) / (aspectRatio * tanHalfFovy);
	matrix4x4F->Data[ScaleY] = (1) / (tanHalfFovy);
	matrix4x4F->Data[ScaleZ] = -(farPlane + nearPlane) / difference;
	matrix4x4F->Data[11] = -(1);
	matrix4x4F->Data[TransformZ] = -((2) * farPlane * nearPlane) / difference;
}

PXBool PXMatrix4x4FInverse(const PXMatrix4x4F* const matrix4x4FInput, PXMatrix4x4F* const matrix4x4FResult)
{
	PXMatrix4x4F temp;
	double det = 0;

	temp.Data[0] = matrix4x4FInput->Data[5] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[5] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[9] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[9] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[13] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[13] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[10];
	temp.Data[4] = -matrix4x4FInput->Data[4] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[10];
	temp.Data[8] = matrix4x4FInput->Data[4] * matrix4x4FInput->Data[9] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[9];
	temp.Data[12] = -matrix4x4FInput->Data[4] * matrix4x4FInput->Data[9] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[10] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[9];
	temp.Data[1] = -matrix4x4FInput->Data[1] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[1] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[9] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[9] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[13] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[13] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[10];
	temp.Data[5] = matrix4x4FInput->Data[0] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[0] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[10];
	temp.Data[9] = -matrix4x4FInput->Data[0] * matrix4x4FInput->Data[9] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[0] * matrix4x4FInput->Data[11] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[9];
	temp.Data[13] = matrix4x4FInput->Data[0] * matrix4x4FInput->Data[9] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[0] * matrix4x4FInput->Data[10] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[10] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[9];
	temp.Data[2] = matrix4x4FInput->Data[1] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[1] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[5] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[5] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[13] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[7] - matrix4x4FInput->Data[13] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[6];
	temp.Data[6] = -matrix4x4FInput->Data[0] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[0] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[7] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[6];
	temp.Data[10] = matrix4x4FInput->Data[0] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[15] - matrix4x4FInput->Data[0] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[15] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[7] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[5];
	temp.Data[14] = -matrix4x4FInput->Data[0] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[14] + matrix4x4FInput->Data[0] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[13] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[14] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[13] - matrix4x4FInput->Data[12] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[6] + matrix4x4FInput->Data[12] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[5];
	temp.Data[3] = -matrix4x4FInput->Data[1] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[1] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[10] + matrix4x4FInput->Data[5] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[5] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[10] - matrix4x4FInput->Data[9] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[7] + matrix4x4FInput->Data[9] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[6];
	temp.Data[7] = matrix4x4FInput->Data[0] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[0] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[10] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[10] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[7] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[6];
	temp.Data[11] = -matrix4x4FInput->Data[0] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[11] + matrix4x4FInput->Data[0] * matrix4x4FInput->Data[7] * matrix4x4FInput->Data[9] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[11] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[9] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[7] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[3] * matrix4x4FInput->Data[5];
	temp.Data[15] = matrix4x4FInput->Data[0] * matrix4x4FInput->Data[5] * matrix4x4FInput->Data[10] - matrix4x4FInput->Data[0] * matrix4x4FInput->Data[6] * matrix4x4FInput->Data[9] - matrix4x4FInput->Data[4] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[10] + matrix4x4FInput->Data[4] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[9] + matrix4x4FInput->Data[8] * matrix4x4FInput->Data[1] * matrix4x4FInput->Data[6] - matrix4x4FInput->Data[8] * matrix4x4FInput->Data[2] * matrix4x4FInput->Data[5];

	det = matrix4x4FInput->Data[0] * temp.Data[0] + matrix4x4FInput->Data[1] * temp.Data[4] + matrix4x4FInput->Data[2] * temp.Data[8] + matrix4x4FInput->Data[3] * temp.Data[12];

	if (det == 0)
		return PXNo;

	det = 1.0 / det;

	for (PXSize i = 0; i < 16u; ++i)
	{
		matrix4x4FResult->Data[i] = temp.Data[i] * det;
	}

	return PXYes;
}

void PXMatrix4x4FTranpose(const PXMatrix4x4F* const matrix4x4FInput, PXMatrix4x4F* const matrix4x4FResult)
{
	//NumberType a = this->Data[0];
	const float b = matrix4x4FInput->Data[1];
	const float c = matrix4x4FInput->Data[2];
	const float d = matrix4x4FInput->Data[3];

	const float e = matrix4x4FInput->Data[4];
	//NumberType f = this->Data[5];
	const float g = matrix4x4FInput->Data[6];
	const float h = matrix4x4FInput->Data[7];

	const float i = matrix4x4FInput->Data[8];
	const float j = matrix4x4FInput->Data[9];
	//NumberType k = this->Data[10];
	const float l = matrix4x4FInput->Data[11];

	const float m = matrix4x4FInput->Data[12];
	const float n = matrix4x4FInput->Data[13];
	const float o = matrix4x4FInput->Data[14];
	//NumberType p = this->Data[15];

	//this->Data[0] = a;
	matrix4x4FResult->Data[1] = e;
	matrix4x4FResult->Data[2] = i;
	matrix4x4FResult->Data[3] = m;

	matrix4x4FResult->Data[4] = b;
	//this->Data[5] = f;
	matrix4x4FResult->Data[6] = j;
	matrix4x4FResult->Data[7] = n;

	matrix4x4FResult->Data[8] = c;
	matrix4x4FResult->Data[9] = g;
	//this->Data[10] = k;
	matrix4x4FResult->Data[11] = o;

	matrix4x4FResult->Data[12] = d;
	matrix4x4FResult->Data[13] = h;
	matrix4x4FResult->Data[14] = l;
	//this->Data[15] = p;
}

void PXMatrix4x4FLookAt(const PXMatrix4x4F* const matrix4x4FInput, const PXVector3F* const eye, const PXVector3F* const center, const PXVector3F* const up, PXMatrix4x4F* const matrix4x4FResult)
{
	PXVector3F centereye;
	PXVector3F f;
	PXVector3F frontUpCross;
	PXVector3F s;
	PXVector3F u;

	PXVector3FSubstract(center, eye, &centereye);
	PXVector3FNormalize(&centereye, &f);
	PXVector3FCrossProduct(&f, up, &frontUpCross);

	PXVector3FNormalize(&frontUpCross, &s);
	PXVector3FCrossProduct(&s, &f, &u);

	matrix4x4FResult->Data[XAxisX] = s.X;
	matrix4x4FResult->Data[XAxisY] = s.Y;
	matrix4x4FResult->Data[XAxisZ] = s.Z;

	matrix4x4FResult->Data[YAxisX] = u.X;
	matrix4x4FResult->Data[YAxisY] = u.Y;
	matrix4x4FResult->Data[YAxisZ] = u.Z;

	matrix4x4FResult->Data[ZAxisX] = -f.X;
	matrix4x4FResult->Data[ZAxisY] = -f.Y;
	matrix4x4FResult->Data[ZAxisZ] = -f.Z;

	matrix4x4FResult->Data[TransformX] = -PXVector3FDotProduct(&s, eye);
	matrix4x4FResult->Data[TransformY] = -PXVector3FDotProduct(&u, eye);
	matrix4x4FResult->Data[TransformZ] = PXVector3FDotProduct(&f, eye);
}

#include <stdio.h>


void PXMatrix4x4FPrint(const PXMatrix4x4F* const matrix4x4F, char* const buffer)
{
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
}
