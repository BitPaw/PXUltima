#include "PXBinaryWindows.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXNEHeaderSignatore[2] = { 'N', 'E' };
const char PXPEHeaderSignatore[4] = { 'P', 'E', '\0', '\0' };

const PXI32U PXDOSHeaderLayout[] =
{
    PXTypeDatax2,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypePadding(8),
    PXTypeI16ULE,
    PXTypeI16ULE,
    PXTypePadding(20),
    PXTypeI32ULE
};
const PXSize PXDOSHeaderLayoutSize = sizeof(PXDOSHeaderLayout) / sizeof(PXI32U);
const char PXBinaryWindowsText[] = "BinaryWindows";

#define PXBinaryWindowsDebug 1

PXResult PXAPI PXBinaryWindowsLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXBinaryWindows PXREF pxBinaryWindows = 0;// (PXBinaryWindows*)pxResourceLoadInfo->ResourceTarget;

    if(!pxBinaryWindows)
    {
        return PXResultRefusedParameterNull;
    }

    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

#if PXBinaryWindowsDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXBinaryWindowsText,
        "Parsing",
        "Start"
    );
#endif

    PXNEHeader pxNEHeaderAA;
    PXNEHeader* pxNEHeader = &pxNEHeaderAA;

    // Read header
    {
        PXDOSHeader PXREF pxDOSHeader = &pxBinaryWindows->Header;
#if 0
        PXFileBinding(pxResourceLoadInfo->FileCurrent, &pxBinaryWindows->Header, PXDOSHeaderLayout, PXDOSHeaderLayoutSize, PXFalse);

#else
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {pxDOSHeader->Magic,PXTypeDatax2 },
            {&pxDOSHeader->LastPageSize, PXTypeI16ULE},
            {&pxDOSHeader->TotalPagesInFile, PXTypeI16ULE},
            {&pxDOSHeader->ReallocationItems, PXTypeI16ULE},
            {&pxDOSHeader->ParagraphsInHeader, PXTypeI16ULE},
            {&pxDOSHeader->ParagraphsExtraMinimum, PXTypeI16ULE},
            {&pxDOSHeader->ParagraphsExtraMaximum, PXTypeI16ULE},
            {&pxDOSHeader->StackInitialSegment, PXTypeI16ULE},
            {&pxDOSHeader->StackInitialPointer, PXTypeI16ULE},
            {&pxDOSHeader->ChecksumCompliment, PXTypeI16ULE},
            {&pxDOSHeader->InstructionPointerInternal, PXTypeI16ULE},
            {&pxDOSHeader->CodeSegmentInitial, PXTypeI16ULE},
            {&pxDOSHeader->e_lfarlc, PXTypeI16ULE},
            {&pxDOSHeader->e_ovno, PXTypeI16ULE},
            {PXNull, PXTypePadding(8)},
            {&pxDOSHeader->e_oemid, PXTypeI16ULE},
            {&pxDOSHeader->e_oeminfo, PXTypeI16ULE},
            {PXNull, PXTypePadding(20)},
            {&pxDOSHeader->FileOffsetToHeader, PXTypeI32ULE}
        };

        const PXSize amount = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
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
      //      return PXResultInvalid;
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
        PXFileCursorMoveTo(pxFile, pxBinaryWindows->Header.FileOffsetToHeader);
    }

    // Probe NE-Header
    {
        const PXBool isValidFile = PXFileReadAndCompare(pxFile, PXNEHeaderSignatore, sizeof(PXNEHeaderSignatore));

        if(isValidFile)
        {

            const PXTypeEntry pxDataStreamElementList[] =
            {
                {&pxNEHeader->LinkerVersionMajor, PXTypeI08U},
                {&pxNEHeader->LinkerVersionMinor, PXTypeI08U},
                {&pxNEHeader->EntryTableOffset, PXTypeI16ULE},
                {&pxNEHeader->EntryTableLength, PXTypeI16ULE},
                {&pxNEHeader->FileLoadCRC, PXTypeI32ULE},
                {&pxNEHeader->FlagList, PXTypeI16ULE},
                {&pxNEHeader->SegmentDataAutoIndex, PXTypeI16ULE},
                {&pxNEHeader->HeapSizeInit, PXTypeI16ULE},
                {&pxNEHeader->StackSizeInit, PXTypeI16ULE},
                {&pxNEHeader->EntryPoint, PXTypeI32ULE},
                {&pxNEHeader->InitStack, PXTypeI32ULE},
                {&pxNEHeader->SegmentTableEntryAmount, PXTypeI16ULE},
                {&pxNEHeader->ModuleReferenceTableEntryAmount, PXTypeI16ULE},
                {&pxNEHeader->NonresidentNameTableEntryAmount, PXTypeI16ULE},
                {&pxNEHeader->SegmentTableOffset, PXTypeI16ULE},
                {&pxNEHeader->ResourceTableOffset, PXTypeI16ULE},
                {&pxNEHeader->ResidentNamesTableOffset, PXTypeI16ULE},
                {&pxNEHeader->ModRefTable, PXTypeI16ULE},
                {&pxNEHeader->ImportNameTable, PXTypeI16ULE},
                {&pxNEHeader->OffStartNonResTab, PXTypeI32ULE},
                {&pxNEHeader->MovEntryCount, PXTypeI16ULE},
                {&pxNEHeader->FileAlnSzShftCnt, PXTypeI16ULE},
                {&pxNEHeader->nResTabEntries, PXTypeI16ULE},
                {&pxNEHeader->targOS, PXTypeI08U}

            };

            const PXSize amount = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
            const PXBool validSize = (amount+2) >= 55;

            // Entry table
            PXFileCursorMoveTo(pxFile, pxNEHeader->EntryTableOffset);

            // segment table
            PXFileCursorMoveTo(pxFile, pxNEHeader->SegmentTableOffset);

            for(PXSize i = 0; i < pxNEHeader->SegmentTableEntryAmount; i++)
            {
                PXNEHeaderSegmentEntry pxNEHeaderSegmentEntry;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxNEHeaderSegmentEntry.LogicalSectorOffset, PXTypeI16ULE},
                    {&pxNEHeaderSegmentEntry.LengthoftheSegment, PXTypeI16ULE},
                    {&pxNEHeaderSegmentEntry.Flag, PXTypeI16ULE},
                    {&pxNEHeaderSegmentEntry.MinimumAllocationSize, PXTypeI16ULE}
                };

                const PXSize amount = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                const PXBool validSize = amount >= 0;


            }
            //----------------------------------------------
            // Resource table
            //----------------------------------------------
            PXFileCursorMoveTo(pxFile, pxNEHeader->ResourceTableOffset);
            //----------------------------------------------



            //----------------------------------------------
            // Resident-Name Table
            //----------------------------------------------
            PXFileCursorMoveTo(pxFile, pxNEHeader->OffStartNonResTab);

            for(PXI16U i = 0; i < pxNEHeader->NonresidentNameTableEntryAmount; ++i)
            {
                char text[0xFF];
                PXI16U ordinal = 0;
                PXI8U stringSize = 0;

                PXFileReadI8U(pxFile, &stringSize);

                if(0 == stringSize) // End of list
                {
                    break;
                }

                PXFileReadB(pxFile, text, stringSize);
                PXFileReadI16UE(pxFile, &ordinal, PXEndianLittle);

                text[stringSize] = '\0';

#if PXBinaryWindowsDebug
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXBinaryWindowsText,
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
            PXFileCursorMoveTo(pxFile, pxNEHeader->ModRefTable);

            for(PXSize i = 0; i < pxNEHeader->ImportNameTable; ++i)
            {
                PXI16U offset = 0;

                PXFileReadI16UE(pxFile, &offset, PXEndianLittle);

                PXSize oldPosition = PXFileDataPosition(pxFile);


                PXFileCursorMoveTo(pxFile, offset);


                PXI16U length = 0;

                PXFileReadI16UE(pxFile, &length, PXEndianLittle);

                char name[64];
                PXFileReadB(pxFile, name, length);


                PXFileCursorMoveTo(pxFile, oldPosition);

            }
        }
    }

    // Start Parse of PE header
    {
        const PXBool isValidFile = PXFileReadAndCompare
        (
            pxFile,
            PXPEHeaderSignatore,
            sizeof(PXPEHeaderSignatore)
        );

        const PXResult coffLoadResult = PXCOFFLoadFromFile(&pxBinaryWindows->COFFHeader, pxFile);

        if(PXResultOK != coffLoadResult) 
            return coffLoadResult;
    }

#if PXBinaryWindowsDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXBinaryWindowsText,
        "Parsing",
        "finished"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXBinaryWindowsSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
