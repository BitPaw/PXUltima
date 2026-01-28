#include "PXZIP.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXZIPText[] = "ZIP";
const char PXZIPSignature[4] = { 'P', 'K', 0x03, 0x04 };
const char PXZIPChunk[4] = { 'P', 'K', 0x05, 0x06 };

PXResult PXAPI PXZIPLoadFromFile(PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXZIP pxZIPAA;
    PXClear(PXZIP, &pxZIPAA);
    PXZIP* pxZIP = &pxZIPAA;

    PXFile* pxFile = pxECSCreateInfo->FileCurrent;

    const PXBool isSignatureValid = PXFileReadAndCompare(pxFile, PXZIPSignature, sizeof(PXZIPSignature));

    if(!isSignatureValid)
    {
        return PXActionRefusedInvalidHeaderSignature;
    }

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {&pxZIP->VersionMinimum, PXTypeI16ULE},
        {&pxZIP->GeneralPurposeBitFlag, PXTypeI16ULE},
        {&pxZIP->CompressionMethod, PXTypeI16ULE},
        {&pxZIP->FileLastmodificationtime, PXTypeI16ULE},
        {&pxZIP->FileLastmodificationdate, PXTypeI16ULE},
        {&pxZIP->CRC32ofuncompressedData, PXTypeI32ULE},
        {&pxZIP->CompressedSize, PXTypeI32ULE},
        {&pxZIP->UncompressedSize, PXTypeI32ULE},
        {&pxZIP->Filenamelength, PXTypeI16ULE},
        {&pxZIP->Extrafieldlength, PXTypeI16ULE}
    };

    const PXSize amount = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    const PXBool validSize = 26 == amount;

    // Name
    pxZIP->FileName = (char*)PXFileDataAtCursor(pxFile);
    PXFileCursorAdvance(pxFile, pxZIP->Filenamelength);

    // Extra field
    pxZIP->ExtraField = (char*)PXFileDataAtCursor(pxFile);
    PXFileCursorAdvance(pxFile, pxZIP->Extrafieldlength);

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

PXResult PXAPI PXZIPSaveToFile(PXECSCreateInfo PXREF PXECSCreateInfo)
{
    return PXResultOK;
}