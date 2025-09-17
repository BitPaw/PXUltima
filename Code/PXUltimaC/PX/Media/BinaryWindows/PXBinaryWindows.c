#include "PXBinaryWindows.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXNEHeaderSignatore[2] = { 'N', 'E' };
const char PXPEHeaderSignatore[4] = { 'P', 'E', '\0', '\0' };



const PXI32U PXDOSHeaderLayout[] =
{
    PXTypeDatax2,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypePadding(8),
    PXTypeInt16ULE,
    PXTypeInt16ULE,
    PXTypePadding(20),
    PXTypeInt32ULE
};
const PXSize PXDOSHeaderLayoutSize = sizeof(PXDOSHeaderLayout) / sizeof(PXI32U);


#define PXBinaryWindowsDebug 1

PXResult PXAPI  PXBinaryWindowsLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXBinaryWindows* const pxBinaryWindows = (PXBinaryWindows*)pxResourceLoadInfo->ResourceTarget;

    if(!pxBinaryWindows)
    {
        return PXActionRefusedArgumentNull;
    }

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
#if 0
        PXFileBinding(pxResourceLoadInfo->FileReference, &pxBinaryWindows->Header, PXDOSHeaderLayout, PXDOSHeaderLayoutSize, PXFalse);

#else
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {pxDOSHeader->Magic,PXTypeDatax2 },
            {&pxDOSHeader->LastPageSize, PXTypeInt16ULE},
            {&pxDOSHeader->TotalPagesInFile, PXTypeInt16ULE},
            {&pxDOSHeader->ReallocationItems, PXTypeInt16ULE},
            {&pxDOSHeader->ParagraphsInHeader, PXTypeInt16ULE},
            {&pxDOSHeader->ParagraphsExtraMinimum, PXTypeInt16ULE},
            {&pxDOSHeader->ParagraphsExtraMaximum, PXTypeInt16ULE},
            {&pxDOSHeader->StackInitialSegment, PXTypeInt16ULE},
            {&pxDOSHeader->StackInitialPointer, PXTypeInt16ULE},
            {&pxDOSHeader->ChecksumCompliment, PXTypeInt16ULE},
            {&pxDOSHeader->InstructionPointerInternal, PXTypeInt16ULE},
            {&pxDOSHeader->CodeSegmentInitial, PXTypeInt16ULE},
            {&pxDOSHeader->e_lfarlc, PXTypeInt16ULE},
            {&pxDOSHeader->e_ovno, PXTypeInt16ULE},
            {PXNull, PXTypePadding(8)},
            {&pxDOSHeader->e_oemid, PXTypeInt16ULE},
            {&pxDOSHeader->e_oeminfo, PXTypeInt16ULE},
            {PXNull, PXTypePadding(20)},
            {&pxDOSHeader->FileOffsetToHeader, PXTypeInt32ULE}
        };

        const PXSize amount = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
        const PXBool validSize = amount >= 0;
#endif

        // if this is not a DOS file, yeet
        const PXBool isValidFile = PXMemoryCompare(pxDOSHeader->Magic, sizeof(PXDOSHeaderSignatore), PXDOSHeaderSignatore, sizeof(PXDOSHeaderSignatore));

        if (!isValidFile)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

      //  if(!validSize)
      //  {
      //      return PXActionInvalid;
     //   }


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

            const PXTypeEntry pxDataStreamElementList[] =
            {
                {&pxNEHeader->LinkerVersionMajor, PXTypeInt08U},
                {&pxNEHeader->LinkerVersionMinor, PXTypeInt08U},
                {&pxNEHeader->EntryTableOffset, PXTypeInt16ULE},
                {&pxNEHeader->EntryTableLength, PXTypeInt16ULE},
                {&pxNEHeader->FileLoadCRC, PXTypeInt32ULE},
                {&pxNEHeader->FlagList, PXTypeInt16ULE},
                {&pxNEHeader->SegmentDataAutoIndex, PXTypeInt16ULE},
                {&pxNEHeader->HeapSizeInit, PXTypeInt16ULE},
                {&pxNEHeader->StackSizeInit, PXTypeInt16ULE},
                {&pxNEHeader->EntryPoint, PXTypeInt32ULE},
                {&pxNEHeader->InitStack, PXTypeInt32ULE},
                {&pxNEHeader->SegmentTableEntryAmount, PXTypeInt16ULE},
                {&pxNEHeader->ModuleReferenceTableEntryAmount, PXTypeInt16ULE},
                {&pxNEHeader->NonresidentNameTableEntryAmount, PXTypeInt16ULE},
                {&pxNEHeader->SegmentTableOffset, PXTypeInt16ULE},
                {&pxNEHeader->ResourceTableOffset, PXTypeInt16ULE},
                {&pxNEHeader->ResidentNamesTableOffset, PXTypeInt16ULE},
                {&pxNEHeader->ModRefTable, PXTypeInt16ULE},
                {&pxNEHeader->ImportNameTable, PXTypeInt16ULE},
                {&pxNEHeader->OffStartNonResTab, PXTypeInt32ULE},
                {&pxNEHeader->MovEntryCount, PXTypeInt16ULE},
                {&pxNEHeader->FileAlnSzShftCnt, PXTypeInt16ULE},
                {&pxNEHeader->nResTabEntries, PXTypeInt16ULE},
                {&pxNEHeader->targOS, PXTypeInt08U}

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

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxNEHeaderSegmentEntry.LogicalSectorOffset, PXTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.LengthoftheSegment, PXTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.Flag, PXTypeInt16ULE},
                    {&pxNEHeaderSegmentEntry.MinimumAllocationSize, PXTypeInt16ULE}
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

            for(PXI16U i = 0; i < pxNEHeader->NonresidentNameTableEntryAmount; ++i)
            {
                char text[0xFF];
                PXI16U ordinal = 0;
                PXI8U stringSize = 0;

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
                PXI16U offset = 0;

                PXFileReadI16UE(pxResourceLoadInfo->FileReference, &offset, PXEndianLittle);

                PXSize oldPosition = pxResourceLoadInfo->FileReference->DataCursor;


                PXFileCursorMoveTo(pxResourceLoadInfo->FileReference, offset);


                PXI16U length = 0;

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

PXResult PXAPI  PXBinaryWindowsSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
