#include "PXType.h"

#include <OS/Memory/PXMemory.h>

PXInt8U PXDataTypeSize(const PXDataType pxDataType)
{
	switch (pxDataType)
	{
		case PXDataTypeInt8S:
		case PXDataTypeInt8U:
			return sizeof(PXInt8U);

		case PXDataTypeLEInt16S:
		case PXDataTypeLEInt16U:
		case PXDataTypeBEInt16S:
		case PXDataTypeBEInt16U:
			return sizeof(PXInt16U);

		case PXDataTypeLEInt32S:
		case PXDataTypeLEInt32U:
		case PXDataTypeBEInt32S:
		case PXDataTypeBEInt32U:
			return sizeof(PXInt32U);

		case PXDataTypeLEInt64S:
		case PXDataTypeLEInt64U:
		case PXDataTypeBEInt64S:
		case PXDataTypeBEInt64U:
			return sizeof(PXInt64U);

		case PXDataTypeFloat:
			return sizeof(float);

		case PXDataTypeDouble:
			return sizeof(double);

		case PXDataTypeTypeInvalid:
		default:
			return 0u;
	}
}

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

void PXEndianSwapV(void** const data, const PXSize dataSize, const PXSize elementSize, const PXEndian endianFrom, const PXEndian endianTo)
{
	const PXSize amount = dataSize / elementSize;

	for (PXSize i = 0; i < amount; ++i)
	{
		void* const insertPoint = data[i];

		PXEndianSwap(insertPoint, elementSize, endianFrom, endianTo);
	}
}