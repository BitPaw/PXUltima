#include "PXBinaryWindows.h"

#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXNEHeaderSignatore[2] = { 'N', 'E' };

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

    PXNEHeader pxNEHeaderAA;
    PXNEHeader* pxNEHeader = &pxNEHeaderAA;

    // Read header
    {
        PXDOSHeader* const pxDOSHeader = &pxBinaryWindows->Header;

        const PXDataTypeEntry pxDataStreamElementList[] =
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

        const PXSize amount = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
        const PXBool validSize = amount >= 0;

        // if this is not a DOS file, yeet
        const PXBool isValidFile = PXMemoryCompare(pxDOSHeader->Magic.Data, sizeof(PXDOSHeaderSignatore), PXDOSHeaderSignatore, sizeof(PXDOSHeaderSignatore));

        if (!isValidFile)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        if(!validSize)
        {
            return PXActionInvalid;
        }


        pxDOSHeader->ActualSize = pxDOSHeader->TotalPagesInFile * 512u;

        if(0 != pxDOSHeader->LastPageSize)
        {
            // Last block is not 512 but less than this, so adjust it.
            pxDOSHeader->ActualSize -= 512u;
            pxDOSHeader->ActualSize += pxDOSHeader->LastPageSize;
        }



        // Extended parse for DOS-Header

        // Move to end of header
        PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxBinaryWindows->Header.FileOffsetToHeader);
    }

    // Probe NE-Header
    {
        const PXBool isValidFile = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXNEHeaderSignatore, sizeof(PXNEHeaderSignatore));

        if(isValidFile)
        {

            const PXDataTypeEntry pxDataStreamElementList[] =
            {
                {&pxNEHeader->LinkerVersionMajor, PXDataTypeInt08U},
                {&pxNEHeader->LinkerVersionMinor, PXDataTypeInt08U},
                {&pxNEHeader->EntryTableOffset, PXDataTypeInt16ULE},
                {&pxNEHeader->EntryTableLength, PXDataTypeInt16ULE},
                {&pxNEHeader->FileLoadCRC, PXDataTypeInt32ULE},
                {&pxNEHeader->FlagList, PXDataTypeInt16ULE},
                {&pxNEHeader->SegmentDataAutoIndex, PXDataTypeInt16ULE},
                {&pxNEHeader->HeapSizeInit, PXDataTypeInt16ULE},
                {&pxNEHeader->StackSizeInit, PXDataTypeInt16ULE},
                {&pxNEHeader->EntryPoint, PXDataTypeInt32ULE},
                {&pxNEHeader->InitStack, PXDataTypeInt32ULE},
                {&pxNEHeader->SegmentTableEntryAmount, PXDataTypeInt16ULE},
                {&pxNEHeader->ModuleReferenceTableEntryAmount, PXDataTypeInt16ULE},
                {&pxNEHeader->NonresidentNameTableEntryAmount, PXDataTypeInt16ULE},
                {&pxNEHeader->SegmentTableOffset, PXDataTypeInt16ULE},
                {&pxNEHeader->ResourceTableOffset, PXDataTypeInt16ULE},
                {&pxNEHeader->ResidentNamesTableOffset, PXDataTypeInt16ULE},
                {&pxNEHeader->ModRefTable, PXDataTypeInt16ULE},
                {&pxNEHeader->ImportNameTable, PXDataTypeInt16ULE},
                {&pxNEHeader->OffStartNonResTab, PXDataTypeInt32ULE},
                {&pxNEHeader->MovEntryCount, PXDataTypeInt16ULE},
                {&pxNEHeader->FileAlnSzShftCnt, PXDataTypeInt16ULE},
                {&pxNEHeader->nResTabEntries, PXDataTypeInt16ULE},
                {&pxNEHeader->targOS, PXDataTypeInt08U}

            };

            const PXSize amount = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
            const PXBool validSize = (amount+2) >= 55;

            // Entry table
            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxNEHeader->EntryTableOffset);

            // segment table
            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxNEHeader->SegmentTableOffset);

            for(PXSize i = 0; i < pxNEHeader->SegmentTableEntryAmount; i++)
            {
                PXNEHeaderSegmentEntry pxNEHeaderSegmentEntry;

                const PXDataTypeEntry pxDataStreamElementList[] =
                {
                    {&pxNEHeaderSegmentEntry.LogicalSectorOffset, PXDataTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.LengthoftheSegment, PXDataTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.Flag, PXDataTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.MinimumAllocationSize, PXDataTypeInt16ULE}
                };

                const PXSize amount = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                const PXBool validSize = amount >= 0;


            }
            //----------------------------------------------
            // Resource table
            //----------------------------------------------
            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxNEHeader->ResourceTableOffset);
            //----------------------------------------------



            //----------------------------------------------
            // Resident-Name Table
            //----------------------------------------------
            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxNEHeader->OffStartNonResTab);

            for(PXInt16U i = 0; i < pxNEHeader->NonresidentNameTableEntryAmount; ++i)
            {
                char text[0xFF];
                PXInt16U ordinal = 0;
                PXInt8U stringSize = 0;

                PXFileReadI8U(pxResourceLoadInfo->FileReference, &stringSize);

                if(0 == stringSize) // End of list
                {
                    break;
                }

                PXFileReadB(pxResourceLoadInfo->FileReference, text, stringSize);
                PXFileReadI16UE(pxResourceLoadInfo->FileReference, &ordinal, PXEndianLittle);

                text[stringSize] = '\0';

#if PXBinaryWindowsDebug
                PXLogPrint
                (
                    PXLoggingInfo,
                    "BinaryWindows",
                    "Parsing",
                    "%8.8X - %s",
                    ordinal,
                    text
                );
#endif
            }
            //----------------------------------------------



            //----------------------------------------------
            // MODULE-REFERENCE TABLE
            //----------------------------------------------
            PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, pxNEHeader->ModRefTable);

            for(PXSize i = 0; i < pxNEHeader->ImportNameTable; ++i)
            {
                PXInt16U offset = 0;

                PXFileReadI16UE(pxResourceLoadInfo->FileReference, &offset, PXEndianLittle);

                PXSize oldPosition = pxResourceLoadInfo->FileReference->DataCursor;


                PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, offset);


                PXInt16U length = 0;

                PXFileReadI16UE(pxResourceLoadInfo->FileReference, &length, PXEndianLittle);

                char name[64];
                PXFileReadB(pxResourceLoadInfo->FileReference, name, length);


                PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, oldPosition);

            }






        }
    }

    // Start Parse of PE header
    {
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
