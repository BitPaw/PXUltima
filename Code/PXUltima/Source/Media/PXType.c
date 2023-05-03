#include "PXType.h"

#include <OS/Memory/PXMemory.h>

void PXEndianSwap(void* const data, const PXSize dataSize, const PXEndian endianFrom, const PXEndian endianTo)
{
	{
		const PXBool inoutIsOutput = endianFrom == endianTo;

		if (inoutIsOutput)
		{
			return;
		}
	}
	const PXSize runntime = dataSize / 2;
	const PXByte* endAdress = (PXByte*)data + (dataSize - 1);

	for (PXSize i = 0; i < runntime; ++i)
	{
		PXByte* a = (PXByte*)data + i;
		PXByte* b = (PXByte*)endAdress - i;

		PXByte c = *a; // Backup a
		*a = *b; // b -> a
		*b = c; // a -> b
	}
}

void PXEndianSwapV(void* const data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo)
{
	{
		const PXBool inoutIsOutput = endianFrom == endianTo;

		if (inoutIsOutput)
		{
			return;
		}
	}

	const PXSize blockSize = elementSize / 2;
	const PXSize blockAmount = elementSize / dataSize;

	for (PXSize j = 0; j < blockAmount; ++j)
	{
		PXByte* blockStart = (PXByte*)data + elementSize * j;

		for (PXSize i = 0; i < blockSize; ++i)
		{
			PXByte* a = (PXByte*)blockStart + i;
			PXByte* b = ((PXByte*)blockStart + elementSize) - (i + 1);

			PXByte c = *a; // Backup a
			*a = *b; // b -> a
			*b = c; // a -> b
		}
	}
}