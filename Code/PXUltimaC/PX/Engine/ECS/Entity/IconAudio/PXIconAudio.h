#pragma once

#ifndef PXIconAudioIncluded
#define PXIconAudioIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXIconAudio_
{
    PXECSInfo Info;
}
PXIconAudio;

typedef struct PXIconAudioCreateInfo_
{
    PXECSCreateInfo Info;
}
PXIconAudioCreateInfo;

PXPublic PXResult PXAPI PXIconAudioRegisterToECS(void);
PXPublic PXResult PXAPI PXIconAudioCreate(PXIconAudio** pxIconAudioREF, PXIconAudioCreateInfo PXREF pxIconAudioCreateInfo);
PXPublic PXResult PXAPI PXIconAudioRelease(PXIconAudio PXREF pxIconAudio);
PXPublic PXResult PXAPI PXIconAudioDraw(PXIconAudio PXREF pxIconAudio, PXDrawInfo PXREF pxDrawInfo);

#endif