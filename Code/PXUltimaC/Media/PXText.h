#ifndef PXTextINLCUDE
#define PXTextINLCUDE

#include <Media/PXType.h>

//---<Makros>------------------------------------------------------------------
#define MakeLetterCaseLower(character) (character | 0b00100000)
#define MakeLetterCaseUpper(character) (character & 0b11011111)
#define CompareLetterCaseIgnore(a, b) (MakeLetterCaseLower(a) == b) || (MakeLetterCaseUpper(a) == b)
#define UnicodeToASCII(wc) (wc <= 0xFF ? (char)wc : '?')
#define IsEndOfString(c) (c == '\0')
#define IsTab(c) (c == '\t')
#define IsEmptySpace(c) (c == ' ')
#define IsEndOfLineCharacter(c) (c == '\r' || c == '\n')

#define PXTextFloatIsAllowedCharacter(c)  ('0' <= c && c <= '9' || c == '-' || c == '+' || c == '.')

#define IsPrintable(c) (0x20 =< c && c =< 0x7E)
#define MakePrintable(c) ((0x20 <= c && c <= 0x7E) ? + c : '?')
//-----------------------------------------------------------------------------

#define PXTextUnkownLength -1
#define PXTextIndexNotFound -1





#ifdef __cplusplus
extern "C"
{
#endif

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
		PXMemorySet((pxText)->TextA, '°', (pxText)->SizeAllocated);


#define PXTextConstructNamedBufferA(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize, TextFormatASCII)
#define PXTextConstructNamedBufferW(pxText, bufferCacheName, bufferCacheSize) PXTextConstructNamedBuffer(pxText, bufferCacheName, bufferCacheSize*2, TextFormatUNICODE)


#define PXTextConstructBufferA(pxText, bufferSize) PXTextConstructNamedBufferA(pxText, __pxUnnamedInternalBuffer, bufferSize);
#define PXTextConstructBufferW(pxText, bufferSize) PXTextConstructNamedBufferW(pxText, __pxUnnamedInternalBuffer, bufferSize);



#define PXTextConstructFromAdress(pxText, address, size, format)\
		if((PXSize)size == (PXSize)-1)\
		{\
			(pxText)->SizeAllocated = PXTextLengthA(address, (PXSize)-1);\
		}\
		else\
		{\
			(pxText)->SizeAllocated = size; \
		}\
		(pxText)->SizeUsed = (pxText)->SizeAllocated;\
		(pxText)->NumberOfCharacters = (pxText)->SizeAllocated;\
		(pxText)->Format = format;\
		(pxText)->TextA = (char*)(address);

#define PXTextConstructFromAdressA(pxText, address, size) PXTextConstructFromAdress(pxText, (char*)address, size, TextFormatASCII)
#define PXTextConstructFromAdressW(pxText, address, size) PXTextConstructFromAdress(pxText, (char*)address, size, TextFormatUNICODE)

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


	PXPublic PXSize PXTextFromInt(PXText* const pxText, int number);
	PXPublic PXSize PXTextFromBool(PXText* const pxText, const PXBool number);
	PXPublic PXSize PXTextFromFloat(PXText* const pxText, const float number);
	
	PXPublic PXSize PXTextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize);

	PXPublic PXSize PXTextToInt(const PXText* const pxText, int* const number);
	PXPublic PXSize PXTextToBool(const PXText* const pxText, PXBool* const number);
	PXPublic PXSize PXTextToFloat(const PXText* const pxText, float* const number);



	PXPublic PXSize PXTextFromIntToBinary8U(char* const string, const PXSize dataSize, const PXInt8U number);
	PXPublic PXSize PXTextFromIntToBinary16U(char* const string, const PXSize dataSize, const PXInt16U number);
	PXPublic PXSize PXTextFromIntToBinary32U(char* const string, const PXSize dataSize, const PXInt32U number);
	PXPublic PXSize PXTextFromIntToBinary64U(char* const string, const PXSize dataSize, const PXInt64U number);
	PXPublic PXSize PXTextFromIntToBinary64UR(char* const string, const PXSize dataSize, const PXInt64U number, const unsigned char numberOfDigits);

	PXPublic PXSize PXTextToLowerCase(const PXText* const pxTextSource, PXText* const pxTextTarget);
	PXPublic PXSize PXTextToUpperCase(const PXText* const pxTextSource, PXText* const pxTextTarget);
		 
	PXPublic PXSize PXTextAppendW(wchar_t* const dataString, const PXSize dataStringSize, const wchar_t* const appaendString, const PXSize appaendStringSize);

	PXPublic PXSize PXTextPrint(PXText* const pxText, const char* style, ...);


	PXPublic PXSize PXTextClear(PXText* const pxText);

	PXPublic void PXTextAdvance(PXText* const pxText, const PXSize advanceBy);

	PXPublic PXSize PXTextLengthA(const char* string, const PXSize stringSize);
	PXPublic PXSize PXTextLengthW(const wchar_t* string, const PXSize stringSize);

	PXPublic PXSize PXTextLengthUntilA(const char* string, const PXSize stringSize, const char character);
	PXPublic PXSize PXTextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character);

	PXPublic PXSize PXTextCopy(const PXText* const source, PXText* const destination);

	PXPublic PXSize PXTextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
	PXPublic PXSize PXTextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);
	PXPublic PXSize PXTextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
	PXPublic PXSize PXTextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);

	PXPublic PXSize PXTextCopyAU(const PXTextASCII source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);
	PXPublic PXSize PXTextCopyWU(const PXTextUNICODE source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);

	PXPublic PXSize PXTextCountA(const char* PXText, const PXSize PXTextSize, const char target);
	PXPublic PXSize PXTextCountW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target);

	PXPublic PXSize PXTextCountUntilA(const char* PXText, const PXSize PXTextSize, const char target, const char stopAt);
	PXPublic PXSize PXTextCountUntilW(const wchar_t* PXText, const PXSize PXTextSize, const wchar_t target, const wchar_t stopAt);

	PXPublic PXBool PXTextCompare(const PXText* const textA, const PXText* const textB);

	PXPublic PXBool PXTextCompareA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
	PXPublic PXBool PXTextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic PXBool PXTextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic PXBool PXTextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

	PXPublic char PXTextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
	PXPublic char PXTextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic char PXTextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic char PXTextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

	PXPublic char* PXTextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize);

	PXPublic PXSize PXTextFindFirstCharacterA(const char* PXRestrict const string, const PXSize dataSize, const char character);
	PXPublic PXSize PXTextFindFirstCharacterBeforeA(const char* PXRestrict const string, const PXSize dataSize, const char target, const char barrier);
	PXPublic PXSize PXTextFindFirstCharacterOfListA(const char* PXRestrict const string, const PXSize dataSize, const char* characterList, const PXSize characterListSize);

	PXPublic PXSize PXTextFindFirstStringA(const char* __restrict string, const PXSize dataSize, const char* PXRestrict const targetString, const PXSize targetStringSize);
	PXPublic PXSize PXTextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character);
	
	PXPublic PXBool PXTextFindLast(const PXText* const stringSource, const PXText* const stringTarget, PXText* const stringResult);
	
	PXPublic void PXTextMoveByOffset(PXText* const pxText, const PXSize offset);

	PXPublic void PXTextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character);

	PXPublic void PXTextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...);

	PXPublic void PXTextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize);

	PXPublic PXBool PXTextMatchW(const wchar_t* input, const PXSize inputSize, const wchar_t* pattern, const PXSize patternSize);


	PXPublic PXSize PXTextReplace(PXText* const pxText, char target, char value);


#ifdef __cplusplus
}
#endif

#endif