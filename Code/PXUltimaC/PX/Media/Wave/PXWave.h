#ifndef PXWAVINCLUDE
#define PXWAVINCLUDE

#include "../PXResource.h"

#include <PX/Media/FMT/PXFMT.h>

// Waveform Audio File
typedef struct PXWave_
{
    PXFMT Format;

    PXInt32U SoundDataSize; // 32-Bit
    void* SoundData;
}
PXWave;

#pragma optimize("", off)
PXPublic PXActionResult PXAPI PXWaveLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);

#pragma optimize("", off)
PXPublic PXActionResult PXAPI PXWaveSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
