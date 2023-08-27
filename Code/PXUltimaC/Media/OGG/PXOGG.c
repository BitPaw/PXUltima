#include "PXOGG.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

const static char PXOGGHeaderSignature[4] = { 'O','g','g','S' };

#define PXOGGHeaderTypeContinuation 0x01
#define PXOGGHeaderTypeBeginningOfStream 0x02
#define PXOGGHeaderTypeEndOfStream 0x04

PXActionResult PXOGGLoadFromFile(PXSound* const pxSound, PXFile* const pxFile)
{
	PXOGG* ogg = PXNull;

	PXMemoryClear(ogg, sizeof(PXOGG));

	while(!PXFileIsAtEnd(pxFile))
	{
		PXOGGPage page;

		//Check PXOGG Header
		{
			// Header tag does exist multible times.
			// You can refocus it when the file is corrupted.
			const PXSize headerSignatureSize = sizeof(PXOGGHeaderSignature);
			const PXBool validHeaderSignature = PXFileReadAndCompare(pxFile, PXOGGHeaderSignature, headerSignatureSize);

			if(!validHeaderSignature)
			{
				return PXActionRefusedInvalidHeaderSignature;
			}
		}


		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXDataTypeInt8U, &page.Version},
			{PXDataTypeInt8U, &page.HeaderType},
			{PXDataTypeLEInt32U, &page.GranulePosition},
			{PXDataTypeLEInt32U, &page.SerialNumber},
			{PXDataTypeLEInt32U, &page.SequenceNumber},
			{PXDataTypeLEInt32U, &page.CRC32CheckSum},
			{PXDataTypeInt8U, &page.PageSegments}
		};
		const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

		PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

		unsigned char segmentSizeList[0xFF];

		printf
		(
			"|         | %7s | %4s | %8s | %6s | %7s | %11s | %4s |\n"
			"|  Chunk  | %7i | %4i | %8zi | %6i | %7i | %11i | %4i |\n",

			"Version",
			"Type",
			"Position",
			"Serial",
			"Sequenc",
			"CRC",
			"Page",

			page.Version,
			page.HeaderType,
			page.GranulePosition,
			page.SerialNumber,
			page.SequenceNumber,
			page.CRC32CheckSum,
			page.PageSegments
		);

		for(PXSize i = 0; i < page.PageSegments; ++i)
		{
			PXFileReadI8U(pxFile, &segmentSizeList[i]);

			printf
			(
				"| Segment | %3i/%3i | %3i Bytes %-45s |\n",
				i + 1,
				page.PageSegments,
				segmentSizeList[i],
				""
			);
		}

		for(PXSize i = 0; i < page.PageSegments; ++i)
		{
			unsigned char x = segmentSizeList[i];

			for(PXSize i = 0; i < x; i++)
			{
				unsigned char* currentPos = (unsigned char*)PXFileCursorPosition(pxFile) + i;

				char print = (*currentPos >= ' ' && *currentPos <= '~') ? *currentPos : '.';

				printf("%c", print);

				if(((i + 1) % 64u) == 0)
				{
					printf("\n");
				}
			}

			printf("\n");

			PXFileCursorAdvance(pxFile, x);
		}
	}

	return PXActionSuccessful;
}

PXActionResult PXOGGSaveToFile(PXSound* const pxSound, PXFile* const pxFile)
{	
	PXFileWriteB(pxFile, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

	return PXActionRefusedNotImplemented;
}