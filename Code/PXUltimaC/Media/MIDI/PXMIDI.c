#include "PXMIDI.h"

#include <OS/File/PXFile.h>

const static char PXMIDITrackHeaderID[4] = { 'M','T','h','d' };
const static char PXMIDITrackChunkID[4] = { 'M','T','r','k' };

PXActionResult PXAPI PXMIDILoadFromFile(PXSound* const pxSound, PXFile* const pxFile)
{
	PXMIDI* const pxMIDI = PXNew(PXMIDI);

	// Parse Chunk header
	{
		PXInt16U chunkLength = 0;

		{
			const PXBool isValid = PXFileReadAndCompare(pxFile, PXMIDITrackHeaderID, sizeof(PXMIDITrackHeaderID));

			if (!isValid)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{&chunkLength,PXDataTypeInt16UBE},
			{&pxMIDI->Format,PXDataTypeInt16UBE},
			{&pxMIDI->TrackListSize,PXDataTypeInt16UBE},
			{&pxMIDI->MusicSpeed,PXDataTypeInt16UBE}
		};

		PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
	}

	if (!pxMIDI->TrackListSize)
	{
		return PXActionSuccessful;
	}

	pxMIDI->TrackList = PXNewList(PXMIDITrack, pxMIDI->TrackListSize);

	// Parse Track Header
	for (PXInt16U i = 0; i < pxMIDI->TrackListSize; ++i)
	{
		PXMIDITrack* const track = &pxMIDI->TrackList[i];
		PXInt32U chunkLength = 0;

		{
			const PXBool isValid = PXFileReadAndCompare(pxFile, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));

			if (!isValid)
			{
				return PXActionFailedFormatNotAsExpected;
			}
		}

		PXFileReadI32UE(pxFile, &chunkLength, PXEndianBig);

		track->ID = i;
		track->EventData = PXNewList(PXByte, chunkLength);
		track->EventDataSize = chunkLength;

		PXFileReadB(pxFile, track->EventData, chunkLength);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDISaveToFile(PXSound* const pxSound, PXFile* const pxFile)
{
	PXMIDI* pxMIDI = PXNull;

	{
		const PXInt16U chunkLength = 6;
		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXMIDITrackHeaderID,PXDataTypeDatax4},
			{&chunkLength,PXDataTypeInt16UBE},
			{&pxMIDI->Format,PXDataTypeInt16UBE},
			{&pxMIDI->TrackListSize,PXDataTypeInt16UBE},
			{&pxMIDI->MusicSpeed,PXDataTypeInt16UBE}
		};

		PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
	}	

	for (PXInt16U i = 0; i < pxMIDI->TrackListSize; ++i)
	{
		PXMIDITrack* const track = &pxMIDI->TrackList[i];

		PXFileWriteB(pxFile, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));
		PXFileWriteI32UE(pxFile, track->EventDataSize, PXEndianBig);
		PXFileWriteB(pxFile, track->EventData, track->EventDataSize);
	}

	return PXActionSuccessful;
}