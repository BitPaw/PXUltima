#include "PXSpriteFont.h"

#include <stdio.h>

#include <PX/Media/PXText.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Media/PXImage.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Compiler/PXCompiler.h>

PXActionResult PXAPI PXSpriteFontLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXFont* const pxFont = (PXFont*)pxResourceLoadInfo->ResourceTarget;



    PXFile tokenStream;
    PXClear(PXFile, &tokenStream);

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.ReadInfo.FileCache = &tokenStream;
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
    pxCompiler.CommentSingleLineSize = 1u;
    pxCompiler.CommentSingleLine = "#";

    PXSpriteFont pxSpriteFontEE;
    PXSpriteFont* pxSpriteFont = &pxSpriteFontEE;

    PXClear(PXSpriteFont, pxSpriteFont);


    PXSize currentPageIndex = 0;
    PXSize currentCharacterIndex = 0;

    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens

    unsigned int indentCounter = 0;

    while (!PXFileIsAtEnd(&tokenStream))
    {
        PXCompilerSymbolEntryExtract(&pxCompiler);

        switch (pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
        case PXCompilerSymbolLexerGeneric:
        {
            PXSpriteFontLineType lineType = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

            switch (lineType)
            {
            case PXSpriteFontSymbolInfo:
            {
                const PXSize targetLine = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;

                while (targetLine == pxCompiler.ReadInfo.SymbolEntryCurrent.Line)
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                    // if OK

                    //---<Check for '='>----------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXBool isEqual = pxCompiler.ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerEqual;

                    if (!isEqual)
                    {
                        return PXActionInvalid;
                    }
                    //-----------------------------------

                    //---<Get Value>---------------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                    //-----------------------------------

                    switch (variableName)
                    {
                    case PXSpriteFontSymbolFontName:
                    {
                        PXTextCopyA(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size, pxSpriteFont->Info.Name, PXSpriteFontFontNameSize);
                        break;
                    }
                    case PXSpriteFontSymbolFontSize:
                    {
                        pxSpriteFont->Info.Size = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }
                    case PXSpriteFontSymbolBold:
                    {
                        pxSpriteFont->Info.Bold = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    case PXSpriteFontSymbolItalic:
                    {
                        pxSpriteFont->Info.Italic = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    case PXSpriteFontSymbolCharSet:
                    {
                        PXTextCopyA(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size, pxSpriteFont->Info.CharSet, PXSpriteFontFontNameSize);
                        break;
                    }

                    case PXSpriteFontSymbolUnicode:
                    {
                        pxSpriteFont->Info.Unicode = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    case PXSpriteFontSymbolStretchH:
                    {
                        pxSpriteFont->Info.StretchH = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }

                    case PXSpriteFontSymbolSmooth:
                    {
                        pxSpriteFont->Info.Smooth = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    case PXSpriteFontSymbolAntilising:
                    {
                        pxSpriteFont->Info.Supersampling = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    case PXSpriteFontSymbolPadding:
                    {
                        pxSpriteFont->Info.CharacterPadding[0] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;

                        PXCompilerSymbolEntryExtract(&pxCompiler);
                        PXCompilerSymbolEntryExtract(&pxCompiler);

                        pxSpriteFont->Info.CharacterPadding[1] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;

                        PXCompilerSymbolEntryExtract(&pxCompiler);
                        PXCompilerSymbolEntryExtract(&pxCompiler);

                        pxSpriteFont->Info.CharacterPadding[2] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;

                        PXCompilerSymbolEntryExtract(&pxCompiler);
                        PXCompilerSymbolEntryExtract(&pxCompiler);

                        pxSpriteFont->Info.CharacterPadding[3] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;

                    }
                    case PXSpriteFontSymbolSpacing:
                    {
                        pxSpriteFont->Info.SpacerOffset[0] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;

                        PXCompilerSymbolEntryExtract(&pxCompiler);
                        PXCompilerSymbolEntryExtract(&pxCompiler);

                        pxSpriteFont->Info.SpacerOffset[1] = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;

                        break;
                    }

                    default:
                        break;
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);
                }

                break;
            }
            case PXSpriteFontSymbolCommon:
            {
                const PXSize targetLine = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;

                while (targetLine == pxCompiler.ReadInfo.SymbolEntryCurrent.Line)
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                    // if OK

                    //---<Check for '='>----------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXBool isEqual = pxCompiler.ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerEqual;

                    if (!isEqual)
                    {
                        return PXActionInvalid;
                    }
                    //-----------------------------------

                    //---<Get Value>---------------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                    //-----------------------------------

                    switch (variableName)
                    {
                    case PXSpriteFontSymbolLineHeight:
                    {
                        pxSpriteFont->CommonData.LineHeight = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }
                    case PXSpriteFontSymbolBase:
                    {
                        pxSpriteFont->CommonData.Base = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }
                    case PXSpriteFontSymbolScaleWidth:
                    {
                        pxSpriteFont->CommonData.ScaleWidth = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }
                    case PXSpriteFontSymbolScaleHeight:
                    {
                        pxSpriteFont->CommonData.ScaleHeight = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                        break;
                    }
                    case PXSpriteFontSymbolPageList:
                    {
                        const PXInt32U amountOfPages = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;

                        if (amountOfPages > 1)
                        {
                            pxFont->PageListAmount = amountOfPages - 1;
                            pxFont->PageList = PXMemoryHeapCallocT(PXFontPage, pxFont->PageListAmount);
                        }

                        break;
                    }
                    case PXSpriteFontSymbolPacked:
                    {
                        pxSpriteFont->CommonData.Packed = pxCompiler.ReadInfo.SymbolEntryCurrent.CU;
                        break;
                    }

                    default:
                        break;
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);
                }

                break;
            }
            case PXSpriteFontSymbolPage:
            {
                PXFontPage* const pxFontPage = PXFontPageGet(pxFont, currentPageIndex);

                const PXSize targetLine = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;

                while (targetLine == pxCompiler.ReadInfo.SymbolEntryCurrent.Line)
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                    // if OK

                    //---<Check for '='>----------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXBool isEqual = pxCompiler.ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerEqual;

                    if (!isEqual)
                    {
                        return PXActionInvalid;
                    }
                    //-----------------------------------

                    //---<Get Value>---------------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                    //-----------------------------------

                    switch (variableName)
                    {
                    case PXSpriteFontSymbolID:
                    {


                        break;
                    }
                    case PXSpriteFontSymbolFilePath:
                    {
                        PXText fileName;
                        PXTextConstructFromAdressA(&fileName, pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                        // Loading Image
                        {
                            PXText fontFilePath;
                            PXTextConstructNamedBufferA(&fontFilePath, fontFilePathBuffer, PXPathSizeMax);
                            PXText resultFullPath;
                            PXTextConstructNamedBufferA(&resultFullPath, resultFullPathBuffer, PXPathSizeMax);

                            PXFilePathGet(pxResourceLoadInfo->FileReference, &fontFilePath);

                            PXFilePathSwapFileName(&fontFilePath, &resultFullPath, &fileName);

                            // Load
                            {
                                PXFontPage* pxFontPage = PXNull;

                                if(pxFont->PageListAmount == 0)
                                {
                                    pxFontPage = &pxFont->PagePrime;
                                }
                                else
                                {
                                    pxFontPage = &pxFont->PageList[currentPageIndex];
                                }

                                PXResourceCreateInfo pxResourceCreateInfoList;
                                PXClear(PXResourceCreateInfo, &pxResourceCreateInfoList);

                                pxResourceCreateInfoList.Type = PXResourceTypeTexture2D;
                                pxResourceCreateInfoList.ObjectReference = (void**)&pxFontPage->Texture;
                                pxResourceCreateInfoList.FilePath = resultFullPath.TextA;
                                pxResourceCreateInfoList.FilePathSize = resultFullPath.SizeUsed;

                                PXResourceManagerAdd(&pxResourceCreateInfoList);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);
                }

                break;
            }
            case PXSpriteFontSymbolCharacterList:
            {
                const PXSize targetLine = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;

                while (targetLine == pxCompiler.ReadInfo.SymbolEntryCurrent.Line)
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                    // if OK

                    //---<Check for '='>----------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXBool isEqual = pxCompiler.ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerEqual;

                    if (!isEqual)
                    {
                        return PXActionInvalid;
                    }
                    //-----------------------------------

                    //---<Get Value>---------------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                    //-----------------------------------

                    switch(variableName)
                    {
                        case PXSpriteFontSymbolCharacterAmount:
                        {
                            PXFontPage* const pxFontPage = PXFontPageGet(pxFont, currentPageIndex);

                            pxFontPage->CharacteListEntrys = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                            pxFontPage->CharacteList = PXMemoryHeapCallocT(PXFontPageCharacter, pxFontPage->CharacteListEntrys);
                            break;
                        }
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);
                }

                break;
            }
            case PXSpriteFontSymbolCharacterDefinition:
            {
                const PXSize targetLine = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;
                PXFontPage* const pxFontPage = PXFontPageGet(pxFont, currentPageIndex);

                // Guarantee size of list
                {
                    pxFontPage->CharacteList = PXMemoryHeapReallocT
                    (
                        PXFontPageCharacter,
                        pxFontPage->CharacteList,
                        pxFontPage->CharacteListEntrys+2
                    );
                }

                PXFontPageCharacter* const pxFontPageCharacter = &pxFontPage->CharacteList[currentCharacterIndex++];

                while (targetLine == pxCompiler.ReadInfo.SymbolEntryCurrent.Line)
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                    // if OK

                    //---<Check for '='>----------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);

                    const PXBool isEqual = pxCompiler.ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerEqual;

                    if (!isEqual)
                    {
                        return PXActionInvalid;
                    }
                    //-----------------------------------

                    //---<Get Value>---------------------
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                    //-----------------------------------

                    switch (variableName)
                    {
                    case PXSpriteFontSymbolID:
                    {
                        pxFontPageCharacter->ID = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolX:
                    {
                        pxFontPageCharacter->Position[0] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolY:
                    {
                        pxFontPageCharacter->Position[1] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolScaleWidth:
                    {
                        pxFontPageCharacter->Size[0] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolScaleHeight:
                    {
                        pxFontPageCharacter->Size[1] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolXOffset:
                    {
                        pxFontPageCharacter->Offset[0] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }
                    case PXSpriteFontSymbolYOffset:
                    {
                        pxFontPageCharacter->Offset[1] = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }

                    case PXSpriteFontSymbolXAdvance:
                    {
                        pxFontPageCharacter->XAdvance = pxCompiler.ReadInfo.SymbolEntryCurrent.I32S;
                        break;
                    }

                    case PXSpriteFontSymbolPage:
                    {
                        // pxFontPageCharacter->Page = compilerSymbolEntry.DataI32S;
                        break;
                    }

                    case PXSpriteFontSymbolChannel:
                    {
                        // pxFontPageCharacter->Chanal = compilerSymbolEntry.DataI32S;
                        break;
                    }

                    default:
                        break;
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);
                }
            }
            case PXSpriteFontLineUnkown:
            default:
                break;
            }

            break;
        }
        default:
        {
            break;
        }
        }
    }



#if 0
    PXSpriteFontPage* currentPage = 0;
    PXSize characterIndex = 0;

    PXSpriteFontConstruct(PXSpriteFont);

    while (!PXFileIsAtEnd(pxFile))
    {
        const char* currentPosition = (char*)PXFileCursorPosition(pxFile);
        const PXSize currentReadableBytes = PXFileRemainingSize(pxFile);
        const PXSpriteFontLineType lineType = PeekLineType(currentPosition, currentReadableBytes);

        switch (lineType)
        {
        case PXSpriteFontLineInfo:
        {
            const char parsingData[] = "face=\0size=\0bold=\0italic\0charset=\0unicode=\0stretchH=\0smooth=\0aa=\0padding=\0spacing=\0";
            const char* indexPosition[11];
            const ParsingTokenA parsingTokenList[] =
            {
                {parsingData + 0, &indexPosition[0]},
                {parsingData + 6, &indexPosition[1]},
                {parsingData + 12, &indexPosition[2]},
                {parsingData + 18, &indexPosition[3]},
                {parsingData + 25, &indexPosition[4]},
                {parsingData + 34, &indexPosition[5]},
                {parsingData + 43, &indexPosition[6]},
                {parsingData + 53, &indexPosition[7]},
                {parsingData + 61, &indexPosition[8]},
                {parsingData + 65, &indexPosition[9]},
                {parsingData + 74, &indexPosition[10]}
            };
            const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

            PXFileCursorAdvance(pxFile, 5u);

            PXTextParseFindAllA
            (
                PXFileCursorPosition(pxFile),
                PXFileRemainingSize(pxFile),
                parsingTokenList,
                values
            );

            PXTextCopyA(indexPosition[0] + 1, PXSpriteFontFontNameSize, PXSpriteFont->Info.Handle.Name, PXSpriteFontFontNameSize);
            PXTextToIntA(indexPosition[1], 5, &PXSpriteFont->Info.Handle.Size);
            PXTextToBoolA(indexPosition[2], 5, &PXSpriteFont->Info.Handle.Bold);
            PXTextToBoolA(indexPosition[3], 5, &PXSpriteFont->Info.Handle.Italic);
            PXTextCopyA(indexPosition[4] + 1, PXSpriteFontCharSetNameSize, PXSpriteFont->Info.Handle.CharSet, PXSpriteFontCharSetNameSize);
            PXTextToBoolA(indexPosition[5], 5, &PXSpriteFont->Info.Handle.Unicode);
            PXTextToIntA(indexPosition[6], 5, &PXSpriteFont->Info.Handle.StretchH);
            PXTextToBoolA(indexPosition[7], 5, &PXSpriteFont->Info.Handle.Smooth);
            PXTextToBoolA(indexPosition[8], 5, &PXSpriteFont->Info.Handle.Supersampling);

            PXTextTerminateBeginFromFirstA(PXSpriteFont->Info.Handle.Name, PXSpriteFontFontNameSize, '\"');
            PXTextTerminateBeginFromFirstA(PXSpriteFont->Info.Handle.CharSet, PXSpriteFontCharSetNameSize, '\"');

            break;
        }
        case PXSpriteFontLineCommon:
        {
            const char parsingData[] = "lineHeight=\0base=\0scaleW=\0scaleH=\0pages=\0packed=\0";
            const char* indexPosition[6];
            const ParsingTokenA parsingTokenList[] =
            {
                {parsingData + 0, &indexPosition[0]},
                {parsingData + 12, &indexPosition[1]},
                {parsingData + 18, &indexPosition[2]},
                {parsingData + 26, &indexPosition[3]},
                {parsingData + 34, &indexPosition[4]},
                {parsingData + 41, &indexPosition[5]}
            };
            const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

            PXFileCursorAdvance(pxFile, 6u);

            PXTextParseFindAllA
            (
                PXFileCursorPosition(pxFile),
                PXFileRemainingSize(pxFile),
                parsingTokenList,
                values
            );

            const PXSize readableSize = PXFileRemainingSize(pxFile);

            PXTextToIntA(indexPosition[0], readableSize, &PXSpriteFont->CommonData.LineHeight);
            PXTextToIntA(indexPosition[1], readableSize, &PXSpriteFont->CommonData.Base);
            PXTextToIntA(indexPosition[2], readableSize, &PXSpriteFont->CommonData.ScaleWidth);
            PXTextToIntA(indexPosition[3], readableSize, &PXSpriteFont->CommonData.ScaleHeight);
            PXTextToIntA(indexPosition[4], readableSize, &PXSpriteFont->CommonData.AmountOfPages);
            PXTextToBoolA(indexPosition[5], readableSize, &PXSpriteFont->CommonData.Packed);

            // Allocate
            {
                const PXSize size = PXSpriteFont->CommonData.AmountOfPages;
                const PXSize sizeInBytes = sizeof(PXSpriteFontPage) * size;
                PXSpriteFontPage* pageList = MemoryAllocateClear(sizeInBytes);

                PXSpriteFont->FontPageListSize = size;
                PXSpriteFont->FontPageList = pageList;

                currentPage = pageList;
            }

            break;
        }
        case PXSpriteFontLinePage:
        {
            const char parsingData[] = "id=\0file=";
            const char* indexPosition[2];
            const ParsingTokenA parsingTokenList[] =
            {
                {parsingData + 0, &indexPosition[0]},
                {parsingData + 4, &indexPosition[1]}
            };
            const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

            PXFileCursorAdvance(pxFile, 5u);

            PXTextParseFindAllA
            (
                PXFileCursorPosition(pxFile),
                PXFileRemainingSize(pxFile),
                parsingTokenList,
                values
            );

            PXTextToIntA(indexPosition[0], 5, &currentPage->PageID);

            // Loading Image
            {
                PXText fontFilePath;
                PXTextConstructWithBufferNamedA(&fontFilePath, fontFilePathBuffer, PXPathSizeMax);
                PXText resultFullPath;
                PXTextConstructWithBufferNamedA(&resultFullPath, resultFullPathBuffer, PXPathSizeMax);
                PXText pageFileName;
                PXTextConstructWithBufferNamedA(&pageFileName, pageFileNameBuffer, PXSpriteFontPageFileNameSize);

                PXFilePathGet(pxFile, &fontFilePath);

                PXTextCopyA
                (
                    indexPosition[1] + 1,
                    PXFileRemainingSize(pxFile),
                    pageFileName.TextA,
                    pageFileName.SizeAllocated
                );

                PXTextTerminateBeginFromFirstA(pageFileName.TextA, pageFileName.SizeAllocated, '\"');

                PXFilePathSwapFileName(&fontFilePath, &resultFullPath, &pageFileName);

                const PXActionResult actionResult = PXImageLoad(&currentPage->FontTextureMap, &resultFullPath);
            }

            break;
        }
        case PXSpriteFontLineCharacterCount:
        {
            const char countText[] = "count=";

            PXFileCursorAdvance(pxFile, 6u);

            char* count = PXTextFindPositionA
                          (
                              PXFileCursorPosition(pxFile),
                              PXFileRemainingSize(pxFile),
                              countText,
                              sizeof(countText) - 1
                          );

            // Allocate
            {
                int size = 0;

                PXTextToIntA
                (
                    count + sizeof(countText) - 1,
                    PXFileRemainingSize(pxFile) - sizeof(countText),
                    &size
                );

                const PXSize sizeInBytes = sizeof(PXSpriteFontCharacter) * size;

                currentPage->CharacteListSize = size;
                currentPage->CharacteList = MemoryAllocateClear(sizeInBytes);
            }

            characterIndex = 0;

            break;
        }
        case PXSpriteFontLineCharacterDefinition:
        {
            const unsigned char acessCharacterOutofBounce = characterIndex >= currentPage->CharacteListSize;

            if (acessCharacterOutofBounce)
            {
                const PXSize sizeCurrent = currentPage->CharacteListSize * sizeof(PXSpriteFontPage);
                const PXSize sizeNew = currentPage->CharacteListSize * sizeof(PXSpriteFontPage) + 1;

                PXSpriteFontCharacter* characteListR = MemoryHeapReallocateClear(currentPage->CharacteList, sizeCurrent, sizeNew);
                const PXBool adresschanged = characteListR != currentPage->CharacteList;

                if (!characteListR)
                {
                    // Error, out of memeory
                }

                currentPage->CharacteListSize++;
                currentPage->CharacteList = characteListR;
            }

            PXSpriteFontCharacter* character = &currentPage->CharacteList[characterIndex++];

            const char parsingData[] = "id=\0x=\0y=\0width=\0height=\0xoffset=\0yoffset=\0xadvance=\0page=\0chnl=";
            const char* indexPosition[10];
            const ParsingTokenA parsingTokenList[] =
            {
                {parsingData + 0, &indexPosition[0]},
                {parsingData + 4, &indexPosition[1]},
                {parsingData + 7, &indexPosition[2]},
                {parsingData + 10, &indexPosition[3]},
                {parsingData + 17, &indexPosition[4]},
                {parsingData + 25, &indexPosition[5]},
                {parsingData + 34, &indexPosition[6]},
                {parsingData + 43, &indexPosition[7]},
                {parsingData + 53, &indexPosition[8]},
                {parsingData + 59, &indexPosition[9]}
            };
            const PXSize values = sizeof(parsingTokenList) / sizeof(ParsingTokenA);

            PXFileCursorAdvance(pxFile, 5u);

            PXTextParseFindAllA
            (
                PXFileCursorPosition(pxFile),
                PXFileRemainingSize(pxFile),
                parsingTokenList,
                values
            );

            PXTextToIntA(indexPosition[0], 5, &character->ID);
            PXTextToPXF32A(indexPosition[1], 5, &character->Position[0]);
            PXTextToPXF32A(indexPosition[2], 5, &character->Position[1]);
            PXTextToPXF32A(indexPosition[3], 5, &character->Size[0]);
            PXTextToPXF32A(indexPosition[4], 5, &character->Size[1]);
            PXTextToPXF32A(indexPosition[5], 5, &character->Offset[0]);
            PXTextToPXF32A(indexPosition[6], 5, &character->Offset[1]);
            PXTextToIntA(indexPosition[7], 5, &character->XAdvance);
            PXTextToIntA(indexPosition[8], 5, &character->Page);
            PXTextToIntA(indexPosition[9], 5, &character->Chanal);

            break;
        }
        }

        PXFileSkipLine(pxFile);
    }
#endif

    PXFileClose(&tokenStream);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXSpriteFontSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}

PXSpriteFontLineType PXAPI PeekSymbol(const char* const line, const PXSize fileDataSize)
{
    if (!line || !fileDataSize)
    {
        return PXSpriteFontInvalid;
    }

    switch (fileDataSize)
    {
    case 1:
    {
        const PXInt8U id = *line;

        switch (id)
        {
        case 'x':
            return PXSpriteFontSymbolX;
        case 'y':
            return PXSpriteFontSymbolY;

        default:
            return PXSpriteFontLineUnkown;
        }

        break;
    }

    case 2:
    {
        const PXInt16U id = PXInt16FromAdress(line);

        switch (id)
        {
        case PXInt16Make('a', 'a'):
            return PXSpriteFontSymbolAntilising;
        case PXInt16Make('i', 'd'):
            return PXSpriteFontSymbolID;

        default:
            return PXSpriteFontLineUnkown;
        }

        break;
    }

    case 4:
    {
        const PXInt32U id = PXInt32FromAdress(line);

        switch (id)
        {
        case PXInt32Make('c', 'h', 'a', 'r'):
            return PXSpriteFontSymbolCharacterDefinition;
        case PXInt32Make('b', 'a', 's', 'e'):
            return PXSpriteFontSymbolBase;
        case PXInt32Make('i', 'n', 'f', 'o'):
            return PXSpriteFontSymbolInfo;
        case PXInt32Make('f', 'a', 'c', 'e'):
            return PXSpriteFontSymbolFontName;
        case PXInt32Make('s', 'i', 'z', 'e'):
            return PXSpriteFontSymbolFontSize;
        case PXInt32Make('b', 'o', 'l', 'd'):
            return PXSpriteFontSymbolBold;
        case PXInt32Make('p', 'a', 'g', 'e'):
            return PXSpriteFontSymbolPage;
        case PXInt32Make('f', 'i', 'l', 'e'):
            return PXSpriteFontSymbolFilePath;
        case PXInt32Make('c', 'h', 'n', 'l'):
            return PXSpriteFontSymbolChannel;

        default:
            return PXSpriteFontLineUnkown;
        }

        break;
    }
    case 5:
    {
        const PXInt64U id = PXInt40FromAdress(line);

        switch (id)
        {
        case PXInt40Make('c', 'h', 'a', 'r', 's'):
            return PXSpriteFontSymbolCharacterList;
        case PXInt40Make('c', 'o', 'u', 'n', 't'):
            return PXSpriteFontSymbolCharacterAmount;
        case PXInt40Make('p', 'a', 'g', 'e', 's'):
            return PXSpriteFontSymbolPageList;
        case PXInt40Make('w', 'i', 'd', 't', 'h'):
            return PXSpriteFontSymbolScaleWidth;

        default:
            return PXSpriteFontLineUnkown;
        }
    }
    case 6:
    {
        const PXInt64U id = PXInt48FromAdress(line);

        switch (id)
        {
        case PXInt48Make('i', 't', 'a', 'l', 'i', 'c'):
            return PXSpriteFontSymbolItalic;
        case PXInt48Make('s', 'c', 'a', 'l', 'e', 'W'):
            return PXSpriteFontSymbolScaleWidth;
        case PXInt48Make('s', 'c', 'a', 'l', 'e', 'H'):
            return PXSpriteFontSymbolScaleHeight;
        case PXInt48Make('p', 'a', 'c', 'k', 'e', 'd'):
            return PXSpriteFontSymbolPacked;
        case PXInt48Make('h', 'e', 'i', 'g', 'h', 't'):
            return PXSpriteFontSymbolScaleHeight;
        case PXInt48Make('c', 'o', 'm', 'm', 'o', 'n'):
            return PXSpriteFontSymbolCommon;
        case PXInt48Make('s', 'm', 'o', 'o', 't', 'h'):
            return PXSpriteFontSymbolSmooth;

        default:
            return PXSpriteFontLineUnkown;
        }
    }
    case 7:
    {
        const PXInt64U id = PXInt56FromAdress(line);

        switch (id)
        {
        case PXInt56Make('c', 'h', 'a', 't', 's', 'e', 't'):
            return PXSpriteFontSymbolCharSet;
        case PXInt56Make('u', 'n', 'i', 'c', 'o', 'd', 'e'):
            return PXSpriteFontSymbolUnicode;
        case PXInt56Make('p', 'a', 'd', 'd', 'i', 'n', 'g'):
            return PXSpriteFontSymbolPadding;
        case PXInt56Make('s', 'p', 'a', 'c', 'i', 'n', 'g'):
            return PXSpriteFontSymbolSpacing;
        case PXInt56Make('x', 'o', 'f', 'f', 's', 'e', 't'):
            return PXSpriteFontSymbolXOffset;
        case PXInt56Make('y', 'o', 'f', 'f', 's', 'e', 't'):
            return PXSpriteFontSymbolYOffset;

        default:
            return PXSpriteFontLineUnkown;
        }

    }

    case 8:
    {
        const PXInt64U id = PXInt64FromAdress(line);

        switch (id)
        {
        case PXInt64Make('x', 'a', 'd', 'v', 'a', 'n', 'c', 'e'):
            return PXSpriteFontSymbolXAdvance;
        case PXInt64Make('s', 't', 'r', 'e', 'a', 'c', 'h', 'H'):
            return PXSpriteFontSymbolStretchH;

        default:
            return PXSpriteFontLineUnkown;
        }
    }
    case 10:
    {
        const char buffer[] = "lineHeight";
        const PXBool isEqual = PXMemoryCompare(buffer, sizeof(buffer)-1, line, fileDataSize);

        if (isEqual)
        {
            return PXSpriteFontSymbolLineHeight;
        }

        return PXSpriteFontLineUnkown;
    }

    default:
        return PXSpriteFontLineUnkown;
    }
}

void PXAPI PXSpriteFontPrtinf(const PXSpriteFont* pxSpriteFont)
{
#if 0
    printf(" +-------------------------------------------------------------------------+\n");
    printf(" | Font (%s) : %s\n", &pxSpriteFont->Info.Handle.CharSet[0], &pxSpriteFont->Info.Handle.Name[0]);
    printf(" +-------------------------------------------------------------------------+\n");
    printf(" | Size     : %4u | Smooth  : %3u |\n", pxSpriteFont->Info.Handle.Size, pxSpriteFont->Info.Handle.Smooth);
    printf(" | Bold     : %4s | AA      : %3u |\n", pxSpriteFont->Info.Handle.Bold ? "Yes" : "No", pxSpriteFont->Info.Handle.Supersampling);
    printf
    (
        " | Italic   : %4s | Padding : %u,%u,%u,%u |\n",
        pxSpriteFont->Info.Handle.Italic ? "Yes" : "No",
        pxSpriteFont->Info.Handle.CharacterPadding[0],
        pxSpriteFont->Info.Handle.CharacterPadding[1],
        pxSpriteFont->Info.Handle.CharacterPadding[2],
        pxSpriteFont->Info.Handle.CharacterPadding[3]

    );
    printf(" | unicode  : %4s | Spacing : %u,%u |\n", pxSpriteFont->Info.Handle.Unicode ? "Yes" : "No", pxSpriteFont->Info.Handle.SpacerOffset[0], pxSpriteFont->Info.Handle.SpacerOffset[1]);
    printf(" | stretchH : %4u | Outline : %3u |\n", pxSpriteFont->Info.Handle.StretchH, pxSpriteFont->Info.Handle.OutlineThickness);


    for(unsigned int pageIndex = 0; pageIndex < pxSpriteFont->FontPageListSize; pageIndex++)
    {
        //    PXSpriteFontPage* page = &pxSpriteFont->FontPageList[pageIndex];

        //printf(" |          |       |       |       |       |       |       |       |\n");
        printf("\n");
        printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
        printf(" | Page <%zu/%zu> \n", pageIndex + 1, pxSpriteFont->FontPageListSize);
        printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
        printf(" | Letter   | X-Pos | Y-Pos | Width | Height| X-Off | Y-Off | X-Step|\n");
        printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");

        for(unsigned int characterIndex = 0; characterIndex < page->CharacteListSize; characterIndex++)
        {
            PXFontPageCharacter* character = &page->CharacteList[characterIndex];

            printf
            (
                " | %3i -> %c | %5.1f | %5.1f | %5.1f | %5.1f | %5.1f | %5.1f | %5i |\n",
                character->ID,
                character->ID,
                character->Position[0],
                character->Position[1],
                character->Size[0],
                character->Size[1],
                character->Offset[0],
                character->Offset[1],
                character->XAdvance
            );
        }

        printf(" +----------+-------+-------+-------+-------+-------+-------+-------|\n");
    }
#endif
}
