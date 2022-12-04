#include "XML.h"

#include <Compiler/PXCompiler.h>
#include <Text/Text.h>

XMLSymbol XMLPeekLine(const char* const text, const PXSize textSize)
{
    const PXBool isOpenTag = text[0] == '<';
    const PXBool isSlashTag = text[1] == '/';
    const PXBool isCloseOpenTag = text[textSize-1] == '>';
    const PXBool isAttributeTagWithEnd = text[textSize-2u] == '\"';
    const PXBool isAttributeTagMiddle= text[textSize - 1] == '\"';

    PXBool isAttributeCalc = 0;

    PXSize indexEqual = TextFindFirstA(text, textSize, '=');

    if (indexEqual != (PXSize)-1)
    {
        isAttributeCalc = text[indexEqual + 1] == '\"';
    }

    const PXBool isAttributeTagEnd = 
        ((text[textSize - 3] == '\"') && (text[textSize -2] == '/') && (text[textSize - 1] == '>')) || // .."/>
        ((text[textSize - 2] == '\"') && (text[textSize - 1] == '>')) ||
        isAttributeCalc; // ..">

    const char result =
        (isOpenTag && !(isSlashTag || isCloseOpenTag)) * 'A' + // IsOpenTagBegin
        (isOpenTag && !isSlashTag && isCloseOpenTag) * 'B' +// IsOpenTagFull
        (!isOpenTag && isSlashTag && isCloseOpenTag) * 'C' +// IsCloseTagInline
        (isOpenTag && isSlashTag && isCloseOpenTag) * 'D' + // IsCloseTagFull
        ((!(isOpenTag || isSlashTag) && (isAttributeTagWithEnd || isAttributeTagMiddle)) || isAttributeTagEnd) * 'F';

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
    PXSize errorCounter = 0;
    DataStream tokenSteam;

    // Lexer - Level I 
    {
        PXCompilerSettings compilerSettings;
        compilerSettings.KeepWhiteSpace = 1u;
        compilerSettings.KeepWhiteSpaceIndentationLeft = 1u;
        compilerSettings.TryAnalyseTypes = 0u;
        compilerSettings.IntrepredNewLineAsWhiteSpace = 1u;
        compilerSettings.KeepTabs = 1u;
        compilerSettings.IntrepredTabsAsWhiteSpace = 1u;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        DataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    PXCompilerSymbolEntry compilerSymbolEntry;
    unsigned char depthCounter = 0;
    PXBool reuseModifiedToken = 0;   

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
                        const PXSize offsetEqualSign = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '=');                                        
                        const PXSize endOfValue = TextFindFirstA(compilerSymbolEntry.Source+ offsetEqualSign+2, compilerSymbolEntry.Size- offsetEqualSign-2, '\"');

                        const PXSize nameSize = offsetEqualSign;
                        const char* const nameAdress = compilerSymbolEntry.Source;
                        const PXSize valueSize = endOfValue;
                        const char* const valueAdrees = compilerSymbolEntry.Source + offsetEqualSign + 2u;
                        
                        //-----------------------------------------------------
                        DataStreamWriteCU(outputStream, depthCounter);
                        DataStreamWriteCU(outputStream, xmlTag);
                        DataStreamWriteSU(outputStream, nameSize, EndianLittle);
                        DataStreamWriteP(outputStream, nameAdress, nameSize);
                        DataStreamWriteSU(outputStream, valueSize, EndianLittle);
                        DataStreamWriteP(outputStream, valueAdrees, valueSize);
                        //-----------------------------------------------------

                        const char* startAfterValue = valueAdrees + valueSize;
                        const PXSize startAfterValueSize = compilerSymbolEntry.Size - (nameSize + valueSize + 3u);

                        const PXSize indexOfCloseTagSymbol = TextFindFirstA(startAfterValue, startAfterValueSize, '/');
                        const PXBool hasCloseTag = indexOfCloseTagSymbol != (PXSize)-1;

                        if (hasCloseTag)
                        {
                            const PXSize curSize = startAfterValueSize - indexOfCloseTagSymbol;
                            const PXSize indexOfCloseTagSymbolEE = TextFindFirstA(startAfterValue, curSize, '>');
                            const PXBool hasCloseTagEEE = indexOfCloseTagSymbolEE != (PXSize)-1;

                            if (hasCloseTagEEE)
                            {       
                                if (indexOfCloseTagSymbolEE < compilerSymbolEntry.Size) // Has even more data
                                {
                                    const PXSize offset = (nameSize + valueSize + 3u);

                                    reuseModifiedToken = 1;

                                    compilerSymbolEntry.Source += offset + 1;
                                    compilerSymbolEntry.Size -= offset + 1;
                                }
                            }                            
                        }

                        break;
                    }
                    case XMLSymbolTagOpenBegin:
                    {         
                        const PXSize indexOfCloseTagSymbol = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '>');
                        const PXBool doesContainMoreData = indexOfCloseTagSymbol != (PXSize)-1;
                        const PXSize cutIndex = doesContainMoreData ? indexOfCloseTagSymbol - 1u : compilerSymbolEntry.Size - 1;

                        // Data
                        DataStreamWriteCU(outputStream, depthCounter);
                        DataStreamWriteCU(outputStream, xmlTag);
                        DataStreamWriteSU(outputStream, cutIndex, EndianLittle);
                        DataStreamWriteP(outputStream, compilerSymbolEntry.Source + 1u, cutIndex);
                        //-----------------------------------------------------

                        ++depthCounter;

                        if (doesContainMoreData)
                        {
                            reuseModifiedToken = 1;
                                                        
                            compilerSymbolEntry.Source += indexOfCloseTagSymbol+1;
                            compilerSymbolEntry.Size -= indexOfCloseTagSymbol+1;
                        } 
                   
                        break;
                    }
                    case XMLSymbolTagOpenFull:
                    {
                        const char* openTagName = compilerSymbolEntry.Source + 1u;
                        const PXSize openTagNameSize = compilerSymbolEntry.Size - 2u;    

                        ++depthCounter;

                        //-----------------------------------------------------
                        DataStreamWriteCU(outputStream, depthCounter);
                        DataStreamWriteCU(outputStream, xmlTag);
                        DataStreamWriteSU(outputStream, openTagNameSize, EndianLittle);
                        DataStreamWriteP(outputStream, openTagName, openTagNameSize);
                        //-----------------------------------------------------

                        break;
                    }
                    case XMLSymbolTagCloseCurrent:
                    {          
                        --depthCounter;

                        printf("[XML][%i] Close current [%s]\n", depthCounter, "");                   

                        break;
                    }
                    case XMLSymbolTagCloseFull:
                    {
                        --depthCounter;

                        //-----------------------------------------------------
                        const char* closeTagName = compilerSymbolEntry.Source + 2u;
                        const PXSize closeTagNameSize = compilerSymbolEntry.Size - 3u;

                        DataStreamWriteCU(outputStream, depthCounter);
                        DataStreamWriteCU(outputStream, xmlTag);
                        DataStreamWriteSU(outputStream, closeTagNameSize, EndianLittle);
                        DataStreamWriteP(outputStream, closeTagName, closeTagNameSize);
                        //-----------------------------------------------------                 

                        break;
                    }

                    case XMLSymbolUnkown:
                    {
                        DataStreamWriteCU(outputStream, depthCounter);
                        DataStreamWriteCU(outputStream, XMLSymbolRawData);

                        PXSize sizePositionOffset = outputStream->DataCursor;
                        PXSize sizeWritten = 0;

                        DataStreamWriteSU(outputStream, 0xFFFF, EndianLittle);

                        do
                        {
                            PXSize indexOfOpenTagSymbol = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '<');
                            const PXBool found = indexOfOpenTagSymbol != (PXSize)-1;

                            if (found)
                            {
                                sizeWritten += DataStreamWriteP(outputStream, compilerSymbolEntry.Source, indexOfOpenTagSymbol);

                                reuseModifiedToken = 1;

                                compilerSymbolEntry.Source += indexOfOpenTagSymbol;
                                compilerSymbolEntry.Size -= indexOfOpenTagSymbol;

                                break;
                            }
                            else
                            {
                                PXSize indexOfCloseInlineTagSymbol = TextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '/');
                                const PXBool found = indexOfCloseInlineTagSymbol > 0 && indexOfCloseInlineTagSymbol != (PXSize)-1;

                                if (found)
                                {
                                    --depthCounter;

                                    DataStreamWriteCU(outputStream, depthCounter);
                                    DataStreamWriteCU(outputStream, XMLSymbolTagCloseCurrent);
                                    DataStreamWriteSU(outputStream, 0, EndianLittle);
                                }
                                else
                                {
                                    sizeWritten += DataStreamWriteP(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                }
                            }
                        }
                        while (1u);         

                        DataStreamWriteAtSU(outputStream, sizeWritten, EndianLittle, sizePositionOffset);

                        break;
                    }
                }       
            }
        }
    }

    outputStream->DataSize = outputStream->DataCursor;


    PXSize currentPosition = outputStream->DataCursor;

    outputStream->DataCursor = 0;

    char textBuffer[512];   

    printf("[XML]\n");

    while (!DataStreamIsAtEnd(outputStream))
    {
        unsigned char depth = 0;
        XMLSymbol mode = 0;

        TextClearA(textBuffer, 512);
        DataStreamReadCU(outputStream, &depth);
        DataStreamReadCU(outputStream, &mode);

        for (PXSize i = 0; i < depth+1; ++i)
        {
            printf("  ");
        }

        switch (mode)
        {
            case XMLSymbolTagOpenBegin:
            { 
                unsigned short size = 0;

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, textBuffer, size);

                printf("<%s>", textBuffer);
                break;
            }
            case XMLSymbolTagOpenFull:
            {
                unsigned short size = 0;

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, textBuffer, size);

                printf("<%s>", textBuffer);

                break;
            }
            case XMLSymbolTagCloseCurrent:
            {
                printf("</>");

                break;
            }
            case XMLSymbolTagCloseFull:
            {
                unsigned short size = 0;

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, textBuffer, size);

                printf("</%s>", textBuffer);
                break;
            }
            case XMLSymbolAttribute:
            {
                unsigned short size = 0;

                char text[256];
                TextClearA(text, 256);

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, textBuffer, size);

                printf("A: %s:", textBuffer);
                fflush(stdout);

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, text , size);

                printf("%s", text);

                break;
            }
            case XMLSymbolRawData:
            {
                unsigned short size = 0;

                DataStreamReadSU(outputStream, &size, EndianLittle);
                DataStreamReadP(outputStream, textBuffer, size);

                for (PXSize i = 0; i < size; i++)
                {
                    textBuffer[i] = textBuffer[i] == '\n' ? ' ' : textBuffer[i];
                }

                printf("D: %s", textBuffer);
                break;
            }
        }

        printf("\n");
    }


    return ActionSuccessful;
}
