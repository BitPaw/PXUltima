#pragma once

#ifndef PXLightIncluded
#define PXLightIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXLightType_
{
    PXLightTypeInvalid,
    PXLightTypePoint,
    PXLightTypeSpot,
    PXLightTypeDirectional
}
PXLightType;

typedef struct PXLight_
{
    PXECSInfo Info;

    PXF32 Diffuse[4];           // Diffuse color of light
    PXF32 Specular[4];          // Specular color of light
    PXF32 Ambient[4];           // Ambient color of light
    PXF32 Position[3];          // Position in world space
    PXF32 Direction[3];         // Direction in world space
    PXF32 CutoffRange;          // Cutoff range
    PXF32 Falloff;              // Falloff
    PXF32 AttenuationConstant;  // Constant attenuation
    PXF32 AttenuationLinear;    // Linear attenuation
    PXF32 AttenuationQuadratic; // Quadratic attenuation
    PXF32 Theta;                // Inner angle of spotlight cone
    PXF32 Phi;                  // Outer angle of spotlight cone

    PXLightType Type;           // Type of light source

    PXBool Enabled;
}
PXLight;

#endif