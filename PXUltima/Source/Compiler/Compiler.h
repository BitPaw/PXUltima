#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <Error/ActionResult.h>
#include <File/DataStream.h>

#define PXCompilerSymbolLexerInvalidID 0xFF000001
#define PXCompilerSymbolLexerWhiteSpaceID PXCompilerSymbolLexerInvalidID + 1u
#define PXCompilerSymbolLexerNewLineID  PXCompilerSymbolLexerInvalidID + 2u
#define PXCompilerSymbolLexerElementID PXCompilerSymbolLexerInvalidID + 3u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXCompilerSymbolLexer_
	{
		PXCompilerSymbolLexerInvalid,
		PXCompilerSymbolLexerWhiteSpace,
		PXCompilerSymbolLexerNewLine,
		PXCompilerSymbolLexerElement
	}
	PXCompilerSymbolLexer;

	typedef struct PXCompilerSymbolEntry_
	{
		unsigned int ID;

		//---<Temporal data------
		unsigned int Coloum;
		unsigned int Line;
		unsigned int Size;
		char* Source;
		//----------------------
	}
	PXCompilerSymbolEntry;
	
	typedef struct PXCompilerSettings_
	{
		unsigned char WhiteSpaceKeep; // Keep or ignore empty spaces
	}
	PXCompilerSettings;

	CPrivate void PXCompilerSymbolEntryAdd
	(
		DataStream* const dataStream, 
		unsigned int id,
		unsigned int coloum,
		unsigned int line,
		unsigned int size,
		char* source
	);

	CPublic void PXCompilerSymbolEntryExtract
	(
		DataStream* const dataStream,
		PXCompilerSymbolEntry* compilerSymbolEntry
	);

	CPublic void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings);

#ifdef __cplusplus
}
#endif

#endif
