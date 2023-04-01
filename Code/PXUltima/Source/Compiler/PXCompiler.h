#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <OS/Error/PXActionResult.h>
#include <File/PXDataStream.h>

#define PXFlagIsSet(value, flag)  ((value & flag) && 1u)

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

		PXCompilerSymbolLexerBrackedRoundOpen, // '('
		PXCompilerSymbolLexerBrackedRoundClose, // ')'
		PXCompilerSymbolLexerBracketSquareOpen, // '['
		PXCompilerSymbolLexerBracketSquareClose, // ']'
		PXCompilerSymbolLexerBracketCurlyOpen, // '{'
		PXCompilerSymbolLexerBracketCurlyClose, // '}'
		PXCompilerSymbolLexerBracketAngleOpen, // '<'
		PXCompilerSymbolLexerBracketAngleClose, // '>'

		//PXCompilerSymbolLexerBigger,
		//PXCompilerSymbolLexerSmaler,
		PXCompilerSymbolLexerBiggerAndEqual,
		PXCompilerSymbolLexerSmalerAndEqual,
		PXCompilerSymbolLexerEqual,
		PXCompilerSymbolLexerEqualDouble,
		PXCompilerSymbolLexerEqualTrippel,
		PXCompilerSymbolLexerCompareThreeWay,

		PXCompilerSymbolLexerQuestionmark,
		PXCompilerSymbolLexerExclamation,
		PXCompilerSymbolLexerDot,
		PXCompilerSymbolLexerComma,
		PXCompilerSymbolLexerColon,
		PXCompilerSymbolLexerSemiColon,
		PXCompilerSymbolLexerHash,
		PXCompilerSymbolLexerPlus,
		PXCompilerSymbolLexerMinus,
		PXCompilerSymbolLexerSlash,
		PXCompilerSymbolLexerAsterisk,
		PXCompilerSymbolLexerSlashBack,
		PXCompilerSymbolLexerAmpercant,
		PXCompilerSymbolLexerPercent,
		PXCompilerSymbolLexerBar,
		PXCompilerSymbolLexerDegree,
		PXCompilerSymbolLexerExponent,
		PXCompilerSymbolLexerTilde,
		PXCompilerSymbolLexerApostrophe,

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
		PXBool KeepNewLines;
		PXBool IntrepredTabsAsWhiteSpace;
		
		PXSize EndOfCommand;
		char* EndOfCommandSize;


		PXBool CommentsKeep;
		PXSize CommentSingleLineSize;
		char* CommentSingleLine;

		PXSize CommentMultibleLineBeginSize;
		char* CommentMultibleLineBegin;

		PXSize CommentMultibleLineEndSize;
		char* CommentMultibleLineEnd;
	}
	PXCompilerSettings;

	PXPublic void PXCompilerSettingsConstruct(PXCompilerSettings* const compilerSettings);

	PXPrivate void PXCompilerSymbolEntryAdd
	(
		PXDataStream* const dataStream, 
		const PXCompilerSymbolEntry* const compilerSymbolEntry
	);

	PXPublic void PXCompilerSymbolEntryExtract
	(
		PXDataStream* const dataStream,
		PXCompilerSymbolEntry* compilerSymbolEntry
	);
	PXPublic void PXCompilerSymbolEntryPeek
	(
		PXDataStream* const dataStream,
		PXCompilerSymbolEntry* compilerSymbolEntry
	);

	PXPrivate PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);

	PXPublic void PXCompilerLexicalAnalysis(PXDataStream* const inputStream, PXDataStream* const outputStream, const PXCompilerSettings* const compilerSettings);


	PXPublic PXBool PXCompilerParseStringUntilNewLine(PXDataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, char* const text, const PXSize textMaxSize, PXSize* const textSize);
	
	PXPublic PXBool PXCompilerParseIntUnsignedSingle(PXDataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int* const value);

	PXPublic PXBool PXCompilerParseFloatSingle(PXDataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, float* const values);
	PXPublic PXBool PXCompilerParseFloatList(PXDataStream* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, float* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);

#ifdef __cplusplus
}
#endif

#endif
