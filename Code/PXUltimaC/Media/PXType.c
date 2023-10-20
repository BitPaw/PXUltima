#include "PXType.h"

#include <OS/Memory/PXMemory.h>

#define PX_X86_BSWAP_ALLOW OS32B 

#define PXDataTypeInt08ULimit 255 // 0xFF
#define PXDataTypeInt16ULimit 65535 // 0xFFFF
#define PXDataTypeInt32ULimit 4294967295 // 0xFFFFFFFF
#define PXDataTypeInt64ULimit 0xFFFFFFFFFFFFFFFF // 0xFFFFFFFFFFFFFFFF

PXInt32U PXDataTypeIntFitting(const PXSize expectedSize)
{
	{
		const PXBool fitIn08Bit = expectedSize <= (PXSize)PXDataTypeInt08ULimit;

		if (fitIn08Bit)
		{
			return PXDataTypeInt08U;
		}
	}

	{
		const PXBool fitIn16Bit = expectedSize <= (PXSize)PXDataTypeInt16ULimit;

		if (fitIn16Bit)
		{
			return PXDataTypeInt16U;
		}
	}

	{
		const PXBool fitIn32Bit = expectedSize <= (PXSize)PXDataTypeInt32ULimit;

		if (fitIn32Bit)
		{
			return PXDataTypeInt32U;
		}
	}

	return PXDataTypeInt64U;
}

void PXEndianSwapI32U(PXInt32U* const value)
{

	

#if PX_X86_BSWAP_ALLOW

	PXInt32U wurst = *value;
	
	__asm
	{
		mov EAX, 0x0
		mov EDX, 0x0

		mov EAX, wurst
		mov EDX, [wurst] // Load variable into register 

		BSWAP EDX        // Execute byte swap in register 'EAX'
		mov [wurst], EDX
	}

	*value = wurst;

#else

#endif // 0
}

void PXEndianSwapI16U(PXInt16U* const value)
{

#if PX_X86_BSWAP_ALLOW

	__asm
	{
		mov ax, 4
		mov bx, 8
		xchg ax, bx
	}

#else

#endif // 0




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