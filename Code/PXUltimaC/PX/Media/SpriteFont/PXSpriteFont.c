#include "PXSpriteFont.h"

#include <PX/Media/PXText.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>

void PXAPI PXSpriteFontParseInfo(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont);
void PXAPI PXSpriteFontParseCommon(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont);
void PXAPI PXSpriteFontParsePage(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont);
void PXAPI PXSpriteFontParseCharacterList(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont);
void PXAPI PXSpriteFontParseCharacterDefinition(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont);


void PXAPI PXSpriteFontParseInfo(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont)
{
    const PXSize targetLine = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        PXBool isNewLine = targetLine != pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
        PXBool isDone = isEndOfFile || isNewLine;

        if(isDone)
        {
            break;
        }

        const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);


        // if OK

        //---<Check for '='>----------------
        PXCompilerSymbolEntryForward(pxCompiler);
        const PXBool isEqual = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);

        if(!isEqual)
        {
            break;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
        //-----------------------------------

        switch(variableName)
        {
            case PXSpriteFontSymbolFontName:
            {
                PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, pxSpriteFont->Info.Name, PXSpriteFontFontNameSize);
                break;
            }
            case PXSpriteFontSymbolFontSize:
            {
                pxSpriteFont->Info.Size = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolBold:
            {
                pxSpriteFont->Info.Bold = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }
            case PXSpriteFontSymbolItalic:
            {
                pxSpriteFont->Info.Italic = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }
            case PXSpriteFontSymbolCharSet:
            {
                PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, pxSpriteFont->Info.CharSet, PXSpriteFontFontNameSize);
                break;
            }
            case PXSpriteFontSymbolUnicode:
            {
                pxSpriteFont->Info.Unicode = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }
            case PXSpriteFontSymbolStretchH:
            {
                pxSpriteFont->Info.StretchH = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolSmooth:
            {
                pxSpriteFont->Info.Smooth = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }
            case PXSpriteFontSymbolAntilising:
            {
                pxSpriteFont->Info.Supersampling = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }
            case PXSpriteFontSymbolPadding:
            {
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);
                pxSpriteFont->Info.CharacterPadding[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;

                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
                PXCompilerSymbolEntryForward(pxCompiler);

                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);
                pxSpriteFont->Info.CharacterPadding[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;

                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
                PXCompilerSymbolEntryForward(pxCompiler);

                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);
                pxSpriteFont->Info.CharacterPadding[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;

                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerComma);
                PXCompilerSymbolEntryForward(pxCompiler);

                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);
                pxSpriteFont->Info.CharacterPadding[3] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;

            }
            case PXSpriteFontSymbolSpacing:
            {
                pxSpriteFont->Info.SpacerOffset[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;

                PXCompilerSymbolEntryExtract(pxCompiler);
                PXCompilerSymbolEntryExtract(pxCompiler);

                pxSpriteFont->Info.SpacerOffset[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;

                break;
            }
            default:
                break;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

void PXAPI PXSpriteFontParseCommon(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont)
{
    const PXSize targetLine = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        PXBool isNewLine = targetLine != pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
        PXBool isDone = isEndOfFile || isNewLine;

        if(isDone)
        {
            break;
        }

        const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

        // if OK

        //---<Check for '='>----------------
        PXCompilerSymbolEntryForward(pxCompiler);
        const PXBool isEqual = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);

        if(!isEqual)
        {
            return PXActionInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
        //-----------------------------------

        switch(variableName)
        {
            case PXSpriteFontSymbolLineHeight:
            {
                pxSpriteFont->CommonData.LineHeight = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolBase:
            {
                pxSpriteFont->CommonData.Base = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolScaleWidth:
            {
                pxSpriteFont->CommonData.ScaleWidth = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolScaleHeight:
            {
                pxSpriteFont->CommonData.ScaleHeight = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                break;
            }
            case PXSpriteFontSymbolPageList:
            {
                const PXI32U amountOfPages = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;

                PXFontPageCreate(pxFont, amountOfPages);

                break;
            }
            case PXSpriteFontSymbolPacked:
            {
                pxSpriteFont->CommonData.Packed = pxCompiler->ReadInfo.SymbolEntryCurrent.CU;
                break;
            }

            default:
                break;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

void PXAPI PXSpriteFontParsePage(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont)
{
    const PXSize targetLine = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        PXBool isNewLine = targetLine != pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
        PXBool isDone = isEndOfFile || isNewLine;

        if(isDone)
        {
            break;
        }

        const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

        // if OK

           //---<Check for '='>----------------
        PXCompilerSymbolEntryForward(pxCompiler);
        const PXBool isEqual = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);

        if(!isEqual)
        {
            return PXActionInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
        //-----------------------------------

        switch(variableName)
        {
            case PXSpriteFontSymbolID:
            {


                break;
            }
            case PXSpriteFontSymbolFilePath:
            {
                PXText fileName;
                PXTextFromAdressA(&fileName, pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

                // Loading Image
                {
                    PXText fontFilePath;
                    PXTextConstructNamedBufferA(&fontFilePath, fontFilePathBuffer, PXPathSizeMax);
                    PXText resultFullPath;
                    PXTextConstructNamedBufferA(&resultFullPath, resultFullPathBuffer, PXPathSizeMax);

                    PXFilePath(pxCompiler->ReadInfo.FileInput, &fontFilePath, PXFalse);

                    PXFilePathSwapFileName(&fontFilePath, &resultFullPath, &fileName);

                    PXFontPage PXREF pxFontPage = PXFontPageGet(pxFont, pxSpriteFont->PageIndexCurrent);

                    // Load
                    {
                        PXECSCreateInfo pxResourceCreateInfoList;
                        PXClear(PXECSCreateInfo, &pxResourceCreateInfoList);

                        pxResourceCreateInfoList.Type = PXResourceTypeTexture2D;
                        pxResourceCreateInfoList.ObjectReference = (PXECSInfo**)&pxFontPage->Texture;
                        pxResourceCreateInfoList.FilePath = resultFullPath;

                        //PXResourceManagerAdd(&pxResourceCreateInfoList);
                    }
                }
                break;
            }
            default:
                break;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

void PXAPI PXSpriteFontParseCharacterList(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont)
{
    const PXSize targetLine = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        PXBool isNewLine = targetLine != pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
        PXBool isDone = isEndOfFile || isNewLine;

        if(isDone)
        {
            break;
        }

        const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

        // if OK

          //---<Check for '='>----------------
        PXCompilerSymbolEntryForward(pxCompiler);
        const PXBool isEqual = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);

        if(!isEqual)
        {
            return PXActionInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
        //-----------------------------------

        switch(variableName)
        {
            case PXSpriteFontSymbolCharacterAmount:
            {
                PXFontPage PXREF pxFontPage = PXFontPageGet(pxFont, pxSpriteFont->PageIndexCurrent);

                pxFontPage->CharacteListEntrys = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;
                pxFontPage->CharacteList = PXMemoryHeapCallocT(PXFontPageCharacter, pxFontPage->CharacteListEntrys);
                break;
            }
        }

        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

void PXAPI PXSpriteFontParseCharacterDefinition(PXSpriteFont PXREF pxSpriteFont, PXCompiler PXREF pxCompiler, PXFont PXREF pxFont)
{
    const PXSize targetLine = pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
    PXFontPage PXREF pxFontPage = PXFontPageGet(pxFont, pxSpriteFont->PageIndexCurrent);

    // Guarantee size of list
    if(pxSpriteFont->CharacterIndexCurrent >= pxFontPage->CharacteListEntrys)
    {
        pxFontPage->CharacteList = PXMemoryHeapReallocT
        (
            PXFontPageCharacter,
            pxFontPage->CharacteList,
            pxFontPage->CharacteListEntrys + 2
        );

        pxFontPage->CharacteListEntrys += 2;
    }

    PXFontPageCharacter PXREF pxFontPageCharacter = &pxFontPage->CharacteList[pxSpriteFont->CharacterIndexCurrent++];

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        PXBool isEndOfFile = PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
        PXBool isNewLine = targetLine != pxCompiler->ReadInfo.SymbolEntryCurrent.Line;
        PXBool isDone = isEndOfFile || isNewLine;

        if(isDone)
        {
            break;
        }

        const PXSpriteFontLineType variableName = PeekSymbol(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

        // if OK

//---<Check for '='>----------------
        PXCompilerSymbolEntryForward(pxCompiler);
        const PXBool isEqual = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerEqual);

        if(!isEqual)
        {
            return PXActionInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
        //-----------------------------------

        switch(variableName)
        {
            case PXSpriteFontSymbolID:
            {
                pxFontPageCharacter->ID = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolX:
            {
                pxFontPageCharacter->Position[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolY:
            {
                pxFontPageCharacter->Position[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolScaleWidth:
            {
                pxFontPageCharacter->Size[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolScaleHeight:
            {
                pxFontPageCharacter->Size[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolXOffset:
            {
                pxFontPageCharacter->Offset[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }
            case PXSpriteFontSymbolYOffset:
            {
                pxFontPageCharacter->Offset[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
                break;
            }

            case PXSpriteFontSymbolXAdvance:
            {
                pxFontPageCharacter->XAdvance = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
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

        PXCompilerSymbolEntryForward(pxCompiler);
    }
}

PXResult PXAPI PXSpriteFontLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFont PXREF pxFont = (PXFont*)pxResourceLoadInfo->ResourceTarget;


    PXFile* tokenStream = PXFileCreate();

    PXSpriteFont pxSpriteFontEE;
    PXSpriteFont* pxSpriteFont = &pxSpriteFontEE;

    PXClear(PXSpriteFont, pxSpriteFont);

    PXCompiler pxCompiler;

    // Lexer
    {
        PXClear(PXCompiler, &pxCompiler);
        pxCompiler.ReadInfo.FileCache = tokenStream;
        pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
        pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
        pxCompiler.CommentSingleLineSize = 1u;
        pxCompiler.CommentSingleLine = "#";

        PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens
    }


    for(;;)
    {
        PXCompilerSymbolEntryPeek(&pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
            break;
        }

        PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerGeneric);

        // Check what line we are in
        PXSpriteFontLineType lineType = PeekSymbol(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

        PXCompilerSymbolEntryForward(&pxCompiler);

        switch(lineType)
        {
            case PXSpriteFontSymbolInfo:
            {
                PXSpriteFontParseInfo(pxSpriteFont, &pxCompiler, pxFont);
                break;
            }
            case PXSpriteFontSymbolCommon:
            {
                PXSpriteFontParseCommon(pxSpriteFont, &pxCompiler, pxFont);
                break;
            }
            case PXSpriteFontSymbolPage:
            {
                PXSpriteFontParsePage(pxSpriteFont, &pxCompiler, pxFont);
                break;
            }
            case PXSpriteFontSymbolCharacterList:
            {
                PXSpriteFontParseCharacterList(pxSpriteFont, &pxCompiler, pxFont);
                break;
            }
            case PXSpriteFontSymbolCharacterDefinition:
            {
                PXSpriteFontParseCharacterDefinition(pxSpriteFont, &pxCompiler, pxFont);
                break;
            }
            case PXSpriteFontLineUnkown:
            default:
                break;
        }
    }

    PXFileClose(&tokenStream);

    return PXActionSuccessful;
}

PXResult PXAPI PXSpriteFontSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}

PXSpriteFontLineType PXAPI PeekSymbol(const char PXREF line, const PXSize fileDataSize)
{
    if(!line || !fileDataSize)
    {
        return PXSpriteFontInvalid;
    }

    switch(fileDataSize)
    {
        case 1:
        {
            const PXI8U id = *line;

            switch(id)
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
            const PXI16U id = PXI16FromAdress(line);

            switch(id)
            {
                case PXI16Make('a', 'a'):
                    return PXSpriteFontSymbolAntilising;
                case PXI16Make('i', 'd'):
                    return PXSpriteFontSymbolID;

                default:
                    return PXSpriteFontLineUnkown;
            }

            break;
        }

        case 4:
        {
            const PXI32U id = PXI32FromAdress(line);

            switch(id)
            {
                case PXI32Make('c', 'h', 'a', 'r'):
                    return PXSpriteFontSymbolCharacterDefinition;
                case PXI32Make('b', 'a', 's', 'e'):
                    return PXSpriteFontSymbolBase;
                case PXI32Make('i', 'n', 'f', 'o'):
                    return PXSpriteFontSymbolInfo;
                case PXI32Make('f', 'a', 'c', 'e'):
                    return PXSpriteFontSymbolFontName;
                case PXI32Make('s', 'i', 'z', 'e'):
                    return PXSpriteFontSymbolFontSize;
                case PXI32Make('b', 'o', 'l', 'd'):
                    return PXSpriteFontSymbolBold;
                case PXI32Make('p', 'a', 'g', 'e'):
                    return PXSpriteFontSymbolPage;
                case PXI32Make('f', 'i', 'l', 'e'):
                    return PXSpriteFontSymbolFilePath;
                case PXI32Make('c', 'h', 'n', 'l'):
                    return PXSpriteFontSymbolChannel;

                default:
                    return PXSpriteFontLineUnkown;
            }

            break;
        }
        case 5:
        {
            const PXI64U id = PXI40FromAdress(line);

            switch(id)
            {
                case PXI40Make('c', 'h', 'a', 'r', 's'):
                    return PXSpriteFontSymbolCharacterList;
                case PXI40Make('c', 'o', 'u', 'n', 't'):
                    return PXSpriteFontSymbolCharacterAmount;
                case PXI40Make('p', 'a', 'g', 'e', 's'):
                    return PXSpriteFontSymbolPageList;
                case PXI40Make('w', 'i', 'd', 't', 'h'):
                    return PXSpriteFontSymbolScaleWidth;

                default:
                    return PXSpriteFontLineUnkown;
            }
        }
        case 6:
        {
            const PXI64U id = PXI48FromAdress(line);

            switch(id)
            {
                case PXI48Make('i', 't', 'a', 'l', 'i', 'c'):
                    return PXSpriteFontSymbolItalic;
                case PXI48Make('s', 'c', 'a', 'l', 'e', 'W'):
                    return PXSpriteFontSymbolScaleWidth;
                case PXI48Make('s', 'c', 'a', 'l', 'e', 'H'):
                    return PXSpriteFontSymbolScaleHeight;
                case PXI48Make('p', 'a', 'c', 'k', 'e', 'd'):
                    return PXSpriteFontSymbolPacked;
                case PXI48Make('h', 'e', 'i', 'g', 'h', 't'):
                    return PXSpriteFontSymbolScaleHeight;
                case PXI48Make('c', 'o', 'm', 'm', 'o', 'n'):
                    return PXSpriteFontSymbolCommon;
                case PXI48Make('s', 'm', 'o', 'o', 't', 'h'):
                    return PXSpriteFontSymbolSmooth;

                default:
                    return PXSpriteFontLineUnkown;
            }
        }
        case 7:
        {
            const PXI64U id = PXI56FromAdress(line);

            switch(id)
            {
                case PXI56Make('c', 'h', 'a', 't', 's', 'e', 't'):
                    return PXSpriteFontSymbolCharSet;
                case PXI56Make('u', 'n', 'i', 'c', 'o', 'd', 'e'):
                    return PXSpriteFontSymbolUnicode;
                case PXI56Make('p', 'a', 'd', 'd', 'i', 'n', 'g'):
                    return PXSpriteFontSymbolPadding;
                case PXI56Make('s', 'p', 'a', 'c', 'i', 'n', 'g'):
                    return PXSpriteFontSymbolSpacing;
                case PXI56Make('x', 'o', 'f', 'f', 's', 'e', 't'):
                    return PXSpriteFontSymbolXOffset;
                case PXI56Make('y', 'o', 'f', 'f', 's', 'e', 't'):
                    return PXSpriteFontSymbolYOffset;

                default:
                    return PXSpriteFontLineUnkown;
            }

        }

        case 8:
        {
            const PXI64U id = PXI64FromAdress(line);

            switch(id)
            {
                case PXI64Make('x', 'a', 'd', 'v', 'a', 'n', 'c', 'e'):
                    return PXSpriteFontSymbolXAdvance;
                case PXI64Make('s', 't', 'r', 'e', 'a', 'c', 'h', 'H'):
                    return PXSpriteFontSymbolStretchH;

                default:
                    return PXSpriteFontLineUnkown;
            }
        }
        case 10:
        {
            const char buffer[] = "lineHeight";
            const PXBool isEqual = PXMemoryCompare(buffer, sizeof(buffer) - 1, line, fileDataSize);

            if(isEqual)
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
