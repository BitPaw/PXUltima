#include "PXOGG.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

const static char PXOGGHeaderSignature[4] = { 'O','g','g','S' };

#define PXOGGHeaderTypeContinuation 0x01
#define PXOGGHeaderTypeBeginningOfStream 0x02
#define PXOGGHeaderTypeEndOfStream 0x04

#define PXOPGGDebug 0

PXActionResult PXAPI PXOGGLoadFromFile(PXSound* const pxSound, PXFile* const pxFile)
{
	PXOGG* ogg = PXNull;

	PXMemoryClear(ogg, sizeof(PXOGG));

	while(!PXFileIsAtEnd(pxFile))
	{
		// Header tag does exist multible times.
		// You can refocus it when the file is corrupted.
		PXOGGPage page;

		PXInt32UCluster signature;

		const PXFileDataElementType pxDataStreamElementList[] =
		{ 
			{signature.Data,PXDataTypeDatax4},
			{&page.Version, PXDataTypeInt08U},
			{&page.HeaderType, PXDataTypeInt08U},
			{&page.GranulePosition, PXDataTypeInt32UBE},
			{&page.SerialNumber, PXDataTypeInt32UBE},
			{&page.SequenceNumber, PXDataTypeInt32UBE},
			{&page.CRC32CheckSum, PXDataTypeInt32UBE},
			{&page.PageSegments, PXDataTypeInt08U}
		};

		PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

		const PXBool validHeaderSignature = PXMemoryCompare(signature.Data, 4u, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

		if (!validHeaderSignature)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}



		unsigned char segmentSizeList[0xFF];

#if PXOPGGDebug
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
#endif

		for(PXSize i = 0; i < page.PageSegments; ++i)
		{
			PXFileReadI8U(pxFile, &segmentSizeList[i]);

#if PXOPGGDebug
			printf
			(
				"| Segment | %3i/%3i | %3i Bytes %-45s |\n",
				i + 1,
				page.PageSegments,
				segmentSizeList[i],
				""
			);
#endif
		}

		for(PXSize i = 0; i < page.PageSegments; ++i)
		{
			unsigned char x = segmentSizeList[i];

			for(PXSize i = 0; i < x; i++)
			{
				unsigned char* currentPos = (unsigned char*)PXFileCursorPosition(pxFile) + i;

				char print = (*currentPos >= ' ' && *currentPos <= '~') ? *currentPos : '.';

#if PXOPGGDebug
				printf("%c", print);

				if(((i + 1) % 64u) == 0)
				{
					printf("\n");
				}
#endif
			}

#if PXOPGGDebug
			printf("\n");
#endif

			PXFileCursorAdvance(pxFile, x);
		}
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXOGGSaveToFile(PXSound* const pxSound, PXFile* const pxFile)
{	
	PXFileWriteB(pxFile, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

	return PXActionRefusedNotImplemented;
}