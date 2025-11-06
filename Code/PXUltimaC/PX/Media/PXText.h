#pragma once

#ifndef PXTextINLCUDE
#define PXTextINLCUDE

//#include <PX/Media/PXType.h>
#include <stdarg.h>

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXActionResult.h>

#define PXTextUnkownLength -1
#define PXTextIndexNotFound -1


typedef struct PXTime_ PXTime;


typedef struct ParsingTokenA_
{
    const char* String;
    const char** Value;
}
ParsingTokenA;

typedef enum PXTextFormat_
{
    TextFormatInvalid,
    TextFormatASCII,    // 1 Byte per character, range from 0 to 255
    TextFormatUNICODE,  // 2 Bytes per character, range from 0 to 65535
    TextFormatUTF8,     // Variable length of characters, 1 to 4 Bytes
    TextFormatUTF16
}
PXTextFormat;


typedef char PXASCII; // ASCII is always 1-Byte per character
typedef wchar_t PXUNICODE; // UNICODE is always 2-Byte per character
typedef char PXUTF8; // Varraiable size 1-4 Bytes per character

typedef struct PXText_
{
    PXSize SizeAllocated; // [8 Byte, Offset 0] Size that the buffer ponited to has
    PXSize SizeUsed; // [8 Byte, Offset 8]
    PXSize NumberOfCharacters;  // [8 Byte, Offset 16]

    union
    {
        PXASCII* A; // [8 Byte, Offset 24]
        PXUNICODE* W; // [8 Byte, Offset 24]
        PXUTF8* U;
    };

    PXTextFormat Format; // [4 Byte, Offset 32]
}
PXText;

//const int xx = sizeof(PXText);
//const int xxss = sizeof(PXTextFormat);



#define PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize, format)\
        char bufferCacheName[bufferCacheSize];\
        (pxText)->SizeAllocated = sizeof(bufferCacheName);\
        (pxText)->SizeUsed = 0;\
        (pxText)->NumberOfCharacters = 0;\
        (pxText)->Format = format;\
        (pxText)->A = bufferCacheName;\
        PXMemorySet((pxText)->A, '\0', (pxText)->SizeAllocated);


#define PXTextConstructNamedBufferA(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize, TextFormatASCII)
#define PXTextConstructNamedBufferW(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize*2, TextFormatUNICODE)


#define PXTextConstructBufferA(pxText, bufferSize) PXTextConstructNamedBufferA(pxText, __pxUnnamedInternalBuffer, bufferSize);
#define PXTextConstructBufferW(pxText, bufferSize) PXTextConstructNamedBufferW(pxText, __pxUnnamedInternalBuffer, bufferSize);


#define PXTextLengthUnkown -1

PXPublic PXResult PXAPI PXTextFromAdress(PXText PXREF pxText, void* address, const PXSize sizeUsed, const PXSize sizeAllocated, const PXTextFormat pxTextFormat);
PXPublic PXResult PXAPI PXTextFromAdressA(PXText PXREF pxText, void* address, const PXSize sizeUsed, const PXSize sizeAllocated);
PXPublic PXResult PXAPI PXTextFromAdressW(PXText PXREF pxText, void* address, const PXSize sizeUsed, const PXSize sizeAllocated);


#define PXTextMakeFixedC(pxText, c)\
        char character = c; \
        (pxText)->SizeAllocated = sizeof(character);\
        (pxText)->SizeUsed = sizeof(character);\
        (pxText)->NumberOfCharacters = sizeof(character);\
        (pxText)->Format = TextFormatASCII;\
        (pxText)->A = &character;

#define PXTextMakeFixedA(pxText, s)\
        char text[] = s;\
        (pxText)->SizeAllocated = sizeof(text);\
        (pxText)->SizeUsed = (pxText)->SizeAllocated;\
        (pxText)->NumberOfCharacters = (pxText)->SizeAllocated;\
        (pxText)->Format = TextFormatASCII;\
        (pxText)->A = text;

#define PXTextMakeFixedGlobalA(pxText, s) \
        (pxText)->A = s; \
        (pxText)->SizeAllocated = PXTextLengthA((pxText)->A, PXTextLengthUnkown);\
        (pxText)->SizeUsed = (pxText)->SizeAllocated;\
        (pxText)->NumberOfCharacters = (pxText)->SizeAllocated;\
        (pxText)->Format = TextFormatASCII;\


#define PXTextMakeFixedNamed(pxText, name, s, format)\
        char name[] = s;\
        (pxText)->SizeAllocated = sizeof(name) - 1u;\
        (pxText)->SizeUsed = (pxText)->SizeAllocated;\
        (pxText)->NumberOfCharacters = 0;\
        (pxText)->Format = format;\
        (pxText)->A = name;

#define PXTextMakeFixedNamedA(pxText, name, str) PXTextMakeFixedNamed(pxText, name, str, TextFormatASCII);
#define PXTextMakeFixedNamedW(pxText, name, str) PXTextMakeFixedNamed(pxText, name, str, TextFormatUNICODE);

#define PXTextMakeFixedW(pxText, s)\
        wchar_t text[] = s;\
        (pxText)->SizeAllocated = sizeof(text);\
        (pxText)->SizeUsed = sizeof(text);\
        (pxText)->NumberOfCharacters = sizeof(text) / 2;\
        (pxText)->Format = TextFormatUNICODE;\
        (pxText)->A = text;


PXPublic PXResult PXAPI PXTextCreateCopy(PXText PXREF pxText, const PXText PXREF pxTextSource);
PXPublic PXResult PXAPI PXTextDestroy(PXText PXREF pxText);

PXPublic PXSize PXAPI PXTextFromInt(PXText PXREF pxText, int number);
PXPublic PXSize PXAPI PXTextFromBool(PXText PXREF pxText, const PXBool number);
PXPublic PXSize PXAPI PXTextFromPXF32(PXText PXREF pxText, const PXF32 number);


// If a text is fetched with UNICODE, check if we can demote this to ASCII.
// This shall reduce memory use. Questionable if useful.
PXPublic PXBool PXAPI PXTextDemote(PXText PXREF pxText);


PXPublic PXBool PXAPI PXTextIsPrintableA(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsLetterCaseLower(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsLetterCaseUpper(const PXASCII c);
PXPublic PXBool PXAPI PXTextCompareCaseIgnore(const PXASCII a, const PXASCII b);
PXPublic PXBool PXAPI PXTextIsEndOfString(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsTab(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsEmptySpace(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsEndOfLine(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsEmpty(const PXASCII c);
PXPublic PXBool PXAPI PXTextIsAllowedForF32(const PXASCII c);

PXPublic PXASCII PXAPI PXTextMakeCaseLower(const PXASCII c);
PXPublic PXASCII PXAPI PXTextMakeCaseUpper(const PXASCII c);

PXPublic PXASCII PXAPI PXTextUnicodeToASCII(const PXASCII wc);
PXPublic PXASCII PXAPI PXTextMakePrintable(const PXASCII c);




PXPublic PXSize PXAPI PXTextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize);

PXPublic PXSize PXAPI PXTextToInt(const PXText PXREF pxText, int PXREF number);
PXPublic PXSize PXAPI PXTextToIntA(const char PXREF text, const PXSize textSize, int PXREF number);
PXPublic PXSize PXAPI PXTextToBool(const PXText PXREF pxText, PXBool PXREF number);
PXPublic PXSize PXAPI PXTextToF32(const PXText PXREF pxText, PXF32 PXREF number);
PXPublic PXSize PXAPI PXTextToF64A(const PXText PXREF pxText, PXF64 PXREF number);

PXPublic PXSize PXAPI PXTextFormatDateTime(PXText PXREF pxText, const PXTime PXREF pxTime);
PXPublic PXSize PXAPI PXTextFormatTime(PXText PXREF pxText, const PXSize pxTime);
PXPublic PXSize PXAPI PXTextFormatSize(PXText PXREF pxText, const PXSize pxSize);
PXPublic PXSize PXAPI PXTextFormatData(PXText PXREF pxText, const void* data, const PXSize dataSize);

// Print a string from a non terminated string into a buffer
// Also converts unprintable characters into printable ones
PXPublic PXSize PXAPI PXTextFromNonTerminated(char PXREF stringOutput, const PXSize stringOutputSize, const char PXREF stringInput, const PXSize stringInputSize);

PXPublic PXSize PXAPI PXTextFromIntToBinary8U(char PXREF string, const PXSize dataSize, const PXI8U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary16U(char PXREF string, const PXSize dataSize, const PXI16U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary32U(char PXREF string, const PXSize dataSize, const PXI32U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary64U(char PXREF string, const PXSize dataSize, const PXI64U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary64UR(char PXREF string, const PXSize dataSize, const PXI64U number, const unsigned char numberOfDigits);

PXPublic PXSize PXAPI PXTextFromIntToBinary(char PXREF string, const PXSize dataSize, const void PXREF data, const unsigned char numberOfDigits);

PXPublic PXSize PXAPI PXTextToLowerCase(const PXText PXREF pxTextSource, PXText PXREF pxTextTarget);
PXPublic PXSize PXAPI PXTextToUpperCase(const PXText PXREF pxTextSource, PXText PXREF pxTextTarget);

PXPublic PXSize PXAPI PXTextTrimA(char PXREF text, const PXSize textSize);

PXPublic PXSize PXAPI PXAppend(PXText PXREF currentString, const PXText PXREF appendingString);
PXPublic PXSize PXAPI PXAppendA(PXText PXREF currentString, const char PXREF appaendString, const char appaendStringSize);
PXPublic PXSize PXAPI PXAppendW(wchar_t PXREF dataString, const PXSize dataStringSize, const wchar_t PXREF appaendString, const PXSize appaendStringSize);
PXPublic PXSize PXAPI PXAppendF(PXText PXREF pxText, const char PXREF fomat, ...);

PXPublic PXSize PXAPI PXTextPrint(PXText PXREF pxText, const char* style, ...);
PXPublic PXSize PXAPI PXTextPrintA(char PXREF text, const PXSize size, const char* style, ...);
PXPublic PXSize PXAPI PXTextPrintW(wchar_t PXREF text, const PXSize size, const char* style, ...);
PXPublic PXSize PXAPI PXTextPrintAV(char PXREF text, const PXSize size, const char* style, va_list parameter);
PXPublic PXSize PXAPI PXTextPrintWV(wchar_t PXREF text, const PXSize size, const char* style, va_list parameter);
PXPublic PXSize PXAPI PXTextPrintV(PXText PXREF pxText, const char* style, va_list parameter);

PXPublic PXSize PXAPI PXTextClear(PXText PXREF pxText);

PXPublic void PXAPI PXAdvance(PXText PXREF pxText, const PXSize advanceBy);

PXPublic void PXAPI PXTextRevise(PXText PXREF pxText);

PXPublic PXSize PXAPI PXTextLengthA(const char* string, const PXSize stringSize);
PXPublic PXSize PXAPI PXTextLengthW(const wchar_t* string, const PXSize stringSize);

PXPublic PXSize PXAPI PXTextLengthUntilA(const char* string, const PXSize stringSize, const char character);
PXPublic PXSize PXAPI PXTextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character);

PXPublic PXSize PXAPI PXTextCopy(const PXText PXREF source, PXText PXREF destination);

PXPublic PXSize PXAPI PXTextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);

PXPublic PXSize PXAPI PXTextCopyAU(const char PXREF source, const PXSize sourceLength, char PXREF destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyWU(const wchar_t PXREF source, const PXSize sourceLength, char PXREF destination, const PXSize destinationLength);

PXPublic PXSize PXAPI PXTextCountA(const char* PXText, const PXSize PXTextSize, const char target);
PXPublic PXSize PXAPI PXTextCountW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target);

PXPublic PXSize PXAPI PXTextPascalCaseCleave(const char* pxText, const PXSize PXTextSize);


PXPublic PXSize PXAPI PXTextCountUntilA(const char* PXText, const PXSize PXTextSize, const char target, const char stopAt);
PXPublic PXSize PXAPI PXTextCountUntilW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target, const wchar_t stopAt);

PXPublic PXBool PXAPI PXTextCompare(const PXText PXREF A, const PXText PXREF textB);

PXPublic PXI8U PXAPI PXTextCompareAVI8(const char* a, PXI8U aSize, const char* PXREF stringList, const PXI8U* stringListSize, const PXI8U amount);


#define PXTextCompareRequireSameLength (1<<0)

PXPublic PXBool PXAPI PXTextCompareA(const char* a, PXSize aSize, const char* b, PXSize bSize, const PXI8U flags);
PXPublic PXBool PXAPI PXTextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic PXBool PXAPI PXTextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic PXBool PXAPI PXTextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

PXPublic char PXAPI PXTextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

PXPublic char* PXAPI PXTextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize);

PXPublic PXSize PXAPI PXTextFindLastCharacter(const PXText PXREF pxText, const char character);
PXPublic PXSize PXAPI PXTextFindLastCharacterA(const char PXREF text, const PXSize textSize, const char character);

PXPublic PXSize PXAPI PXTextFindFirstCharacter(const PXText PXREF pxText, const char character);

PXPublic PXSize PXAPI PXTextFindFirstCharacterA(const char* PXRestrict const string, const PXSize dataSize, const char character);
PXPublic PXSize PXAPI PXTextFindFirstCharacterBeforeA(const char* PXRestrict const string, const PXSize dataSize, const char target, const char barrier);
PXPublic PXSize PXAPI PXTextFindFirstCharacterOfListA(const char* PXRestrict const string, const PXSize dataSize, const char* characterList, const PXSize characterListSize);

PXPublic PXSize PXAPI PXTextFindFirstStringA(const char* __restrict string, const PXSize dataSize, const char* PXRestrict const targetString, const PXSize targetStringSize);
PXPublic PXSize PXAPI PXTextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character);

PXPublic PXSize PXAPI PXTextFindLast(const PXText PXREF stringSource, const PXText PXREF stringTarget, PXText PXREF stringResult);

PXPublic void PXAPI PXTextMoveByOffset(PXText PXREF pxText, const PXSize offset);

PXPublic void PXAPI PXTextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character);

PXPublic void PXAPI PXTextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...);

PXPublic void PXAPI PXTextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize);

PXPublic PXBool PXAPI PXTextMatchW(const wchar_t* input, const PXSize inputSize, const wchar_t* pattern, const PXSize patternSize);

PXPublic PXSize PXAPI PXTextReplace(PXText PXREF pxText, char target, char value);
PXPublic PXSize PXAPI PXTextReplaceByte(char PXREF text, PXSize textSize, char target, char value);
PXPublic PXSize PXAPI PXTextReplaceA(char PXREF text, PXSize textSize, const char PXREF target, const PXSize targetSize, const char value);

#endif