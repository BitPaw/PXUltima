#include "PXUSD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/LZ4/PXLZ4.h>

// Logging
const char PXUSDName[] = "USD";
const char PXUSDAName[] = "USD-Text";
const char PXUSDCName[] = "USD-Binary";

// Text format
const char PXUSDTextDef[] = "def";
const char PXUSDTextXform[] = "Xform";
const char PXUSDTextdefaultPrim[] = "defaultPrim";
const char PXUSDTextupAxis[] = "upAxis";
const char PXUSDTextkind[] = "kind";
const char PXUSDTextprepend[] = "prepend";
const char PXUSDTextreferences[] = "references";
const char PXUSDTextdouble3[] = "double3";
const char PXUSDTextuniform[] = "uniform";

// Binary Format
const char PXUSDBinarySignature[8] = { 'P','X','R','-','U','S','D','C' }; // PXR-USDC
const char PXUSDBinaryTOKENS[] = "TOKENS";
const char PXUSDBinarySTRINGS[] = "STRINGS";
const char PXUSDBinaryFields[] = "FIELDS";
const char PXUSDBinarFIELDSETSy[] = "FIELDSETS";
const char PXUSDBinarySPECS[] = "SPECS";
const char PXUSDBinaryPATHS[] = "PATHS";

typedef PXActionResult(PXAPI* PXUSDLoadFunction)(PXResourceTransphereInfo* const pxResourceLoadInfo);
typedef PXActionResult(PXAPI* PXUSDSegmentLoadFunction)(PXFile* const pxFile, void* object);

const char* listData[6] =
{
    PXUSDBinaryTOKENS,
    PXUSDBinarySTRINGS,
    PXUSDBinaryFields,
    PXUSDBinarFIELDSETSy,
    PXUSDBinarySPECS,
    PXUSDBinaryPATHS
};
const PXInt8U listsize[6] =
{
    sizeof(PXUSDBinaryTOKENS),
    sizeof(PXUSDBinarySTRINGS),
    sizeof(PXUSDBinaryFields),
    sizeof(PXUSDBinarFIELDSETSy),
    sizeof(PXUSDBinarySPECS),
    sizeof(PXUSDBinaryPATHS)
};
const PXUSDSegmentLoadFunction listFunction[6] =
{
   PXUSDCSectionTokensLoad,
   PXUSDCSectionStringsLoad,
   PXUSDCSectionFields,
   PXUSDCSectionFieldSets,
   PXUSDCSectionSpecs,
   PXUSDCSectionPaths
};
const PXInt8U amount = sizeof(listsize) / sizeof(PXSize);







void PXAPI PXUSDParsePropertysScene(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam);



PXActionResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXFile* const pxFile = pxResourceLoadInfo->FileReference; 
    PXUSDLoadFunction pxUSDLoadFunction;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDName,
        "Parsing",
        "Begin.."
    );
#endif

    switch(*(char*)pxFile->Data) // Unsafe
    {
        case '#':
            pxUSDLoadFunction = PXUSDALoadFromFile;
            break;

        case 'P':
            pxUSDLoadFunction = PXUSDCLoadFromFile;
            break;

            // PXUSDZLoadFromFile

        default:
            return PXActionRefusedInvalidHeaderSignature;
    }

    const PXActionResult result = pxUSDLoadFunction(pxResourceLoadInfo);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDName,
        "Parsing",
        "DONE!"
    );
#endif


    return PXActionRefusedNotImplemented;
}


PXActionResult PXAPI PXUSDALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXCompiler pxCompiler;
    PXFile tokenSteam;
    PXUSD pxUSD;

    // Lexer
    {
        PXClear(PXCompiler, &pxCompiler);
        pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
        pxCompiler.ReadInfo.FileCache = &tokenSteam;
        pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
        //pxCompiler.CommentSingleLineSize = 1u;
        //pxCompiler.CommentSingleLine = "#";

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDAName,
            "Parsing",
            "Start"
        );
#endif


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDAName,
            "Parsing",
            "Step 1, Lexer..."
        );
#endif

        // Lexer - Level I
        PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens
    }


    // Check if it's an text file
    {
        PXBool isHash = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerHash);

        if(!isHash)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXBool isusda = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerGeneric);

        if(!isusda)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXBool isFloatNumber = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerPXF32);

        if(!isFloatNumber)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxUSD.Text.Version = pxCompiler.ReadInfo.SymbolEntryCurrent.DataF;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDAName,
            "Parsing",
            "Version: %f",
            pxUSD.Text.Version
        );
#endif

        PXCompilerSymbolEntryForward(&pxCompiler);
    }





    while(!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntryPeek(&pxCompiler);

        switch(pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundOpen:
            {
                PXUSDParsePropertysScene(&pxCompiler, &tokenSteam);
                break;
            }
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isDef = PXTextCompareA(PXUSDTextDef, sizeof(PXUSDTextDef) - 1, pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

                if(isDef)
                {
                    PXCompilerSymbolEntryForward(&pxCompiler);
                    PXUSDParseDEF(&pxCompiler, &tokenSteam);
                }

                break;
            }

            default:
                break;
        }

    }
}

PXActionResult PXAPI PXUSDCSectionTokensLoad(PXFile* const pxFile, PXTOCSectionTokens* const pxTOCSectionTokens)
{
    PXFileReadI64U(pxFile, &pxTOCSectionTokens->SizeUncompressed);
    PXFileReadI64U(pxFile, &pxTOCSectionTokens->SizeCompressed);


    PXFile pxFileCompressed;
    PXFile pxFileUncompressed;

    {
        PXFileOpenInfo pxFileCompressedInfo;
        //PXFileOpen();
    }


    {

        PXFileOpenInfo pxFileCompressedInfo;
        //PXFileOpen();
    }

    PXLZ4Decompress(&pxFileCompressed, &pxFileUncompressed);

}

PXActionResult PXAPI PXUSDCSectionStringsLoad(PXFile* const pxFile, PXTOCSectionStrings* const pxTOCSectionStrings)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXUSDCSectionFields(PXFile* const pxFile, PXTOCSectionFields* const pxTOCSectionFields)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXUSDCSectionFieldSets(PXFile* const pxFile, PXTOCSectionFieldSets* const pxTOCSectionFieldSets)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXUSDCSectionSpecs(PXFile* const pxFile, PXTOCSectionSpecs* const pxTOCSectionSpecs)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXUSDCSectionPaths(PXFile* const pxFile, PXTOCSectionPaths* const pxTOCSectionPaths)
{
    return PXActionRefusedNotImplemented;
}


PXActionResult PXAPI PXUSDCLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXUSD pxUSD;
    PXFile* const pxFile = pxResourceLoadInfo->FileReference;

    PXBool isSignatureValid = PXFileReadAndCompare(pxFile, PXUSDBinarySignature, sizeof(PXUSDBinarySignature));

    if(!isSignatureValid)
    {
        return PXActionRefusedInvalidHeaderSignature;
    }

    char versionString;

    PXFileReadB(pxFile, versionString, 8);
    PXFileReadI64U(pxFile, &pxUSD.Binary.TOCOffset);


    // TOC-Chunk
    {
        PXFileCursorMoveTo(pxFile, pxUSD.Binary.TOCOffset);
        PXFileReadI64U(pxFile, &pxUSD.Binary.TOCSectionsAmount);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDCName,
            "Load",
            "Dections: %i",
            pxUSD.Binary.TOCSectionsAmount
        );
#endif

        pxUSD.Binary.TOCSectionList = PXMemoryHeapCallocT(PXTOCSection, pxUSD.Binary.TOCSectionsAmount);

        for(PXSize i = 0; i < pxUSD.Binary.TOCSectionsAmount; ++i)
        {
            PXTOCSection* const pxTOCSection = &pxUSD.Binary.TOCSectionList[i];

            PXFileReadTextA(pxFile, pxTOCSection->Name, 16);
            PXFileReadI64U(pxFile, &pxTOCSection->OffsetStart);
            PXFileReadI64U(pxFile, &pxTOCSection->BlockSize);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXUSDCName,
                "Load",
                "Dection: %i/%i, %s, Start:%i, Size:%i",
                i+1,
                pxUSD.Binary.TOCSectionsAmount,
                pxTOCSection->Name,
                pxTOCSection->OffsetStart,
                pxTOCSection->BlockSize
            );
#endif
        }
    }


    // Process tokens
    {
        for(PXSize i = 0; i < pxUSD.Binary.TOCSectionsAmount; ++i)
        {
            PXTOCSection* const pxTOCSection = &pxUSD.Binary.TOCSectionList[i];

            const PXInt8U index = PXTextCompareAVI8(pxTOCSection->Name, 16, PXUSDBinaryTOKENS, listsize, amount);

            if(-1 == index)
            {
                // Not registerd, cant be handled
                continue;
            }

            PXFileCursorMoveTo(pxFile, pxTOCSection->OffsetStart);

            PXFileReadI64U(pxFile, &pxTOCSection->Tokens.NumberOfTokens);

            listFunction[index](pxFile, pxTOCSection->Data);

            const PXBool isAlligned = pxFile->DataCursor == (pxTOCSection->OffsetStart + pxTOCSection->BlockSize);
        }
    }


    /*
        PXSize TOCSectionCounterTokens;
    PXSize TOCSectionCounterPaths;
    PXSize TOCSectionCounterStrings;
    PXSize TOCSectionCounterFields;
    PXSize TOCSectionCounterFieldsets;
    PXSize TOCSectionCounterSpecs;
    */


}



PXActionResult PXAPI PXUSDZLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{

}





void PXAPI PXUSDParsePropertysScene(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundClose:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }

            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }
}

void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isxform = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXform, sizeof(PXUSDTextXform) - 1);

                PXCompilerSymbolEntryForward(pxCompiler);

                PXUSDParseFormThing(pxCompiler, tokenSteam);

                return;
            }
            case PXCompilerSymbolLexerString:
            {
                PXUSDParseFormThing(pxCompiler, tokenSteam);

                break;
            }

            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }
}


void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);

    PXBool done = 0;

    while(!done)
    {
        if(PXFileIsAtEnd(tokenSteam))
        {
            done = 1;
            continue;
        }

        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundClose:
            {
                done = 1;
                continue;
            }
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isPrepend = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextprepend, sizeof(PXUSDTextprepend) - 1);

                if(isPrepend)
                {                    
                    PXCompilerSymbolEntryForward(pxCompiler);

                    PXSize pathSize = 0;
                    char pathCache[PXPathSizeMax];

                    const PXBool isReference = PXCompilerEnsurePropertyText
                    (
                        pxCompiler,
                        PXUSDTextreferences, 
                        sizeof(PXUSDTextreferences) - 1,
                        pathCache,
                        &pathSize
                    );

                    if(isReference)
                    {
                       
#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            PXUSDAName,
                            "Parsing",
                            "Path:%s",
                            pathCache
                        );
#endif

           
                    }
                }

                PXCompilerSymbolEntryForward(pxCompiler);

                break;
            }
            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                break;
            }
        }
    }

    PXBool isClose = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);

    if(!isClose)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDParseFormThingContent(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);

    PXBool done = 0;

    while(!done)
    {
        if(PXFileIsAtEnd(tokenSteam))
        {
            done = 1;
            continue;
        }

        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isDef = PXTextCompareA(PXUSDTextDef, sizeof(PXUSDTextDef) - 1, pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

                if(isDef)
                {
                    PXCompilerSymbolEntryForward(pxCompiler);
                    PXUSDParseDEF(pxCompiler, tokenSteam);
                }

                break;
            }
            case PXCompilerSymbolLexerBracketCurlyClose:
            {
                done = 1;
                continue;
            }
            default:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }
        }
    }


    PXBool isClose = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);

    if(!isClose)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    PXBool isName = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    if(!isName)
    {
        return;
    }

#if PXLogEnable
    char nameCache[64];
    PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, nameCache, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parsing",
        "Element:%s",
        nameCache
    );
#endif

    PXCompilerSymbolEntryForward(pxCompiler);

    PXUSDParseFormThingParam(pxCompiler, tokenSteam);
    PXUSDParseFormThingContent(pxCompiler, tokenSteam);
}






























PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}













