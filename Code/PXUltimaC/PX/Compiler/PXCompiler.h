#ifndef PXCompilerINCLUDE
#define PXCompilerINCLUDE

#include <PX/Media/PXResource.h>

#define PXCompilerDEBUG 1


//--------------------------------------------------------
// PX::Compiler::Symbol::Lexer
//--------------------------------------------------------

// MSB is set to 1 to signal the use of a custom ID
// That the user can define themself.
#define PXCompilerSymbolLexerCustomMask 0b10000000 

#define PXCompilerSymbolLexerInvalid            0 // Normally unused, undefined tokens
#define PXCompilerSymbolLexerEndOfFile          1 // EOF
#define PXCompilerSymbolLexerWhiteSpace         2 // ' '
#define PXCompilerSymbolLexerNewLine            3 // '\n'
#define PXCompilerSymbolLexerTab                4 // '\t'
#define PXCompilerSymbolLexerEmpty              5 // Signals a deleted or allignment element
#define PXCompilerSymbolLexerComment            6 // "//... or #..."
#define PXCompilerSymbolLexerGeneric            7 // Strings that are jet to be analysed

#define PXCompilerSymbolLexerBrackedRoundOpen   8 // '('
#define PXCompilerSymbolLexerBrackedRoundClose  9 // ')'
#define PXCompilerSymbolLexerBracketSquareOpen  10 // '['
#define PXCompilerSymbolLexerBracketSquareClose 11 // ']'
#define PXCompilerSymbolLexerBracketCurlyOpen   12 // '{'
#define PXCompilerSymbolLexerBracketCurlyClose  13 // '}'
#define PXCompilerSymbolLexerBracketAngleOpen   14 // '<'
#define PXCompilerSymbolLexerBracketAngleClose  15 // '>'
#define PXCompilerSymbolLexerBigger             PXCompilerSymbolLexerBracketAngleOpen
#define PXCompilerSymbolLexerSmaler             PXCompilerSymbolLexerBracketAngleClose,
#define PXCompilerSymbolLexerBiggerAndEqual     16 // >=
#define PXCompilerSymbolLexerSmalerAndEqual     17 // <=
#define PXCompilerSymbolLexerEqual              18 // '='
#define PXCompilerSymbolLexerEqualDouble        19 // ==
#define PXCompilerSymbolLexerEqualTrippel       20 // ===
#define PXCompilerSymbolLexerCompareThreeWay    21 // <=>
#define PXCompilerSymbolLexerQuestionmark       22 // ?
#define PXCompilerSymbolLexerExclamation        23 // !
#define PXCompilerSymbolLexerDot                24 // .
#define PXCompilerSymbolLexerComma              25 // ,
#define PXCompilerSymbolLexerColon              26 // :
#define PXCompilerSymbolLexerSemiColon          27 // ;
#define PXCompilerSymbolLexerHash               28 // #
#define PXCompilerSymbolLexerPlus               29 // +
#define PXCompilerSymbolLexerMinus              30 // -
#define PXCompilerSymbolLexerSlash              31 // /
#define PXCompilerSymbolLexerAsterisk           32 // '*'
#define PXCompilerSymbolLexerSlashBack          33 // '\'
#define PXCompilerSymbolLexerAmpercant          34 // '&'
#define PXCompilerSymbolLexerPercent            35 // '%'
#define PXCompilerSymbolLexerBar                36 // '|'
#define PXCompilerSymbolLexerDegree             37 // '°'
#define PXCompilerSymbolLexerExponent           38 // '^'
#define PXCompilerSymbolLexerTilde              39 // ~
#define PXCompilerSymbolLexerApostrophe         40 // `
#define PXCompilerSymbolLexerApostropheB        41 // ´
#define PXCompilerSymbolLexerQuiteSingle        42 // '
#define PXCompilerSymbolLexerQuoteDouble        43 // "
#define PXCompilerSymbolLexerUnderscore         44 // _
#define PXCompilerSymbolLexerAt                 45 // @
#define PXCompilerSymbolLexerParagraf           46 // §
#define PXCompilerSymbolLexerDoller             47 // $

#define PXCompilerSymbolLexerPlusDouble         50
#define PXCompilerSymbolLexerMinusDouble        51

#define PXCompilerSymbolLexerBool               52 // true, false
#define PXCompilerSymbolLexerReal               55 // 0.123
#define PXCompilerSymbolLexerNumeric            56 // 1234, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
#define PXCompilerSymbolLexerString             57 // "Text", 'Text', @Text@
//--------------------------------------------------------

typedef PXI8U PXCompilerSymbolLexer;


typedef struct PXCompilerSymbolEntry_
{
    union
    {
        char* Source; // Valid pointer if ID is text element

#if OS64B
        // If we are in X64 bit mode, we can embedd 64-Bit types for free
        PXF64 F64;
        PXI64S I64S;
        PXI64U I64U;
#endif

        char Data[4]; // Can be used to embedd something

        PXF32 F32;
        PXI32S I32S;
        PXI32U I32U;
        
        PXF16 F16;
        PXI16S I16S;
        PXI16U I16U;
        
        PXI8S CS;
        PXI8U CU;
    };
   
    PXI32U Line; // Files can be very long, although rarely they do. is this enough?
    PXI16U Size; // Used mostly only for strings, we will not have 4GB strings.
    PXI8U Coloum;  // Text files normally do not exceed colum 60,80 or alike. 255 should be fine
    PXI8U ID;
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

    PXI8U TABSize;
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

    PXI32U Flags;

    PXSize SymbolsRead;

    //PXBool IntrepredNewLineAsWhiteSpace;
    //PXBool IntrepredTabsAsWhiteSpace;
    //PXBool DoPrintOutput;
}
PXCompiler;


PXPublic PXActionResult PXAPI PXCompilerLexicalAnalysis(PXCompiler* const pxCompiler);


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

// Peek next symbol but also ensure it is the correct type
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekEnsure(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer);

// Same as PXCompilerSymbolEntryPeekEnsure() but not as harsh
PXPublic PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer);

// Exact behaviour as PXCompilerSymbolEntryPeekCheck but allows a list of possible values
PXPublic PXBool PXAPI PXCompilerSymbolEntryEnsureCheckList(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer* const pxCompilerSymbolLexerList, const PXSize amount);

// Fetch next symbol and consume symbol
PXPublic PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler* const pxCompiler);

// Without fetching the symbol, consume it.
PXPublic PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler* const pxCompiler);

PXPrivate PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile* const inputStream, const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry);


PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler* const pxCompiler, PXText* const pxText);
PXPublic PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler* const pxCompiler, char* const text, const PXSize textMaxSize, PXSize* const textSize);

// Expect a generic token and compare it against provided text
PXPublic PXBool PXAPI PXCompilerEnsureTextAndCompare(PXCompiler* const pxCompiler, const char* const text, const PXSize textSize);

PXPublic PXI8U PXAPI PXCompilerEnsureTextListAndCompare
(
    PXCompiler* const pxCompiler, 
    const char** const listTextData,
    const PXI8U* listTextSize,
    const PXI8U amount
);


PXPublic PXBool PXAPI PXCompilerEnsurePropertyText
(
    PXCompiler* const pxCompiler,
    const char* const propertyKey,
    const PXSize propertyKeySize,
    char** const propertyValue,
    PXSize* propertyValueSize
);



PXPublic PXBool PXAPI PXCompilerParseI32V(PXCompiler* const pxCompiler, PXI32U* const values, const PXSize valuesExpectedSize);


PXPublic PXBool PXAPI PXCompilerParseF16V(PXCompiler* const pxCompiler, PXF16* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);
PXPublic PXBool PXAPI PXCompilerParseF32V(PXCompiler* const pxCompiler, PXF32* const values, const PXSize valuesMaxSize, PXSize* const valuesSize);

PXPublic PXBool PXAPI PXCompilerParseCSVF32(PXCompiler* const pxCompiler, PXF32* const values, const PXSize valuesSize);
PXPublic PXBool PXAPI PXCompilerParseCSVF64(PXCompiler* const pxCompiler, PXF64* const values, const PXSize valuesSize);


#define PXCompilerParseTextDetectUntilNextLine (1<<0)

PXPublic PXSize PXAPI PXCompilerParseText(PXCompiler* const pxCompiler, char* const text, const PXSize textLengthMax, const PXI8U flags);



// Write a ASL into a given format. Uses a default way to write and can be overloaded
PXPublic void PXAPI PXCompilerWrite(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteNode(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteInclude(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteComment(PXCompiler* const pxCompiler);
PXPublic void PXAPI PXCompilerWriteParameterList(PXCompiler* const pxCompiler);

#endif
