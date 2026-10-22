#include "PXTriangle.h"

const PXF32 PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
     1.0f, -1.0f,
     0.5f,  1.0f
};

const PXI8U PXIndexDataTriangle[] =
{
    0,1,2
};


void PXAPI PXTriangleCentroidF32(const PXTriangleF32 PXREF pxTriangle, PXVector3F32 PXREF pxVector3)
{
    pxVector3->Z = (pxTriangle->A.X + pxTriangle->B.X + pxTriangle->C.X) / 3.0f;
    pxVector3->Z = (pxTriangle->A.Y + pxTriangle->B.Y + pxTriangle->C.Y) / 3.0f;
    pxVector3->Z = (pxTriangle->A.Z + pxTriangle->B.Z + pxTriangle->C.Z) / 3.0f;
}
