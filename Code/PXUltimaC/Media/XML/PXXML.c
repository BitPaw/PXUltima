#include "PXXML.h"

#include <Compiler/PXCompiler.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>
#include <Media/PXDocument.h>

PXXMLSymbol PXAPI PXXMLPeekLine(const char* const text, const PXSize textSize)
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
        case 'A': return PXXMLSymbolTagOpenBegin;
        case 'B': return PXXMLSymbolTagOpenFull;
        case 'C': return PXXMLSymbolTagCloseCurrent;
        case 'D': return PXXMLSymbolTagCloseFull;
        case 'F': return PXXMLSymbolAttribute;
    }

    return PXXMLSymbolUnkown;
}

void PXAPI PXXMLBlockParse(PXDocument* const pxDocument, PXCompiler* const pxCompiler)
{
    const PXBool isOpenAngleBracked = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketAngleOpen);

    // Expect '<'
    {
        if (isOpenAngleBracked) // Is XML tag <xxxx> 
        {
            PXCompilerSymbolEntryForward(pxCompiler);
         
            PXBool isOpeningTag = PXTrue;

            for (;;)
            {         
                PXBool isClosingTag = PXFalse;

                PXCompilerSymbolEntryPeek(pxCompiler);

                // Exit if closing bracked
                {
                    switch (pxCompiler->SymbolEntryCurrent.ID)
                    {
                        case PXCompilerSymbolLexerBracketAngleClose:
                        {
                            PXCompilerSymbolEntryExtract(pxCompiler);
                            return; // DONE
                        }
                        case PXCompilerSymbolLexerBracketAngleOpen:    // Go deeper into
                        {
                            PXXMLBlockParse(pxDocument, pxCompiler);
                            continue;
                        }
                        case PXCompilerSymbolLexerSlash: // Closing Tag 
                        {
                            //PXCompilerSymbolEntryMergeCurrentWithNext(pxFile, &pxCompilerSymbolEntry);
                            PXCompilerSymbolEntryExtract(pxCompiler);

                            isClosingTag = PXTrue;

                            // Fall through
                        }
                        default:
                        {
                            PXCompilerSymbolEntryExtract(pxCompiler);
                            PXCompilerSymbolEntryPeek(pxCompiler);

                            PXBool isAttribute = PXCompilerSymbolLexerEqual==pxCompiler->SymbolEntryCurrent.ID;
                            PXBool isTagEnd =
                                PXCompilerSymbolLexerQuestionmark == pxCompiler->SymbolEntryCurrent.ID &&
                                PXCompilerSymbolLexerBracketAngleClose == pxCompiler->SymbolEntryCurrent.ID;

                            PXDocumentElement pxDocumentElement;
                            PXClear(PXDocumentElement, &pxDocumentElement);
                            pxDocumentElement.Depth = pxDocument->Depth;
                            pxDocumentElement.NameAdress = pxCompiler->SymbolEntryCurrent.Source;
                            pxDocumentElement.NameSize = pxCompiler->SymbolEntryCurrent.Size;
                            pxDocumentElement.Depth = pxDocument->Depth;

                            if (isAttribute)
                            {                                 
                                pxDocumentElement.Type = PXDocumentElementTypeClassMember;

                                PXCompilerSymbolEntryExtract(pxCompiler);
                                PXCompilerSymbolEntryExtract(pxCompiler);
                             
                                pxDocumentElement.ElementMember.ValueType = PXDataTypeString;
                                pxDocumentElement.ElementMember.ValueAdress = pxCompiler->SymbolEntryCurrent.Source;
                                pxDocumentElement.ElementMember.ValueSize = pxCompiler->SymbolEntryCurrent.Size;
                            }
                            else
                            {
                                pxDocumentElement.Type = PXDocumentElementTypeClass;     
                                pxDocumentElement.ElementClass.IsTerminateSignal = isClosingTag;
                            }   

                            // Calc Depth
                            {
                                if (isClosingTag || isTagEnd)
                                {
                                    --(pxDocument->Depth);

                                    pxDocumentElement.Depth = pxDocument->Depth;
                                }
                                else if(isOpeningTag)
                                {
                                    ++(pxDocument->Depth);                             

                                    isOpeningTag = PXFalse;
                                }                             
                            }

                            PXDocumentElementAdd(pxDocument, &pxDocumentElement);

                            break;
                        }
                    }
                }               
            }
        }
        else // Is plain text, not an XML tag
        {
            for (;;)
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeek(pxCompiler);
                PXCompilerSymbolRewind(pxCompiler, 1);

                const PXBool isOpenAngleBracked = PXCompilerSymbolLexerBracketAngleOpen == pxCompiler->SymbolEntryCurrent.ID;

                if (isOpenAngleBracked)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);

                    PXDocumentElement pxDocumentElement;
                    PXClear(PXDocumentElement, &pxDocumentElement);
                    pxDocumentElement.Depth = 0;
                    pxDocumentElement.ElementMember.ValueAdress = pxCompiler->SymbolEntryCurrent.Source;
                    pxDocumentElement.ElementMember.ValueSize = pxCompiler->SymbolEntryCurrent.Size;
                    pxDocumentElement.Type = PXDocumentElementTypeClassMember;
               
                    switch (pxCompiler->SymbolEntryCurrent.ID)
                    {
                        case PXCompilerSymbolLexerBool:
                            pxDocumentElement.ElementMember.ValueType = PXDataTypeInt08S;
                            break;

                        case PXCompilerSymbolLexerFloat:
                            pxDocumentElement.ElementMember.ValueType = PXDataTypeFloat;
                            break;

                        case PXCompilerSymbolLexerInteger:
                            pxDocumentElement.ElementMember.ValueType = PXDataTypeIntS;
                            break;

                        default:
                        case PXCompilerSymbolLexerString:
                            pxDocumentElement.ElementMember.ValueType = PXDataTypeString;
                            break;
                    }

                    PXDocumentElementAdd(pxDocument, &pxDocumentElement);

                    return;
                }

                PXCompilerSymbolEntryMergeCurrentWithNext(pxCompiler, PXNull);
                // Check if we need to merge more
             
            }                    
        }
    }
}

PXActionResult PXAPI PXXMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    PXDocument* const pxDocument = (PXDocument*)pxResourceLoadInfo->Target;

    PXFile tokenSteam;
    PXClear(PXFile, &tokenSteam);

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.FileCache = &tokenSteam;

    PXClear(PXDocument, pxDocument);

    PXFileBufferAllocate(&pxDocument->Data, 2048*10);

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;
        PXClear(PXCompilerSettings, &compilerSettings);

        compilerSettings.KeepWhiteSpace = PXYes;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXYes;
        compilerSettings.IntrepredNewLineAsWhiteSpace = PXYes;
        compilerSettings.KeepTabs = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;

        PXCompilerLexicalAnalysis(&pxCompiler, &compilerSettings); // Raw-File-Input -> Lexer tokens           
    }

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXXMLBlockParse(pxDocument, &pxCompiler);
    }

    PXFileCursorPositionTerminate(&pxDocument->Data);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXXMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}