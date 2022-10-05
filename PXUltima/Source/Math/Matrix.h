#ifndef MatrixINCLUDE
#define MatrixINCLUDE

#include <Format/Type.h>

#define XAxisX 0
#define XAxisY 4
#define XAxisZ 8
#define XAxisW 12

#define YAxisX 1
#define YAxisY 5
#define YAxisZ 9
#define YAxisW 13

#define ZAxisX 2
#define ZAxisY 6
#define ZAxisZ 10
#define ZAxisW 14

#define WAxisX 3
#define WAxisY 7
#define WAxisZ 11
#define WAxisW 15

#define ScaleX 0
#define ScaleY 5
#define ScaleZ 10
#define ScaleW 15

#define TransformX 12
#define TransformY 13
#define TransformZ 14
#define TransformW 15

#ifdef __cplusplus
extern "C"
{
#endif

	/*
		+----+----+----+----+
		| XA | YA | ZA | Or |
		+----+----+----+----+
		|  0 |  1 |  2 |  3 |
		|  4 |  5 |  6 |  7 |
		|  8 |  9 | 10 | 11 |
		| 12 | 13 | 14 | 15 |
		+----+----+----+----+
	*/
	typedef struct PXMatrix4x4F_
	{
		union
		{
			struct
			{
				float XX;
				float XY;
				float XZ;
				float XW;
				float YX;
				float YY;
				float YZ;
				float YW;
				float ZX;
				float ZY;
				float ZZ;
				float ZW;
				float WX;
				float WY;
				float WZ;
				float WW;
			};

			float Data[16];

			float DataXY[4][4];
		};
	}
	PXMatrix4x4F;

	CPublic void PXMatrix4x4FReset(PXMatrix4x4F* const matrix4x4F);
	CPublic void PXMatrix4x4FResetAxisW(PXMatrix4x4F* const matrix4x4F);

	CPublic void PXMatrix4x4FMultiply(const PXMatrix4x4F* matrixA, const PXMatrix4x4F* matrixB, PXMatrix4x4F* const matrixResult);
	CPublic void PXMatrix4x4FRotate(const PXMatrix4x4F* matrixA, const float x, const float y, const float z, PXMatrix4x4F* const matrixResult);

	CPublic void PXMatrix4x4FMoveXY(const PXMatrix4x4F* const matrixA, const float x, const float y, PXMatrix4x4F* const matrixResult);
	CPublic void PXMatrix4x4FMoveXYZ(const PXMatrix4x4F* const matrixA, const float x, const float y, const float z, PXMatrix4x4F* const matrixResult);

	CPublic void PXMatrix4x4FScaleBy(const PXMatrix4x4F* const matrixA, const float scalar, PXMatrix4x4F* const matrixResult);

	CPublic void PXMatrix4x4FScaleSet(const float x, const float y, const float z, PXMatrix4x4F* const matrixResult);

#ifdef __cplusplus
}
#endif

#endif