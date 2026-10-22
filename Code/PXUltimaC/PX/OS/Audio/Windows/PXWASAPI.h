#pragma once

#ifndef PXWASAPIIncluded
#define PXWASAPIIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXAudioFilterBuffer.h>

#if OSWindows

PXPublic PXResult PXAPI PXWASAPIInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);

PXPublic PXResult PXAPI PXWASAPIDeviceListFetch(PXEngineAudio PXREF pxEngineAudio);

PXPublic PXResult PXAPI PXWASAPIUpload(PXEngineAudio PXREF pxAudio, PXBuffer PXREF pxBuffer);
PXPublic PXResult PXAPI PXWASAPIFilterApply
(
    PXEngineAudio PXREF pxAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioFilterFunction pxAudioFilterFunction,
    void PXREF filterData
);

PXPublic PXResult PXAPI PXWSAPISoundLoad(PXText PXREF pxText, PXBuffer PXREF pxBuffer);

#endif

#endif