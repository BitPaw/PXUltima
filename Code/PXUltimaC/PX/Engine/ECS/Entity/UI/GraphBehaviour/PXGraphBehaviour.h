#pragma once

#ifndef PXGraphBehaviourIncluded
#define PXGraphBehaviourIncluded
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXGraphBehaviour_
{
    int x;
}
PXGraphBehaviour;

PXPublic PXResult PXAPI PXGraphBehaviourDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif