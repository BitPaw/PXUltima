#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <Error/ActionResult.h>
#include <File/DataStream.h>

#define PXCompilerSymbolLexerInvalidID 0xFF000001
#define PXCompilerSymbolLexerWhiteSpaceID PXCompilerSymbolLexerInvalidID + 1u
#define PXCompilerSymbolLexerNewLineID  PXCompilerSymbolLexerInvalidID + 2u
#define PXCompilerSymbolLexerElementID PXCompilerSymbolLexerInvalidID + 3u
#define PXCompilerSymbolLexerTrueID PXCompilerSymbolLexerInvalidID + 4u
#define PXCompilerSymbolLexerFalseID PXCompilerSymbolLexerInvalidID + 5u
#define PXCompilerSymbolLexerIntegerID PXCompilerSymbolLexerInvalidID + 6u
#define PXCompilerSymbolLexerFloatID PXCompilerSymbolLexerInvalidID + 7u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXCompilerSymbolLexer_
	{
		PXCompilerSymbolLexerInvalid,

		PXCompilerSymbolLexerWhiteSpace,
		PXCompilerSymbolLexerNewLine,
		PXCompilerSymbolLexerTab,
		PXCompilerSymbolLexerGenericElement,
		PXCompilerSymbolLexerSingleCharacter,

		PXCompilerSymbolLexerTrue,
		PXCompilerSymbolLexerFalse,

		PXCompilerSymbolLexerFloat,
		PXCompilerSymbolLexerInteger,
		PXCompilerSymbolLexerString
	}
	PXCompilerSymbolLexer;

	typedef struct PXCompilerSymbolEntry_
	{
		//---<Temporal data------
		char* Source;
		unsigned int Coloum;
		unsigned int Line;
		unsigned int Size;	
		//----------------------

		PXCompilerSymbolLexer ID;
	}
	PXCompilerSymbolEntry;
	
	typedef struct PXCompilerSettings_
	{
		PXBool KeepWhiteSpace; // Keep or ignore general empty space.
		PXBool KeepWhiteSpaceIndentationLeft;
		PXBool TryAnalyseTypes;
		PXBool IntrepredNewLineAsWhiteSpace;
		PXBool KeepTabs;
		PXBool IntrepredTabsAsWhiteSpace;

	}
	PXCompilerSettings;

	PXPrivate void PXCompilerSymbolEntryAdd
	(
		DataStream* const dataStream, 
		PXCompilerSymbolLexer id,
		unsigned int coloum,
		unsigned int line,
		unsigned int size,
		char* source
	);

	PXPublic void PXCompilerSymbolEntryExtract
	(
		DataStream* const dataStream,
		PXCompilerSymbolEntry* compilerSymbolEntry
	);

	PXPrivate PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const size_t textSize);

	PXPublic void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings);

#ifdef __cplusplus
}
#endif

#endif
