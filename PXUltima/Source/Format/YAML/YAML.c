#include "YAML.h"

#include <Compiler/PXCompiler.h>
#include <Container/ClusterValue.h>

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
            const unsigned short checkA = MakeShort('-', '-');
            const unsigned short checkB = MakeShort(data[0], data[1]);
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
        compilerSettings.KeepWhiteSpace = PXYes;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXYes;
        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.IntrepredNewLineAsWhiteSpace = PXNo;
        compilerSettings.KeepTabs = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXNo;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";
        compilerSettings.CommentMultibleLineSize = 0;
        compilerSettings.CommentMultibleLine = 0;

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

                        PXDataStreamWriteCU(outputStream, YAMLLineTypeKeyValueDeclare);
                        PXDataStreamWriteCU(outputStream, indentCounter);
                        PXDataStreamWriteSU(outputStream, declSize, EndianLittle);
                        PXDataStreamWriteP(outputStream, declname, declSize);


                        // Fetch next value
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // consume "xxxxx: "
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // sonsume empty space or number


                        switch (compilerSymbolEntry.ID)
                        {

                        case PXCompilerSymbolLexerNewLine:
                        case PXCompilerSymbolLexerWhiteSpace:
                            PXDataStreamWriteSU(outputStream, 0, EndianLittle);
                            indentCounter = compilerSymbolEntry.Size;
                            break;

                        case PXCompilerSymbolLexerBool:
                        {
                            PXDataStreamWriteSU(outputStream, sizeof(unsigned char), EndianLittle);
                            PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerBool);
                            PXDataStreamWriteCU(outputStream, compilerSymbolEntry.DataC);
                            break;
                        }

                        case PXCompilerSymbolLexerInteger:
                        {
                            PXDataStreamWriteSU(outputStream, sizeof(unsigned int), EndianLittle);
                            PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerInteger);
                            PXDataStreamWriteIU(outputStream, compilerSymbolEntry.DataI, EndianLittle);
                            break;
                        }

                        case PXCompilerSymbolLexerFloat:
                        {
                            PXDataStreamWriteSU(outputStream, sizeof(float), EndianLittle);
                            PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerFloat);
                            PXDataStreamWriteF(outputStream, compilerSymbolEntry.DataF);
                            break;
                        }
                        case PXCompilerSymbolLexerGenericElement:
                        case PXCompilerSymbolLexerString:
                        {
                            PXDataStreamWriteSU(outputStream, compilerSymbolEntry.Size, EndianLittle);
                            PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerString);
                            PXDataStreamWriteP(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
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

            MemorySet(textA, 256, 0);
            MemorySet(textB, 256, 0);
            MemorySet(emotySpace, 25, 0);

            PXDataStreamReadI16U(outputStream, &textASize, EndianLittle);
            PXDataStreamReadB(outputStream, textA, textASize);
            PXDataStreamReadI16U(outputStream, &textBSize, EndianLittle);

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

                        PXDataStreamReadI32U(outputStream, &x, EndianLittle);
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
