#ifndef PXMatrixINCLUDE
#define PXMatrixINCLUDE

#include <Media/PXType.h>
#include <Math/PXVector.h>

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

typedef struct PXRectangleOffset_ PXRectangleOffset;

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

PXPublic void PXAPI PXMatrix4x4FIdentity(PXMatrix4x4F* const matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FResetAxisW(PXMatrix4x4F* const matrix4x4F);

PXPublic void PXAPI PXMatrix4x4FPositionGet(const PXMatrix4x4F* const matrix, PXVector3F* const position);
PXPublic void PXAPI PXMatrix4x4FPositionSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const position);

PXPublic void PXAPI PXMatrix4x4FRotationGet(const PXMatrix4x4F* const matrix, PXVector3F* const position);
PXPublic void PXAPI PXMatrix4x4FRotationSet(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const position);
PXPublic void PXAPI PXMatrix4x4FRotationMatrixGenerate(PXMatrix4x4F* const matrix4x4F, PXVector3F* const position);

PXPublic void PXAPI PXMatrix4x4FMultiply(const PXMatrix4x4F* matrixA, const PXMatrix4x4F* matrixB, PXMatrix4x4F* const matrixResult);
PXPublic void PXAPI PXMatrix4x4FRotate(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const vector3F);

PXPublic void PXAPI PXMatrix4x4FCopy(const PXMatrix4x4F* const matrixA, PXMatrix4x4F* const matrixResult);

PXPublic void PXAPI PXMatrix4x4FMove3F(PXMatrix4x4F* const matrixData, const PXVector3F* const vector3F);
PXPublic void PXAPI PXMatrix4x4FMoveXY(PXMatrix4x4F* const matrixData, const float x, const float y);
PXPublic void PXAPI PXMatrix4x4FMoveToScaleXY(PXMatrix4x4F* const matrixData, const float x, const float y);
PXPublic void PXAPI PXMatrix4x4FMoveXYZ(PXMatrix4x4F* const matrixData, const float x, const float y, const float z);

PXPublic void PXAPI PXMatrix4x4FScaleBy(PXMatrix4x4F* const matrixA, const float scalar);

PXPublic void PXAPI PXMatrix4x4FScaleByMargin(PXMatrix4x4F* const pxMatrix4x4F, const PXRectangleOffset* const pxMargin);
PXPublic void PXAPI PXMatrix4x4FScaleByXY(PXMatrix4x4F* const pxMatrix4x4F, const float x, const float y);

PXPublic void PXAPI PXMatrix4x4FScaleSet(PXMatrix4x4F* const pxMatrix4x4F, const PXVector3F* const pxVector3F);
PXPublic void PXAPI PXMatrix4x4FScaleSetXY(PXMatrix4x4F* const pxMatrix4x4F, const float x, const float y);
PXPublic void PXAPI PXMatrix4x4FScaleSetXYZ(PXMatrix4x4F* const pxMatrix4x4F, const float x, const float y, const float z);

PXPublic void PXAPI PXMatrix4x4FScaleGet(const PXMatrix4x4F* const matrixResult, PXVector3F* const pxVector3F);
PXPublic void PXAPI PXMatrix4x4FScaleGetXYZ(const PXMatrix4x4F* const matrixResult, float* const x, float* const y, float* const z);

PXPublic void PXAPI PXMatrix4x4FOrthographic(PXMatrix4x4F* const matrix4x4F, const float left, const float right, const float bottom, const float top, const float nearPlane, const float farPlane);
PXPublic void PXAPI PXMatrix4x4FPerspective(PXMatrix4x4F* const matrix4x4F, const float fielfOfView, const float aspectRatio, const float nearPlane, const float farPlane);

PXPublic PXBool PXAPI PXMatrix4x4FInverse(PXMatrix4x4F* const matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FTranpose(PXMatrix4x4F* const matrix4x4F);
PXPublic void PXAPI PXMatrix4x4FLookAt(PXMatrix4x4F* const matrix4x4F, const PXVector3F* const eye, const PXVector3F* const  center, const PXVector3F* const up);

#endif