#pragma once

#ifndef PXNetworkTesterIncluded
#define PXNetworkTesterIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXNetworkTester_
{
    int x;
}
PXNetworkTester;

PXPublic PXResult PXAPI PXNetworkTesterDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif