#ifndef PXSpriteMeshInclude
#define PXSpriteMeshInclude

#include <PX/Media/PXResource.h>

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
    const PXInt32U amountWidth,
    const PXInt32U amountHeight,
    const PXInt16U cellWidth,
    const PXInt16U cellHeight
);
PXPublic void PXAPI PXSpriteMeshSpriteInsert(PXSpriteMesh* const pxSpriteMesh);

#endif