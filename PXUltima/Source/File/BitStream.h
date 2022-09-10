#ifndef BitStreamInclude
#define BitStreamInclude

#include <Format/Type.h>

#include "Endian.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct BitStream_
	{
		unsigned char* Data;
		size_t DataSize;
		size_t DataCursor;
		size_t BitOffset;
	}
	BitStream;

	CPublic void BitStreamConstruct(BitStream* bitStream, void* data, const size_t dataSize);

	CPublic size_t BitStreamRemainingSize(BitStream* bitStream);
	CPublic unsigned char* BitStreamCursorPosition(BitStream* bitStream);

	//CPublic void BitStreamCursorPositionSet(BitStream* bitStream, unsigned char* startAdress, unsigned int dataLengh);
	CPublic void BitStreamSkipBitsToNextByte(BitStream* bitStream);
	CPublic size_t BitStreamCursorMoveInBytes(BitStream* bitStream, const size_t amountOfBytes);
	CPublic size_t BitStreamCursorMoveInBits(BitStream* bitStream, const size_t amountOfBits);
	CPublic void BitStreamAllign(BitStream* bitStream);

	CPublic size_t BitStreamRead(BitStream* bitStream, const size_t amountOfBits);

	CPublic size_t BitStreamWrite(BitStream* const bitStream, const size_t bitData, const size_t amountOfBits);

	CPublic size_t BitStreamReadFullByte(BitStream* bitStream);
	CPublic size_t BitStreamPeek(BitStream* bitStream, const size_t amountOfBits);

#endif

#ifdef __cplusplus
}
#endif