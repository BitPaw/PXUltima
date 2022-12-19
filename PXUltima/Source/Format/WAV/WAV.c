#include "WAV.h"

#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>
#include <File/Endian.h>
#include <Container/ClusterValue.h>
#include <Format/RIFF/RIFF.h>

#define WAVListMarker MakeInt('L', 'I', 'S', 'T')

PXActionResult WAVParse(WAV* wav, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXDataStream dataStream;
	Endian endian = EndianInvalid;

	MemoryClear(wav, sizeof(WAV));
	*dataRead = 0;

	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);

	// RIFF
	{
		RIFF riff;
		const unsigned char* riffHeaderStart = PXDataStreamCursorPosition(&dataStream);
		const PXSize maximalSize = PXDataStreamRemainingSize(&dataStream);
		PXSize parsedBytes = 0;

		const PXActionResult actionResult = RIFFParse(&riff, riffHeaderStart, maximalSize, &parsedBytes);

		if(!riff.Valid)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}

		PXDataStreamCursorAdvance(&dataStream, parsedBytes);

		endian = riff.EndianFormat;
	}
	//-------------------------------------------------------------------------

	//---<FMT Chunk>-----------------------------------------------------------
	{
		const PXActionResult actionResult = FMTParse(&wav->Format, &dataStream, endian);
		const PXBool sucessful = PXActionSuccessful == actionResult;

		if(!sucessful)
		{
			return PXActionFailedFormatNotAsExpected;
		}
	}
	//---------------------------------------

	//---------------------------------------
	{
		const unsigned int value = WAVListMarker;
		const unsigned char isRIFFListChunk = PXDataStreamReadAndCompare(&dataStream, &value, sizeof(unsigned int));

		if(isRIFFListChunk)
		{
			PXDataStreamCursorAdvance(&dataStream, 30u);
		}
	}
	//---------------------------------------
	{
		const unsigned int value = MakeInt('d', 'a', 't', 'a');
		const unsigned char validDataChunk = PXDataStreamReadAndCompare(&dataStream, &value, sizeof(unsigned int));

		if(!validDataChunk)
		{
			return PXActionFailedFormatNotAsExpected;
		}
	}

	PXDataStreamReadI32UE(&dataStream, wav->SoundDataSize, endian);

	wav->SoundData = MemoryAllocate(sizeof(unsigned char) * wav->SoundDataSize);

	PXDataStreamReadB(&dataStream, wav->SoundData, wav->SoundDataSize);

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
