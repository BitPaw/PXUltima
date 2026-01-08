#pragma once


#ifndef PXSoundIncluded
#define PXSoundIncluded


#define PXSoundBehaviourLoop (1<<0)

#include <PX/Engine/PXResource.h>

typedef struct PXSound_
{
    PXECSInfo Info;

    void* BaseObject;
    void* Data;

    PXI32U SampleRate;
    PXI32U ByteRate;
    PXI16U NumerOfChannels;
    PXI16U BlockAllign;
    PXI16U BitsPerSample;

    PXSize DataSize;
    PXI32U ChunkSize;
    PXI16U AudioFormat;
}
PXSound;


typedef struct PXSoundCreateInfo_
{
    PXECSCreateInfo Info;
    PXBool SoundLoop;
}
PXSoundCreateInfo;

PXPublic PXResult PXAPI PXSoundCreate(PXSound PXREF pxSound, PXSoundCreateInfo PXREF pxSoundCreateInfo);

#endif