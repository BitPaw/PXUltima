#include "YAML.h"

#include <Compiler\Compiler.h>
#include <Container/ClusterValue.h>

YAMLLineType YAMLPeekLine(const void* line, const size_t size)
{
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
        compilerSettings.KeepWhiteSpace = 0u;
        compilerSettings.KeepWhiteSpaceIndentationLeft = 1u;
        compilerSettings.TryAnalyseTypes = 1u;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        DataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);
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

            case PXCompilerSymbolLexerSingleCharacter:
            case PXCompilerSymbolLexerGenericElement:
            {
                const YAMLLineType lineType = YAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (lineType)
                {
                    case YAMLLineTypeComment:
                    {
                        do // We are at a comment. Skip everything until "end of line"
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                        } 
                        while (!DataStreamIsAtEnd(&tokenSteam) && (compilerSymbolEntry.ID != PXCompilerSymbolLexerNewLine));

                        break;
                    }
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
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        if (compilerSymbolEntry.ID == PXCompilerSymbolLexerNewLine)
                        {
                            continue; // Finished
                        }

                       // printf();

                        break;
                    }


                    default:
                        break;
                }

                break;
            }
            default:
                // Error
                break;
        }
    }


    // Comment
    // # Text

    // List 
    // - Text


    // [milk, pumpkin pie, eggs, juice]
}
