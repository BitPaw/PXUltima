#pragma once

#ifndef PXVoxelGridIncluded
#define PXVoxelGridIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Engine/PXDictionaryECS.h>
#include <PX/Math/PXTransform.h>
#include <PX/Type/PXVertex.h>
#include <PX/Type/PXIndex.h>
#include <PX/Type/PXModel.h>


typedef enum PXVoxelType_
{
    PXTileTypeEmpty,
}
PXVoxelType;

typedef struct PXVoxel_
{
    PXECSInfo Info;

    PXModel* Model;
}
PXVoxel;

typedef struct PXGreedyQuad 
{
    PXVector3I8U IndexInChunk;
    PXI8U w, h;
    PXI8U axis; // direction
    PXI8U pass;
    PXI16U materialID;
} PXGreedyQuad;

typedef struct PXGreedyMaterial_
{
    PXI16U VoxelValue;   // Eqivilent to materialID.
    PXI32U IndexCount;   // How many indices belong to this material

    PXI32U Consumed;

    //PXVertexP3N3T2F32* VertexData;
    PXIndexTriangle32* IndexData;
} 
PXGreedyMaterial;

// nxm sized voxel group.
typedef struct PXVoxelChunk_
{
    PXECSInfo Info;

    PXVector4I64S Position;

    PXBuffer Data; // Should contain index data to voxel

    PXMesh* Mesh; // Mesh element for a bigger model 

    // Cache

    PXGreedyMaterial GreedyMaterialStack[0xff]; // Calculated to finalize
    PXSize GreedyMaterialStackAmount; // Used to calculate final indexdata

    PXSize PXGreedyQuadAmount; // Can be used to calculate final vertexdata
    PXGreedyQuad PXGreedyQuadList[16*16*16*6];
}
PXVoxelChunk;

typedef struct PXVoxelGrid_
{
    PXECSInfo Info;

    //PXSize4D Size;
    PXI8U Dimension; // 1D, 2D, 3D, 4D, ...
    PXI8U ChunkSize; // 16, 32, 64, 128, ... 

    PXSize PixelPerUnit; // Do we need this?


    PXSize AmountOfChunks;
    PXBuffer Data; // Direct stored data of chunks.

    PXSize ChunkIndex;
    PXVoxelChunk* ChunkList;

    // Lookup for all tiles.
    PXDictionaryECS LookupVoxel; // Voxels, defined
    PXDictionaryECS LookupChunk; // Chunks, loaded or unloaded

    PXDictionary LookupChunkX16; // 255x255
    PXDictionary LookupChunkX32;
    PXDictionary LookupChunkX64;

    // Final model output
    PXModel* ChunkModel;

    PXBool DoXP;
    PXBool DoXN;
    PXBool DoYP;
    PXBool DoYN;
    PXBool DoZP;
    PXBool DoZN;
}
PXVoxelGrid;


typedef struct PXVoxelGridCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize ChunkPreallocate;
    PXSize ChunkSize;
}
PXVoxelGridCreateInfo;


PXPublic void PXAPI PXVoxelGridRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXVoxelGridCreate(PXVoxelGrid** pxVoxelGridREF, PXVoxelGridCreateInfo PXREF pxVoxelGridCreateInfo);
PXPublic PXResult PXAPI PXVoxelGridRelease(PXVoxelGrid PXREF pxVoxelGrid);

PXPublic PXSize PXAPI PXVoxelGridVoxelPerChunk(const PXVoxelGrid PXREF pxVoxelGrid);

PXPublic PXResult PXAPI PXVoxelGridResize
(
    PXVoxelGrid PXREF pxVoxelGrid,
    PXSize amountOfChunks
);

PXPublic PXResult PXAPI PXVoxelGridPopulateTest(PXVoxelGrid PXREF pxVoxelGrid);


//---------------------------------------------------------
// Voxels
//---------------------------------------------------------

// We define our types here, we create a tile once then 
// just set it with a lookup index. Gives us power to use 
// smaler types and not pointers or IDs.
PXPublic PXResult PXAPI PXVoxelGridVoxelDefine(PXVoxelGrid PXREF pxVoxelGrid, PXVoxel PXREF pxVoxel);

// Remove a registered tile. 
// Problem accur if the tile is still used. Check
PXPublic PXResult PXAPI PXVoxelGridVoxelRemove(PXVoxelGrid PXREF pxVoxelGrid, const PXID voxelID);


PXPublic PXVoxel* PXAPI PXVoxelGridVoxelGetViaID(PXVoxelGrid PXREF pxVoxelGrid, const PXID voxelID);
PXPublic PXVoxel* PXAPI PXVoxelGridVoxelGetViaIndex(PXVoxelGrid PXREF pxVoxelGrid, const PXSize voxelIndex);
//---------------------------------------------------------

PXPublic PXSize PXAPI PXVoxelGridVoxelAmountNotEmpty(PXVoxelGrid PXREF pxVoxelGrid, const PXVoxelChunk PXREF pxVoxelChunk);

PXPublic PXResult PXAPI PXVoxelGridVoxelSet
(
    PXVoxelGrid PXREF pxVoxelGrid,
    PXVector4F32 PXREF pxPositon,
    const PXSize tileIndex
);

//---------------------------------------------------------
// Chunk
//---------------------------------------------------------
PXPublic PXResult PXAPI PXVoxelGridChunkLoad(PXVoxelGrid PXREF pxVoxelGrid, PXVector4I64S PXREF pxVector4I64S);
PXPublic PXResult PXAPI PXVoxelGridChunkUnload(PXVoxelGrid PXREF pxVoxelGrid, PXVector4I64S PXREF pxVector4I64S);

PXPublic PXSize PXAPI PXVoxelGridChunkAmount(const PXVoxelGrid PXREF pxVoxelGrid);
PXPublic PXVoxelChunk* PXAPI PXVoxelGridChunkGetViaIndex(const PXVoxelGrid PXREF pxVoxelGrid, const PXSize chunkIndex);

PXPublic PXVoxelChunk* PXAPI PXVoxelGridChunkGet(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex, const PXBool createOnMiss);
PXPublic PXVoxelChunk* PXAPI PXVoxelGridChunkSearch(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex);
PXPublic PXVoxelChunk* PXAPI PXVoxelGridChunkCreate(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex);

PXPublic void* PXAPI PXVoxelGridChunkVoxelGet(const PXVoxelGrid PXREF pxVoxelGrid, const PXVoxelChunk PXREF pxVoxelChunk, const PXVector4I64U PXREF pxVector);
PXPublic void PXAPI PXVoxelGridChunkVoxelSet(const PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk, const PXVector4I64U PXREF pxVector, const PXSize voxelIndex);

//---------------------------------------------------------



//---------------------------------------------------------
PXPublic PXResult PXAPI PXVoxelGridChunkMeshMend(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic PXResult PXAPI PXVoxelGridChunkMeshMendNaive(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic PXResult PXAPI PXVoxelGridChunkMeshMendFast(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);


PXPublic PXGreedyMaterial* PXMeshGreedyMaterialGet(PXVoxelChunk PXREF pxVoxelChunk, const PXSize materialID);


PXPublic void PXMeshGenerateBuffersP3N3T2(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshQuadAdd(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk, PXGreedyQuad PXREF pxGreedyQuad);

PXPublic void PXMeshMendXP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshMendXN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshMendYP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshMendYN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshMendZP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);
PXPublic void PXMeshMendZN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk);


PXPublic PXResult PXAPI PXVoxelGridMeshFinalize(PXVoxelGrid PXREF pxVoxelGrid, PXMaterial PXREF pxMaterial);
//---------------------------------------------------------


PXPublic PXResult PXAPI PXVoxelGridTileGetAtWorldPos(PXVoxelGrid PXREF pxVoxelGrid, float world_x, float world_y);


#endif