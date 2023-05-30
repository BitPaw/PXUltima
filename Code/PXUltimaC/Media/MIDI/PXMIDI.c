#include "PXMIDI.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>


#define PXMIDITrackHeaderID {'M','T','h','d'}
#define PXMIDITrackChunkID  {'M','T','r','k'}

/*
PXMIDICommand midiCommand;
unsigned int byteLength = -1;

switch (midiCommand)
{
	default:
	case PXMIDICommand::Unkown:
		byteLength = -1;
		break;

	case PXMIDICommand::PXProgrammChange:
	case PXMIDICommand::ChannelAfterTouch:
		byteLength = 1;
		break;

	case PXMIDICommand::NoteOFF:
	case PXMIDICommand::NoteON:
	case PXMIDICommand::PolyphonicKeyAfterTouch:
	case PXMIDICommand::ControlChange:
	case PXMIDICommand::PitchBendChange:
	case PXMIDICommand::NoteEvent:
		byteLength = 2;
		break;
}*/

PXActionResult PXMIDIParse(PXMIDI* mid, PXFile* const PXFile)
{
	// Parse Chunk header
	{
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = PXMIDITrackHeaderID;
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

	mid->TrackList = (PXMIDITrack*)PXMemoryAllocate(sizeof(PXMIDITrack) * mid->TrackListSize);

	// Parse Track Header
	for(PXSize i = 0; i < mid->TrackListSize; ++i)
	{
		PXMIDITrack* track = &mid->TrackList[i];
		unsigned int chunkLength = 0;

		{
			const char headerSignature[] = PXMIDITrackChunkID;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const PXBool isValid = PXFileReadAndCompare(PXFile, headerSignature, headerSignatureSize);

			if(!isValid)
			{
				return PXActionFailedFormatNotAsExpected;
			}
		}

		PXFileReadI32UE(PXFile, chunkLength, PXEndianBig);

		track->ID = i;
		track->EventData = (PXByte*)PXMemoryAllocate(sizeof(PXByte) * chunkLength);
		track->EventDataSize = chunkLength;

		PXFileReadB(PXFile, track->EventData, chunkLength);
	}

	return PXActionSuccessful;
}

PXActionResult PXMIDISerialize(PXMIDI* mid, PXFile* const PXFile)
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

	const char midiTagData[] = PXMIDITrackHeaderID;

	file.WriteToDisk(midiTagData, 4u); // "MThd"
	file.WriteToDisk(6u, PXEndianBig);
	file.WriteToDisk(Format, PXEndianBig);
	file.WriteToDisk(TrackListSize, PXEndianBig);
	file.WriteToDisk(MusicSpeed, PXEndianBig);

	for(PXSize i = 0; i < TrackListSize; i++)
	{
		const char midiTrackTag[] = PXMIDITrackChunkID;

		PXMIDITrack& track = TrackList[i];

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
