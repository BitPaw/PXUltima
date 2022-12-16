#include "MID.h"

#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>


#define MIDITrackHeaderID {'M','T','h','d'}
#define MIDITrackChunkID  {'M','T','r','k'}

/*
MIDICommand midiCommand;
unsigned int byteLength = -1;

switch (midiCommand)
{
	default:
	case MIDICommand::Unkown:
		byteLength = -1;
		break;

	case MIDICommand::ProgrammChange:
	case MIDICommand::ChannelAfterTouch:
		byteLength = 1;
		break;

	case MIDICommand::NoteOFF:
	case MIDICommand::NoteON:
	case MIDICommand::PolyphonicKeyAfterTouch:
	case MIDICommand::ControlChange:
	case MIDICommand::PitchBendChange:
	case MIDICommand::NoteEvent:
		byteLength = 2;
		break;
}*/

PXActionResult MIDParse(MID* mid, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXDataStream dataStream;

	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);

	// Pasre Chunk header
	{
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = MIDITrackHeaderID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const PXBool isValid = PXDataStreamReadAndCompare(&dataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		PXDataStreamReadI16UE(&dataStream, chunkLength, EndianBig);
		PXDataStreamReadI16UE(&dataStream, &mid->Format, EndianBig);
		PXDataStreamReadI16UE(&dataStream, &mid->TrackListSize, EndianBig);
		PXDataStreamReadI16UE(&dataStream, &mid->MusicSpeed, EndianBig);
	}

	if(!mid->TrackListSize)
	{
		return PXActionSuccessful;
	}

	mid->TrackList = (MIDITrack*)MemoryAllocate(sizeof(MIDITrack) * mid->TrackListSize);

	// Parse Track Header
	for(PXSize i = 0; i < mid->TrackListSize; ++i)
	{
		MIDITrack* track = &mid->TrackList[i];
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = MIDITrackChunkID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const PXBool isValid = PXDataStreamReadAndCompare(&dataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		PXDataStreamReadI32UE(&dataStream, chunkLength, EndianBig);

		track->ID = i;
		track->EventData = (PXByte*)MemoryAllocate(sizeof(PXByte) * chunkLength);
		track->EventDataSize = chunkLength;

		PXDataStreamReadB(&dataStream, track->EventData, chunkLength);
	}

	*dataRead = dataStream.DataCursor;

	return PXActionSuccessful;
}

PXActionResult MIDSerialize(MID* mid, void* data, const PXSize dataSize, PXSize* dataWritten)
{
	/*
	File file;

	{
		const PXActionResult fileOpenResult = file.Open(filePath, FileOpenMode::Write);
		const bool sucessful = fileOpenResult == PXActionSuccessful;

		if(!sucessful)
		{
			return fileOpenResult;
		}
	}

	const char midiTagData[] = MIDITrackHeaderID;

	file.WriteToDisk(midiTagData, 4u); // "MThd"
	file.WriteToDisk(6u, EndianBig);
	file.WriteToDisk(Format, EndianBig);
	file.WriteToDisk(TrackListSize, EndianBig);
	file.WriteToDisk(MusicSpeed, EndianBig);

	for(PXSize i = 0; i < TrackListSize; i++)
	{
		const char midiTrackTag[] = MIDITrackChunkID;

		MIDITrack& track = TrackList[i];

		file.WriteToDisk(midiTrackTag, 4u);
		file.WriteToDisk(track.EventDataSize, EndianBig);
		file.WriteToDisk(track.EventData, track.EventDataSize);
	}

	{
		const PXActionResult fileCloseResult = file.Close();
		const bool sucessful = fileCloseResult == PXActionSuccessful;

		if(!sucessful)
		{
			return fileCloseResult;
		}
	}

	return PXActionSuccessful;*/

	return PXActionSuccessful;
}
