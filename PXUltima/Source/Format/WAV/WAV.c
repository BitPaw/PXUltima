#include "WAV.h"

#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>
#include <File/Endian.h>
#include <Container/ClusterValue.h>
#include <Format/RIFF/RIFF.h>

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

/*

PXActionResult BF::WAV::Save(const wchar_t* filePath)
{
	// Note: The sample data must end on an even byte boundary. Whatever that means.
	File fileStream;

	const Endian endian = EndianLittle;
	unsigned int riffSize = 0;

	fileStream.Write("RIFF", 4);
	fileStream.Write(riffSize, endian);
	fileStream.Write("WAVE", 4);

	fileStream.Write("fmt ", 4u);
	fileStream.Write(Format.ChunkSize, endian);
	fileStream.Write(Format.AudioFormat, endian);
	fileStream.Write(Format.NumerOfChannels, endian);
	fileStream.Write(Format.SampleRate, endian);
	fileStream.Write(Format.ByteRate, endian);
	fileStream.Write(Format.BlockAllign, endian);
	fileStream.Write(Format.BitsPerSample, endian);

	// LIST?

	fileStream.Write("data", 4);
	fileStream.Write(SoundDataSize, endian);
	fileStream.Write(SoundData, SoundDataSize);

	fileStream.WriteToDisk(filePath);

	return PXActionSuccessful;
}

PXActionResult BF::WAV::ConvertTo(Sound& sound)
{
	sound.NumerOfChannels = Format.NumerOfChannels;
	sound.SampleRate = Format.SampleRate;
	sound.BitsPerSample = Format.BitsPerSample;
	sound.DataSize = SoundDataSize;
	sound.Data = Memory::Allocate<unsigned char>(SoundDataSize);

	MemoryCopy(SoundData, SoundDataSize, sound.Data, sound.DataSize);

	return PXActionSuccessful;
}

*/
