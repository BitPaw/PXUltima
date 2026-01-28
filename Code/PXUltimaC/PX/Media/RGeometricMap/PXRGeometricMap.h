#pragma once

#ifndef PXRGeometricMapIncluded
#define PXRGeometricMapIncluded

#include <PX/Engine/PXResource.h>

#include <PX/Math/PXMatrix.h>

typedef struct PXRGeometricMapHeader_
{
    char Signature[4];
    PXI32U Version;
}
PXRGeometricMapHeader;


typedef struct PXRGeometricMapSceneGraphGeometryBucket_
{
    PXF32 MaxStickOutX;
    PXF32 MaxStickOutZ;
    PXI32U StartIndex;
    PXI32U BaseVertex;
    PXI16U InsideFaceCount;
    PXI16U StickingOutFaceCount;
}
PXRGeometricMapSceneGraphGeometryBucket;

#define PXRGeometricMapSceneGraphHasFaceVisibilityFlags (1 << 0)

typedef struct PXRGeometricMapSceneGraph_
{
    PXI32U VisibilityControllerPathHash;

    PXF32 MinX;
    PXF32 MinZ;
    PXF32 MaxX;
    PXF32 MaxZ;
    PXF32 MaxStickOutX;
    PXF32 MaxStickOutZ;
    PXF32 BucketSizeX;
    PXF32 BucketSizeZ;

    PXI16U BucketsPerSide;
    PXBool IsDisabled;
    PXI8U Flags;

    PXI32U VertexCount;
    PXI32U IndexCount;

    PXI32U FaceCount;

    PXVector3F32* VertexData;
    PXVector3F32* IndexData;

    PXRGeometricMapSceneGraphGeometryBucket* GeometryBucket;


    PXBool IsLegacy;
}
PXRGeometricMapSceneGraph;


typedef struct PXEnvironmentAssetChannel_
{
    PXI32U TextureNameSize; // CorrectName??
    char TextureName[64]; // CorrectName??
    PXVector2F32 TextureScale; // CorrectName??
    PXVector2F32 TextureBias; // CorrectName??
}
PXEnvironmentAssetChannel;



typedef struct PXRGeometricMapShaderEntry_
{
    PXI32U Index;
    PXI32U NameLength;
    char Name[64];
}
PXRGeometricMapShaderEntry;

typedef struct PXRGeometricMapMeshElement_
{
    PXI32U Hash;
    PXI32U MaterialNameLength;
    PXSize MaterialNameOffset;
    PXI32U StartIndex;
    PXI32U IndexCount;
    PXI32U MinVertex;
    PXI32U MaxVertex;
}
PXRGeometricMapMeshElement;


typedef struct PXRGeometricMapMesh_
{
    char Name[260];
    PXI32U NameSize;
    PXI32U VertexCount;
    PXI32U VertexDeclarationCount;

    // This ID is always that of the "instanced" vertex buffer which
    // means that the data of the first vertex buffer is instanced (unique to this mesh instance).
    // (Assuming that this mesh uses at least 2 vertex buffers)
    PXI32U VertexDeclarationID;

    PXI32U IndexCount;
    PXI32U IndexBufferID;



    PXI32U SubMeshCount;

    union
    {
        PXRGeometricMapMeshElement SubMeshElementPrime;
        PXRGeometricMapMeshElement* SubMeshElementList;
    };





    PXVector3F32 BoundingBox[2];
    PXMatrix4x4F Transform;

    PXVector3F32 PointLight;

    PXVector2F32 BakedPaintScale;
    PXVector2F32 BakedPaintBias;


    PXVector3F32 SphericalHarmonics[9];


    PXEnvironmentAssetChannel BakedLight;
    PXEnvironmentAssetChannel BakedPaint;
    PXEnvironmentAssetChannel StationaryLight;


    PXI32U VisibilityControllerPathHash;
    PXI8U EnvironmentVisibilityFlags;

    PXI8U DisableBackfaceCulling;
    PXI8U EnvironmentQualityFilterFlags;
    PXI8U VisibilityFlags;

    PXI8U RenderFlags;
    PXI8U LayerTransitionBehavior;

    PXI16U EnvironmentAssetMeshRenderFlags;
}
PXRGeometricMapMesh;

typedef struct PXRGeometricMapVertexDeclaration_
{
    PXI32U VertexBufferUsage; // Can be ignored, as 0 is expected to be default
    PXI32U VertexElementCount;
}
PXRGeometricMapVertexDeclaration;


typedef struct PXRGeometricMap_
{
    PXRGeometricMapHeader Header;

    PXI32U ShaderEntryAmount;

    union
    {
        PXRGeometricMapShaderEntry ShaderEntryPrime[2]; 
        PXRGeometricMapShaderEntry* ShaderEntryList;
    };

    PXI32U VertexDeclarationCount;
    PXRGeometricMapVertexDeclaration* VertexDeclarationList;

    PXI32U VertexBufferCount;
    PXI32U IndexBufferCount;
    PXI32U MeshCount;
    PXI32U SceneGraphCount;
    PXI32U PlanarReflectorCount;

    union
    {
        PXRGeometricMapSceneGraph SceneGraphPrime;
        PXRGeometricMapSceneGraph* SceneGraphList;
    };

    PXBool UseSeparatePointLights;
}
PXRGeometricMap;

PXPrivate void PXAPI PXRGeometricMapBucketedGeometryLoad(PXRGeometricMap PXREF pxRGeometricMap, PXRGeometricMapSceneGraph PXREF pxRGeometricMapSceneGraph, PXFile PXREF pxFile);
PXPrivate void PXAPI PXRGeometricMapMeshLoad(PXRGeometricMap PXREF pxRGeometricMap, PXRGeometricMapMesh PXREF pxRGeometricMapMesh, PXFile PXREF pxFile);

PXPublic PXResult PXAPI PXRGeometricMapLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRGeometricMapSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif