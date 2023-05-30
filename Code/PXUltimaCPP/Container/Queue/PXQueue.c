#include "PXQueue.h"

#include <OS/Memory/PXMemory.h>

void PXQueueConstruct(PXQueue* const queue)
{
    PXMemoryClear(queue, sizeof(PXQueue));
}

void PXQueueDestruct(PXQueue* const queue)
{
  
}

void PXQueueInitialize(PXQueue* const queue, void* const data, const PXSize dataTypeSize, const PXSize sizeAllocated)
{
    queue->Data = data;
    queue->SizeAllocated = sizeAllocated;
    queue->SizeUsed = 0;
}

PXBool PXQueueEnqueue(PXQueue* const queue, const void* const dataElement)
{
    return 0;
}

PXBool PXQueueDequeue(PXQueue* const queue, void* const dataElement)
{
    return 0;
}
