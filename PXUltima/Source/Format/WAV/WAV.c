#include "WAV.h"

#include <File/DataStream.h>
#include <Memory/Memory.h>
#include <File/Endian.h>
#include <Container/ClusterValue.h>
#include <Format/RIFF/RIFF.h>

#define WAVListMarker MakeInt('L', 'I', 'S', 'T')

ActionResult WAVParse(WAV* wav, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;
	Endian endian = EndianInvalid;

	MemorySet(wav, sizeof(WAV), 0);
	*dataRead = 0;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);

	// RIFF
	{
		RIFF riff;
		const unsigned char* riffHeaderStart = DataStreamCursorPosition(&dataStream);
		const size_t maximalSize = DataStreamRemainingSize(&dataStream);
		size_t parsedBytes = 0;

		const ActionResult actionResult = RIFFParse(&riff, riffHeaderStart, maximalSize, &parsedBytes);

		if(!riff.Valid)
		{
			return ActionInvalidHeaderSignature;
		}

		DataStreamCursorAdvance(&dataStream, parsedBytes);

		endian = riff.EndianFormat;
	}
	//-------------------------------------------------------------------------

	//---<FMT Chunk>-----------------------------------------------------------
	{
		const unsigned char* fmtHeaderStart = DataStreamCursorPosition(&dataStream);
		const size_t maximalSize = DataStreamRemainingSize(&dataStream);
		size_t parsedBytes = 0;
	
		const ActionResult actionResult = FMTParse(&wav->Format, fmtHeaderStart, maximalSize, &parsedBytes, endian);
		const unsigned char sucessful = actionResult == ActionSuccessful;

		if(!sucessful)
		{
			return ResultFormatNotAsExpected;
		}

		DataStreamCursorAdvance(&dataStream, parsedBytes);
	}
	//---------------------------------------

	//---------------------------------------	
	{
		const unsigned int value = WAVListMarker;
		const unsigned char isRIFFListChunk = DataStreamReadAndCompare(&dataStream, &value, sizeof(unsigned int));

		if(isRIFFListChunk)
		{
			DataStreamCursorAdvance(&dataStream, 30u);
		}
	}
	//---------------------------------------
	{
		const unsigned int value = MakeInt('d', 'a', 't', 'a');
		const unsigned char validDataChunk = DataStreamReadAndCompare(&dataStream, &value, sizeof(unsigned int));

		if(!validDataChunk)
		{
			return ResultFormatNotAsExpected;
		}
	}

	DataStreamReadIU(&dataStream, wav->SoundDataSize, endian);

	wav->SoundData = MemoryAllocate(sizeof(unsigned char) * wav->SoundDataSize);

	DataStreamReadD(&dataStream, wav->SoundData, wav->SoundDataSize);

	return ActionSuccessful;
}

/*

ActionResult BF::WAV::Save(const wchar_t* filePath)
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

	return ActionSuccessful;
}

ActionResult BF::WAV::ConvertTo(Sound& sound)
{
	sound.NumerOfChannels = Format.NumerOfChannels;
	sound.SampleRate = Format.SampleRate;
	sound.BitsPerSample = Format.BitsPerSample;
	sound.DataSize = SoundDataSize;
	sound.Data = Memory::Allocate<unsigned char>(SoundDataSize);

	MemoryCopy(SoundData, SoundDataSize, sound.Data, sound.DataSize);

	return ActionSuccessful;
}

*/