#pragma once

#ifndef PXAudioFilterBufferIncluded
#define PXAudioFilterBufferIncluded

#include <PX/Media/PXType.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXAudioFilterBuffer_
{
    PXECSInfo Info;

    PXBuffer Buffer;

    PXSize Samples;
    PXSize SampleRate;
}
PXAudioFilterBuffer;

typedef void (PXAPI* PXAudioFilterFunction)(void* PXREF pxFilter, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

//typedef void (PXAPI* PXAudioFilterFunction)(float* in_buffer, float* out_buffer, size_t frames, size_t channels, void PXREF user_data);

#endif