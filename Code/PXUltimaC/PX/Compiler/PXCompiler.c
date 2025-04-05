#include "PXCompiler.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXDocument.h>

#define PXCompilerDebug 0
#define PXCompilerSanitise 1

#if PXCompilerDebug
#include <stdio.h>
#endif

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
        return "�";
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
        return "PXF32";
    case PXCompilerSymbolLexerNumeric:
        return "int";
    case PXCompilerSymbolLexerString:
        return "string";
    case PXCompilerSymbolLexerEndOfFile:
        return "EOF";

    default:
        return PXNull;
    }
}

void PXAPI PXCompilerSymbolEntryAdd(PXCompiler* const pxCompiler, const PXCompilerSymbolEntry* const compilerSymbolEntry)
{
    const PXSize written = PXFileWriteB(pxCompiler->ReadInfo.FileCache, compilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

    ++pxCompiler->SymbolsRead;

#if PXCompilerDebug && PXCompilerDEBUG
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
            "L:%-4i C:%-4i S:%-4i (%s) %6.2f",
            compilerSymbolEntry->Line,
            compilerSymbolEntry->Coloum,
            compilerSymbolEntry->Size,
            typeName,
            compilerSymbolEntry->DataF
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
            "L:%-4i C:%-4i S:%-4i (%s) %i",
            compilerSymbolEntry->Line,
            compilerSymbolEntry->Coloum,
            compilerSymbolEntry->Size,
            typeName,
            compilerSymbolEntry->DataI32U
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
            "L:%-4i C:%-4i S:%-4i (%s) �6%s",
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
            "L:%-4i C:%-4i S:%-4i (%s)",
            compilerSymbolEntry->Line,
            compilerSymbolEntry->Coloum,
            compilerSymbolEntry->Size,
            typeName
        );
        break;
    }
#endif
}

PXSize PXAPI PXCompilerSymbolEntryMergeCurrentWithNext(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
    PXSize offset = 0;

    PXCompilerSymbolEntry oldCopy;
    PXClear(PXCompilerSymbolEntry, &oldCopy);

    PXCompilerSymbolEntry mergCopy;
    PXClear(PXCompilerSymbolEntry, &mergCopy);

    PXCompilerSymbolEntryExtract(pxCompiler);// Copy old value

    PXCopy(PXCompilerSymbolEntry, &pxCompiler->ReadInfo.SymbolEntryCurrent, &oldCopy);

    PXSize skippedBytes = PXCompilerSymbolEntryPeek(pxCompiler);
    PXSize skippedBlocks = (skippedBytes / 21 ) - 1;

    PXCopy(PXCompilerSymbolEntry, &pxCompiler->ReadInfo.SymbolEntryCurrent, &mergCopy);

    PXCompilerSymbolRewind(pxCompiler, 1); // Go back again

    // Write merged symbol
    {
        const PXInt8U symbolID = compilerSymbolEntry->ID;
        const PXInt32U size = oldCopy.Size + ((mergCopy.Coloum + mergCopy.Size) - (oldCopy.Coloum + oldCopy.Size));
        const PXTypeEntry pxFileDataElementType[] =
        {
            &oldCopy.ID, PXTypeInt08U,
            &oldCopy.Coloum, PXTypeInt32U,
            &oldCopy.Line, PXTypeInt32U,
            &size, PXTypeInt32U,
            &oldCopy.Source, PXTypeAdress
        };

        const PXSize written = PXFileWriteMultible(pxCompiler->ReadInfo.FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
    }

    // add magic offset of previous deleted entrys
    PXFileCursorAdvance(pxCompiler->ReadInfo.FileCache, skippedBlocks * 21);

    // delete old symbol
    {
        const PXInt8U symbolID = PXCompilerSymbolLexerInvalid;
        const PXInt32U emptyValue = 0;
        const void* emptyAdress = 0;
        const PXTypeEntry pxFileDataElementType[] =
        {
            &symbolID, PXTypeInt08U,
            &mergCopy.Coloum, PXTypeInt32U,
            &mergCopy.Line, PXTypeInt32U,
            &emptyValue, PXTypeInt32U,
            &emptyAdress, PXTypeAdress
        };

        //const PXSize written = PXFileWriteB(pxCompiler->ReadInfo.FileCache, pxCompilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

       PXFileWriteMultible(pxCompiler->ReadInfo.FileCache, pxFileDataElementType, sizeof(pxFileDataElementType));
    }

    PXCompilerSymbolRewind(pxCompiler, skippedBlocks+2); // Go back again, again

    PXCompilerSymbolEntryPeek(pxCompiler);

    return PXActionSuccessful;
}

PXSize PXAPI PXCompilerSymbolEntryMergeCurrentUntilNextLine(PXCompiler* const pxCompiler, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
    const PXSize currentLine = compilerSymbolEntry->Line;


    PXCompilerSymbolEntry mergCopy;


    PXBool isInNewLine = currentLine != mergCopy.Line;

    while(!isInNewLine)
    {
        PXCompilerSymbolEntryPeek(&mergCopy);

        isInNewLine = currentLine != mergCopy.Line;

        if(!isInNewLine)
        {
            PXCompilerSymbolEntryMergeCurrentWithNext(pxCompiler, &mergCopy);
        }
    }

    PXCompilerSymbolEntryPeek(&mergCopy);

    return 0;
}

PXSize PXAPI PXCompilerSymbolRewind(PXCompiler* const pxCompiler, const PXSize amount)
{
    const PXSize totalSize = sizeof(PXCompilerSymbolEntry);

    PXSize reveredSize = pxCompiler->ReadInfo.FileCache->DataCursor;
    PXBool isInvalidToken;

    do
    {
        PXFileCursorRewind(pxCompiler->ReadInfo.FileCache, totalSize * amount);

        isInvalidToken = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInvalid);
    }
    while (isInvalidToken);

    reveredSize = reveredSize - pxCompiler->ReadInfo.FileCache->DataCursor;

    return reveredSize;
}

PXSize PXAPI PXCompilerSymbolEntryExtract(PXCompiler* const pxCompiler)
{
    PXCompilerSymbolEntry* pxCompilerSymbolEntry = &pxCompiler->ReadInfo.SymbolEntryCurrent;

    PXSize readBytes = 0;

    do
    {
        readBytes += PXFileReadB(pxCompiler->ReadInfo.FileCache, pxCompilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

        if (readBytes == 0)
        {
            pxCompilerSymbolEntry->ID = PXCompilerSymbolLexerEndOfFile;
            pxCompilerSymbolEntry->Source = PXNull;
            pxCompilerSymbolEntry->Coloum = -1;
            pxCompilerSymbolEntry->Line = -1;
            pxCompilerSymbolEntry->Size = 0;
            return 0;
        }
    }
    while (PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry->ID);

    return readBytes;
}

PXSize PXAPI PXCompilerSymbolEntryForward(PXCompiler* const pxCompiler)
{
    const PXSize totalSize = sizeof(PXCompilerSymbolEntry);
    const PXSize positionBefore = pxCompiler->ReadInfo.FileCache->DataCursor;

    PXCompilerSymbolEntry pxCompilerSymbolEntry;

    PXFileCursorAdvance(pxCompiler->ReadInfo.FileCache, totalSize);

    /*
    *     do
    {
        PXFileCursorAdvance(pxCompiler->FileCache, totalSize);
        PXCompilerSymbolEntryPeek(pxCompiler, &pxCompilerSymbolEntry);
    }
    while (PXCompilerSymbolLexerInvalid == pxCompilerSymbolEntry.ID);
    */



    return pxCompiler->ReadInfo.FileCache->DataCursor - positionBefore;
}

PXSize PXAPI PXCompilerSymbolEntryPeek(PXCompiler* const pxCompiler)
{
    const PXSize readBytes = PXCompilerSymbolEntryExtract(pxCompiler);

    PXFileCursorRewind(pxCompiler->ReadInfo.FileCache, readBytes);

    return readBytes;
}

PXBool PXAPI PXCompilerSymbolEntryPeekEnsure(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
    const PXBool isTarget = PXCompilerSymbolEntryPeekCheck(pxCompiler, pxCompilerSymbolLexer);

    if(!isTarget)
    {
#if PXLogEnable && PXCompilerDEBUG
        const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);
        const char* gotTypeText = PXCompilerCompilerSymbolLexerToString( pxCompiler->ReadInfo.SymbolEntryCurrent.ID);

        PXLogPrint
        (
            PXLoggingError,
            "Compiler",
            "Parse",
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
    }

    return isTarget;
}

PXBool PXAPI PXCompilerSymbolEntryPeekCheck(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer pxCompilerSymbolLexer)
{
    const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);
    const PXBool isCorrectType = pxCompilerSymbolLexer ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    return isCorrectType;
}

PXBool PXAPI PXCompilerSymbolEntryEnsureCheckList(PXCompiler* const pxCompiler, const PXCompilerSymbolLexer* const pxCompilerSymbolLexerList, const PXSize amount)
{
    const PXSize readBytes = PXCompilerSymbolEntryPeek(pxCompiler);

    for(PXSize i = 0; i < amount; ++i)
    {
        PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];

        PXBool succ = pxCompilerSymbolLexer ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

        if(succ)
        {
            return PXTrue;
        }
    }

#if PXLogEnable && PXCompilerDEBUG

    const char* gotTypeText = PXCompilerCompilerSymbolLexerToString( pxCompiler->ReadInfo.SymbolEntryCurrent.ID);

    PXText bufferOptions;
    PXTextConstructBufferA(&bufferOptions, 64);

    for(size_t i = 0; i < amount; i++)
    {
        PXCompilerSymbolLexer pxCompilerSymbolLexer = pxCompilerSymbolLexerList[i];

        const char* expectedTypeText = PXCompilerCompilerSymbolLexerToString(pxCompilerSymbolLexer);

        PXTextAppendF(&bufferOptions, "%s, ", expectedTypeText);
    }


    PXLogPrint
    (
        PXLoggingError,
        "Compiler",
        "Parse",
        "Symbol not as expected!\n"
        "Allowed:%s but got <%s>\n"
        "Line: %i, Character: %i, Size: %i",
        bufferOptions.TextA,
        gotTypeText,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Line,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum,
        pxCompiler->ReadInfo.SymbolEntryCurrent.Size
    );
#endif

    return PXFalse;
}

PXCompilerSymbolLexer PXAPI PXCompilerTryAnalyseType(PXFile* const tokenStream, const char* const text, const PXSize textSize, PXCompilerSymbolEntry* const compilerSymbolEntry)
{
    if (textSize == 1)
    {
        switch (text[0])
        {
        case '.':
            return PXCompilerSymbolLexerDot;
        case ',':
            return PXCompilerSymbolLexerComma;
        case ':':
            return PXCompilerSymbolLexerColon;
        case ';':
            return PXCompilerSymbolLexerSemiColon;
        case '?':
            return PXCompilerSymbolLexerQuestionmark;
        case '!':
            return PXCompilerSymbolLexerExclamation;
        case '#':
            return PXCompilerSymbolLexerHash;
        case '+':
            return PXCompilerSymbolLexerPlus;
        case '-':
            return PXCompilerSymbolLexerMinus;
        case '/':
            return PXCompilerSymbolLexerSlash;
        case '*':
            return PXCompilerSymbolLexerAsterisk;
        case '\\':
            return PXCompilerSymbolLexerSlashBack;
        case '&':
            return PXCompilerSymbolLexerAmpercant;
        case '%':
            return PXCompilerSymbolLexerPercent;
        case '|':
            return PXCompilerSymbolLexerBar;
        case '�':
            return PXCompilerSymbolLexerDegree;
        case '^':
            return PXCompilerSymbolLexerExponent;
        case '~':
            return PXCompilerSymbolLexerTilde;
        case '\'':
            return PXCompilerSymbolLexerApostrophe;
        }
    }

    switch (text[0])
    {
    case '\r':
    case '\n':
        return PXCompilerSymbolLexerNewLine;

    case ' ':
    case '\t':
    {
        return PXCompilerSymbolLexerWhiteSpace;
    }

    case '.':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerDot;
    }
    case ',':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerComma;
    }
    case ':':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerColon;
    }
    case ';':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerSemiColon;
    }

    case '?':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerQuestionmark;
    }
    case '!':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerExclamation;
    }
    case '*':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerAsterisk;
    }
    case '&':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerAmpercant;
    }
    case '%':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerPercent;
    }
    case '|':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerBar;
    }
    case '�':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerDegree;
    }
    case '^':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerExponent;
    }
    case '~':
    {
        compilerSymbolEntry->Size = 1;
        return PXCompilerSymbolLexerTilde;
    }

    case '=':
    {
        if (text[1] == '=')
        {
            if (text[2] == '=')
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

    case '{':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBracketCurlyOpen;
    }
    case '}':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBracketCurlyClose;
    }
    case '(':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBrackedRoundOpen;
    }
    case ')':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBrackedRoundClose;
    }
    case '[':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBracketSquareOpen;
    }
    case ']':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBracketSquareClose;
    }
    case '#':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerHash;
    }
    case '>':
    {
        if (text[1] == '=')
        {
            compilerSymbolEntry->Size = 2;

            return PXCompilerSymbolLexerBiggerAndEqual;
        }

        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerBracketAngleClose;
    }
    case '<':
    {
        if (text[1] == '=')
        {
            if (text[2] == '>')
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
    case '/':
    {
        compilerSymbolEntry->Size = 1;

        return PXCompilerSymbolLexerSlash;
    }
    case '@':
    {
        const PXBool isFull = text[textSize - 1] == '@';

        if(!isFull)
        {
            char* const dataAdress = compilerSymbolEntry->Source + 1u;
            const PXSize symbolPositionQuotationMark = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, '@');
            const PXBool hasIndex = symbolPositionQuotationMark != (PXSize)-1;

            if(!hasIndex)
            {
                // Error
            }

            compilerSymbolEntry->Size = symbolPositionQuotationMark + 2u;
        }
        return PXCompilerSymbolLexerString;
    }
    case '\'':
    {
        const PXBool isFull = text[textSize - 1] == '\'';

        if (!isFull)
        {
            char* const dataAdress = compilerSymbolEntry->Source + 1u;
            const PXSize symbolPositionQuotationMark = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, '\'');
            const PXBool hasIndex = symbolPositionQuotationMark != (PXSize)-1;

            if (!hasIndex)
            {
                // Error
            }

            compilerSymbolEntry->Size = symbolPositionQuotationMark + 2u;
        }

        return PXCompilerSymbolLexerString;
    }
    case '\"':
    {
        const PXBool isFull = text[textSize - 1] == '\"';

        if (!isFull)
        {
            char* const dataAdress = compilerSymbolEntry->Source + 1u;
            PXSize symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, compilerSymbolEntry->Size, '\"');
            PXBool hasIndex = symbolPositionIndex != (PXSize)-1;

            if (!hasIndex) // Prop string has empty space, so we need to mend them together
            {
                char* const dataAdressExtended = &dataAdress[compilerSymbolEntry->Size-1];

                PXBool isEmptySpace = dataAdressExtended[0] == ' ';

                if (isEmptySpace) // Parse into oblivion for next
                {
                    const PXSize spaceLeft = PXFileRemainingSize(tokenStream);

                    symbolPositionIndex = PXTextFindFirstCharacterA(dataAdress, spaceLeft, '\"');
                    hasIndex = symbolPositionIndex != (PXSize)-1;

                    if (hasIndex)
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
    case 'T':
    case 't':
    {
        const PXBool  result = PXTextCompareIgnoreCaseA(text, textSize, "true", 4) && textSize > 4;

        if (result)
        {
            compilerSymbolEntry->CU = PXYes;

            return PXCompilerSymbolLexerBool;
        }

        break;
    }
    case 'F':
    case 'f':
    {
        const PXBool  result = PXTextCompareIgnoreCaseA(text, textSize, "false", 5) && textSize > 5;

        if (result)
        {
            compilerSymbolEntry->CU = PXNo;

            return PXCompilerSymbolLexerBool;
        }

        break;
    }
    case '+':
    case '-':
    {
        PXBool isDouble = text[0] == text[1];

        if (isDouble)
        {
            compilerSymbolEntry->Size = 2;

            return PXCompilerSymbolLexerGeneric;
        }
    }
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        // Probe for number
        const PXSize dotIndex = PXTextFindFirstCharacterA(text, textSize, '.');
        const PXBool probablyFloatingPoint = dotIndex != (PXSize)-1;
        PXSize writtenNumbers = 0;

        PXBool isValidFloatSyntax = PXTrue;
        PXSize floatTextSize = 0;

        if (probablyFloatingPoint)
        {
            // Validate is float is valid until the '.'
            for (floatTextSize = 0; (floatTextSize <= dotIndex + 1) && isValidFloatSyntax; ++floatTextSize)
            {
                isValidFloatSyntax = PXTextPXF32IsAllowedCharacter(text[floatTextSize]);
            }

            if (isValidFloatSyntax)
            {
                // If this is a float, check after the '.' until we hit non numbers
                for (floatTextSize = dotIndex + 1; (floatTextSize <= textSize) && isValidFloatSyntax; ++floatTextSize)
                {
                    isValidFloatSyntax = PXTextPXF32IsAllowedCharacter(text[floatTextSize]);
                }

                floatTextSize -= 1;
                isValidFloatSyntax = PXTrue;
            }
        }

        PXText numberText;
        PXTextConstructFromAdressA(&numberText, text, floatTextSize, textSize);

        if (probablyFloatingPoint && isValidFloatSyntax)
        {
            PXF64 value = 0;

            const PXSize writtenNumbers = PXTextToF64A(&numberText, &value);
            const PXBool isPXF32 = floatTextSize == writtenNumbers;

            if (isPXF32)
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

            if (writtenNumbers > 0)
            {
                compilerSymbolEntry->Source = 0;
                compilerSymbolEntry->Size = writtenNumbers;
                compilerSymbolEntry->I32S = value;

                return PXCompilerSymbolLexerNumeric;
            }
        }
    }
    }

    // Before we end, check if we can cleave internal stuff mixed into the block
#if 1
    PXSize i = compilerSymbolEntry->Size;
    PXSize newSize = 0;

    for (i = 0; i < compilerSymbolEntry->Size; ++i)
    {
        const char symbol = compilerSymbolEntry->Source[i];
        const PXBool isNumber =
            ('A' <= symbol && symbol <= 'Z') ||
            ('a' <= symbol && symbol <= 'z') ||
            ('0' <= symbol && symbol <= '9') || // We already checked if it begins with a letter. This here prevents problems that "Texture2D" will get splitted
            symbol == '.' || symbol == '_' ||  symbol == '/' || symbol == '\\'; // symbol == ':'

        newSize += isNumber;

        if (!isNumber)
        {
            break;
        }
    }

    compilerSymbolEntry->Size = newSize;
#endif

    return PXCompilerSymbolLexerGeneric;
}

PXActionResult PXAPI PXCompilerLexicalAnalysis(PXCompiler* const pxCompiler)
{
    if(!pxCompiler)
    {
        return PXActionRefusedArgumentNull;
    }

    PXFile* const pxFileInput = pxCompiler->ReadInfo.FileInput;

    if(!PXFileDataAvailable(pxFileInput))
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "Compiler",
            "Lexer",
            "No data to analyse!"
        );
#endif

        return PXActionRefusedArgumentInvalid;
    }

    const PXInt64U timeCounter = PXTimeCounterStampGet();

#if PXLogEnable
    PXText pxTextSize;
    PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
    PXTextFormatSize(&pxTextSize, pxFileInput->DataUsed);

    PXLogPrint
    (
        PXLoggingInfo,
        "Compiler",
        "Lexer",
        "Starting analisis for <%s>",
        pxTextSize.TextA
    );
#endif

    // Valid call
    {
        const PXBool isValidCall = pxCompiler->ReadInfo.FileCache && pxCompiler->ReadInfo.FileInput;

        if (!isValidCall)
        {
            return PXActionRefusedArgumentInvalid;
        }
    }


    // Do you have a valid input file?

    // Do we have a valid outputfile?

    // Settings invalid?

    PXFileOpenInfo pxFileOpenInfo;
    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
    pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;
    pxFileOpenInfo.FileSizeRequest = pxCompiler->ReadInfo.FileInput->DataUsed * 4u;
    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;

    const PXActionResult pxOpenResult = PXFileOpen(pxCompiler->ReadInfo.FileCache, &pxFileOpenInfo);


    PXSize currentLine = 1;
    PXSize currentColoum = 1;
    PXBool isFirstWhiteSpaceInLine = 1u;

#if PXCompilerSanitise && 0
    PXMemorySet(pxCompiler->FileCache, '#', pxCompiler->FileCache->DataSize);
#endif

    const PXCompilerSymbolLexer newLineSymbol = pxCompiler->Flags & PXCompilerInterpretNewLineAsWhiteSpace ? PXCompilerSymbolLexerWhiteSpace : PXCompilerSymbolLexerNewLine;

    while (!PXFileIsAtEnd(pxFileInput))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;
        PXClear(PXCompilerSymbolEntry, &compilerSymbolEntry);

        compilerSymbolEntry.Size = PXFileRemainingSize(pxFileInput);
        compilerSymbolEntry.Source = (char*)PXFileCursorPosition(pxFileInput);

        //-----------------------------------------------------------------------------
        // Consume whitespace
        //-----------------------------------------------------------------------------
        {
            const PXSize whiteSpaceSize = PXFileSkipEmptySpace(pxFileInput); // Skip 'space' and TABs

            currentColoum += whiteSpaceSize;

            if (whiteSpaceSize)
            {
                isFirstWhiteSpaceInLine = 0;

                compilerSymbolEntry.ID = PXCompilerSymbolLexerWhiteSpace;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = whiteSpaceSize;

                if (pxCompiler->Flags & PXCompilerKeepWhiteSpace && isFirstWhiteSpaceInLine)
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

            if (endofLineSize)
            {
                const PXSize linesSkipped = PXTextCountA(compilerSymbolEntry.Source, endofLineSize, '\n');

                compilerSymbolEntry.ID = newLineSymbol;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = endofLineSize;

                isFirstWhiteSpaceInLine = 1u;

                currentColoum = 1; // Reset, next entry will begin in new line
                currentLine += linesSkipped;

                if (pxCompiler->Flags & PXCompilerKeepNewLine)
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
            PXBool isLineComment = PXTextCompareA(compilerSymbolEntry.Source, pxCompiler->CommentSingleLineSize, pxCompiler->CommentSingleLine, pxCompiler->CommentSingleLineSize);

            if (isLineComment)
            {
                compilerSymbolEntry.ID = PXCompilerSymbolLexerComment; // This is a comment
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Source += pxCompiler->CommentSingleLineSize;
                compilerSymbolEntry.Size = PXFileSkipLine(pxFileInput) - pxCompiler->CommentSingleLineSize;

                // if we have multtible line comments in a row, we want to merge them.
                PXBool merged = PXFalse;

                do
                {
                    const char* isChainedComment = &compilerSymbolEntry.Source[compilerSymbolEntry.Size];
                    isLineComment = PXTextCompareA(isChainedComment, pxCompiler->CommentSingleLineSize, pxCompiler->CommentSingleLine, pxCompiler->CommentSingleLineSize);

                    if(isLineComment)
                    {
                        compilerSymbolEntry.Size += PXFileSkipLine(pxFileInput);
                    }
                }
                while(isLineComment);

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

            const PXBool isStartOfMultibleLineComment = PXTextCompareA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, pxCompiler->CommentMultibleLineBegin, pxCompiler->CommentMultibleLineBeginSize);

            if (isStartOfMultibleLineComment)
            {
                const PXSize index = PXTextFindFirstStringA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, pxCompiler->CommentMultibleLineEnd, pxCompiler->CommentMultibleLineEndSize);
                const PXBool isCompleate = index != -1;

                if (!isCompleate)
                {
                    // Error, we have a multiline comment without an end.
                    // printf("Errr\n");
                    return PXActionInvalid;
                }

                compilerSymbolEntry.ID = PXCompilerSymbolLexerComment;
                compilerSymbolEntry.Source += pxCompiler->CommentMultibleLineEndSize;
                compilerSymbolEntry.Size -= pxCompiler->CommentMultibleLineEndSize;
                compilerSymbolEntry.Size += index;

                if (pxCompiler->Flags & PXCompilerKeepComments)
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
            PXSize fullBlockSize = PXFileSkipBlock(pxFileInput);    // consume block
            char* blockStart = compilerSymbolEntry.Source;

            while (fullBlockSize > 0)
            {
                compilerSymbolEntry.Source = blockStart;
                compilerSymbolEntry.Line = currentLine;
                compilerSymbolEntry.Coloum = currentColoum;
                compilerSymbolEntry.Size = fullBlockSize;
                compilerSymbolEntry.ID = PXCompilerTryAnalyseType(pxFileInput, compilerSymbolEntry.Source, compilerSymbolEntry.Size, &compilerSymbolEntry);

                if (compilerSymbolEntry.ID == PXCompilerSymbolLexerWhiteSpace || compilerSymbolEntry.ID == PXCompilerSymbolLexerNewLine)
                {
                    break;
                }

                if (compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
                {
                    ++compilerSymbolEntry.Source;
                    compilerSymbolEntry.Size -= 2;
                }

                PXCompilerSymbolEntryAdd(pxCompiler, &compilerSymbolEntry);

                if (compilerSymbolEntry.ID == PXCompilerSymbolLexerString)
                {
                    --compilerSymbolEntry.Source;
                    compilerSymbolEntry.Size += 2;
                }

                if (fullBlockSize < compilerSymbolEntry.Size) // Block got expanded manually, considering this as done
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
    pxCompiler->ReadInfo.FileCache->DataUsed = pxCompiler->ReadInfo.FileCache->DataCursor;  

    PXInt64U timeCounterB = PXTimeCounterStampGet() - timeCounter;
    PXF32 delta = PXTimeCounterStampToSecoundsF(timeCounterB);


#if PXLogEnable
    int percentage = 0;

    if(pxCompiler->ReadInfo.FileCache->DataAllocated > 0)
    {
        percentage = (int)((pxCompiler->ReadInfo.FileCache->DataUsed / (PXF32)pxCompiler->ReadInfo.FileCache->DataAllocated) * 100);
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "Compiler",
        "Lexer",
        "Finished analisis.\n"
        "%10s : %i\n"
        "%10s : %-6.3fs\n"
        "%10s : %i/%i (%i%%)",
        "Entrys", pxCompiler->SymbolsRead,
        "Time", delta,
        "Buffer", pxCompiler->ReadInfo.FileCache->DataCursor, pxCompiler->ReadInfo.FileCache->DataAllocated, percentage
    );
#endif

    PXFileCursorToBeginning(pxCompiler->ReadInfo.FileCache);

    return PXActionSuccessful;
}

PXBool PXAPI PXCompilerParseStringUntilNewLine(PXCompiler* const pxCompiler, PXText* const pxText)
{
    PXCompilerSymbolEntryExtract(pxCompiler); // Expect a name.

    const PXBool isText = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    if (!isText)
    {
        return PXNo;
    }

    PXSize line =  pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
    PXSize coloumStart =  pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum;
    PXSize dataBlockSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
    char* dataBlockPoint =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;

    while (1u)
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


        if (newLine)
        {
            break;
        }

        PXCompilerSymbolEntryExtract(pxCompiler);

        dataBlockSize = ( pxCompiler->ReadInfo.SymbolEntryCurrent.Coloum - coloumStart) +  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
    }

    pxText->SizeUsed = PXTextCopyA(dataBlockPoint, dataBlockSize, pxText->TextA, pxText->SizeAllocated);

    return PXYes;
}

PXBool PXAPI PXCompilerParseStringUntilNewLineA(PXCompiler* const pxCompiler, char* const text, const PXSize textMaxSize, PXSize* const textSize)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, text, 0, textMaxSize);
    const PXBool result = PXCompilerParseStringUntilNewLine(pxCompiler, &pxText);

    *textSize = pxText.SizeUsed;

    return result;
}

PXBool PXAPI PXCompilerEnsureTextAndCompare(PXCompiler* const pxCompiler, const char* const text, const PXSize textSize)
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
        textSize
    );

    return isxform;
}

PXInt8U PXAPI PXCompilerEnsureTextListAndCompare(PXCompiler* const pxCompiler, const char** const listTextData, const PXInt8U* listTextSize, const PXInt8U amount)
{
    for(PXInt8U i = 0; i < amount; ++i)
    {
        const char* text = listTextData[i];
        const PXInt8U size = listTextSize[i];

        const PXBool isTarget = PXCompilerEnsureTextAndCompare(pxCompiler, text, size);

        if(isTarget)
        {
            return i;
        }
    }

    return (PXInt8U)-1;
}

PXBool PXAPI PXCompilerEnsurePropertyText
(
    PXCompiler* const pxCompiler,
    const char* const propertyKey,
    const PXSize propertyKeySize,
    char** const propertyValue,
    PXSize* propertyValueSize
)
{
    // Key
    const PXBool isTarget = PXCompilerEnsureTextAndCompare(pxCompiler, propertyKey, propertyKeySize);

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

PXBool PXAPI PXCompilerParseF32(PXCompiler* const pxCompiler, PXF32* const values)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    const PXBool isF32 =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerReal;
    const PXBool isInt =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerNumeric;
    const PXBool isValid = isF32 || isInt;

    if (isF32)
    {
#if OS64B
        *values =  pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
        *values = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif

        PXCompilerSymbolEntryExtract(pxCompiler);
    }

    if (isInt)
    {
#if OS64B
        *values = pxCompiler->ReadInfo.SymbolEntryCurrent.I64S;
#else
        *values = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
#endif

        PXCompilerSymbolEntryExtract(pxCompiler);
    }

    return isValid;
}

PXBool PXAPI PXCompilerParseF16V(PXCompiler* const pxCompiler, PXF16* const values, const PXSize valuesMaxSize, PXSize* const valuesSize)
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

PXBool PXAPI PXCompilerParseF32V(PXCompiler* const pxCompiler, PXF32* const values, const PXSize valuesMaxSize, PXSize* const valuesSize)
{
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

PXBool PXAPI PXCompilerParseCSVF32(PXCompiler* const pxCompiler, PXF32* const values, const PXSize valuesSize)
{
    for(PXSize i = 0; i < valuesSize; i++)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;

        PXCompilerSymbolEntryForward(pxCompiler);

        if(i == (valuesSize - 1))
        {
            continue;
        }

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

PXBool PXAPI PXCompilerParseCSVF64(PXCompiler* const pxCompiler, PXF64* const values, const PXSize valuesSize)
{    
    for(PXSize i = 0; i < valuesSize; i++)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        values[i] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;

        PXCompilerSymbolEntryForward(pxCompiler);

        if(i == (valuesSize-1))
        {   
            continue;
        }

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

void PXAPI PXCompilerWrite(PXCompiler* const pxCompiler)
{
    PXCompilerWriteInfo* const pxCompilerWriteInfo = &pxCompiler->WriteInfo;

    // Check if valid call

    // Add missing parsing functions
    pxCompilerWriteInfo->WriteNode = PXCompilerWriteNode;

    if(!pxCompilerWriteInfo->WriteInclude)
    {
        pxCompilerWriteInfo->WriteInclude = PXCompilerWriteInclude;
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
        "Compiler",
        "Writer",
        "Starting writing..."
    );
#endif

    PXInt64U timeCounter = PXTimeCounterStampGet();

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

    PXInt64U timeCounterB = PXTimeCounterStampGet() - timeCounter;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Compiler",
        "Writer",
        "Finished writing. Took %i ticks",
        timeCounterB
    );
#endif
}

void PXAPI PXCompilerWriteNode(PXCompiler* const pxCompiler)
{
    PXCodeDocumentElement* const entry = pxCompiler->WriteInfo.CodeElementCurrent;

#if 1
    switch(entry->Type)
    {
    case PXDocumentElementTypeFile:
    {
        pxCompiler->WriteInfo.WriteFile(pxCompiler);
        break;
    }
    case PXDocumentElementTypePreprocessorInclude:
    {
        pxCompiler->WriteInfo.WriteInclude(pxCompiler);
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
        case PXDocumentElementTypeInclude:
        {
            PXJavaIncludeWrite(sibling, pxFile);
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

void PXAPI PXCompilerWriteInclude(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const entry = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * pxCompiler->WriteInfo.CodeElementCurrent->Depth);
    PXFileWriteA(pxFile, "// depends on file ", 19);
    PXFileWriteA(pxFile, entry->NameAdress, entry->NameSize);
}

void PXAPI PXCompilerWriteComment(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const current = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * pxCompiler->WriteInfo.CodeElementCurrent->Depth);

    if(current->CommentSize > 0)
    {
        PXFileWriteA(pxFile, pxCompiler->CommentSingleLine, pxCompiler->CommentSingleLineSize);
        PXFileWriteA(pxFile, current->CommentAdress, current->CommentSize);
        PXFileWriteNewLine(pxFile);
    }
}

void PXAPI PXCompilerWriteParameterList(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const current = pxCompiler->WriteInfo.CodeElementCurrent;

    PXFileWriteC(pxFile, '(');

    for(PXHierarchicalNode* i = &current->Hierachy; i; i = (PXHierarchicalNode*)i->Sibling)
    {
        pxCompiler->WriteInfo.CodeElementCurrent = i;
        pxCompiler->WriteInfo.WriteDefinition(pxCompiler);
    }

    PXFileWriteC(pxFile, ')');

    pxCompiler->WriteInfo.CodeElementCurrent = current; // Recover
}
