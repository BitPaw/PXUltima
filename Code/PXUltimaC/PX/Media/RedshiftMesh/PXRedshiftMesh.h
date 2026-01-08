#pragma once

#ifndef PXRedshiftMeshIncluded
#define PXRedshiftMeshIncluded

#include <PX/Engine/PXResource.h>

#define PXR3D2HeaderFlagHasVcp                          (1<<0)
#define PXR3D2HeaderFlagHasLocalOriginLocatorAndPivot   (1<<1)

typedef struct R3D2MeshStaticFace_
{
    PXI32U VertexID[3];
    char Material[64];
    PXVector2F32 UV[3]; // (new(uvs[0], uvs[3]), new(uvs[1], uvs[4]), new(uvs[2], uvs[5])));
    PXColorRGBAI8 Color[3];
}
R3D2MeshStaticFace;

typedef struct PXR3D2Header_
{
    char Signature[8];
    PXI16U VersionMajor;
    PXI16U VersionMinor;
    char SourcePath[128];
    PXI32U VertexCount;
    PXI32U IndexCount;
    PXI32U Flags;
    PXVector3F32 BoundingBoxMinimim;
    PXVector3F32 BoundingBoxMaximum;
}
PXR3D2Header;

typedef struct PXR3D2_
{
    PXR3D2Header Header;

    PXVector3F32* VertexData;
    PXColorRGBAI8* VertexColors;
    PXVector3F32 CentralPoint;

    // Text only
    char Name[64];
    PXVector3F32 PivotPoint;

    R3D2MeshStaticFace* StaticFaceList;
}
PXR3D2;

PXPrivate PXResult PXAPI PXR3D2ParseBinary(PXR3D2 PXREF pxR3D2, PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPrivate PXResult PXAPI PXR3D2ParseText(PXR3D2 PXREF pxR3D2, PXResourceMoveInfo PXREF pxResourceLoadInfo);

PXPublic PXResult PXAPI PXR3D2LoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXR3D2SaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif