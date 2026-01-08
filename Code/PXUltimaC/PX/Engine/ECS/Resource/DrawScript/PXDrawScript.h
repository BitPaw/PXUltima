#pragma once

#ifndef PXDrawScriptIncluded
#define PXDrawScriptIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXDrawScriptType_
{
    PXDrawScriptTypeInvalid,
    PXDrawScriptTypeAll,
    PXDrawScriptTypePixelState,
    PXDrawScriptTypeVertexState
}
PXDrawScriptType;

// A sequence of actions that tells the fixed pipline how to act.
// This is a precursor of a shader, so it's use is discurraged for
// modern applications and hardware that support shaders, not only
// for performance reasons but for versitility too.
typedef struct PXDrawScript_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        struct IDirect3DStateBlock9* StateBlock_9;
    };

    PXDrawScriptType Type;
}
PXDrawScript;

#endif


