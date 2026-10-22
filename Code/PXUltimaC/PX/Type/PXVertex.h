#pragma once

#ifndef PXVertexIncluded
#define PXVertexIncluded

#include <PX/Math/PXVector.h>

// Classical vertex used in most of all games
typedef struct PXVertexP3N3T2F32_
{
    PXVector3F32 Position;
    PXVector3F32 Normal;
    PXVector2F32 Texture;
}
PXVertexP3N3T2F32;

// 4D version
typedef struct PXVertexP4N4T2F32_
{
    PXVector4F32 Position;
    PXVector4F32 Normal; 
    PXVector2F32 Texture;
}
PXVertexP4N4T2F32;

#endif