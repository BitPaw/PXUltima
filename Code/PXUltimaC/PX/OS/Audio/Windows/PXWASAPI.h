#pragma once

#ifndef PXWASAPIIncluded
#define PXWASAPIIncluded

#include <PX/Engine/PXAudioSystem.h>

typedef struct PXWASAPI_ PXWASAPI;

PXPublic PXResult PXAPI PXWASAPIInitialize(PXAudioSystem PXREF pxAudioSystem, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo);

PXPublic PXResult PXAPI PXWASAPIDeviceListFetch(PXAudioSystem PXREF pxAudioSystem);

PXPublic PXResult PXAPI PXWASAPIUpload(PXAudioSystem PXREF pxAudio, PXBuffer PXREF pxBuffer);
PXPublic PXResult PXAPI PXWASAPIFilterApply
(
    PXAudioSystem PXREF pxAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioFilterFunction pxAudioFilterFunction,
    void PXREF filterData
);

PXPublic PXResult PXAPI PXWSAPISoundLoad(PXText PXREF pxText, PXBuffer PXREF pxBuffer);

#endif