#include "PXVoxelGrid.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/PXOS.h>
#include <PX/Type/PXVertex.h>
#include <PX/Type/PXIndex.h>
#include <PX/Algorithm/SpiralStep/PXSpiralStep.h>
#include <PX/Algorithm/HilbertCurve/PXHilbertCurve.h>

#include <intrin.h>


const char PXVoxelGridText[] = "VoxelGrid";
const PXI8U PXVoxelGridTextLength = sizeof(PXVoxelGridText);
const PXECSRegisterInfoStatic PXVoxelGridRegisterInfo =
{
    {sizeof(PXVoxelGridText), sizeof(PXVoxelGridText), PXVoxelGridText, TextFormatASCII},
    sizeof(PXVoxelGrid),
    __alignof(PXVoxelGrid),
    PXECSTypeEntity,
    PXVoxelGridCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXVoxelGridRegisterInfoDynamic;


void PXAPI PXVoxelGridRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXVoxelGridRegisterInfo;
    pxECSRegisterInfo->InfoDynamic = &PXVoxelGridRegisterInfoDynamic;
}

PXResult PXAPI PXVoxelGridCreate(PXVoxelGrid** pxVoxelGridREF, PXVoxelGridCreateInfo PXREF pxVoxelGridCreateInfo)
{
    PXVoxelGrid* pxVoxelGrid = PXNull;

    pxVoxelGridCreateInfo->Info.Static = &PXVoxelGridRegisterInfo;
    pxVoxelGridCreateInfo->Info.Dynamic = &PXVoxelGridRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxVoxelGridREF, pxVoxelGridCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxVoxelGrid = *pxVoxelGridREF;
    pxVoxelGrid->ChunkSize = pxVoxelGridCreateInfo->ChunkSize;
    pxVoxelGrid->Dimension = 3;

    PXVoxelGridResize(pxVoxelGrid, pxVoxelGridCreateInfo->ChunkPreallocate);

    /*
    PXDictionaryCreateInfo pxDictionaryCreateInfo;
    PXClear(PXDictionaryCreateInfo, &pxDictionaryCreateInfo);
    pxDictionaryCreateInfo.KeySize = sizeof(PXI16U);
    pxDictionaryCreateInfo.ValueSize = sizeof();
    pxDictionaryCreateInfo.ValueLocality;
    pxDictionaryCreateInfo.StartAmount;

    PXDictionaryCreate(&PXVoxelGrid->LookupChunkX16, &pxDictionaryCreateInfo);
    */
    return pxResult;
}

PXResult PXAPI PXVoxelGridRelease(PXVoxelGrid PXREF pxVoxelGrid)
{
    return PXResultOK;
}

PXSize PXAPI PXVoxelGridVoxelPerChunk(const PXVoxelGrid PXREF pxVoxelGrid)
{
    return PXMathPowerI64U(pxVoxelGrid->ChunkSize, pxVoxelGrid->Dimension);
}

PXResult PXAPI PXVoxelGridResize
(
    PXVoxelGrid PXREF pxVoxelGrid, 
    PXSize amountOfChunks
)
{
    if(!pxVoxelGrid)
    {
        return PXResultRefusedParameterNull;
    }

    PXSize voxelPerChunk = PXVoxelGridVoxelPerChunk(pxVoxelGrid);
    PXSize size = amountOfChunks * voxelPerChunk * 2u;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXVoxelGridText,
        "Resize",
        "%i^%i = (total voxels) %i",
        pxVoxelGrid->ChunkSize, pxVoxelGrid->Dimension, voxelPerChunk
    );
#endif

    PXBufferEnsureTotal(&pxVoxelGrid->Data, size);

    pxVoxelGrid->AmountOfChunks = amountOfChunks;
    pxVoxelGrid->ChunkList = PXMemoryHeapCallocT(PXVoxelChunk, amountOfChunks);

    return PXResultOK;
}


// Simple hash function to generate smooth pseudo-random gradients
static inline float PX_Noise2D(int x, int z) 
{
#if 1
    // 1. Cast directly to unsigned int to safely preserve raw bit patterns
    unsigned int ux = (unsigned int)x;
    unsigned int uz = (unsigned int)z;

    // 2. A robust, unsigned bit-mixer (avoids all negative shift issues)
    unsigned int n = ux * 8121 + uz * 28411;
    n = (n ^ (n >> 15)) * 0x85ebca6b;
    n = (n ^ (n >> 13)) * 0xc2b2ae35;
    n = n ^ (n >> 16);

    // 3. Map safely to a float between -1.0f and 1.0f
    return 1.0f - ((float)(n & 0x7fffffff) / 1073741824.0f);
#else

    int n = x * 8121 + z * 28411;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
#endif
}

// Bilieneally interpolated smooth noise for continuous terrain hills
static float PX_GetSmoothHeight(float x, float z) 
{
    int ix = (int)PXMathFloorF(x);
    int iz = (int)PXMathFloorF(z);
    float fx = x - (float)ix;
    float fz = z - (float)iz;

    float v1 = PX_Noise2D(ix, iz);
    float v2 = PX_Noise2D(ix + 1, iz);
    float v3 = PX_Noise2D(ix, iz + 1);
    float v4 = PX_Noise2D(ix + 1, iz + 1);

    // Linear interpolations
    float i1 = v1 + fx * (v2 - v1);
    float i2 = v3 + fx * (v4 - v3);
    return i1 + fz * (i2 - i1);
}

PXResult PXAPI PXVoxelGridPopulateTest(PXVoxelGrid PXREF pxVoxelGrid)
{
    if(!pxVoxelGrid)
    {
        return PXResultRefusedParameterNull;
    }

    PXVector4I64S chunkIndex;
    chunkIndex.X = 0;
    chunkIndex.Y = 0;
    chunkIndex.Z = 0;
    chunkIndex.W = 0;

    for(PXSize i = 0; i < pxVoxelGrid->AmountOfChunks; i++)
    {
        // Get index
        const PXVector2F32 spiralIndex = PXSpiralStepGet(i); // PXHilbertCurveGet
        chunkIndex.X = spiralIndex.X;
        chunkIndex.Z = spiralIndex.Y;

        PXVoxelChunk PXREF pxVoxelChunk = PXVoxelGridChunkGet(pxVoxelGrid, &chunkIndex, PXTrue);

        if(!pxVoxelChunk)
        {
            continue;
        }

#if 1
        // Loop through all 4,096 cells within this single sub-chunk block
        for(PXSize lz = 0; lz < pxVoxelGrid->ChunkSize; lz++)
        {
            for(PXSize lx = 0; lx < pxVoxelGrid->ChunkSize; lx++)
            {
                // Calculate absolute global world positioning coordinates
                PXF32 global_x = (PXF32)(chunkIndex.X * pxVoxelGrid->ChunkSize + lx) * 0.1f;
                PXF32 global_z = (PXF32)(chunkIndex.Z * pxVoxelGrid->ChunkSize + lz) * 0.1f;

                // Compute height boundary limit using our noise function (scales 0 to 15)
                PXF32 noise_val = PX_GetSmoothHeight(global_x, global_z);
                PXI32S terrain_height = (PXI32S)((noise_val + 1.0f) * 0.5f * (PXF32)pxVoxelGrid->ChunkSize);

                for(PXI32S ly = 0; ly < pxVoxelGrid->ChunkSize; ly++)
                {
                    PXI32S flat_idx = PXIndex3D(pxVoxelGrid->ChunkSize, lx, ly, lz);

                    if(ly < terrain_height - 1)
                    {
                        pxVoxelChunk->Data.I16U[flat_idx] = 1; // Solid Stone Deep Under
                    }
                    else if(ly == terrain_height)
                    {
                        pxVoxelChunk->Data.I16U[flat_idx] = 1; // Grass Surface Block
                    }
                    else
                    {
                        pxVoxelChunk->Data.I16U[flat_idx] = 0; // Air / Open Sky Space
                    }
                }
            }
        }
#elif 0
        // Loop through all 4,096 cells within this single sub-chunk block
        for(PXSize lz = 0; lz < pxVoxelGrid->ChunkSize; lz++)
        {
            for(PXSize lx = 0; lx < pxVoxelGrid->ChunkSize; lx++)
            {
                PXI32U terrain_height = 1+(lz + lx)/2;

                for(PXI32S ly = 0; ly < pxVoxelGrid->ChunkSize; ly++)
                {
                    PXI32S flat_idx = PXIndex3D(pxVoxelGrid->ChunkSize, lx, ly, lz);

                    if(ly < terrain_height)
                    {
                        pxVoxelChunk->Data.I16U[flat_idx] = 1; // Solid Stone Deep Under
                    }
                    else
                    {
                        pxVoxelChunk->Data.I16U[flat_idx] = 0; // Air / Open Sky Space
                    }
                }
            }
        }

#else

        // Loop through all 4,096 cells within this single sub-chunk block
        for(PXSize lz = 0; lz < pxVoxelGrid->ChunkSize; lz++)
        {
            for(PXSize lx = 0; lx < pxVoxelGrid->ChunkSize; lx++)
            {
                for(PXI32S ly = 0; ly < pxVoxelGrid->ChunkSize; ly++)
                {
                    PXI32S flat_idx = PXIndex3D(pxVoxelGrid->ChunkSize, lx, ly, lz);

                    pxVoxelChunk->Data.I16U[flat_idx] = 1;
                }
            }
        }


#endif

        PXVoxelGridChunkMeshMend(pxVoxelGrid, pxVoxelChunk);
    }

    return PXResultOK;
}

PXSize PXAPI PXVoxelGridVoxelAmountNotEmpty(PXVoxelGrid PXREF pxVoxelGrid, const PXVoxelChunk PXREF pxVoxelChunk)
{
    PXSize total_elements = PXMathPowerI64U(pxVoxelGrid->ChunkSize, 3);

    PXSize counter = 0;

    for(PXSize i = 0; i < total_elements; i++)
    {
        PXBool isNotEmpty = pxVoxelChunk->Data.I16U[i] > 0;

        if(isNotEmpty)
        {
            ++counter;
        }        
    }

    return counter;
}

PXResult PXAPI PXVoxelGridVoxelSet
(
    PXVoxelGrid PXREF pxVoxelGrid, 
    PXVector4F32 PXREF pxPositon,
    const PXSize tileIndex
)
{
    if(!pxVoxelGrid)
    {
        return PXResultRefusedParameterNull;
    }

    PXVector4I64S positionFixed;
    PXVector4I64S indexChunk;
    PXVector4I64S indexLocal;

    // Snap our current pos to the grid
    positionFixed.X = PXMathFloorF(pxPositon->X);
    positionFixed.Y = PXMathFloorF(pxPositon->Y);
    positionFixed.Z = PXMathFloorF(pxPositon->Z);
    positionFixed.W = PXMathFloorF(pxPositon->W);

    // Get the chunkposition
    indexChunk.X = positionFixed.X / pxVoxelGrid->ChunkSize; //  x >> 4
    indexChunk.Y = positionFixed.Y / pxVoxelGrid->ChunkSize;
    indexChunk.Z = positionFixed.Z / pxVoxelGrid->ChunkSize;
    indexChunk.W = positionFixed.W / pxVoxelGrid->ChunkSize;

    // Get the index in the chunk
    indexLocal.X = positionFixed.X % (pxVoxelGrid->ChunkSize - 1); // x & 15
    indexLocal.Y = positionFixed.Y % (pxVoxelGrid->ChunkSize - 1);
    indexLocal.Z = positionFixed.Z % (pxVoxelGrid->ChunkSize - 1);
    indexLocal.W = positionFixed.W % (pxVoxelGrid->ChunkSize - 1);

    /*
    // Seacrh for chunk to insert
    PXTileChunk PXREF pxTileChunk = PXVoxelGridChunkGet(PXVoxelGrid, indexChunk.X, indexChunk.Y);

    if(!pxTileChunk)
    {
        return PXResultInvalid;
    }*/


    return PXResultOK;
}

PXResult PXAPI PXVoxelGridChunkLoad(PXVoxelGrid PXREF pxVoxelGrid, PXVector4I64S PXREF pxVector4I64S)
{
    return PXResultOK;
}

PXResult PXAPI PXVoxelGridChunkUnload(PXVoxelGrid PXREF pxVoxelGrid, PXVector4I64S PXREF pxVector4I64S)
{
    return PXResultOK;
}

PXSize PXAPI PXVoxelGridChunkAmount(const PXVoxelGrid PXREF pxVoxelGrid)
{
    if(!pxVoxelGrid)
    {
        return 0;
    }

    return pxVoxelGrid->AmountOfChunks;
}

PXVoxelChunk* PXAPI PXVoxelGridChunkGetViaIndex(const PXVoxelGrid PXREF pxVoxelGrid, const PXSize chunkIndex)
{
    if(!pxVoxelGrid)
    {
        return PXNull;
    }

    PXBool isInBounce = chunkIndex < pxVoxelGrid->AmountOfChunks;

    if(!isInBounce)
    {
        return PXNull;
    }

    return &pxVoxelGrid->ChunkList[chunkIndex];
}

PXVoxelChunk* PXAPI PXVoxelGridChunkGet(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex, const PXBool createOnMiss)
{
    PXVoxelChunk* pxVoxelChunk = PXVoxelGridChunkSearch(pxVoxelGrid, chunkIndex);

    if(pxVoxelChunk)
    {
        return pxVoxelChunk;
    }

    if(!createOnMiss)
    {
        return PXNull;
    }

    pxVoxelChunk = PXVoxelGridChunkCreate(pxVoxelGrid, chunkIndex);

    return pxVoxelChunk;
}

PXVoxelChunk* PXAPI PXVoxelGridChunkSearch(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex)
{
    if(!pxVoxelGrid)
    {
        return PXNull;
    }

    PXVoxelChunk* pxVoxelChunkTarget = PXNull;

    for(PXSize i = 0; i < pxVoxelGrid->ChunkIndex; i++)
    {
        PXVoxelChunk* pxVoxelChunk = &pxVoxelGrid->ChunkList[i];

        PXBool isTarget = PXMemoryCompareN(&pxVoxelChunk->Position, chunkIndex, sizeof(PXVector4I64S));

        if(isTarget)
        {
            pxVoxelChunkTarget = pxVoxelChunk;
            break;
        }
    }

    return pxVoxelChunkTarget;

    // We can combine the positions for a uniqe key
    const PXI8U xWidth = PXMathDetectMinimalTypeWidthS(chunkIndex->X);
    const PXI8U yWidth = PXMathDetectMinimalTypeWidthS(chunkIndex->Y);
    const PXI8U width = PXMathMaximumIU(xWidth, yWidth);

#if 1
    switch(width)
    {
        case 1:
        {
           // PXI8U xL = x;
           // PXI8U yL = y;
          //  PXI16U key = PXI16Make(xL, yL);



            break;
        }
        case 2:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 8:
        {
            break;
        }
        default:
            break;
    }
#endif

    return PXNull;
}

PXVoxelChunk* PXAPI PXVoxelGridChunkCreate(PXVoxelGrid PXREF pxVoxelGrid, const PXVector4I64S PXREF chunkIndex)
{
    PXVoxelChunk* pxVoxelChunk = &pxVoxelGrid->ChunkList[pxVoxelGrid->ChunkIndex];

    pxVoxelChunk->Position = *chunkIndex;

    PXSize voxelPerChunk = PXVoxelGridVoxelPerChunk(pxVoxelGrid);
    void* x = pxVoxelGrid->Data.Data + (voxelPerChunk * pxVoxelGrid->ChunkIndex * 2);

    PXBufferSet(&pxVoxelChunk->Data, x, voxelPerChunk);

    pxVoxelGrid->ChunkIndex++;

    for(size_t i = 0; i < 0xff; i++)
    {
        pxVoxelChunk->GreedyMaterialStack[i].VoxelValue = -1;
        pxVoxelChunk->GreedyMaterialStack[i].IndexCount = -1;
        pxVoxelChunk->GreedyMaterialStack[i].IndexData = 0;
    }

    //PXClearList(PXVoxelChunk, pxVoxelChunk->GreedyMaterialStack, 0xff);

    return pxVoxelChunk;
}

void* PXAPI PXVoxelGridChunkVoxelGet(const PXVoxelGrid PXREF pxVoxelGrid, const PXVoxelChunk PXREF pxVoxelChunk, const PXVector4I64U PXREF pxVector)
{
    // Get the raw ID from your flat memory pool array
    const PXSize flat_index = PXIndex4D(pxVoxelGrid->ChunkSize, pxVector->X, pxVector->Y, pxVector->Z, pxVector->W);

    // Calculate diff

    void* address = &pxVoxelChunk->Data.I16U[flat_index];



    //uint32_t byte_idx = index >> 1; // Bit-shift instead of division
    //uint8_t  raw_byte = raw_ptr[byte_idx];
    //return (index & 1) ? (raw_byte >> 4) : (raw_byte & 0x0F);


    return address;
}

void PXAPI PXVoxelGridChunkVoxelSet(const PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk, const PXVector4I64U PXREF pxVector, const PXSize voxelIndex)
{
    PXI16U* value = (PXI16U*)PXVoxelGridChunkVoxelGet(pxVoxelGrid, pxVoxelChunk, pxVector);

    *value = voxelIndex;
}

PXResult PXAPI PXVoxelGridMeshFinalize(PXVoxelGrid PXREF pxVoxelGrid, PXMaterial PXREF pxMaterial)
{
    PXResult pxResult = PXResultInvalid;
    PXSize amount = PXVoxelGridChunkAmount(pxVoxelGrid);

    PXModel* pxModel = pxVoxelGrid->ChunkModel;

    // Create model if not done already, not that efficent but ok for now.
    if(!pxModel)
    {
        PXModelCreateInfo pxModelCreateInfo;
        PXClear(PXModelCreateInfo, &pxModelCreateInfo);
        pxModelCreateInfo.MeshListAmount = amount;

        pxResult = PXModelCreate(&pxVoxelGrid->ChunkModel, &pxModelCreateInfo);

        pxModel = pxVoxelGrid->ChunkModel;
    }

    // move mesh arcoding to grid
    //PXTransform4D PXREF pxTransform4D = PXModelTransformGet(pxModel);
    //PXVector4F32SetXYZW(&pxTransform4D->Position, );


    pxResult = PXModelMeshAmountPrepare(pxModel, amount);

    for(PXSize i = 0; i < amount; i++)
    {
        PXVoxelChunk PXREF pxVoxelChunk = PXVoxelGridChunkGetViaIndex(pxVoxelGrid, i);
        PXMesh PXREF pxMesh = pxVoxelChunk->Mesh;

        pxResult = PXModelMeshSet(pxModel, pxMesh, i);

        //PXModelMaterialSet(pxModel, pxMaterial, i, 0);
    }

    return pxResult;
}

// Relative unit structures for constructing 6 independent faces
const PXI8S PXNormals[6][3] =
{
    { 0,  0,  1}, // Front
    { 0,  0, -1}, // Back
    { 1,  0,  0}, // Right
    {-1,  0,  0}, // Left
    { 0,  1,  0}, // Top
    { 0, -1,  0}  // Bottom
};

// Vertex configurations per face relative to the minimum (x, y, z) corner
const PXI8U PXface_vertices[6][4][3] =
{
    { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} }, // Front (+Z)
    { {1,0,0}, {0,0,0}, {0,1,0}, {1,1,0} }, // Back (-Z)
    { {1,0,1}, {1,0,0}, {1,1,0}, {1,1,1} }, // Right (+X)
    { {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} }, // Left (-X)
    { {0,1,1}, {1,1,1}, {1,1,0}, {0,1,0} }, // Top (+Y)
    { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} }  // Bottom (-Y)
};

PXResult PXAPI PXVoxelGridChunkMeshMend(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize solid_voxels = PXVoxelGridVoxelAmountNotEmpty(pxVoxelGrid, pxVoxelChunk);

    if(solid_voxels == 0)
        return PXResultOK; // Nothing to do

    PXMesh* pxMesh = pxVoxelChunk->Mesh;
    PXVertexBuffer* pxVertexBuffer = 0;
    PXIndexBuffer* pxIndexBuffer = 0;

    PXResult pxResult = PXVoxelGridChunkMeshMendFast(pxVoxelGrid, pxVoxelChunk);

    if(pxVoxelChunk->Mesh)
    {
        // Mesh aviable

        pxVertexBuffer = pxMesh->VertexBuffer;
        pxIndexBuffer = pxMesh->IndexBuffer;
    }
    else
    {
        // Dont have mesh
        PXVertexBufferCreateInfo pxVertexBufferCreateInfo;
        PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);
        pxVertexBufferCreateInfo.VertexDataSize =
            4 * pxVoxelChunk->PXGreedyQuadAmount * // 24 = 4 points * 6 faces 
            sizeof(PXVertexP3N3T2F32);

        PXVertexBufferCreate(&pxVertexBuffer, &pxVertexBufferCreateInfo);


        // Create infex data
        PXIndexBufferCreateInfo pxIndexBufferCreateInfo;
        PXClear(PXIndexBufferCreateInfo, &pxIndexBufferCreateInfo);
        pxIndexBufferCreateInfo.IndexAmount = pxVoxelChunk->PXGreedyQuadAmount * 2; // 6 faces, 2 Triangles
        pxIndexBufferCreateInfo.BitPerIndex = 32;
        pxIndexBufferCreateInfo.AmountSegments = pxVoxelChunk->GreedyMaterialStackAmount;
        pxIndexBufferCreateInfo.Dimensions = 3;

        PXIndexBufferCreate(&pxIndexBuffer, &pxIndexBufferCreateInfo);

        for(PXSize i = 0; i < pxVoxelChunk->GreedyMaterialStackAmount; i++)
        {
            PXGreedyMaterial PXREF pxGreedyMaterial = &pxVoxelChunk->GreedyMaterialStack[i];

            PXIndexSegment PXREF pxIndexSegment = PXIndexSegmentGetViaIndex(pxIndexBuffer, i);

            pxIndexSegment->MaterialID = pxGreedyMaterial->VoxelValue;
            pxIndexSegment->Range = pxGreedyMaterial->IndexCount;

            pxGreedyMaterial->IndexData = (PXIndexTriangle32*)PXIndexSegmentDataGet(pxIndexBuffer, i);
        }


        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        pxMeshCreateInfo.VertexBuffer = pxVertexBuffer;
        pxMeshCreateInfo.IndexBuffer = pxIndexBuffer;

        PXMeshCreate(&pxVoxelChunk->Mesh, &pxMeshCreateInfo);

        pxMesh = pxVoxelChunk->Mesh;
    }

    PXMeshGenerateBuffersP3N3T2(pxVoxelGrid, pxVoxelChunk);

    return pxResult;
}

PXResult PXAPI PXVoxelGridChunkMeshMendNaive(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    PXMesh* pxMesh = pxVoxelChunk->Mesh;
    PXVertexBuffer* pxVertexBuffer = pxMesh->VertexBuffer;
    PXIndexBuffer* pxIndexBuffer = pxMesh->IndexBuffer;  

    int indexCounter = 0;
    int v_idx = 0;
    int t_idx = 0;

    PXVector3F32 chunkOffset;
    chunkOffset.X = (PXF32)pxVoxelChunk->Position.X * (PXF32)pxVoxelGrid->ChunkSize;
    chunkOffset.Y = (PXF32)pxVoxelChunk->Position.Y * (PXF32)pxVoxelGrid->ChunkSize;
    chunkOffset.Z = (PXF32)pxVoxelChunk->Position.Z * (PXF32)pxVoxelGrid->ChunkSize;


    for(PXI8U x = 0; x < pxVoxelGrid->ChunkSize; x++)
    {
        for(PXI8U y = 0; y < pxVoxelGrid->ChunkSize; y++)
        {
            for(PXI8U z = 0; z < pxVoxelGrid->ChunkSize; z++)
            {
                PXSize index = PXIndex3D(pxVoxelGrid->ChunkSize, x, y, z);
                PXBool hasBlock = pxVoxelChunk->Data.I16U[index] > 0;

                if(!hasBlock)
                    continue;

                // Build all 6 faces for this voxel
                for(PXI8U f = 0; f < 6; f++)
                {
                    PXVertexP3N3T2F32 pxVertexList[4];

                    // Normal vectors for this face
                    PXI8S nx = PXNormals[f][0];
                    PXI8S ny = PXNormals[f][1];
                    PXI8S nz = PXNormals[f][2];

                    PXF32 offsetU = 1;

                    PXVector3F32 position;
                    position.X = chunkOffset.X + x;
                    position.Y = chunkOffset.Y + y;
                    position.Z = chunkOffset.Z + z;



                    //-------------------------------------
                    // Corner 0: Bottom-Left
                    pxVertexList[0].Position.X = position.X + (PXF32)PXface_vertices[f][0][0];
                    pxVertexList[0].Position.Y = position.Y + (PXF32)PXface_vertices[f][0][1];
                    pxVertexList[0].Position.Z = position.Z + (PXF32)PXface_vertices[f][0][2];
                    pxVertexList[0].Normal.X = nx;
                    pxVertexList[0].Normal.Y = ny;
                    pxVertexList[0].Normal.Z = nz;
                    pxVertexList[0].Texture.X = 0.0f;
                    pxVertexList[0].Texture.Y = 0.0f;
                    //-------------------------------------

                    //-------------------------------------
                    // Corner 1: Bottom-Right
                    pxVertexList[1].Position.X = position.X + (PXF32)PXface_vertices[f][1][0];
                    pxVertexList[1].Position.Y = position.Y + (PXF32)PXface_vertices[f][1][1];
                    pxVertexList[1].Position.Z = position.Z + (PXF32)PXface_vertices[f][1][2];
                    pxVertexList[1].Normal.X = nx;
                    pxVertexList[1].Normal.Y = ny;
                    pxVertexList[1].Normal.Z = nz;
                    pxVertexList[1].Texture.X = 1.0f;
                    pxVertexList[1].Texture.Y = 0.0f;
                    //-------------------------------------

                    //-------------------------------------
                    // Corner 2: Top-Right
                    pxVertexList[2].Position.X = position.X + (PXF32)PXface_vertices[f][2][0];
                    pxVertexList[2].Position.Y = position.Y + (PXF32)PXface_vertices[f][2][1];
                    pxVertexList[2].Position.Z = position.Z + (PXF32)PXface_vertices[f][2][2];
                    pxVertexList[2].Normal.X = nx;
                    pxVertexList[2].Normal.Y = ny;
                    pxVertexList[2].Normal.Z = nz;
                    pxVertexList[2].Texture.X = 1.0f;
                    pxVertexList[2].Texture.Y = 1.0f;
                    //-------------------------------------

                    //-------------------------------------
                    // Corner 3: Top-Left
                    pxVertexList[3].Position.X = position.X + (PXF32)PXface_vertices[f][3][0];
                    pxVertexList[3].Position.Y = position.Y + (PXF32)PXface_vertices[f][3][1];
                    pxVertexList[3].Position.Z = position.Z + (PXF32)PXface_vertices[f][3][2];
                    pxVertexList[3].Normal.X = nx;
                    pxVertexList[3].Normal.Y = ny;
                    pxVertexList[3].Normal.Z = nz;
                    pxVertexList[3].Texture.X = 0.0f;
                    pxVertexList[3].Texture.Y = 1.0f;
                    //-------------------------------------

                    PXIndexTriangle32 indexData[2] =
                    {
                        {indexCounter, indexCounter + 1, indexCounter + 2},
                        {indexCounter, indexCounter + 2, indexCounter + 3}
                    };

                    indexCounter += 4;

                    pxVertexBuffer->Data.VertexP3N3T2F32[v_idx++] = pxVertexList[0];
                    pxVertexBuffer->Data.VertexP3N3T2F32[v_idx++] = pxVertexList[1];
                    pxVertexBuffer->Data.VertexP3N3T2F32[v_idx++] = pxVertexList[2];
                    pxVertexBuffer->Data.VertexP3N3T2F32[v_idx++] = pxVertexList[3];

                    pxIndexBuffer->Data.IndexTriangle32[t_idx++] = indexData[0];
                    pxIndexBuffer->Data.IndexTriangle32[t_idx++] = indexData[1];
                }
            }
        }
    }

    return PXResultOK;
}



// Shared temporary workspace buffers to avoid heavy heap re-allocations per chunk
static PXGreedyQuad s_QuadBuffer[16 * 16 * 6];
static PXI16U s_FaceMasks[16];



#include <string.h>
#include <stdint.h>

// --- Hardware Intrinsic Fallbacks ---
#if defined(_MSC_VER) && !defined(__clang__)
#include <intrin.h>
static inline uint32_t PXTrailingZeros16(uint16_t mask) {
    unsigned long index;
    if(_BitScanForward(&index, mask)) return index;
    return 16;
}
#elif defined(__GNUC__) || defined(__clang__)
static inline uint32_t PXTrailingZeros16(uint16_t mask) {
    if(mask == 0) return 16;
    return __builtin_ctz(mask);
}
#else
    // Pure software fallback for CPUs without intrinsics (or unknown compilers)
static inline uint32_t PXTrailingZeros16(uint16_t mask) {
    if(mask == 0) return 16;
    uint32_t d = 0;
    while((mask & 1) == 0) { d++; mask >>= 1; }
    return d;
}
#endif


PXResult PXAPI PXVoxelGridChunkMeshMendFast(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    // We look at each axis in a as 2D planes.
    // This allows us to merge them in an more efficent way.
    // 6 Axis, for X, Y, Z both positive and negative
#if 1
    PXMeshMendXP(pxVoxelGrid, pxVoxelChunk);
    PXMeshMendXN(pxVoxelGrid, pxVoxelChunk);
    PXMeshMendYP(pxVoxelGrid, pxVoxelChunk);
    PXMeshMendYN(pxVoxelGrid, pxVoxelChunk);
    PXMeshMendZP(pxVoxelGrid, pxVoxelChunk);
    PXMeshMendZN(pxVoxelGrid, pxVoxelChunk);
#else
    PXGreedyQuad quad;
    quad.IndexInChunk.X = 0;
    quad.IndexInChunk.Y = 0;
    quad.IndexInChunk.Z = 0;
    quad.w = (uint8_t)1;
    quad.h = (uint8_t)1; // Width spans X, Height spans Z
    quad.axis = 2; // +Y Axis identifier
    quad.materialID = 0;

    for(quad.IndexInChunk.Y = 0; quad.IndexInChunk.Y < pxVoxelGrid->ChunkSize; ++quad.IndexInChunk.Y)
    {
        for(quad.IndexInChunk.Z = 0; quad.IndexInChunk.Z < pxVoxelGrid->ChunkSize; ++quad.IndexInChunk.Z)
        {
            for(quad.IndexInChunk.X = 0; quad.IndexInChunk.X < pxVoxelGrid->ChunkSize; ++quad.IndexInChunk.X)
            {
                int currentPtr = PXIndex3D(pxVoxelGrid->ChunkSize, quad.IndexInChunk.X, quad.IndexInChunk.Y, quad.IndexInChunk.Z);
                const PXI16U voxel = pxVoxelChunk->Data.I16U[currentPtr];

                if(0 == voxel)
                {
                    continue;
                }

                quad.materialID = voxel;

                for(size_t i = 0; i < 6; i++)
                {
                    quad.axis = i;

                    PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
                }

         
            }
        }
    }
#endif

    return PXResultOK;
}



// Lookup tracking vectors for the 4 corners of a quad [CornerID][XYZ Offset Factor]
// Vertex layout order: 0: Bottom-Left, 1: Top-Left, 2: Top-Right, 3: Bottom-Right
static const float QuadOffsetsW[6][3] = {
    {0, 0, 1}, // 0 (+X): Width spans Z
    {0, 0, 1}, // 1 (-X): Width spans Z
    {1, 0, 0}, // 2 (+Y): Width spans X
    {1, 0, 0}, // 3 (-Y): Width spans X
    {1, 0, 0}, // 4 (+Z): Width spans X
    {1, 0, 0}  // 5 (-Z): Width spans X
};

static const float QuadOffsetsH[6][3] = {
    {0, 1, 0}, // 0 (+X): Height spans Y
    {0, 1, 0}, // 1 (-X): Height spans Y
    {0, 0, 1}, // 2 (+Y): Height spans Z
    {0, 0, 1}, // 3 (-Y): Height spans Z
    {0, 1, 0}, // 4 (+Z): Height spans Y
    {0, 1, 0}  // 5 (-Z): Height spans Y
};

static const float FaceNormals[6][3] = {
    { 1.0f,  0.0f,  0.0f}, // 0: +X
    {-1.0f,  0.0f,  0.0f}, // 1: -X
    { 0.0f,  1.0f,  0.0f}, // 2: +Y
    { 0.0f, -1.0f,  0.0f}, // 3: -Y
    { 0.0f,  0.0f,  1.0f}, // 4: +Z
    { 0.0f,  0.0f, -1.0f}  // 5: -Z
};


PXGreedyMaterial* PXMeshGreedyMaterialGet(PXVoxelChunk PXREF pxVoxelChunk, const PXSize materialID)
{
    PXGreedyMaterial* pxGreedyMaterial = 0;

    for(PXSize i = 0; i < pxVoxelChunk->GreedyMaterialStackAmount; i++)
    {
        PXGreedyMaterial* pxGreedyMaterialCurrent = &pxVoxelChunk->GreedyMaterialStack[i];

        PXBool isSameMaterial = pxGreedyMaterialCurrent->VoxelValue == materialID;

        if(isSameMaterial)
        {
            pxGreedyMaterial = pxGreedyMaterialCurrent;
            break;
        }
    }

    if(!pxGreedyMaterial)
    {
        // Add material
        pxGreedyMaterial = &pxVoxelChunk->GreedyMaterialStack[pxVoxelChunk->GreedyMaterialStackAmount++];
        pxGreedyMaterial->VoxelValue = materialID; // Set new.
        //pxGreedyMaterial->IndexOffset = 0;
        pxGreedyMaterial->IndexCount = 0;
    }

    return pxGreedyMaterial;
}

void PXMeshGenerateBuffersP3N3T2(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    PXMesh* pxMesh = pxVoxelChunk->Mesh;
    PXVertexBuffer* pxVertexBuffer = pxMesh->VertexBuffer;
    PXIndexBuffer* pxIndexBuffer = pxMesh->IndexBuffer;

    PXVector3F32 chunkOffset;
    chunkOffset.X = (PXF32)pxVoxelChunk->Position.X * (PXF32)pxVoxelGrid->ChunkSize;
    chunkOffset.Y = (PXF32)pxVoxelChunk->Position.Y * (PXF32)pxVoxelGrid->ChunkSize;
    chunkOffset.Z = (PXF32)pxVoxelChunk->Position.Z * (PXF32)pxVoxelGrid->ChunkSize;


    uint32_t quadCount = pxVoxelChunk->PXGreedyQuadAmount;

    //output.VertexCount = quadCount * 4;
    //output.IndexCount = quadCount * 6;

    // Allocate raw standard arrays
    //output.Vertices = (PXVertexP3N3T2*)malloc(output.VertexCount * sizeof(PXVertexP3N3T2));
    //output.Indices = (uint32_t*)malloc(output.IndexCount * sizeof(uint32_t));
    //output.MaterialIDs = (uint16_t*)malloc(output.VertexCount * sizeof(uint16_t));

    uint32_t vOffset = 0;
    uint32_t iCounter = 0;

    for(PXSize i = 0; i < quadCount; ++i)
    {
        PXGreedyQuad PXREF pxGreedyQuad = &pxVoxelChunk->PXGreedyQuadList[i];
        uint8_t axis = pxGreedyQuad->axis;

        PXF32 base_x = pxGreedyQuad->IndexInChunk.X + chunkOffset.X;
        PXF32 base_y = pxGreedyQuad->IndexInChunk.Y + chunkOffset.Y;
        PXF32 base_z = pxGreedyQuad->IndexInChunk.Z + chunkOffset.Z;
        PXF32 w = (PXF32)pxGreedyQuad->w;
        PXF32 h = (PXF32)pxGreedyQuad->h;

        // Face adjustment: Positive faces sit at the far edge of their unit voxel block
        if(axis == 0)
            base_x += 1.0f; // +X face pushed right

        if(axis == 2) 
            base_y += 1.0f; // +Y face pushed up

        if(axis == 4) 
            base_z += 1.0f; // +Z face pushed forward

        // Fetch corresponding directional normal
        float nx = FaceNormals[axis][0];
        float ny = FaceNormals[axis][1];
        float nz = FaceNormals[axis][2];

        PXVertexP3N3T2F32 vertexData[4];

        // vOffset + x

        // --- Build Corner 0: Bottom-Left (Offset: 0w, 0h) ---
        vertexData[0].Position.X = base_x;
        vertexData[0].Position.Y = base_y;
        vertexData[0].Position.Z = base_z;
        vertexData[0].Normal.X = nx;
        vertexData[0].Normal.Y = ny;
        vertexData[0].Normal.Z = nz;
        vertexData[0].Texture.X = 0.0f;
        vertexData[0].Texture.Y = 0.0f;
        //vertexData[0].MaterialIDs = quad->materialID;

        // --- Build Corner 1: Top-Left (Offset: 0w, 1h) ---
        vertexData[1].Position.X = base_x + QuadOffsetsH[axis][0] * h;
        vertexData[1].Position.Y = base_y + QuadOffsetsH[axis][1] * h;
        vertexData[1].Position.Z = base_z + QuadOffsetsH[axis][2] * h;
        vertexData[1].Normal.X = nx;
        vertexData[1].Normal.Y = ny;
        vertexData[1].Normal.Z = nz;
        vertexData[1].Texture.X = 0.0f;
        vertexData[1].Texture.Y = h; // Tiled vertically
        //vertexData[1].MaterialIDs = quad->materialID;

        // --- Build Corner 2: Top-Right (Offset: 1w, 1h) ---
        vertexData[2].Position.X = base_x + QuadOffsetsW[axis][0] * w + QuadOffsetsH[axis][0] * h;
        vertexData[2].Position.Y = base_y + QuadOffsetsW[axis][1] * w + QuadOffsetsH[axis][1] * h;
        vertexData[2].Position.Z = base_z + QuadOffsetsW[axis][2] * w + QuadOffsetsH[axis][2] * h;
        vertexData[2].Normal.X = nx;
        vertexData[2].Normal.Y = ny;
        vertexData[2].Normal.Z = nz;
        vertexData[2].Texture.X = w; // Tiled horizontally
        vertexData[2].Texture.Y = h; // Tiled vertically
        //vertexData[2].MaterialIDs = quad->materialID;

        // --- Build Corner 3: Bottom-Right (Offset: 1w, 0h) ---
        vertexData[3].Position.X = base_x + QuadOffsetsW[axis][0] * w;
        vertexData[3].Position.Y = base_y + QuadOffsetsW[axis][1] * w;
        vertexData[3].Position.Z = base_z + QuadOffsetsW[axis][2] * w;
        vertexData[3].Normal.X = nx;
        vertexData[3].Normal.Y = ny;
        vertexData[3].Normal.Z = nz;
        vertexData[3].Texture.X = w; // Tiled horizontally
        vertexData[3].Texture.Y = 0.0f;
        //vertexData[3].MaterialIDs = quad->materialID;



        pxVertexBuffer->Data.VertexP3N3T2F32[vOffset+0] = vertexData[0];
        pxVertexBuffer->Data.VertexP3N3T2F32[vOffset+1] = vertexData[1];
        pxVertexBuffer->Data.VertexP3N3T2F32[vOffset+2] = vertexData[2];
        pxVertexBuffer->Data.VertexP3N3T2F32[vOffset+3] = vertexData[3];


        // --- Standard Counter-Clockwise Element Indices ---

        PXGreedyMaterial* pxGreedyMaterial = 0;

        for(size_t i = 0; i < pxVoxelChunk->GreedyMaterialStackAmount; i++)
        {
            pxGreedyMaterial = &pxVoxelChunk->GreedyMaterialStack[i];

            if(pxGreedyMaterial->VoxelValue == pxGreedyQuad->materialID)
            {
                break;
            }

            pxGreedyMaterial = PXNull;
        }

        PXI32U iOffset = pxGreedyMaterial->Consumed;

        PXI32U* indexArray = (PXI32U*)PXIndexSegmentDataGetViaMaterial(pxIndexBuffer, pxGreedyMaterial->VoxelValue);

        indexArray = &indexArray[iOffset];

        PXAssert(indexArray[0] == 0, "Possible override detected");

        indexArray[0] = vOffset + 0;
        indexArray[1] = vOffset + 1;
        indexArray[2] = vOffset + 2;

        indexArray[3] = vOffset + 0;
        indexArray[4] = vOffset + 2;
        indexArray[5] = vOffset + 3;

        vOffset += 4;
        iCounter += 6;
        pxGreedyMaterial->Consumed += 6;
    }

    pxVertexBuffer->Data.SizeAllowedToUse = vOffset * sizeof(PXVertexP3N3T2F32);
    pxIndexBuffer->Data.SizeAllowedToUse = iCounter * sizeof(PXI32U);
}

void PXMeshQuadAdd(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk, PXGreedyQuad PXREF pxGreedyQuad)
{
    PXGreedyMaterial PXREF pxGreedyMaterial = PXMeshGreedyMaterialGet(pxVoxelChunk, pxGreedyQuad->materialID);  

    pxGreedyMaterial->IndexCount += 6;

    pxVoxelChunk->PXGreedyQuadList[pxVoxelChunk->PXGreedyQuadAmount++] = *pxGreedyQuad;
}

/*
PXBool skipCulling = 0;

// ignore
if(skipCulling)
{
    PXGreedyQuad quad;
    quad.IndexInChunk.X = 0;
    quad.IndexInChunk.Y = 0;
    quad.IndexInChunk.Z = 0;
    quad.w = (uint8_t)1;
    quad.h = (uint8_t)1; // Width spans X, Height spans Z
    quad.axis = 2; // +Y Axis identifier
    quad.materialID = 0;

    for(quad.IndexInChunk.Y = 0; quad.IndexInChunk.Y < chunkSize; ++quad.IndexInChunk.Y)
    {
        for(quad.IndexInChunk.Z = 0; quad.IndexInChunk.Z < chunkSize; ++quad.IndexInChunk.Z)
        {
            for(quad.IndexInChunk.X = 0; quad.IndexInChunk.X < chunkSize; ++quad.IndexInChunk.X)
            {
                int currentPtr = PXIndex3D(chunkSize, quad.IndexInChunk.X, quad.IndexInChunk.Y, quad.IndexInChunk.Z);
                PXI16U voxel = &pxVoxelChunk->Data.I16U[currentPtr];

                if(0 == voxel)
                {
                    continue;
                }

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }

    return;
}
*/

void PXMeshMendXP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    uint16_t sliceMaterials[16][16];
    uint16_t visibilityMask[16];

    for(PXSize x = 0; x < chunkSize; ++x)
    {
        for(PXSize y = 0; y < chunkSize; ++y)
            visibilityMask[y] = 0;

        // PHASE 1: Face Culling (X is constant, row elements match Y scanning)
        for(PXSize y = 0; y < chunkSize; ++y)
        {
            for(PXSize z = 0; z < chunkSize; ++z)
            {
                int currentPtr = PXIndex3D(chunkSize, x, y, z);
                uint16_t voxel = pxVoxelChunk->Data.I16U[currentPtr];

                uint16_t neighbor = 0;
                if(x < chunkSize - 1)
                    neighbor = pxVoxelChunk->Data.I16U[PXIndex3D(chunkSize, x + 1, y, z)];

                if(voxel > 0 && neighbor == 0)
                {
                    sliceMaterials[y][z] = voxel;
                    visibilityMask[y] |= (1 << z);
                }
            }
        }

        // PHASE 2: Greedy Merging (w along Z, h along Y)
        for(int y = 0; y < (int)chunkSize; ++y) {
            while(visibilityMask[y] > 0) {
                int z = _tzcnt_u32(visibilityMask[y]);
                uint16_t targetMaterial = sliceMaterials[y][z];

                int w = 1;
                while((z + w) < (int)chunkSize && (visibilityMask[y] & (1 << (z + w))) && (sliceMaterials[y][z + w] == targetMaterial)) w++;

                uint16_t combinedRowMask = ((1 << w) - 1) << z;

                int h = 1;
                while((y + h) < (int)chunkSize) {
                    if((visibilityMask[y + h] & combinedRowMask) != combinedRowMask) break;
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) {
                        if(sliceMaterials[y + h][z + k] != targetMaterial) { match = PXFalse; break; }
                    }
                    if(!match) break;
                    h++;
                }

                for(int ny = y; ny < y + h; ++ny)
                    visibilityMask[ny] &= ~combinedRowMask;

                PXGreedyQuad quad;
                quad.IndexInChunk.X = x;
                quad.IndexInChunk.Y = y;
                quad.IndexInChunk.Z = z;
                quad.w = w; // Width along Z
                quad.h = h; // Height along Y
                quad.axis = 0;
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}

void PXMeshMendXN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    uint16_t sliceMaterials[16][16];
    uint16_t visibilityMask[16];

    for(PXSize x = 0; x < chunkSize; ++x)
    {
        for(PXSize y = 0; y < chunkSize; ++y)
            visibilityMask[y] = 0;

        for(PXSize y = 0; y < chunkSize; ++y)
        {
            for(PXSize z = 0; z < chunkSize; ++z)
            {
                int currentPtr = PXIndex3D(chunkSize, x, y, z);
                uint16_t voxel = pxVoxelChunk->Data.I16U[currentPtr];

                uint16_t neighbor = 0;
                if(x > 0)
                    neighbor = pxVoxelChunk->Data.I16U[PXIndex3D(chunkSize, x - 1, y, z)];

                if(voxel > 0 && neighbor == 0)
                {
                    sliceMaterials[y][z] = voxel;
                    visibilityMask[y] |= (1 << z);
                }
            }
        }

        for(int y = 0; y < (int)chunkSize; ++y) {
            while(visibilityMask[y] > 0) {
                int z = _tzcnt_u32(visibilityMask[y]);
                uint16_t targetMaterial = sliceMaterials[y][z];

                int w = 1;
                while((z + w) < (int)chunkSize && (visibilityMask[y] & (1 << (z + w))) && (sliceMaterials[y][z + w] == targetMaterial)) w++;

                uint16_t combinedRowMask = ((1 << w) - 1) << z;

                int h = 1;
                while((y + h) < (int)chunkSize) {
                    if((visibilityMask[y + h] & combinedRowMask) != combinedRowMask) break;
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) {
                        if(sliceMaterials[y + h][z + k] != targetMaterial) { match = PXFalse; break; }
                    }
                    if(!match) break;
                    h++;
                }

                for(int ny = y; ny < y + h; ++ny)
                    visibilityMask[ny] &= ~combinedRowMask;

                PXGreedyQuad quad;
                quad.IndexInChunk.X = (PXF32)x;
                quad.IndexInChunk.Y = (PXF32)y;
                quad.IndexInChunk.Z = (PXF32)z;
                quad.w = (uint8_t)w; // Width along Z
                quad.h = (uint8_t)h; // Height along Y
                quad.axis = 1;
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}

void PXMeshMendYP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    PXI16U sliceMaterials[16][16];
    uint16_t visibilityMask[16];   

    for(PXSize y = 0; y < chunkSize; ++y)
    {
        for(PXSize z = 0; z < chunkSize; ++z)
            visibilityMask[z] = 0;

        // PHASE 1: SIMD Face Culling
        for(PXSize z = 0; z < chunkSize; ++z)
        {
            // X is still contiguous! We load starting at (0, y, z)
            int currentPtr = PXIndex3D(chunkSize, 0, y, z);
            __m256i currentReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            _mm256_store_si256((__m256i*) & sliceMaterials[z][0], currentReg);

            __m256i neighborReg;
            if(y < chunkSize - 1)
            {
                // Neighbor is at y + 1
                currentPtr = PXIndex3D(chunkSize, 0, y + 1, z);
                neighborReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            }
            else
            {
                neighborReg = _mm256_setzero_si256();
            }

            __m256i airReg = _mm256_setzero_si256();
            __m256i currentIsSolid = _mm256_cmpgt_epi16(currentReg, airReg);
            __m256i neighborIsAir = _mm256_cmpeq_epi16(neighborReg, airReg);
            __m256i facesVisible = _mm256_and_si256(currentIsSolid, neighborIsAir);

            uint32_t byteMask = _mm256_movemask_epi8(facesVisible);
            uint16_t rowMask = 0;
            for(int bit = 0; bit < chunkSize; ++bit)
            {
                if(byteMask & (1 << (bit * 2)))
                    rowMask |= (1 << bit);
            }
            visibilityMask[z] = rowMask;
        }

        // PHASE 2: Material Greedy Merging (Vertical axis is now Z)
        for(int z = 0; z < chunkSize; ++z) 
        {
            while(visibilityMask[z] > 0) 
            {
                int x = _tzcnt_u32(visibilityMask[z]);
                uint16_t targetMaterial = sliceMaterials[z][x];

                // 1. Expand horizontally (X axis)
                int w = 1;
                while((x + w) < chunkSize && (visibilityMask[z] & (1 << (x + w))) && (sliceMaterials[z][x + w] == targetMaterial)) {
                    w++;
                }

                uint16_t combinedRowMask = ((1 << w) - 1) << x;

                // 2. Expand vertically (Z axis)
                int h = 1;
                while((z + h) < chunkSize) {
                    // Check if the next row has the required visibility bits
                    if((visibilityMask[z + h] & combinedRowMask) != combinedRowMask) {
                        break;
                    }

                    // Verify all materials in that row's span match perfectly
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) 
                    {
                        if(sliceMaterials[z + h][x + k] != targetMaterial) 
                        {
                            match = PXFalse;
                            break;
                        }
                    }

                    if(!match) 
                    {
                        break; // Failed material match, stop expanding
                    }

                    h++; // Row is 100% valid, safe to increment height
                }

                // 3. Clear visibility masks safely for verified rows
                for(PXSize nz = z; nz < z + h; ++nz) 
                {
                    visibilityMask[nz] &= ~combinedRowMask;
                }

                // 4. Correctly initialize the Greedy Quad properties
                PXGreedyQuad quad;
                quad.IndexInChunk.X = (PXF32)x;
                quad.IndexInChunk.Y = (PXF32)y;
                quad.IndexInChunk.Z = (PXF32)z;
                quad.w = (uint8_t)w;
                quad.h = (uint8_t)h; // Width spans X, Height spans Z
                quad.axis = 2;       // +Y Axis identifier
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}

void PXMeshMendYN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    uint16_t sliceMaterials[16][16];// __attribute__((aligned(32)));
    uint16_t visibilityMask[16];

    for(PXSize y = 0; y < chunkSize; ++y)
    {
        for(PXSize z = 0; z < chunkSize; ++z)
            visibilityMask[z] = 0;

        // PHASE 1: SIMD Rows
        for(PXSize z = 0; z < chunkSize; ++z)
        {
            int currentPtr = PXIndex3D(chunkSize, 0, y, z);
            __m256i currentReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            _mm256_store_si256((__m256i*) & sliceMaterials[z][0], currentReg);

            __m256i neighborReg;
            if(y > 0)
            {
                currentPtr = PXIndex3D(chunkSize, 0, y - 1, z);
                neighborReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            }
            else
            {
                neighborReg = _mm256_setzero_si256();
            }

            __m256i airReg = _mm256_setzero_si256();
            __m256i currentIsSolid = _mm256_cmpgt_epi16(currentReg, airReg);
            __m256i neighborIsAir = _mm256_cmpeq_epi16(neighborReg, airReg);
            __m256i facesVisible = _mm256_and_si256(currentIsSolid, neighborIsAir);

            uint32_t byteMask = _mm256_movemask_epi8(facesVisible);
            uint16_t rowMask = 0;
            for(int bit = 0; bit < (int)chunkSize; ++bit)
            {
                if(byteMask & (1 << (bit * 2)))
                    rowMask |= (1 << bit);
            }
            visibilityMask[z] = rowMask;
        }

        // PHASE 2: Greedy Merging (w along X, h along Z)
        for(int z = 0; z < (int)chunkSize; ++z) {
            while(visibilityMask[z] > 0) {
                int x = _tzcnt_u32(visibilityMask[z]);
                uint16_t targetMaterial = sliceMaterials[z][x];

                int w = 1;
                while((x + w) < (int)chunkSize && (visibilityMask[z] & (1 << (x + w))) && (sliceMaterials[z][x + w] == targetMaterial)) w++;

                uint16_t combinedRowMask = ((1 << w) - 1) << x;

                int h = 1;
                while((z + h) < (int)chunkSize) {
                    if((visibilityMask[z + h] & combinedRowMask) != combinedRowMask) break;
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) {
                        if(sliceMaterials[z + h][x + k] != targetMaterial) { match = PXFalse; break; }
                    }
                    if(!match) break;
                    h++;
                }

                for(int nz = z; nz < z + h; ++nz)
                    visibilityMask[nz] &= ~combinedRowMask;

                PXGreedyQuad quad;
                quad.IndexInChunk.X = (PXF32)x;
                quad.IndexInChunk.Y = (PXF32)y;
                quad.IndexInChunk.Z = (PXF32)z;
                quad.w = (uint8_t)w; // Width along X
                quad.h = (uint8_t)h; // Height along Z
                quad.axis = 3;
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}

void PXMeshMendZP(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    uint16_t sliceMaterials[16][16];// __attribute__((aligned(32)));
    uint16_t visibilityMask[16];

    for(PXSize z = 0; z < chunkSize; ++z)
    {
        for(PXSize y = 0; y < chunkSize; ++y)
            visibilityMask[y] = 0;

        // PHASE 1: SIMD Row matching X slice transitions
        for(PXSize y = 0; y < chunkSize; ++y)
        {
            int currentPtr = PXIndex3D(chunkSize, 0, y, z);
            __m256i currentReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            _mm256_store_si256((__m256i*) & sliceMaterials[y][0], currentReg);

            __m256i neighborReg;
            if(z < chunkSize - 1)
            {
                currentPtr = PXIndex3D(chunkSize, 0, y, z + 1);
                neighborReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            }
            else
            {
                neighborReg = _mm256_setzero_si256();
            }

            __m256i airReg = _mm256_setzero_si256();
            __m256i currentIsSolid = _mm256_cmpgt_epi16(currentReg, airReg);
            __m256i neighborIsAir = _mm256_cmpeq_epi16(neighborReg, airReg);
            __m256i facesVisible = _mm256_and_si256(currentIsSolid, neighborIsAir);

            uint32_t byteMask = _mm256_movemask_epi8(facesVisible);
            uint16_t rowMask = 0;
            for(int bit = 0; bit < (int)chunkSize; ++bit)
            {
                if(byteMask & (1 << (bit * 2)))
                    rowMask |= (1 << bit);
            }
            visibilityMask[y] = rowMask;
        }

        // PHASE 2: Greedy Merging (w along X, h along Y)
        for(int y = 0; y < (int)chunkSize; ++y) {
            while(visibilityMask[y] > 0) {
                int x = _tzcnt_u32(visibilityMask[y]);
                uint16_t targetMaterial = sliceMaterials[y][x];

                int w = 1;
                while((x + w) < (int)chunkSize && (visibilityMask[y] & (1 << (x + w))) && (sliceMaterials[y][x + w] == targetMaterial)) w++;

                uint16_t combinedRowMask = ((1 << w) - 1) << x;

                int h = 1;
                while((y + h) < (int)chunkSize) {
                    if((visibilityMask[y + h] & combinedRowMask) != combinedRowMask) break;
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) {
                        if(sliceMaterials[y + h][x + k] != targetMaterial) { match = PXFalse; break; }
                    }
                    if(!match) break;
                    h++;
                }

                for(int ny = y; ny < y + h; ++ny)
                    visibilityMask[ny] &= ~combinedRowMask;

                PXGreedyQuad quad;
                quad.IndexInChunk.X = (PXF32)x;
                quad.IndexInChunk.Y = (PXF32)y;
                quad.IndexInChunk.Z = (PXF32)z;
                quad.w = (uint8_t)w; // Width along X
                quad.h = (uint8_t)h; // Height along Y
                quad.axis = 4;
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}

void PXMeshMendZN(PXVoxelGrid PXREF pxVoxelGrid, PXVoxelChunk PXREF pxVoxelChunk)
{
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    uint16_t sliceMaterials[16][16];// __attribute__((aligned(32)));
    uint16_t visibilityMask[16];

    for(PXSize z = 0; z < chunkSize; ++z)
    {
        for(PXSize y = 0; y < chunkSize; ++y)
            visibilityMask[y] = 0;

        // PHASE 1: SIMD Row matching X slice transitions
        for(PXSize y = 0; y < chunkSize; ++y)
        {
            int currentPtr = PXIndex3D(chunkSize, 0, y, z);
            __m256i currentReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            _mm256_store_si256((__m256i*) & sliceMaterials[y][0], currentReg);

            __m256i neighborReg;
            if(z > 0)
            {
                currentPtr = PXIndex3D(chunkSize, 0, y, z - 1);
                neighborReg = _mm256_loadu_si256((const __m256i*) & pxVoxelChunk->Data.I16U[currentPtr]);
            }
            else
            {
                neighborReg = _mm256_setzero_si256();
            }

            __m256i airReg = _mm256_setzero_si256();
            __m256i currentIsSolid = _mm256_cmpgt_epi16(currentReg, airReg);
            __m256i neighborIsAir = _mm256_cmpeq_epi16(neighborReg, airReg);
            __m256i facesVisible = _mm256_and_si256(currentIsSolid, neighborIsAir);

            uint32_t byteMask = _mm256_movemask_epi8(facesVisible);
            uint16_t rowMask = 0;
            for(int bit = 0; bit < (int)chunkSize; ++bit)
            {
                if(byteMask & (1 << (bit * 2)))
                    rowMask |= (1 << bit);
            }
            visibilityMask[y] = rowMask;
        }

        // PHASE 2: Greedy Merging (w along X, h along Y)
        for(int y = 0; y < (int)chunkSize; ++y) {
            while(visibilityMask[y] > 0) {
                int x = _tzcnt_u32(visibilityMask[y]);
                uint16_t targetMaterial = sliceMaterials[y][x];

                int w = 1;
                while((x + w) < (int)chunkSize && (visibilityMask[y] & (1 << (x + w))) && (sliceMaterials[y][x + w] == targetMaterial)) w++;

                uint16_t combinedRowMask = ((1 << w) - 1) << x;

                int h = 1;
                while((y + h) < (int)chunkSize) {
                    if((visibilityMask[y + h] & combinedRowMask) != combinedRowMask) break;
                    PXBool match = PXTrue;
                    for(int k = 0; k < w; ++k) {
                        if(sliceMaterials[y + h][x + k] != targetMaterial) { match = PXFalse; break; }
                    }
                    if(!match) break;
                    h++;
                }

                for(int ny = y; ny < y + h; ++ny)
                    visibilityMask[ny] &= ~combinedRowMask;

                PXGreedyQuad quad;
                quad.IndexInChunk.X = (PXF32)x;
                quad.IndexInChunk.Y = (PXF32)y;
                quad.IndexInChunk.Z = (PXF32)z;
                quad.w = (uint8_t)w; // Width along X
                quad.h = (uint8_t)h; // Height along Y
                quad.axis = 5;
                quad.materialID = targetMaterial;

                PXMeshQuadAdd(pxVoxelGrid, pxVoxelChunk, &quad);
            }
        }
    }
}













// 4D Direction offsets: [Face ID][Axis: X, Y, Z, W]
static const PXI8S DIR_4D[8][4] =
{
    { 1,  0,  0,  0}, {-1,  0,  0,  0}, // +X, -X (Right, Left)
    { 0,  1,  0,  0}, { 0, -1,  0,  0}, // +Y, -Y (Up, Down)
    { 0,  0,  1,  0}, { 0,  0, -1,  0}, // +Z, -Z (Forward, Backward)
    { 0,  0,  0,  1}, { 0,  0,  0, -1}  // +W, -W (Ana, Kata)
};

// 3D Direction offsets for neighbors (Up, Down, Left, Right, Forward, Back)
static const PXI8S DIR_3D[6][3] =
{
    { 1, 0, 0}, {-1,  0,  0},
    { 0, 1, 0}, { 0, -1,  0},
    { 0, 0, 1}, { 0,  0, -1}
};

// 2D Direction offsets for neighboring tiles (Up, Down, Left, Right)
static const PXI8S DIR_2D[4][2] =
{
    {1, 0}, {-1, 0},
    {0, 1}, {0, -1}
};

// Local vertex layout tables to construct flat quads based on face direction
static const PXI8S CUBE_OFFSETS[6][4][3] = {
    { {0,1,0}, {0,1,1}, {1,1,1}, {1,1,0} }, // Top (+Y)
    { {0,0,0}, {1,0,0}, {1,0,1}, {0,0,1} }, // Bottom (-Y)
    { {0,0,0}, {0,0,1}, {0,1,1}, {0,1,0} }, // Left (-X)
    { {1,0,0}, {1,1,0}, {1,1,1}, {1,0,1} }, // Right (+X)
    { {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1} }, // Forward (+Z)
    { {0,0,0}, {0,1,0}, {1,1,0}, {1,0,0} }  // Backward (-Z)
};


// Helper macro to calculate safe UV offsets based on a flat Atlas configuration
void APPLY_UV(int v_idx, int tex_id, int out_u, int out_v)
{
    do
    {
        float u_base = (float)(tex_id % 16) * 0.0625f;
        float v_base = (float)(tex_id / 16) * 0.0625f;
        float u_offs[4] = { 0.0f, 0.0625f, 0.0625f, 0.0f };
        float v_offs[4] = { 0.0f, 0.0f, 0.0625f, 0.0625f };
        out_u = u_base + u_offs[v_idx];
        out_v = v_base + v_offs[v_idx];
    } while(0);
}


/*

// Helper to push indexes safely into the preallocated buffer
static inline void PushQuadIndices(PX_GeometryScratchpad* scratch, uint32_t start_v)
{
    uint32_t* idx = &scratch->indices[scratch->index_count];
    idx[0] = start_v + 0;
    idx[1] = start_v + 1;
    idx[2] = start_v + 2;
    idx[3] = start_v + 2;
    idx[4] = start_v + 3;
    idx[5] = start_v + 0;
    scratch->index_count += 6;
}



PXBool IsInChunk2D(PXVector4I64U PXREF pxVector4I64U, const PXSize chunkSize)
{
    return
        pxVector4I64U->X >= 0 && pxVector4I64U->X < chunkSize &&
        pxVector4I64U->Y >= 0 && pxVector4I64U->Y < chunkSize;
}


void PX_GenerateMesh2D
(
    PXVoxelGrid PXREF pxVoxelGrid,
    const PXVoxelChunk PXREF pxVoxelChunk,
    PXVector4I64U PXREF pxPositionVoxelCurrent,
    const uint16_t* voxel_data,
    int current_w,
    PXMesh PXREF pxMesh
)
{
    // 2D ignores Z data entirely, interpreting the 3D block array as flat layers

    PXBool edge_visible = 0;

    PXVector4I64U neighborPosition;
    neighborPosition.X = 0;
    neighborPosition.Y = 0;
    neighborPosition.Z = 0;
    neighborPosition.W = 0;




    for(size_t i = 0; i < length; i++)
    {

    }






    // Look at 2D directions to determine edge visibility
    for(PXI8U directionIndex = 0; directionIndex < 4; directionIndex++)
    {
        neighborPosition.X = pxPositionVoxelCurrent->X + DIR_2D[directionIndex][0];
        neighborPosition.Y = pxPositionVoxelCurrent->Y + DIR_2D[directionIndex][1];

        const PXBool isInChunk = IsInChunk2D(&neighborPosition, pxVoxelGrid->ChunkSize);

        if(!isInChunk)
        {
            edge_visible = 1; // Border of chunk is exposed
            break;
        }

        uint16_t n_id = voxel_data[nx + (ny * CHUNK_SIZE)];

        if(!(g_VoxelRegistry[n_id].physics_flags & PHYSICS_SOLID))
        {
            edge_visible = 1; // Adjacent tile is air or transparent
            break;
        }
    }

    if(!edge_visible)
    {
        return;
    }

    if(edge_visible)
    {
        PX_VoxelDefinition voxel = g_VoxelRegistry[block_id];

        PXVertexP3N3T2F32 pxVertexList[4];

        // Emit 1 flat quad directly facing the camera
        for(PXI8U i = 0; i < 4; i++)
        {
            PXVertexP3N3T2F32* pxVertex = &pxVertexList[i];


            pxVertex->Position.X = (float)pxPositionVoxelCurrent->X + (i == 1 || i == 2 ? 1.0f : 0.0f);
            pxVertex->Position.Y = (float)pxPositionVoxelCurrent->Y + (i == 2 || i == 3 ? 1.0f : 0.0f);
            pxVertex->Position.Z = 0.0f; // Hardcoded Z axis layer
            APPLY_UV(i, voxel.texture_atlas_id, v->u, v->v);
            v->extra_dim_attr = (float)current_w; // Could be used as Z-sorting layout
        }



        scratch->vertex_count += 4;
        PushQuadIndices(scratch, start_v);
    }
}

void PXAPI PX_GenerateMesh3D
(
    PXVoxelGrid PXREF pxVoxelGrid,
    const PXVoxelChunk PXREF pxVoxelChunk,
    PXVector4I64U PXREF pxPositionVoxelCurrent,
    const uint16_t* voxel_data,
    int current_w,
    PX_GeometryScratchpad* scratch
)
{
    uint32_t idx = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
    uint16_t block_id = voxel_data[idx];

    if(block_id == VOXEL_AIR)
        continue;

    PX_VoxelDefinition voxel = g_VoxelRegistry[block_id];

    PXVector4I64U neighborPosition;
    neighborPosition.X = 0;
    neighborPosition.Y = 0;
    neighborPosition.Z = 0;
    neighborPosition.W = 0;

    // Loop through all 6 cube faces
    for(PXI8U face = 0; face < 6; face++)
    {
        neighborPosition.X = pxPositionVoxelCurrent->X + DIR_3D[face][0];
        neighborPosition.Y = pxPositionVoxelCurrent->Y + DIR_3D[face][1];
        neighborPosition.Z = pxPositionVoxelCurrent->Z + DIR_3D[face][2];

        // Check if neighbor occludes this face
        if(nx >= 0 && nx < CHUNK_SIZE && ny >= 0 && ny < CHUNK_SIZE && nz >= 0 && nz < CHUNK_SIZE) {
            uint32_t n_idx = nx + (ny * CHUNK_SIZE) + (nz * CHUNK_SIZE * CHUNK_SIZE);
            uint16_t n_id = voxel_data[n_idx];
            if(g_VoxelRegistry[n_id].physics_flags & PHYSICS_SOLID) {
                continue; // Occluded, cull face
            }
        }

        // Face is exposed! Build vertices straight inside the scratchpad
        uint32_t start_v = scratch->vertex_count;
        for(int i = 0; i < 4; i++) {
            PX_Vertex* v = &scratch->vertices[start_v + i];
            v->x = (float)x + CUBE_OFFSETS[face][i][0];
            v->y = (float)y + CUBE_OFFSETS[face][i][1];
            v->z = (float)z + CUBE_OFFSETS[face][i][2];
            APPLY_UV(i, voxel.texture_atlas_id, v->u, v->v);
            v->extra_dim_attr = 0.0f; // Unused in pure 3D
        }

        scratch->vertex_count += 4;
        PushQuadIndices(scratch, start_v);
    }
}


void PX_GenerateMesh4D(const uint16_t* voxel_data, int current_w, PX_GeometryScratchpad* scratch) {
    // We assume voxel_data passed here represents the 3D cross-section at a locked W layer.
    // However, to check if a block face is exposed to the 4th dimension (Ana/Kata),
    // we query hypothetical structural boundary states or use bitflags stored inside the element definitions.

    uint32_t idx = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
    uint16_t block_id = voxel_data[idx];
    if(block_id == VOXEL_AIR) continue;

    PX_VoxelDefinition voxel = g_VoxelRegistry[block_id];

    // 4D logic evaluates standard 3D faces first
    for(int face = 0; face < 6; face++) {
        int32_t nx = x + DIR_3D[face][0];
        int32_t ny = y + DIR_3D[face][1];
        int32_t nz = z + DIR_3D[face][2];

        bool exposed_3d = true;
        if(nx >= 0 && nx < CHUNK_SIZE && ny >= 0 && ny < CHUNK_SIZE && nz >= 0 && nz < CHUNK_SIZE) {
            uint32_t n_idx = nx + (ny * CHUNK_SIZE) + (nz * CHUNK_SIZE * CHUNK_SIZE);
            uint16_t n_id = voxel_data[n_idx];
            if(g_VoxelRegistry[n_id].physics_flags & PHYSICS_SOLID) {
                exposed_3d = false;
            }
        }

        // Check hyperdimensional exposure
        // If a block doesn't exist in the hyper-regions W+1 or W-1, it has a 4D edge.
        // For the sake of a lean memory pipeline example, we read if the block itself has morphing characteristics.
        bool exposed_4d = (block_id % 7 == 0); // Mock condition: certain blocks bleed into 4D space

        if(exposed_3d || exposed_4d) {
            uint32_t start_v = scratch->vertex_count;
            for(int i = 0; i < 4; i++) {
                PX_Vertex* v = &scratch->vertices[start_v + i];
                v->x = (float)x + CUBE_OFFSETS[face][i][0];
                v->y = (float)y + CUBE_OFFSETS[face][i][1];
                v->z = (float)z + CUBE_OFFSETS[face][i][2];
                APPLY_UV(i, voxel.texture_atlas_id, v->u, v->v);

                // CRUCIAL: Pass the hyper-dimensional delta metric to the GPU.
                // Shaders use this factor to dynamically morph alpha transparency or
                // change texture tints as blocks slide out of phase.
                v->extra_dim_attr = exposed_4d ? 0.5f : 1.0f;
            }

            scratch->vertex_count += 4;
            PushQuadIndices(scratch, start_v);
        }
    }
}






PXPublic void PXAPI PXVoxelGridChunkFacesProcess
(
    PXVoxelGrid PXREF pxVoxelGrid,
    const PXVoxelChunk PXREF pxVoxelChunk,
    PXVector4I64U PXREF pxPositionVoxelCurrent,
    PXSize element_id,
    MeshBuilder& builder
)
{
    PXVoxel PXREF pxVoxel = PXVoxelGridVoxelGetViaIndex(pxVoxelGrid, element_id);

    if(!pxVoxel)
    {
        return; // Invalid ID! Resolve??
    }

    // Define the 6 directions to check (Up, Down, Left, Right, Forward, Back)
    PXI8S directions[6][3] =
    {
        {0, 1, 0},
        {0, -1, 0},
        {-1, 0, 0},
        {1, 0, 0},
        {0, 0, 1},
        {0, 0, -1}
    };

    for(int face = 0; face < 6; face++)
    {
        PXVector4I64U neighborPosition;
        neighborPosition.X = pxVector->X + directions[face][0];
        neighborPosition.Y = pxVector->Y + directions[face][1];
        neighborPosition.Z = pxVector->Z + directions[face][2];
        neighborPosition.W = 0;

        // Face Culling Optimization Check:
        // If the neighbor is inside this chunk and is SOLID, hide this face!
        const PXSize chunkSize = pxVoxelGrid->ChunkSize;
        PXBool isInChunk =
            neighborPosition.X >= 0 && neighborPosition.X < chunkSize &&
            neighborPosition.Y >= 0 && neighborPosition.Y < chunkSize &&
            neighborPosition.Z >= 0 && neighborPosition.Z < chunkSize;

        if(isInChunk)
        {
            PXI16U* neighbor_id = PXVoxelGridChunkVoxelGet(pxVoxelGrid, pxVoxelChunk, &neighborPosition);

            const PXBool isSolid = PXFalse; // Check if block

            if(isSolid)
            {
                continue; // Neighbor blocks our view; skip drawing this face!
            }
        }

        // If the face is visible, push it to the MeshBuilder (Go to Step 3)
        builder.AddFace(x, y, z, face, voxel.texture_atlas_id);
    }
}








// On a background thread inside your MeshSystem:
PXPublic void PXAPI PXVoxelGridChunkFacesExtract
(
    PXVoxelGrid PXREF pxVoxelGrid,
    const PXVoxelChunk PXREF pxVoxelChunk,
    MeshBuilder& builder
)
{
    // 16x16x16 loop
    const PXSize chunkSize = pxVoxelGrid->ChunkSize;
    PXVector4I64U index;

    for(index.W = 0; index.W < chunkSize; ++index.W)
    {
        for(index.Z = 0; index.Z < chunkSize; ++index.Z)
        {
            for(index.Y = 0; index.Y < chunkSize; ++index.Y)
            {
                for(index.X = 0; index.X < chunkSize; ++index.X)
                {
                    PXI16U* value = PXVoxelGridChunkVoxelGet(pxVoxelGrid, pxVoxelChunk, &index);

                    // Skip processing completely if the cell is empty
                    if(*value == 0)
                    {
                        continue;
                    }

                    // Go to Step 2
                    ProcessVoxelFaces(chunk, x, y, z, chunk_element_id, builder);
                }
            }
        }
    }
}
*/
