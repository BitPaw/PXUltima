#include "PXMIDI.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/PXOS.h>

const static char PXMIDITrackHeaderID[4] = { 'M','T','h','d' };
const static char PXMIDITrackChunkID[4] = { 'M','T','r','k' };

PXResult PXAPI PXMIDILoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXMIDI PXREF pxMIDI = PXMemoryHeapCallocT(PXMIDI, 1);

    // Parse Chunk header
    {
        PXI16U chunkLength = 0;

        {
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileCurrent, PXMIDITrackHeaderID, sizeof(PXMIDITrackHeaderID));

            if (!isValid)
            {
                return PXActionRefusedInvalidHeaderSignature;
            }
        }

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&chunkLength,PXTypeI16UBE},
            {&pxMIDI->Format,PXTypeI16UBE},
            {&pxMIDI->TrackListAmount,PXTypeI16UBE},
            {&pxMIDI->MusicSpeed,PXTypeI16UBE}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    if (!pxMIDI->TrackListSize)
    {
        return PXResultOK;
    }

    pxMIDI->TrackListSize = pxMIDI->TrackListAmount;
    pxMIDI->TrackList = PXMemoryHeapCallocT(PXMIDITrack, pxMIDI->TrackListAmount);

    // Parse Track Header
    for (PXI16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack PXREF track = &pxMIDI->TrackList[i];
        PXI32U chunkLength = 0;

        {
            const PXBool isValid = PXFileReadAndCompare(pxResourceLoadInfo->FileCurrent, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));

            if (!isValid)
            {
                return PXActionFailedFormatNotAsExpected;
            }
        }

        PXFileReadI32UE(pxResourceLoadInfo->FileCurrent, &chunkLength, PXEndianBig);

        track->ID = i;
        track->EventDataSize = chunkLength;
        track->EventData = PXMemoryHeapCallocT(PXByte, chunkLength);

        PXFileReadB(pxResourceLoadInfo->FileCurrent, track->EventData, chunkLength);
    }

    return PXResultOK;
}

PXResult PXAPI PXMIDISaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    PXMIDI* pxMIDI = PXNull;

    PXFile* pxFile = pxResourceSaveInfo->FileCurrent;

    {
        const PXI16U chunkLength = 6;
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {PXMIDITrackHeaderID,PXTypeDatax4},
            {&chunkLength,PXTypeI16UBE},
            {&pxMIDI->Format,PXTypeI16UBE},
            {&pxMIDI->TrackListSize,PXTypeI16UBE},
            {&pxMIDI->MusicSpeed,PXTypeI16UBE}
        };

        PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    for (PXI16U i = 0; i < pxMIDI->TrackListSize; ++i)
    {
        PXMIDITrack PXREF track = &pxMIDI->TrackList[i];

        PXFileWriteB(pxFile, PXMIDITrackChunkID, sizeof(PXMIDITrackChunkID));
        PXFileWriteI32UE(pxFile, track->EventDataSize, PXEndianBig);
        PXFileWriteB(pxFile, track->EventData, track->EventDataSize);
    }

    return PXResultOK;
}
