#ifndef PXBufferINCLUDED
#define PXBufferINCLUDED

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXBufferType_
	{
		PXBufferTypeInvalid,
		PXBufferTypeStack,
		PXBufferTypeHeap,
		PXBufferExtern,
	}
	PXBufferType;

	typedef struct PXBuffer_
	{		
		void* Data;
		PXSize SizeMaximum; // Data that can be written
		PXSize SizeCurrent; // Data that is usable
		PXSize SizeOffset; // Data that has been used/processed
		PXBufferType Type;
	}
	PXBuffer;

	PXPublic void PXBufferConstruct(PXBuffer* const pxBuffer, void* buffer, PXSize size, PXBufferType pxBufferType);
	PXPublic void PXBufferDestruct(PXBuffer* const pxBuffer);

#ifdef __cplusplus
}
#endif



#endif