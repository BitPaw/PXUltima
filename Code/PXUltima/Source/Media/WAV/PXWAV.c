#include "PXWAV.h"

#include <OS/Memory/PXMemory.h>
#include <Media/RIFF/RIFF.h>
#include <Math/PXMath.h>

#define WAVSignatureLIST { 'L', 'I', 'S', 'T' }
#define WAVSignatureData { 'd', 'a', 't', 'a' }

PXActionResult PXWAVParse(PXWAV* const wav, PXFile* const PXFile)
{
	RIFF riff;

	MemoryClear(wav, sizeof(PXWAV));

	// RIFF
	{
		const PXActionResult actionResult = RIFFParse(&riff, PXFile);

		PXActionExitOnError(actionResult);

		// Valid RIFF
		{
			const PXBool isAVI = riff.Format == RIFFWaveformAudio;

			if (!isAVI)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}
	}
	//-------------------------------------------------------------------------

	//---<FMT Chunk>-----------------------------------------------------------
	{
		const PXActionResult actionResult = PXFMTParse(&wav->Format, PXFile, riff.EndianFormat);

		PXActionExitOnError(actionResult);
	}
	//---------------------------------------

	//---------------------------------------
	{
		const char signature[] = WAVSignatureLIST;
		const PXSize signatureSize = sizeof(signature);
		const PXBool isRIFFListChunk = PXFileReadAndCompare(PXFile, signature, signatureSize);

		if(isRIFFListChunk)
		{
			PXFileCursorAdvance(PXFile, 30u);
		}
	}
	//---------------------------------------
	{
		const char signature[] = WAVSignatureData;
		const PXSize signatureSize = sizeof(signature);
		const PXBool validDataChunk = PXFileReadAndCompare(PXFile, signature, signatureSize);

		if(!validDataChunk)
		{
			return PXActionFailedFormatNotAsExpected;
		}
	}

	PXFileReadI32UE(PXFile, &wav->SoundDataSize, riff.EndianFormat);

	wav->SoundData = MemoryAllocate(sizeof(PXByte) * wav->SoundDataSize);

	PXFileReadB(PXFile, wav->SoundData, wav->SoundDataSize);

	return PXActionSuccessful;
}

PXActionResult PXWAVSerialize(PXWAV* const wav, PXFile* const PXFile)
{
	unsigned int bitdepth = 16, bpm = 120;
	float wave = 0, duration = 12;
	float frequences[120] = { 523.251,/* 523.251, 523.251, 523.251, 523.251,*/ 0,/* 0, 0, 0, 0,*/ 554.365,/* 554.365, 554.365, 554.365, 554.365,*/
		0,/* 0, 0, 0, 0,*/ 587.330, /*587.330, 587.330, 587.330, 587.330,*/ 0,/* 0, 0, 0, 0,*/ 622.254,/* 622.254, 622.254, 622.254, 622.254,*/
		0,/* 0, 0, 0, 0,*/ 659.255, /*659.255, 659.255, 659.255, 659.255,*/ 0, /*0, 0, 0, 0,*/ 698.456, /*698.456, 698.456, 698.456, 698.456,*/
		0, /*0, 0, 0, 0,*/ 739.989, /*739.989, 739.989, 739.989, 739.989,*/ 0, /*0, 0, 0, 0,*/ 783.991, /*783.991, 783.991, 783.991, 783.991,*/
		0, /*0, 0, 0, 0,*/ 830.609, /*830.609, 830.609, 830.609, 830.609,*/ 0, /*0, 0, 0, 0,*/ 880, /*880, 880, 880, 880,*/ 0, /*0, 0, 0, 0,*/
		932.328, /*932.328, 932.328, 932.328, 932.328,*/ 0, /*0, 0, 0, 0,*/ 987.767, /*987.767, 987.767, 987.767, 987.767*/ };
	unsigned int maxAmp = 1;

	for (PXSize i = 0; i < bitdepth - 1; i++)
	{
		maxAmp = maxAmp << 1;
	}

	maxAmp--;
	int dataSize = (int)((duration * 44100) * (bitdepth / 8));

	/*
	PXFile audioFile;
	PXFileMapToMemoryA(&audioFile, "C:/Users/Jona/Desktop/test.wav", dataSize + 44, PXMemoryAccessModeWriteOnly);

	if (&audioFile == 0) return 0;
	*/

	PXEndian targetEndian = PXEndianLittle;

	// Write RIFF
	{
		RIFF riff;
		riff.EndianFormat = targetEndian;
		riff.ChunkSize = dataSize+36;
		riff.Format = RIFFWaveformAudio;

		const PXActionResult riffResult = RIFFSerialize(&riff, PXFile);
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

		const PXActionResult riffResult = FMTSerialize(&fmt, PXFile, targetEndian);
	}

	//Data chunk
	{
		const char data[] = WAVSignatureData;

		PXFileWriteB(PXFile, data, 4u);
		PXFileWriteI32U(PXFile, dataSize);
	}

	for (PXSize section = 0; section < (duration / 60) * bpm; section++)
	{
		for (PXSize i = 0; i < (44100 * 60) / bpm; i++)
		{
			float sample = (frequences[section] > 0) * 0.5f *  ((1 + PXMathSinus(1 + (3.7 / ((44100 * 60) / bpm) * i))) / 2) +  PXMathSinus(wave);
			unsigned int correctedsample = sample * maxAmp;
			PXFileWriteB(PXFile, &correctedsample, bitdepth / 8);
			wave += 2 * 3.14159265358979323846 * frequences[section] * (100 / (((i >= ((44100 * 60) / bpm) * i) + 1))) / 44100;
		}
	}

	return PXActionSuccessful;
}