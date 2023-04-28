#include "MID.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>


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

PXActionResult MIDParse(MID* mid, PXFile* const PXFile)
{
	// Parse Chunk header
	{
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = MIDITrackHeaderID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const PXBool isValid = PXFileReadAndCompare(PXFile, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		PXFileReadI16UE(PXFile, chunkLength, PXEndianBig);
		PXFileReadI16UE(PXFile, &mid->Format, PXEndianBig);
		PXFileReadI16UE(PXFile, &mid->TrackListSize, PXEndianBig);
		PXFileReadI16UE(PXFile, &mid->MusicSpeed, PXEndianBig);
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
			const PXBool isValid = PXFileReadAndCompare(PXFile, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionFailedFormatNotAsExpected;
			}
		}

		PXFileReadI32UE(PXFile, chunkLength, PXEndianBig);

		track->ID = i;
		track->EventData = (PXByte*)MemoryAllocate(sizeof(PXByte) * chunkLength);
		track->EventDataSize = chunkLength;

		PXFileReadB(PXFile, track->EventData, chunkLength);
	}

	return PXActionSuccessful;
}

PXActionResult MIDSerialize(MID* mid, PXFile* const PXFile)
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
	file.WriteToDisk(6u, PXEndianBig);
	file.WriteToDisk(Format, PXEndianBig);
	file.WriteToDisk(TrackListSize, PXEndianBig);
	file.WriteToDisk(MusicSpeed, PXEndianBig);

	for(PXSize i = 0; i < TrackListSize; i++)
	{
		const char midiTrackTag[] = MIDITrackChunkID;

		MIDITrack& track = TrackList[i];

		file.WriteToDisk(midiTrackTag, 4u);
		file.WriteToDisk(track.EventDataSize, PXEndianBig);
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
