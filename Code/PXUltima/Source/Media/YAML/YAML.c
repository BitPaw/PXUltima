#include "YAML.h"

#include <Compiler/PXCompiler.h>

YAMLLineType YAMLPeekLine(const void* line, const PXSize size)
{
    if (!(line && size))
    {
        return YAMLLineTypeInvalid;
    }

    const unsigned char id = *(const unsigned char* const)line;

    switch (id)
    {
        case '#':
            return YAMLLineTypeComment;

        case ':':
            return YAMLLineTypeKeyValueSeperator;

        case '>':
            return YAMLLineTypeNotationStyleExtendedLine;

        case '|':
            return YAMLLineTypeNotationStyleBlock;

        case '-':
        {
            const unsigned char* data = (unsigned char*)line + 1;
            const PXInt16U checkA = PXInt16Make('-', '-');
            const PXInt16U checkB = PXInt16Make(data[0], data[1]);
            const PXBool isSeperator = checkA == checkB;

            if (isSeperator)
            {
                return YAMLLineTypeSeperator;
            }
            else
            {
                return YAMLLineTypeListElement;
            }
        }
    }

    // unkown thing detected, check if it is a declaration
    {
        const char expectedColon = ((char*)line)[size - 1] ;
        const PXBool isColon = ':' == expectedColon;

        if (isColon)
        {
            return YAMLLineTypeKeyDeclare;
        }
    }

    return YAMLLineTypeUnkown;
}

PXActionResult YAMLFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXSize errorCounter = 0;
    PXDataStream tokenSteam;

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.KeepWhiteSpace = PXYes;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXYes;
        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.KeepTabs = PXYes;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        PXDataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    unsigned int indentCounter = 0;

    while (!PXDataStreamIsAtEnd(&tokenSteam))
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
                const YAMLLineType lineType = YAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (lineType)
                {
                    case YAMLLineTypeNotationStyleExtendedLine:
                    {
                        // interpret a new line as a whitespace.
                        // Line ends after new symbol declaration
                    }

                    case YAMLLineTypeUnkown:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const YAMLLineType lineType = YAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
                        const PXBool isColonSymbol = YAMLLineTypeKeyValueSeperator == lineType;

                        if (!isColonSymbol) // Format: xxxx :
                        {
                            // Error
                            continue;
                        }

                        // Fall though
                    }
                    case YAMLLineTypeKeyDeclare: // xxxx:
                    {
                        char* declname = compilerSymbolEntry.Source;
                        unsigned short declSize = compilerSymbolEntry.Size - 1;

                        PXDataStreamWriteI8U(outputStream, YAMLLineTypeKeyValueDeclare);
                        PXDataStreamWriteI8U(outputStream, indentCounter);
                        PXDataStreamWriteI16U(outputStream, declSize);
                        PXDataStreamWriteB(outputStream, declname, declSize);


                        // Fetch next value
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // consume "xxxxx: "
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // sonsume empty space or number


                        switch (compilerSymbolEntry.ID)
                        {

                        case PXCompilerSymbolLexerNewLine:
                        case PXCompilerSymbolLexerWhiteSpace:
                            PXDataStreamWriteI16U(outputStream, 0);
                            indentCounter = compilerSymbolEntry.Size;
                            break;

                        case PXCompilerSymbolLexerBool:
                        {
                            PXDataStreamWriteI16U(outputStream, sizeof(unsigned char));
                            PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerBool);
                            PXDataStreamWriteI8U(outputStream, compilerSymbolEntry.DataC);
                            break;
                        }

                        case PXCompilerSymbolLexerInteger:
                        {
                            PXDataStreamWriteI16U(outputStream, sizeof(unsigned int));
                            PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                            PXDataStreamWriteI32U(outputStream, compilerSymbolEntry.DataI);
                            break;
                        }

                        case PXCompilerSymbolLexerFloat:
                        {
                            PXDataStreamWriteI16U(outputStream, sizeof(float));
                            PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerFloat);
                            PXDataStreamWriteF(outputStream, compilerSymbolEntry.DataF);
                            break;
                        }
                        case PXCompilerSymbolLexerGenericElement:
                        case PXCompilerSymbolLexerString:
                        {
                            PXDataStreamWriteI16U(outputStream, compilerSymbolEntry.Size);
                            PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerString);
                            PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
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

    while (!PXDataStreamIsAtEnd(outputStream))
    {
        unsigned char depth = 0;
        YAMLLineType lineType = 0;

        PXDataStreamReadI8U(outputStream, &lineType);
        PXDataStreamReadI8U(outputStream, &depth);

        switch (lineType)
        {
        case YAMLLineTypeKeyValueDeclare:
        {
            unsigned short textASize = 0;
            char textA[256];
            unsigned short textBSize = 0;
            char textB[256];

            char emotySpace[25];

            MemoryClear(textA, 256u);
            MemoryClear(textB, 256u);
            MemoryClear(emotySpace, 25u);

            PXDataStreamReadI16U(outputStream, &textASize);
            PXDataStreamReadB(outputStream, textA, textASize);
            PXDataStreamReadI16U(outputStream, &textBSize);

            for (PXSize i = 0; i < depth; i++)
            {
                emotySpace[i] = ' ';
            }

            if (textBSize > 0)
            {
                PXCompilerSymbolLexer lexer;
                unsigned char lx = 0;

                PXDataStreamReadI8U(outputStream, &lx);

                lexer = lx;

                switch (lexer)
                {
                    case PXCompilerSymbolLexerBool:
                    {
                        unsigned char x = 0;

                        PXDataStreamReadI8U(outputStream, &x);
                        sprintf(textB, "%x", x);
                        break;
                    }

                    case PXCompilerSymbolLexerInteger:
                    {
                        unsigned int x = 0;

                        PXDataStreamReadI32U(outputStream, &x);
                        sprintf(textB, "%i", x);

                        break;
                    }

                    case PXCompilerSymbolLexerFloat:
                    {
                        float x = 0;

                        PXDataStreamReadF(outputStream, &x);

                        sprintf(textB, "%f", x);

                        break;
                    }

                    case PXCompilerSymbolLexerString:
                    {
                        PXDataStreamReadB(outputStream, textB, textBSize);
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
}

PXActionResult YAMLFileParse(PXDataStream* const ymlTokenInputStream, SerializationTypeInfo* const serializationTypeInfoList, const PXSize serializationTypeInfoListSize)
{


    return PXActionSuccessful;
}
