#include "XML.h"

#include <Compiler/PXCompiler.h>
#include <Media/PXText.h>

XMLSymbol XMLPeekLine(const char* const text, const PXSize textSize)
{
    const PXBool isOpenTag = text[0] == '<';
    const PXBool isSlashTag = text[1] == '/';
    const PXBool isCloseOpenTag = text[textSize-1] == '>';
    const PXBool isAttributeTagWithEnd = text[textSize-2u] == '\"';
    const PXBool isAttributeTagMiddle= text[textSize - 1] == '\"';

    PXBool isAttributeCalc = 0;

    PXSize indexEqual = PXTextFindFirstCharacterA(text, textSize, '=');

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



PXActionResult XMLFileCompile(PXFile* const inputStream, PXFile* const outputStream)
{
    PXSize errorCounter = 0;
    PXFile tokenSteam;

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.KeepWhiteSpace = PXYes;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXYes;
        compilerSettings.IntrepredNewLineAsWhiteSpace = PXYes;
        compilerSettings.KeepTabs = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens
            
        PXFileBufferExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    unsigned char depthCounter = 0;
    PXBool reuseModifiedToken = 0;

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

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
                        const PXSize offsetEqualSign = PXTextFindFirstCharacterA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '=');
                        const PXSize endOfValue = PXTextFindFirstCharacterA(compilerSymbolEntry.Source+ offsetEqualSign+2, compilerSymbolEntry.Size- offsetEqualSign-2, '\"');

                        const PXSize nameSize = offsetEqualSign;
                        const char* const nameAdress = compilerSymbolEntry.Source;
                        const PXSize valueSize = endOfValue;
                        const char* const valueAdrees = compilerSymbolEntry.Source + offsetEqualSign + 2u;

                        //-----------------------------------------------------
                        PXFileWriteI8U(outputStream, depthCounter);
                        PXFileWriteI8U(outputStream, xmlTag);
                        PXFileWriteI16U(outputStream, nameSize);
                        PXFileWriteB(outputStream, nameAdress, nameSize);
                        PXFileWriteI16U(outputStream, valueSize);
                        PXFileWriteB(outputStream, valueAdrees, valueSize);
                        //-----------------------------------------------------

                        const char* startAfterValue = valueAdrees + valueSize;
                        const PXSize startAfterValueSize = compilerSymbolEntry.Size - (nameSize + valueSize + 3u);

                        const PXSize indexOfCloseTagSymbol = PXTextFindFirstCharacterA(startAfterValue, startAfterValueSize, '/');
                        const PXBool hasCloseTag = indexOfCloseTagSymbol != (PXSize)-1;

                        if (hasCloseTag)
                        {
                            const PXSize curSize = startAfterValueSize - indexOfCloseTagSymbol;
                            const PXSize indexOfCloseTagSymbolEE = PXTextFindFirstCharacterA(startAfterValue, curSize, '>');
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
                        const PXSize indexOfCloseTagSymbol = PXTextFindFirstCharacterA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '>');
                        const PXBool doesContainMoreData = indexOfCloseTagSymbol != (PXSize)-1;
                        const PXSize cutIndex = doesContainMoreData ? indexOfCloseTagSymbol - 1u : compilerSymbolEntry.Size - 1;

                        // Data
                        PXFileWriteI8U(outputStream, depthCounter);
                        PXFileWriteI8U(outputStream, xmlTag);
                        PXFileWriteI16U(outputStream, cutIndex);
                        PXFileWriteB(outputStream, compilerSymbolEntry.Source + 1u, cutIndex);
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
                        PXFileWriteI8U(outputStream, depthCounter);
                        PXFileWriteI8U(outputStream, xmlTag);
                        PXFileWriteI16U(outputStream, openTagNameSize);
                        PXFileWriteB(outputStream, openTagName, openTagNameSize);
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

                        PXFileWriteI8U(outputStream, depthCounter);
                        PXFileWriteI8U(outputStream, xmlTag);
                        PXFileWriteI16U(outputStream, closeTagNameSize);
                        PXFileWriteB(outputStream, closeTagName, closeTagNameSize);
                        //-----------------------------------------------------

                        break;
                    }

                    case XMLSymbolUnkown:
                    {
                        PXFileWriteI8U(outputStream, depthCounter);
                        PXFileWriteI8U(outputStream, XMLSymbolRawData);

                        PXSize sizePositionOffset = outputStream->DataCursor;
                        PXSize sizeWritten = 0;

                        PXFileWriteI16U(outputStream, 0xFFFF);

                        do
                        {
                            PXSize indexOfOpenTagSymbol = PXTextFindFirstCharacterA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '<');
                            const PXBool found = indexOfOpenTagSymbol != (PXSize)-1;

                            if (found)
                            {
                                sizeWritten += PXFileWriteB(outputStream, compilerSymbolEntry.Source, indexOfOpenTagSymbol);

                                reuseModifiedToken = 1;

                                compilerSymbolEntry.Source += indexOfOpenTagSymbol;
                                compilerSymbolEntry.Size -= indexOfOpenTagSymbol;

                                break;
                            }
                            else
                            {
                                PXSize indexOfCloseInlineTagSymbol = PXTextFindFirstCharacterA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, '/');
                                const PXBool found = indexOfCloseInlineTagSymbol > 0 && indexOfCloseInlineTagSymbol != (PXSize)-1;

                                if (found)
                                {
                                    --depthCounter;

                                    PXFileWriteI8U(outputStream, depthCounter);
                                    PXFileWriteI8U(outputStream, XMLSymbolTagCloseCurrent);
                                    PXFileWriteI16U(outputStream, 0);
                                }
                                else
                                {
                                    sizeWritten += PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                }
                            }
                        }
                        while (1u);

                        PXFileWriteAtI16U(outputStream, sizeWritten, sizePositionOffset);

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

    while (!PXFileIsAtEnd(outputStream))
    {
        unsigned char depth = 0;
        XMLSymbol mode = 0;

        PXTextClearA(textBuffer, 512);
        PXFileReadI8U(outputStream, &depth);
        PXFileReadI8U(outputStream, &mode);

        for (PXSize i = 0; i < (PXSize)depth+1; ++i)
        {
            printf("  ");
        }

        switch (mode)
        {
            case XMLSymbolTagOpenBegin:
            {
                unsigned short size = 0;

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, textBuffer, size);

                printf("<%s>", textBuffer);
                break;
            }
            case XMLSymbolTagOpenFull:
            {
                unsigned short size = 0;

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, textBuffer, size);

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

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, textBuffer, size);

                printf("</%s>", textBuffer);
                break;
            }
            case XMLSymbolAttribute:
            {
                unsigned short size = 0;

                char text[256];
                PXTextClearA(text, 256);

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, textBuffer, size);

                printf("A: %s:", textBuffer);
                fflush(stdout);

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, text , size);

                printf("%s", text);

                break;
            }
            case XMLSymbolRawData:
            {
                unsigned short size = 0;

                PXFileReadI16U(outputStream, &size);
                PXFileReadB(outputStream, textBuffer, size);

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
