#include "PXEXE.h"

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXPEHeaderSignatore[4] = { 'P', 'E', '\0', '\0' };

PXActionResult PXEXELoadFromFile(PXEXE* const pxEXE, PXFile* const pxFile)
{
	// Read header
	{
        PXDOSHeader* const pxEXEHeader = &pxEXE->Header;

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {PXDataTypeTextx2, pxEXEHeader->Magic.Data},
            {PXDataTypeLEInt16U, &pxEXEHeader->LastPageSize},
            {PXDataTypeLEInt16U, &pxEXEHeader->TotalPagesInFile},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReallocationItems},

            {PXDataTypeLEInt16U, &pxEXEHeader->ParagraphsInHeader},
            {PXDataTypeLEInt16U, &pxEXEHeader->ParagraphsExtraMinimum},
            {PXDataTypeLEInt16U, &pxEXEHeader->ParagraphsExtraMaximum},
            {PXDataTypeLEInt16U, &pxEXEHeader->StackInitialSegment},

            {PXDataTypeLEInt16U, &pxEXEHeader->StackInitialPointer},
            {PXDataTypeLEInt16U, &pxEXEHeader->ChecksumCompliment},
            {PXDataTypeLEInt16U, &pxEXEHeader->InstructionPointerInternal},
            {PXDataTypeLEInt16U, &pxEXEHeader->CodeSegmentInitial},

            {PXDataTypeLEInt16U, &pxEXEHeader->e_lfarlc},
            {PXDataTypeLEInt16U, &pxEXEHeader->e_ovno},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedA[0]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedA[1]},

            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedA[2]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedA[3]},
            {PXDataTypeLEInt16U, &pxEXEHeader->e_oemid},
            {PXDataTypeLEInt16U, &pxEXEHeader->e_oeminfo},

            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[0]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[1]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[2]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[3]},

            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[4]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[5]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[6]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[7]},

            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[8]},
            {PXDataTypeLEInt16U, &pxEXEHeader->ReservedB[9]},
            {PXDataTypeLEInt32U, &pxEXEHeader->FileOffsetToHeader}
        };
        const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

        PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

        // if this is not a DOS file, yeet
        const PXBool isValidFile = PXMemoryCompare(pxEXEHeader->Magic.Data, 2, PXDOSHeaderSignatore, sizeof(PXDOSHeaderSignatore));

        if (!isValidFile)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        // Extended parse for DOS-Header
	}

    // Start Parse of PE header
    {
        PXFileCursorMoveTo(pxFile, pxEXE->Header.FileOffsetToHeader);

        const PXBool isValidFile = PXFileReadAndCompare(pxFile, PXPEHeaderSignatore, sizeof(PXPEHeaderSignatore));

        PXCOFFLoadFromFile(&pxEXE->COFFHeader, pxFile);

    }


	return PXActionRefusedNotImplemented;
}

PXActionResult PXEXESaveToFile(PXEXE* const pxEXE, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}