#include "YAML.h"

#include <Compiler\Compiler.h>
#include <Container/ClusterValue.h>

YAMLLineType YAMLPeekLine(const void* line, const size_t size)
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

ActionResult YAMLFileCompile(DataStream* const inputStream, DataStream* const outputStream)
{
    size_t errorCounter = 0;
    DataStream tokenSteam;

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

        DataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    unsigned int indentCounter = 0;

    while (!DataStreamIsAtEnd(&tokenSteam))
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

                        DataStreamWriteCU(outputStream, YAMLLineTypeKeyValueDeclare);
                        DataStreamWriteCU(outputStream, indentCounter);
                        DataStreamWriteSU(outputStream, declSize, EndianLittle);
                        DataStreamWriteP(outputStream, declname, declSize);


                        // Fetch next value
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // consume "xxxxx: "
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // sonsume empty space or number


                        switch (compilerSymbolEntry.ID)
                        {

                        case PXCompilerSymbolLexerNewLine:
                        case PXCompilerSymbolLexerWhiteSpace:
                            DataStreamWriteSU(outputStream, 0, EndianLittle);
                            indentCounter = compilerSymbolEntry.Size;
                            break;

                        case PXCompilerSymbolLexerBool:
                        {
                            DataStreamWriteSU(outputStream, sizeof(unsigned char), EndianLittle);
                            DataStreamWriteCU(outputStream, PXCompilerSymbolLexerBool);
                            DataStreamWriteCU(outputStream, compilerSymbolEntry.DataC);
                            break;
                        }

                        case PXCompilerSymbolLexerInteger:
                        {
                            DataStreamWriteSU(outputStream, sizeof(unsigned int), EndianLittle);
                            DataStreamWriteCU(outputStream, PXCompilerSymbolLexerInteger);
                            DataStreamWriteIU(outputStream, compilerSymbolEntry.DataI, EndianLittle);
                            break;
                        }

                        case PXCompilerSymbolLexerFloat:
                        {
                            DataStreamWriteSU(outputStream, sizeof(float), EndianLittle);
                            DataStreamWriteCU(outputStream, PXCompilerSymbolLexerFloat);
                            DataStreamWriteF(outputStream, compilerSymbolEntry.DataF);
                            break;
                        }
                        case PXCompilerSymbolLexerGenericElement:
                        case PXCompilerSymbolLexerString:
                        {
                            DataStreamWriteSU(outputStream, compilerSymbolEntry.Size, EndianLittle);
                            DataStreamWriteCU(outputStream, PXCompilerSymbolLexerString);
                            DataStreamWriteP(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
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

    size_t oldpos = outputStream->DataCursor;

    outputStream->DataCursor = 0;

    while (!DataStreamIsAtEnd(outputStream))
    {
        unsigned char depth = 0;
        YAMLLineType lineType = 0;

        DataStreamReadCU(outputStream, &lineType);
        DataStreamReadCU(outputStream, &depth);

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

            DataStreamReadSU(outputStream, &textASize, EndianLittle);
            DataStreamReadP(outputStream, textA, textASize);
            DataStreamReadSU(outputStream, &textBSize, EndianLittle);

            for (size_t i = 0; i < depth; i++)
            {
                emotySpace[i] = ' ';
            }

            if (textBSize > 0)
            {
                PXCompilerSymbolLexer lexer;
                unsigned char lx = 0;

                DataStreamReadCU(outputStream, &lx);

                lexer = lx;

                switch (lexer)
                {
                    case PXCompilerSymbolLexerBool:
                    {
                        unsigned char x = 0;

                        DataStreamReadCU(outputStream, &x, EndianLittle);
                        sprintf(textB, "%x", x);
                        break;
                    }

                    case PXCompilerSymbolLexerInteger:
                    {
                        unsigned int x = 0;

                        DataStreamReadIU(outputStream, &x, EndianLittle);
                        sprintf(textB, "%i", x);

                        break;
                    }

                    case PXCompilerSymbolLexerFloat:
                    {
                        float x = 0;

                        DataStreamReadF(outputStream, &x);

                        sprintf(textB, "%f", x);

                        break;
                    }

                    case PXCompilerSymbolLexerString:
                    {
                        DataStreamReadP(outputStream, textB, textBSize);
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

ActionResult YAMLFileParse(DataStream* const ymlTokenInputStream, SerializationTypeInfo* const serializationTypeInfoList, const size_t serializationTypeInfoListSize)
{
    

    return ActionSuccessful;
}
