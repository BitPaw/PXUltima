#pragma once

#ifndef PXMaterialIncluded
#define PXMaterialIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>
#include <PX/Media/PXColor.h>

typedef enum PXIlluminationMode_
{
    PXIlluminationNone,
    PXIlluminationColorAndAmbientDisable,        // [0] Color on and Ambient off
    PXIlluminationColorAndAmbientEnable,        // [1] Color on and Ambient on
    PXIlluminationHighlightEnable,    // [2] Highlight on
    PXIlluminationReflectionOnRayTraceEnable,    // [3] Reflection on and Ray trace on
    PXIlluminationReflectionOnRayTraceTransparency,     // [4] Transparency: Glass on, Reflection : Ray trace on
    PXIlluminationReflectionOnRayTraceFresnel,     // [5] Reflection : Fresnel on and Ray trace on
    PXIlluminationReflectionOnRayTraceTransparencyFresnel,     // [6] Transparency : Refraction on, Reflection : Fresnel offand Ray trace on
    PXIlluminationReflectionOnRayTraceFullEnable,    // [7] Transparency : Refraction on, Reflection : Fresnel onand Ray trace on
    PXIlluminationReflectionEnable,     // [8] Reflection on and Ray trace off
    PXIlluminationTransparencyEnable,     // [9] Transparency : Glass on, Reflection : Ray trace off
    PXIlluminationShadowsEnable      // [10] Casts shadows onto invisible surfaces
}
PXIlluminationMode;

typedef enum PXBlendingMode_
{
    PXBlendingModeInvalid,

    PXBlendingModeNone,

    PXBlendingModeOneToOne, // Direct 1:1 mixing
    PXBlendingModeSoureAlphaOnly
}
PXBlendingMode;

// Material for a surface to render on
typedef struct PXMaterial_
{
    PXECSInfo Info;

    //Texture* normalMap;
    //Texture* roughnessMap;
    //Texture* metalnessMap;

    float opacity;
    float roughness;
    float metalness;
    float emissionStrength;

   // Shader* shader;
   // Vector2 uvTiling;
   // Vector2 uvOffset;
   // BlendMode blendMode;
  //  bool doubleSided;


    // Name would be too wasteful here, we shall store it in another container
    // TexturePath can not be put here, but we might store it differently.

    PXColorRGBAF* DiffuseColor;
    PXTexture* DiffuseTexture;

    PXColorRGBAF* AmbientColor;
    PXTexture* AmbientTexture;

    PXColorRGBAF* SpecularColor;    // shininess
    PXTexture* SpecularTexture;

    PXColorRGBAF* EmissionColor;
    PXTexture* EmissionTexture;

    PXF32 Power;        // Sharpness if specular highlight

    PXF32 Weight;         // Ranges between 0 and 1000
    PXF32 Dissolved;
    PXF32 Density; // range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

    PXIlluminationMode IlluminationMode;
}
PXMaterial;


// Container to save a list of materials.
// This is only ment to be a faster lookup for a model.
typedef struct PXMaterialContainer_
{
    PXSize MaterialListAmount;
    PXMaterial* MaterialList;
}
PXMaterialContainer;

typedef struct PXMaterialCreateInfo_
{
    PXECSCreateInfo Info;
}
PXMaterialCreateInfo;


PXPublic PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer PXREF pxMaterialContainer, PXText PXREF pxMaterialName);

PXPublic PXResult PXAPI PXMaterialCreate(PXMaterial PXREF pxMaterial, PXMaterialCreateInfo PXREF pxMaterialCreateInfo);

#endif