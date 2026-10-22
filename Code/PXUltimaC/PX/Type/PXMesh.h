#pragma once

#ifndef PXMeshIncluded
#define PXMeshIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXMaterial.h>
#include <PX/Type/PXVertexBuffer.h>
#include <PX/Type/PXIndexBuffer.h>
#include <PX/Type/PXBufferLayout.h>

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




//PXPublic void PXAPI PXVertexBufferFormatInfoExtract(PXVertexBufferFormatInfo PXREF pxVertexBufferFormatInfo, const PXVertexBufferFormat pxVertexBufferFormat);


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

// Container for vertex and index data.
typedef struct PXMesh_
{
    PXECSInfo Info;

    union
    {
        PXI32U VAO; // OpenGL
    };

    PXIndexBuffer* IndexBuffer; // IBO
    PXVertexBuffer* VertexBuffer; // VBO
}
PXMesh;

typedef struct PXMeshCreateInfo_
{
    PXECSCreateInfo Info;

    PXIndexBuffer* IndexBuffer;
    PXVertexBuffer* VertexBuffer;
}
PXMeshCreateInfo;

PXPublic void PXAPI PXMeshRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXMeshCreate(PXMesh** pxMeshREF, PXMeshCreateInfo PXREF pxMeshCreateInfo);
PXPublic PXResult PXAPI PXMeshRelease(PXMesh PXREF pxMesh);
PXPublic PXResult PXAPI PXMeshDraw(PXMesh PXREF pxMesh, PXEngineUIDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXMeshVertexLayoutTransmute(PXMesh PXREF pxMesh);

#endif