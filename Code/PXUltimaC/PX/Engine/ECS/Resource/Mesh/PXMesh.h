#pragma once

#ifndef PXMeshIncluded
#define PXMeshIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/ECS/Component/Material/PXMaterial.h>
#include "PXVertexBuffer.h"
#include "PXBufferLayout.h"
#include "PXIndexBuffer.h"

/*
// OLD SOLUTION!


#define PXVertexBufferFormatFormat          0
#define PXVertexBufferFormatPosition        1
#define PXVertexBufferFormatTexturePosition 2
#define PXVertexBufferFormatNormal          3
#define PXVertexBufferFormatColor           4

#define PXVertexBufferFormatI08        0
#define PXVertexBufferFormatI16        1
#define PXVertexBufferFormatI32        2
#define PXVertexBufferFormatF16        3
#define PXVertexBufferFormatF32        4

#define PXVertexBufferFormatMake(format, position, texturePosition, normal, color) \
format << (4 * PXVertexBufferFormatFormat) | \
position << (4 * PXVertexBufferFormatPosition) | \
texturePosition << (4 * PXVertexBufferFormatTexturePosition) | \
normal << (4 * PXVertexBufferFormatNormal) | \
color << (4 * PXVertexBufferFormatColor)

#define PXVertexBufferFormatExtract(target, value) (value >> (4*target)) & 0x0F


#define PXVertexBufferFormatP3F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 3, 0, 0, 0)
#define PXVertexBufferFormatP3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 0, 0, 0)

#define PXVertexBufferFormatN3F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 0, 0, 3, 0)
#define PXVertexBufferFormatN3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 0, 0, 3, 0)

#define PXVertexBufferFormatT2F16 PXVertexBufferFormatMake(PXVertexBufferFormatF16, 0, 2, 0, 0)
#define PXVertexBufferFormatT2F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 0, 2, 0, 0)


#define PXVertexBufferFormatT2P3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 0, 0)
#define PXVertexBufferFormatT2N3P3F32 PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 3, 0)

typedef PXI32U PXVertexBufferFormat;
*/



//---------------------------------------------------------
// Vertex
//---------------------------------------------------------




#if 0

typedef enum PXVertexBufferFormat_
{
    PXVertexBufferFormatInvalid,

    // Non inteleaved
    PXVertexBufferFormatP2I8,   // XY - Int8 - Position 
    PXVertexBufferFormatP3I8,   // XYX - Int8 - Position 

    PXVertexBufferFormatP3F16,    // XYZ - PXF32 - Position
    PXVertexBufferFormatN3F16,    // XYZ - PXF32 - Normals 
    PXVertexBufferFormatT2F16,    // UV - PXF32 - Texture coordiantes

    PXVertexBufferFormatP3F32,    // XYZ - PXF32 - Position
    PXVertexBufferFormatN3F32,    // XYZ - PXF32 - Normals 
    PXVertexBufferFormatT2F32,    // UV - PXF32 - Texture coordiantes


    // OpenGL

    PXVertexBufferFormatP2F32, // GL_V2F
    PXVertexBufferFormatC4UBP2F32,
    PXVertexBufferFormatC4UBP3F32,
    PXVertexBufferFormatC3P3F32,
    PXVertexBufferFormatN3P3F32, // NNN PPP
    PXVertexBufferFormatC4N3P3F32,
    PXVertexBufferFormatT2P3F32, // TT PPP
    PXVertexBufferFormatT4P4,
    PXVertexBufferFormatT2C4UBP3,
    PXVertexBufferFormatT2C3P3,
    PXVertexBufferFormatT2N3P3, // TT NNN PPP
    PXVertexBufferFormatT2C4N3P3,
    PXVertexBufferFormatT4C4N3P4,

    // Direct X


    PXVertexBufferFormatXYZC, // Normal spcace (x, y, z, color-RGB as 32-int)

    PXVertexBufferFormatXYZRHW, // DirectX only, use for pixel size instead of normal space.

    PXVertexBufferFormatXYZHWC, // X, Y, Z, Tx, Ty, color-RGB as 32-int

    PXVertexBufferFormatP3B1,
    PXVertexBufferFormatP3B2,
    PXVertexBufferFormatP3B3,
    PXVertexBufferFormatP3B4,
    PXVertexBufferFormatP3B5,
    PXVertexBufferFormatP4,
}
PXVertexBufferFormat;
#endif




//PXPublic void PXAPI PXVertexBufferFormatInfoExtract(PXVertexBufferFormatInfo PXREF pxVertexBufferFormatInfo, const PXVertexBufferFormat pxVertexBufferFormat);




#define PXVertexBufferLayoutTypeInvalid     0
#define PXVertexBufferLayoutTypePosition    1
#define PXVertexBufferLayoutTypeNormal      2
#define PXVertexBufferLayoutTypeTexturePos  3
#define PXVertexBufferLayoutTypeColor       4
#define PXVertexBufferLayoutTypeCustom      5








/*
PXPublic void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer PXREF pxVertexBuffer, const PXVertexBufferFormat XVertexBufferFormat, const PXSize index);
*/



#define PXDrawModeIDQuad                    (1 << 0)
#define PXDrawModeIDInvalid                 (1 << 1)
#define PXDrawModeIDPoint                   (1 << 2)
#define PXDrawModeIDLine                    (1 << 3)
#define PXDrawModeIDLineLoop                (1 << 4)
#define PXDrawModeIDLineStrip               (1 << 5)
#define PXDrawModeIDLineStripAdjacency      (1 << 6)
#define PXDrawModeIDLineAdjacency           (1 << 7)
#define PXDrawModeIDTriangle                (1 << 8)
#define PXDrawModeIDTriangleAdjacency       (1 << 9)
#define PXDrawModeIDTriangleFAN             (1 << 10)
#define PXDrawModeIDTriangleStrip           (1 << 11)
#define PXDrawModeIDTriangleStripAdjacency  (1 << 12)
#define PXDrawModeIDWireFrame               (1 << 13)
#define PXDrawModeIDSquare                  (1 << 14)
#define PXDrawModeIDPatches                 (1 << 15)






typedef enum PXModelForm_
{
    PXModelFormInvalid,
    PXModelFormCustom,
    PXModelFormTriangle,
    PXModelFormRectangle,
    PXModelFormRectangleTX,
    PXModelFormCircle,
    PXModelFormCube
}
PXModelForm;






// A mesh is a structure that contains vertex and index data to render itself
typedef struct PXMesh_ PXMesh;



typedef struct PXMesh_
{
    PXECSInfo Info; // Contains VAO

    union
    {
        PXI32U VAO; // OpenGL
    };

    // Primary allocatedspace
    //PXSize VertexDataSize;
    //void* VertexDataAdress;

    // Can either store interleaved data or seperate ones
    PXSize VertexBufferListAmount; // Realistically, is not more than 1,2,3,4,..

    union
    {
        PXVertexBuffer VertexBufferPrime[PXEmbeddedArraySize]; // Can store position, normal, texturepos and one additional parameter 
        PXVertexBuffer* VertexBufferList; // Used as the same above, but can store many more.
    };

    PXIndexBuffer* IndexBuffer; // Contains IBO


    PXSize MaterialContaierListAmount;
    PXMaterialContainer* MaterialContaierList;
}
PXMesh;



typedef struct PXMeshCreateInfo_
{
    PXECSCreateInfo Info;

    PXVector3F32 Scaling;
}
PXMeshCreateInfo;



PXPublic PXResult PXAPI PXMeshRegisterToECS();


PXPublic PXResult PXAPI PXMeshVertexLayoutPrint(PXMesh PXREF pxMesh);

// Define and allocate vertex data to be stored and how
PXPublic PXResult PXAPI PXMeshVertexLayout(PXMesh PXREF pxMesh, const PXSize index, PXBufferLayout PXREF pxVertexBufferLayoutList, const PXSize amount);
PXPublic PXResult PXAPI PXMeshIndexLayout(PXMesh PXREF pxMesh, const PXSize primitveAmount, const PXSize segmentAmount);

PXPublic PXBufferLayout* PXAPI PXMeshVertexBufferGET(PXMesh PXREF pxMesh, const PXI8U type);

PXPublic void* PXAPI PXMeshVertexInsert(PXMesh PXREF pxMesh, const PXI8U type);
PXPublic void* PXAPI PXMeshIndexInsert(PXMesh PXREF pxMesh, const PXI8U type);

PXPublic PXSize PXAPI PXMeshVertexStrideGET(PXMesh PXREF pxMesh);

PXPublic PXVertexBuffer* PXAPI PXMeshVertexBufferListGET(PXMesh PXREF pxMesh);
PXPublic PXVertexBuffer* PXAPI PXMeshVertexBufferListSET(PXMesh PXREF pxMesh, const PXSize amount);

PXPublic PXSize PXAPI PXMeshIndexBufferLengthGET(PXMesh PXREF pxMesh);

PXPublic PXResult PXAPI PXMeshVertexLayoutTransmute(PXMesh PXREF pxMesh);
PXPublic PXResult PXAPI PXMeshNormalDataGenerate(PXMesh PXREF pxMesh);
PXPublic PXResult PXAPI PXMeshVertexArrayAdd
(
    PXMesh PXREF pxMesh,
    void* data,
    const PXSize dataLength,
    PXBufferLayout PXREF pxVertexBufferLayoutList,
    const PXSize pxVertexBufferLayoutListAmount
);
//---------------------------------------------------------


PXPublic PXSize PXAPI PXMeshTriangleAmount(PXMesh PXREF pxMesh);
PXPublic PXF32* PXAPI PXMeshTriangleIndex(PXMesh PXREF pxMesh, const PXSize index);
//PXPublic void* PXAPI PXMeshVertexDataInsertionPoint(PXMesh PXREF pxMesh, const PXVertexBufferDataType pxVertexBufferDataType);

PXPublic PXResult PXAPI PXMeshRegisterToECS();

PXPublic PXResult PXAPI PXMeshCreate(PXMesh** pxMeshREF, PXMeshCreateInfo PXREF pxMeshCreateInfo);

#endif