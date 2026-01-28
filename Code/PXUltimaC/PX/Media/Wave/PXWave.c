#include "PXWave.h"

#include <PX/Media/RIFF/PXRIFF.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>

const static char WAVSignatureLIST[4] = "LIST";
const static char WAVSignatureData[4] = "data";


PXResult PXAPI PXWaveLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    /*
    PXSound PXREF pxSound = (PXSound*)pxResourceLoadInfo->Target;

    PXWave wav;;
    PXClear(PXWave, &wav);

    //pxSound->BaseObject = &wav;

    PXRIFF riff;

    // PXRIFF
    {
       const PXResult actionResult = PXRIFFLoadFromFile(&riff, pxResourceLoadInfo->FileCurrent);

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
       const PXResult actionResult = PXFMTLoadFromFile(&wav.Format, pxResourceLoadInfo->FileCurrent, riff.EndianFormat);

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
       const PXBool isPXRIFFListChunk = PXFileReadAndCompare(pxResourceLoadInfo->FileCurrent, WAVSignatureLIST, sizeof(WAVSignatureLIST));

       if (isPXRIFFListChunk)
       {
           PXFileCursorAdvance(pxResourceLoadInfo->FileCurrent, 30u);
       }
    }
    //---------------------------------------
    {
       const PXBool validDataChunk = PXFileReadAndCompare(pxResourceLoadInfo->FileCurrent, WAVSignatureData, sizeof(WAVSignatureData));

       if (!validDataChunk)
       {
           return PXActionFailedFormatNotAsExpected;
       }
    }

    PXFileReadI32UE(pxResourceLoadInfo->FileCurrent, &wav.SoundDataSize, riff.EndianFormat);

    PXNewList(PXByte, wav.SoundDataSize, &pxSound->Data, &pxSound->DataSize);

    PXFileReadB(pxResourceLoadInfo->FileCurrent, pxSound->Data, pxSound->DataSize);
    */

    return PXResultOK;
}


PXResult PXAPI PXWaveSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
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

        const PXResult riffResult = PXRIFFSaveToFile(&riff, pxResourceSaveInfo->FileReference);
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

        const PXResult fmtResult = PXFMTSaveToFile(&fmt, pxResourceSaveInfo->FileReference, targetEndian);
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

    return PXResultOK;
}
