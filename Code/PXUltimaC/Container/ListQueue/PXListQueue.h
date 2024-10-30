#ifndef PXQueueDEFINED
#define PXQueueDEFINED

#include <Media/PXType.h>

typedef struct PXQueue_
{
    void* Data;

    PXSize SizeUsed;
    PXSize SizeAllocated;
}
PXQueue;

PXPublic void PXAPI PXQueueConstruct(PXQueue* const queue);
PXPublic void PXAPI PXQueueDestruct(PXQueue* const queue);

PXPublic void PXAPI PXQueueInitialize(PXQueue* const queue, void* const data, const PXSize dataTypeSize, const PXSize sizeAllocated);

PXPublic PXBool PXAPI PXQueueEnqueue(PXQueue* const queue, const void* const dataElement);
PXPublic PXBool PXAPI PXQueueDequeue(PXQueue* const queue, void* const dataElement);

#endif