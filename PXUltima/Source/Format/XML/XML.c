#include "XML.h"

#include <Compiler/Compiler.h>
#include <Text/Text.h>

XMLSymbol XMLPeekLine(const char* const text, const size_t textSize)
{
    const PXBool isOpenTag = text[0] == '<';
    const PXBool isSlashTag = text[1] == '/';
    const PXBool isCloseOpenTag = text[textSize-1] == '>';
    const PXBool isAttributeTagWithEnd = text[textSize-2u] == '\"';
    const PXBool isAttributeTagMiddle= text[textSize - 1] == '\"';

    const char result =
        (isOpenTag && !(isSlashTag || isCloseOpenTag)) * 'A' + // IsOpenTagBegin
        (isOpenTag && !isSlashTag && isCloseOpenTag) * 'B' +// IsOpenTagFull
        (!isOpenTag && isSlashTag && isCloseOpenTag) * 'C' +// IsCloseTagInline
        (isOpenTag && isSlashTag && isCloseOpenTag) * 'D' + // IsCloseTagFull
        (!(isOpenTag || isSlashTag) && (isAttributeTagWithEnd || isAttributeTagMiddle)) * 'F';

    switch (result)
    {
        case 'A': return XMLSymbolTagOpenBegin;
        case 'B': return XMLSymbolTagOpenFull;
        case 'C': return XMLSymbolTagCloseCurrent;
        case 'D': return XMLSymbolTagCloseFull;
        case 'F': return XMLSymbolAttribute;
    }

    return XMLSymbolUnkown;
}

ActionResult XMLFileCompile(DataStream* const inputStream, DataStream* const outputStream)
{
    size_t errorCounter = 0;
    DataStream tokenSteam;

    // Lexer - Level I 
    {
        PXCompilerSettings compilerSettings;
        compilerSettings.KeepWhiteSpace = 1u;
        compilerSettings.KeepWhiteSpaceIndentationLeft = 1u;
        compilerSettings.TryAnalyseTypes = 0u;
        compilerSettings.IntrepredNewLineAsWhiteSpace = 1u;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        DataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    unsigned int depthCounter = 0;
    PXBool reuseModifiedToken = 0;
    char tagName[256];
    PXCompilerSymbolEntry compilerSymbolEntry;

    while (!DataStreamIsAtEnd(&tokenSteam))
    {
        if (!reuseModifiedToken)
        {
            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
        }   
        else
        {
            // DO nothging
            reuseModifiedToken = 0;
        }

        switch (compilerSymbolEntry.ID)
        {
            case PXCompilerSymbolLexerGenericElement:
            {
                const XMLSymbol xmlTag = XMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (xmlTag)
                {
                    case XMLSymbolAttribute:
                    {
                        const size_t offsetEqualSign = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '=');                                        

                        const size_t nameSize = compilerSymbolEntry.Size - offsetEqualSign - 1u;
                        const char* const nameAdress = compilerSymbolEntry.Source;
                        const size_t valueSize = compilerSymbolEntry.Size  - (offsetEqualSign + 3u);
                        const char* const valueAdrees = compilerSymbolEntry.Source + offsetEqualSign + 1u;

                        /*
                        DataStreamWriteCU(outputStream, 'A');
                        DataStreamWriteSU(outputStream, nameSize, EndianLittle);
                        DataStreamWriteP(outputStream, nameAdress, nameSize);
                        DataStreamWriteSU(outputStream, nameSize, EndianLittle);
                        DataStreamWriteP(outputStream, valueAdrees, valueSize);*/



                        char vvalue[256];
            
                        TextCopyA(nameAdress, nameSize, tagName, 256);

                        TextCopyA(valueAdrees, valueSize, vvalue, 256);

                        printf("[XML][%i] Element attribute [%s:%s]\n", depthCounter, tagName, vvalue);

                        break;
                    }
                    case XMLSymbolTagOpenBegin:
                    {         
                        const size_t indexOfCloseTagSymbol = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '>');
                        const PXBool doesContainMoreData = indexOfCloseTagSymbol != (size_t)-1;
                        const size_t cutIndex = doesContainMoreData ? indexOfCloseTagSymbol - 2 : compilerSymbolEntry.Size - 1;

                        ++depthCounter;

                        TextCopyA(compilerSymbolEntry.Source + 1u, cutIndex, tagName, 256);

                        printf("[XML][%i] Element begin [%s]\n", depthCounter, tagName);                    

                        if (doesContainMoreData)
                        {
                            reuseModifiedToken = 1;
                                                        
                            compilerSymbolEntry.Source += indexOfCloseTagSymbol;
                            compilerSymbolEntry.Size -= indexOfCloseTagSymbol;
                        }                

                   
                        break;
                    }
                    case XMLSymbolTagOpenFull:
                    {
                        TextCopyA(compilerSymbolEntry.Source + 1u, compilerSymbolEntry.Size-2u, tagName, 256);
                        
                        ++depthCounter;

                        printf("[XML] Open Full [%s]\n", tagName);
                        break;
                    }
                    case XMLSymbolTagCloseCurrent:
                    {                  
                        printf("[XML][%i] Close current [%s]\n", depthCounter, "");

                        --depthCounter;

                        break;
                    }
                    case XMLSymbolTagCloseFull:
                    {
                        char tagName[256];

                        TextCopyA(compilerSymbolEntry.Source + 2u, compilerSymbolEntry.Size-3u, tagName, 256);
                                           

                        printf("[XML][%i] Element close [%s]\n", depthCounter, tagName);

                        --depthCounter;
                        break;
                    }

                    case XMLSymbolUnkown:
                    {
                        printf("[XML][%i] **Raw info** [", depthCounter);

                        do
                        {
                            size_t indexOfOpenTagSymbol = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '<');
                            const PXBool found = indexOfOpenTagSymbol > 0 && indexOfOpenTagSymbol != (size_t)-1;

                            if (found)
                            {
                                --indexOfOpenTagSymbol;

                                TextCopyA(compilerSymbolEntry.Source, indexOfOpenTagSymbol, tagName, 256);

                                printf("%s", tagName);

                                reuseModifiedToken = 1;

                                compilerSymbolEntry.Source += indexOfOpenTagSymbol;
                                compilerSymbolEntry.Size -= indexOfOpenTagSymbol;

                                break;
                            }
                            else
                            {
                                TextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, tagName, 256);

                                printf("%s", tagName);

                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                            }
                        }
                        while (1u);                     

                        printf("]\n");

                        break;
                    }
                }       
            }
        }
    }


    return ActionSuccessful;
}
