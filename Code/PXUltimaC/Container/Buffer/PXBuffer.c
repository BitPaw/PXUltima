#include "PXBuffer.h"

#include <OS/Memory/PXMemory.h>

void PXBufferConstruct(PXBuffer* const pxBuffer, void* buffer, PXSize size, PXBufferType pxBufferType)
{
	pxBuffer->Data = buffer;
	pxBuffer->SizeMaximum = size;
	pxBuffer->SizeCurrent = 0;
	pxBuffer->Type = pxBufferType;
}

void PXBufferDestruct(PXBuffer* const pxBuffer)
{
	if (pxBuffer->Type == PXBufferTypeHeap)
	{
		PXMemoryRelease(pxBuffer->Data, pxBuffer->SizeMaximum);
	}
}