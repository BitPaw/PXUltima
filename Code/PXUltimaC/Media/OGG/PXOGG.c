#include "PXOGG.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define PXOGGHeaderSignature { 'O','g','g','S' }

#define PXOGGHeaderTypeContinuation 0x01
#define PXOGGHeaderTypeBeginningOfStream 0x02
#define PXOGGHeaderTypeEndOfStream 0x04

PXActionResult PXOGGParse(PXOGG* const ogg, PXFile* const pxFile)
{
	PXMemoryClear(ogg, sizeof(PXOGG));

	while(!PXFileIsAtEnd(pxFile))
	{
		PXOGGPage page;

		//Check PXOGG Header
		{
			// Header tag does exist multible times.
			// You can refocus it when the file is corrupted.
			const unsigned char* headerSignature[] = PXOGGHeaderSignature;
			const PXSize headerSignatureSize = sizeof(headerSignature);
			const unsigned char validHeaderSignature = PXFileReadAndCompare(pxFile, headerSignature, headerSignatureSize);

			if(!validHeaderSignature)
			{
				printf("Chunk Index missing!\n");
				return PXActionRefusedInvalidHeaderSignature;
			}
		}

		PXFileReadI8U(pxFile, &page.Version);
		PXFileReadI8U(pxFile, &page.HeaderType);
		PXFileReadI32UE(pxFile, &page.GranulePosition, PXEndianLittle);
		PXFileReadI32UE(pxFile, &page.SerialNumber, PXEndianLittle);
		PXFileReadI32UE(pxFile, &page.SequenceNumber, PXEndianLittle);
		PXFileReadI32UE(pxFile, &page.CRC32CheckSum, PXEndianLittle);
		PXFileReadI8U(pxFile, &page.PageSegments);

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