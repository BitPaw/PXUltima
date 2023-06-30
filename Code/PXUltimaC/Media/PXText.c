#include "PXText.h"

#include <OS/System/PXOSVersion.h>
#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define PXTextAssertEnable 0

#if PXTextAssertEnable
#include <assert.h>
#endif

#if OSUnix
#define PrintSN snprintf
#elif OSWindows
#define PrintSN sprintf_s
#endif

PXSize PXTextFromIntToBinary8U(char* const string, const PXSize dataSize, const PXInt8U number)
{
	const PXSize numberOfDigits = sizeof(PXInt8U) * 8u;
	const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

	return written;
}

PXSize PXTextFromIntToBinary16U(char* const string, const PXSize dataSize, const PXInt16U number)
{
	const PXSize numberOfDigits = sizeof(PXInt16U) * 8u;
	const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

	return written;
}

PXSize PXTextFromIntToBinary32U(char* const string, const PXSize dataSize, const PXInt32U number)
{
	const PXSize numberOfDigits = sizeof(PXInt32U) * 8u;
	const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

	return written;
}

PXSize PXTextFromIntToBinary64U(char* const string, const PXSize dataSize, const PXInt64U number)
{
	const PXSize numberOfDigits = sizeof(PXInt64U) * 8u;
	const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

	return written;
}

PXSize PXTextFromIntToBinary64UR(char* const string, const PXSize dataSize, const PXInt64U number, const unsigned char numberOfDigits)
{
	PXSize offset = 0;

	string[offset++] = '0';
	string[offset++] = 'b';

	for (int i = numberOfDigits - 1u; i >= 0; --i)
	{
		string[offset++] = '0' + ((number & (1LLU << i)) >> i);
	}

	string[offset] = '\0';

	return offset;
}

PXSize PXTextToLowerCase(const PXText* const pxTextSource, PXText* const pxTextTarget)
{
	switch (pxTextSource->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			for (PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
			{
				pxTextTarget->TextA[i] = MakeLetterCaseLower(pxTextSource->TextA[i]);
			}
		}
		case TextFormatUNICODE:
		{
			for (PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
			{
				pxTextTarget->TextW[i] = MakeLetterCaseLower(pxTextSource->TextW[i]);
			}

			break;
		}
	}

	pxTextTarget->SizeAllocated = pxTextSource->SizeAllocated;
	pxTextTarget->SizeUsed = pxTextSource->SizeUsed;
	pxTextTarget->NumberOfCharacters = pxTextSource->NumberOfCharacters;
	pxTextTarget->Format = pxTextSource->Format;

	return pxTextTarget->SizeUsed;
}

PXSize PXTextToUpperCase(const PXText* const pxTextSource, PXText* const pxTextTarget)
{
	switch (pxTextSource->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			for (PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
			{
				pxTextTarget->TextA[i] = MakeLetterCaseUpper(pxTextSource->TextA[i]);
			}

			break;
		}
		case TextFormatUNICODE:
		{
			for (PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
			{
				pxTextTarget->TextW[i] = MakeLetterCaseUpper(pxTextSource->TextW[i]);
			}

			break;
		}
	}

	pxTextTarget->SizeAllocated = pxTextSource->SizeAllocated;
	pxTextTarget->SizeUsed = pxTextSource->SizeUsed;
	pxTextTarget->NumberOfCharacters = pxTextSource->NumberOfCharacters;
	pxTextTarget->Format = pxTextSource->Format;

	return pxTextTarget->SizeUsed;
}

PXSize PXTextAppendW(wchar_t* const dataString, const PXSize dataStringSize, const wchar_t* const appaendString, const PXSize appaendStringSize)
{
	const PXSize length = PXTextLengthW(dataString, dataStringSize);
	const PXSize lengthAdded = PXTextCopyW(appaendString, appaendStringSize, &dataString[length], dataStringSize - length);
	const PXSize fullSize = length + lengthAdded;

	return fullSize;
}

PXSize PXTextPrint(PXText* const pxText, const char* style, ...)
{
	va_list args;
	va_start(args, style);

	switch (pxText->Format)
	{
		case TextFormatASCII:	
		case TextFormatUTF8:
		{
#if OSUnix
			pxText->SizeUsed = vsnprintf(pxText->TextA, pxText->SizeAllocated, style, args);
#elif OSWindows
			pxText->SizeUsed = vsprintf_s(pxText->TextA, pxText->SizeAllocated, style, args);
#endif
			break;
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			pxText->SizeUsed = vsnwprintf(pxText->TextW, pxText->SizeAllocated / 2, style, args);
#elif OSWindows
			pxText->SizeUsed = vswprintf_s(pxText->TextW, pxText->SizeAllocated/2, (wchar_t*)style, args);
#endif
			break;
		}

		default:
			return 0;
	}

	pxText->NumberOfCharacters = pxText->SizeUsed;

	va_end(args);

	// Mark end of data
	//PXMemorySet(pxText->TextA + pxText->SizeUsed + 1, '\0', pxText->SizeAllocated - pxText->SizeUsed - 1);

	return pxText->SizeUsed;
}

PXSize PXTextClear(PXText* const pxText)
{
	PXMemoryClear(pxText->TextA, pxText->SizeAllocated);

	return pxText->SizeAllocated;
}

void PXTextAdvance(PXText* const pxText, const PXSize advanceBy)
{
	pxText->TextA += advanceBy;
	pxText->SizeUsed -= advanceBy;
	pxText->SizeAllocated -= advanceBy;
	pxText->NumberOfCharacters -= advanceBy;
}

PXSize PXTextLengthA(const char* string, const PXSize stringSize)
{
	PXSize index = 0;

	for (; (string[index] != '\0') && (index < stringSize); ++index);

	return index;
}

PXSize PXTextLengthW(const wchar_t* string, const PXSize stringSize)
{
	PXSize index = 0;

	for (; (string[index] != L'\0') && (index < stringSize); ++index);

	return index;
}

PXSize PXTextLengthUntilA(const char* string, const PXSize stringSize, const char character)
{
	PXSize index = 0;

	for (; (index < stringSize) && (string[index] != '\0') && (string[index] != character); ++index);

	return index;
}

PXSize PXTextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character)
{
	return 0;
}

PXSize PXTextCopy(const PXText* const source, PXText* const destination)
{
	if (!(source && destination))
	{
		return 0;
	}

	const PXSize minLength = PXMathMinimumIU(source->SizeAllocated, destination->SizeAllocated);

	switch (source->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
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

			for (; (i < minLength) && (source->TextA[i] != '\0'); ++i)
			{
				destination->TextA[i] = source->TextA[i];
			}

			destination->Format = TextFormatASCII;
			destination->TextA[i] = '\0';
			destination->SizeUsed = i;
			destination->NumberOfCharacters = i;

			return i;

			break;
		}

		case TextFormatUNICODE:
		{
			PXSize i = 0;

#if PXTextAssertEnable
			assert(destination);
			assert(source);
#endif

			for (; (i < minLength) && (source->TextW[i] != '\0'); ++i)
			{
				destination->TextW[i] = source->TextW[i];
			}

			destination->Format = TextFormatUNICODE;
			destination->TextW[i] = '\0';
			destination->SizeUsed = i;
			destination->NumberOfCharacters = i;

			return i;

			break;
		}
	}

	return 0;
}

PXSize PXTextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
	const PXSize minLength = PXMathMinimumIU(sourceLength, destinationLength);
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

	for (; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

PXSize PXTextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
	const PXSize minLength = PXMathMinimum(sourceLength, destinationLength);
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

	for (; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

PXSize PXTextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
	const PXSize minLength = PXMathMinimum(sourceLength, destinationLength);
	PXSize i = 0;

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#endif

	for (; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = UnicodeToASCII(source[i]);
	}

	destination[i] = '\0';

	return i;
}

PXSize PXTextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
	const PXSize minLength = PXMathMinimumIU(sourceLength, destinationLength);
	PXSize i = 0;

#if PXTextAssertEnable
	assert(destination);
	assert(source);
#endif

	for (; (i < minLength) && (source[i] != '\0'); ++i)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

	return i;
}

void PXTextToUTFConvert(const PXSize symbol, PXByte* dataBuffer, PXSize* dataBufferSize)
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

	*dataBufferSize = utfTrailBytesAmount + 1u;
}

PXSize PXTextCopyAU(const PXTextASCII source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength)
{
	const PXSize minimalSize = PXMathMinimumIU(sourceLength, destinationLength);
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

		PXTextToUTFConvert(wideCharacter, buffer, &size);

		destinationIndex += PXMemoryCopy(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
	}

	destination[destinationIndex++] = 0u;

	return destinationIndex;
}



PXSize PXTextCopyWU(const PXTextUNICODE source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength)
{
#if 1
	const PXSize minimalSize = PXMathMinimumIU(sourceLength, destinationLength);
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

		PXTextToUTFConvert(wideCharacter, buffer, &size);

		destinationIndex += PXMemoryCopy(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
	}

	destination[destinationIndex++] = 0u;

	return destinationIndex;

#else
	const PXSize copyedBytes = wcstombs(destination, source, destinationLength);

	return copyedBytes;
#endif
}

PXSize PXTextCountA(const char* pxText, const PXSize textSize, const char target)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (pxText[index] != '\0'); ++index)
		samecounter += target == pxText[index];

	return samecounter;
}

PXSize PXTextCountW(const wchar_t* pxText, const PXSize textSize, const wchar_t target)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (pxText[index] != '\0'); ++index)
		samecounter += target == pxText[index];

	return samecounter;
}

PXSize PXTextCountUntilA(const char* pxText, const PXSize textSize, const char target, const char stopAt)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (pxText[index] != '\0' && pxText[index] != stopAt); ++index)
		samecounter += target == pxText[index];

	return samecounter;
}

PXSize PXTextCountUntilW(const wchar_t* pxText, const PXSize textSize, const wchar_t target, const wchar_t stopAt)
{
	PXSize samecounter = 0;

	for (PXSize index = 0; (index < textSize) && (pxText[index] != '\0' && pxText[index] != stopAt); ++index)
		samecounter += target == pxText[index];

	return samecounter;
}

PXBool PXTextCompare(const PXText* const textA, const PXText* const textB)
{
	return PXTrue;
}

PXBool PXTextCompareA(const char* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimumIU(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
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

PXBool PXTextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

PXBool PXTextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

PXBool PXTextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
		samecounter += a[index] == b[index];

	return (index == samecounter);
}

char PXTextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char PXTextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char PXTextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
	const PXSize textSize = PXMathMinimum(aSize, bSize);

	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for (; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore((wchar_t)a[index], b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char PXTextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
	PXSize index = 0;
	PXSize samecounter = 0;
	unsigned char wasLastLetterSame = 1;

	for (; (index < aSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
	{
		wasLastLetterSame = CompareLetterCaseIgnore(a[index], (wchar_t)b[index]);
		samecounter += wasLastLetterSame;
	}

	return index == samecounter;
}

char* PXTextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize)
{
	const char* source = 0;
	unsigned char found = 0;

	for (PXSize i = 0; (data[i] != '\0') && (i + targetSize) < dataSize && !found; i++)
	{
		source = data + i;
		found = PXTextCompareA(source, targetSize - i, target, targetSize);
	}

	return (char*)(found * (PXSize)source);
}

PXSize PXTextFindFirstCharacterA(const char* PXRestrict  const string, const PXSize dataSize, const char character)
{
	PXBool found = 0;

	PXSize index = 0;

	for (; index < dataSize && !found; ++index)
	{
		found = character == string[index];
	}

	return found ? index - 1 : PXTextIndexNotFound;
}

PXSize PXTextFindFirstCharacterBeforeA(const char* PXRestrict const string, const PXSize dataSize, const char target, const char barrier)
{
	PXSize index = 0;
	PXBool hitBarrier = 0;
	PXBool found = 0;

	for (; index < dataSize && !found; ++index)
	{
		hitBarrier = barrier == string[index];
		found = target == string[index] || hitBarrier;
	}

	return (found && !hitBarrier) ? index - 1 : PXTextIndexNotFound;
}

PXSize PXTextFindFirstCharacterOfListA(const char* PXRestrict const string, const PXSize dataSize, const char* characterList, const PXSize characterListSize)
{
	for (PXSize i = 0; i < characterListSize; ++i)
	{
		const PXSize index = PXTextFindFirstCharacterA(string, dataSize, characterList[i]);
		const PXBool suc = -1 != index;

		if (suc)
		{
			return index;
		}
	}

	return -1;
}

PXSize PXTextFindFirstStringA(const char* PXRestrict const string, const PXSize dataSize, const char* PXRestrict const targetString, const PXSize targetStringSize)
{
	PXSize index = 0;
	PXBool found = 0;

	for (; index < dataSize && !found; ++index)
	{
		found = string[index] == targetString[0];

		if (found)
		{
			found = PXTextCompareA(&string[index], dataSize - index, targetString[0], targetStringSize);
		}
	}

	return found ? index - 1 : PXTextIndexNotFound;
}

PXSize PXTextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character)
{
	PXBool found = 0;
	PXSize i = PXTextLengthW(string, dataSize);

	for (; i > 0 && !found; --i)
	{
		found = character == string[i];
	}

	++i;

	return found ? i + 1 : PXTextIndexNotFound;
}

PXBool PXTextFindLast(const PXText* const stringSource, const PXText* const stringTarget, PXText* const stringResult)
{
	PXBool found = 0;
	PXSize i = stringSource->SizeUsed - stringTarget->SizeUsed; // As we start from the back, the symbol can only be as long

	stringResult->SizeAllocated = 0;
	stringResult->SizeUsed = 0;
	stringResult->NumberOfCharacters = 0;

	if (stringSource->Format == TextFormatASCII && stringTarget->Format == TextFormatASCII) // Is Ascii
	{
		stringResult->Format = TextFormatASCII;

		for (; i > 0 && !found; --i)
		{
			// We can make a mask with a 64-bit view, then ask if we have atleast one match in that block
			// if we don't have a match, we can be sure this block does not contain the target


			// Check if the firsat symbol is the same
			found = stringTarget->TextA[0] == stringSource->TextA[i];

			if (found)
			{
				found = PXMemoryCompare(stringTarget->TextA, stringTarget->SizeUsed, stringSource->TextA + i, i);
			}
		}

		if (found)
		{
			stringResult->SizeUsed = stringSource->SizeUsed - i - 1;
			stringResult->NumberOfCharacters = stringSource->SizeUsed - i - 1;
			stringResult->TextA = stringSource->TextA + i + 1;
		}
	}

	if (stringSource->Format == TextFormatUNICODE && stringTarget->Format == TextFormatUNICODE) // Is Ascii
	{
		stringResult->Format = TextFormatUNICODE;

		for (; i > 0 && !found; --i)
		{
			// We can make a mask with a 64-bit view, then ask if we have atleast one match in that block
			// if we don't have a match, we can be sure this block does not contain the target


			// Check if the firsat symbol is the same
			found = stringTarget->TextW[0] == stringSource->TextW[i];

			if (found)
			{
				found = PXMemoryCompare(stringTarget->TextW, stringTarget->SizeUsed, stringSource->TextW + i, i);
			}
		}

		if (found)
		{
			stringResult->SizeUsed = stringSource->SizeUsed - i - 1;
			stringResult->NumberOfCharacters = stringSource->SizeUsed - i - 1;
			stringResult->TextW = stringSource->TextW + i + 1;
		}
	}

	return found;
}

void PXTextMoveByOffset(PXText* const pxText, const PXSize offset)
{
	switch (pxText->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			pxText->SizeAllocated -= sizeof(char) * offset;
			pxText->SizeUsed -= sizeof(char) * offset;
			pxText->NumberOfCharacters -= 1 * offset;
			pxText->TextA += 1 * offset;
			break;
		}
		case TextFormatUNICODE:
		{
			pxText->SizeAllocated -= sizeof(wchar_t) * offset;
			pxText->SizeUsed -= sizeof(wchar_t) * offset;
			pxText->NumberOfCharacters -= 1 * offset;
			pxText->TextW += 1 * offset;
			break;
		}
	}
}

void PXTextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character)
{
	PXSize index = PXTextFindFirstCharacterA(string, dataSize, character);

	if (index != -1)
	{
		string[index] = '\0';
	}
}

void PXTextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...)
{
	va_list args;
	va_start(args, syntax);

	PXSize offsetData = 0;

	for (PXSize commandIndex = 0; syntax[commandIndex] != '\0'; ++commandIndex)
	{
		const char commandKey = syntax[commandIndex];
		//const bool commandIsNumber = commandKey == 'i' || commandKey == 'f' || commandKey == 'u';

		PXSize offsetLength = offsetData;

		// Get length until new block
		{
			while (offsetLength < bufferSize)
			{
				const char symbol = buffer[offsetLength];
				const PXBool stop = symbol == '\n' || symbol == '\0' || symbol == ' ';
				//const bool skip = symbol == '\n'; 	if(commandIsNumber && current == '/' || current == ' ' || finished)

				if (stop)
				{
					break; // End of string
				}

				++offsetLength;
			}
		}

		switch (commandKey)
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

				const PXSize readBytes = PXTextCopyA(source, offsetLength, destination, -1);

				offsetData += readBytes;

				break;
			}
			case 'i':
			case 'd':
			case 'u':
			{
				int* const i = va_arg(args, int*);

				PXText pyText;
				PXTextConstructFromAdressA(&pyText, buffer + offsetData, offsetLength);

				const PXSize readBytes = PXTextToInt(&pyText, i);

#if PXTextAssertEnable
				assert(readBytes);
#endif

				offsetData += readBytes;

				break;
			}
			case 'f':
			{
				float* number = va_arg(args, float*);
				PXText pyText;
				PXTextConstructFromAdressA(&pyText, buffer + offsetData, offsetLength);

				const PXSize readBytes = PXTextToFloat(&buffer, number);

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

		while (offsetData < bufferSize)
		{
			const char symbol = buffer[offsetData];
			const PXBool stop = !(symbol == ' ' || symbol == '\0');

			if (stop)
			{
				break;
			}

			++offsetData;
		}
	}

	va_end(args);
}

void PXTextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize)
{
	unsigned char finished = 0;
	unsigned char foundItem = 0;
	PXSize foundTargets = 0;

	for (size_t i = 0; i < parsingTokenListSize; i++)
	{
		PXMemorySet(parsingTokenList[i].Value, 0, sizeof(void*));
	}

	for (PXSize i = 0; (i < stringSize) && (string[i] != '\0') && !finished; ++i)
	{
		foundItem = 0;

		for (PXSize t = 0; (t < parsingTokenListSize) && (string[i] != ' ') && !foundItem; t++)
		{
			const ParsingTokenA* parsingToken = &parsingTokenList[t];
			const char* targetString = parsingToken->String;
			const PXSize targetStringSize = PXTextLengthA(targetString, -1);
			const char* sourceString = string + i;

			foundItem = PXMemoryCompare(sourceString, stringSize, targetString, targetStringSize); // Compare whole word

			if (foundItem)
			{
				PXSize lengthTag = PXTextLengthUntilA(sourceString, stringSize, '=');
				const char* valueString = sourceString + lengthTag + 1;

				i += lengthTag + 1;

				(*parsingToken->Value) = valueString;

				for (; (string[i] != '\0') && string[i] != ' '; i++); // Skip data

				++foundTargets;
			}
		}

		finished = foundTargets == parsingTokenListSize;
	}
}

PXBool PXTextMatchW(const wchar_t* input, const PXSize inputSize, const wchar_t* pattern, const PXSize patternSize)
{
	PXSize patternIndex = 0;

	if (!input || !inputSize || !pattern || !patternSize)
	{
		return PXFalse;
	}

	// If we only have '*' as a pattern, we can just stop.
	const PXBool quitFast = (patternSize == 1) && ((pattern[0] == '*') || (input[0] == pattern[0]));

	if (quitFast)
	{
		return PXTrue;
	}

	for (PXSize index = 0; index < inputSize && input[index] != '\0'; ++index)
	{
		const PXBool isMatch = input[index] == pattern[patternIndex];
		const PXBool isStar = pattern[index] == '*';

		if (isStar)
		{
			++patternIndex;

			while (input[index] != pattern[patternIndex] && index < inputSize && input[index] != '\0')
			{
				++index;
			}

			++patternIndex;
			continue;
		}

		if (!isMatch)
		{
			return PXFalse;
		}
	}

	return PXTrue;
}

PXSize PXTextReplace(PXText* const pxText, char target, char value)
{
	switch (pxText->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			for (size_t i = 0; i < pxText->SizeUsed; i++)
			{
				pxText->TextA[i] = pxText->TextA[i] == target ? value : pxText->TextA[i];
			}

			break;
		}
		case TextFormatUNICODE:
		{
			for (size_t i = 0; i < pxText->SizeUsed; i++)
			{
				pxText->TextW[i] = pxText->TextW[i] == target ? value : pxText->TextW[i];
			}

			break;
		}
	}
}

PXSize PXTextFromInt(PXText* const pxText, int number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			PXBool isSigned = PXFalse;
			pxText->SizeUsed = 0; // Current offset of the data buffer

			if (number < 0) // number negative
			{
				isSigned = PXTrue; // Save state that we have a negative number
				number *= -1; // Remove negative value
			}

			do
			{
				pxText->TextA[pxText->SizeUsed++] = number % 10 + '0'; // Get the value of the most right digit and convert to ASCII-Number
			} while ((number /= 10) > 0); // Remove the most right digit by interget division and check if we still have a number to process

			if (isSigned) // if we had a minus, add it now
			{
				pxText->TextA[pxText->SizeUsed++] = '-'; // Add the minus
			}

			pxText->TextA[pxText->SizeUsed] = '\0'; // Add the termination byte

			// Reverse the order of the string
			{
				const PXSize halfSize = pxText->SizeUsed / 2u;

				for (PXSize i = 0; i < halfSize; i++)
				{
					const PXSize index = pxText->SizeUsed - 1 - i; // index from end position, before the \0
					const PXByte temp = pxText->TextA[index];

					pxText->TextA[index] = pxText->TextA[i];
					pxText->TextA[i] = temp;
				}
			}

			//itoa(number, string, dataSize);

			//int bytesWritten = PrintSN(string, dataSize, "%i", number);

			return pxText->SizeUsed;
		}


		case TextFormatUNICODE:
		{
			PXText pxTextA;
			PXTextConstructBufferA(&pxTextA, 16);

			PXTextFromInt(&pxTextA, number);

			PXTextCopyAW(pxTextA.TextA, pxTextA.SizeUsed, pxText->TextW, pxText->SizeAllocated);
			 
			return pxText->SizeUsed;
		}
	}
}

PXSize PXTextFromBool(PXText* const pxText, const PXBool number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			pxText->TextA[0] = number ? '1' : '0';

			return sizeof(char);
		}


		case TextFormatUNICODE:
		{
			pxText->TextW[0] = number ? '1' : '0';

			return sizeof(wchar_t);
		}
	}

	return 0;
}

PXSize PXTextFromFloat(PXText* const pxText, const float number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
			pxText->SizeUsed = snprintf(pxText->TextA, pxText->SizeAllocated, "%f", number);
#elif OSWindows
			pxText->SizeUsed = sprintf_s(pxText->TextA, pxText->SizeAllocated, "%f", number);
#endif
			return pxText->SizeUsed;
		}


		case TextFormatUNICODE:
		{
#if OSUnix
			pxText->SizeUsed = snprintf(pxText->TextW, pxText->SizeAllocated, "%f", number);
#elif OSWindows
			pxText->SizeUsed = sprintf_s(pxText->TextW, pxText->SizeAllocated, "%f", number);
#endif

			return pxText->SizeUsed;
		}
	}

	return 0;
}

PXSize PXTextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize)
{
	const PXSize length = PXMathMinimumIU(dataSize, stringSize);
	PXSize outputIndex = 0;

	PXMemoryClear(string, stringSize);

	string[outputIndex++] = '0';
	string[outputIndex++] = 'b';

	for (PXSize dataIndex = 0; dataIndex < length; ++dataIndex)
	{
		const PXByte dataElement = ((PXAdress)data)[dataIndex];

		for (unsigned char bitIndex = 0; bitIndex < 8u; ++bitIndex)
		{
			const PXBool bit = (dataElement & (1 << bitIndex)) >> bitIndex;
			const char writeCharacter = bit ? '1' : '0';

			string[outputIndex++] = writeCharacter;
		}
	}

	return outputIndex;
}

PXSize PXTextToInt(const PXText* const pxText, int* const number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			int accumulator = 0;
			PXSize index = 0;
			PXBool isNegative = 0;

			if (!pxText->TextA)
			{
				return 0;
			}

			if (pxText->TextA[0] == '-')
			{
				index++;
				isNegative = 1u;
			}

			while (pxText->TextA[index] != '\0')
			{
				char character = pxText->TextA[index];
				const PXBool isValidCharacter = (character >= '0' && character <= '9');
				int numberElement = character - '0';

				if (!isValidCharacter)
				{
					break;
				}

				accumulator *= 10; // "Shft number to left" Example 12 -> 120
				accumulator += numberElement; // ASCII character to actual number.

				++index;
			}

			if (isNegative)
			{
				accumulator *= -1;
			}

			*number = accumulator;

			return index;
		}


		case TextFormatUNICODE:
		{


			return sizeof(wchar_t);
		}
	}

	return 0;
}

PXSize PXTextToBool(const PXText* const pxText, PXBool* const number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			switch (pxText->TextA[0])
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

			return sizeof(char);
		}


		case TextFormatUNICODE:
		{


			return sizeof(wchar_t);
		}
	}

	return 0;
}

PXSize PXTextToFloat(const PXText* const pxText, float* const number)
{
	switch (pxText->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
			int accumulator = 0;
			PXSize digitsAfterDot = 1;
			PXSize index = 0;
			unsigned char isNegative = 0;
			unsigned char isWholeNumberChunk = 1;



			if (!pxText)
			{
				return 0;
			}

			if (pxText->TextA[0] == '-')
			{
				index++;
				isNegative = 1;
			}

			for (; pxText->TextA[index] != '\0'; ++index)
			{
				const char character = pxText->TextA[index];
				const PXBool isDot = character == '.';
				const PXBool isValidCharacter = (character >= '0' && character <= '9') || isDot;
				const int numberElement = character - '0';

				if (!isValidCharacter)
				{
					break;
				}

				// Trigger when we switch to after dot
				if (isDot && isWholeNumberChunk)
				{
					isWholeNumberChunk = 0;
					continue;
				}

				accumulator *= 10; // "Shft number to left" Example 12 -> 120
				accumulator += numberElement; // ASCII character to actual number.

				if (!isWholeNumberChunk)
				{
					digitsAfterDot *= 10;
				}
			}

			if (isNegative)
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


		case TextFormatUNICODE:
		{


			return sizeof(wchar_t);
		}
	}

	return 0;
}