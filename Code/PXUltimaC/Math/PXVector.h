#ifndef PXVectorINCLUDE
#define PXVectorINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXVector2F_
	{
		union
		{
			struct
			{
				float X;
				float Y;
			};

			float Data[2];
		};
	}
	PXVector2F;

	typedef struct PXVector3F_
	{
		union
		{
			struct
			{
				float X;
				float Y;
				float Z;
			};

			float Data[3];
		};
	}
	PXVector3F;

	typedef struct PXVector4F_
	{
		union
		{
			struct
			{
				float X;
				float Y;
				float Z;
				float W;
			};

			float Data[4];
		};
	}
	PXVector4F;


	//-----------------------------------------------------
	// Vector2
	//-----------------------------------------------------
	PXPublic void PXVector2FConstruct(PXVector2F* const vector2F);

	PXPublic void PXVector2FSet(PXVector2F* const vector, const PXVector2F* const vectorSet);
	PXPublic void PXVector2FSetXY(PXVector2F* const vector, const float x, const float y);

	PXPublic void PXVector2FAdd( PXVector2F* const vector, const PXVector2F* const pxVectorData);
	PXPublic void PXVector2FAddXY(PXVector2F* const vector, const float x, const float y);

	PXPublic void PXVector2FMultiply(PXVector2F* const vector, const PXVector2F* const pxVectorData);
	PXPublic void PXVector2FMultiplyXY(PXVector2F* const vector, const float x, const float y);

	PXPublic float PXVector2FDistanceTo(PXVector2F* const vectorA, PXVector2F* const vectorB);
	PXPublic float PXVector2FDotProduct(const PXVector2F* const vectorA, const PXVector2F* const vectorB);



	//-----------------------------------------------------
	// Vector3
	//-----------------------------------------------------
	PXPublic void PXVector3FConstruct(PXVector3F* const pxVector3F);
	PXPublic void PXVector3FSetXYZ(PXVector3F* const pxVector3F, const float x, const float y, const float z);

	PXPublic void PXVector3FAdd(PXVector3F* const pxVector3F, const PXVector3F* const vectorB);
	PXPublic void PXVector3FAddXYZ(PXVector3F* const pxVector3F, const float x, const float y, const float z);

	PXPublic void PXVector3FSubstract(PXVector3F* const pxVector3F, const PXVector3F* const pxVector3FInput);

	PXPublic void PXVector3FMultiply(PXVector3F* const pxVector3F, PXVector3F* const vectorResult);
	PXPublic void PXVector3FMultiplyXYZ(PXVector3F* const pxVector3F, const float x, const float y, const float z);
	PXPublic void PXVector3FMultiplyS(PXVector3F* const pxVector3F, const float scalar);


	PXPublic void PXVector3FDivide(PXVector3F* const pxVector3F, const PXVector3F* const vectorB);
	PXPublic void PXVector3FDivideS(PXVector3F* const pxVector3F, const float scalar);

	PXPublic void PXVector3FNormalize(PXVector3F* const pxVector3F);
	PXPublic float PXVector3FLength(const PXVector3F* const vector);

	PXPublic void PXVector3FCrossProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB, PXVector3F* const vectorResult);
	PXPublic float PXVector3FDotProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB);
	PXPublic void PXVector3FInterpolate(const PXVector3F* const vectorA, const PXVector3F* const vectorB, const float speed, PXVector3F* const vectorResult);


	//-----------------------------------------------------
	// Vector4
	//-----------------------------------------------------
	PXPublic void PXVector4FConstruct(PXVector4F* const vector4F);
	PXPublic void PXVector4FSetXYZW(PXVector4F* const vector, const float x, const float y, const float z, const float w);

	PXPublic void PXVector4FAddXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult);
	PXPublic void PXVector4FMultiplyXYZW(const PXVector4F* const vector, const float x, const float y, const float z, const float w, PXVector4F* const vectorResult);

#ifdef __cplusplus
}
#endif

#endif