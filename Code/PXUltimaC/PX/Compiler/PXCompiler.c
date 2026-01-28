#include "PXCompiler.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXDocument.h>
#include <PX/OS/Time/PXTime.h>


#define PXCompilerDebug 0
#define PXCompilerSanitise 1

#if PXCompilerDebug
#include <stdio.h>
#endif

const char PXCompilerText[] = "Compiler";
const char PXCompilerTextLexer[] = "Lexer";
const char PXCompilerTextParse[] = "Parse";

const char* PXAPI PXCompilerCompilerSymbolLexerToString(const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
    switch(pxCompilerSymbolLexer)
    {
        case PXCompilerSymbolLexerWhiteSpace:
            return "WhiteSpace";
        case PXCompilerSymbolLexerNewLine:
            return "NewLine";
        case PXCompilerSymbolLexerTab:
            return "Tab";
        case PXCompilerSymbolLexerGeneric:
            return "Generic";
        case PXCompilerSymbolLexerBrackedRoundOpen:
            return "(";
        case PXCompilerSymbolLexerBrackedRoundClose:
            return ")";
        case PXCompilerSymbolLexerBracketSquareOpen:
            return "[";
        case PXCompilerSymbolLexerBracketSquareClose:
            return "]";
        case PXCompilerSymbolLexerBracketCurlyOpen:
            return "{";
        case PXCompilerSymbolLexerBracketCurlyClose:
            return "}";
        case PXCompilerSymbolLexerBracketAngleOpen:
            return "<";
        case PXCompilerSymbolLexerBracketAngleClose:
            return ">";
        case PXCompilerSymbolLexerBiggerAndEqual:
            return "<=";
        case PXCompilerSymbolLexerSmalerAndEqual:
            return ">=";
        case PXCompilerSymbolLexerEqual:
            return "=";
        case PXCompilerSymbolLexerEqualDouble:
            return "==";
        case PXCompilerSymbolLexerEqualTrippel:
            return "===";
        case PXCompilerSymbolLexerCompareThreeWay:
            return "<=>";
        case PXCompilerSymbolLexerQuestionmark:
            return "?";
        case PXCompilerSymbolLexerExclamation:
            return "!";
        case PXCompilerSymbolLexerDot:
            return ".";
        case PXCompilerSymbolLexerComma:
            return ",";
        case PXCompilerSymbolLexerColon:
            return ":";
        case PXCompilerSymbolLexerSemiColon:
            return ";";
        case PXCompilerSymbolLexerHash:
            return "#";
        case PXCompilerSymbolLexerPlus:
            return "+";
        case PXCompilerSymbolLexerMinus:
            return "-";
        case PXCompilerSymbolLexerSlash:
            return "/";
        case PXCompilerSymbolLexerAsterisk:
            return "*";
        case PXCompilerSymbolLexerSlashBack:
            return "\\";
        case PXCompilerSymbolLexerAmpercant:
            return "&";
        case PXCompilerSymbolLexerPercent:
            return "%";
        case PXCompilerSymbolLexerBar:
            return "|";
        case PXCompilerSymbolLexerDegree:
            return "°";
        case PXCompilerSymbolLexerExponent:
            return "^";
        case PXCompilerSymbolLexerTilde:
            return "~";
        case PXCompilerSymbolLexerApostrophe:
            return "";
        case PXCompilerSymbolLexerComment:
            return "Comment";
        case PXCompilerSymbolLexerBool:
            return "bool";
        case PXCompilerSymbolLexerReal:
            return "Real";
        case PXCompilerSymbolLexerNumeric:
            return "Numeric";
        case PXCompilerSymbolLexerString:
            return "string";
        case PXCompilerSymbolLexerEndOfFile:
            return "EOF";

        default:
            return PXNull;
    }
}

void PXAPI PXCompilerSymbolEntryAdd(PXCompiler PXREF pxCompiler, const PXCompilerSymbolEntry PXREF compilerSymbolEntry)
{
    const PXSize written = PXFileWriteB(pxCompiler->ReadInfo.FileCache, compilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

    ++pxCompiler->SymbolsRead;

#if PXCompilerDebug && 0  //PXCompilerDEBUG
    const char* typeName = PXCompilerCompilerSymbolLexerToString(compilerSymbolEntry->ID);

    switch(compilerSymbolEntry->ID)
    {
        case PXCompilerSymbolLexerReal:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Compiler",
                "Entry",
                "L:%-4i C:%-4i S:%-4i %10s : §3%6.2f",
                compilerSymbolEntry->Line,
                compilerSymbolEntry->Coloum,
                compilerSymbolEntry->Size,
                typeName,
#if OS64B
                (float)compilerSymbolEntry->F64
#else
                compilerSymbolEntry->F32
#endif
      
            );

            break;
        }

        case PXCompilerSymbolLexerNumeric:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Compiler",
                "Entry",
                "L:%-4i C:%-4i S:%-4i - %10s : §3%i",
                compilerSymbolEntry->Line,
                compilerSymbolEntry->Coloum,
                compilerSymbolEntry->Size,
                typeName,
                compilerSymbolEntry->I32U
            );

            break;
        }
        case PXCompilerSymbolLexerComment:
        case PXCompilerSymbolLexerGeneric:
        {
            char buffer[32];
            PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 21);

            PXLogPrint
            (
                PXLoggingInfo,
                "Compiler",
                "Entry",
                "L:%-4i C:%-4i S:%-4i - %10s : §6%s",
                compilerSymbolEntry->Line,
                compilerSymbolEntry->Coloum,
                compilerSymbolEntry->Size,
                typeName,
                buffer
            );
            break;
        }
        default:
            PXLogPrint
            (
                PXLoggingInfo,
                "Compiler",
                "Entry",
                "L:%-4i C:%-4i S:%-4i - %10s",
                compilerSymbolEntry->Line,
                compilerSymbolEntry->Coloum,
                compilerSymbolEntry->Size,
                typeName
            );
            break;
    }
#endif
}

PXSize PXAPI PXCompilerSymbolEntryMergeCurrentWithNext(PXCompiler PXREF pxCompiler, PXCompilerSymbolEntry PXREF compilerSymbolEntry)
{
    PXSize offset = 0;

    PXCompilerSymbolEntry oldCopy;
    PXClear(PXCompilerSymbolEntry, &oldCopy);

    PXCompilerSymbolEntry mergCopy;
    PXClear(PXCompilerSymbolEntry, &mergCopy);

    PXCompilerSymbolEntryExtract(pxCompiler);// Copy old value

    PXCopy(PXCompilerSymbolEntry, &pxCompiler->ReadInfo.SymbolEntryCurrent, &oldCopy);

    PXSize skippedBytes = PXCompilerSymbolEntryPeek(pxCompiler);
    PXSize skippedBlocks = (skippedBytes / 21) - 1;

    PXCopy(PXCompilerSymbolEntry, &pxCompiler->ReadInfo.SymbolEntryCurrent, &mergCopy);

    PXCompilerSymbolRewind(pxCompiler, 1); // Go back again

    // Write merged symbol
    {
        const PXI8U symbolID = compilerSymbolEntry->ID;
        const PXI32U size = oldCopy.Size + ((mergCopy.Coloum + mergCopy.Size) - (oldCopy.Coloum + oldCopy.Size));
        PXTypeEntry pxFileDataElementType[] =
        {
            &oldCopy.ID, PXTypeI08U,
            &oldCopy.Coloum, PXTypeI32U,
            &oldCopy.Line, PXTypeI32U,
            &size, PXTypeI32U,
            &oldCopy.Source, PXTypeAdress
        };

        const PXSize written = PXFileWriteMultible(pxCompiler->ReadInfo.FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
    }

    // add magic offset of previous deleted entrys
    PXFileCursorAdvance(pxCompiler->ReadInfo.FileCache, skippedBlocks * 21);

    // delete old symbol
    {
        const PXI8U symbolID = PXCompilerSymbolLexerInvalid;
        const PXI32U emptyValue = 0;
        const void* emptyAdress = 0;
        const PXTypeEntry pxFileDataElementType[] =
        {
            &symbolID, PXTypeI08U,
            &mergCopy.Coloum, PXTypeI32U,
            &mergCopy.Line, PXTypeI32U,
            &emptyValue, PXTypeI32U,
            &emptyAdress, PXTypeAdress
        };

        //const PXSize written = PXFileWriteB(pxCompiler->ReadInfo.FileCache, pxCompilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

        PXFileWriteMultible(pxCompiler->ReadInfo.FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
    }

    PXCompilerSymbolRewind(pxCompiler, skippedBlocks + 2); // Go back again, again

    PXCompilerSymbolEntryPeek(pxCompiler);

    return PXResultOK;
}

PXSize PXAPI PXCompilerSymbolEntryMergeCurrentUntilNextLine(PXCompiler PXREF pxCompiler, PXCompilerSymbolEntry PXREF compilerSymbolEntry)
{
    const PXSize currentLine = compilerSymbolEntry->Line;


    PXCompilerSymbolEntry mergCopy;


    PXBool isInNewLine = currentLine != mergCopy.Line;

    while(!isInNewLine)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        isInNewLine = currentLine != mergCopy.Line;

        if(!isInNewLine)
        {
            PXCompilerSymbolEntryMergeCurrentWithNext(pxCompiler, &mergCopy);
        }
    }

    PXCompilerSymbolEntryPeek(pxCompiler);

    return 0;
}

PXSize PXAPI PXCompilerSymbolRewind(PXCompiler PXREF pxCompiler, const PXSize amount)
{
    const PXSize totalSize = sizeof(PXCompilerSymbolEntry);

    PXSize reveredSize = PXFileDataPosition(pxCompiler->ReadInfo.FileCache);
    PXBool isInvalidToken;

    do
    {
        PXFileCursorRewind(pxCompiler->ReadInfo.FileCache, totalSize * amount);

        isInvalidToken = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInvalid);
    }
    while(isInvalidToken);

    reveredSize = reveredSize - PXFileDataPosition(pxCompiler->ReadInfo.FileCache);

    return reveredSize;
}

PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntry* pxCompilerSymbolEntry = &pxCompiler->ReadInfo.SymbolEntryCurrent;

    PXSize readBytes = 0;

    do
    {
        readBytes += PXFileReadB(pxCompiler->ReadInfo.FileCache, pxCompilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

        if(readBytes == 0)
        {
            pxCompilerSymbolEntry->ID = PXCompilerSymbolLexerEndOfFile;
            pxCompilerSymbolEntry->Source = PXNull;
            pxCompilerSymbolEntry->Coloum = -1;
            pxCompilerSymbolEntry->Line = -1;
            pxCompilerSymbolEntry->Size = 0;
            return 0;
        }
    }
    while(PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry->ID);

    return readBytes;
}

PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler PXREF pxCompiler)
{
    const PXSize totalSize = sizeof(PXCompilerSymbolEntry);
    const PXSize positionBefore = PXFileDataPosition(pxCompiler->ReadInfo.FileCache);

    PXCompilerSymbolEntry pxCompilerSymbolEntry;

    PXFileCursorAdvance(pxCompiler->ReadInfo.FileCache, totalSize);

    /*
    * do
    {
    PXFileCursorAdvance(pxCompiler->FileCache, totalSize);
    PXCompilerSymbolEntryPeek(pxCompiler, &pxCompilerSymbolEntry);
    }
    while (PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry.ID);
    */



    return PXFileDataPosition(pxCompiler->ReadInfo.FileCache) - positionBefore;
}

PXSize PXAPI PXCompilerSymbolEntryPeek(PXCompiler PXREF pxCompiler)
{
    const PXSize readBytes = PXCompilerSymbolEntryExtract(pxCompiler);

    PXFileCursorRewind(pxCompiler->ReadInfo.FileCache, readBytes);

    return readBytes;
}

PXBool PXAPI PXCompilerSymbolEntryPeekEnsure(PXCompiler PXREF pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
    const PXBool isTarget = PXCompilerSymbolEntryPeekCheck(pxCompiler, pxCompilerSymbolLexer);

    if(!isTarget)
    {
#if PXLogEnable && PXCompilerDEBUG
        const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);
        const char* gotTypeText = PXCompilerCompilerSymbolLexerToString(pxCompiler->ReadInfo.SymbolEntryCurrent.ID);

        PXLogPrint
        (
            PXLoggingError,
            PXCompilerText,
            PXCompilerTextParse,
            "Symbol not as expected!\n"
            "Expected:<%s> but got <%s>\n"
            "Line: %i, Character: %i, Size: %i",
            expectedTypeText,
            gotTypeText,
            pxCompiler->ReadInfo.SymbolEntryCurrent.Line,
            pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum,
            pxCompiler->ReadInfo.SymbolEntryCurrent.Size
        );
#endif

        DebugBreak();
    }

    return isTarget;
}

PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler PXREF pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
    const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);
    const PXBool isCorrectType = pxCompilerSymbolLexer == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    return isCorrectType;
}

PXBool PXAPI PXCompilerSymbolEntryEnsureCheckList(PXCompiler PXREF pxCompiler, const PXCompilerSymbolLexer PXREF pxCompilerSymbolLexerList, const PXSize amount)
{
    const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);

    for(PXSize i = 0; i < amount; ++i)
    {
        PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];

        PXBool succ = pxCompilerSymbolLexer == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

        if(succ)
        {
            return PXTrue;
        }
    }

#if PXLogEnable && PXCompilerDEBUG

    const char* gotTypeText = PXCompilerCompilerSymbolLexerToString(pxCompiler->ReadInfo.SymbolEntryCurrent.ID);

    PXText bufferOptions;
    PXTextConstructBufferA(&bufferOptions, 64);

    for(PXSize i = 0; i < amount; ++i)
    {
        PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];

        const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);

        PXAppendF(&bufferOptions, "%s, ", expectedTypeText);
    }


    PXLogPrint
    (
        PXLoggingError,
        PXCompilerText,
        PXCompilerTextParse,
        "Symbol not as expected!\n"
        "Allowed:%s but got <%s>\n"
        "Line: %i, Character: %i, Size: %i",
        bufferOptions.A,
        gotTypeText,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Line,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Size
    );
#endif

    return PXFalse;
}




const PXI8U PXCompilerCharMatchList[] =
{
    // white space / new lines
    '\r',
    '\n',
    ' ',
    '\t',

    '.',
    ',',
    ':',
    ';',

    '?',
    '!',
    '_',
    '#',
    '+',
    '-',
    '*',
    '/',
    '\\',
    '&',
    '%',
    '|',
    '°',
    '^',
    '~',
    '`',
    '´',
    '\'',
    '\"',
    '@',

    '=',
    '{',
    '}',
    '(',
    ')',
    '[',
    ']',  
    '<',
    '>',

    // true / false
    'T',
    't',
    'F',
    'f',

    // floats/ints
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9'
};
const PXI8U PXCompilerCharMatchListSize = sizeof(PXCompilerCharMatchList) / sizeof(PXI8U);

const PXI8U PXCompilerCharIDList[] =
{
PXCompilerSymbolLexerNewLine,
PXCompilerSymbolLexerNewLine,
PXCompilerSymbolLexerWhiteSpace,
PXCompilerSymbolLexerTab,
PXCompilerSymbolLexerDot,
PXCompilerSymbolLexerComma,
PXCompilerSymbolLexerColon,
PXCompilerSymbolLexerSemiColon,
PXCompilerSymbolLexerQuestionmark,
PXCompilerSymbolLexerExclamation,
PXCompilerSymbolLexerUnderscore,
PXCompilerSymbolLexerHash,
PXCompilerSymbolLexerPlus,
PXCompilerSymbolLexerMinus,
PXCompilerSymbolLexerAsterisk,
PXCompilerSymbolLexerSlash,
PXCompilerSymbolLexerSlashBack,
PXCompilerSymbolLexerAmpercant,
PXCompilerSymbolLexerPercent,
PXCompilerSymbolLexerBar,
PXCompilerSymbolLexerDegree,
PXCompilerSymbolLexerExponent,
PXCompilerSymbolLexerTilde,
PXCompilerSymbolLexerApostrophe,
PXCompilerSymbolLexerApostropheB,
PXCompilerSymbolLexerQuiteSingle,
PXCompilerSymbolLexerQuoteDouble,
PXCompilerSymbolLexerAt,
PXCompilerSymbolLexerEqual,
PXCompilerSymbolLexerBracketCurlyOpen,
PXCompilerSymbolLexerBracketCurlyClose,
PXCompilerSymbolLexerBrackedRoundOpen,
PXCompilerSymbolLexerBrackedRoundClose,
PXCompilerSymbolLexerBracketSquareOpen,
PXCompilerSymbolLexerBracketSquareClose,
PXCompilerSymbolLexerBracketAngleOpen,
PXCompilerSymbolLexerBracketAngleClose,
PXCompilerSymbolLexerBool,
PXCompilerSymbolLexerBool,
PXCompilerSymbolLexerBool,
PXCompilerSymbolLexerBool,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric,
PXCompilerSymbolLexerNumeric
};


PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile PXREF tokenStream, const char PXREF text, const PXSize textSize, PXCompilerSymbolEntry PXREF compilerSymbolEntry)
{
    const PXI8U index = PXMemoryCompareI8V(PXCompilerCharMatchList, PXCompilerCharMatchListSize, text[0]);


    // Special behaviour if we only have one symbol
    if(textSize == 1)
    {
        if(0xFF != index)
        {
            PXCompilerSymbolLexer symbol = PXCompilerCharIDList[index];

            switch(symbol)
            {
                case PXCompilerSymbolLexerNumeric:
                {
                    compilerSymbolEntry->I32U = text[0] - '0';
                    break;
                }
                case PXCompilerSymbolLexerBool:
                {
                    compilerSymbolEntry->ID = PXCompilerSymbolLexerGeneric;
                    break;
                }
            }

            return symbol; // Is defines symbol
        }
        else
        {
            // Simple symbol, maybe one single letter
            return PXCompilerSymbolLexerGeneric;
        }
    }


    if(0xFF != index)
    {
        compilerSymbolEntry->ID = PXCompilerCharIDList[index];
               
        switch(compilerSymbolEntry->ID)
        {
            case PXCompilerSymbolLexerSlash:
            case PXCompilerSymbolLexerHash:
            case PXCompilerSymbolLexerBracketSquareClose:
            case PXCompilerSymbolLexerBracketSquareOpen:
            case PXCompilerSymbolLexerBrackedRoundClose:
            case PXCompilerSymbolLexerBrackedRoundOpen:
            case PXCompilerSymbolLexerBracketCurlyClose:
            case PXCompilerSymbolLexerBracketCurlyOpen:
            case PXCompilerSymbolLexerTilde:
            case PXCompilerSymbolLexerExponent:
            case PXCompilerSymbolLexerDegree:
            case PXCompilerSymbolLexerBar:
            case PXCompilerSymbolLexerPercent:
            case PXCompilerSymbolLexerAmpercant:
            case PXCompilerSymbolLexerAsterisk:
            case PXCompilerSymbolLexerExclamation:
            case PXCompilerSymbolLexerQuestionmark:
            case PXCompilerSymbolLexerSemiColon:
            case PXCompilerSymbolLexerColon:
            case PXCompilerSymbolLexerComma:
            case PXCompilerSymbolLexerDot:
            {
                compilerSymbolEntry->Size = 1;

                return compilerSymbolEntry->ID;
            }
            case PXCompilerSymbolLexerAt:
            case PXCompilerSymbolLexerQuiteSingle:
            case PXCompilerSymbolLexerQuoteDouble:
            {
                const PXBool isFull = text[textSize - 1] == text[0];

                if(!isFull)
                {
                    char PXREF dataAdress = compilerSymbolEntry->Source + 1u;
                    PXSize symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, text[0]);
                    PXBool hasIndex = symbolPositionIndex != (PXSize)-1;

                    if(!hasIndex) // Prop string has empty space, so we need to mend them together
                    {
                        char PXREF dataAdressExtended = &dataAdress[compilerSymbolEntry->Size - 1];

                        PXBool isEmptySpace = dataAdressExtended[0] == ' ';

                        if(isEmptySpace) // Parse into oblivion for next
                        {
                            const PXSize spaceLeft = PXFileRemainingSize(tokenStream);

                            symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, spaceLeft, text[0]);
                            hasIndex = symbolPositionIndex != (PXSize)-1;

                            if(hasIndex)
                            {
                                compilerSymbolEntry->Size = symbolPositionIndex;
                                PXFileCursorAdvance(tokenStream, symbolPositionIndex);
                            }
                        }
                    }

                    compilerSymbolEntry->Size = symbolPositionIndex + 2u;
                }

                return PXCompilerSymbolLexerString;
            }
            case PXCompilerSymbolLexerEqual:
            {
                if(text[1] == '=')
                {
                    if(text[2] == '=')
                    {
                        compilerSymbolEntry->Size = 3;

                        return PXCompilerSymbolLexerEqualTrippel;
                    }

                    compilerSymbolEntry->Size = 2;

                    return PXCompilerSymbolLexerEqualDouble;
                }

                compilerSymbolEntry->Size = 1;

                return PXCompilerSymbolLexerEqual;
            }
            case PXCompilerSymbolLexerBracketAngleClose:
            {
                if(text[1] == '=')
                {
                    compilerSymbolEntry->Size = 2;

                    return PXCompilerSymbolLexerBiggerAndEqual;
                }

                compilerSymbolEntry->Size = 1;

                return PXCompilerSymbolLexerBracketAngleClose;
            }
            case PXCompilerSymbolLexerBracketAngleOpen:
            {
                if(text[1] == '=')
                {
                    if(text[2] == '>')
                    {
                        compilerSymbolEntry->Size = 3;

                        return PXCompilerSymbolLexerCompareThreeWay;
                    }

                    compilerSymbolEntry->Size = 2;

                    return PXCompilerSymbolLexerSmalerAndEqual;
                }

                compilerSymbolEntry->Size = 1;

                return PXCompilerSymbolLexerBracketAngleOpen;
            }
            case PXCompilerSymbolLexerBool:
            {
                PXBool result = PXTextCompareIgnoreCaseA(text, textSize, "true", 4) && textSize > 4;

                if(result)
                {
                    compilerSymbolEntry->CU = PXYes;

                    return PXCompilerSymbolLexerBool;
                }

                result = PXTextCompareIgnoreCaseA(text, textSize, "false", 5) && textSize > 5;

                if(result)
                {
                    compilerSymbolEntry->CU = PXNo;

                    return PXCompilerSymbolLexerBool;
                }      

                break;
            }
            case PXCompilerSymbolLexerPlus:
            case PXCompilerSymbolLexerMinus:
            {
                const PXBool isDouble = text[0] == text[1];

                if(isDouble)
                {
                    compilerSymbolEntry->Size = 2;

                    if(PXCompilerSymbolLexerPlus == compilerSymbolEntry->ID)
                    {
                        return PXCompilerSymbolLexerPlusDouble;
                    }
                    else // if(PXCompilerSymbolLexerMinus == symbol)
                    {
                        return PXCompilerSymbolLexerMinusDouble;
                    }
                }

                if(!PXMathIsInRange(text[1], '0', '9'))
                {
                    break;
                }

                // Fall through!
            }
            case PXCompilerSymbolLexerNumeric:
            {
                // Probe for number
                const PXSize dotIndex = PXTextFindFirstCharacterA(text, textSize, '.');
                const PXBool probablyFloatingPoint = dotIndex != (PXSize)-1;
                PXSize writtenNumbers = 0;

                PXBool isValidFloatSyntax = PXTrue;
                PXSize floatTextSize = 0;

                if(probablyFloatingPoint)
                {
                    // Validate is float is valid until the '.'
                    for(floatTextSize = 0; (floatTextSize <= dotIndex + 1) && isValidFloatSyntax; ++floatTextSize)
                    {
                        isValidFloatSyntax = PXTextIsAllowedForF32(text[floatTextSize]);
                    }

                    if(isValidFloatSyntax)
                    {
                        // If this is a float, check after the '.' until we hit non numbers
                        for(floatTextSize = dotIndex + 1; (floatTextSize <= textSize) && isValidFloatSyntax; ++floatTextSize)
                        {
                            isValidFloatSyntax = PXTextIsAllowedForF32(text[floatTextSize]);
                        }

                        floatTextSize -= 1;
                        isValidFloatSyntax = PXTrue;
                    }
                }

                PXText numberText;
                PXTextFromAdressA(&numberText, text, floatTextSize, textSize);

                if(probablyFloatingPoint && isValidFloatSyntax)
                {
                    PXF64 value = 0;

                    const PXSize writtenNumbers = PXTextToF64A(&numberText, &value);
                    const PXBool isPXF32 = floatTextSize == writtenNumbers;

                    if(isPXF32)
                    {
                        compilerSymbolEntry->Source = 0;
                        compilerSymbolEntry->Size = writtenNumbers;

#if OS64B
                        compilerSymbolEntry->F64 = value;
#else
                        compilerSymbolEntry->F32 = value;
#endif


                        return PXCompilerSymbolLexerReal;
                    }
                }
                else
                {
                    int value = 0;

                    const PXSize writtenNumbers = PXTextToInt(&numberText, &value);

                    if(writtenNumbers > 0)
                    {
                        compilerSymbolEntry->Source = 0;
                        compilerSymbolEntry->Size = writtenNumbers;
                        compilerSymbolEntry->I32S = value;

                        return PXCompilerSymbolLexerNumeric;
                    }
                }

                break;
            }
            case PXCompilerSymbolLexerUnderscore:
            {
                // Special case, we assume that if something that is longer 
                // that 1 char and starts with an underscore, it must be a 
                // generic element like a name.
                break; // Fall through to be handled as a string
            }
            default:
                return compilerSymbolEntry->ID;
        }
    }

    // Before we end, check if we can cleave internal stuff mixed into the block
#if 1
    PXSize i = compilerSymbolEntry->Size;
    PXSize newSize = 0;

    for(i = 0; i < compilerSymbolEntry->Size; ++i)
    {
        const char symbol = compilerSymbolEntry->Source[i];
        const PXBool isNumber =
            ('A' <= symbol && symbol <= 'Z') ||
            ('a' <= symbol && symbol <= 'z') ||
            ('0' <= symbol && symbol <= '9') || // We already checked if it begins with a letter. This here prevents problems that "Texture2D" will get splitted
            symbol == '.' || symbol == '_' || symbol == '/' || symbol == '\\' || symbol == '_'; // symbol == ':'

        newSize += isNumber;

        if(!isNumber)
        {
            break;
        }
    }

    compilerSymbolEntry->Size = newSize;
#endif

    return PXCompilerSymbolLexerGeneric;
}

PXResult PXAPI PXCompilerLexicalAnalysis(PXCompiler PXREF pxCompiler)
{
    if(!pxCompiler)
    {
        return PXResultRefusedParameterNull;
    }

    PXFile PXREF pxFileInput = pxCompiler->ReadInfo.FileInput;

    if(!PXFileDataAvailable(pxFileInput))
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXCompilerText,
            PXCompilerTextLexer,
            "No data to analyse!"
        );
#endif

        return PXResultRefusedParameterInvalid;
    }

    const PXI64U timeCounter = PXTimeCounterStampGet();

#if PXLogEnable
    PXText pxTextSize;
    PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
    PXTextFormatSize(&pxTextSize, PXFileDataPosition(pxFileInput));

    PXLogPrint
    (
        PXLoggingInfo,
        PXCompilerText,
        PXCompilerTextLexer,
        "Starting analisis for <%s>",
        pxTextSize.A
    );
#endif

    // Valid call
    {
        const PXBool isValidCall = pxCompiler->ReadInfo.FileCache && pxCompiler->ReadInfo.FileInput;

        if(!isValidCall)
        {
            return PXResultRefusedParameterInvalid;
        }
    }


    // Do you have a valid input file?

    // Do we have a valid outputfile?

    // Settings invalid?

    PXFileOpenInfo pxFileOpenInfo;
    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
    pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;
    pxFileOpenInfo.FileSizeRequest = PXFileRemainingSize(pxCompiler->ReadInfo.FileInput) * 8u;
    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;

    const PXResult pxOpenResult = PXFileOpen(pxCompiler->ReadInfo.FileCache, &pxFileOpenInfo);


    PXSize currentLine = 1;
    PXSize currentColoum = 1;
    PXBool isFirstWhiteSpaceInLine = 1u;

#if PXCompilerSanitise && 0
    PXMemorySet(pxCompiler->FileCache, '#', pxCompiler->FileCache->DataSize);
#endif

    const PXCompilerSymbolLexer newLineSymbol = pxCompiler->Flags & PXCompilerInterpretNewLineAsWhiteSpace ? PXCompilerSymbolLexerWhiteSpace : PXCompilerSymbolLexerNewLine;

    PXCompilerSymbolEntry compilerSymbolEntry;

    for(;;)
    {       
        PXBool isAtEnd = PXFileIsAtEnd(pxFileInput);

        if(isAtEnd)
        {
            break;
        }

        PXClear(PXCompilerSymbolEntry, &compilerSymbolEntry);

        compilerSymbolEntry.Size = PXFileRemainingSize(pxFileInput);
        compilerSymbolEntry.Source = (char*)PXFileDataAtCursor(pxFileInput);

        PXBool isEndOfString = '\0' == compilerSymbolEntry.Source[0];

        if(isEndOfString)
        {
            break;
        }

        //-----------------------------------------------------------------------------
        // Consume whitespace
        //-----------------------------------------------------------------------------
        {
            const PXSize whiteSpaceSize = PXFileSkipEmptySpace(pxFileInput); // Skip 'space' and TABs

            currentColoum += whiteSpaceSize;

            if(whiteSpaceSize)
            {
                isFirstWhiteSpaceInLine = 0;

                compilerSymbolEntry.ID = PXCompilerSymbolLexerWhiteSpace;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = whiteSpaceSize;

                if((PXCompilerKeepWhiteSpace & pxCompiler->Flags) && isFirstWhiteSpaceInLine)
                {
                    PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
                }

                continue;
            }
        }

        //-----------------------------------------------------------------------------
        // Consume new line
        //-----------------------------------------------------------------------------
        {
            const PXSize endofLineSize = PXFileSkipEndOfLineCharacters(pxFileInput);

            if(endofLineSize)
            {
                const PXSize linesSkipped = PXTextCountA(compilerSymbolEntry.Source, endofLineSize, '\n');

                compilerSymbolEntry.ID = newLineSymbol;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = endofLineSize;

                isFirstWhiteSpaceInLine = 1u;

                currentColoum = 1; // Reset, next entry will begin in new line
                currentLine += linesSkipped;

                if(PXCompilerKeepNewLine & pxCompiler->Flags)
                {
                    PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
                }

                continue;
            }
        }
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Extract comments, Single line and multible lines
        //-----------------------------------------------------------------------------
        {
            PXBool isLineComment = PXTextCompareA
            (
                compilerSymbolEntry.Source, 
                pxCompiler->CommentSingleLine.SizeUsed, 
                pxCompiler->CommentSingleLine.A,
                pxCompiler->CommentSingleLine.SizeUsed,
                PXTextCompareRequireSameLength
            );

            if(isLineComment)
            {
                compilerSymbolEntry.ID = PXCompilerSymbolLexerComment; // This is a comment
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Source += pxCompiler->CommentSingleLine.SizeUsed;
                compilerSymbolEntry.Size = PXFileSkipLine(pxFileInput) - pxCompiler->CommentSingleLine.SizeUsed;

                // if we have multtible line comments in a row, we want to merge them.
                PXBool merged = PXFalse;

                do
                {
                    const char* isChainedComment = &compilerSymbolEntry.Source[compilerSymbolEntry.Size];
                    isLineComment = PXTextCompareA
                    (
                        isChainedComment,
                        pxCompiler->CommentSingleLine.SizeUsed,
                        pxCompiler->CommentSingleLine.A, 
                        pxCompiler->CommentSingleLine.SizeUsed,
                        0
                    );

                    if(isLineComment)
                    {
                        compilerSymbolEntry.Size += PXFileSkipLine(pxFileInput);
                    }
                } while(isLineComment);

                // Cleanup
                while(compilerSymbolEntry.Source[0] == ' ')
                {
                    ++compilerSymbolEntry.Source;
                    --compilerSymbolEntry.Size;
                }

                while
                    (
                    compilerSymbolEntry.Source[compilerSymbolEntry.Size - 1] == '\n' ||
                    compilerSymbolEntry.Source[compilerSymbolEntry.Size - 1] == '\r'
                    )
                {
                    --compilerSymbolEntry.Size;
                }

                if(pxCompiler->Flags & PXCompilerKeepComments)
                {
                    PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
                }

                continue;
            }

            const PXBool isStartOfMultibleLineComment = PXTextCompareA
            (
                compilerSymbolEntry.Source, 
                compilerSymbolEntry.Size,
                pxCompiler->CommentMultibleLineBegin.Data,
                pxCompiler->CommentMultibleLineBegin.SizeUsed,
                0
            );

            if(isStartOfMultibleLineComment)
            {
                const PXSize index = PXTextFindFirstStringA
                (
                    compilerSymbolEntry.Source,
                    compilerSymbolEntry.Size,
                    pxCompiler->CommentMultibleLineEnd.Data,
                    pxCompiler->CommentMultibleLineEnd.SizeUsed
                );
                const PXBool isCompleate = index != -1;

                if(!isCompleate)
                {
                    // Error, we have a multiline comment without an end.
                    // printf("Errr\n");
                    return PXResultInvalid;
                }

                compilerSymbolEntry.ID = PXCompilerSymbolLexerComment;
                compilerSymbolEntry.Source += pxCompiler->CommentMultibleLineEnd.SizeUsed;
                compilerSymbolEntry.Size -= pxCompiler->CommentMultibleLineEnd.SizeUsed;
                compilerSymbolEntry.Size += index;

                if(pxCompiler->Flags & PXCompilerKeepComments)
                {
                    PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);
                }

                continue;
            }
        }
        //-----------------------------------------------------------------------------

        //-----------------------------------------------------------------------------
        // Consume block
        //-----------------------------------------------------------------------------
        {
            PXSize fullBlockSize = PXFileSkipBlock(pxFileInput); // consume block
            char* blockStart = compilerSymbolEntry.Source;

            while(fullBlockSize > 0)
            {
                compilerSymbolEntry.Source = blockStart;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = fullBlockSize;
                compilerSymbolEntry.ID = PXCompilerTryAnalyseType(pxFileInput, compilerSymbolEntry.Source, compilerSymbolEntry.Size, &compilerSymbolEntry);

                const PXBool skip =
                    PXCompilerSymbolLexerWhiteSpace == compilerSymbolEntry.ID ||
                    PXCompilerSymbolLexerNewLine == compilerSymbolEntry.ID ||
                    PXCompilerSymbolLexerTab == compilerSymbolEntry.ID;

                if(skip)
                {
                    PXFileCursorRewind(pxFileInput, compilerSymbolEntry.Size);
                    break;
                }

                if(compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
                {
                    ++compilerSymbolEntry.Source;
                    compilerSymbolEntry.Size -= 2;
                }

                PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);

                if(compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
                {
                    --compilerSymbolEntry.Source;
                    compilerSymbolEntry.Size += 2;
                }

                if(fullBlockSize < compilerSymbolEntry.Size) // Block got expanded manually, considering this as done
                {
                    break;
                }

                // Change data for next itteration
                blockStart += compilerSymbolEntry.Size; // Move data point forward
                fullBlockSize -= compilerSymbolEntry.Size; // Reduce blocksize by scanned data
                currentColoum += compilerSymbolEntry.Size; // Forward the coloum of the data position
            }
        }
    }

    // Mark end of output Stream
   // pxCompiler->ReadInfo.FileCache->DataUsed = pxCompiler->ReadInfo.FileCache->DataCursor;
    PXFileCursorPositionTerminate(pxCompiler->ReadInfo.FileCache);

    PXI64U timeCounterB = PXTimeCounterStampGet() - timeCounter;
    PXF32 delta = PXTimeCounterStampToSecoundsF(timeCounterB);


#if PXLogEnable
    PXSize percentage = PXFileDataUtilized(pxCompiler->ReadInfo.FileCache);

    PXLogPrint
    (
        PXLoggingInfo,
        PXCompilerText,
        PXCompilerTextLexer,
        "Finished analisis.\n"
        "%10s : %i\n"
        "%10s : %-6.3fs\n"
        "%10s : %i/%i (%i%%)",
        "Entrys", pxCompiler->SymbolsRead,
        "Time", delta,
        "Buffer", PXFileDataPosition(pxCompiler->ReadInfo.FileCache), PXFileAllocatedSize(pxCompiler->ReadInfo.FileCache), percentage
    );
#endif

    PXFileCursorToBeginning(pxCompiler->ReadInfo.FileCache);

    return PXResultOK;
}

PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler PXREF pxCompiler, PXText PXREF pxText)
{
    PXCompilerSymbolEntryExtract(pxCompiler); // Expect a name.

    const PXBool isText = PXCompilerSymbolLexerGeneric == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    if(!isText)
    {
        return PXNo;
    }

    PXSize line = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
    PXSize coloumStart = pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum;
    PXSize dataBlockSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
    char* dataBlockPoint = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;

    while(1u)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        const PXBool newLine = (pxCompiler->ReadInfo.SymbolEntryCurrent.Line != line);
        const PXBool validSymbols =
            (pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerHash) ||
            (pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerNumeric) ||
            (pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric);

        const PXBool isDone =
            (newLine ||
             (pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerNewLine)) ||
            !validSymbols;


        if(newLine)
        {
            break;
        }

        PXCompilerSymbolEntryExtract(pxCompiler);

        dataBlockSize = (pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum - coloumStart) + pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
    }

    pxText->SizeUsed = PXTextCopyA(dataBlockPoint, dataBlockSize, pxText->A, pxText->SizeAllocated);

    return PXYes;
}

PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler PXREF pxCompiler, char PXREF text, const PXSize textMaxSize, PXSize PXREF textSize)
{
    PXText pxText;
    PXTextFromAdressA(&pxText, text, 0, textMaxSize);
    const PXBool result = PXCompilerParseStringUntilNewLine(pxCompiler, &pxText);

    *textSize = pxText.SizeUsed;

    return result;
}

PXBool PXAPI PXCompilerEnsureAndCompare(PXCompiler PXREF pxCompiler, const char PXREF text, const PXSize textSize)
{
    const PXBool isText = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric);

    if(!isText)
    {
        return PXFalse;
    }

    const PXBool isxform = PXTextCompareA
    (
        pxCompiler->ReadInfo.SymbolEntryCurrent.Source,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Size,
        text,
        textSize,
        PXTextCompareRequireSameLength
    );

    return isxform;
}

PXI8U PXAPI PXCompilerEnsureTextListAndCompare(PXCompiler PXREF pxCompiler, const char* PXREF listTextData, const PXI8U* listTextSize, const PXI8U amount)
{
    for(PXI8U i = 0; i < amount; ++i)
    {
        const char* text = listTextData[i];
        const PXI8U size = listTextSize[i];

        const PXBool isTarget = PXCompilerEnsureAndCompare(pxCompiler, text, size);

        if(isTarget)
        {
            return i;
        }
    }

    return (PXI8U)-1;
}

PXBool PXAPI PXCompilerEnsurePropertyText
(
    PXCompiler PXREF pxCompiler,
    const char PXREF propertyKey,
    const PXSize propertyKeySize,
    char* PXREF propertyValue,
    PXSize* propertyValueSize
)
{
    // Key
    const PXBool isTarget = PXCompilerEnsureAndCompare(pxCompiler, propertyKey, propertyKeySize);

    if(!isTarget)
    {
        return PXFalse;
    }

    // Equal
    PXCompilerSymbolEntryForward(pxCompiler);

    const PXBool isEqual = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);

    if(!isEqual)
    {
        return PXFalse;
    }

    // Value
    PXCompilerSymbolEntryForward(pxCompiler);

    const PXBool isString = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    if(!isString)
    {
        return PXFalse;
    }


    if(!propertyValue) // If we dont have a target, we are done, no copy
    {
        PXCompilerSymbolEntryForward(pxCompiler);

        return PXTrue;
    }

    *propertyValue = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;

    if(propertyValueSize)
    {
        *propertyValueSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
    }

    PXCompilerSymbolEntryForward(pxCompiler);

    // Write if we have a target

    return PXTrue;
}

PXBool PXAPI PXCompilerParseI32V(PXCompiler PXREF pxCompiler, PXI32U PXREF values, const PXSize valuesExpectedSize)
{
    PXSize i = 0;

    for(; i < valuesExpectedSize; ++i)
    {
        const PXBool isNumber = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);

        if(!isNumber)
        {
            return 0;
        }

        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;

        PXCompilerSymbolEntryForward(pxCompiler);
    }

    PXCompilerSymbolEntryPeek(pxCompiler);

    return i;
}

PXBool PXAPI PXCompilerParseF32(PXCompiler PXREF pxCompiler, PXF32 PXREF values)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    const PXBool isF32 = pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerReal;
    const PXBool isInt = pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerNumeric;
    const PXBool isValid = isF32 || isInt;

    if(isF32)
    {
#if OS64B
        * values = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
        * values = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif

        PXCompilerSymbolEntryForward(pxCompiler);
    }

    if(isInt)
    {
#if OS64B
        * values = pxCompiler->ReadInfo.SymbolEntryCurrent.I64S;
#else
        * values = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
#endif

        PXCompilerSymbolEntryForward(pxCompiler);
    }

    return isValid;
}

PXBool PXAPI PXCompilerParseF16V(PXCompiler PXREF pxCompiler, PXF16 PXREF values, const PXSize valuesMaxSize, PXSize PXREF valuesSize)
{
    PXF32 cache[8];

    for(PXSize i = 0; i < valuesMaxSize; ++i)
    {
        const PXBool isValid = PXCompilerParseF32(pxCompiler, &cache[i]);

        if(!isValid)
        {
            // we are at the line end

            return PXNo; // finished
        }
    }

    PXMathF32ToF16(values, cache, valuesMaxSize);

    return PXYes;
}

PXBool PXAPI PXCompilerParseF32V(PXCompiler PXREF pxCompiler, PXF32 PXREF values, const PXSize valuesMaxSize, PXSize PXREF valuesSize)
{
    PXAssert(values, "Array cant be NULL");

    for(PXSize i = 0; i < valuesMaxSize; ++i)
    {
        const PXBool isValid = PXCompilerParseF32(pxCompiler, &values[*valuesSize]);

        if(!isValid)
        {
            // we are at the line end

            return PXNo; // finished
        }

        ++(*valuesSize);
    }

    return PXYes;
}

PXBool PXAPI PXCompilerParseCSVF32(PXCompiler PXREF pxCompiler, PXF32 PXREF values, const PXSize valuesSize)
{
    for(PXSize i = 0; i < valuesSize; i++)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

#if OS64B
        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif

        PXCompilerSymbolEntryForward(pxCompiler);

        if(i == (valuesSize - 1))
        {
            continue;
        }

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

PXBool PXAPI PXCompilerParseCSVF64(PXCompiler PXREF pxCompiler, PXF64 PXREF values, const PXSize valuesSize)
{
    for(PXSize i = 0; i < valuesSize; i++)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

#if OS64B
        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif

        PXCompilerSymbolEntryForward(pxCompiler);

        if(i == (valuesSize - 1))
        {
            continue;
        }

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

PXSize PXAPI PXCompilerParseText(PXCompiler PXREF pxCompiler, char PXREF text, const PXSize textLengthMax, const PXI8U flags)
{
    const PXCompilerSymbolLexer pxCompilerSymbolLexerList[2] =
    {
        PXCompilerSymbolLexerGeneric,
        PXCompilerSymbolLexerString
    };
    const PXI8U pxCompilerSymbolLexerListAmount = sizeof(pxCompilerSymbolLexerList) / sizeof(PXCompilerSymbolLexer);

    const PXBool check = PXCompilerSymbolEntryEnsureCheckList(pxCompiler, pxCompilerSymbolLexerList, pxCompilerSymbolLexerListAmount);

    if(!check)
    {
        return 0; // Definitly not a string
    }

    // We definitly have a string
    PXSize lengh = PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, text, textLengthMax);

    const PXI32U lineStart = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    PXCompilerSymbolEntryForward(pxCompiler);

    // There are stings that do not use "" and also conatin non-string characters like : or spaces.
    // To still detect those, we need to catch all symbols until the end of the line

    // If we dont want to detect more, we are done
    if(!(PXCompilerParseTextDetectUntilNextLine & flags))
    {
        return lengh;
    }

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        const PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        const PXI32U lineCurrent = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

        if((lineStart < lineCurrent) || isEndOfFile)
        {
            break;
        }

        lengh += PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, &text[lengh], textLengthMax - lengh);

        PXCompilerSymbolEntryForward(pxCompiler);
    }

    return lengh;
}

void PXAPI PXCompilerWrite(PXCompiler PXREF pxCompiler)
{
    PXCompilerWriteInfo PXREF pxCompilerWriteInfo = &pxCompiler->WriteInfo;

    // Check if valid call

    // Add missing parsing functions
    pxCompilerWriteInfo->WriteNode = PXCompilerWriteNode;

    if(!pxCompilerWriteInfo->WriteIncluded)
    {
        pxCompilerWriteInfo->WriteIncluded = PXCompilerWriteIncluded;
    }

    if(!pxCompilerWriteInfo->WriteComment)
    {
        pxCompilerWriteInfo->WriteComment = PXCompilerWriteComment;
    }

    if(!pxCompilerWriteInfo->WriteParameter)
    {
        pxCompilerWriteInfo->WriteParameter = PXCompilerWriteParameterList;
    }

    if(pxCompilerWriteInfo->TABSize == 0)
    {
        pxCompilerWriteInfo->TABSize = 2;
    }


    pxCompilerWriteInfo->CodeElementCurrent = &pxCompiler->CodeDocument->ElementList[0];

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXCompilerText,
        "Writer",
        "Starting writing..."
    );
#endif

    PXI64U timeCounter = PXTimeCounterStampGet();

    PXTime pxTime;
    PXTimeNow(&pxTime);

    PXFileWriteAF
    (
        pxCompiler->WriteInfo.FileOutput,
        "// This file is generated by PXUltima!\n"
        "// Date: %02i.%02i.%04i\n"
        "// Time: %02i:%02i:%02i\n\n",
        (int)pxTime.Day,
        (int)pxTime.Month,
        (int)pxTime.Year,
        (int)pxTime.Hour,
        (int)pxTime.Minute,
        (int)pxTime.Second
    );

    PXCompilerWriteNode(pxCompiler);

    PXI64U timeCounterB = PXTimeCounterStampGet() - timeCounter;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXCompilerText,
        "Writer",
        "Finished writing. Took %i ticks",
        timeCounterB
    );
#endif
}

void PXAPI PXCompilerWriteNode(PXCompiler PXREF pxCompiler)
{
    PXCodeDocumentElement PXREF entry = pxCompiler->WriteInfo.CodeElementCurrent;

#if 1
    switch(entry->Type)
    {
        case PXDocumentElementTypeFile:
        {
            pxCompiler->WriteInfo.WriteFile(pxCompiler);
            break;
        }
        case PXDocumentElementTypePreprocessorIncluded:
        {
            pxCompiler->WriteInfo.WriteIncluded(pxCompiler);
            break;
        }
        case PXDocumentElementTypeEnum:
        case PXDocumentElementTypeClass:
        case PXDocumentElementTypeStruct:
        {
            pxCompiler->WriteInfo.WriteContainer(pxCompiler);
            break;
        }
        case PXDocumentElementTypeFunction:
        {
            pxCompiler->WriteInfo.WriteFunction(pxCompiler);
            break;
        }

        default:
            break;
    }
#else
    PXCodeDocumentElement* sibling = pxCodeDocumentElement;

    for(size_t i = 0; sibling; ++i)
    {
        switch(sibling->Type)
        {
            case PXDocumentElementTypeFile:
            {
                PXJavaContainerWrite(sibling, pxFile);
                break;
            }
            case PXDocumentElementTypeIncluded:
            {
                PXJavaIncludedWrite(sibling, pxFile);
                break;
            }
            case PXDocumentElementTypeEnum:
            case PXDocumentElementTypeClass:
            case PXDocumentElementTypeStruct:
            {
                PXJavaContainerWrite(sibling, pxFile);
                break;
            }
            case PXDocumentElementTypeFunction:
            {
                PXJavaFunctionWrite(sibling, pxFile);
                break;
            }


            default:
                break;
        }

        if(sibling->ElementChildFirstBorn)
        {
            PXJavaElementWrite(sibling->ElementChildFirstBorn, pxFile);
        }

        sibling = sibling->ElementSibling;
    }
#endif
}

void PXAPI PXCompilerWriteIncluded(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF entry = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * pxCompiler->WriteInfo.CodeElementCurrent->Depth);
    PXFileWriteA(pxFile, "// depends on file ", 19);
    PXFileWriteA(pxFile, entry->NameAdress, entry->NameSize);
}

void PXAPI PXCompilerWriteComment(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF current = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * pxCompiler->WriteInfo.CodeElementCurrent->Depth);

    if(current->CommentSize > 0)
    {
        PXFileWriteText(pxFile, &pxCompiler->CommentSingleLine);
        PXFileWriteA(pxFile, current->CommentAdress, current->CommentSize);
        PXFileWriteNewLine(pxFile);
    }
}

void PXAPI PXCompilerWriteParameterList(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF current = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteC(pxFile, '(');

    for(PXHierarchicalNode* i = &current->Hierachy; i; i = (PXHierarchicalNode*)i->Sibling)
    {
        pxCompiler->WriteInfo.CodeElementCurrent = i;
        pxCompiler->WriteInfo.WriteDefinition(pxCompiler);
    }

    PXFileWriteC(pxFile, ')');

    pxCompiler->WriteInfo.CodeElementCurrent = current; // Recover
}