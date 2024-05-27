#ifndef PXTextINLCUDE
#define PXTextINLCUDE

#include <Media/PXType.h>
#include <stdarg.h>

//---<Makros>------------------------------------------------------------------

#define PXTextIsLetterCaseLower(character) ((character & 0b00100000) >> 5)
#define PXTextIsLetterCaseUpper(character) !PXTextIsLetterCaseLower(character)


#define MakeLetterCaseLower(character) (character | 0b00100000)
#define MakeLetterCaseUpper(c) (('a' <= c && c <= 'z') ? c & 0b11011111 : c)
#define CompareLetterCaseIgnore(a, b) (MakeLetterCaseLower(a) == b) || (MakeLetterCaseUpper(a) == b)
#define UnicodeToASCII(wc) (wc <= 0xFF ? (char)wc : '?')
#define IsEndOfString(c) (c == '\0')
#define IsTab(c) (c == '\t')
#define IsEmptySpace(c) (c == ' ')
#define IsEndOfLineCharacter(c) (c == '\r' || c == '\n')

#define PXTextFloatIsAllowedCharacter(c)  ('0' <= c && c <= '9' || c == '-' || c == '+' || c == '.')

#define IsPrintable(c) (0x20 <= c && c <= 0x7E)
#define MakePrintable(c) ((0x20 <= c && c <= 0x7E) ? + c : '.')
//-----------------------------------------------------------------------------

#define PXTextUnkownLength -1
#define PXTextIndexNotFound -1




typedef struct ParsingTokenA_
{
	const char* String;
	const char** Value;
}
ParsingTokenA;

typedef enum PXTextFormat_
{
	TextFormatInvalid,
	TextFormatASCII, 	// 1 Byte per character, range from 0 to 255	
	TextFormatUNICODE, 	// 2 Bytes per character, range from 0 to 65535	
	TextFormatUTF8, 	// Variable length of characters, 1 to 4 Bytes
	TextFormatUTF16
}
PXTextFormat;

typedef struct PXText_
{
	PXSize SizeAllocated; // [8 Byte, Offset 0] Size that the buffer ponited to has
	PXSize SizeUsed; // [8 Byte, Offset 8]
	PXSize NumberOfCharacters;  // [8 Byte, Offset 16]

	union
	{
		char* TextA; // [8 Byte, Offset 24]
		wchar_t* TextW; // [8 Byte, Offset 24]
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
		(pxText)->TextA = bufferCacheName;\
		PXMemorySet((pxText)->TextA, '\0', (pxText)->SizeAllocated);


#define PXTextConstructNamedBufferA(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize, TextFormatASCII)
#define PXTextConstructNamedBufferW(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize*2, TextFormatUNICODE)


#define PXTextConstructBufferA(pxText, bufferSize) PXTextConstructNamedBufferA(pxText, __pxUnnamedInternalBuffer, bufferSize);
#define PXTextConstructBufferW(pxText, bufferSize) PXTextConstructNamedBufferW(pxText, __pxUnnamedInternalBuffer, bufferSize);



#define PXTextConstructFromAdress(pxText, address, sizeUsed, sizeAllocated, format) \
		if((PXSize)sizeAllocated == (PXSize)PXTextLengthUnkown) \
		{ \
			(pxText)->SizeAllocated = PXTextLengthA(address, (PXSize)PXTextLengthUnkown); \
			(pxText)->SizeUsed = (pxText)->SizeAllocated; \
			(pxText)->NumberOfCharacters = (pxText)->SizeAllocated; \
		} \
		else \
		{ \
			(pxText)->SizeAllocated = sizeAllocated; \
			(pxText)->SizeUsed = sizeUsed; \
			(pxText)->NumberOfCharacters = sizeUsed; \
		} \
		(pxText)->Format = format; \
		(pxText)->TextA = (char*)(address);

#define PXTextLengthUnkown -1

#define PXTextConstructFromAdressA(pxText, address, sizeUsed, sizeAllocated) PXTextConstructFromAdress(pxText, (char*)address, sizeUsed, sizeAllocated, TextFormatASCII)
#define PXTextConstructFromAdressW(pxText, address, sizeUsed, sizeAllocated) PXTextConstructFromAdress(pxText, (char*)address, sizeUsed, sizeAllocated, TextFormatUNICODE)

#define PXTextMakeFixedC(pxText, c)\
		char character = c; \
		(pxText)->SizeAllocated = sizeof(character);\
		(pxText)->SizeUsed = sizeof(character);\
		(pxText)->NumberOfCharacters = sizeof(character);\
		(pxText)->Format = TextFormatASCII;\
		(pxText)->TextA = &character;

#define PXTextMakeFixedA(pxText, s)\
		char text[] = s;\
		(pxText)->SizeAllocated = sizeof(text);\
		(pxText)->SizeUsed = (pxText)->SizeAllocated;\
		(pxText)->NumberOfCharacters = (pxText)->SizeAllocated;\
		(pxText)->Format = TextFormatASCII;\
		(pxText)->TextA = text;

#define PXTextMakeFixedGlobalA(pxText, s) \
		(pxText)->TextA = s; \
		(pxText)->SizeAllocated = PXTextLengthA((pxText)->TextA, PXTextLengthUnkown);\
		(pxText)->SizeUsed = (pxText)->SizeAllocated;\
		(pxText)->NumberOfCharacters = (pxText)->SizeAllocated;\
		(pxText)->Format = TextFormatASCII;\


#define PXTextMakeFixedNamed(pxText, name, s, format)\
		char name[] = s;\
		(pxText)->SizeAllocated = sizeof(name) - 1u;\
		(pxText)->SizeUsed = (pxText)->SizeAllocated;\
		(pxText)->NumberOfCharacters = 0;\
		(pxText)->Format = format;\
		(pxText)->TextA = name;

#define PXTextMakeFixedNamedA(pxText, name, str) PXTextMakeFixedNamed(pxText, name, str, TextFormatASCII);
#define PXTextMakeFixedNamedW(pxText, name, str) PXTextMakeFixedNamed(pxText, name, str, TextFormatUNICODE);

#define PXTextMakeFixedW(pxText, s)\
		wchar_t text[] = s;\
		(pxText)->SizeAllocated = sizeof(text);\
		(pxText)->SizeUsed = sizeof(text);\
		(pxText)->NumberOfCharacters = sizeof(text) / 2;\
		(pxText)->Format = TextFormatUNICODE;\
		(pxText)->TextA = text;


PXPublic enum PXActionResult_ PXAPI PXTextCreateCopy(PXText* const pxText, const PXText* const pxTextSource);
PXPublic enum PXActionResult_ PXAPI PXTextDestroy(PXText* const pxText);

PXPublic PXSize PXAPI PXTextFromInt(PXText* const pxText, int number);
PXPublic PXSize PXAPI PXTextFromBool(PXText* const pxText, const PXBool number);
PXPublic PXSize PXAPI PXTextFromFloat(PXText* const pxText, const float number);

PXPublic PXSize PXAPI PXTextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize);

PXPublic PXSize PXAPI PXTextToInt(const PXText* const pxText, int* const number);
PXPublic PXSize PXAPI PXTextToBool(const PXText* const pxText, PXBool* const number);
PXPublic PXSize PXAPI PXTextToFloat(const PXText* const pxText, float* const number);

PXPublic PXSize PXAPI PXTextFormatTime(PXText* const pxText, const PXSize pxTime);
PXPublic PXSize PXAPI PXTextFormatSize(PXText* const pxText, const PXSize pxSize);
PXPublic PXSize PXAPI PXTextFormatData(PXText* const pxText, const void* data, const PXSize dataSize);

PXPublic PXSize PXAPI PXTextFromIntToBinary8U(char* const string, const PXSize dataSize, const PXInt8U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary16U(char* const string, const PXSize dataSize, const PXInt16U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary32U(char* const string, const PXSize dataSize, const PXInt32U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary64U(char* const string, const PXSize dataSize, const PXInt64U number);
PXPublic PXSize PXAPI PXTextFromIntToBinary64UR(char* const string, const PXSize dataSize, const PXInt64U number, const unsigned char numberOfDigits);

PXPublic PXSize PXAPI PXTextToLowerCase(const PXText* const pxTextSource, PXText* const pxTextTarget);
PXPublic PXSize PXAPI PXTextToUpperCase(const PXText* const pxTextSource, PXText* const pxTextTarget);

PXPublic PXSize PXAPI PXTextAppend(PXText* const currentString, const PXText* const appendingString);
PXPublic PXSize PXAPI PXTextAppendA(PXText* const currentString, const char* const appaendString, const char appaendStringSize);
PXPublic PXSize PXAPI PXTextAppendW(wchar_t* const dataString, const PXSize dataStringSize, const wchar_t* const appaendString, const PXSize appaendStringSize);
PXPublic PXSize PXAPI PXTextAppendF(PXText* const pxText, const char* const fomat, ...);

PXPublic PXSize PXAPI PXTextPrint(PXText* const pxText, const char* style, ...);
PXPublic PXSize PXAPI PXTextPrintA(char* const text, const PXSize size, const char* style, ...);
PXPublic PXSize PXAPI PXTextPrintAV(char* const text, const PXSize size, const char* style, va_list parameter);
PXPublic PXSize PXAPI PXTextPrintV(PXText* const pxText, const char* style, va_list parameter);

PXPublic PXSize PXAPI PXTextClear(PXText* const pxText);

PXPublic void PXAPI PXTextAdvance(PXText* const pxText, const PXSize advanceBy);

PXPublic PXSize PXAPI PXTextLengthA(const char* string, const PXSize stringSize);
PXPublic PXSize PXAPI PXTextLengthW(const wchar_t* string, const PXSize stringSize);

PXPublic PXSize PXAPI PXTextLengthUntilA(const char* string, const PXSize stringSize, const char character);
PXPublic PXSize PXAPI PXTextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character);

PXPublic PXSize PXAPI PXTextCopy(const PXText* const source, PXText* const destination);

PXPublic PXSize PXAPI PXTextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);

PXPublic PXSize PXAPI PXTextCopyAU(const PXTextASCII source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);
PXPublic PXSize PXAPI PXTextCopyWU(const PXTextUNICODE source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);

PXPublic PXSize PXAPI PXTextCountA(const char* PXText, const PXSize PXTextSize, const char target);
PXPublic PXSize PXAPI PXTextCountW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target);

PXPublic PXSize PXAPI PXTextPascalCaseCleave(const char* pxText, const PXSize PXTextSize);


PXPublic PXSize PXAPI PXTextCountUntilA(const char* PXText, const PXSize PXTextSize, const char target, const char stopAt);
PXPublic PXSize PXAPI PXTextCountUntilW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target, const wchar_t stopAt);

PXPublic PXBool PXAPI PXTextCompare(const PXText* const textA, const PXText* const textB);

PXPublic PXBool PXAPI PXTextCompareA(const char* a, PXSize aSize, const char* b, PXSize bSize);
PXPublic PXBool PXAPI PXTextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic PXBool PXAPI PXTextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic PXBool PXAPI PXTextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

PXPublic char PXAPI PXTextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
PXPublic char PXAPI PXTextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

PXPublic char* PXAPI PXTextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize);

PXPublic PXSize PXAPI PXTextFindLastCharacter(const PXText* const pxText, const char character);
PXPublic PXSize PXAPI PXTextFindFirstCharacter(const PXText* const pxText, const char character);

PXPublic PXSize PXAPI PXTextFindFirstCharacterA(const char* PXRestrict const string, const PXSize dataSize, const char character);
PXPublic PXSize PXAPI PXTextFindFirstCharacterBeforeA(const char* PXRestrict const string, const PXSize dataSize, const char target, const char barrier);
PXPublic PXSize PXAPI PXTextFindFirstCharacterOfListA(const char* PXRestrict const string, const PXSize dataSize, const char* characterList, const PXSize characterListSize);

PXPublic PXSize PXAPI PXTextFindFirstStringA(const char* __restrict string, const PXSize dataSize, const char* PXRestrict const targetString, const PXSize targetStringSize);
PXPublic PXSize PXAPI PXTextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character);

PXPublic PXBool PXAPI PXTextFindLast(const PXText* const stringSource, const PXText* const stringTarget, PXText* const stringResult);

PXPublic void PXAPI PXTextMoveByOffset(PXText* const pxText, const PXSize offset);

PXPublic void PXAPI PXTextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character);

PXPublic void PXAPI PXTextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...);

PXPublic void PXAPI PXTextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize);

PXPublic PXBool PXAPI PXTextMatchW(const wchar_t* input, const PXSize inputSize, const wchar_t* pattern, const PXSize patternSize);

PXPublic PXSize PXAPI PXTextReplace(PXText* const pxText, char target, char value);

#endif