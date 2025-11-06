#include "PXXML.h"

#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/Media/PXDocument.h>

PXXMLSymbol PXAPI PXXMLPeekLine(const char PXREF text, const PXSize textSize)
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
    case 'A':
        return PXXMLSymbolTagOpenBegin;
    case 'B':
        return PXXMLSymbolTagOpenFull;
    case 'C':
        return PXXMLSymbolTagCloseCurrent;
    case 'D':
        return PXXMLSymbolTagCloseFull;
    case 'F':
        return PXXMLSymbolAttribute;
    }

    return PXXMLSymbolUnkown;
}

void PXAPI PXXMLBlockParse(PXCodeDocument PXREF pxDocument, PXCompiler PXREF pxCompiler)
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
                    switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
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

                        PXBool isAttribute = PXCompilerSymbolLexerEqual== pxCompiler->ReadInfo.SymbolEntryCurrent.ID;
                        PXBool isTagEnd =
                            PXCompilerSymbolLexerQuestionmark ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID &&
                            PXCompilerSymbolLexerBracketAngleClose ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

                        PXCodeDocumentElement pxDocumentElement;
                        PXClear(PXCodeDocumentElement, &pxDocumentElement);
                        //  pxDocumentElement.Depth = pxDocument->Depth;
                        pxDocumentElement.NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                        pxDocumentElement.NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
                        // pxDocumentElement.Depth = pxDocument->Depth;

                        if (isAttribute)
                        {
                            pxDocumentElement.Type = PXDocumentElementTypeClassMember;

                            PXCompilerSymbolEntryExtract(pxCompiler);
                            PXCompilerSymbolEntryExtract(pxCompiler);

                            pxDocumentElement.ElementMember.ValueType = PXTypeString;
                            pxDocumentElement.ElementMember.ValueAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                            pxDocumentElement.ElementMember.ValueSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
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
                                //  --(pxDocument->Depth);

                                //  pxDocumentElement.Depth = pxDocument->Depth;
                            }
                            else if(isOpeningTag)
                            {
                                //++(pxDocument->Depth);

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

                const PXBool isOpenAngleBracked = PXCompilerSymbolLexerBracketAngleOpen ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

                if (isOpenAngleBracked)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);

                    PXCodeDocumentElement pxDocumentElement;
                    PXClear(PXCodeDocumentElement, &pxDocumentElement);
                    pxDocumentElement.Depth = 0;
                    pxDocumentElement.ElementMember.ValueAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                    pxDocumentElement.ElementMember.ValueSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
                    pxDocumentElement.Type = PXDocumentElementTypeClassMember;

                    switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
                    {
                    case PXCompilerSymbolLexerBool:
                        pxDocumentElement.ElementMember.ValueType = PXTypeInt08S;
                        break;

                    case PXCompilerSymbolLexerReal:
                        pxDocumentElement.ElementMember.ValueType = PXTypeF32;
                        break;

                    case PXCompilerSymbolLexerNumeric:
                        pxDocumentElement.ElementMember.ValueType = PXTypeIntS;
                        break;

                    default:
                    case PXCompilerSymbolLexerString:
                        pxDocumentElement.ElementMember.ValueType = PXTypeString;
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

PXResult PXAPI PXXMLLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo)
{
    PXFile tokenSteam;
    PXClear(PXFile, &tokenSteam);

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.CodeDocument = (PXCodeDocument*)pxResourceLoadInfo->ResourceTarget;
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = &tokenSteam;
    //pxCompiler.Flags =

    // Lexer - Level I
    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXXMLBlockParse(0, &pxCompiler);
    }

    PXFileCursorPositionTerminate(0);

    return PXActionSuccessful;
}

PXResult PXAPI PXXMLSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
