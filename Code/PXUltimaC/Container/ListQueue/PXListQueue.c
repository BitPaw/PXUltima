#include "PXListQueue.h"

#include <OS/Memory/PXMemory.h>

void PXAPI PXQueueConstruct(PXQueue* const queue)
{
    PXClear(PXQueue, queue);
}

void PXAPI PXQueueDestruct(PXQueue* const queue)
{
  
}

void PXAPI PXQueueInitialize(PXQueue* const queue, void* const data, const PXSize dataTypeSize, const PXSize sizeAllocated)
{
    queue->Data = data;
    queue->SizeAllocated = sizeAllocated;
    queue->SizeUsed = 0;
}

PXBool PXAPI PXQueueEnqueue(PXQueue* const queue, const void* const dataElement)
{
    return 0;
}

PXBool PXAPI PXQueueDequeue(PXQueue* const queue, void* const dataElement)
{
    return 0;
}