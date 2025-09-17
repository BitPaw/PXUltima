#ifndef PXWAVIncluded
#define PXWAVIncluded

#include <PX/Engine/PXResource.h>

#include <PX/Media/FMT/PXFMT.h>

// Waveform Audio File
typedef struct PXWave_
{
    PXFMT Format;

    PXI32U SoundDataSize; // 32-Bit
    void* SoundData;
}
PXWave;


PXPublic PXResult PXAPI PXWaveLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);


PXPublic PXResult PXAPI PXWaveSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
