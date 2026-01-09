#include "PXZIP.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXZIPText[] = "ZIP";
const char PXZIPSignature[4] = { 'P', 'K', 0x03, 0x04 };
const char PXZIPChunk[4] = { 'P', 'K', 0x05, 0x06 };

PXResult PXAPI PXZIPLoadFromFile(PXResourceMoveInfo PXREF PXResourceMoveInfo)
{
    PXZIP pxZIPAA;
    PXClear(PXZIP, &pxZIPAA);
    PXZIP* pxZIP = &pxZIPAA;


    const PXBool isSignatureValid = PXFileReadAndCompare(PXResourceMoveInfo->FileReference, PXZIPSignature, sizeof(PXZIPSignature));

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

    const PXSize amount = PXFileReadMultible(PXResourceMoveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    const PXBool validSize = 26 == amount;

    // Name
    pxZIP->FileName = (char*)PXFileDataAtCursor(PXResourceMoveInfo->FileReference);
    PXFileCursorAdvance(PXResourceMoveInfo->FileReference, pxZIP->Filenamelength);

    // Extra field
    pxZIP->ExtraField = (char*)PXFileDataAtCursor(PXResourceMoveInfo->FileReference);
    PXFileCursorAdvance(PXResourceMoveInfo->FileReference, pxZIP->Extrafieldlength);

#if PXLogEnable
    char nameBuffer[64];
    PXTextCopyA(pxZIP->FileName, pxZIP->Filenamelength, nameBuffer, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        PXZIPText,
        "Parsing",
        "Version:%i, Name:%s",
        pxZIP->VersionMinimum,
        nameBuffer
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXZIPSaveToFile(PXResourceMoveInfo PXREF PXResourceMoveInfo)
{
    return PXResultOK;
}