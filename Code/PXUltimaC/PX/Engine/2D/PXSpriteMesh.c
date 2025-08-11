#include "PXSpriteMesh.h"

#include <PX/OS/PXOS.h>

void PXAPI PXSpriteMeshInit
(
    PXSpriteMesh* const pxSpriteMesh, 
    const PXInt32U amountWidth,
    const PXInt32U amountHeight,
    const PXInt16U cellWidth,
    const PXInt16U cellHeight
)
{
    PXClear(PXSpriteMesh, pxSpriteMesh);

    // Alloc
    pxSpriteMesh->MeshSize.X = amountWidth;
    pxSpriteMesh->MeshSize.Y = amountHeight;
    pxSpriteMesh->CellSize.X = cellWidth;
    pxSpriteMesh->CellSize.Y = cellHeight;
    
    pxSpriteMesh->Mesh = PXMemoryHeapCallocT(PXSpriteMeshEntry, amountWidth * amountHeight);

    for(PXInt32U row = 0; row < amountWidth; ++row)
    {
        for(PXInt32U col = 0; col < amountHeight; ++col)
        {
            const PXSize index = amountWidth * row + col;
            PXSpriteMeshEntry* const pxSpriteMeshEntry = &pxSpriteMesh->Mesh[index];

            // ???
        }
    }
}

void PXAPI PXSpriteMeshSpriteInsert(PXSpriteMesh* const pxSpriteMesh)
{
   
}
