#ifndef ParsingStreamInclude
#define ParsingStreamInclude

#include <stddef.h>

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


	extern void ParsingStreamConstruct(ParsingStream* const parsingStream, void* data, const size_t dataSize);
	//extern void ParsingStreamAllocate(ParsingStream* parsingStream, const size_t dataSize);
	extern void ParsingStreamClear(ParsingStream* const parsingStream);

	extern size_t ParsingStreamRemainingSize(ParsingStream* const parsingStream);
	extern unsigned char ParsingStreamIsAtEnd(ParsingStream* const parsingStream);

	extern unsigned char* ParsingStreamCursorPosition(ParsingStream* const parsingStream);
	extern void ParsingStreamCursorToBeginning(ParsingStream* const parsingStream);
	extern void ParsingStreamCursorAdvance(ParsingStream* const parsingStream, const size_t steps);
	extern void ParsingStreamCursorRewind(ParsingStream* const parsingStream, const size_t steps);
	extern void ParsingStreamCursorToEnd(ParsingStream* const parsingStream);



	extern size_t ParsingStreamReadNextLineInto(ParsingStream* const parsingStream, void* exportBuffer, const size_t exportBufferSize);


	extern size_t ParsingStreamSkipEndOfLineCharacters(ParsingStream* const  parsingStream);
	extern size_t ParsingStreamSkipEmptySpace(ParsingStream* const parsingStream);

	// Skips current block thats seperated with spaces.
	// Example : '|' is the cursor.
	// Before : |AAAA  BBBB CCC
	// After :  AAAA |BBBB CCC
	extern size_t ParsingStreamSkipBlock(ParsingStream* const parsingStream);
	extern size_t ParsingStreamSkipLine(ParsingStream* const parsingStream);


	extern size_t ParsingStreamReadC(ParsingStream* const parsingStream, char* value);
	extern size_t ParsingStreamReadCU(ParsingStream* const parsingStream, unsigned char* value);
	extern size_t ParsingStreamReadS(ParsingStream* const parsingStream, short* value, const Endian endian);
	extern size_t ParsingStreamReadSU(ParsingStream* const parsingStream, unsigned short* value, const Endian endian);
	extern size_t ParsingStreamReadI(ParsingStream* const parsingStream, int* value, const  Endian endian);
	extern size_t ParsingStreamReadIU(ParsingStream* const parsingStream, unsigned int* value, const Endian endian);
	extern size_t ParsingStreamReadLL(ParsingStream* const parsingStream, long long* value, const Endian endian);
	extern size_t ParsingStreamReadLLU(ParsingStream* const parsingStream, unsigned long long* value, const  Endian endian);
	extern size_t ParsingStreamReadD(ParsingStream* const parsingStream, void* value, const size_t length);
	extern size_t ParsingStreamRead(ParsingStream* const parsingStream, const void* format, const size_t length, ...);


	extern void ParsingStreamReadUntil(ParsingStream* const parsingStream, void* value, const size_t length, const char character);

	// Additional check how many bytes are read.
	// Slower version of Read(), this function can't fail.
	//size_t ReadSafe(Byte__* value, const size_t length);


	unsigned char ParsingStreamReadAndCompare(ParsingStream* const parsingStream, const void* value, const size_t length);
	//unsigned char ParsingStreamReadAndCompareC(ParsingStream* parsingStream, const char value);
	//unsigned char ParsingStreamReadAndCompareIU(ParsingStream* parsingStream, const unsigned int value);

	extern size_t ParsingStreamWriteC(ParsingStream* const parsingStream, const char value);
	extern size_t ParsingStreamWriteCU(ParsingStream* const parsingStream, const unsigned char value);
	extern size_t ParsingStreamWriteS(ParsingStream* const parsingStream, const short value, const Endian endian);
	extern size_t ParsingStreamWriteSU(ParsingStream* const parsingStream, const unsigned short value, const Endian endian);
	extern size_t ParsingStreamWriteI(ParsingStream* const  parsingStream, const int value, const Endian endian);
	extern size_t ParsingStreamWriteIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian);
	extern size_t ParsingStreamWriteLL(ParsingStream* const parsingStream, const long long value, const Endian endian);
	extern size_t ParsingStreamWriteLU(ParsingStream* const parsingStream, const unsigned long long value, const Endian endian);
	extern size_t ParsingStreamWriteD(ParsingStream* const parsingStream, const void* value, const size_t length);
	extern size_t ParsingStreamWriteFill(ParsingStream* const parsingStream, const unsigned char value, const size_t length);
	extern size_t ParsingStreamWrite(ParsingStream* const parsingStream, const char* format, ...);


	extern size_t ParsingStreamWriteAtIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian, const size_t index);
#endif

#ifdef __cplusplus
}
#endif