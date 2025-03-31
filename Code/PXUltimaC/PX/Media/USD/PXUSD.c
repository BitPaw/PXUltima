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

const char PXUSDTextString[6] = "string";
const PXInt8U PXUSDTextStringSize = sizeof(PXUSDTextString);

const char PXUSDTextAsset[5] = "asset";
const PXInt8U PXUSDTextAssetSize = sizeof(PXUSDTextAsset);


const char PXUSDTextidentifier[] = "identifier";
const PXInt8U PXUSDTextidentifierSize = sizeof(PXUSDTextidentifier);

const char PXUSDTextName[] = "name";
const PXInt8U PXUSDTextNameSize = sizeof(PXUSDTextName);



const char PXUSDTextmodelingVariant[15] = "modelingVariant";
const PXInt8U PXUSDTextmodelingVariantSize = sizeof(PXUSDTextmodelingVariant);


const char PXUSDTextvariants[8] = "variants";
const PXInt8U PXUSDTextvariantsSize = sizeof(PXUSDTextvariants);

const char PXUSDTextAssetInfo[9] = "assetInfo";
const PXInt8U PXUSDTextAssetInfoSize = sizeof(PXUSDTextAssetInfo);


const char PXUSDTextdefaultPrim[11] = "defaultPrim";
const PXInt8U PXUSDTextdefaultPrimSize = sizeof(PXUSDTextdefaultPrim);

const char PXUSDTextupAxis[6] = "upAxis";
const PXInt8U PXUSDTextupAxisSize = sizeof(PXUSDTextupAxis);

const char PXUSDTextkind[4] = "kind";
const PXInt8U PXUSDTextkindSize = sizeof(PXUSDTextkind);

const char PXUSDTextPayload[7] = "payload";
const PXInt8U PXUSDTextPayloadSize = sizeof(PXUSDTextPayload);


const char PXUSDTextreferences[10] = "references";
const PXInt8U PXUSDTextreferencesSize = sizeof(PXUSDTextreferences);

const char PXUSDTextFloat[5] = "float";
const PXInt8U PXUSDTextFloatSize = sizeof(PXUSDTextFloat);

const char PXUSDTextFloat3[6] = "float3";
const PXInt8U PXUSDTextFloat3Size = sizeof(PXUSDTextFloat3);

const char PXUSDTextdouble3[7] = "double3";
const PXInt8U PXUSDTextdouble3Size = sizeof(PXUSDTextdouble3);

const char PXUSDTextuniform[7] = "uniform";
const PXInt8U PXUSDTextuniformSize = sizeof(PXUSDTextuniform);


const char PXUSDTextXFormOp[7] = "xformOp";
const PXInt8U PXUSDTextXFormOpSize = sizeof(PXUSDTextXFormOp);


const char PXUSDTextScale[5] = "scale";
const PXInt8U PXUSDTextScaleSize = sizeof(PXUSDTextScale);

const char PXUSDTextXrotateX[7] = "rotateX";
const PXInt8U PXUSDTextXrotateXSize = sizeof(PXUSDTextXrotateX);

const char PXUSDTextXrotateZ[7] = "rotateZ";
const PXInt8U PXUSDTextXrotateZSize = sizeof(PXUSDTextXrotateZ);

const char PXUSDTextXrotateXYZ[9] = "rotateXYZ";
const PXInt8U PXUSDTextXrotateXYZSize = sizeof(PXUSDTextXrotateXYZ);

const char PXUSDTexttranslate[9] = "translate";
const PXInt8U PXUSDTexttranslateSize = sizeof(PXUSDTexttranslate);


const char PXUSDTexttoken[5] = "token";
const PXInt8U PXUSDTexttokenSize = sizeof(PXUSDTexttoken);


const char PXUSDTextXFormOpOrder[12] = "xformOpOrder";
const PXInt8U PXUSDTextXFormOpOrderSize = sizeof(PXUSDTextXFormOpOrder);



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


void PXAPI PXUSDParseParameterVarriants(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseParameterAssetInfo(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
void PXAPI PXUSDParseParameterPayLoad(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);


void PXAPI PXUSDParseContentPropertyFloat1(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry);
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


    return result;
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





    for(;;)
    {
        PXCompilerSymbolEntryPeek(&pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
            break;
        }

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


    // We parsed the whole file, now we need to load 
    // all sup-data elements that are references

    for(PXSize i = 0; i < length; i++)
    {

    }



    return PXActionSuccessful;
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
    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            break;
        }

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

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            break;
        } 

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
                else
                {
                    const PXBool isDef = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

                    if(isDef)
                    {
                        PXUSDParseDEF(pxCompiler, tokenSteam);
                    }
                }

                break;
            }
            case PXCompilerSymbolLexerBracketCurlyClose:
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }
            case PXCompilerSymbolLexerString:
            {
                PXUSDParseFormThing(pxCompiler, tokenSteam, &pxUSDEntry);

                break;
            }

            default:
            {
                //PXCompilerSymbolEntryForward(pxCompiler);
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
                    const PXBool isvariants = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextvariants, PXUSDTextvariantsSize);
                    const PXBool isAssetInfo = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextAssetInfo, PXUSDTextAssetInfoSize);
                    const PXBool isKind = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextkind, PXUSDTextkindSize);
                    const PXBool isPayload = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextkind, PXUSDTextkindSize);


                    if(isPrepend)
                    {
                        PXCompilerSymbolEntryForward(pxCompiler);

                        PXSize pathSize = 0;
       
                        const PXBool isReference = PXCompilerEnsurePropertyText
                        (
                            pxCompiler,
                            PXUSDTextreferences,
                            PXUSDTextreferencesSize,
                            &pxUSDEntry->Include,
                            &pathSize
                        );

                        pxUSDEntry->IncludeSize = pathSize;
                    }
                    else if(isvariants)
                    {
                        PXUSDParseParameterVarriants(pxCompiler, tokenSteam, pxUSDEntry);
                    }
                    else if(isAssetInfo)
                    {
                        PXUSDParseParameterAssetInfo(pxCompiler, tokenSteam, pxUSDEntry);
                    }
                    else if(isKind)
                    {
                        PXCompilerSymbolEntryForward(pxCompiler);
                    }
                    else if(isPayload)
                    {
                        PXUSDParseParameterPayLoad(pxCompiler, tokenSteam, pxUSDEntry);

                    
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

    PXCompilerSymbolEntryForward(pxCompiler);

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            return;
        }

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



                const PXBool isFloatSignle = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextFloat, PXUSDTextFloatSize);

                if(isFloatSignle)
                {
                    PXUSDParseContentPropertyFloat1(pxCompiler, tokenSteam, pxUSDEntry);
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
                PXCompilerSymbolEntryForward(pxCompiler);
                return;
            }
            default:
            {         
                break;
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

void PXAPI PXUSDParseParameterVarriants(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // remove varriant
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

    for(;;)
    {
        const PXBool isEnd = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);

        if(isEnd)
        {
            break;
        }

        PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextString, PXUSDTextStringSize);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextmodelingVariant, PXUSDTextmodelingVariantSize);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =
    }

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove '}'
}

void PXAPI PXUSDParseParameterAssetInfo(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler);



    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);
    PXCompilerSymbolEntryForward(pxCompiler);


    for(;;)
    {
        const PXBool isClose = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);

        if(isClose)
        {
            break;
        }

        
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric);


        PXBool isString = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextString, PXUSDTextStringSize);
        PXBool isAsset = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextAsset, PXUSDTextAssetSize);


        if(isString)
        {
            PXCompilerSymbolEntryForward(pxCompiler);

            PXBool isIdentifier = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextidentifier, PXUSDTextidentifierSize);

            if(isIdentifier)
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
                PXCompilerSymbolEntryForward(pxCompiler);

                // identifier

                PXCompilerSymbolEntryForward(pxCompiler);
            }
        }
        else if(isAsset)
        {
            PXCompilerSymbolEntryForward(pxCompiler);

            PXBool isName = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextName, PXUSDTextNameSize);

            if(isName)
            {
                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
                PXCompilerSymbolEntryForward(pxCompiler);

                // name

                PXCompilerSymbolEntryForward(pxCompiler);
            }
        }
    }


    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove '}'
}

void PXAPI PXUSDParseParameterPayLoad(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketAngleOpen);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerSlash);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);



    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketAngleClose);
    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDParseContentPropertyFloat1(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // float3

    const PXBool iXFormOp = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(!iXFormOp)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler); // remove op
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
    PXCompilerSymbolEntryForward(pxCompiler);

    const PXBool isRoateX = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXrotateX, PXUSDTextXrotateXSize);
  //  const PXBool isRoateY = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXrotateX, PXUSDTextXrotateXSize);
    const PXBool isRoateZ = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXrotateZ, PXUSDTextXrotateZSize);
      
    PXCompilerSymbolEntryForward(pxCompiler);
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolLexer pxCompilerSymbolLexer[2] = { PXCompilerSymbolLexerReal, PXCompilerSymbolLexerNumeric };

    PXCompilerSymbolEntryEnsureCheckList(pxCompiler, pxCompilerSymbolLexer, 2);

    if(pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerReal)
    {
        if(isRoateX)
        {
            pxUSDEntry->Rotation[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
        }
        else if(isRoateZ)
        {
            pxUSDEntry->Rotation[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
        }
    }
    else
    {
        if(isRoateX)
        {
            pxUSDEntry->Rotation[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
        }
        else if(isRoateZ)
        {
            pxUSDEntry->Rotation[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32S;
        }
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
        const PXBool isScale = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextScale, PXUSDTextScaleSize);

        PXCompilerSymbolEntryForward(pxCompiler);

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);
        PXCompilerSymbolEntryForward(pxCompiler);

        if(isRoate)
        {
            PXCompilerParseCSVF32(pxCompiler, pxUSDEntry->Rotation, 3);
        }
        else if(isScale)
        {
            PXCompilerParseCSVF32(pxCompiler, pxUSDEntry->Scale, 3);
        }

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);
        PXCompilerSymbolEntryForward(pxCompiler);

    }
}

void PXAPI PXUSDParseContentPropertyDouble3(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // double3

    const PXBool iXFormOp = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(iXFormOp)
    {
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isTranslate = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTexttranslate, PXUSDTexttranslateSize);

        if(isTranslate)
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);
            PXCompilerSymbolEntryForward(pxCompiler);   

            PXCompilerParseCSVF64(pxCompiler, &pxUSDEntry->Position, 3);

            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);
            PXCompilerSymbolEntryForward(pxCompiler);
        }
    }

    //  PXCompilerEnsureTextAndCompare(pxCompiler, );
}

void PXAPI PXUSDParseContentPropertyUniform(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryForward(pxCompiler); // uniform

    const PXBool isToken = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTexttoken, PXUSDTexttokenSize);

    if(isToken)
    {
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareClose);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isOpOrder = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextXFormOpOrder, PXUSDTextXFormOpOrderSize);

        if(isOpOrder)
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);

            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);       
            PXCompilerSymbolEntryForward(pxCompiler);



            for(;;)
            {
                // Extract text content that holds object operation
                PXCompilerSymbolEntryForward(pxCompiler); // remove text

                // Maybe we have a list of operations seperated per comma
                const PXBool isComma = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerComma);

                if(!isComma)
                {
                    break;
                }

                PXCompilerSymbolEntryForward(pxCompiler); // remove comma
            }

            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareClose);
            PXCompilerSymbolEntryForward(pxCompiler);
        }
    }


}

void PXAPI PXUSDParseFormThing(PXCompiler* pxCompiler, PXFile* tokenSteam, PXUSDEntry* const pxUSDEntry)
{
    PXCompilerSymbolEntryPeek(pxCompiler);

    PXBool isName = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    if(!isName)
    {
        return;
    }

    pxUSDEntry->Name = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    pxUSDEntry->NameSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

    PXCompilerSymbolEntryForward(pxCompiler);

    PXUSDParseFormThingParam(pxCompiler, tokenSteam, pxUSDEntry);
    PXUSDParseFormThingContent(pxCompiler, tokenSteam, pxUSDEntry);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parsing",
        "Element detected!\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %10.4lf %10.4lf %10.4lf\n"
        "%20s : %10.4f %10.4f %10.4f\n"
        "%20s : %10.4f %10.4f %10.4f",
        "Name", pxUSDEntry->Name,
        "Include", pxUSDEntry->Include,
        "Position", pxUSDEntry->Position[0], pxUSDEntry->Position[1], pxUSDEntry->Position[2],
        "Rotation", pxUSDEntry->Rotation[0], pxUSDEntry->Rotation[1], pxUSDEntry->Rotation[2],
        "Scale", pxUSDEntry->Scale[0], pxUSDEntry->Scale[1], pxUSDEntry->Scale[2]
    );
#endif
}






























PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}













