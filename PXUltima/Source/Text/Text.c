#include "Text.h"

#include <OS/OSVersion.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define PXTextAssertEnable 0

#if PXTextAssertEnable
#include <assert.h>
#endif

#if OSUnix
#define PrintSN snprintf
#elif OSWindows
#define PrintSN sprintf_s
#endif

PXSize TextToIntA(const char* string, const PXSize dataSize, int* number)
{
	int accumulator = 0;
	PXSize index = 0;
	unsigned char isNegative = 0;

	if(!string)
	{
		return 0;
	}

	if(string[0] == '-')
	{
		index++;
		isNegative = 1u;
	}

	while(string[index] != '\0')
	{
		char character = string[index];
		char isValidCharacter = (character >= '0' && character <= '9');
		int numberElement = character - '0';

		if(!isValidCharacter)
		{
			break;
		}

		accumulator *= 10; // "Shft number to left" Example 12 -> 120
		accumulator += numberElement; // ASCII character to actual number.

		++index;
	}

	if(isNegative)
	{
		accumulator *= -1;
	}

	*number = accumulator;

	return index;
}

PXSize TextToIntW(const wchar_t* string, const PXSize dataSize, int* number)
{
	PXSize index = 0;
	unsigned char isNegative = 0;
	int accumulator = 0;

	if(!string)
	{
		return 0;
	}

	if(string[0] == '-')
	{
		index++;
		isNegative = 1u;
	}

	while(string[index] != '\0')
	{
		char character = string[index];
		char isValidCharacter = (character >= '0' && character <= '9');
		int numberElement = character - '0';

		if(!isValidCharacter)
		{
			break;
		}

		accumulator *= 10; // "Shft number to left" Example 12 -> 120
		accumulator += numberElement; // ASCII character to actual number.

		++index;
	}

	if(isNegative)
	{
		accumulator *= -1;
	}

	*number = accumulator;

	return index;
}

PXSize TextToBoolA(const char* string, const PXSize dataSize, unsigned char* number)
{
	switch(string[0])
	{
		default:
		case '0':
		case 'F':
		case 'f':
			*number = 0;
			break;

		case '1':
		case 'T':
		case 't':
			*number = 1u;
			break;
	}

	return 1u;
}

PXSize TextToBoolW(const wchar_t* string, const PXSize dataSize, unsigned char* number)
{
	return 0;
}

PXSize TextToFloatA(const char* string, const PXSize dataSize, float* number)
{
	double x = 0;

	const PXSize readBytes = TextToDoubleA(string, dataSize, &x);

	*number = (float)x;

	return readBytes;
}

PXSize TextToFloatW(const wchar_t* string, const PXSize dataSize, float* number)
{
	return 0;
}

PXSize TextToDoubleA(const char* string, const PXSize dataSize, double* number)
{
	int accumulator = 0;
	PXSize digitsAfterDot = 1;
	PXSize index = 0;
	unsigned char isNegative = 0;
	unsigned char isWholeNumberChunk = 1;

	

	if(!string)
	{
		return 0;
	}

	if(string[0] == '-')
	{
		index++;
		isNegative = 1;
	}

	for(; string[index] != '\0'; ++index)
	{
		const char character = string[index];
		const unsigned char isDot = character == '.';
		const unsigned char isValidCharacter = (character >= '0' && character <= '9') || isDot;
		const int numberElement = character - '0';

		if(!isValidCharacter)
		{
			break;
		}

		// Trigger when we switch to after dot
		if(isDot && isWholeNumberChunk)
		{
			isWholeNumberChunk = 0;
			continue;
		}

		accumulator *= 10; // "Shft number to left" Example 12 -> 120
		accumulator += numberElement; // ASCII character to actual number.

		if(!isWholeNumberChunk)
		{
			digitsAfterDot *= 10;
		}
	}

	if(isNegative)
	{
		accumulator *= -1;
	}

	//double stdResult = std::strtof(string, 0); // STD Method

	// Calculate
	{
		const double a = accumulator;
		const double b = digitsAfterDot;
		const double c = a / b;

		*number = c;
	}

	return index;
}

PXSize TextToDoubleW(const wchar_t* string, const PXSize dataSize, double* number)
{
	return 0;
}

PXSize TextAppendW(wchar_t* const dataString, const PXSize dataStringSize, const wchar_t* const appaendString, const PXSize appaendStringSize)
{
	const PXSize length = TextLengthW(dataString, dataStringSize);
	const PXSize lengthAdded = TextCopyW(appaendString, appaendStringSize, &dataString[length], dataStringSize - length);
	const PXSize fullSize = length + lengthAdded;

	return fullSize;
}

PXSize TextClearA(char* string, const PXSize stringSize)
{
	MemorySet(string, stringSize * sizeof(char), 0x00);

	return stringSize;
}

PXSize TextClearW(wchar_t* string, const PXSize stringSize)
{
	MemorySet(string, stringSize * sizeof(wchar_t), 0x00);

	return stringSize;
}

PXSize TextLengthA(const char* string, const PXSize stringSize)
{
	PXSize index = 0;

	for(; (string[index] != '\0') && (index < stringSize); ++index);

	return index;
}

PXSize TextLengthW(const wchar_t* string, const PXSize stringSize)
{
	PXSize index = 0;

	for(; (string[index] != L'\0') && (index < stringSize); ++index);

	return index;
}

PXSize TextLengthUntilA(const char* string, const PXSize stringSize, const char character)
{
	PXSize index = 0;

	for(; (index < stringSize) && (string[index] != '\0') && (string[index] != character); ++index);

	return index;
}

PXSize TextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character)
{
	return 0;
}

PXSize TextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
	const PXSize minLength = MathMinimumIU(sourceLength, destinationLength);
	PXSize i = 0;

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#else
	if (!(destination && source))
	{
		return 0;
	}
#endif

	for(; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

PXSize TextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
	const PXSize minLength = MathMinimum(sourceLength, destinationLength);
	const PXBool validCall = source && destination && minLength;
	PXSize i = 0;

	if (!validCall)
	{
		return 0;
	}

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#endif

	for(; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

PXSize TextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
	const PXSize minLength = MathMinimum(sourceLength, destinationLength);
	PXSize i = 0;

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#endif

	for(; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = UnicodeToASCII(source[i]);
	}

	destination[i] = '\0';

	return i;
}

PXSize TextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
	const PXSize minLength = MathMinimumIU(sourceLength, destinationLength);
	PXSize i = 0;

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#endif

	for(; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

void TextToUTFConvert(const PXSize symbol, PXByte* dataBuffer, PXSize* dataBufferSize)
{
	const char utfTrailBytesAmount =
		0u * (symbol >= 0 && symbol < 128u) + // 7-bit
		1u * (symbol >= 128u && symbol < 2048u) +  // 11-Bit
		2u * (symbol >= 2048u && symbol < 65535u) + // 16-Bit
		3u * (symbol >= 65535u && symbol < 2097152u); // 21-Bit

	switch (utfTrailBytesAmount)
	{
		case 0u:
		{
			dataBuffer[0] = (PXByte)symbol;  // 0xxxxxxx - 
			break;
		}
		case 1u:
		{
			dataBuffer[0] = 0b11000000 | ((symbol & (0b11111 << 6u)) >> 6u);  // 110xxxxx
			break;
		}
		case 2u:
		{
			dataBuffer[0] = 0b11100000 | ((symbol & (0b1111 << 12u)) >> 12u); // 1110xxxx
			break;
		}
		case 3u:
		{
			dataBuffer[0] = 0b11110000 | ((symbol & (0b111 << 18u)) >> 18u); // 1110xxxx
			break;
		}
	}

	for (PXSize i = 1; i < (PXSize)utfTrailBytesAmount; ++i)
	{
		dataBuffer[i] = 0b10000000 | ((symbol & (0b111111 << 6u * i)) >> 6u * i);
	}

	*dataBufferSize = utfTrailBytesAmount+1u;
}

PXSize TextCopyAU(const PXTextASCII source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength)
{
	const PXSize minimalSize = MathMinimumIU(sourceLength, destinationLength);
	PXSize destinationIndex = 0;

	if (!(source && destinationLength))
	{
		return 0;
	}

	for (PXSize sourceIndex = 0; (sourceIndex < minimalSize) && (destinationIndex < destinationLength) && (source[sourceIndex] != '\0'); ++sourceIndex)
	{
		const char wideCharacter = source[sourceIndex];
		PXByte buffer[4];
		PXSize size = 0;

		TextToUTFConvert(wideCharacter, buffer, &size);

		destinationIndex += MemoryCopy(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
	}

	destination[destinationIndex++] = 0u;

	return destinationIndex;
}



PXSize TextCopyWU(const PXTextUNICODE source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength)
{
#if 1
	const PXSize minimalSize = MathMinimumIU(sourceLength, destinationLength);
	PXSize destinationIndex = 0;

	if (!(source && destinationLength))
	{
		return 0;
	}

	for (PXSize sourceIndex = 0; (sourceIndex < minimalSize) && (destinationIndex < destinationLength) && (source[sourceIndex] != '\0'); ++sourceIndex)
	{
		const PXCharUNICODE wideCharacter = source[sourceIndex];
		PXByte buffer[4];
		PXSize size = 0;

		TextToUTFConvert(wideCharacter, buffer, &size);

		destinationIndex += MemoryCopy(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
	}

	destination[destinationIndex++] = 0u;

	return destinationIndex;

#else
	const PXSize copyedBytes = wcstombs(destination, source, destinationLength);

	return copyedBytes;
#endif
}

PXSize TextCountA(const char* text, const PXSize textSize, const char target)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (text[index] != '\0'); ++index)
		samecounter += target == text[index];

	return samecounter;
}

PXSize TextCountW(const wchar_t* text, const PXSize textSize, const wchar_t target)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (text[index] != '\0'); ++index)
		samecounter += target == text[index];

	return samecounter;
}

PXSize TextCountUntilA(const char* text, const PXSize textSize, const char target, const char stopAt)
{
	PXSize samecounter = 0;

	for(PXSize index = 0; (index < textSize) && (text[index] != '\0' && text[index] != stopAt); ++index)
		samecounter += target == text[index];

	return samecounter;
}

PXSize TextCountUntilW(const wchar_t* text, const PXSize textSize, const wchar_t target, const wchar_t stopAt)
{
	PXSize samecounter = 0;

	for(PXSize index = 0; (index < textSize) && (text[index] != '\0' && text[index] != stopAt); ++index)
		samecounter += target == text[index];

	return samecounter;
}

PXBool TextCompareA(const char* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimumIU(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	const PXBool stillHasDatainA = textSize < aSize;
	const PXBool stillHasDatainB = textSize < bSize;

	// Check if data is just empty
	{
		const PXBool isEmptyData = stillHasDatainA && stillHasDatainB;

		if (isEmptyData)
		{
			return (index == samecounter); // End check
		}
	}

	if (stillHasDatainA)
	{
		samecounter += a[index] == '\0';
		++index; // keep for comparsion
	}

	if (stillHasDatainB)
	{
		samecounter += b[index] == '\0';
		++index; // keep for comparsion
	}

	return (index == samecounter);
}

PXBool TextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

PXBool TextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

PXBool TextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

char TextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char TextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char TextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = MathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore((wchar_t)a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char TextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for(; (index < aSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], (wchar_t)b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char* TextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize)
{
	const char* source = 0;
	unsigned char found = 0;

	for(PXSize i = 0; (data[i] != '\0') && (i + targetSize) < dataSize && !found; i++)
	{
		source = data + i;
		found = TextCompareA(source, targetSize-i, target, targetSize);
	}

	return (char*)(found * (PXSize)source);
}

PXSize TextFindFirstA(const char* string, const PXSize dataSize, const char character)
{
	PXBool found = 0;

	PXSize index = 0;

	for (; index < dataSize && !found; ++index)
	{
		found = character == string[index];
	}

	return found ? index -1 : TextIndexNotFound;
}

PXSize TextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character)
{
	PXBool found = 0;
	PXSize i = TextLengthW(string, dataSize);

	for (; i > 0 && !found; --i)
	{
		found = character == string[i];
	}

	++i;

	return found ? i + 1 : TextIndexNotFound;
}

PXSize TextFindLastA(const char* string, const PXSize dataSize, const char character)
{
	PXBool found = 0;
	PXSize i = TextLengthA(string, dataSize);

	for(; i > 0 && !found; --i)
	{
		found = character == string[i];
	}

	++i;

	return found ? i+1 : TextIndexNotFound;
}

PXSize TextFindLastW(const wchar_t* string, const PXSize dataSize, const wchar_t character)
{
	unsigned char found = 0;
	PXSize i = TextLengthW(string, dataSize);

	for(; i > 0 && !found; --i)
	{
		found = character == string[i];
	}

	return found ? i+1 : TextIndexNotFound;
}

void TextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character)
{
	PXSize index = TextFindFirstA(string, dataSize, character);

	if(index != -1)
	{
		string[index] = '\0';
	}
}

void TextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...)
{
	va_list args;
	va_start(args, syntax);

	PXSize offsetData = 0;

	for(PXSize commandIndex = 0; syntax[commandIndex] != '\0'; ++commandIndex)
	{
		const char commandKey = syntax[commandIndex];
		//const bool commandIsNumber = commandKey == 'i' || commandKey == 'f' || commandKey == 'u';

		PXSize offsetLength = offsetData;

		// Get length until new block
		{
			while(offsetLength < bufferSize)
			{
				const char symbol = buffer[offsetLength];
				const unsigned char stop = symbol == '\n' || symbol == '\0' || symbol == ' ';
				//const bool skip = symbol == '\n'; 	if(commandIsNumber && current == '/' || current == ' ' || finished)

				if(stop)
				{
					break; // End of string
				}

				++offsetLength;
			}
		}

		switch(commandKey)
		{
			case '§':
			{
				++offsetData;
				break;
			}
			case 's':
			{
				char* destination = va_arg(args, char*);
				const char* source = buffer + offsetData;

				const PXSize readBytes = TextCopyA(source, offsetLength, destination, -1);

				offsetData += readBytes;

				break;
			}
			case 'i':
			case 'd':
			case 'u':
			{
				int* i = va_arg(args, int*);
				const char* source = buffer + offsetData;

				const PXSize readBytes = TextToIntA(source, offsetLength, i);

#if PXTextAssertEnable
				assert(readBytes);
#endif				

				offsetData += readBytes;

				break;
			}
			case 'f':
			{
				float* number = va_arg(args, float*);
				const char* source = buffer + offsetData;

				const PXSize readBytes = TextToFloatA(source, offsetLength, number);

#if PXTextAssertEnable
				assert(readBytes);
#endif			

				offsetData += readBytes;

				break;
			}
			case 'c':
			{
				char* character = va_arg(args, char*);
				const char* source = buffer + offsetData;

				*character = *source;

				break;
			}
			default:
				break;
		}

		while(offsetData < bufferSize)
		{
			const char symbol = buffer[offsetData];
			const unsigned char stop = !(symbol == ' ' || symbol == '\0');

			if(stop)
			{
				break;
			}

			++offsetData;
		}
	}

	va_end(args);
}

void TextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize)
{
	unsigned char finished = 0;
	unsigned char foundItem = 0;
	PXSize foundTargets = 0;

	for(PXSize i = 0; (i < stringSize) && (string[i] != '\0') && !finished; ++i)
	{
		foundItem = 0;

		for(PXSize t = 0; (t < parsingTokenListSize) && (string[i] != ' ') && !foundItem; t++)
		{
			const ParsingTokenA* parsingToken = &parsingTokenList[t];
			const char* targetString = parsingToken->String;
			const PXSize targetStringSize = TextLengthA(targetString, -1);
			const char* sourceString = string + i;

			foundItem = TextCompareA(sourceString, stringSize, targetString, targetStringSize); // Compare whole word

			if(foundItem)
			{
				PXSize lengthTag = TextLengthUntilA(sourceString, stringSize, '=');
				const char* valueString = sourceString + lengthTag + 1;

				i += lengthTag + 1;

				(*parsingToken->Value) = valueString;

				for(; (string[i] != '\0') && string[i] != ' '; i++); // Skip data

				++foundTargets;
			}
		}

		finished = foundTargets == parsingTokenListSize;
	}
}

PXSize TextFromIntA(const int number, char* string, const PXSize dataSize)
{
	int bytesWritten = PrintSN(string, dataSize, "%i", &number);

	return bytesWritten;
}

PXSize TextFromIntW(const int number, wchar_t* string, const PXSize dataSize)
{
	int bytesWritten = PrintSN(string, dataSize, "%i", &number);

	return bytesWritten;
}

PXSize TextFromBoolA(const unsigned char number, char* string, const PXSize dataSize)
{
	string[0] = number ? '1' : '0';

	return 1u;
}

PXSize TextFromBoolW(const unsigned char number, wchar_t* string, const PXSize dataSize)
{
	string[0] = number ? '1' : '0';

	return 1u;
}

PXSize TextFromFloatA(const float number, char* string, const PXSize dataSize)
{
#if OSUnix
	int bytesWritten = snprintf(string, dataSize, "%f", number);
#elif OSWindows
	int bytesWritten = sprintf_s(string, dataSize, "%f", number);
#endif
	return bytesWritten;
}

PXSize TextFromFloatW(const float number, wchar_t* string, const PXSize dataSize)
{
#if OSUnix
	int bytesWritten = snprintf(string, dataSize, "%f", number);
#elif OSWindows
	int bytesWritten = sprintf_s(string, dataSize, "%f", number);
#endif

	return bytesWritten;
}

PXSize TextFromDoubleA(const double number, char* string, const PXSize dataSize)
{
#if OSUnix
	int bytesWritten = snprintf(string, dataSize, "%li", number);
#elif OSWindows
	int bytesWritten = sprintf_s(string, dataSize, "%lf", number);
#endif
	return bytesWritten;
}

PXSize TextFromDoubleW(const double number, wchar_t* string, const PXSize dataSize)
{
#if OSUnix
    int bytesWritten = snprintf(string, dataSize, "%lf", number);
#elif OSWindows
	int bytesWritten = sprintf_s(string, dataSize, "%lf", number);
#endif
	return bytesWritten;
}

PXSize TextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize)
{
	const PXSize length = MathMinimumIU(dataSize, stringSize);
	PXSize outputIndex = 0;

	MemoryClear(string, stringSize);

	string[outputIndex++] = '0';
	string[outputIndex++] = 'b';

	for(PXSize dataIndex = 0; dataIndex < length; ++dataIndex)
	{
		const PXByte dataElement = ((PXAdress)data)[dataIndex];

		for(unsigned char bitIndex = 0; bitIndex < 8u; ++bitIndex)
		{
			const PXBool bit = (dataElement & (1 << bitIndex)) >> bitIndex;
			const char writeCharacter = bit ? '1' : '0';

			string[outputIndex++] = writeCharacter;
		}
	}

	return outputIndex;
}
