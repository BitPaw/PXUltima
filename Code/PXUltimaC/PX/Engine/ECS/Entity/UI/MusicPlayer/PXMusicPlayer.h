#pragma once

#ifndef PXMusicPlayerIncluded
#define PXMusicPlayerIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXMusicPlayer_
{
    int x;
}
PXMusicPlayer;

PXPublic PXResult PXAPI PXMusicPlayerDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif