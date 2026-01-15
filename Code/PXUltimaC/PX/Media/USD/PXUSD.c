#include "PXUSD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/LZ4/PXLZ4.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/PXECS.h>

// Logging
const char PXUSDName[] = "USD";
const char PXUSDAName[] = "Text-USD";
const char PXUSDCName[] = "Binary-USD";

// Text format
const char PXUSDTextSignature[4] = "usda";
const PXI8U PXUSDTextSignatureSize = sizeof(PXUSDTextSignature);

const char PXUSDTextDef[3] = "def";
const PXI8U PXUSDTextDefSize = sizeof(PXUSDTextDef);

const char PXUSDTextXform[5] = "Xform";
const PXI8U PXUSDTextXformSize = sizeof(PXUSDTextXform);

const char PXUSDTextprepend[7] = "prepend";
const PXI8U PXUSDTextprependSize = sizeof(PXUSDTextprepend);

const char PXUSDTextString[6] = "string";
const PXI8U PXUSDTextStringSize = sizeof(PXUSDTextString);

const char PXUSDAsset[5] = "asset";
const PXI8U PXUSDAssetSize = sizeof(PXUSDAsset);


const char PXUSDTextidentifier[10] = "identifier";
const PXI8U PXUSDTextidentifierSize = sizeof(PXUSDTextidentifier);

const char PXUSDTextName[4] = "name";
const PXI8U PXUSDTextNameSize = sizeof(PXUSDTextName);



const char PXUSDTextmodelingVariant[15] = "modelingVariant";
const PXI8U PXUSDTextmodelingVariantSize = sizeof(PXUSDTextmodelingVariant);


const char PXUSDTextvariants[8] = "variants";
const PXI8U PXUSDTextvariantsSize = sizeof(PXUSDTextvariants);

const char PXUSDAssetInfo[9] = "assetInfo";
const PXI8U PXUSDAssetInfoSize = sizeof(PXUSDAssetInfo);


const char PXUSDTextdefaultPrim[11] = "defaultPrim";
const PXI8U PXUSDTextdefaultPrimSize = sizeof(PXUSDTextdefaultPrim);

const char PXUSDTextupAxis[6] = "upAxis";
const PXI8U PXUSDTextupAxisSize = sizeof(PXUSDTextupAxis);

const char PXUSDTextkind[4] = "kind";
const PXI8U PXUSDTextkindSize = sizeof(PXUSDTextkind);

const char PXUSDTextPayload[7] = "payload";
const PXI8U PXUSDTextPayloadSize = sizeof(PXUSDTextPayload);


const char PXUSDTextreferences[10] = "references";
const PXI8U PXUSDTextreferencesSize = sizeof(PXUSDTextreferences);

const char PXUSDTextFloat[5] = "float";
const PXI8U PXUSDTextFloatSize = sizeof(PXUSDTextFloat);

const char PXUSDTextFloat3[6] = "float3";
const PXI8U PXUSDTextFloat3Size = sizeof(PXUSDTextFloat3);

const char PXUSDTextdouble3[7] = "double3";
const PXI8U PXUSDTextdouble3Size = sizeof(PXUSDTextdouble3);

const char PXUSDTextuniform[7] = "uniform";
const PXI8U PXUSDTextuniformSize = sizeof(PXUSDTextuniform);


const char PXUSDTextXFormOp[7] = "xformOp";
const PXI8U PXUSDTextXFormOpSize = sizeof(PXUSDTextXFormOp);


const char PXUSDTextScale[5] = "scale";
const PXI8U PXUSDTextScaleSize = sizeof(PXUSDTextScale);

const char PXUSDTextXrotateX[7] = "rotateX";
const PXI8U PXUSDTextXrotateXSize = sizeof(PXUSDTextXrotateX);

const char PXUSDTextXrotateZ[7] = "rotateZ";
const PXI8U PXUSDTextXrotateZSize = sizeof(PXUSDTextXrotateZ);

const char PXUSDTextXrotateXYZ[9] = "rotateXYZ";
const PXI8U PXUSDTextXrotateXYZSize = sizeof(PXUSDTextXrotateXYZ);

const char PXUSDTexttranslate[9] = "translate";
const PXI8U PXUSDTexttranslateSize = sizeof(PXUSDTexttranslate);


const char PXUSDTexttoken[5] = "token";
const PXI8U PXUSDTexttokenSize = sizeof(PXUSDTexttoken);


const char PXUSDTextXFormOpOrder[12] = "xformOpOrder";
const PXI8U PXUSDTextXFormOpOrderSize = sizeof(PXUSDTextXFormOpOrder);




typedef void (PXAPI* PXUSDAParseElementFunction)(void PXREF pxUSDEntry, PXCompiler PXREF pxCompiler);



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
    PXUSDAssetInfo,
    PXUSDTextkind,
    PXUSDTextPayload
};

const PXI8U PXUSDAParseEntryParameterListSize[5] =
{
    sizeof(PXUSDTextprepend),
    sizeof(PXUSDTextvariants),
    sizeof(PXUSDAssetInfo),
    sizeof(PXUSDTextkind),
    sizeof(PXUSDTextPayload)
};
const PXI8U PXUSDAParseEntryParameterListAmount = sizeof(PXUSDAParseEntryParameterListSize) / sizeof(PXI8U);











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

const PXI8U PXUSDAParseEntryPropertyListSize[5] =
{
    sizeof(PXUSDTextDef),
    sizeof(PXUSDTextFloat),
    sizeof(PXUSDTextFloat3),
    sizeof(PXUSDTextdouble3),
    sizeof(PXUSDTextuniform)
};
const PXI8U PXUSDAParseEntryPropertyListAmount = sizeof(PXUSDAParseEntryPropertyListSize) / sizeof(PXI8U);






// Binary Format
const char PXUSDBinarySignature[8] = "PXR-USDC";
const char PXUSDBinaryTOKENS[6] = "TOKENS";
const char PXUSDBinarySTRINGS[7] = "STRINGS";
const char PXUSDBinaryFields[6] = "FIELDS";
const char PXUSDBinarFIELDSETSy[9] = "FIELDSETS";
const char PXUSDBinarySPECS[5] = "SPECS";
const char PXUSDBinaryPATHS[5] = "PATHS";

typedef PXResult(PXAPI* PXUSDLoadFunction)(PXResourceMoveInfo PXREF pxResourceLoadInfo);
typedef PXResult(PXAPI* PXUSDSegmentLoadFunction)(PXFile PXREF pxFile, void* object);

const char* PXUSDBinaryTokenListData[6] =
{
    PXUSDBinaryTOKENS,
    PXUSDBinarySTRINGS,
    PXUSDBinaryFields,
    PXUSDBinarFIELDSETSy,
    PXUSDBinarySPECS,
    PXUSDBinaryPATHS
};
const PXI8U PXUSDBinaryTokenListsize[6] =
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
const PXI8U PXUSDBinaryTokenListAmount = sizeof(PXUSDBinaryTokenListsize) / sizeof(PXI8U);




PXResult PXAPI PXUSDLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference; 
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

    char* text = PXFileDataAtCursor(pxFile);

    switch(*text) // Unsafe
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

    const PXResult result = pxUSDLoadFunction(pxResourceLoadInfo);


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






PXResult PXAPI PXUSDCLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXUSD pxUSD;
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

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
            PXTOCSection PXREF pxTOCSection = &pxUSD.Binary.TOCSectionList[i];

            PXFileReadA(pxFile, pxTOCSection->Name, 16);
            PXFileReadI64U(pxFile, &pxTOCSection->OffsetStart);
            PXFileReadI64U(pxFile, &pxTOCSection->BlockSize);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXUSDCName,
                "Load",
                "Dection: %i/%i, %10s, Start:%7i, Size:%i",
                i + 1,
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
            PXTOCSection PXREF pxTOCSection = &pxUSD.Binary.TOCSectionList[i];

            const PXI8U index = PXTextCompareAVI8(pxTOCSection->Name, PXTextUnkownLength, PXUSDBinaryTokenListData, PXUSDBinaryTokenListsize, PXUSDBinaryTokenListAmount);

            if(-1 == index)
            {
                // Not registerd, cant be handled
                continue;
            }

            PXFileCursorMoveTo(pxFile, pxTOCSection->OffsetStart);

            PXFileReadI64U(pxFile, &pxTOCSection->Tokens.NumberOfTokens);

            PXUSDBinaryTokenListFunction[index](pxFile, pxTOCSection->Data);    

            const PXBool isAlligned = PXFileCursorIsAt(pxFile, pxTOCSection->OffsetStart + pxTOCSection->BlockSize);
        }
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDCName,
        "Load",
        "DONE"
    );
#endif


    /*
        PXSize TOCSectionCounterTokens;
    PXSize TOCSectionCounterPaths;
    PXSize TOCSectionCounterStrings;
    PXSize TOCSectionCounterFields;
    PXSize TOCSectionCounterFieldsets;
    PXSize TOCSectionCounterSpecs;
    */

    return PXResultOK;
}


PXResult PXAPI PXUSDCSectionTokensLoad(PXFile PXREF pxFile, PXTOCSectionTokens PXREF pxTOCSectionTokens)
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



    PXFile* pxFileCompressed = PXFileCreate();
    PXFile* pxFileUncompressed = PXFileCreate();

    {
        PXFileOpenInfo pxFileCompressedInfo;
        PXClear(PXFileOpenInfo, &pxFileCompressedInfo);

        pxFileCompressedInfo.AccessMode = PXAccessModeReadOnly;
        pxFileCompressedInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCompressedInfo.FlagList = PXFileIOInfoFileMemory;

        PXBufferSet(&pxFileCompressedInfo.Data, PXFileDataAtCursor(pxFile), pxTOCSectionTokens->SizeCompressed);

        PXFileOpen(pxFileCompressed, &pxFileCompressedInfo);

        // Load B
        PXClear(PXFileOpenInfo, &pxFileCompressedInfo);

        pxFileCompressedInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileCompressedInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCompressedInfo.FlagList = PXFileIOInfoFileVirtual;
        pxFileCompressedInfo.FileSizeRequest = pxTOCSectionTokens->SizeUncompressed;
        PXFileOpen(pxFileUncompressed, &pxFileCompressedInfo);
    }

    PXResult uncompressResult = PXLZ4Decompress(pxFileCompressed, pxFileUncompressed);

#if PXLogEnable
    PXSize offset = 0;

    const char* ddata = (char*)PXFileDataAtCursor(pxFileUncompressed);

    for(PXSize i = 0; i < pxTOCSectionTokens->NumberOfTokens; ++i)
    {
        const char* text = &ddata[offset];
        const PXSize length = PXTextLengthA(text, -1);

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

PXResult PXAPI PXUSDCReadCompressedInts(PXFile PXREF input,  PXFile PXREF output, size_t num_ints)
{
    // Check for max size
    if(num_ints > 0xFFFFFFFF) 
    {
        return PXResultInvalid;
    }

    PXI64U compSize;
    PXFileReadI64U(input, &compSize);
    /*

    std::vector<char> compBuffer;
    compBuffer.resize(compBufferSize);


    PXI32U* buffer = PXMemoryHeapCallocT(PXI32U, pxTOCSectionFields->AmountOFFields);


    if(!_sr->read(size_t(compSize), size_t(compSize),
       reinterpret_cast<uint8_t*>(compBuffer.data()))) {
        PUSH_ERROR_AND_RETURN_TAG(kTag, "Failed to read compressedInts.");
    }

    bool ret = Compressor::DecompressFromBuffer(
        compBuffer.data(), size_t(compSize), out, num_ints, &_err);

    REDUCE_MEMORY_USAGE(compBufferSize);
    */

    return PXResultOK;
}

PXResult PXAPI PXUSDCSectionStringsLoad(PXFile PXREF pxFile, PXTOCSectionStrings PXREF pxTOCSectionStrings)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parse-Section",
        "Strings"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXUSDCSectionFields(PXFile PXREF pxFile, PXTOCSectionFields PXREF pxTOCSectionFields)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parse-Section",
        "Fields"
    );
#endif

    pxTOCSectionFields->FieldList = PXMemoryHeapCallocT(PXI32U, pxTOCSectionFields->AmountOFFields);



    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXUSDCSectionFieldSets(PXFile PXREF pxFile, PXTOCSectionFieldSets PXREF pxTOCSectionFieldSets)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parse-Section",
        "FieldSets"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXUSDCSectionSpecs(PXFile PXREF pxFile, PXTOCSectionSpecs PXREF pxTOCSectionSpecs)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parse-Section",
        "Specs"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXUSDCSectionPaths(PXFile PXREF pxFile, PXTOCSectionPaths PXREF pxTOCSectionPaths)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUSDAName,
        "Parse-Section",
        "Paths"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXUSDZLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{

}

PXResult PXAPI PXUSDALoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXCompiler pxCompiler;
    PXFile* tokenSteam = PXFileCreate();
    PXUSD* pxUSD = 0;
    PXBool isPrime = !pxResourceLoadInfo->ResourceLoadContainer;
    PXSize offset = 0;

    if(isPrime)
    {
        pxResourceLoadInfo->ResourceLoadContainer = PXMemoryHeapCallocT(PXUSD, 1);
    }

    pxUSD = (PXUSD*)pxResourceLoadInfo->ResourceLoadContainer;

    offset = pxUSD->Text.EntryAmount;

    // Lexer
    {
        PXClear(PXCompiler, &pxCompiler);
        pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
        pxCompiler.ReadInfo.FileCache = tokenSteam;
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
        PXBool isusda = PXCompilerEnsureAndCompare(&pxCompiler, PXUSDTextSignature, PXUSDTextSignatureSize);

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
                const PXBool isDef = PXCompilerEnsureAndCompare(&pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

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
        PXUSDEntry PXREF pxUSDEntry = &pxUSD->Text.EntryList[i];

        if(!pxUSDEntry->Included)
        {
            continue;
        }


        // Get actual path
        PXText subFile;
        PXText resultPath;

        PXTextConstructBufferA(&resultPath, 260);
        PXTextFromAdressA(&subFile, pxUSDEntry->Included, pxUSDEntry->IncludedSize, pxUSDEntry->IncludedSize);

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
            i + 1,
            pxUSD->Text.EntryAmount,
            "Name", nameBuffer,
            "Included", resultPath.A,
            "Position", pxUSDEntry->Position[0], pxUSDEntry->Position[1], pxUSDEntry->Position[2],
            "Rotation", pxUSDEntry->Rotation[0], pxUSDEntry->Rotation[1], pxUSDEntry->Rotation[2],
            "Scale", pxUSDEntry->Scale[0], pxUSDEntry->Scale[1], pxUSDEntry->Scale[2]
        );
#endif

        PXResourceMoveInfo PXResourceMoveInfo;
        PXClear(PXResourceMoveInfo, &PXResourceMoveInfo);
        PXResourceMoveInfo.ResourceLoadContainer = pxUSD;

        PXResourceLoad(&PXResourceMoveInfo, &resultPath);
    }
#endif



    return PXResultOK;
}

void PXAPI PXUSDParsePropertysScene(PXUSDA PXREF pxUSDA, PXCompiler PXREF pxCompiler)
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

void PXAPI PXUSDAParseElementDefine(PXUSDA PXREF pxUSDA, PXCompiler PXREF pxCompiler)
{
    PXUSDEntry PXREF pxUSDEntry = &pxUSDA->EntryList[pxUSDA->EntryAmount++];

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
                const PXBool isxform = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXform, PXUSDTextXformSize);

                if(isxform)
                {
                    pxUSDEntry->Flags |= PXUSDEntryXFORM;

                    PXCompilerSymbolEntryForward(pxCompiler);

                    PXUSDAParseEntryName(pxUSDA, pxUSDEntry, pxCompiler);
                }
                else
                {
                    const PXBool isDef = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextDef, PXUSDTextDefSize);

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

void PXAPI PXUSDAParseEntryParameter(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
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
                const PXI8U index = PXCompilerEnsureTextListAndCompare
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

void PXAPI PXUSDAParseEntryProperty(PXUSDA PXREF pxUSDA, PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
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
                const PXI8U index = PXCompilerEnsureTextListAndCompare
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

void PXAPI PXUSDAParseXMLEndTag(PXCompiler PXREF pxCompiler, char* PXREF name, PXSize PXREF nameSize)
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

void PXAPI PXUSDAParseEntryParameterPrepend(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // remove prepend

    PXSize pathSize = 0;

    const PXBool isReference = PXCompilerEnsurePropertyText
    (
        pxCompiler,
        PXUSDTextreferences,
        PXUSDTextreferencesSize,
        &pxUSDEntry->Included,
        &pathSize
    );

    pxUSDEntry->IncludedSize = pathSize;


    // In some files, one payload file, this ends with a XML token. For some reason
    const PXBool isOpenAngleBracked = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketAngleOpen);

    if(isOpenAngleBracked)
    {
        char* name = 0;
        PXSize nameSize = 0;

        PXUSDAParseXMLEndTag(pxCompiler, &name, &nameSize);
    }
}

void PXAPI PXUSDAParseEntryParameterVarriants(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
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

        PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextString, PXUSDTextStringSize);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextmodelingVariant, PXUSDTextmodelingVariantSize);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =

        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);
        PXCompilerSymbolEntryForward(pxCompiler); // rmeove =
    }

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);
    PXCompilerSymbolEntryForward(pxCompiler); // rmeove '}'
}

void PXAPI PXUSDAParseEntryParameterAssetInfo(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
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


        PXBool isString = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextString, PXUSDTextStringSize);
        PXBool isAsset = PXCompilerEnsureAndCompare(pxCompiler, PXUSDAsset, PXUSDAssetSize);


        if(isString)
        {
            PXCompilerSymbolEntryForward(pxCompiler);

            PXBool isIdentifier = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextidentifier, PXUSDTextidentifierSize);
            PXBool isName = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextName, PXUSDTextNameSize);

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

            PXBool isIdentifier = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextidentifier, PXUSDTextidentifierSize);
            PXBool isName = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextName, PXUSDTextNameSize);


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

void PXAPI PXUSDAParseEntryParameterPayLoad(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerString);

    pxUSDEntry->Included = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    pxUSDEntry->IncludedSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

    PXCompilerSymbolEntryForward(pxCompiler);


    char* name = 0;
    PXSize nameSize = 0;

    PXUSDAParseXMLEndTag(pxCompiler, &name, &nameSize);
}

void PXAPI PXUSDAParseEntryParameterKind(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler);
}

void PXAPI PXUSDAParseEntryPropertyFloat1(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // float3

    const PXBool iXFormOp = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(!iXFormOp)
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler); // remove op
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
    PXCompilerSymbolEntryForward(pxCompiler);

    const PXBool isRoateX = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXrotateX, PXUSDTextXrotateXSize);
  //  const PXBool isRoateY = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXrotateX, PXUSDTextXrotateXSize);
    const PXBool isRoateZ = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXrotateZ, PXUSDTextXrotateZSize);
      
    PXCompilerSymbolEntryForward(pxCompiler);
    PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
    PXCompilerSymbolEntryForward(pxCompiler);

    PXCompilerSymbolLexer pxCompilerSymbolLexer[2] = { PXCompilerSymbolLexerReal, PXCompilerSymbolLexerNumeric };

    PXCompilerSymbolEntryEnsureCheckList(pxCompiler, pxCompilerSymbolLexer, 2);

    if(pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerReal)
    {
        if(isRoateX)
        {
#if OS64B
            pxUSDEntry->Rotation[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
            pxUSDEntry->Rotation[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif      
        }
        else if(isRoateZ)
        {
#if OS64B
            pxUSDEntry->Rotation[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.F64;
#else
            pxUSDEntry->Rotation[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.F32;
#endif

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

void PXAPI PXUSDAParseEntryPropertyFloat3(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // float3

    const PXBool iXFormOp = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(iXFormOp)
    {
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isRoate = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXrotateXYZ, PXUSDTextXrotateXYZSize);
        const PXBool isScale = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextScale, PXUSDTextScaleSize);

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

void PXAPI PXUSDAParseEntryPropertyDouble3(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // double3

    const PXBool iXFormOp = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXFormOp, PXUSDTextXFormOpSize);

    if(iXFormOp)
    {
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerColon);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isTranslate = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTexttranslate, PXUSDTexttranslateSize);

        if(isTranslate)
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerEqual);
            PXCompilerSymbolEntryForward(pxCompiler);
            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);
            PXCompilerSymbolEntryForward(pxCompiler);   

            PXCompilerParseCSVF64(pxCompiler, pxUSDEntry->Position, 3);

            PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBrackedRoundClose);
            PXCompilerSymbolEntryForward(pxCompiler);
        }
    }

    //  PXCompilerEnsureAndCompare(pxCompiler, );
}

void PXAPI PXUSDAParseEntryPropertyUniform(PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
{
    PXCompilerSymbolEntryForward(pxCompiler); // uniform

    const PXBool isToken = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTexttoken, PXUSDTexttokenSize);

    if(isToken)
    {
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);
        PXCompilerSymbolEntryForward(pxCompiler);
        PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareClose);
        PXCompilerSymbolEntryForward(pxCompiler);

        const PXBool isOpOrder = PXCompilerEnsureAndCompare(pxCompiler, PXUSDTextXFormOpOrder, PXUSDTextXFormOpOrderSize);

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

void PXAPI PXUSDAParseEntryName(PXUSDA PXREF pxUSDA, PXUSDEntry PXREF pxUSDEntry, PXCompiler PXREF pxCompiler)
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






PXResult PXAPI PXUSDSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}