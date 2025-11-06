#include "PXSpriteMesh.h"

#include <PX/OS/PXOS.h>

void PXAPI PXSpriteMeshInit
(
    PXSpriteMesh PXREF pxSpriteMesh, 
    const PXI32U amountWidth,
    const PXI32U amountHeight,
    const PXI16U cellWidth,
    const PXI16U cellHeight
)
{
    PXClear(PXSpriteMesh, pxSpriteMesh);

    // Alloc
    pxSpriteMesh->MeshSize.X = amountWidth;
    pxSpriteMesh->MeshSize.Y = amountHeight;
    pxSpriteMesh->CellSize.X = cellWidth;
    pxSpriteMesh->CellSize.Y = cellHeight;
    
    pxSpriteMesh->Mesh = PXMemoryHeapCallocT(PXSpriteMeshEntry, amountWidth * amountHeight);

    for(PXI32U row = 0; row < amountWidth; ++row)
    {
        for(PXI32U col = 0; col < amountHeight; ++col)
        {
            const PXSize index = amountWidth * row + col;
            PXSpriteMeshEntry PXREF pxSpriteMeshEntry = &pxSpriteMesh->Mesh[index];

            // ???
        }
    }
}

void PXAPI PXSpriteMeshSpriteInsert(PXSpriteMesh PXREF pxSpriteMesh)
{
   
}
