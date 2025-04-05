#include "PXUSD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/LZ4/PXLZ4.h>

// Logging
const char PXUSDName[] = "USD";
const char PXUSDAName[] = "Text-USD";
const char PXUSDCName[] = "Binary-USD";

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


const char PXUSDTextidentifier[10] = "identifier";
const PXInt8U PXUSDTextidentifierSize = sizeof(PXUSDTextidentifier);

const char PXUSDTextName[4] = "name";
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




typedef void (PXAPI* PXUSDAParseElementFunction)(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler);



const PXUSDAParseElementFunction PXUSDAParseEntryParameterListFunction[] =
{
    PXUSDAParseEntryParameterPrepend,
    PXUSDAParseEntryParameterVarriants,
    PXUSDAParseEntryParameterAssetInfo,
    PXUSDAParseEntryParameterKind,
    PXUSDAParseEntryParameterPayLoad
};

const PXUSDAParseElementFunction PXUSDAParseEntryParameterListText[5] =
{
    PXUSDTextprepend,
    PXUSDTextvariants,
    PXUSDTextAssetInfo,
    PXUSDTextkind,
    PXUSDTextPayload
};

const PXInt8U PXUSDAParseEntryParameterListSize[5] =
{
    sizeof(PXUSDTextprepend),
    sizeof(PXUSDTextvariants),
    sizeof(PXUSDTextAssetInfo),
    sizeof(PXUSDTextkind),
    sizeof(PXUSDTextPayload)
};
const PXInt8U PXUSDAParseEntryParameterListAmount = sizeof(PXUSDAParseEntryParameterListSize) / sizeof(PXInt8U);











const PXUSDAParseElementFunction PXUSDAParseEntryPropertyListFunction[] =
{
    PXUSDAParseElementDefine,
    PXUSDAParseEntryPropertyFloat1,
    PXUSDAParseEntryPropertyFloat3,
    PXUSDAParseEntryPropertyDouble3,
    PXUSDAParseEntryPropertyUniform
};

const PXUSDAParseElementFunction PXUSDAParseEntryPropertyListText[5] =
{
    PXUSDTextDef,
    PXUSDTextFloat,
    PXUSDTextFloat3,
    PXUSDTextdouble3,
    PXUSDTextuniform
};

const PXInt8U PXUSDAParseEntryPropertyListSize[5] =
{
    sizeof(PXUSDTextDef),
    sizeof(PXUSDTextFloat),
    sizeof(PXUSDTextFloat3),
    sizeof(PXUSDTextdouble3),
    sizeof(PXUSDTextuniform)
};
const PXInt8U PXUSDAParseEntryPropertyListAmount = sizeof(PXUSDAParseEntryPropertyListSize) / sizeof(PXInt8U);






// Binary Format
const char PXUSDBinarySignature[8] = "PXR-USDC";
const char PXUSDBinaryTOKENS[6] = "TOKENS";
const char PXUSDBinarySTRINGS[7] = "STRINGS";
const char PXUSDBinaryFields[6] = "FIELDS";
const char PXUSDBinarFIELDSETSy[9] = "FIELDSETS";
const char PXUSDBinarySPECS[5] = "SPECS";
const char PXUSDBinaryPATHS[5] = "PATHS";

typedef PXActionResult(PXAPI* PXUSDLoadFunction)(PXResourceTransphereInfo* const pxResourceLoadInfo);
typedef PXActionResult(PXAPI* PXUSDSegmentLoadFunction)(PXFile* const pxFile, void* object);

const char* PXUSDBinaryTokenListData[6] =
{
    PXUSDBinaryTOKENS,
    PXUSDBinarySTRINGS,
    PXUSDBinaryFields,
    PXUSDBinarFIELDSETSy,
    PXUSDBinarySPECS,
    PXUSDBinaryPATHS
};
const PXInt8U PXUSDBinaryTokenListsize[6] =
{
    sizeof(PXUSDBinaryTOKENS),
    sizeof(PXUSDBinarySTRINGS),
    sizeof(PXUSDBinaryFields),
    sizeof(PXUSDBinarFIELDSETSy),
    sizeof(PXUSDBinarySPECS),
    sizeof(PXUSDBinaryPATHS)
};
const PXUSDSegmentLoadFunction PXUSDBinaryTokenListFunction[6] =
{
   PXUSDCSectionTokensLoad,
   PXUSDCSectionStringsLoad,
   PXUSDCSectionFields,
   PXUSDCSectionFieldSets,
   PXUSDCSectionSpecs,
   PXUSDCSectionPaths
};
const PXInt8U PXUSDBinaryTokenListAmount = sizeof(PXUSDBinaryTokenListsize) / sizeof(PXInt8U);




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
    PXUSD* pxUSD = 0;
    PXBool isPrime = !pxResourceLoadInfo->ResourceLoadContainer;
    PXSize offset = 0;

    if(isPrime)
    {
        pxResourceLoadInfo->ResourceLoadContainer = PXMemoryHeapCallocT(PXUSD, 1);   
    }

    pxUSD = pxResourceLoadInfo->ResourceLoadContainer;

    offset = pxUSD->Text.EntryAmount;

    // Lexer
    {
        PXClear(PXCompiler, &pxCompiler);
        pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
        pxCompiler.ReadInfo.FileCache = &tokenSteam;
        pxCompiler.Flags = PXCompilerKeepAnalyseTypes;

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

        pxUSD->Text.Version = pxCompiler.ReadInfo.SymbolEntryCurrent.F32;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDAName,
            "Parsing",
            "Version: %f",
            pxUSD->Text.Version
        );
#endif

        PXCompilerSymbolEntryForward(&pxCompiler);
    }


    // Prealloc
    if(pxUSD->Text.EntryAmount < 10)
    {
        pxUSD->Text.EntryAmount = 0;
        pxUSD->Text.EntryList = PXMemoryHeapCallocT(PXUSDEntry, pxCompiler.SymbolsRead / 50);
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
                PXUSDParsePropertysScene(&pxUSD->Text, &pxCompiler);
                break;
            }
            case PXCompilerSymbolLexerGeneric:
            {
                const PXBool isDef = PXCompilerEnsureTextAndCompare(&pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

                if(isDef)
                {            
                    PXUSDAParseElementDefine(&pxUSD->Text, &pxCompiler);
                }

                break;
            }

            default:
                break;
        }
    }


    // We parsed the whole file, now we need to load 
    // all sup-data elements that are references
 
#if 1
    for(PXSize i = offset; i < pxUSD->Text.EntryAmount; ++i)
    {
        PXUSDEntry* const pxUSDEntry = &pxUSD->Text.EntryList[i];

        if(!pxUSDEntry->Include)
        {
            continue;
        }


        // Get actual path
        PXText subFile;
        PXText resultPath;

        PXTextConstructBufferA(&resultPath, 260);
        PXTextConstructFromAdressA(&subFile, pxUSDEntry->Include, pxUSDEntry->IncludeSize, pxUSDEntry->IncludeSize);

        PXFilePathRelativeFromFile(pxResourceLoadInfo->FileReference, &subFile, &resultPath);

#if PXLogEnable
        char nameBuffer[64];

        PXTextCopyA(pxUSDEntry->Name, pxUSDEntry->NameSize, nameBuffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDAName,
            "Parsing",
            "Element detected! (%3i/%3i)\n"
            "%20s : %s\n"
            "%20s : %s\n"
            "%20s : %10.4lf %10.4lf %10.4lf\n"
            "%20s : %10.4f %10.4f %10.4f\n"
            "%20s : %10.4f %10.4f %10.4f",
            i+1, 
            pxUSD->Text.EntryAmount,
            "Name", nameBuffer,
            "Include", resultPath.TextA,
            "Position", pxUSDEntry->Position[0], pxUSDEntry->Position[1], pxUSDEntry->Position[2],
            "Rotation", pxUSDEntry->Rotation[0], pxUSDEntry->Rotation[1], pxUSDEntry->Rotation[2],
            "Scale", pxUSDEntry->Scale[0], pxUSDEntry->Scale[1], pxUSDEntry->Scale[2]
        );
#endif

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.ResourceLoadContainer = pxUSD;

        PXResourceLoad(&pxResourceTransphereInfo, &resultPath);
    }
#endif



    return PXActionSuccessful;
}

PXActionResult PXAPI PXUSDCSectionTokensLoad(PXFile* const pxFile, PXTOCSectionTokens* const pxTOCSectionTokens)
{
    PXFileReadI64U(pxFile, &pxTOCSectionTokens->SizeUncompressed);
    PXFileReadI64U(pxFile, &pxTOCSectionTokens->SizeCompressed);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDCName,
        "Load",
        "Tokens\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %i",
        "NumberOfTokens", pxTOCSectionTokens->NumberOfTokens,
        "SizeCompressed", pxTOCSectionTokens->SizeCompressed,
        "SizeUncompressed", pxTOCSectionTokens->SizeUncompressed
    );
#endif



    PXFile pxFileCompressed;
    PXFile pxFileUncompressed;

    {
        PXFileOpenInfo pxFileCompressedInfo;
        PXClear(PXFileOpenInfo, &pxFileCompressedInfo);

        pxFileCompressedInfo.AccessMode = PXAccessModeReadOnly;
        pxFileCompressedInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCompressedInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileCompressedInfo.BufferData = PXFileCursorPosition(pxFile);
        pxFileCompressedInfo.BufferSize = pxTOCSectionTokens->SizeCompressed;
        PXFileOpen(&pxFileCompressed, &pxFileCompressedInfo);

        // Load B
        PXClear(PXFileOpenInfo, &pxFileCompressedInfo);

        pxFileCompressedInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileCompressedInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCompressedInfo.FlagList = PXFileIOInfoFileVirtual;
        pxFileCompressedInfo.FileSizeRequest = pxTOCSectionTokens->SizeUncompressed;
        PXFileOpen(&pxFileUncompressed, &pxFileCompressedInfo);
    }

    PXActionResult uncompressResult = PXLZ4Decompress(&pxFileCompressed, &pxFileUncompressed);

#if PXLogEnable
    PXSize offset = 0;

    for(PXSize i = 0; i < pxTOCSectionTokens->NumberOfTokens; ++i)
    {
        char* text = &((char*)pxFileUncompressed.Data)[offset];

        PXSize length = PXTextLengthA(text, -1);

        offset += length+1;

        PXLogPrint
        (
            PXLoggingInfo,
            PXUSDCName,
            "Load",
            "%3i/%3i - %s",
            i+1,
            pxTOCSectionTokens->NumberOfTokens,
            text
        );
    }
#endif

    PXConsoleWrite(0, 0);

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

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDCName,
        "Load",
        "Binary format detected"
    );
#endif

    char versionString[8];

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
                "Dection: %i/%i, %10s, Start:%7i, Size:%i",
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

            const PXInt8U index = PXTextCompareAVI8(pxTOCSection->Name, PXTextUnkownLength, PXUSDBinaryTokenListData, PXUSDBinaryTokenListsize, PXUSDBinaryTokenListAmount);

            if(-1 == index)
            {
                // Not registerd, cant be handled
                continue;
            }

            PXFileCursorMoveTo(pxFile, pxTOCSection->OffsetStart);

            PXFileReadI64U(pxFile, &pxTOCSection->Tokens.NumberOfTokens);

            PXUSDBinaryTokenListFunction[index](pxFile, pxTOCSection->Data);

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





void PXAPI PXUSDParsePropertysScene(PXUSDA* const pxUSDA, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseElementDefine(PXUSDA* const pxUSDA, PXCompiler* const pxCompiler)
{
    PXUSDEntry* const pxUSDEntry = &pxUSDA->EntryList[pxUSDA->EntryAmount++];
        
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
                    pxUSDEntry->Flags |= PXUSDEntryXFORM;

                    PXCompilerSymbolEntryForward(pxCompiler);

                    PXUSDAParseEntryName(pxUSDA, pxUSDEntry, pxCompiler);
                }
                else
                {
                    const PXBool isDef = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

                    if(isDef)
                    {
                        PXUSDAParseElementDefine(pxUSDA, pxCompiler);
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
                PXUSDAParseEntryName(pxUSDA, pxUSDEntry, pxCompiler);

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




void PXAPI PXUSDAParseEntryParameter(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
{
    const PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);

    for(;;)
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        if(PXCompilerSymbolLexerEndOfFile == pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
            return;
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
                const PXInt8U index = PXCompilerEnsureTextListAndCompare
                (
                    pxCompiler, 
                    PXUSDAParseEntryParameterListText,
                    PXUSDAParseEntryParameterListSize,
                    PXUSDAParseEntryParameterListAmount
                );

                if(0xFF == index)
                {
                    break;
                }

                PXUSDAParseEntryParameterListFunction[index](pxUSDEntry, pxCompiler);

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







void PXAPI PXUSDAParseEntryProperty(PXUSDA* const pxUSDA, PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
{
    const PXBool isOpen = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);

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
                const PXInt8U index = PXCompilerEnsureTextListAndCompare
                (
                    pxCompiler,
                    PXUSDAParseEntryPropertyListText,
                    PXUSDAParseEntryPropertyListSize,
                    PXUSDAParseEntryPropertyListAmount
                );

                if(0xFF == index)
                {
                    break;
                }

                if(0 == index)
                {
                    PXUSDAParseEntryPropertyListFunction[0](pxUSDA, pxCompiler); 
                    break;
                }

                PXUSDAParseEntryPropertyListFunction[index](pxUSDEntry, pxCompiler);                

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

void PXAPI PXUSDAParseXMLEndTag(PXCompiler* const pxCompiler, char** const name, PXSize* const nameSize)
{
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketAngleOpen);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerSlash);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric); // Name but not string

    *name = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    *nameSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

    PXCompilerSymbolEntryForward(pxCompiler);
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketAngleClose);
    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDAParseEntryParameterPrepend(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // remove prepend

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


    // In some files, one payload file, this ends with a XML token. For some reason
    const PXBool isOpenAngleBracked = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketAngleOpen);

    if(isOpenAngleBracked)
    {
        char* name = 0;
        PXSize nameSize = 0;

        PXUSDAParseXMLEndTag(pxCompiler, &name, &nameSize);
    }
}

void PXAPI PXUSDAParseEntryParameterVarriants(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseEntryParameterAssetInfo(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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
            PXBool isName = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextName, PXUSDTextNameSize);

            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);

            if(isName)
            {

            }
            else if(isIdentifier)
            {
         

                // identifier


            }

            PXCompilerSymbolEntryForward(pxCompiler);
        }
        else if(isAsset)
        {
            PXCompilerSymbolEntryForward(pxCompiler);

            PXBool isIdentifier = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextidentifier, PXUSDTextidentifierSize);
            PXBool isName = PXCompilerEnsureTextAndCompare(pxCompiler, PXUSDTextName, PXUSDTextNameSize);


            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);

            if(isName)
            {

            }else if(isIdentifier)
            {

            }

            PXCompilerSymbolEntryForward(pxCompiler);

         
        }
    }


    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove '}'
}

void PXAPI PXUSDAParseEntryParameterPayLoad(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    pxUSDEntry->Include = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    pxUSDEntry->IncludeSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

    PXCompilerSymbolEntryForward(pxCompiler);


    char* name = 0;
    PXSize nameSize = 0;

    PXUSDAParseXMLEndTag(pxCompiler, &name, &nameSize);
}

void PXAPI PXUSDAParseEntryParameterKind(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDAParseEntryPropertyFloat1(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseEntryPropertyFloat3(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseEntryPropertyDouble3(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseEntryPropertyUniform(PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

void PXAPI PXUSDAParseEntryName(PXUSDA* const pxUSDA, PXUSDEntry* const pxUSDEntry, PXCompiler* const pxCompiler)
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

    PXUSDAParseEntryParameter(pxUSDEntry, pxCompiler);
    PXUSDAParseEntryProperty(pxUSDA, pxUSDEntry, pxCompiler);
}






























PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}













