#include "PXMIDI.h"

#include <OS/File/PXFile.h>

const static char PXMIDITrackHeaderID[4] = { 'M','T','h','d' };
const static char PXMIDITrackChunkID[4] = { 'M','T','r','k' };

PXActionResult PXAPI PXMIDILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXMIDI* const pxMIDI = PXNull;
    PXNew(PXMIDI,&pxMIDI);

    // Parse Chunk header
    {
        PXInt16U chunkLength = 0;

        {
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXMIDITrackHeaderID, sizeof(PXMIDITrackHeaderID));

            if (!isValid)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&chunkLength,PXTypeInt16UBE},
            {&pxMIDI->Format,PXTypeInt16UBE},
            {&pxMIDI->TrackListAmount,PXTypeInt16UBE},
            {&pxMIDI->MusicSpeed,PXTypeInt16UBE}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    if (!pxMIDI->TrackListSize)
    {
        return PXActionSuccessful;
    }

    PXNewList(PXMIDITrack, pxMIDI->TrackListAmount, &pxMIDI->TrackList, &pxMIDI->TrackListSize);

    // Parse Track Header
    for (PXInt16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack* const track = &pxMIDI->TrackList[i];
        PXInt32U chunkLength = 0;

        {
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));

            if (!isValid)
            {
                return PXActionFailedFormatNotAsExpected;
            }
        }

        PXFileReadI32UE(pxResourceLoadInfo->FileReference, &chunkLength, PXEndianBig);

        track->ID = i;
        PXNewList(PXByte, chunkLength, &track->EventData, &track->EventDataSize);

        PXFileReadB(pxResourceLoadInfo->FileReference, track->EventData, chunkLength);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXMIDI* pxMIDI = PXNull;

    {
        const PXInt16U chunkLength = 6;
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {PXMIDITrackHeaderID,PXTypeDatax4},
            {&chunkLength,PXTypeInt16UBE},
            {&pxMIDI->Format,PXTypeInt16UBE},
            {&pxMIDI->TrackListSize,PXTypeInt16UBE},
            {&pxMIDI->MusicSpeed,PXTypeInt16UBE}
        };

        PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    for (PXInt16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack* const track = &pxMIDI->TrackList[i];

        PXFileWriteB(pxResourceSaveInfo->FileReference, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));
        PXFileWriteI32UE(pxResourceSaveInfo->FileReference, track->EventDataSize, PXEndianBig);
        PXFileWriteB(pxResourceSaveInfo->FileReference, track->EventData, track->EventDataSize);
    }

    return PXActionSuccessful;
}
