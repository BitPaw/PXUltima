#include "PXZIP.h"

#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>

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

    const PXFileDataElementType pxDataStreamElementList[] =
    {
        {&pxZIP->VersionMinimum, PXDataTypeInt16ULE},
        {&pxZIP->GeneralPurposeBitFlag, PXDataTypeInt16ULE},
        {&pxZIP->CompressionMethod, PXDataTypeInt16ULE},
        {&pxZIP->FileLastmodificationtime, PXDataTypeInt16ULE},
        {&pxZIP->FileLastmodificationdate, PXDataTypeInt16ULE},
        {&pxZIP->CRC32ofuncompressedData, PXDataTypeInt32ULE},
        {&pxZIP->CompressedSize, PXDataTypeInt32ULE},
        {&pxZIP->UncompressedSize, PXDataTypeInt32ULE},
        {&pxZIP->Filenamelength, PXDataTypeInt16ULE},
        {&pxZIP->Extrafieldlength, PXDataTypeInt16ULE}
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