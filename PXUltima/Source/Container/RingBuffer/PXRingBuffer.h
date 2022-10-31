#ifndef PXListDEFINED
#define PXListDEFINED

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXRingBuffer_
	{
		void* Data; // Contains the whole data.
		size_t DataTypeSize; // Size of a single object.
		size_t DataIndexCurrent;  // Current data index.
		size_t SizeAllocated; // Size currently allocated and usable
	}
	PXRingBuffer;

	PXPublic void PXRingBufferConstruct(PXRingBuffer* const ringBuffer);
	PXPublic void PXRingBufferDestruct(PXRingBuffer* const ringBuffer);

	PXPublic void PXRingBufferInitialize(PXRingBuffer* const ringBuffer, void* const data, const size_t dataTypeSize, const size_t sizeAllocated);

	PXPublic void PXRingBufferAdd(PXRingBuffer* const ringBuffer, void* const dataElement);

#ifdef __cplusplus
}
#endif

#endif