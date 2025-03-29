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
const char PXUSDTextSignature[4] = "usda";
const PXInt8U PXUSDTextSignatureSize = sizeof(PXUSDTextSignature);

const char PXUSDTextDef[3] = "def";
const PXInt8U PXUSDTextDefSize = sizeof(PXUSDTextDef);

const char PXUSDTextXform[5] = "Xform";
const PXInt8U PXUSDTextXformSize = sizeof(PXUSDTextXform);

const char PXUSDTextprepend[7] = "prepend";
const PXInt8U PXUSDTextprependSize = sizeof(PXUSDTextprepend);

const char PXUSDTextdefaultPrim[11] = "defaultPrim";
const PXInt8U PXUSDTextdefaultPrimSize = sizeof(PXUSDTextdefaultPrim);

const char PXUSDTextupAxis[6] = "upAxis";
const PXInt8U PXUSDTextupAxisSize = sizeof(PXUSDTextupAxis);

const char PXUSDTextkind[4] = "kind";
const PXInt8U PXUSDTextkindSize = sizeof(PXUSDTextkind);

const char PXUSDTextreferences[10] = "references";
const PXInt8U PXUSDTextreferencesSize = sizeof(PXUSDTextreferences);

const char PXUSDTextFloat3[6] = "float3";
const PXInt8U PXUSDTextFloat3Size = sizeof(PXUSDTextFloat3);

const char PXUSDTextdouble3[7] = "double3";
const PXInt8U PXUSDTextdouble3Size = sizeof(PXUSDTextdouble3);

const char PXUSDTextuniform[7] = "uniform";
const PXInt8U PXUSDTextuniformSize = sizeof(PXUSDTextuniform);


const char PXUSDTextXFormOp[7] = "xformOp";
const PXInt8U PXUSDTextXFormOpSize = sizeof(PXUSDTextXFormOp);


const char PXUSDTextXrotateXYZ[9] = "rotateXYZ";
const PXInt8U PXUSDTextXrotateXYZSize = sizeof(PXUSDTextXrotateXYZ);

const char PXUSDTexttranslate[9] = "translate";
const PXInt8U PXUSDTexttranslateSize = sizeof(PXUSDTexttranslate);




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
void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam);
void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseFormThingContent(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);


void PXAPI PXUSDParseContentPropertyFloat3(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseContentPropertyDouble3(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseContentPropertyUniform(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);




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
        PXBool isusda = PXCompilerEnsureTextAndCompare(&pxCompiler, PXUSDTextSignature, PXUSDTextSignatureSize);

        if(!isusda)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXBool isFloatNumber = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerReal);

        if(!isFloatNumber)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }

        pxUSD.Text.Version = pxCompiler.ReadInfo.SymbolEntryCurrent.F32;

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
                const PXBool isDef = PXCompilerEnsureTextAndCompare(&pxCompiler, PXUSDTextDef, PXUSDTextDefSize);               

                if(isDef)
                {            
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
            case PXCompilerSymbolLexerGeneric:
            {
                PXCompilerEnsurePropertyText(pxCompiler, PXUSDTextdefaultPrim, PXUSDTextdefaultPrimSize, PXNull, PXNull);
                PXCompilerEnsurePropertyText(pxCompiler, PXUSDTextupAxis, PXUSDTextupAxisSize, PXNull, PXNull);

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

void PXAPI PXUSDParseDEF(PXCompiler* pxCompiler, PXFile* tokenSteam)
{
    PXUSDEntry pxUSDEntry;
    PXClear(PXUSDEntry, &pxUSDEntry);
        
    PXCompilerSymbolEntryForward(pxCompiler); // remove "def"

    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isxform = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXform, PXUSDTextXformSize);

                if(isxform)
                {
                    pxUSDEntry.Flags |= PXUSDEntryXFORM;

                    PXCompilerSymbolEntryForward(pxCompiler);

                    PXUSDParseFormThing(pxCompiler, tokenSteam, &pxUSDEntry);
                }      

                return;
            }
            case PXCompilerSymbolLexerString:
            {
                PXUSDParseFormThing(pxCompiler, tokenSteam, &pxUSDEntry);

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


void PXAPI PXUSDParseFormThingParam(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
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
                if(PXUSDEntryXFORM & pxUSDEntry->Flags) // is XFORM
                {
                    PXCompilerEnsurePropertyText(pxCompiler, PXUSDTextkind, PXUSDTextkindSize, PXNull, PXNull);
                }
                else
                {
                    const PXBool isPrepend = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextprepend, PXUSDTextprependSize);

                    if(isPrepend)
                    {
                        PXCompilerSymbolEntryForward(pxCompiler);

                        PXSize pathSize = 0;
                        char pathCache[PXPathSizeMax];

                        const PXBool isReference = PXCompilerEnsurePropertyText
                        (
                            pxCompiler,
                            PXUSDTextreferences,
                            PXUSDTextreferencesSize,
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

                   //PXCompilerSymbolEntryForward(pxCompiler);
                }     

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

void PXAPI PXUSDParseFormThingContent(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);

    PXBool done = 0;

    PXCompilerSymbolEntryForward(pxCompiler);

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
                const PXBool isDef = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

                if(isDef)
                {             
                    PXUSDParseDEF(pxCompiler, tokenSteam);
                    break;
                }


                const PXBool isFloat = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextFloat3, PXUSDTextFloat3Size);

                if(isFloat)
                {
                    PXUSDParseContentPropertyFloat3(pxCompiler, tokenSteam, pxUSDEntry);
                    break;
                }

                const PXBool isDouble = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextdouble3, PXUSDTextdouble3Size);

                if(isDouble)
                {
                    PXUSDParseContentPropertyDouble3(pxCompiler, tokenSteam, pxUSDEntry);
                    break;
                }


                const PXBool isUniform = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextuniform, PXUSDTextuniformSize);

                if(isUniform)
                {
                    PXUSDParseContentPropertyUniform(pxCompiler, tokenSteam, pxUSDEntry);
                    break;
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

void PXAPI PXUSDParseContentPropertyFloat3(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // float3

    const PXBool iXFormOp = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(iXFormOp)
    {

        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isRoate = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXrotateXYZ, PXUSDTextXrotateXYZSize);

        if(isRoate)
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);
            PXCompilerSymbolEntryForward(pxCompiler);

            pxUSDEntry->Rotation.X = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
            PXCompilerSymbolEntryForward(pxCompiler);

            pxUSDEntry->Rotation.Y = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
            PXCompilerSymbolEntryForward(pxCompiler);

            pxUSDEntry->Rotation.Z = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
            PXCompilerSymbolEntryForward(pxCompiler);

            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);
            PXCompilerSymbolEntryForward(pxCompiler);
        }

        const PXBool isTranslate = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTexttranslate, PXUSDTexttranslateSize);

        if(isTranslate)
        {
         
            PXCompilerSymbolEntryForward(pxCompiler);
        }
    }

  //  PXCompilerEnsureTextAndCompare(pxCompiler, );

}

void PXAPI PXUSDParseContentPropertyDouble3(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // double3
}

void PXAPI PXUSDParseContentPropertyUniform(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // uniform
}

void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    PXBool isName = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    if(!isName)
    {
        return;
    }

    PXTextCopyA(pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, pxUSDEntry->Name, 32);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parsing",
        "Element:%s",
        pxUSDEntry->Name
    );
#endif

    PXCompilerSymbolEntryForward(pxCompiler);

    PXUSDParseFormThingParam(pxCompiler, tokenSteam, pxUSDEntry);
    PXUSDParseFormThingContent(pxCompiler, tokenSteam, pxUSDEntry);
}






























PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}













