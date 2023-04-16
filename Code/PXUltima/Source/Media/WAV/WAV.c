#include "WAV.h"

#include <OS/Memory/PXMemory.h>
#include <Media/RIFF/RIFF.h>
#include <Math/PXMath.h>

#define WAVSignatureLIST { 'L', 'I', 'S', 'T' }
#define WAVSignatureData { 'd', 'a', 't', 'a' }

PXActionResult WAVParse(WAV* const wav, PXDataStream* const pxDataStream)
{
	RIFF riff;

	MemoryClear(wav, sizeof(WAV));

	// RIFF
	{
		const PXActionResult actionResult = RIFFParse(&riff, pxDataStream);

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
		const PXActionResult actionResult = FMTParse(&wav->Format, pxDataStream, riff.EndianFormat);

		PXActionExitOnError(actionResult);
	}
	//---------------------------------------

	//---------------------------------------
	{
		const char signature[] = WAVSignatureLIST;
		const PXSize signatureSize = sizeof(signature);
		const PXBool isRIFFListChunk = PXDataStreamReadAndCompare(pxDataStream, signature, signatureSize);

		if(isRIFFListChunk)
		{
			PXDataStreamCursorAdvance(pxDataStream, 30u);
		}
	}
	//---------------------------------------
	{
		const char signature[] = WAVSignatureData;
		const PXSize signatureSize = sizeof(signature);
		const PXBool validDataChunk = PXDataStreamReadAndCompare(pxDataStream, signature, signatureSize);

		if(!validDataChunk)
		{
			return PXActionFailedFormatNotAsExpected;
		}
	}

	PXDataStreamReadI32UE(pxDataStream, &wav->SoundDataSize, riff.EndianFormat);

	wav->SoundData = MemoryAllocate(sizeof(PXByte) * wav->SoundDataSize);

	PXDataStreamReadB(pxDataStream, wav->SoundData, wav->SoundDataSize);

	return PXActionSuccessful;
}

PXActionResult WAVSerialize(WAV* const wav, PXDataStream* const pxDataStream)
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
	PXDataStream audioFile;
	PXDataStreamMapToMemoryA(&audioFile, "C:/Users/Jona/Desktop/test.wav", dataSize + 44, MemoryWriteOnly);

	if (&audioFile == 0) return 0;
	*/

	Endian targetEndian = EndianLittle;

	// Write RIFF
	{
		RIFF riff;
		riff.EndianFormat = targetEndian;
		riff.ChunkSize = dataSize+36;
		riff.Format = RIFFWaveformAudio;

		const PXActionResult riffResult = RIFFSerialize(&riff, pxDataStream);
	}

	// Write Format chunk
	{
		const FMT fmt =
		{
			bitdepth, // ChunkSize;
			1, // AudioFormat;
			1, // NumerOfChannels;
			44100, // SampleRate;
			44100 * (bitdepth / 8), // ByteRate;
			bitdepth / 8, // BlockAllign;
			bitdepth// BitsPerSample;
		};

		const PXActionResult riffResult = FMTSerialize(&fmt, pxDataStream, targetEndian);
	}

	//Data chunk
	{
		const char data[] = WAVSignatureData;

		PXDataStreamWriteB(pxDataStream, data, 4u);
		PXDataStreamWriteI32U(pxDataStream, dataSize);
	}

	for (PXSize section = 0; section < (duration / 60) * bpm; section++)
	{
		for (PXSize i = 0; i < (44100 * 60) / bpm; i++)
		{
			float sample = (frequences[section] > 0) * 0.5f *  ((1 + MathSinus(1 + (3.7 / ((44100 * 60) / bpm) * i))) / 2) +  MathSinus(wave);
			unsigned int correctedsample = sample * maxAmp;
			PXDataStreamWriteB(pxDataStream, &correctedsample, bitdepth / 8);
			wave += 2 * 3.14159265358979323846 * frequences[section] * (100 / (((i >= ((44100 * 60) / bpm) * i) + 1))) / 44100;
		}
	}

	return PXActionSuccessful;
}