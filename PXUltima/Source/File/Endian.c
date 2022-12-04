#include "Endian.h"

void EndianSwap(void* data, const PXSize dataSize, const Endian endianFrom, const Endian endianTo)
{
	{
		const unsigned char inoutIsOutput = endianFrom == endianTo;

		if(inoutIsOutput)
		{
			return;
		}
	}
	PXSize runntime = dataSize / 2;
	const unsigned char* endAdress = (unsigned char*)data + (dataSize - 1);

	for(PXSize i = 0; i < runntime; ++i)
	{
		unsigned char* a = (unsigned char*)data + i;
		unsigned char* b = (unsigned char*)endAdress - i;

		unsigned char c = *a; // Backup a
		*a = *b; // b -> a
		*b = c; // a -> b
	}
}