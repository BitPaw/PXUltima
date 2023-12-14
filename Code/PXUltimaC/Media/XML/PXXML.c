#include "PXXML.h"

#include <Compiler/PXCompiler.h>
#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>

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

void PXAPI PXXMLBlockParse(PXDocument* const pxDocument, PXFile* const pxFile)
{
    PXCompilerSymbolEntry pxCompilerSymbolEntry;

    PXCompilerSymbolEntryPeek(pxFile, &pxCompilerSymbolEntry);

    // Expect '<'
    {
        const PXBool isOpenAngleBracked = PXCompilerSymbolLexerBracketAngleOpen == pxCompilerSymbolEntry.ID;

        if (isOpenAngleBracked) // Is XML tag <xxxx> 
        {
            PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);
         
            PXBool isOpeningTag = PXTrue;

            for (;;)
            {         
                PXBool isClosingTag = PXFalse;

                PXCompilerSymbolEntryPeek(pxFile, &pxCompilerSymbolEntry);

                // Exit if closing bracked
                {
                    switch (pxCompilerSymbolEntry.ID)
                    {
                        case PXCompilerSymbolLexerBracketAngleClose:
                        {
                            PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);                    
                            return; // DONE
                        }
                        case PXCompilerSymbolLexerBracketAngleOpen:    // Go deeper into
                        {
                            PXXMLBlockParse(pxDocument, pxFile);
                            continue;
                        }
                        case PXCompilerSymbolLexerSlash: // Closing Tag 
                        {
                            //PXCompilerSymbolEntryMergeCurrentWithNext(pxFile, &pxCompilerSymbolEntry);
                            PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);

                            isClosingTag = PXTrue;

                            // Fall through
                        }
                        default:
                        {
                            PXCompilerSymbolEntry pxCompilerSymbolEntryPeek;

                            PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);
                            PXCompilerSymbolEntryPeek(pxFile, &pxCompilerSymbolEntryPeek);

                            PXBool isAttribute = PXCompilerSymbolLexerEqual==pxCompilerSymbolEntryPeek.ID;
                            PXBool isTagEnd =
                                PXCompilerSymbolLexerQuestionmark == pxCompilerSymbolEntry.ID &&
                                PXCompilerSymbolLexerBracketAngleClose == pxCompilerSymbolEntryPeek.ID;

                            PXDocumentElement pxDocumentElement;
                            PXClear(PXDocumentElement, &pxDocumentElement);
                            pxDocumentElement.Depth = pxDocument->Depth;
                            pxDocumentElement.NameAdress = pxCompilerSymbolEntry.Source;
                            pxDocumentElement.NameSize = pxCompilerSymbolEntry.Size;   
                            pxDocumentElement.Depth = pxDocument->Depth;

                            if (isAttribute)
                            {                                 
                                pxDocumentElement.Type = PXDocumentElementTypeClassMember;

                                PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);
                                PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);                      
                             
                                pxDocumentElement.ElementMember.ValueType = PXDataTypeString;
                                pxDocumentElement.ElementMember.ValueAdress = pxCompilerSymbolEntry.Source;
                                pxDocumentElement.ElementMember.ValueSize = pxCompilerSymbolEntry.Size;       
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
                PXCompilerSymbolEntry pxCompilerSymbolEntryEEE;

                PXCompilerSymbolEntryForward(pxFile);
                PXCompilerSymbolEntryPeek(pxFile, &pxCompilerSymbolEntryEEE);
                PXCompilerSymbolRewind(pxFile, 1);

                const PXBool isOpenAngleBracked = PXCompilerSymbolLexerBracketAngleOpen == pxCompilerSymbolEntryEEE.ID;

                if (isOpenAngleBracked)
                {
                    PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntryEEE);

                    PXDocumentElement pxDocumentElement;
                    PXClear(PXDocumentElement, &pxDocumentElement);
                    pxDocumentElement.Depth = 0;
                    pxDocumentElement.ElementMember.ValueAdress = pxCompilerSymbolEntry.Source;
                    pxDocumentElement.ElementMember.ValueSize = pxCompilerSymbolEntry.Size;
                    pxDocumentElement.Type = PXDocumentElementTypeClassMember;
               
                    switch (pxCompilerSymbolEntryEEE.ID)
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

                PXCompilerSymbolEntryMergeCurrentWithNext(pxFile, &pxCompilerSymbolEntry);
                // Check if we need to merge more
             
            }                    
        }
    }
}

PXActionResult PXAPI PXXMLLoadFromFile(PXDocument* const pxDocument, PXFile* const pxFile)
{
    PXSize errorCounter = 0;
    PXFile tokenSteam;

    PXClear(PXDocument, pxDocument);

    PXFileBufferAllocate(&pxDocument->Data, 2048*10);

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.KeepWhiteSpace = PXYes;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXYes;
        compilerSettings.IntrepredNewLineAsWhiteSpace = PXYes;
        compilerSettings.KeepTabs = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;

        PXFileBufferAllocate(&tokenSteam, pxFile->DataAllocated * 20);

        PXCompilerLexicalAnalysis(pxFile, &tokenSteam, &compilerSettings); // Raw-File-Input -> Lexer tokens           
    }

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXXMLBlockParse(pxDocument, &tokenSteam);
    }

    PXFileCursorPositionTerminate(&pxDocument->Data);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXXMLSaveToFile(PXDocument* const pxDocument, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}