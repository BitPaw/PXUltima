#include "PXBinaryWindows.h"

const char PXDOSHeaderSignatore[2] = { 'M', 'Z' };
const char PXPEHeaderSignatore[4] = { 'P', 'E', '\0', '\0' };

PXActionResult PXBinaryWindowsLoadFromFile(PXBinaryWindows* const pxBinaryWindows, PXFile* const pxFile)
{
	// Read header
	{
        PXDOSHeader* const pxDOSHeader = &pxBinaryWindows->Header;

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {PXDataTypeTextx2, pxDOSHeader->Magic.Data},
            {PXDataTypeLEInt16U, &pxDOSHeader->LastPageSize},
            {PXDataTypeLEInt16U, &pxDOSHeader->TotalPagesInFile},
            {PXDataTypeLEInt16U, &pxDOSHeader->ReallocationItems},

            {PXDataTypeLEInt16U, &pxDOSHeader->ParagraphsInHeader},
            {PXDataTypeLEInt16U, &pxDOSHeader->ParagraphsExtraMinimum},
            {PXDataTypeLEInt16U, &pxDOSHeader->ParagraphsExtraMaximum},
            {PXDataTypeLEInt16U, &pxDOSHeader->StackInitialSegment},

            {PXDataTypeLEInt16U, &pxDOSHeader->StackInitialPointer},
            {PXDataTypeLEInt16U, &pxDOSHeader->ChecksumCompliment},
            {PXDataTypeLEInt16U, &pxDOSHeader->InstructionPointerInternal},
            {PXDataTypeLEInt16U, &pxDOSHeader->CodeSegmentInitial},

            {PXDataTypeLEInt16U, &pxDOSHeader->e_lfarlc},
            {PXDataTypeLEInt16U, &pxDOSHeader->e_ovno},
            {PXDataTypeTextx4, pxDOSHeader->ReservedA},
            {PXDataTypeLEInt16U, &pxDOSHeader->e_oemid},
            {PXDataTypeLEInt16U, &pxDOSHeader->e_oeminfo},

            {PXDataTypeTextx8, &pxDOSHeader->ReservedB},
            {PXDataTypeTextx2, &pxDOSHeader->ReservedB + 8},
            {PXDataTypeLEInt32U, &pxDOSHeader->FileOffsetToHeader}
        };
        const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

        PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

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
        PXFileCursorMoveTo(pxFile, pxBinaryWindows->Header.FileOffsetToHeader);

        const PXBool isValidFile = PXFileReadAndCompare(pxFile, PXPEHeaderSignatore, sizeof(PXPEHeaderSignatore));

        PXCOFFLoadFromFile(&pxBinaryWindows->COFFHeader, pxFile);
    }


	return PXActionRefusedNotImplemented;
}

PXActionResult PXBinaryWindowsSaveToFile(PXBinaryWindows* const pxBinaryWindows, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}