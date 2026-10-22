#include "PXRectangleNineCell.h"

#include <PX/Math/PXMath.h>
#include <PX/Type/PXVertex.h>
#include <PX/OS/PXOS.h>


const char PXRectangleNineCellName[] = "RectangleNineCell";
const PXI8U PXRectangleNineCellLength = sizeof(PXRectangleNineCellName);
const PXECSRegisterInfoStatic PXRectangleNineCellRegisterInfoStatic =
{
    {sizeof(PXRectangleNineCellName), sizeof(PXRectangleNineCellName), PXRectangleNineCellName, TextFormatASCII},
    sizeof(PXRectangleNineCell),
    __alignof(PXRectangleNineCell),
    PXECSTypeEntity,
    PXRectangleNineCellCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXRectangleNineCellRegisterInfoDynamic;

void PXAPI PXRectangleNineCellRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXRectangleNineCellRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXRectangleNineCellRegisterInfoDynamic;
}




// ------------------------------------------------------------------
// 9?slice indices – 9 quads × 2 triangles × 3 vertices = 54 entries
// ------------------------------------------------------------------
const PXI8U PXNineSliceIndices[54] =
{
    0,  4, 5, 0, 5, 1, // TL
    1,  5, 6, 1, 6, 2, // TC
    2,  6, 7, 2, 7, 3, // TR
    4,  8, 9, 4, 9, 5, // ML
    5,  9,10, 5,10, 6, // MC
    6, 10,11, 6,11, 7, // MR
    8, 12,13, 8,13, 9, // BL
    9, 13,14, 9,14,10, // BC
    10,14,15,10,15,11 // BR
};
const PXI8U PXNineSliceIndicesSize = sizeof(PXNineSliceIndices);


PXIndexBuffer* PXNineSliceIndexBuffer = 0;


PXResult PXAPI PXRectangleNineCellCreate(PXRectangleNineCell** pxRectangleNineCellREF, PXRectangleNineCellCreateInfo PXREF pxRectangleNineCellCreateInfo)
{
    PXRectangleNineCell* pxRectangleNineCell = PXNull;

    pxRectangleNineCellCreateInfo->Info.Static = &PXRectangleNineCellRegisterInfoStatic;
    pxRectangleNineCellCreateInfo->Info.Dynamic = &PXRectangleNineCellRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxRectangleNineCellREF, pxRectangleNineCellCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxRectangleNineCell = *pxRectangleNineCellREF;

    //-----------------------------------------------------
    if(!PXNineSliceIndexBuffer)
    {
        PXIndexBufferCreateInfo pxIndexBufferCreateInfo;
        PXClear(PXIndexBufferCreateInfo, &pxIndexBufferCreateInfo);
        PXTextFromAddressA(&pxIndexBufferCreateInfo.Info.Name, "NineSlice_IndexBuffer", 22, 22);
        pxIndexBufferCreateInfo.IsConstData = PXTrue;
        pxIndexBufferCreateInfo.IndexDataAddressConst = PXNineSliceIndices;
        pxIndexBufferCreateInfo.IndexAmount = sizeof(PXNineSliceIndices);
        pxIndexBufferCreateInfo.Dimensions = 3;
        pxIndexBufferCreateInfo.BitPerIndex = 8;
        pxIndexBufferCreateInfo.AmountSegments = 1; // aka amount of materials

        pxResult = PXIndexBufferCreate(&PXNineSliceIndexBuffer, &pxIndexBufferCreateInfo);
    }
    //-----------------------------------------------------


    // DEF
    pxRectangleNineCell->width = 2.0f;
    pxRectangleNineCell->height = 1.0f;
    pxRectangleNineCell->BorderInnerInPixel.Left = 9;
    pxRectangleNineCell->BorderInnerInPixel.Right = 9;
    pxRectangleNineCell->BorderInnerInPixel.Top = 9;
    pxRectangleNineCell->BorderInnerInPixel.Bottom = 9;
    pxRectangleNineCell->BorderOuterInPixel.Left = 0.0f;
    pxRectangleNineCell->BorderOuterInPixel.Right = 0.0f;
    pxRectangleNineCell->BorderOuterInPixel.Top = 0.0f;
    pxRectangleNineCell->BorderOuterInPixel.Bottom = 0.0f;
    pxRectangleNineCell->Texture = pxRectangleNineCellCreateInfo->Texture;

    PXRectangleNineCellVertexCalc(pxRectangleNineCell);

    {
        PXMeshCreateInfo pxMeshCreateInfo;
        PXClear(PXMeshCreateInfo, &pxMeshCreateInfo);
        PXTextFromAddressA(&pxMeshCreateInfo.Info.Name, "NineSlice_Mesh", 15, 15);
        pxMeshCreateInfo.IndexBuffer = PXNineSliceIndexBuffer;
        pxMeshCreateInfo.VertexBuffer = pxRectangleNineCell->VertexBuffer;

        pxResult = PXMeshCreate(&pxRectangleNineCell->Mesh, &pxMeshCreateInfo);
    }

    {
        PXModelCreateInfo pxModelCreateInfo;
        PXClear(PXModelCreateInfo, &pxModelCreateInfo);
        PXTextFromAddressA(&pxModelCreateInfo.Info.Name, "NineSlice_Model", 16, 16);
        pxModelCreateInfo.MeshList = pxRectangleNineCell->Mesh;
        pxModelCreateInfo.MeshListAmount = 1;

        pxResult = PXModelCreate(&pxRectangleNineCell->Model, &pxModelCreateInfo);
    }

    return PXResultOK;
}

PXResult PXAPI PXRectangleNineCellVertexCalc(PXRectangleNineCell PXREF pxRectangleNineCell)
{
    PXResult pxResult = PXResultInvalid;
    PXVertexP3N3T2F32 pxVertexP3N3T2List[16];

    const PXSize pxVertexP3N3T2ListSize = sizeof(pxVertexP3N3T2List);

    const PXF32 textureWidth = pxRectangleNineCell->Texture->Width;
    const PXF32 textureHeight = pxRectangleNineCell->Texture->Height;

    /* ---------- Sanity / clamping --------------------------------------- */
    // Avoid degenerate geometry if borders are too large.

    pxRectangleNineCell->BorderInnerNormalized.Left = pxRectangleNineCell->BorderInnerInPixel.Left / textureWidth;
    pxRectangleNineCell->BorderInnerNormalized.Top = pxRectangleNineCell->BorderInnerInPixel.Top / textureHeight;
    pxRectangleNineCell->BorderInnerNormalized.Right = pxRectangleNineCell->BorderInnerInPixel.Right / textureWidth;
    pxRectangleNineCell->BorderInnerNormalized.Bottom = pxRectangleNineCell->BorderInnerInPixel.Bottom / textureHeight;
   
    pxRectangleNineCell->BorderOuterNormalized.Left = pxRectangleNineCell->BorderOuterInPixel.Left / textureWidth;
    pxRectangleNineCell->BorderOuterNormalized.Top = pxRectangleNineCell->BorderOuterInPixel.Top / textureHeight;
    pxRectangleNineCell->BorderOuterNormalized.Right = pxRectangleNineCell->BorderOuterInPixel.Right / textureWidth;
    pxRectangleNineCell->BorderOuterNormalized.Bottom = pxRectangleNineCell->BorderOuterInPixel.Bottom / textureHeight;

    PXRectangleLTRBF32 vertexPoint;

    vertexPoint.Left = PXMathMinimum(pxRectangleNineCell->BorderInnerNormalized.Left, pxRectangleNineCell->width * 0.5f);
    vertexPoint.Right = PXMathMinimum(pxRectangleNineCell->BorderInnerNormalized.Right, pxRectangleNineCell->width * 0.5f);
    vertexPoint.Top = PXMathMinimum(pxRectangleNineCell->BorderInnerNormalized.Top, pxRectangleNineCell->height * 0.5f);
    vertexPoint.Bottom = PXMathMinimum(pxRectangleNineCell->BorderInnerNormalized.Bottom, pxRectangleNineCell->height * 0.5f);



    /* ---------- UV boundaries ------------------------------------------ */
    // These are the fractions of the texture that belong to each slice.

#if 1
    const PXF32 u_start = 0.0f;
    const PXF32 u_end = 1.0f;
    const PXF32 v_start = 0.0f;
    const PXF32 v_end = 1.0f;

    const PXF32 u_left = pxRectangleNineCell->BorderInnerNormalized.Left;        // [0,1]
    const PXF32 v_bottom = pxRectangleNineCell->BorderInnerNormalized.Bottom;       // lower?left origin
    const PXF32 u_right = 1.0f-pxRectangleNineCell->BorderInnerNormalized.Right;
    const PXF32 v_top = 1.0f - pxRectangleNineCell->BorderInnerNormalized.Top;
#else
    // Find where the usable texture area actually begins and ends [0.0 to 1.0]
    PXF32 u_start = pxRectangleNineCell->BorderInner.Left / textureWidth;
    PXF32 u_end = 1.0f - (pxRectangleNineCell->BorderInner.Right / textureWidth);
    PXF32 v_start = pxRectangleNineCell->BorderInner.Bottom / textureHeight;
    PXF32 v_end = 1.0f - (pxRectangleNineCell->BorderInner.Top / textureHeight);

    // The slice points must now be relative to the usable inner texture region
    PXF32 u_left = u_start + (clampedLeft / textureWidth);
    PXF32 u_right = u_end - (clampedRight / textureWidth);
    PXF32 v_bottom = v_start + (clampedBottom / textureHeight);
    PXF32 v_top = v_end - (clampedTop / textureHeight);
#endif



    /* ---------- Grid of positions & UVs -------------------------------- */
    const PXF32 x[4] = 
    {
        0.f, 
        vertexPoint.Left,
        pxRectangleNineCell->width - vertexPoint.Right,
        pxRectangleNineCell->width
    };
    const PXF32 y[4] =
    {
        0.f, 
        vertexPoint.Bottom,
        pxRectangleNineCell->height - vertexPoint.Top,
        pxRectangleNineCell->height 
    };

    PXF32 u[4] = 
    { 
        u_start, 
        u_left, 
        u_right, 
        u_end 
    };
    PXF32 v[4] = 
    { 
        v_start,
        v_bottom,
        v_top,
        v_end
    };

    // Build the 4×4 grid (16 unique vertices)
    for(PXI8U yi = 0; yi < 4; ++yi)
    {
        for(PXI8U xi = 0; xi < 4; ++xi)
        {
            PXVertexP3N3T2F32 PXREF pxVertexP3N3T2 = &pxVertexP3N3T2List[yi + (xi * 4)];
            pxVertexP3N3T2->Position.X = x[xi];
            pxVertexP3N3T2->Position.Y = y[yi];
            pxVertexP3N3T2->Position.Z = 0.f; // UI plane
            pxVertexP3N3T2->Normal.X = 0.f;
            pxVertexP3N3T2->Normal.Y = 0.f;
            pxVertexP3N3T2->Normal.Z = 1.f; // constant normal
            pxVertexP3N3T2->Texture.X = u[xi];
            pxVertexP3N3T2->Texture.Y = v[yi];
        }
    }

    //-----------------------------------------------------
    {
        PXVertexBufferCreateInfo pxVertexBufferCreateInfo;
        PXClear(PXVertexBufferCreateInfo, &pxVertexBufferCreateInfo);
        PXTextFromAddressA(&pxVertexBufferCreateInfo.Info.Name, "NineSlice_VertexBuffer", 23, 23);
        pxVertexBufferCreateInfo.VertexDataSize = pxVertexP3N3T2ListSize;
        pxVertexBufferCreateInfo.VertexDataAddress = pxVertexP3N3T2List;

        pxResult = PXVertexBufferCreate(&pxRectangleNineCell->VertexBuffer, &pxVertexBufferCreateInfo);
    }
    //-----------------------------------------------------

    return pxResult;
}