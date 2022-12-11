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

ActionResult MIDParse(MID* mid, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXDataStream dataStream;

	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);

	// Pasre Chunk header
	{
		unsigned int chunkLength = 0;

		{
			const unsigned char headerSignature[] = MIDITrackHeaderID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const unsigned char isValid = PXDataStreamReadAndCompare(&dataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return ActionInvalidHeaderSignature;
			}
		}

		PXDataStreamReadI16U(&dataStream, chunkLength, EndianBig);
		PXDataStreamReadI16U(&dataStream, &mid->Format, EndianBig);
		PXDataStreamReadI16U(&dataStream, &mid->TrackListSize, EndianBig);
		PXDataStreamReadI16U(&dataStream, &mid->MusicSpeed, EndianBig);
	}

	if(!mid->TrackListSize)
	{
		return ActionSuccessful;
	}

	mid->TrackList = MemoryAllocate(sizeof(MIDITrack) * mid->TrackListSize);

	// Parse Track Header
	for(PXSize i = 0; i < mid->TrackListSize; ++i)
	{
		MIDITrack* track = &mid->TrackList[i];
		unsigned int chunkLength = 0;

		{
			const unsigned char headerSignature[] = MIDITrackChunkID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const unsigned char isValid = PXDataStreamReadAndCompare(&dataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return ActionInvalidHeaderSignature;
			}
		}

		PXDataStreamReadI32U(&dataStream, chunkLength, EndianBig);

		track->ID = i;
		track->EventData = MemoryAllocate(sizeof(unsigned char) * chunkLength);
		track->EventDataSize = chunkLength;

		PXDataStreamReadB(&dataStream, track->EventData, chunkLength);
	}

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}

ActionResult MIDSerialize(MID* mid, void* data, const PXSize dataSize, PXSize* dataWritten)
{
	/*
	File file;

	{
		const ActionResult fileOpenResult = file.Open(filePath, FileOpenMode::Write);
		const bool sucessful = fileOpenResult == ActionSuccessful;

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
		const ActionResult fileCloseResult = file.Close();
		const bool sucessful = fileCloseResult == ActionSuccessful;

		if(!sucessful)
		{
			return fileCloseResult;
		}
	}

	return ActionSuccessful;*/

	return ActionSuccessful;
}