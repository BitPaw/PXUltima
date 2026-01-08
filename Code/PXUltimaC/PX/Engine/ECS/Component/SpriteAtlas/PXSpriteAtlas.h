#pragma once

#ifndef PXSpriteAnimationIncluded
#define PXSpriteAnimationIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Math/PXVector.h>
#include <PX/Engine/ECS/Resource/Shader/PXShader.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>

typedef struct PXSpriteMapEntity_
{
    PXVector2F32 Position; // XY in the world, will be alligned to world space
    PXVector2F32 SizeInCells;
    PXVector2F32 Offset; // Slight offset, used for animation or general control.
}
PXSpriteMapEntity;


typedef struct PXSpriteMap_
{
    PXECSInfo Info;

    PXShaderProgram* Shader;
    PXModel* Model;
    PXTexture* Texture;

    PXSpriteMapEntity* SpriteMapEntityList;
}
PXSpriteMap;


typedef struct PXSpriteMapInfo_
{
    PXECSCreateInfo Info;

    PXShaderProgram* ShaderProgramCurrent;

    PXVector2I32S CellSize;
    PXVector2I32U MapSize;

    char* MapFilePath; // The texture that contains all textures to build the map.
}
PXSpriteMapInfo;


PXPublic PXResult PXAPI PXSpriteMapCreate(PXSpriteMap PXREF pxSpriteMap, PXSpriteMapInfo PXREF pxSpriteMapInfo);

#endif