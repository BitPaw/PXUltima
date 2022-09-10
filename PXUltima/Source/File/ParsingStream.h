#ifndef ParsingStreamInclude
#define ParsingStreamInclude

#include <Format/Type.h>

#include "Endian.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct ParsingStream_
	{
		unsigned char* Data;
		size_t DataSize;
		size_t DataCursor;
	}
	ParsingStream;


	CPublic void ParsingStreamConstruct(ParsingStream* const parsingStream, void* data, const size_t dataSize);
	//CPublic void ParsingStreamAllocate(ParsingStream* parsingStream, const size_t dataSize);
	CPublic void ParsingStreamClear(ParsingStream* const parsingStream);

	CPublic size_t ParsingStreamRemainingSize(ParsingStream* const parsingStream);
	CPublic unsigned char ParsingStreamIsAtEnd(ParsingStream* const parsingStream);

	CPublic unsigned char* ParsingStreamCursorPosition(ParsingStream* const parsingStream);
	CPublic void ParsingStreamCursorToBeginning(ParsingStream* const parsingStream);
	CPublic void ParsingStreamCursorAdvance(ParsingStream* const parsingStream, const size_t steps);
	CPublic void ParsingStreamCursorRewind(ParsingStream* const parsingStream, const size_t steps);
	CPublic void ParsingStreamCursorToEnd(ParsingStream* const parsingStream);



	CPublic size_t ParsingStreamReadNextLineInto(ParsingStream* const parsingStream, void* exportBuffer, const size_t exportBufferSize);


	CPublic size_t ParsingStreamSkipEndOfLineCharacters(ParsingStream* const  parsingStream);
	CPublic size_t ParsingStreamSkipEmptySpace(ParsingStream* const parsingStream);

	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	CPublic size_t ParsingStreamSkipBlock(ParsingStream* const parsingStream);
	CPublic size_t ParsingStreamSkipLine(ParsingStream* const parsingStream);


	CPublic size_t ParsingStreamReadC(ParsingStream* const parsingStream, char* value);
	CPublic size_t ParsingStreamReadCU(ParsingStream* const parsingStream, unsigned char* value);
	CPublic size_t ParsingStreamReadS(ParsingStream* const parsingStream, short* value, const Endian endian);
	CPublic size_t ParsingStreamReadSU(ParsingStream* const parsingStream, unsigned short* value, const Endian endian);
	CPublic size_t ParsingStreamReadI(ParsingStream* const parsingStream, int* value, const  Endian endian);
	CPublic size_t ParsingStreamReadIU(ParsingStream* const parsingStream, unsigned int* value, const Endian endian);
	CPublic size_t ParsingStreamReadLL(ParsingStream* const parsingStream, long long* value, const Endian endian);
	CPublic size_t ParsingStreamReadLLU(ParsingStream* const parsingStream, unsigned long long* value, const  Endian endian);
	CPublic size_t ParsingStreamReadD(ParsingStream* const parsingStream, void* value, const size_t length);
	CPublic size_t ParsingStreamRead(ParsingStream* const parsingStream, const void* format, const size_t length, ...);


	CPublic void ParsingStreamReadUntil(ParsingStream* const parsingStream, void* value, const size_t length, const char character);

	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//size_t ReadSafe(Byte__* value, const size_t length);


	unsigned char ParsingStreamReadAndCompare(ParsingStream* const parsingStream, const void* value, const size_t length);
	//unsigned char ParsingStreamReadAndCompareC(ParsingStream* parsingStream, const char value);
	//unsigned char ParsingStreamReadAndCompareIU(ParsingStream* parsingStream, const unsigned int value);

	CPublic size_t ParsingStreamWriteC(ParsingStream* const parsingStream, const char value);
	CPublic size_t ParsingStreamWriteCU(ParsingStream* const parsingStream, const unsigned char value);
	CPublic size_t ParsingStreamWriteS(ParsingStream* const parsingStream, const short value, const Endian endian);
	CPublic size_t ParsingStreamWriteSU(ParsingStream* const parsingStream, const unsigned short value, const Endian endian);
	CPublic size_t ParsingStreamWriteI(ParsingStream* const  parsingStream, const int value, const Endian endian);
	CPublic size_t ParsingStreamWriteIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian);
	CPublic size_t ParsingStreamWriteLL(ParsingStream* const parsingStream, const long long value, const Endian endian);
	CPublic size_t ParsingStreamWriteLU(ParsingStream* const parsingStream, const unsigned long long value, const Endian endian);
	CPublic size_t ParsingStreamWriteD(ParsingStream* const parsingStream, const void* value, const size_t length);
	CPublic size_t ParsingStreamWriteFill(ParsingStream* const parsingStream, const unsigned char value, const size_t length);
	CPublic size_t ParsingStreamWrite(ParsingStream* const parsingStream, const char* format, ...);


	CPublic size_t ParsingStreamWriteAtIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian, const size_t index);
#endif

#ifdef __cplusplus
}
#endif