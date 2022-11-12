#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <Error/ActionResult.h>
#include <File/DataStream.h>

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

		PXCompilerSymbolLexerComment,

		PXCompilerSymbolLexerBool,
		PXCompilerSymbolLexerFloat,
		PXCompilerSymbolLexerInteger,
		PXCompilerSymbolLexerString,

		PXCompilerSymbolLexerStringBegin,
		PXCompilerSymbolLexerStringEnd
	}
	PXCompilerSymbolLexer;

	typedef struct PXCompilerSymbolEntry_
	{
		//---<Temporal data------

		union 
		{
			char* Source;
			float DataF;
			unsigned int DataI;
			unsigned short DataS;
			unsigned char DataC;
		};	
		
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
		
		PXBool CommentsKeep;
		size_t CommentSingleLineSize;
		char* CommentSingleLine;

		size_t CommentMultibleLineSize;
		char* CommentMultibleLine;
	}
	PXCompilerSettings;

	PXPrivate void PXCompilerSymbolEntryAdd
	(
		DataStream* const dataStream, 
		const PXCompilerSymbolEntry* const compilerSymbolEntry
	);

	PXPublic void PXCompilerSymbolEntryExtract
	(
		DataStream* const dataStream,
		PXCompilerSymbolEntry* compilerSymbolEntry
	);

	PXPrivate PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const size_t textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);

	PXPublic void PXCompilerLexicalAnalysis(DataStream* const inputStream, DataStream* const outputStream, const PXCompilerSettings* const compilerSettings);


	PXPublic PXBool PXCompilerParseStringUntilNewLine(DataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, char* const text, const size_t textMaxSize, size_t* const textSize);
	
	PXPublic PXBool PXCompilerParseIntUnsignedSingle(DataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int* const value);

	PXPublic PXBool PXCompilerParseFloatSingle(DataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, float* const values);
	PXPublic PXBool PXCompilerParseFloatList(DataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, float* const values, const size_t valuesMaxSize, size_t* const valuesSize);

#ifdef __cplusplus
}
#endif

#endif
