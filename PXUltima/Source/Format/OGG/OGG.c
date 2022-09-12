#include "OGG.h"

#include <File/DataStream.h>
#include <Memory/Memory.h>

#define OGGHeaderSignature { 'O','g','g','S' }

#define OGGHeaderTypeContinuation 0x01
#define OGGHeaderTypeBeginningOfStream 0x02
#define OGGHeaderTypeEndOfStream 0x04

ActionResult OGGParse(OGG* ogg, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;

	MemorySet(ogg, sizeof(OGG), 0);
	*dataRead = 0;
	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);

	while(!DataStreamIsAtEnd(&dataStream))
	{
		OGGPage page;

		//Check OGG Header
		{
			// Header tag does exist multible times.
			// You can refocus it when the file is corrupted.
			const unsigned char* headerSignature[] = OGGHeaderSignature;
			const size_t headerSignatureSize = sizeof(headerSignature);
			const unsigned char validHeaderSignature = DataStreamReadAndCompare(&dataStream, headerSignature, headerSignatureSize);

			if(!validHeaderSignature)
			{
				printf("Chunk Index missing!\n");
				return ActionInvalidHeaderSignature;
			}
		}

		DataStreamReadCU(&dataStream, &page.Version);
		DataStreamReadCU(&dataStream, &page.HeaderType);
		DataStreamReadIU(&dataStream, &page.GranulePosition, EndianLittle);
		DataStreamReadIU(&dataStream, &page.SerialNumber, EndianLittle);
		DataStreamReadIU(&dataStream, &page.SequenceNumber, EndianLittle);
		DataStreamReadIU(&dataStream, &page.CRC32CheckSum, EndianLittle);
		DataStreamReadCU(&dataStream, &page.PageSegments);

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

		for(size_t i = 0; i < page.PageSegments; ++i)
		{
			DataStreamReadCU(&dataStream, &segmentSizeList[i]);

			printf
			(
				"| Segment | %3i/%3i | %3i Bytes %-45s |\n",
				i + 1,
				page.PageSegments,
				segmentSizeList[i],
				""
			);
		}

		for(size_t i = 0; i < page.PageSegments; ++i)
		{
			unsigned char x = segmentSizeList[i];

			for(size_t i = 0; i < x; i++)
			{
				unsigned char* currentPos = (unsigned char*)DataStreamCursorPosition(&dataStream) + i;

				char print = (*currentPos >= ' ' && *currentPos <= '~') ? *currentPos : '.';

				printf("%c", print);

				if(((i + 1) % 64u) == 0)
				{
					printf("\n");
				}
			}

			printf("\n");

			DataStreamCursorAdvance(&dataStream, x);
		}
	}

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}