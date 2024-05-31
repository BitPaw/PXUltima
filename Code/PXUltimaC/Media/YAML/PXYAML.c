#include "PXYAML.h"

#include <Compiler/PXCompiler.h>

PXYAMLLineType PXAPI PXYAMLPeekLine(const void* line, const PXSize size)
{
    if (!(line && size))
    {
        return PXYAMLLineTypeInvalid;
    }

    const unsigned char id = *(const unsigned char* const)line;

    switch (id)
    {
        case '#':
            return PXYAMLLineTypeComment;

        case ':':
            return PXYAMLLineTypeKeyValueSeperator;

        case '>':
            return PXYAMLLineTypeNotationStyleExtendedLine;

        case '|':
            return PXYAMLLineTypeNotationStyleBlock;

        case '-':
        {
            const unsigned char* data = (unsigned char*)line + 1;
            const PXInt16U checkA = PXInt16Make('-', '-');
            const PXInt16U checkB = PXInt16Make(data[0], data[1]);
            const PXBool isSeperator = checkA == checkB;

            if (isSeperator)
            {
                return PXYAMLLineTypeSeperator;
            }
            else
            {
                return PXYAMLLineTypeListElement;
            }
        }
    }

    // unkown thing detected, check if it is a declaration
    {
        const char expectedColon = ((char*)line)[size - 1];
        const PXBool isColon = ':' == expectedColon;

        if (isColon)
        {
            return PXYAMLLineTypeKeyDeclare;
        }
    }

    return PXYAMLLineTypeUnkown;
}

PXActionResult PXAPI PXYAMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    PXSize errorCounter = 0;
    PXFile tokenSteam;
    PXClear(PXFile, &tokenSteam);

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.CodeDocument = (PXCodeDocument*)pxResourceLoadInfo->Target;
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = &tokenSteam;
    pxCompiler.CommentSingleLineSize = 1u;
    pxCompiler.CommentSingleLine = "#";
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes | PXCompilerKeepTABs;

    // Lexer - Level I
    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens

#if 0

    unsigned int indentCounter = 0;

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        switch (compilerSymbolEntry.ID)
        {
            case PXCompilerSymbolLexerWhiteSpace:
                indentCounter = compilerSymbolEntry.Size;
                break;

            case PXCompilerSymbolLexerNewLine:
                indentCounter = 0;
                break;

            default:
            {
                break;
            }
            case PXCompilerSymbolLexerSingleCharacter:
            case PXCompilerSymbolLexerGenericElement:
            {
                const PXYAMLLineType lineType = PXYAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (lineType)
                {
                    case PXYAMLLineTypeNotationStyleExtendedLine:
                    {
                        // interpret a new line as a whitespace.
                        // Line ends after new symbol declaration
                    }

                    case PXYAMLLineTypeUnkown:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXYAMLLineType lineType = PXYAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
                        const PXBool isColonSymbol = PXYAMLLineTypeKeyValueSeperator == lineType;

                        if (!isColonSymbol) // Format: xxxx :
                        {
                            // Error
                            continue;
                        }

                        // Fall though
                    }
                    case PXYAMLLineTypeKeyDeclare: // xxxx:
                    {
                        char* declname = compilerSymbolEntry.Source;
                        unsigned short declSize = compilerSymbolEntry.Size - 1;

                        PXFileWriteI8U(outputStream, PXYAMLLineTypeKeyValueDeclare);
                        PXFileWriteI8U(outputStream, indentCounter);
                        PXFileWriteI16U(outputStream, declSize);
                        PXFileWriteB(outputStream, declname, declSize);


                        // Fetch next value
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // consume "xxxxx: "
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // sonsume empty space or number


                        switch (compilerSymbolEntry.ID)
                        {

                            case PXCompilerSymbolLexerNewLine:
                            case PXCompilerSymbolLexerWhiteSpace:
                                PXFileWriteI16U(outputStream, 0);
                                indentCounter = compilerSymbolEntry.Size;
                                break;

                            case PXCompilerSymbolLexerBool:
                            {
                                PXFileWriteI16U(outputStream, sizeof(unsigned char));
                                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerBool);
                                PXFileWriteI8U(outputStream, compilerSymbolEntry.DataC);
                                break;
                            }

                            case PXCompilerSymbolLexerInteger:
                            {
                                PXFileWriteI16U(outputStream, sizeof(unsigned int));
                                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                                PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);
                                break;
                            }

                            case PXCompilerSymbolLexerFloat:
                            {
                                PXFileWriteI16U(outputStream, sizeof(float));
                                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerFloat);
                                PXFileWriteF(outputStream, compilerSymbolEntry.DataF);
                                break;
                            }
                            case PXCompilerSymbolLexerGenericElement:
                            case PXCompilerSymbolLexerString:
                            {
                                PXFileWriteI16U(outputStream, compilerSymbolEntry.Size);
                                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerString);
                                PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
                                break;
                            }
                        }

                        break;
                    }

                }

                break;
            }
        }
    }


    // Comment
    // # Text

    // List
    // - Text


    // [milk, pumpkin pie, eggs, juice]

    PXSize oldpos = outputStream->DataCursor;

    outputStream->DataCursor = 0;

    while (!PXFileIsAtEnd(outputStream))
    {
        PXInt8U depth = 0;
        PXInt8U lineTypeID = 0;

        PXYAMLLineType lineType = PXYAMLLineTypeInvalid;

        PXFileReadI8U(outputStream, &lineTypeID);
        PXFileReadI8U(outputStream, &depth);

        lineType = (PXYAMLLineType)lineTypeID;


        switch (lineType)
        {
            case PXYAMLLineTypeKeyValueDeclare:
            {
                unsigned short textASize = 0;
                char textA[256];
                unsigned short textBSize = 0;
                char textB[256];

                PXText pxTextBuffer;
                PXTextConstructFromAdressA(&pxTextBuffer, textB, 0, 256);

                char emotySpace[25];

                PXMemoryClear(textA, 256u);
                PXMemoryClear(textB, 256u);
                PXMemoryClear(emotySpace, 25u);

                PXFileReadI16U(outputStream, &textASize);
                PXFileReadB(outputStream, textA, textASize);
                PXFileReadI16U(outputStream, &textBSize);

                for (PXSize i = 0; i < depth; i++)
                {
                    emotySpace[i] = ' ';
                }

                if (textBSize > 0)
                {
                    PXCompilerSymbolLexer lexer;

                    {
                        PXInt8U lx = 0;

                        PXFileReadI8U(outputStream, &lx);

                        lexer = (PXCompilerSymbolLexer)lx;
                    }

                    switch (lexer)
                    {
                        case PXCompilerSymbolLexerBool:
                        {
                            PXBool x = 0;

                            PXFileReadI8U(outputStream, &x);

                            PXTextFromBool(&pxTextBuffer, x);

                            break;
                        }

                        case PXCompilerSymbolLexerInteger:
                        {
                            PXInt32U x = 0;

                            PXFileReadI32U(outputStream, &x);

                            PXTextFromInt(&pxTextBuffer, x);

                            break;
                        }

                        case PXCompilerSymbolLexerFloat:
                        {
                            float x = 0;

                            PXFileReadF(outputStream, &x);

                            PXTextFromInt(&pxTextBuffer, x);

                            break;
                        }

                        case PXCompilerSymbolLexerString:
                        {
                            PXFileReadB(outputStream, textB, textBSize);
                            break;
                        }
                    }
                }

                printf("[YAML][%i] %s %s:%s\n", depth, emotySpace, textA, textB);
            }

            default:
                break;
        }
    }
    outputStream->DataCursor = oldpos;

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXYAMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}