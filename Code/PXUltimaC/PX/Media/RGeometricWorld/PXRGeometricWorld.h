#pragma once

#ifndef PXRGeometricWorldIncluded
#define PXRGeometricWorldIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXRGeometricWorldMesh_
{
    char TexturePath[260];
    char MaterialName[64];

    float Sphere[4]; // X, Y, Z, Radius
    float Rectangle[6]; // A: XYZ, B: XYZ 

    PXI32U VertexCount;
    PXI32U IndexCount;
}
PXRGeometricWorldMesh;

typedef struct PXRGeometricWorldHeader_
{
    char Signature[4];
    PXI16U VersionMajor;
    PXI16U VersionMinor;
    PXI32U ModelCount;
    PXI32U FaceCount;
}
PXRGeometricWorldHeader;

typedef struct PXRGeometricWorld_
{
    PXRGeometricWorldHeader Header;

    PXRGeometricWorldMesh* GeometryMeshList;
}
PXRGeometricWorld;

PXPublic PXResult PXAPI PXRGeometricWorldLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRGeometricWorldSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif