#ifndef TextInclude
#define TextInclude

#include <Format/Type.h>

#define MakeLetterCaseLower(character) (character | 0b00100000)
#define MakeLetterCaseUpper(character) (character & 0b11011111)
#define CompareLetterCaseIgnore(a, b) (MakeLetterCaseLower(a) == b) || (MakeLetterCaseUpper(a) == b)
#define UnicodeToASCII(wc) (wc <= 0xFF ? (char)wc : '?')
#define IsEndOfString(c) (c == '\0')
#define IsTab(c) (c == '\t')
#define IsEmptySpace(c) (c == ' ')
#define IsEndOfLineCharacter(c) (c == '\r' || c == '\n')

#define TextUnkownLength -1
#define TextIndexNotFound -1

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

	typedef enum TextFormat_
	{
		TextFormatInvalid,

		// 1 Byte__ per character, range from 0 to 255
		TextFormatASCII,

		// 2 Bytes per character, range from 0 to 65535
		TextFormatUNICODE,

		// Variable length of characters, 1 to 4 Bytes
		TextFormatUTF8,
		TextFormatUTF16
	}
	TextFormat;

	typedef struct Text_
	{
		TextFormat Format;
		PXSize SizeInCharacters;
		PXSize SizeInBytes;

		union
		{
			char* TextA;
			wchar_t* TextW;
			void* TextData;
		};
	}
	Text;

	PXPublic PXSize TextFromIntA(const int number,  char* string, const PXSize dataSize);
	PXPublic PXSize TextFromIntW(const int number,  wchar_t* string, const PXSize dataSize);
	PXPublic PXSize TextFromBoolA(const unsigned char number,  char* string, const PXSize dataSize);
	PXPublic PXSize TextFromBoolW(const unsigned char number,  wchar_t* string, const PXSize dataSize);
	PXPublic PXSize TextFromFloatA(const float number,  char* string, const PXSize dataSize);
	PXPublic PXSize TextFromFloatW(const float number,  wchar_t* string, const PXSize dataSize);
	PXPublic PXSize TextFromDoubleA(const double number,  char* string, const PXSize dataSize);
	PXPublic PXSize TextFromDoubleW(const double number,  wchar_t* string, const PXSize dataSize);
	PXPublic PXSize TextFromBinaryDataA(const void* data, const PXSize dataSize, char* string, const PXSize stringSize);

	PXPublic PXSize TextToIntA(const char* string, const PXSize dataSize, int* number);
	PXPublic PXSize TextToIntW(const wchar_t* string, const PXSize dataSize, int* number);
	PXPublic PXSize TextToBoolA(const char* string, const PXSize dataSize, unsigned char* number);
	PXPublic PXSize TextToBoolW(const wchar_t* string, const PXSize dataSize, unsigned char* number);
	PXPublic PXSize TextToFloatA(const char* string, const PXSize dataSize, float* number);
	PXPublic PXSize TextToFloatW(const wchar_t* string, const PXSize dataSize, float* number);
	PXPublic PXSize TextToDoubleA(const char* string, const PXSize dataSize, double* number);
	PXPublic PXSize TextToDoubleW(const wchar_t* string, const PXSize dataSize, double* number);

	PXPublic PXSize TextAppendW(wchar_t* const dataString, const PXSize dataStringSize, const wchar_t* const appaendString, const PXSize appaendStringSize);

	PXPublic PXSize TextClearA(char* string, const PXSize stringSize);
	PXPublic PXSize TextClearW(wchar_t* string, const PXSize stringSize);

	PXPublic PXSize TextLengthA(const char* string, const PXSize stringSize);
	PXPublic PXSize TextLengthW(const wchar_t* string, const PXSize stringSize);

	PXPublic PXSize TextLengthUntilA(const char* string, const PXSize stringSize, const char character);
	PXPublic PXSize TextLengthUntilW(const wchar_t* string, const PXSize stringSize, const wchar_t character);

	PXPublic PXSize TextCopyA(const char* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
	PXPublic PXSize TextCopyAW(const char* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);
	PXPublic PXSize TextCopyWA(const wchar_t* source, const PXSize sourceLength, char* destination, const PXSize destinationLength);
	PXPublic PXSize TextCopyW(const wchar_t* source, const PXSize sourceLength, wchar_t* destination, const PXSize destinationLength);

	PXPublic PXSize TextCopyAU(const PXASCIIChar source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);
	PXPublic PXSize TextCopyWU(const PXTextUNICODE source, const PXSize sourceLength, PXTextUTF8 destination, const PXSize destinationLength);

	PXPublic PXSize TextCountA(const char* text, const PXSize textSize, const char target);
	PXPublic PXSize TextCountW(const wchar_t* text, const PXSize textSize, const wchar_t target);

	PXPublic PXSize TextCountUntilA(const char* text, const PXSize textSize, const char target, const char stopAt);
	PXPublic PXSize TextCountUntilW(const wchar_t* text, const PXSize textSize, const wchar_t target, const wchar_t stopAt);

	PXPublic PXBool TextCompareA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
	PXPublic PXBool TextCompareAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic PXBool TextCompareW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic PXBool TextCompareWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

	PXPublic char TextCompareIgnoreCaseA(const char* a, const PXSize aSize, const char* b, const PXSize bSize);
	PXPublic char TextCompareIgnoreCaseW(const wchar_t* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic char TextCompareIgnoreCaseAW(const char* a, const PXSize aSize, const wchar_t* b, const PXSize bSize);
	PXPublic char TextCompareIgnoreCaseWA(const wchar_t* a, const PXSize aSize, const char* b, const PXSize bSize);

	PXPublic char* TextFindPositionA(const char* data, PXSize dataSize, const char* target, PXSize targetSize);

	PXPublic PXSize TextFindFirstA(const char* string, const PXSize dataSize, const char character);
	PXPublic PXSize TextFindFirstW(const wchar_t* string, const PXSize dataSize, const wchar_t character);
	PXPublic PXSize TextFindLastA(const char* string, const PXSize dataSize, const char character);
	PXPublic PXSize TextFindLastW(const wchar_t* string, const PXSize dataSize, const wchar_t character);

	PXPublic void TextTerminateBeginFromFirstA(char* string, const PXSize dataSize, const char character);

	PXPublic void TextParseA(const char* buffer, const PXSize bufferSize, const char* syntax, ...);

	PXPublic void TextParseFindAllA(const char* string, const PXSize stringSize, const ParsingTokenA* parsingTokenList, const PXSize parsingTokenListSize);

#ifdef __cplusplus
}
#endif

#endif
