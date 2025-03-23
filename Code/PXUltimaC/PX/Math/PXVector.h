#ifndef PXVectorINCLUDE
#define PXVectorINCLUDE

#include <PX/Media/PXType.h>

typedef struct PXVector2I_
{
    union
    {
        struct
        {
            int X;
            int Y;
        };

        int Data[2];
    };
}
PXVector2I;

typedef struct PXVector2F_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
        };

        PXF32 Data[2];
    };
}
PXVector2F;

typedef struct PXVector3F_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
        };

        PXF32 Data[3];
    };
}
PXVector3F;

typedef struct PXVector4F_
{
    union
    {
        struct
        {
            PXF32 X;
            PXF32 Y;
            PXF32 Z;
            PXF32 W;
        };

        PXF32 Data[4];
    };
}
PXVector4F;

PXPublic void PXAPI PXVector2FSet(PXVector2F* const pxVector2F, const PXVector2F* const vectorSet);
PXPublic void PXAPI PXVector2FSetXY(PXVector2F* const pxVector2F, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3FSet(PXVector3F* const pxVector3F, const PXVector3F* const pxVector3FSource);
PXPublic void PXAPI PXVector3FSetXYZ(PXVector3F* const pxVector3F, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4FSetXYZW(PXVector4F* const pxVector4F, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);

PXPublic void PXAPI PXVector2FAdd(PXVector2F* const vector, const PXVector2F* const pxVectorData);
PXPublic void PXAPI PXVector2FAddXY(PXVector2F* const vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3FAdd(PXVector3F* const pxVector3F, const PXVector3F* const vectorB);
PXPublic void PXAPI PXVector3FAddXYZ(PXVector3F* const pxVector3F, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector4FAddXYZW(const PXVector4F* const vector, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w, PXVector4F* const vectorResult);

PXPublic void PXAPI PXVector3FSubstract(PXVector3F* const pxVector3F, const PXVector3F* const pxVector3FInput);

PXPublic void PXAPI PXVector2FMultiply(PXVector2F* const vector, const PXVector2F* const pxVectorData);
PXPublic void PXAPI PXVector2FMultiplyXY(PXVector2F* const vector, const PXF32 x, const PXF32 y);
PXPublic void PXAPI PXVector3FMultiply(PXVector3F* const pxVector3F, PXVector3F* const vectorResult);
PXPublic void PXAPI PXVector3FMultiplyXYZ(PXVector3F* const pxVector3F, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic void PXAPI PXVector3FMultiplyS(PXVector3F* const pxVector3F, const PXF32 scalar);
PXPublic void PXAPI PXVector4FMultiplyXYZW(PXVector4F* const pxVector4F, const PXF32 x, const PXF32 y, const PXF32 z, const PXF32 w);
 
PXPublic void PXAPI PXVector3FDivide(PXVector3F* const pxVector3F, const PXVector3F* const vectorB);
PXPublic void PXAPI PXVector3FDivideS(PXVector3F* const pxVector3F, const PXF32 scalar);

PXPublic PXF32 PXAPI PXVector2FDistanceTo(PXVector2F* const vectorA, PXVector2F* const vectorB);
PXPublic PXF32 PXAPI PXVector2FDotProduct(const PXVector2F* const vectorA, const PXVector2F* const vectorB);

PXPublic void PXAPI PXVector3FAbsolute(PXVector3F* const pxVector3F);

PXPublic void PXAPI PXVector3FNormalize(PXVector3F* const pxVector3F);
PXPublic PXF32 PXAPI PXVector3FLength(const PXVector3F* const vector);

PXPublic void PXAPI PXVector3FCrossProduct(PXVector3F* const vectorResult, const PXVector3F* const vectorA, const PXVector3F* const vectorB);
PXPublic PXF32 PXAPI PXVector3FDotProduct(const PXVector3F* const vectorA, const PXVector3F* const vectorB);
PXPublic void PXAPI PXVector3FInterpolate(PXVector3F* const vectorA, const PXVector3F* const vectorB, PXF32 speed);

#endif
