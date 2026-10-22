#pragma once

#ifndef PXMusicPlayerIncluded
#define PXMusicPlayerIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXMusicPlayer_
{
    int x;
}
PXMusicPlayer;

PXPublic PXResult PXAPI PXMusicPlayerDraw(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo);

#endif