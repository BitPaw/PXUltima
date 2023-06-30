#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

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
		PXCompilerSymbolLexerStringEnd,

		PXCompilerSymbolLexerEndOfFile
	}
	PXCompilerSymbolLexer;

	typedef struct PXCompilerSymbolEntry_
	{
		//---<Temporal data------

		union 
		{
			char* Source;
			float DataF;
			int DataI32S;
			unsigned int DataI32U;
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
		PXBool DoPrintOutput;
		
		PXSize EndOfCommand;
		const char* EndOfCommandSize;


		PXBool CommentsKeep;
		PXSize CommentSingleLineSize;
		const char* CommentSingleLine;

		PXSize CommentMultibleLineBeginSize;
		const char* CommentMultibleLineBegin;

		PXSize CommentMultibleLineEndSize;
		const char* CommentMultibleLineEnd;
	}
	PXCompilerSettings;

	PXPublic void PXCompilerSettingsConstruct(PXCompilerSettings* const compilerSettings);

	PXPrivate void PXCompilerSymbolEntryAdd
	(
		PXFile* const dataStream, 
		const PXCompilerSymbolEntry* const compilerSymbolEntry
	);

	PXPublic PXSize PXCompilerSymbolEntryExtract
	(
		PXFile* const pxFile,
		PXCompilerSymbolEntry* const compilerSymbolEntry
	);
	PXPublic PXSize PXCompilerSymbolEntryPeek
	(
		PXFile* const pxFile,
		PXCompilerSymbolEntry* const compilerSymbolEntry
	);

	PXPrivate PXCompilerSymbolLexer PXCompilerTryAnalyseType(const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);

	PXPublic void PXCompilerLexicalAnalysis(PXFile* const inputStream, PXFile* const outputStream, const PXCompilerSettings* const compilerSettings);


	PXPublic PXBool PXCompilerParseStringUntilNewLine(PXFile* const inputStream, char* const text, const PXSize textMaxSize, PXSize* const textSize);
	
	PXPublic PXBool PXCompilerParseIntUnsignedSingle(PXFile* const inputStream, PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int* const value);

	PXPublic PXBool PXCompilerParseFloatSingle(PXFile* const inputStream, float* const values);
	PXPublic PXBool PXCompilerParseFloatList(PXFile* const inputStream,  float* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);

#ifdef __cplusplus
}
#endif

#endif
