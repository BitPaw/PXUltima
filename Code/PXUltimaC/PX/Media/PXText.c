#include "PXText.h"

#include <PX/OS/System/Version/PXOSVersion.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/PXOS.h>

#include <stdio.h>


#define PXAssertEnable 0

#if PXAssertEnable
#include <assert.h>
#endif

PXSize PXAPI PXTextFromNonTerminated(char PXREF stringOutput, const PXSize stringOutputSize, const char PXREF stringInput, const PXSize stringInputSize)
{
    const PXSize length = PXMathMinimumIU(stringOutputSize, stringInputSize);
    PXSize i = 0;

    for(; i < length; ++i)
    {
        stringOutput[i] = PXTextMakePrintable(stringInput[i]);
    }

    stringOutput[i] = '\0';

    return i;
}

PXSize PXAPI PXTextFromIntToBinary8U(char PXREF string, const PXSize dataSize, const PXI8U number)
{
    const PXSize numberOfDigits = sizeof(PXI8U) * 8u;
    const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

    return written;
}

PXSize PXAPI PXTextFromIntToBinary16U(char PXREF string, const PXSize dataSize, const PXI16U number)
{
    const PXSize numberOfDigits = sizeof(PXI16U) * 8u;
    const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

    return written;
}

PXSize PXAPI PXTextFromIntToBinary32U(char PXREF string, const PXSize dataSize, const PXI32U number)
{
    const PXSize numberOfDigits = sizeof(PXI32U) * 8u;
    const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

    return written;
}

PXSize PXAPI PXTextFromIntToBinary64U(char PXREF string, const PXSize dataSize, const PXI64U number)
{
    const PXSize numberOfDigits = sizeof(PXI64U) * 8u;
    const PXSize written = PXTextFromIntToBinary64UR(string, dataSize, number, numberOfDigits);

    return written;
}

PXSize PXAPI PXTextFromIntToBinary64UR(char PXREF string, const PXSize dataSize, const PXI64U number, const unsigned char numberOfDigits)
{
    PXSize offset = 0;

    string[offset++] = '0';
    string[offset++] = 'b';

    for(int i = numberOfDigits - 1u; i >= 0; --i)
    {
        string[offset++] = '0' + ((number & (1LLU << i)) >> i);
    }

    string[offset] = '\0';

    return offset;
}

PXSize PXAPI PXTextFromIntToBinary(char PXREF string, const PXSize dataSize, const void PXREF data, const unsigned char numberOfDigits)
{
    PXSize offset = 0;

    string[offset++] = '0';
    string[offset++] = 'b';

    PXSize trips = (numberOfDigits + 1 / 8) + 1;

    for(PXSize y = 0; y < trips; ++y)
    {
        const PXByte target = ((char*)data)[y];

        PXI8U left = (numberOfDigits - (y * 8)) % 8;

        for(PXI8U x = 0; x < left; ++x)
        {
            string[offset++] = '0' + ((target & (1 << x)) >> x);
        }
    }

    string[offset] = '\0';

    return offset;
}

PXSize PXAPI PXTextToLowerCase(const PXText PXREF pxTextSource, PXText PXREF pxTextTarget)
{
    switch(pxTextSource->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            for(PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
            {
                pxTextTarget->A[i] = PXTextMakeCaseLower(pxTextSource->A[i]);
            }
        }
        case TextFormatUNICODE:
        {
            for(PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
            {
                pxTextTarget->W[i] = PXTextMakeCaseLower(pxTextSource->W[i]);
            }

            break;
        }
    }

    pxTextTarget->SizeAllocated = pxTextSource->SizeAllocated;
    pxTextTarget->SizeUsed = pxTextSource->SizeUsed;
    //pxTextTarget->NumberOfCharacters = pxTextSource->NumberOfCharacters;
    pxTextTarget->Format = pxTextSource->Format;

    return pxTextTarget->SizeUsed;
}

PXSize PXAPI PXTextToUpperCase(const PXText PXREF pxTextSource, PXText PXREF pxTextTarget)
{
    switch(pxTextSource->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            for(PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
            {
                pxTextTarget->A[i] = PXTextMakeCaseUpper(pxTextSource->A[i]);
            }

            break;
        }
        case TextFormatUNICODE:
        {
            for(PXSize i = 0; i < pxTextSource->SizeUsed; ++i)
            {
                pxTextTarget->W[i] = PXTextMakeCaseUpper(pxTextSource->W[i]);
            }

            break;
        }
    }

    pxTextTarget->SizeAllocated = pxTextSource->SizeAllocated;
    pxTextTarget->SizeUsed = pxTextSource->SizeUsed;
    //pxTextTarget->NumberOfCharacters = pxTextSource->NumberOfCharacters;
    pxTextTarget->Format = pxTextSource->Format;

    return pxTextTarget->SizeUsed;
}

PXSize PXAPI PXTextTrimA(char PXREF text, const PXSize textSize)
{
    if(!(text && textSize))
    {
        return 0;
    }

    PXSize i;

    for(i = textSize - 1; i; --i)
    {
        const PXBool isEmotySpace = PXTextIsEmpty(text[i]);

        if(!isEmotySpace)
        {
            break;
        }

        text[i] = '\0';
    }

    return i + 1;
}

PXSize PXAPI PXAppend(PXText PXREF currentString, const PXText PXREF appendingString)
{
    const PXSize freeSpace = currentString->SizeAllocated - currentString->SizeUsed;
    const PXBool isEnoughSpace = freeSpace > appendingString->SizeUsed;

    if(!isEnoughSpace && appendingString->A == PXNull)
    {
        return 0;
    }

    char* data = &currentString->A[currentString->SizeUsed];

    currentString->SizeUsed += PXTextCopyA(appendingString->A, appendingString->SizeUsed, data, freeSpace);

    return currentString->SizeUsed;
}

PXSize PXAPI PXAppendA(PXText PXREF currentString, const char PXREF appaendString, const char appaendStringSize)
{
    currentString->SizeUsed += PXTextCopyA
    (
        appaendString,
        appaendStringSize,
        &currentString->A[currentString->SizeUsed],
        currentString->SizeAllocated - currentString->SizeUsed
    );

    return currentString->SizeUsed;
}

PXSize PXAPI PXAppendW(wchar_t PXREF dataString, const PXSize dataStringSize, const wchar_t PXREF appaendString, const PXSize appaendStringSize)
{
    const PXSize length = PXTextLengthW(dataString, dataStringSize);
    const PXSize lengthAdded = PXTextCopyW(appaendString, appaendStringSize, &dataString[length], dataStringSize - length);
    const PXSize fullSize = length + lengthAdded;

    return fullSize;
}

PXSize PXAPI PXAppendF(PXText PXREF pxText, const char PXREF format, ...)
{
    PXSize sizeLeft = pxText->SizeAllocated - pxText->SizeUsed;
    char* beginning = pxText->A + pxText->SizeUsed;

    va_list args;
    va_start(args, format);

    const PXSize added = PXTextPrintAV(beginning, sizeLeft, format, args);

    pxText->SizeUsed += added;

    va_end(args);

    return added;
}

PXSize PXAPI PXTextPrint(PXText PXREF pxText, const char* style, ...)
{
    va_list args;
    va_start(args, style);

    const PXSize written = PXTextPrintV(pxText, style, args);

    va_end(args);

    return written;
}

PXSize PXAPI PXTextPrintA(char PXREF text, const PXSize size, const char* style, ...)
{
    va_list args;
    va_start(args, style);

    PXText pxText;
    PXTextFromAdressA(&pxText, text, 0, size);

    PXSize xx = PXTextPrintV(&pxText, style, args);

    va_end(args);

    return xx;
}

PXSize PXAPI PXTextPrintW(wchar_t PXREF text, const PXSize size, const char* style, ...)
{
    va_list args;
    va_start(args, style);

    PXSize xx = PXTextPrintAV((char*)text, size, style, args);

    va_end(args);

    char cache[128];

    PXTextCopyA(text, xx, cache, 128);

    PXTextCopyAW(cache, 128, text, size);

    return xx;
}

PXSize PXAPI PXTextPrintAV(char PXREF text, const PXSize size, const char* style, va_list parameter)
{
#if OSUnix
    return vsnprintf(text, size, style, parameter);
#elif OSWindows && PXDefaultLibraryEnable
    return vsprintf_s(text, size, style, parameter);
#endif
}

PXSize PXAPI PXTextPrintWV(wchar_t PXREF text, const PXSize size, const char* style, va_list parameter)
{
#if OSUnix
    return 0;// vsnwprintf(text, size, style, parameter);
#elif OSWindows && PXDefaultLibraryEnable
    return vswprintf_s(text, size, (wchar_t*)style, parameter);
#else
    return 0;
#endif
}

PXSize PXAPI PXTextPrintV(PXText PXREF pxText, const char* style, va_list parameter)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            pxText->SizeUsed = PXTextPrintAV(pxText->A, pxText->SizeAllocated, style, parameter);
            break;
        }
        case TextFormatUNICODE:
        {
            pxText->SizeUsed = PXTextPrintWV(pxText->W, pxText->SizeAllocated, style, parameter);
            break;
        }

        default:
            return 0;
    }

   // pxText->NumberOfCharacters = pxText->SizeUsed;


    // Mark end of data
    //PXMemorySet(pxText->A + pxText->SizeUsed + 1, '\0', pxText->SizeAllocated - pxText->SizeUsed - 1);

    return pxText->SizeUsed;
}

PXSize PXAPI PXTextClear(PXText PXREF pxText)
{
    PXMemoryClear(pxText->A, pxText->SizeAllocated);

    return pxText->SizeAllocated;
}

void PXAPI PXAdvance(PXText PXREF pxText, const PXSize advanceBy)
{
    pxText->A += advanceBy;
    pxText->SizeUsed -= advanceBy;
    pxText->SizeAllocated -= advanceBy;
    //pxText->NumberOfCharacters -= advanceBy;
}

void PXAPI PXTextRevise(PXText PXREF pxText)
{
    if(!pxText)
    {
        return;
    }


    PXBool sizeIsMissing = pxText->A && (pxText->SizeUsed > 0);

    if(sizeIsMissing)
    {
        switch(pxText->Format)
        {
            case TextFormatASCII:
            {
                pxText->SizeUsed = PXTextLengthA(pxText->A, pxText->SizeAllocated);
                break;
            }
            case TextFormatUNICODE:
            {
                pxText->SizeUsed = PXTextLengthW(pxText->W, pxText->SizeAllocated);
                break;
            }
            case TextFormatUTF8:
            case TextFormatUTF16:
            {
                pxText->SizeUsed = PXTextLengthA(pxText->A, pxText->SizeAllocated);
                break;
            }
            default:
                return;
        }
    }

}

PXSize PXAPI PXTextLengthA(const char* string, const PXSize stringSize)
{
    if(!(string && stringSize))
    {
        return 0;
    }

    PXSize index = 0;

    for(; (string[index] != '\0') && (index < stringSize); ++index);

    return index;
}

PXSize PXAPI PXTextLengthW(const wchar_t* string, const PXSize stringSize)
{
    if(!(string && stringSize))
    {
        return 0;
    }

#if PXDefaultLibraryEnable
    return wcslen(string);
#else
    PXSize index = 0;

    for(; (string[index] != L'\0') && (index < stringSize); ++index);

    return index;
#endif
}

PXSize PXAPI PXTextLengthUntilA(const char* string, const PXSize stringSize, const char character)
{
    if(!(string && stringSize))
    {
        return 0;
    }

    PXSize index = 0;

    for(; (index < stringSize) && (string[index] != '\0') && (string[index] != character); ++index);

    return index;
}

PXSize PXAPI PXTextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character)
{
    return 0;
}

PXSize PXAPI PXTextCopy(PXText PXREF pxText, const PXText PXREF pxTextsource)
{
    if(!(pxText && pxTextsource))
    {
        return 0;
    }

    const PXSize minLength = PXMathMinimumIU(pxTextsource->SizeUsed, pxText->SizeAllocated);

    if(minLength == 0)
    {
        return 0;
    }

    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            PXSize i = 0;

            for(; (i < minLength) && (pxTextsource->A[i] != '\0'); ++i)
            {
                pxText->A[i] = pxTextsource->A[i];
            }

            pxText->Format = TextFormatASCII;
            pxText->A[i] = '\0';
            pxText->SizeUsed = i;
           // destination->NumberOfCharacters = i;

            return i;
        }

        case TextFormatUNICODE:
        {
            PXSize i = 0;

#if PXAssertEnable
            assert(destination);
            assert(source);
#endif

            for(; (i < minLength) && (pxTextsource->W[i] != '\0'); ++i)
            {
                pxText->W[i] = pxTextsource->W[i];
            }

            pxText->Format = TextFormatUNICODE;
            pxText->W[i] = '\0';
            pxText->SizeUsed = i * sizeof(wchar_t);
            //destination->NumberOfCharacters = i;

            return i;
        }
    }

    return 0;
}

PXSize PXAPI PXTextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
    PXSize maxLength = PXMathMinimumIU(sourceLength, destinationLength);
    PXSize i = 0;

#if PXAssertEnable
    assert(destination);
    assert(source);
#else
    if(!(destination && source))
    {
        return 0;
    }

    if(maxLength == 0) // We have no size to do anything, so quit.
    {
        return 0;
    }

    //--maxLength; // We make room for then \0

#endif

    for(; (i < maxLength) && (source[i] != '\0'); ++i)
    {
        destination[i] = source[i];
    }

    destination[i] = '\0';

    return i;
}

PXSize PXAPI PXTextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
    const PXSize minLength = PXMathMinimum(sourceLength, destinationLength);
    const PXBool validCall = source && destination && minLength;
    PXSize i = 0;

    if(!validCall)
    {
        return 0;
    }

#if PXAssertEnable
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

PXSize PXAPI PXTextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength)
{
    const PXSize minLength = PXMathMinimum(sourceLength, destinationLength);
    PXSize i = 0;

#if PXAssertEnable
    assert(destination);
    assert(source);
#endif

    for(; (i < minLength) && (source[i] != '\0'); ++i)
    {
        destination[i] = PXTextUnicodeToASCII(source[i]);
    }

    destination[i] = '\0';

    return i;
}

PXSize PXAPI PXTextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength)
{
    const PXSize minLength = PXMathMinimumIU(sourceLength, destinationLength);
    PXSize i = 0;

#if PXAssertEnable
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

void PXAPI PXTextToUTFConvert(const PXSize symbol, PXByte* dataBuffer, PXSize* dataBufferSize)
{
    const char utfTrailBytesAmount =
        0u * (symbol >= 0 && symbol < 128u) + // 7-bit
        1u * (symbol >= 128u && symbol < 2048u) +  // 11-Bit
        2u * (symbol >= 2048u && symbol < 65535u) + // 16-Bit
        3u * (symbol >= 65535u && symbol < 2097152u); // 21-Bit

    switch(utfTrailBytesAmount)
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

    for(PXSize i = 1; i < (PXSize)utfTrailBytesAmount; ++i)
    {
        dataBuffer[i] = 0b10000000 | ((symbol & (0b111111 << 6u * i)) >> 6u * i);
    }

    *dataBufferSize = utfTrailBytesAmount + 1u;
}

PXSize PXAPI PXTextCopyAU(const char PXREF source, const PXSize sourceLength, char PXREF destination, const PXSize destinationLength)
{
    const PXSize minimalSize = PXMathMinimumIU(sourceLength, destinationLength);
    PXSize destinationIndex = 0;

    if(!(source && destinationLength))
    {
        return 0;
    }

    for(PXSize sourceIndex = 0; (sourceIndex < minimalSize) && (destinationIndex < destinationLength) && (source[sourceIndex] != '\0'); ++sourceIndex)
    {
        const char wideCharacter = source[sourceIndex];
        PXByte buffer[4];
        PXSize size = 0;

        PXTextToUTFConvert(wideCharacter, buffer, &size);

        destinationIndex += PXMemoryCopyX(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
    }

    destination[destinationIndex++] = 0u;

    return destinationIndex;
}



PXSize PXAPI PXTextCopyWU(const wchar_t PXREF source, const PXSize sourceLength, char PXREF destination, const PXSize destinationLength)
{
#if 1
    const PXSize minimalSize = PXMathMinimumIU(sourceLength, destinationLength);
    PXSize destinationIndex = 0;

    if(!(source && destinationLength))
    {
        return 0;
    }

    for(PXSize sourceIndex = 0; (sourceIndex < minimalSize) && (destinationIndex < destinationLength) && (source[sourceIndex] != '\0'); ++sourceIndex)
    {
        const PXUNICODE wideCharacter = source[sourceIndex];
        PXByte buffer[4];
        PXSize size = 0;

        PXTextToUTFConvert(wideCharacter, buffer, &size);

        destinationIndex += PXMemoryCopyX(buffer, size, destination + destinationIndex, destinationLength - destinationIndex);
    }

    destination[destinationIndex++] = 0u;

    return destinationIndex;

#else
    const PXSize copyedBytes = wcstombs(destination, source, destinationLength);

    return copyedBytes;
#endif
}

PXSize PXAPI PXTextCountA(const char* pxText, const PXSize textSize, const char target)
{
    PXSize samecounter = 0;

    for(PXSize index = 0; (index < textSize) && (pxText[index] != '\0'); ++index)
        samecounter += target == pxText[index];

    return samecounter;
}

PXSize PXAPI PXTextCountW(const wchar_t* pxText, const PXSize textSize, const wchar_t target)
{
    PXSize samecounter = 0;

    for(PXSize index = 0; (index < textSize) && (pxText[index] != '\0'); ++index)
        samecounter += target == pxText[index];

    return samecounter;
}

PXSize PXAPI PXTextPascalCaseCleave(const char* pxText, const PXSize PXTextSize)
{
    PXSize index = 0;

    if(PXTextIsLetterCaseUpper(pxText[index]))
    {
        index++;
    }

    for(; PXTextIsLetterCaseUpper(pxText[index]) && PXTextIsLetterCaseUpper(pxText[index + 1]) && !PXTextIsEndOfString(pxText[index]); ++index);
    for(; PXTextIsLetterCaseLower(pxText[index]) && !PXTextIsEndOfString(pxText[index]); ++index);

    return index;
}

PXSize PXAPI PXTextCountUntilA(const char* pxText, const PXSize textSize, const char target, const char stopAt)
{
    PXSize samecounter = 0;

    for(PXSize index = 0; (index < textSize) && (pxText[index] != '\0' && pxText[index] != stopAt); ++index)
        samecounter += target == pxText[index];

    return samecounter;
}

PXSize PXAPI PXTextCountUntilW(const wchar_t* pxText, const PXSize textSize, const wchar_t target, const wchar_t stopAt)
{
    PXSize samecounter = 0;

    for(PXSize index = 0; (index < textSize) && (pxText[index] != '\0' && pxText[index] != stopAt); ++index)
        samecounter += target == pxText[index];

    return samecounter;
}

PXBool PXAPI PXTextCompare(const PXText PXREF A, const PXText PXREF textB)
{
    return PXTrue;
}

PXI8U PXAPI PXTextCompareAVI8(const char* a, PXI8U aSize, const char* PXREF stringList, const PXI8U* stringListSize, const PXI8U amount)
{
    PXSize inputSize = aSize;

    if(aSize = 0xFF)
    {
        inputSize = (PXSize)-1;
    }

    for(PXI8U i = 0; i < amount; ++i)
    {
        const char PXREF stringB = stringList[i];
        const PXI8U stringBSize = stringListSize[i];


        const PXBool isTarget = PXTextCompareA(a, inputSize, stringB, stringBSize, 0);

        if(isTarget)
        {
            return i;
        }
    }

    return (PXI8U)-1;
}

PXBool PXAPI PXTextCompareA(const char* a, PXSize aSize, const char* b, PXSize bSize, const PXI8U flags)
{
    if(!(a && aSize && b && bSize))
    {
        return PXFalse;
    }

    if(PXTextUnkownLength == aSize)
    {
        aSize = PXTextLengthA(a, PXTextUnkownLength);
    }

    if(PXTextUnkownLength == bSize)
    {
        bSize = PXTextLengthA(b, PXTextUnkownLength);
    }

    if((PXTextCompareRequireSameLength & flags) && (aSize != bSize)) // Should be correct but can make errors?
    {
        return PXFalse;
    }

    const PXSize textSize = PXMathMinimumIU(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
        samecounter += a[index] == b[index];

    if(!(PXTextCompareRequireSameLength & flags))
    {
        return samecounter == index;
    }

    const PXBool stillHasDatainA = textSize < aSize;
    const PXBool stillHasDatainB = textSize < bSize;

    // Check if data is just empty
    {
        const PXBool isEmptyData = stillHasDatainA && stillHasDatainB;

        if(isEmptyData)
        {
            return (index == samecounter); // End check
        }
    }

    if(stillHasDatainA)
    {
        samecounter += a[index] == '\0';
        ++index; // keep for comparsion
    }

    if(stillHasDatainB)
    {
        samecounter += b[index] == '\0';
        ++index; // keep for comparsion
    }

    return (index == samecounter);
}

PXBool PXAPI PXTextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
        samecounter += a[index] == b[index];

    return (index == samecounter);
}

PXBool PXAPI PXTextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
        samecounter += a[index] == b[index];

    return (index == samecounter);
}

PXBool PXAPI PXTextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0'); ++index)
        samecounter += a[index] == b[index];

    return (index == samecounter);
}

char PXAPI PXTextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;
    unsigned char wasLastLetterSame = 1;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
    {
        wasLastLetterSame = PXTextCompareCaseIgnore(a[index], b[index]);
        samecounter += wasLastLetterSame;
    }

    return index == samecounter;
}

char PXAPI PXTextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;
    unsigned char wasLastLetterSame = 1;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
    {
        wasLastLetterSame = PXTextCompareCaseIgnore(a[index], b[index]);
        samecounter += wasLastLetterSame;
    }

    return index == samecounter;
}

char PXAPI PXTextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize)
{
    const PXSize textSize = PXMathMinimum(aSize, bSize);

    PXSize index = 0;
    PXSize samecounter = 0;
    unsigned char wasLastLetterSame = 1;

    for(; (index < textSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
    {
        wasLastLetterSame = PXTextCompareCaseIgnore((wchar_t)a[index], b[index]);
        samecounter += wasLastLetterSame;
    }

    return index == samecounter;
}

char PXAPI PXTextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize)
{
    PXSize index = 0;
    PXSize samecounter = 0;
    unsigned char wasLastLetterSame = 1;

    for(; (index < aSize) && (a[index] != '\0') && (b[index] != '\0') && wasLastLetterSame; ++index)
    {
        wasLastLetterSame = PXTextCompareCaseIgnore(a[index], (wchar_t)b[index]);
        samecounter += wasLastLetterSame;
    }

    return index == samecounter;
}

char* PXAPI PXTextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize)
{
    const char* source = 0;
    unsigned char found = 0;

    for(PXSize i = 0; (data[i] != '\0') && (i + targetSize) < dataSize && !found; i++)
    {
        source = data + i;
        found = PXTextCompareA(source, targetSize - i, target, targetSize, 0);
    }

    return (char*)(found * (PXSize)source);
}

PXSize PXAPI PXTextFindLastCharacter(const PXText PXREF pxText, const char character)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            return PXTextFindLastCharacterA(pxText->A, pxText->SizeUsed, character);
        }
    }

    return -1;
}

PXSize PXAPI PXTextFindLastCharacterA(const char PXREF text, const PXSize textSize, const char character)
{
    if(!text)
    {
        return -1;
    }

    const void* foundAdress = PXMemoryLocateLast(text, character, textSize);

    if(!foundAdress)
    {
        return -1;
    }

    PXSize offset = (PXSize)foundAdress - (PXSize)text;


    return offset;
}

PXSize PXAPI PXTextFindFirstCharacter(const PXText PXREF pxText, const char character)
{
    return PXTextFindFirstCharacterA(pxText->A, pxText->SizeUsed, character);
}

PXSize PXAPI PXTextFindFirstCharacterA(const char* PXRestrict  const string, const PXSize dataSize, const char character)
{
    PXBool found = 0;

    PXSize index = 0;

    for(; index < dataSize && !found; ++index)
    {
        found = character == string[index];
    }

    return found ? index - 1 : PXTextIndexNotFound;
}

PXSize PXAPI PXTextFindFirstCharacterBeforeA(const char* PXRestrict const string, const PXSize dataSize, const char target, const char barrier)
{
    PXSize index = 0;
    PXBool hitBarrier = 0;
    PXBool found = 0;

    for(; index < dataSize && !found; ++index)
    {
        hitBarrier = barrier == string[index];
        found = target == string[index] || hitBarrier;
    }

    return (found && !hitBarrier) ? index - 1 : PXTextIndexNotFound;
}

PXSize PXAPI PXTextFindFirstCharacterOfListA(const char* PXRestrict const string, const PXSize dataSize, const char* characterList, const PXSize characterListSize)
{
    for(PXSize i = 0; i < characterListSize; ++i)
    {
        const PXSize index = PXTextFindFirstCharacterA(string, dataSize, characterList[i]);
        const PXBool suc = -1 != index;

        if(suc)
        {
            return index;
        }
    }

    return -1;
}

PXSize PXAPI PXTextFindFirstStringA(const char* PXRestrict const string, const PXSize dataSize, const char* PXRestrict const targetString, const PXSize targetStringSize)
{
    PXSize index = 0;
    PXBool found = 0;

    for(; index < dataSize && !found; ++index)
    {
        found = string[index] == targetString[0];

        if(found)
        {
            const PXSize limitedLength = PXMathMinimumI(dataSize - index, targetStringSize);

            found = PXTextCompareA(&string[index], limitedLength, &targetString[0], limitedLength, 0);
        }
    }

    return found ? index - 1 : PXTextIndexNotFound;
}

PXSize PXAPI PXTextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character)
{
    PXBool found = 0;
    PXSize i = PXTextLengthW(string, dataSize);

    for(; i > 0 && !found; --i)
    {
        found = character == string[i];
    }

    ++i;

    return found ? i + 1 : PXTextIndexNotFound;
}

PXSize PXAPI PXTextFindLast(const PXText PXREF stringSource, const PXText PXREF stringTarget, PXText PXREF stringResult)
{
    PXBool found = 0;
    PXSize i = stringSource->SizeUsed - stringTarget->SizeUsed; // As we start from the back, the symbol can only be as long

    stringResult->SizeAllocated = 0;
    stringResult->SizeUsed = 0;
    //stringResult->NumberOfCharacters = 0;

    if(stringSource->Format == TextFormatASCII && stringTarget->Format == TextFormatASCII) // Is Ascii
    {
        stringResult->Format = TextFormatASCII;

        for(; i > 0 && !found; --i)
        {
            // We can make a mask with a 64-bit view, then ask if we have atleast one match in that block
            // if we don't have a match, we can be sure this block does not contain the target


            // Check if the firsat symbol is the same
            found = stringTarget->A[0] == stringSource->A[i];

            if(found)
            {
                found = PXMemoryCompare(stringTarget->A, stringTarget->SizeUsed, stringSource->A + i, i);
            }
        }

        if(found)
        {
            stringResult->SizeUsed = stringSource->SizeUsed - i - 1;
            //stringResult->NumberOfCharacters = stringSource->SizeUsed - i - 1;
            stringResult->A = stringSource->A + i + 1;

            return i;
        }
    }

    if(stringSource->Format == TextFormatUNICODE && stringTarget->Format == TextFormatUNICODE) // Is Ascii
    {
        stringResult->Format = TextFormatUNICODE;

        for(; i > 0 && !found; --i)
        {
            // We can make a mask with a 64-bit view, then ask if we have atleast one match in that block
            // if we don't have a match, we can be sure this block does not contain the target


            // Check if the firsat symbol is the same
            found = stringTarget->W[0] == stringSource->W[i];

            if(found)
            {
                found = PXMemoryCompare(stringTarget->W, stringTarget->SizeUsed, stringSource->W + i, i);
            }
        }

        if(found)
        {
            stringResult->SizeUsed = stringSource->SizeUsed - i - 1;
            //stringResult->NumberOfCharacters = stringSource->SizeUsed - i - 1;
            stringResult->W = stringSource->W + i + 1;
            return i;
        }
    }

    return PXTextIndexNotFound;
}

void PXAPI PXTextMoveByOffset(PXText PXREF pxText, const PXSize offset)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            if(pxText->SizeAllocated > 0)
            {
                pxText->SizeAllocated -= sizeof(char) * offset;
            }

            pxText->SizeUsed -= sizeof(char) * offset;
            //pxText->NumberOfCharacters -= 1 * offset;
            pxText->A += 1 * offset;
            break;
        }
        case TextFormatUNICODE:
        {
            if(pxText->SizeAllocated > 0)
            {
                pxText->SizeAllocated -= sizeof(wchar_t) * offset;
            }

            pxText->SizeUsed -= sizeof(wchar_t) * offset;
            //pxText->NumberOfCharacters -= 1 * offset;
            pxText->W += 1 * offset;
            break;
        }
    }
}

void PXAPI PXTextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character)
{
    PXSize index = PXTextFindFirstCharacterA(string, dataSize, character);

    if(index != -1)
    {
        string[index] = '\0';
    }
}

void PXAPI PXTextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...)
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
                const PXBool stop = symbol == '\n' || symbol == '\0' || symbol == ' ';
                //const bool skip = symbol == '\n';     if(commandIsNumber && current == '/' || current == ' ' || finished)

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

                const PXSize readBytes = PXTextCopyA(source, offsetLength, destination, -1);

                offsetData += readBytes;

                break;
            }
            case 'i':
            case 'd':
            case 'u':
            {
                int PXREF i = va_arg(args, int*);

                PXText pxTextInt;
                PXTextFromAdressA(&pxTextInt, buffer + offsetData, offsetLength, offsetLength);

                const PXSize readBytes = PXTextToInt(&pxTextInt, i);

#if PXAssertEnable
                assert(readBytes);
#endif

                offsetData += readBytes;

                break;
            }
            case 'f':
            {
                PXF32* number = va_arg(args, PXF32*);
                PXText pxTextPXF32;
                PXTextFromAdressA(&pxTextPXF32, buffer + offsetData, offsetLength, offsetLength);

                const PXSize readBytes = PXTextToF32(&pxTextPXF32, number);

#if PXAssertEnable
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
            const PXBool stop = !(symbol == ' ' || symbol == '\0');

            if(stop)
            {
                break;
            }

            ++offsetData;
        }
    }

    va_end(args);
}

void PXAPI PXTextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize)
{
    unsigned char finished = 0;
    unsigned char foundItem = 0;
    PXSize foundTargets = 0;

    for(size_t i = 0; i < parsingTokenListSize; i++)
    {
        PXMemorySet(parsingTokenList[i].Value, 0, sizeof(void*));
    }

    for(PXSize i = 0; (i < stringSize) && (string[i] != '\0') && !finished; ++i)
    {
        foundItem = 0;

        for(PXSize t = 0; (t < parsingTokenListSize) && (string[i] != ' ') && !foundItem; t++)
        {
            const ParsingTokenA* parsingToken = &parsingTokenList[t];
            const char* targetString = parsingToken->String;
            const PXSize targetStringSize = PXTextLengthA(targetString, -1);
            const char* sourceString = string + i;

            foundItem = PXMemoryCompare(sourceString, stringSize, targetString, targetStringSize); // Compare whole word

            if(foundItem)
            {
                PXSize lengthTag = PXTextLengthUntilA(sourceString, stringSize, '=');
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

PXBool PXAPI PXTextMatchW(const wchar_t* input, const PXSize inputSize, const wchar_t* pattern, const PXSize patternSize)
{
    PXSize patternIndex = 0;

    if(!input || !inputSize || !pattern || !patternSize)
    {
        return PXFalse;
    }

    // If we only have '*' as a pattern, we can just stop.
    const PXBool quitFast = (patternSize == 1) && ((pattern[0] == '*') || (input[0] == pattern[0]));

    if(quitFast)
    {
        return PXTrue;
    }

    for(PXSize index = 0; index < inputSize && input[index] != '\0'; ++index)
    {
        const PXBool isMatch = input[index] == pattern[patternIndex];
        const PXBool isStar = pattern[index] == '*';

        if(isStar)
        {
            ++patternIndex;

            while(input[index] != pattern[patternIndex] && index < inputSize && input[index] != '\0')
            {
                ++index;
            }

            ++patternIndex;
            continue;
        }

        if(!isMatch)
        {
            return PXFalse;
        }
    }

    return PXTrue;
}

PXSize PXAPI PXTextReplace(PXText PXREF pxText, char target, char value)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            PXTextReplaceByte(pxText->A, pxText->SizeUsed, target, value);
            break;
        }
        case TextFormatUNICODE:
        {
            for(size_t i = 0; i < pxText->SizeUsed; i++)
            {
                pxText->W[i] = pxText->W[i] == target ? value : pxText->W[i];
            }

            break;
        }
        default:
            return 0;
    }
}

PXSize PXAPI PXTextReplaceA(char PXREF text, PXSize textSize, const char PXREF target, const PXSize targetSize, const char value)
{
    PXSize cursorSize = textSize;
    char* cursorAdress = text;

    for(;;)
    {
        const PXSize indexStart = PXTextFindFirstCharacterA(cursorAdress, cursorSize, target[0]);

        if(indexStart == -1)
        {
            break;
        }

        char* indexAdress = &text[indexStart];
        const PXSize indexSize = PXMathMinimumIU(textSize - indexStart, targetSize);

        const PXBool doesMatch = PXTextCompareA(indexAdress, indexSize, target, targetSize, 0);

        if(!doesMatch)
        {
            break;
        }

        PXMemorySet(indexAdress, value, indexSize);

        cursorAdress = indexAdress + targetSize;
    }

    return textSize;
}

PXSize PXAPI PXTextReplaceByte(char PXREF text, PXSize textSize, char target, char value)
{
    if(!(text && textSize > 0) && ((PXSize)-1 != textSize))
    {
        return 0;
    }

    // Would not change the state
    if(target == value)
    {
        return textSize; // Done
    }

    PXSize counter = 0;

    for(PXSize i = 0; i < textSize; ++i)
    {
        const char isSame = target == text[i];

        if(isSame)
        {
            text[i] = value;
            ++counter;
        }
    }

    return counter;
}

PXResult PXAPI PXTextFromAdress(PXText PXREF pxText, void* address, const PXSize sizeUsed, const PXSize sizeAllocated, const PXTextFormat pxTextFormat)
{
    PXBool isLenghKnown = (PXSize)PXTextLengthUnkown != sizeUsed;

    if(!isLenghKnown)
    {
        switch(pxTextFormat)
        {
            case TextFormatASCII:
            {
                pxText->SizeAllocated = PXTextLengthA(address, sizeAllocated);
                break;
            }
            case TextFormatUNICODE:
            {
                pxText->SizeAllocated = PXTextLengthW(address, sizeAllocated);
                break;
            }
            default:
                return PXResultRefusedParameterInvalid;
        }
      
        pxText->SizeUsed = pxText->SizeAllocated;
       // pxText->NumberOfCharacters = pxText->SizeAllocated;
    }
    else
    {
        pxText->SizeAllocated = sizeAllocated;
        pxText->SizeUsed = sizeUsed;
       // pxText->NumberOfCharacters = sizeUsed;
    }
    pxText->Format = pxTextFormat;
    pxText->Data = address;



    return PXResultOK;
}

PXResult PXAPI PXTextFromAdressA(PXText PXREF pxText, const char* address, const PXSize sizeUsed, const PXSize sizeAllocated)
{
    return PXTextFromAdress(pxText, (char*)address, sizeUsed, sizeAllocated, TextFormatASCII);
}

PXResult PXAPI PXTextFromAdressW(PXText PXREF pxText, const wchar_t* address, const PXSize sizeUsed, const PXSize sizeAllocated)
{
    return PXTextFromAdress(pxText, (char*)address, sizeUsed, sizeAllocated, TextFormatUNICODE);
}

PXResult PXAPI PXTextCreateCopy(PXText PXREF pxText, const PXText PXREF pxTextSource)
{
    *pxText = *pxTextSource;

    pxText->Data = PXMemoryHeapCallocT(PXByte, pxTextSource->SizeUsed+1);
    PXTextCopy(pxTextSource, pxText);

    return PXResultOK;
}

PXResult PXAPI PXTextDestroy(PXText PXREF pxText)
{
    PXMemoryHeapFree(PXNull, pxText->A);
    PXClear(PXText, pxText);

    return PXResultOK;
}

PXSize PXAPI PXTextFromInt(PXText PXREF pxText, int number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            PXBool isSigned = PXFalse;
            pxText->SizeUsed = 0; // Current offset of the data buffer

            if(number < 0) // number negative
            {
                isSigned = PXTrue; // Save state that we have a negative number
                number *= -1; // Remove negative value
            }

            do
            {
                pxText->A[pxText->SizeUsed++] = number % 10 + '0'; // Get the value of the most right digit and convert to ASCII-Number
            } while((number /= 10) > 0);   // Remove the most right digit by interget division and check if we still have a number to process

            if(isSigned) // if we had a minus, add it now
            {
                pxText->A[pxText->SizeUsed++] = '-'; // Add the minus
            }

            pxText->A[pxText->SizeUsed] = '\0'; // Add the termination byte

            // Reverse the order of the string
            {
                const PXSize halfSize = pxText->SizeUsed / 2u;

                for(PXSize i = 0; i < halfSize; i++)
                {
                    const PXSize index = pxText->SizeUsed - 1 - i; // index from end position, before the \0
                    const PXByte temp = pxText->A[index];

                    pxText->A[index] = pxText->A[i];
                    pxText->A[i] = temp;
                }
            }

            //itoa(number, string, dataSize);

            //int bytesWritten = PrintSN(string, dataSize, "%i", number);

            return pxText->SizeUsed;
        }


        case TextFormatUNICODE:
        {
            PXText pxA;
            PXTextConstructBufferA(&pxA, 16);

            PXTextFromInt(&pxA, number);

            PXTextCopyAW(pxA.A, pxA.SizeUsed, pxText->W, pxText->SizeAllocated);

            return pxText->SizeUsed;
        }
        default:
            return 0;
    }
}

PXSize PXAPI PXTextFromBool(PXText PXREF pxText, const PXBool number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            pxText->A[0] = number ? '1' : '0';

            return sizeof(char);
        }


        case TextFormatUNICODE:
        {
            pxText->W[0] = number ? '1' : '0';

            return sizeof(wchar_t);
        }
        default:
            return 0;
    }
}

PXSize PXAPI PXTextFromPXF32(PXText PXREF pxText, const PXF32 number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
#if OSUnix
            pxText->SizeUsed = snprintf(pxText->A, pxText->SizeAllocated, "%f", number);
#elif OSWindows
            //pxText->SizeUsed = sprintf_s(pxText->A, pxText->SizeAllocated, "%f", number);
#endif
            return pxText->SizeUsed;
        }


        case TextFormatUNICODE:
        {
#if OSUnix
            pxText->SizeUsed = snprintf(pxText->W, pxText->SizeAllocated, "%f", number);
#elif OSWindows
            //pxText->SizeUsed = sprintf_s(pxText->W, pxText->SizeAllocated, "%f", number);
#endif

            return pxText->SizeUsed;
        }
        default:
            return 0;
    }
}

PXBool PXAPI PXTextDemote(PXText PXREF pxText)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
            return PXFalse;

        case TextFormatUNICODE:
        {
            PXBool isCollapseable = PXTrue;

            for(PXSize i = 0; i < pxText->SizeUsed; i++)
            {
                if(pxText->W[i] >= 0xFF)
                {
                    isCollapseable = PXFalse;
                    break;
                }
            }

            if(isCollapseable)
            {
                for(PXSize i = 0; i < pxText->SizeUsed; i++)
                {
                    pxText->A[i] = pxText->W[i];
                }
                pxText->Format = TextFormatASCII;
            }

            break;
        }

        default:
            return PXFalse;
    }

    return PXTrue;
}

PXBool PXAPI PXTextIsPrintableA(const PXASCII c)
{
    return 0x20 <= c && c <= 0x7E;
}

PXBool PXAPI PXTextIsLetterCaseLower(const PXASCII c)
{
    return ((c & 0b00100000) >> 5);
}
PXBool PXAPI PXTextIsLetterCaseUpper(const PXASCII c)
{
    return !PXTextIsLetterCaseLower(c);
}
PXBool PXAPI PXTextCompareCaseIgnore(const PXASCII a, const PXASCII b)
{
    return (PXTextMakeCaseLower(a) == b) || (PXTextMakeCaseUpper(a) == b);
}
PXBool PXAPI PXTextIsEndOfString(const PXASCII c)
{
    return c == '\0';
}
PXBool PXAPI PXTextIsTab(const PXASCII c)
{
    return c == '\t';
}
PXBool PXAPI PXTextIsEmptySpace(const PXASCII c)
{
    return c == ' ';
}
PXBool PXAPI PXTextIsEndOfLine(const PXASCII c)
{
    return c == '\r' || c == '\n';
}
PXBool PXAPI PXTextIsEmpty(const PXASCII c)
{
    return (PXTextIsTab(c) || PXTextIsEndOfLine(c) || PXTextIsEmptySpace(c) || PXTextIsEndOfString(c));
}
PXBool PXAPI PXTextIsAllowedForF32(const PXASCII c)
{
    return ('0' <= c && c <= '9' || c == '-' || c == '+' || c == '.' || c == 'e'); // e=exponent
}

PXASCII PXAPI PXTextMakeCaseLower(const PXASCII c)
{
    return (c | 0b00100000);
}
PXASCII PXAPI PXTextMakeCaseUpper(const PXASCII c)
{
    PXBool isOK = 'a' <= c && c <= 'z';

    if(isOK)
    {
        return c;
    }

    return c & 0b11011111;
}

PXASCII PXAPI PXTextUnicodeToASCII(const PXASCII wc)
{
    return (wc <= 0xFF ? (char)wc : '?');
}
PXASCII PXAPI PXTextMakePrintable(const PXASCII c)
{
    return (0x20 <= c && c <= 0x7E) ? c : '.';
}

PXSize PXAPI PXTextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize)
{
    const PXSize length = PXMathMinimumIU(dataSize, stringSize);
    PXSize outputIndex = 0;

    PXMemoryClear(string, stringSize);

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

PXSize PXAPI PXTextToInt(const PXText PXREF pxText, int PXREF number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            return PXTextToIntA(pxText->A, pxText->SizeUsed, number);
        }
        case TextFormatUNICODE:
        {
            return sizeof(wchar_t);
        }
    }

    return 0;
}

PXSize PXAPI PXTextToIntA(const char PXREF text, const PXSize textSize, int PXREF number)
{
    int accumulator = 0;
    PXSize index = 0;
    PXBool isNegative = 0;

    if(!text)
    {
        return 0;
    }

    if(text[0] == '-')
    {
        index++;
        isNegative = 1u;
    }

    while(text[index] != '\0')
    {
        char character = text[index];
        const PXBool isValidCharacter = (character >= '0' && character <= '9');
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

PXSize PXAPI PXTextToBool(const PXText PXREF pxText, PXBool PXREF number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            switch(pxText->A[0])
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

PXSize PXAPI PXTextToF32(const PXText PXREF pxText, PXF32 PXREF number)
{
    PXF64 pxF64 = 0;

    PXSize res = PXTextToF64A(pxText, &pxF64);

    *number = pxF64;

    return res;
}

PXSize PXAPI PXTextToF64A(const PXText PXREF pxText, PXF64 PXREF number)
{
    switch(pxText->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            PXI64S accumulator = 0;
            PXSize digitsAfterDot = 0;
            PXSize index = 0;
            unsigned char isNegative = 0;
            unsigned char isWholeNumberChunk = 1;

            if(!pxText)
            {
                return 0;
            }

            if(pxText->A[0] == '-')
            {
                index++;
                isNegative = 1;
            }

            for(; pxText->A[index] != '\0'; ++index)
            {
                const char character = pxText->A[index];
                const PXBool isDot = character == '.';
                const PXBool isValidCharacter = (character >= '0' && character <= '9') || isDot;
                const int numberElement = character - '0';

                if(!isValidCharacter)
                {
                    // float could end with a "e" for exponent, this needs to be detected
                    if(character == 'e')
                    {
                        int exponent = 0;

                        index += PXTextToIntA(&pxText->A[index + 1], pxText->SizeUsed - (index + 1), &exponent);
                        ++index;

                        digitsAfterDot += exponent * -1;

                        /*

                        // Caluclate expontent
                        if(exponent <= 0)
                        {
                            exponent *= -1;
                            digitsAfterDot += exponent;
                        }
                        else
                        {
                            digitsAfterDot -= exponent;
                        }
                        */


                        break; // Quit
                    }

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
                    ++digitsAfterDot;
                }
            }

            if(isNegative)
            {
                accumulator *= -1;
            }

            //double stdResult = std::strtof(string, 0); // STD Method

            // Calculate
            {
                const PXF64 a = accumulator;
                const PXF64 b = PXMathPowerI64U(10, digitsAfterDot);
                const PXF64 c = a / b;

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

#define PXSizeKB 1000
#define PXSizeMB 1000000
#define PXSizeGB 1000000000
#define PXSizeTB 1000000000000
#define PXSizePB 1000000000000000
#define PXSizeEB 1000000000000000000

#include <PX/OS/Time/PXTime.h>

PXSize PXAPI PXTextFormatDateTime(PXText PXREF pxText, const PXTime PXREF pxTime)
{
    // YYYY.MM.DD - HH.MM.SS
    return  PXTextPrint
    (
        pxText,
        "%4.4i.%2.2i.%2.2i - %2.2i:%2.2i:%2.2i",
        pxTime->Year,
        pxTime->Month,
        pxTime->Day,
        pxTime->Hour,
        pxTime->Minute,
        pxTime->Second
    );
}

PXSize PXAPI PXTextFormatTime(PXText PXREF pxText, const PXSize pxTime)
{
    if(pxTime < 1000)
    {
        return PXTextPrint(pxText, "%i us", pxTime);
    }

    if(pxTime < 1000000)
    {
        return PXTextPrint(pxText, "%.2f ms", pxTime / (PXF32)1000);
    }

    if(pxTime < 1000000000)
    {
        return PXTextPrint(pxText, "%.2f  s", pxTime / (PXF32)1000000);
    }

    return 0;

#if 0
    if(pxTime < PXSizeTB)
    {
        return PXTextPrint(pxText, "%.2f m", pxTime / (PXF32)PXSizeGB);
    }

    if(pxTime < PXSizePB)
    {
        return PXTextPrint(pxText, "%.2f h", pxTime / (PXF32)PXSizeTB);
    }

    return PXTextPrint(pxText, "%.2f d", pxTime / (PXF32)PXSizePB);
#endif
}

PXSize PXAPI PXTextFormatSize(PXText PXREF pxText, const PXSize pxSize)
{
    if(pxSize < PXSizeKB)
    {
        return PXTextPrint(pxText, "%i.-  B", pxSize);
    }

    if(pxSize < PXSizeMB)
    {
        return PXTextPrint(pxText, "%.1f KB", pxSize / (PXF32)PXSizeKB);
    }

    if(pxSize < PXSizeGB)
    {
        return PXTextPrint(pxText, "%.1f MB", pxSize / (PXF32)PXSizeMB);
    }

    if(pxSize < PXSizeTB)
    {
        return PXTextPrint(pxText, "%.1f GB", pxSize / (PXF32)PXSizeGB);
    }

    if(pxSize < PXSizePB)
    {
        return PXTextPrint(pxText, "%.1f TB", pxSize / (PXF32)PXSizeTB);
    }

    if(pxSize < PXSizeEB)
    {
        return PXTextPrint(pxText, "%.1f PB", pxSize / (PXF32)PXSizePB);
    }

    return PXTextPrint(pxText, "%.1f EB", pxSize / (PXF32)PXSizeEB);
}

PXSize PXAPI PXTextFormatData(PXText PXREF pxText, const void* data, const PXSize dataSize)
{
    for(PXSize i = 0; i < dataSize; i++)
    {
        const char dataChar = PXTextMakePrintable(((char*)data)[i]);

        pxText->A[pxText->SizeUsed] = dataChar;
        pxText->SizeUsed += 1;
    }

    pxText->A[pxText->SizeUsed + 1] = 0;

    return dataSize;
}