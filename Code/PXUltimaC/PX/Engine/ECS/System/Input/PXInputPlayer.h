#pragma once

#ifndef PXInputPlayerIncluded
#define PXInputPlayerIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXVector.h>
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXInputPlayer_
{
    PXECSInfo Info;

    PXVector2F32 MovementWalk;
    PXVector2F32 MovementView;
}
PXInputPlayer;

#endif