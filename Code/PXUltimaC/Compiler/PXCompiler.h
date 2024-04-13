#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#define PXFlagIsSet(value, flag)  ((value & flag) && 1u)

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
	PXCompilerSymbolLexerAsterisk, // '*'
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


typedef struct PXCompiler_
{
	PXFile* FileInput;
	PXFile* FileCache;

	PXCompilerSymbolEntry SymbolEntryCurrent;

	PXSize ErrorCounter;
}
PXCompiler;

PXPublic const char* PXAPI PXCompilerCompilerSymbolLexerToString(const PXCompilerSymbolLexer pxCompilerSymbolLexer);

PXPrivate void PXAPI PXCompilerSymbolEntryAdd(PXCompiler* const pxCompiler, const PXCompilerSymbolEntry* const compilerSymbolEntry);

// Take current Node and merge it with the next one.
PXPublic PXSize PXAPI PXCompilerSymbolEntryMergeCurrentWithNext(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry);

// rewind one symbol back
PXPublic PXSize PXAPI PXCompilerSymbolRewind(PXCompiler* const pxCompiler, const PXSize amount);

// Fetch next symbol without actually changing the file-cursor-position
PXPublic PXSize PXAPI PXCompilerSymbolEntryPeek(PXCompiler* const pxCompiler);

// Peek enxt symbol but also ensure it is the correct type
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer);

// Exact behaviour as PXCompilerSymbolEntryPeekCheck but allows a list of possible values
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekCheckList(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer* const pxCompilerSymbolLexerList, const PXSize amount);

// Fetch next symbol and consume symbol
PXPublic PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler* const pxCompiler);

// Without fetching the symbol, consume it.
PXPublic PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler* const pxCompiler);

PXPrivate PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile* const inputStream, const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);

PXPublic PXActionResult PXAPI PXCompilerLexicalAnalysis(PXCompiler* const pxCompiler, const PXCompilerSettings* const compilerSettings);

PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler* const pxCompiler, PXText* const pxText);
PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler* const pxCompiler, char* const text, const PXSize textMaxSize, PXSize* const textSize);

PXPublic PXBool PXAPI PXCompilerParseFloatSingle(PXCompiler* const pxCompiler, float* const values);
PXPublic PXBool PXAPI PXCompilerParseFloatList(PXCompiler* const pxCompiler, float* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);

PXPublic void PXAPI PXCompilerErrorInvoke(PXCompiler* const pxCompiler);

#endif