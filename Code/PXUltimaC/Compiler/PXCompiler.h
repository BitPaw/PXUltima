#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#define PXCompilerDEBUG 0

#define PXFlagIsSet(value, flag)  ((value & flag) && 1u)

typedef enum PXCompilerSymbolLexer_
{
	PXCompilerSymbolLexerInvalid,

	PXCompilerSymbolLexerWhiteSpace,
	PXCompilerSymbolLexerNewLine,
	PXCompilerSymbolLexerTab,

	PXCompilerSymbolLexerGeneric,

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


// Position where the attached comment to a symbol is in
typedef enum PXCommentPosition_
{
	PXCommentPositionInvalid,
	PXCommentPositionHeader,
	PXCommentPositionInline
}
PXCommentPosition;

typedef struct PXComment_
{
	PXSize Line;
	char* Data;
	PXSize Size;
	PXCommentPosition Position;
}
PXComment;


typedef struct PXCompilerReadInfo_
{
	PXCompilerSymbolEntry SymbolEntryCurrent;

	// TODO: temp solution
	// Comment to be parsed if it is out of bounce for the symbol
	PXComment Comment;

	PXFile* FileInput;
	PXFile* FileCache;
	//PXFile* TokenStream;

	// the current depth of the current object stack
	PXSize DepthCurrent;

	PXSize ErrorCounter;


}
PXCompilerReadInfo;

typedef void (PXAPI* PXCompilerWriteFunction)(PXCompiler* const pxCompiler);

typedef struct PXCompilerWriteInfo_
{
	PXFile* FileOutput;

	PXCodeDocumentElement* CodeElementCurrent;

	PXCompilerWriteFunction WriteNode;
	PXCompilerWriteFunction WriteComment; // Special behaviour, if position does not match, dont write
	PXCompilerWriteFunction WriteFile; // Regarded as the root of all things. The physical file
	PXCompilerWriteFunction WriteInclude;
	PXCompilerWriteFunction WriteContainer;
	PXCompilerWriteFunction WriteFunction;
	PXCompilerWriteFunction WriteDefinition;
	PXCompilerWriteFunction WriteParameter;

	PXInt8U TABSize;
}
PXCompilerWriteInfo;



#define PXCompilerKeepWhiteSpace (1 << 0) // Keep or ignore general empty space.
#define PXCompilerKeepComments (1 << 1)
#define PXCompilerKeepTABs (1 << 2)
#define PXCompilerKeepAnalyseTypes (1 << 3)
#define PXCompilerKeepNewLine (1 << 4)
#define PXCompilerInterpretNewLineAsWhiteSpace (1 << 5)

typedef struct PXCompiler_
{
	union
	{
		PXCompilerReadInfo ReadInfo;
		PXCompilerWriteInfo WriteInfo;
	};

	PXCodeDocument* CodeDocument;

	PXSize EndOfCommand;
	const char* EndOfCommandSize;

	PXSize CommentSingleLineSize;
	const char* CommentSingleLine; // Example : "//" or "#"

	PXSize CommentMultibleLineBeginSize;
	const char* CommentMultibleLineBegin;  // Example : "/*"

	PXSize CommentMultibleLineEndSize;
	const char* CommentMultibleLineEnd;  // Example : "*/"

	PXInt32U Flags;


	//PXBool IntrepredNewLineAsWhiteSpace;
	//PXBool IntrepredTabsAsWhiteSpace;
	//PXBool DoPrintOutput;
}
PXCompiler;

PXPublic const char* PXAPI PXCompilerCompilerSymbolLexerToString(const PXCompilerSymbolLexer pxCompilerSymbolLexer);

PXPrivate void PXAPI PXCompilerSymbolEntryAdd(PXCompiler* const pxCompiler, const PXCompilerSymbolEntry* const compilerSymbolEntry);

// Take current Node and merge it with the next one.
PXPublic PXSize PXAPI PXCompilerSymbolEntryMergeCurrentWithNext(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry);

// Merge all symbols togteher until end of line.
// This is used for strings that can contain unexpected or illegal symbols
PXPublic PXSize PXAPI PXCompilerSymbolEntryMergeCurrentUntilNextLine(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry);

// rewind one symbol back
PXPublic PXSize PXAPI PXCompilerSymbolRewind(PXCompiler* const pxCompiler, const PXSize amount);

// Fetch next symbol without actually changing the file-cursor-position
PXPublic PXSize PXAPI PXCompilerSymbolEntryPeek(PXCompiler* const pxCompiler);

// Peek enxt symbol but also ensure it is the correct type
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekEnsure(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer);

// Same as PXCompilerSymbolEntryPeekEnsure() but as harsh
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer);

// Exact behaviour as PXCompilerSymbolEntryPeekCheck but allows a list of possible values
PXPublic PXBool PXAPI PXCompilerSymbolEntryEnsureCheckList(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer* const pxCompilerSymbolLexerList, const PXSize amount);

// Fetch next symbol and consume symbol
PXPublic PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler* const pxCompiler);

// Without fetching the symbol, consume it.
PXPublic PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler* const pxCompiler);

PXPrivate PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile* const inputStream, const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);

PXPublic PXActionResult PXAPI PXCompilerLexicalAnalysis(PXCompiler* const pxCompiler);

PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler* const pxCompiler, PXText* const pxText);
PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler* const pxCompiler, char* const text, const PXSize textMaxSize, PXSize* const textSize);

PXPublic PXBool PXAPI PXCompilerParseFloatSingle(PXCompiler* const pxCompiler, float* const values);
PXPublic PXBool PXAPI PXCompilerParseFloatList(PXCompiler* const pxCompiler, float* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);

PXPublic void PXAPI PXCompilerErrorInvoke(PXCompiler* const pxCompiler);


// Write a ASL into a given format. Uses a default way to write and can be overloaded
PXPublic void PXAPI PXCompilerWrite(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteNode(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteInclude(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteComment(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteParameterList(PXCompiler* const pxCompiler);

#endif