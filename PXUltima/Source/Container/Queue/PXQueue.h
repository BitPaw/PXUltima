#ifndef PXQueueDEFINED
#define PXQueueDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXQueue_
	{
		void* Data;
	
		size_t SizeUsed;
		size_t SizeAllocated;
	}
	PXQueue;

	PXPublic void PXQueueConstruct(PXQueue* const queue);
	PXPublic void PXQueueDestruct(PXQueue* const queue);

	PXPublic void PXQueueInitialize(PXQueue* const queue, void* const data, const size_t dataTypeSize, const size_t sizeAllocated);

	PXPublic PXBool PXQueueEnqueue(PXQueue* const queue, const void* const dataElement);
	PXPublic PXBool PXQueueDequeue(PXQueue* const queue, void* const dataElement);

#ifdef __cplusplus
}
#endif

#endif