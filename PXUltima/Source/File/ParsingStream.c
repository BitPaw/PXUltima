#include "ParsingStream.h"

#include <OS/OSVersion.h>
#include <Memory/Memory.h>
#include <Math/Math.h>
#include <Text/Text.h>

#include <iso646.h>
#include <stdarg.h>
#include <stdio.h>

#define IsEndOfString(c) (c == '\0')
#define IsEmptySpace(c) (c == ' ')
#define IsEndOfLineCharacter(c) (c == '\r' || c == '\n')

#if defined(OSUnix)
#define PrintSVN vsnprintf
#elif defined(OSWindows)
#define PrintSVN vsprintf_s
#endif

void ParsingStreamConstruct(ParsingStream* const parsingStream, void* data, const size_t dataSize)
{
	parsingStream->Data = data;
	parsingStream->DataSize = dataSize;
	parsingStream->DataCursor = 0;
}

void ParsingStreamClear(ParsingStream* const parsingStream)
{
	parsingStream->Data = 0;
	parsingStream->DataSize = 0;
	parsingStream->DataCursor = 0;
}

size_t ParsingStreamRemainingSize(ParsingStream* const parsingStream)
{
	return parsingStream->DataSize - parsingStream->DataCursor;
}

unsigned char ParsingStreamIsAtEnd(ParsingStream* const parsingStream)
{
	return parsingStream->DataCursor == parsingStream->DataSize;
}

unsigned char* ParsingStreamCursorPosition(ParsingStream* const parsingStream)
{
	return parsingStream->Data + parsingStream->DataCursor;
}

void ParsingStreamCursorToBeginning(ParsingStream* const parsingStream)
{
	parsingStream->DataCursor = 0;
}

void ParsingStreamCursorAdvance(ParsingStream* const parsingStream, const size_t steps)
{
	parsingStream->DataCursor += steps; // Check overflow
}

void ParsingStreamCursorRewind(ParsingStream* const parsingStream, const size_t steps)
{
	parsingStream->DataCursor -= steps; // Check underflow
}

void ParsingStreamCursorToEnd(ParsingStream* const parsingStream)
{
	parsingStream->DataCursor = parsingStream->DataSize;
}

size_t ParsingStreamReadNextLineInto(ParsingStream* const parsingStream, void* exportBuffer, const size_t exportBufferSize)
{
	ParsingStreamSkipEndOfLineCharacters(parsingStream);

	const size_t dataPositionBefore = parsingStream->DataCursor;

	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if(!advance)
		{
			break;
		}

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}

	const unsigned char* dataPoint = parsingStream->Data + dataPositionBefore;
	const size_t dataPositionAfter = parsingStream->DataCursor;
	const size_t length = dataPositionAfter - dataPositionBefore;

	if(length == 0)
	{
		return 0;
	}

	TextCopyA(dataPoint, length, exportBuffer, length);

	ParsingStreamSkipEndOfLineCharacters(parsingStream);

	return length;
}

size_t ParsingStreamSkipEndOfLineCharacters(ParsingStream* const parsingStream)
{
	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if(!advance)
		{
			break;
		}

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}
}

size_t ParsingStreamSkipEmptySpace(ParsingStream* const parsingStream)
{
	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = IsEmptySpace(*data);

		if(!advance)
		{
			break;
		}

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}
}

size_t ParsingStreamSkipBlock(ParsingStream* const parsingStream)
{
	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = !IsEndOfString(*data) and !IsEmptySpace(*data);

		if(!advance)
		{
			break;
		}

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}

	ParsingStreamSkipEmptySpace(parsingStream);
}

size_t ParsingStreamSkipLine(ParsingStream* const parsingStream)
{
	const size_t positionBefore = parsingStream->DataCursor;

	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if(!advance)
		{
			break;
		}

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}

	ParsingStreamSkipEndOfLineCharacters(parsingStream);

	const size_t skippedBytes = parsingStream->DataCursor - positionBefore;

	return skippedBytes;
}

size_t ParsingStreamReadC(ParsingStream* const parsingStream, char* value)
{
	return ParsingStreamReadCU(parsingStream, (unsigned char*)value);
}

size_t ParsingStreamReadCU(ParsingStream* const parsingStream, unsigned char* value)
{
	const size_t sizeOfChar = sizeof(char);
	const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
	const unsigned char character = *data;

	*value = character;

	ParsingStreamCursorAdvance(parsingStream, sizeOfChar);

	return sizeOfChar;
}

size_t ParsingStreamReadS(ParsingStream* const parsingStream, short* value, const Endian endian)
{
	return ParsingStreamReadSU(parsingStream, (unsigned short*)value, endian);
}

size_t ParsingStreamReadSU(ParsingStream* const parsingStream, unsigned short* value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned short);
	const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
	const unsigned short* dataValue = (unsigned short*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	ParsingStreamCursorAdvance(parsingStream, dataSize);

	return dataSize;
}

size_t ParsingStreamReadI(ParsingStream* const parsingStream, int* value, const Endian endian)
{
	return ParsingStreamReadIU(parsingStream, (unsigned int*)value, endian);
}

size_t ParsingStreamReadIU(ParsingStream* const parsingStream, unsigned int* value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned int);
	const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
	const unsigned int* dataValue = (unsigned int*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	ParsingStreamCursorAdvance(parsingStream, dataSize);

	return dataSize;
}

size_t ParsingStreamReadLL(ParsingStream* const parsingStream, long long* value, const Endian endian)
{
	return ParsingStreamReadLLU(parsingStream, (unsigned long long*)value, endian);
}

size_t ParsingStreamReadLLU(ParsingStream* const parsingStream, unsigned long long* value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned long long);
	const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
	const unsigned long long* dataValue = (unsigned long long*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	ParsingStreamCursorAdvance(parsingStream, dataSize);

	return dataSize;
}

size_t ParsingStreamReadD(ParsingStream* const parsingStream, void* value, const size_t length)
{
	const unsigned char* currentPosition = ParsingStreamCursorPosition(parsingStream);
	const size_t readableSize = ParsingStreamRemainingSize(parsingStream);
	const size_t copyedBytes = MemoryCopy(currentPosition, readableSize, value, length);

	ParsingStreamCursorAdvance(parsingStream, copyedBytes);

	return copyedBytes;
}

size_t ParsingStreamRead(ParsingStream* const parsingStream, const void* format, const size_t length, ...)
{
	return 0;
}

void ParsingStreamReadUntil(ParsingStream* const parsingStream, void* value, const size_t length, const char character)
{
	const unsigned char* currentPosition = ParsingStreamCursorPosition(parsingStream);

	size_t lengthCopy = 0;

	while(!ParsingStreamIsAtEnd(parsingStream))
	{
		const unsigned char* data = ParsingStreamCursorPosition(parsingStream);
		const unsigned char advance = *data != character && length <= lengthCopy;

		if(!advance)
		{
			break;
		}

		++lengthCopy;

		ParsingStreamCursorAdvance(parsingStream, 1u);
	}

	const size_t readableSize = ParsingStreamRemainingSize(parsingStream);

	MemoryCopy(currentPosition, readableSize, value, lengthCopy);
}

unsigned char ParsingStreamReadAndCompare(ParsingStream* const parsingStream, const void* value, const size_t length)
{
	const unsigned char* currentPosition = ParsingStreamCursorPosition(parsingStream);
	const size_t readableSize = ParsingStreamRemainingSize(parsingStream);

	const unsigned char result = MemoryCompare(currentPosition, readableSize, value, length);

	if(result)
	{
		ParsingStreamCursorAdvance(parsingStream, length);
	}

	return result;
}

size_t ParsingStreamWriteC(ParsingStream* const parsingStream, const char value)
{
	return ParsingStreamWriteCU(parsingStream, value);
}

size_t ParsingStreamWriteCU(ParsingStream* const parsingStream, const unsigned char value)
{
	const size_t dataSize = sizeof(unsigned char);

	ParsingStreamWriteD(parsingStream, &value, dataSize);

	return dataSize;
}

size_t ParsingStreamWriteS(ParsingStream* const parsingStream, const short value, const Endian endian)
{
	return ParsingStreamWriteSU(parsingStream, value, endian);
}

size_t ParsingStreamWriteSU(ParsingStream* const parsingStream, const unsigned short value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned short);
	unsigned short dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	ParsingStreamWriteD(parsingStream, &dataValue, dataSize);

	return dataSize;
}

size_t ParsingStreamWriteI(ParsingStream* const parsingStream, const int value, const Endian endian)
{
	return ParsingStreamWriteIU(parsingStream, value, endian);
}

size_t ParsingStreamWriteIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned int);
	unsigned int dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	ParsingStreamWriteD(parsingStream, &dataValue, dataSize);

	return dataSize;
}

size_t ParsingStreamWriteLL(ParsingStream* const parsingStream, const long long value, const Endian endian)
{
	return ParsingStreamWriteLU(parsingStream, value, endian);
}

size_t ParsingStreamWriteLU(ParsingStream* const parsingStream, const unsigned long long value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned long long);
	unsigned long long dataValue = value;

	EndianSwap(value, dataSize, EndianCurrentSystem, endian);

	ParsingStreamWriteD(parsingStream, &dataValue, dataSize);

	return dataSize;
}

size_t ParsingStreamWriteD(ParsingStream* const parsingStream, const void* value, const size_t length)
{
	const size_t writableSize = ParsingStreamRemainingSize(parsingStream);
	unsigned char* currentPosition = ParsingStreamCursorPosition(parsingStream);

	const size_t copyedBytes = MemoryCopy(value, length, currentPosition, writableSize);

	ParsingStreamCursorAdvance(parsingStream, copyedBytes);

	return copyedBytes;
}

size_t ParsingStreamWriteFill(ParsingStream* const parsingStream, const unsigned char value, const size_t length)
{
	const size_t writableSize = ParsingStreamRemainingSize(parsingStream);
	unsigned char* beforePosition = ParsingStreamCursorPosition(parsingStream);
	const size_t write = MathMinimumI(writableSize, length);

	for (size_t i = 0; i < write; ++i)
	{
		beforePosition[i] = value;
	}

	ParsingStreamCursorAdvance(parsingStream, write);

	return write;
}

size_t ParsingStreamWrite(ParsingStream* const parsingStream, const char* format, ...)
{
	const unsigned char* currentPosition = ParsingStreamCursorPosition(parsingStream);

	va_list args;
	va_start(args, format);

	const size_t writableSize = ParsingStreamRemainingSize(parsingStream);
	const int writtenBytes = PrintSVN(currentPosition, writableSize, format, args);

	va_end(args);

	{
		const unsigned char sucessful = writtenBytes >= 0;

		if(!sucessful)
		{
			return 0;
		}
	}

	ParsingStreamCursorAdvance(parsingStream, writtenBytes);

	return writtenBytes;
}

size_t ParsingStreamWriteAtIU(ParsingStream* const parsingStream, const unsigned int value, const Endian endian, const size_t index)
{
	const size_t positionBefore = parsingStream->DataCursor; // save current position
	
	parsingStream->DataCursor = index; // jump to offset

	ParsingStreamWriteIU(parsingStream, value, endian); // Length

	parsingStream->DataCursor = positionBefore; // Reset old position

	return 4u;
}
