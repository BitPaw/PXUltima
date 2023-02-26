#include "XML.h"

#include <Compiler/PXCompiler.h>
#include <Text/PXText.h>

XMLSymbol XMLPeekLine(const char* const text, const PXSize textSize)
{
    const PXBool isOpenTag = text[0] == '<';
    const PXBool isSlashTag = text[1] == '/';
    const PXBool isCloseOpenTag = text[textSize-1] == '>';
    const PXBool isAttributeTagWithEnd = text[textSize-2u] == '\"';
    const PXBool isAttributeTagMiddle= text[textSize - 1] == '\"';

    PXBool isAttributeCalc = 0;

    PXSize indexEqual = PXTextFindFirstA(text, textSize, '=');

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



PXActionResult XMLFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXSize errorCounter = 0;
    PXDataStream tokenSteam;

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

        PXDataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    PXCompilerSymbolEntry compilerSymbolEntry;
    unsigned char depthCounter = 0;
    PXBool reuseModifiedToken = 0;

    MemoryClear(&compilerSymbolEntry, sizeof(PXCompilerSymbolEntry));

    while (!PXDataStreamIsAtEnd(&tokenSteam))
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
                        const PXSize offsetEqualSign = PXTextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '=');
                        const PXSize endOfValue = PXTextFindFirstA(compilerSymbolEntry.Source+ offsetEqualSign+2, compilerSymbolEntry.Size- offsetEqualSign-2, '\"');

                        const PXSize nameSize = offsetEqualSign;
                        const char* const nameAdress = compilerSymbolEntry.Source;
                        const PXSize valueSize = endOfValue;
                        const char* const valueAdrees = compilerSymbolEntry.Source + offsetEqualSign + 2u;

                        //-----------------------------------------------------
                        PXDataStreamWriteI8U(outputStream, depthCounter);
                        PXDataStreamWriteI8U(outputStream, xmlTag);
                        PXDataStreamWriteI16U(outputStream, nameSize);
                        PXDataStreamWriteB(outputStream, nameAdress, nameSize);
                        PXDataStreamWriteI16U(outputStream, valueSize);
                        PXDataStreamWriteB(outputStream, valueAdrees, valueSize);
                        //-----------------------------------------------------

                        const char* startAfterValue = valueAdrees + valueSize;
                        const PXSize startAfterValueSize = compilerSymbolEntry.Size - (nameSize + valueSize + 3u);

                        const PXSize indexOfCloseTagSymbol = PXTextFindFirstA(startAfterValue, startAfterValueSize, '/');
                        const PXBool hasCloseTag = indexOfCloseTagSymbol != (PXSize)-1;

                        if (hasCloseTag)
                        {
                            const PXSize curSize = startAfterValueSize - indexOfCloseTagSymbol;
                            const PXSize indexOfCloseTagSymbolEE = PXTextFindFirstA(startAfterValue, curSize, '>');
                            const PXBool hasCloseTagEEE = indexOfCloseTagSymbolEE != (PXSize)-1;

                            if (hasCloseTagEEE)
                            {
                                if (indexOfCloseTagSymbolEE < compilerSymbolEntry.Size) // Has even more data
                                {
                                    const PXSize offset = (nameSize + valueSize + 3u);

                                    reuseModifiedToken = 1;

                                    compilerSymbolEntry.Source += offset + 1u;
                                    compilerSymbolEntry.Size -= offset + 1u;
                                }
                            }
                        }

                        break;
                    }
                    case XMLSymbolTagOpenBegin:
                    {
                        const PXSize indexOfCloseTagSymbol = PXTextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '>');
                        const PXBool doesContainMoreData = indexOfCloseTagSymbol != (PXSize)-1;
                        const PXSize cutIndex = doesContainMoreData ? indexOfCloseTagSymbol - 1u : compilerSymbolEntry.Size - 1;

                        // Data
                        PXDataStreamWriteI8U(outputStream, depthCounter);
                        PXDataStreamWriteI8U(outputStream, xmlTag);
                        PXDataStreamWriteI16U(outputStream, cutIndex);
                        PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source + 1u, cutIndex);
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
                        PXDataStreamWriteI8U(outputStream, depthCounter);
                        PXDataStreamWriteI8U(outputStream, xmlTag);
                        PXDataStreamWriteI16U(outputStream, openTagNameSize);
                        PXDataStreamWriteB(outputStream, openTagName, openTagNameSize);
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

                        PXDataStreamWriteI8U(outputStream, depthCounter);
                        PXDataStreamWriteI8U(outputStream, xmlTag);
                        PXDataStreamWriteI16U(outputStream, closeTagNameSize);
                        PXDataStreamWriteB(outputStream, closeTagName, closeTagNameSize);
                        //-----------------------------------------------------

                        break;
                    }

                    case XMLSymbolUnkown:
                    {
                        PXDataStreamWriteI8U(outputStream, depthCounter);
                        PXDataStreamWriteI8U(outputStream, XMLSymbolRawData);

                        PXSize sizePositionOffset = outputStream->DataCursor;
                        PXSize sizeWritten = 0;

                        PXDataStreamWriteI16U(outputStream, 0xFFFF);

                        do
                        {
                            PXSize indexOfOpenTagSymbol = PXTextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '<');
                            const PXBool found = indexOfOpenTagSymbol != (PXSize)-1;

                            if (found)
                            {
                                sizeWritten += PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, indexOfOpenTagSymbol);

                                reuseModifiedToken = 1;

                                compilerSymbolEntry.Source += indexOfOpenTagSymbol;
                                compilerSymbolEntry.Size -= indexOfOpenTagSymbol;

                                break;
                            }
                            else
                            {
                                PXSize indexOfCloseInlineTagSymbol = PXTextFindFirstA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '/');
                                const PXBool found = indexOfCloseInlineTagSymbol > 0 && indexOfCloseInlineTagSymbol != (PXSize)-1;

                                if (found)
                                {
                                    --depthCounter;

                                    PXDataStreamWriteI8U(outputStream, depthCounter);
                                    PXDataStreamWriteI8U(outputStream, XMLSymbolTagCloseCurrent);
                                    PXDataStreamWriteI16U(outputStream, 0);
                                }
                                else
                                {
                                    sizeWritten += PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                }
                            }
                        }
                        while (1u);

                        PXDataStreamWriteAtI16U(outputStream, sizeWritten, sizePositionOffset);

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

    while (!PXDataStreamIsAtEnd(outputStream))
    {
        unsigned char depth = 0;
        XMLSymbol mode = 0;

        PXTextClearA(textBuffer, 512);
        PXDataStreamReadI8U(outputStream, &depth);
        PXDataStreamReadI8U(outputStream, &mode);

        for (PXSize i = 0; i < (PXSize)depth+1; ++i)
        {
            printf("  ");
        }

        switch (mode)
        {
            case XMLSymbolTagOpenBegin:
            {
                unsigned short size = 0;

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, textBuffer, size);

                printf("<%s>", textBuffer);
                break;
            }
            case XMLSymbolTagOpenFull:
            {
                unsigned short size = 0;

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, textBuffer, size);

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

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, textBuffer, size);

                printf("</%s>", textBuffer);
                break;
            }
            case XMLSymbolAttribute:
            {
                unsigned short size = 0;

                char text[256];
                PXTextClearA(text, 256);

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, textBuffer, size);

                printf("A: %s:", textBuffer);
                fflush(stdout);

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, text , size);

                printf("%s", text);

                break;
            }
            case XMLSymbolRawData:
            {
                unsigned short size = 0;

                PXDataStreamReadI16U(outputStream, &size);
                PXDataStreamReadB(outputStream, textBuffer, size);

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


    return PXActionSuccessful;
}
