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

PXActionResult MIDParse(MID* mid, PXDataStream* const pxDataStream)
{
	// Parse Chunk header
	{
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = MIDITrackHeaderID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const PXBool isValid = PXDataStreamReadAndCompare(pxDataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		PXDataStreamReadI16UE(pxDataStream, chunkLength, EndianBig);
		PXDataStreamReadI16UE(pxDataStream, &mid->Format, EndianBig);
		PXDataStreamReadI16UE(pxDataStream, &mid->TrackListSize, EndianBig);
		PXDataStreamReadI16UE(pxDataStream, &mid->MusicSpeed, EndianBig);
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
			const PXBool isValid = PXDataStreamReadAndCompare(pxDataStream, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionFailedFormatNotAsExpected;
			}
		}

		PXDataStreamReadI32UE(pxDataStream, chunkLength, EndianBig);

		track->ID = i;
		track->EventData = (PXByte*)MemoryAllocate(sizeof(PXByte) * chunkLength);
		track->EventDataSize = chunkLength;

		PXDataStreamReadB(pxDataStream, track->EventData, chunkLength);
	}

	return PXActionSuccessful;
}

PXActionResult MIDSerialize(MID* mid, PXDataStream* const pxDataStream)
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
