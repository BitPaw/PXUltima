#include "PXMIDI.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/PXOS.h>

const static char PXMIDITrackHeaderID[4] = { 'M','T','h','d' };
const static char PXMIDITrackChunkID[4] = { 'M','T','r','k' };

PXResult PXAPI  PXMIDILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXMIDI* const pxMIDI = PXMemoryHeapCallocT(PXMIDI, 1);

    // Parse Chunk header
    {
        PXI16U chunkLength = 0;

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

    pxMIDI->TrackListSize = pxMIDI->TrackListAmount;
    pxMIDI->TrackList = PXMemoryHeapCallocT(PXMIDITrack, pxMIDI->TrackListAmount);

    // Parse Track Header
    for (PXI16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack* const track = &pxMIDI->TrackList[i];
        PXI32U chunkLength = 0;

        {
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));

            if (!isValid)
            {
                return PXActionFailedFormatNotAsExpected;
            }
        }

        PXFileReadI32UE(pxResourceLoadInfo->FileReference, &chunkLength, PXEndianBig);

        track->ID = i;
        track->EventDataSize = chunkLength;
        track->EventData = PXMemoryHeapCallocT(PXByte, chunkLength);

        PXFileReadB(pxResourceLoadInfo->FileReference, track->EventData, chunkLength);
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXMIDISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXMIDI* pxMIDI = PXNull;

    {
        const PXI16U chunkLength = 6;
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

    for (PXI16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack* const track = &pxMIDI->TrackList[i];

        PXFileWriteB(pxResourceSaveInfo->FileReference, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));
        PXFileWriteI32UE(pxResourceSaveInfo->FileReference, track->EventDataSize, PXEndianBig);
        PXFileWriteB(pxResourceSaveInfo->FileReference, track->EventData, track->EventDataSize);
    }

    return PXActionSuccessful;
}
