#include "PXZIP.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

const char PXZIPSignature[4] = { 'P', 'K', 0x03, 0x04 };
const char PXZIPChunk[4] = { 'P', 'K', 0x05, 0x06 };

PXActionResult PXAPI PXZIPLoadFromFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    PXZIP pxZIPAA;
    PXClear(PXZIP, &pxZIPAA);
    PXZIP* pxZIP = &pxZIPAA;


    const PXBool isSignatureValid = PXFileReadAndCompare(pxResourceTransphereInfo->FileReference, PXZIPSignature, sizeof(PXZIPSignature));

    if(!isSignatureValid)
    {
        return PXActionRefusedInvalidHeaderSignature;
    }

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {&pxZIP->VersionMinimum, PXTypeInt16ULE},
        {&pxZIP->GeneralPurposeBitFlag, PXTypeInt16ULE},
        {&pxZIP->CompressionMethod, PXTypeInt16ULE},
        {&pxZIP->FileLastmodificationtime, PXTypeInt16ULE},
        {&pxZIP->FileLastmodificationdate, PXTypeInt16ULE},
        {&pxZIP->CRC32ofuncompressedData, PXTypeInt32ULE},
        {&pxZIP->CompressedSize, PXTypeInt32ULE},
        {&pxZIP->UncompressedSize, PXTypeInt32ULE},
        {&pxZIP->Filenamelength, PXTypeInt16ULE},
        {&pxZIP->Extrafieldlength, PXTypeInt16ULE}
    };

    const PXSize amount = PXFileReadMultible(pxResourceTransphereInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    const PXBool validSize = 26 == amount;

    // Name
    pxZIP->FileName = PXFileCursorPosition(pxResourceTransphereInfo->FileReference);
    PXFileCursorAdvance(pxResourceTransphereInfo->FileReference, pxZIP->Filenamelength);

    // Extra field
    pxZIP->ExtraField = PXFileCursorPosition(pxResourceTransphereInfo->FileReference);
    PXFileCursorAdvance(pxResourceTransphereInfo->FileReference, pxZIP->Extrafieldlength);

#if PXLogEnable
    char nameBuffer[64];
    PXTextCopyA(pxZIP->FileName, pxZIP->Filenamelength, nameBuffer, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        "ZIP",
        "Parsing",
        "Version:%i, Name:%s",
        pxZIP->VersionMinimum,
        nameBuffer
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXZIPSaveToFile(PXResourceTransphereInfo* const pxResourceTransphereInfo)
{
    return PXActionSuccessful;
}