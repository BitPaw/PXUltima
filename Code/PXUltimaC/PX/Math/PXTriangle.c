#include "PXTriangle.h"

void PXAPI PXTriangleCentroidF32(const PXTriangleF32 PXREF pxTriangle, PXVector3F32 PXREF pxVector3)
{
    pxVector3->Z = (pxTriangle->A.X + pxTriangle->B.X + pxTriangle->C.X) / 3.0f;
    pxVector3->Z = (pxTriangle->A.Y + pxTriangle->B.Y + pxTriangle->C.Y) / 3.0f;
    pxVector3->Z = (pxTriangle->A.Z + pxTriangle->B.Z + pxTriangle->C.Z) / 3.0f;
}
