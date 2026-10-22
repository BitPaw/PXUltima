#pragma once

#ifndef PXRectangleNineCellIncluded
#define PXRectangleNineCellIncluded
#include <PX/Type/PXModel.h>
#include <PX/Type/PXRectangle.h>

typedef struct PXRectangleNineCell_
{
    PXECSInfo Info;

    // Vertex
    PXF32 width;
    PXF32 height;

    PXRectangleLTRBI32 BorderInnerInPixel;
    PXRectangleLTRBI32 BorderOuterInPixel;

    PXRectangleLTRBF32 BorderInnerNormalized;
    PXRectangleLTRBF32 BorderOuterNormalized;

    //PXRectangleLTRBF32 BorderInner;
    //PXRectangleLTRBF32 BorderOuter;


    PXTexture2D* Texture;
    PXMesh* Mesh;
    PXModel* Model;
    PXVertexBuffer* VertexBuffer;
    //PXIndexBuffer* IndexBuffer;
}
PXRectangleNineCell;

typedef struct PXRectangleNineCellCreateInfo_
{
    PXECSCreateInfo Info;

    PXF32 width;
    PXF32 height;
    PXF32 leftBorder;
    PXF32 rightBorder;
    PXF32 topBorder;
    PXF32 bottomBorder;

    PXTexture2D* Texture;
}
PXRectangleNineCellCreateInfo;


PXInternal void PXAPI PXRectangleNineCellRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXRectangleNineCellCreate(PXRectangleNineCell** pxRectangleNineCellREF, PXRectangleNineCellCreateInfo PXREF pxRectangleNineCellCreateInfo);

PXPublic PXResult PXAPI PXRectangleNineCellVertexCalc(PXRectangleNineCell PXREF pxRectangleNineCell);


#endif