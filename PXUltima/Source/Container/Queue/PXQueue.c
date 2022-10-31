#include "PXQueue.h"

#include <Memory/PXMemory.h>

void PXQueueConstruct(PXQueue* const queue)
{
    MemorySet(queue, sizeof(PXQueue), 0);
}

void PXQueueDestruct(PXQueue* const queue)
{
  
}

void PXQueueInitialize(PXQueue* const queue, void* const data, const size_t dataTypeSize, const size_t sizeAllocated)
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
