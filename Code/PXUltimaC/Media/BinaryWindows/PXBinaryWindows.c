#include "PXBinaryWindows.h"

#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXPEHeaderSignatore[4] = { 'P', 'E', '\0', '\0' };

#define PXBinaryWindowsDebug 1

PXActionResult PXAPI PXBinaryWindowsLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXBinaryWindows* const pxBinaryWindows = (PXBinaryWindows*)pxResourceLoadInfo->ResourceTarget;

#if PXBinaryWindowsDebug
    PXLogPrint
    (
        PXLoggingInfo,
        "BinaryWindows",
        "Parsing",
        "Start"
    );
#endif

    // Read header
    {
        PXDOSHeader* const pxDOSHeader = &pxBinaryWindows->Header;

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {pxDOSHeader->Magic.Data,PXDataTypeDatax2 },
            {&pxDOSHeader->LastPageSize, PXDataTypeInt16ULE},
            {&pxDOSHeader->TotalPagesInFile, PXDataTypeInt16ULE},
            {&pxDOSHeader->ReallocationItems, PXDataTypeInt16ULE},
            {&pxDOSHeader->ParagraphsInHeader, PXDataTypeInt16ULE},
            {&pxDOSHeader->ParagraphsExtraMinimum, PXDataTypeInt16ULE},
            {&pxDOSHeader->ParagraphsExtraMaximum, PXDataTypeInt16ULE},
            {&pxDOSHeader->StackInitialSegment, PXDataTypeInt16ULE},
            {&pxDOSHeader->StackInitialPointer, PXDataTypeInt16ULE},
            {&pxDOSHeader->ChecksumCompliment, PXDataTypeInt16ULE},
            {&pxDOSHeader->InstructionPointerInternal, PXDataTypeInt16ULE},
            {&pxDOSHeader->CodeSegmentInitial, PXDataTypeInt16ULE},
            {&pxDOSHeader->e_lfarlc, PXDataTypeInt16ULE},
            {&pxDOSHeader->e_ovno, PXDataTypeInt16ULE},
            {PXNull, PXDataTypePadding(8)},
            {&pxDOSHeader->e_oemid, PXDataTypeInt16ULE},
            {&pxDOSHeader->e_oeminfo, PXDataTypeInt16ULE},
            {PXNull, PXDataTypePadding(20)},
            {&pxDOSHeader->FileOffsetToHeader, PXDataTypeInt32ULE}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        // if this is not a DOS file, yeet
        const PXBool isValidFile = PXMemoryCompare(pxDOSHeader->Magic.Data, sizeof(PXDOSHeaderSignatore), PXDOSHeaderSignatore, sizeof(PXDOSHeaderSignatore));

        if (!isValidFile)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        // Extended parse for DOS-Header
    }

    // Start Parse of PE header
    {
        PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxBinaryWindows->Header.FileOffsetToHeader);

        const PXBool isValidFile = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXPEHeaderSignatore, sizeof(PXPEHeaderSignatore));

        const PXActionResult coffLoadResult = PXCOFFLoadFromFile(&pxBinaryWindows->COFFHeader, pxResourceLoadInfo->FileReference);

        PXActionReturnOnError(coffLoadResult);
    }

#if PXBinaryWindowsDebug
    PXLogPrint
    (
        PXLoggingInfo,
        "BinaryWindows",
        "Parsing",
        "finished"
    );
#endif


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXBinaryWindowsSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}