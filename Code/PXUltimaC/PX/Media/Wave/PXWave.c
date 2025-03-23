#include "PXWave.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>

const static char WAVSignatureLIST[4] = { 'L', 'I', 'S', 'T' };
const static char WAVSignatureData[4] = { 'd', 'a', 't', 'a' };

#pragma optimize("", off)
PXActionResult PXAPI PXWaveLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    /*
    PXSound* const pxSound = (PXSound*)pxResourceLoadInfo->Target;

    PXWave wav;;
    PXClear(PXWave, &wav);

    //pxSound->BaseObject = &wav;

    PXRIFF riff;

    // PXRIFF
    {
       const PXActionResult actionResult = PXRIFFLoadFromFile(&riff, pxResourceLoadInfo->FileReference);

       PXActionReturnOnError(actionResult);

       // Valid PXRIFF
       {
           const PXBool isAVI = riff.Format == PXRIFFWaveformAudio;

           if (!isAVI)
           {
               return PXActionRefusedInvalidHeaderSignature;
           }
       }
    }
    //-------------------------------------------------------------------------

    //---<FMT Chunk>-----------------------------------------------------------
    {
       const PXActionResult actionResult = PXFMTLoadFromFile(&wav.Format, pxResourceLoadInfo->FileReference, riff.EndianFormat);

       PXActionReturnOnError(actionResult);

       pxSound->ChunkSize = wav.Format.ChunkSize;
       pxSound->AudioFormat = wav.Format.AudioFormat;
       pxSound->NumerOfChannels = wav.Format.NumerOfChannels;
       pxSound->SampleRate = wav.Format.SampleRate;
       pxSound->ByteRate = wav.Format.ByteRate;
       pxSound->BlockAllign = wav.Format.BlockAllign;
       pxSound->BitsPerSample = wav.Format.BitsPerSample;

       //pxSound->BlockAllign = (wav->Format.NumerOfChannels * wav->Format.BitsPerSample) / 8u;
    }
    //---------------------------------------

    //---------------------------------------
    {
       const PXBool isPXRIFFListChunk = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, WAVSignatureLIST, sizeof(WAVSignatureLIST));

       if (isPXRIFFListChunk)
       {
           PXFileCursorAdvance(pxResourceLoadInfo->FileReference, 30u);
       }
    }
    //---------------------------------------
    {
       const PXBool validDataChunk = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, WAVSignatureData, sizeof(WAVSignatureData));

       if (!validDataChunk)
       {
           return PXActionFailedFormatNotAsExpected;
       }
    }

    PXFileReadI32UE(pxResourceLoadInfo->FileReference, &wav.SoundDataSize, riff.EndianFormat);

    PXNewList(PXByte, wav.SoundDataSize, &pxSound->Data, &pxSound->DataSize);

    PXFileReadB(pxResourceLoadInfo->FileReference, pxSound->Data, pxSound->DataSize);
    */

    return PXActionSuccessful;
}

#pragma optimize("", off)
PXActionResult PXAPI PXWaveSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
#if 0
    unsigned int bitdepth = 16, bpm = 120;
    PXF32 wave = 0, duration = 12;
    PXF32 frequences[120] = { 523.251,/* 523.251, 523.251, 523.251, 523.251,*/ 0,/* 0, 0, 0, 0,*/ 554.365,/* 554.365, 554.365, 554.365, 554.365,*/
                              0,/* 0, 0, 0, 0,*/ 587.330, /*587.330, 587.330, 587.330, 587.330,*/ 0,/* 0, 0, 0, 0,*/ 622.254,/* 622.254, 622.254, 622.254, 622.254,*/
                              0,/* 0, 0, 0, 0,*/ 659.255, /*659.255, 659.255, 659.255, 659.255,*/ 0, /*0, 0, 0, 0,*/ 698.456, /*698.456, 698.456, 698.456, 698.456,*/
                              0, /*0, 0, 0, 0,*/ 739.989, /*739.989, 739.989, 739.989, 739.989,*/ 0, /*0, 0, 0, 0,*/ 783.991, /*783.991, 783.991, 783.991, 783.991,*/
                              0, /*0, 0, 0, 0,*/ 830.609, /*830.609, 830.609, 830.609, 830.609,*/ 0, /*0, 0, 0, 0,*/ 880, /*880, 880, 880, 880,*/ 0, /*0, 0, 0, 0,*/
                              932.328, /*932.328, 932.328, 932.328, 932.328,*/ 0, /*0, 0, 0, 0,*/ 987.767, /*987.767, 987.767, 987.767, 987.767*/
                            };
    unsigned int maxAmp = 1;

    for (PXSize i = 0; i < bitdepth - 1; i++)
    {
        maxAmp = maxAmp << 1;
    }

    maxAmp--;
    int dataSize = (int)((duration * 44100) * (bitdepth / 8));

    /*
    PXFile audioFile;
    PXFileMapToMemoryA(&audioFile, "C:/Users/Jona/Desktop/test.wav", dataSize + 44, PXAccessModeWriteOnly);

    if (&audioFile == 0) return 0;
    */

    PXEndian targetEndian = PXEndianLittle;

    // Write PXRIFF
    {
        PXRIFF riff;
        riff.EndianFormat = targetEndian;
        riff.ChunkSize = dataSize + 36;
        riff.Format = PXRIFFWaveformAudio;

        const PXActionResult riffResult = PXRIFFSaveToFile(&riff, pxResourceSaveInfo->FileReference);
    }

    // Write Format chunk
    {
        const PXFMT fmt =
        {
            bitdepth, // ChunkSize;
            1, // AudioFormat;
            1, // NumerOfChannels;
            44100, // SampleRate;
            44100 * (bitdepth / 8), // ByteRate;
            bitdepth / 8, // BlockAllign;
            bitdepth// BitsPerSample;
        };

        const PXActionResult fmtResult = PXFMTSaveToFile(&fmt, pxResourceSaveInfo->FileReference, targetEndian);
    }

    //Data chunk
    {
        PXFileWriteB(pxResourceSaveInfo->FileReference, WAVSignatureData, sizeof(WAVSignatureData));
        PXFileWriteI32U(pxResourceSaveInfo->FileReference, dataSize);
    }


    for (PXSize section = 0; section < (duration / 60) * bpm; section++)
    {
        for (PXSize i = 0; i < (44100 * 60) / bpm; i++)
        {
            PXF32 sample = (frequences[section] > 0) * 0.5f * ((1 + PXMathSinus(1 + (3.7 / ((44100 * 60) / bpm) * i))) / 2) + PXMathSinus(wave);
            unsigned int correctedsample = sample * maxAmp;
            PXFileWriteB(pxResourceSaveInfo->FileReference, &correctedsample, bitdepth / 8);
            wave += 2 * PXMathConstantPI * frequences[section] * (100 / (((i >= ((44100 * 60) / bpm) * i) + 1))) / 44100;
        }
    }
#endif

    return PXActionSuccessful;
}
