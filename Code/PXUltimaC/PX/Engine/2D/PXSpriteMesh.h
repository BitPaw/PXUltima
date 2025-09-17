#ifndef PXSpriteMeshIncluded
#define PXSpriteMeshIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSpriteMeshEntry_
{
    PXVector2I16U PositionInCells;
    PXVector2I16U SizeInCells;
    PXVector2I8S OffsetInPixel;
}
PXSpriteMeshEntry;

typedef struct PXSpriteMesh_
{
    PXVector2I16U CellSize; // Size of each cell, width and height.
    PXVector2I32U MeshSize;

    PXModel* ModelRectangle;

    PXSpriteMeshEntry* Mesh;
}
PXSpriteMesh;

PXPublic void PXAPI PXSpriteMeshInit
(
    PXSpriteMesh* const pxSpriteMesh,
    const PXI32U amountWidth,
    const PXI32U amountHeight,
    const PXI16U cellWidth,
    const PXI16U cellHeight
);
PXPublic void PXAPI PXSpriteMeshSpriteInsert(PXSpriteMesh* const pxSpriteMesh);

#endif