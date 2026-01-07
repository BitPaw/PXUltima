#pragma once

#ifndef PXWavefrontIncluded
#define PXWavefrontIncluded

#include <PX/Engine/PXResource.h>

#include <PX/Media/MTL/PXMTL.h>

#define PXWavefrontNameSize 64u
#define PXWavefrontElementNameLength 50u
#define PXWavefrontDebug 1

typedef enum PXWavefrontLineType_
{
    PXWavefrontLineInvalid,
    PXWavefrontLineNone,
    PXWavefrontLineComment,
    PXWavefrontLineMaterialLibraryIncluded,
    PXWavefrontLineMaterialLibraryUse,
    PXWavefrontLineObjectName,
    PXWavefrontLineObjectGroup,
    PXWavefrontLineVertexGeometric,
    PXWavefrontLineVertexTexture,
    PXWavefrontLineVertexNormal,
    PXWavefrontLineVertexParameter,
    PXWavefrontLineSmoothShading,
    PXWavefrontLineFaceElement,

    PXWavefrontEmbeddedMTL
}
PXWavefrontLineType;

typedef struct PXWavefrontElementMaterialInfo_
{
    PXSize MaterialIndex;
    PXSize Size; // Size of buffer to use given material
}
PXWavefrontElementMaterialInfo;

typedef struct PXWavefrontElement_
{
    char Name[PXWavefrontElementNameLength];
    PXF32 Smoothing;

    PXWavefrontElementMaterialInfo* MaterialInfo;
    PXSize MaterialInfoSize;

    PXSize VertexPositionListSize;
    PXF32* VertexPositionList;     // List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.

    PXSize TextureCoordinateListSize;
    PXF32* TextureCoordinateList;     // List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.

    PXSize VertexNormalPositionListSize;
    PXF32* VertexNormalPositionList;     // List of vertex normals in (x,y,z) form; normals might not be unit vectors.

    PXSize VertexParameterListSize;
    PXF32* VertexParameterList; // Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement.

    PXSize FaceElementListSize;
    unsigned int* FaceElementList; // Polygonal face element.
}
PXWavefrontElement;

PXPrivate PXWavefrontLineType PXAPI PXWavefrontPeekLine(const void* line, const PXSize size);


// allowed syntax is
// A: "f 1 2 3"
// B: "f 1/2/3"
// C: "f 1//3"
// D: "f 1/3"
PXPublic void PXAPI PXWavefrontFaceLineParse(PXCompiler PXREF pxCompiler, PXI32U PXREF PXI32U);

PXPublic PXResult PXAPI PXWavefrontLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWavefrontSaveFromFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
