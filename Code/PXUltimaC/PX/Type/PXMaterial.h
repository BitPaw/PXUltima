#pragma once

#ifndef PXMaterialIncluded
#define PXMaterialIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXTexture.h>
#include <PX/Type/PXColor.h>
#include <PX/Type/PXShader.h>
#include <PX/Type/PXTextureCube.h>

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

    PXShaderProgram* ShaderProgram;

    //Texture* normalMap;
    //Texture* roughnessMap;
    //Texture* metalnessMap;

    PXF32 opacity;
    PXF32 roughness;
    PXF32 metalness;
    PXF32 emissionStrength;

   // Shader* shader;
   // Vector2 uvTiling;
   // Vector2 uvOffset;
   // BlendMode blendMode;
  //  bool doubleSided;

    PXTextureCube* TextureCube;


    // Name would be too wasteful here, we shall store it in another container
    // TexturePath can not be put here, but we might store it differently.

    PXColorRGBAF* ColorBase;
    PXTexture2D* TextureBase; //Diffuse, Albedo, 

    PXColorRGBAF* AmbientColor;
    PXTexture2D* AmbientTexture;

    PXColorRGBAF* SpecularColor;    // shininess
    PXTexture2D* SpecularTexture;

    PXColorRGBAF* EmissionColor;
    PXTexture2D* EmissionTexture;

    PXF32 Power;        // Sharpness if specular highlight

    PXF32 Weight;         // Ranges between 0 and 1000
    PXF32 Dissolved;
    PXF32 Density; // range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.

    PXIlluminationMode IlluminationMode;
}
PXMaterial;

typedef struct PXMaterialCreateInfo_
{
    PXECSCreateInfo Info;

    PXShaderProgram* ShaderProgram;
    PXColorRGBAF* ColorBase;
    PXTexture2D* TextureBase;
}
PXMaterialCreateInfo;

PXInternal void PXAPI PXMaterialRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXMaterialCreate(PXMaterial** pxMaterialREF, PXMaterialCreateInfo PXREF pxMaterialCreateInfo);

#endif