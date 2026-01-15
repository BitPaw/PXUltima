#include "PXRedshiftMesh.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/PXOS.h>

const char PXR3D2Text[] = "R3D2";
const char PXR3D2TextText[] = "Text-R3D2";
const char PXR3D2BinaryText[] = "Binary-R3D2";

const char PXR3D2HeaderSignature[8] = "r3d2Mesh";
const PXI8U PXR3D2HeaderSignatureLength = sizeof(PXR3D2HeaderSignature);


const PXI32U PXR3D2HeaderList[] =
{
    PXTypeDatax8,
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeText(128),
    PXTypeInt32U,
    PXTypeInt32U,
    PXTypeInt32U,
    PXTypeF32List(3),
    PXTypeF32List(3)
};
const PXI8U PXR3D2HeaderListSize = sizeof(PXR3D2HeaderList) / sizeof(PXI32U);




PXResult PXAPI PXR3D2LoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXR3D2 pxR3D2;
    PXClear(PXR3D2, &pxR3D2); 

    PXResult pxLoadResult;

    switch(PXFileFormatVarriantMask & pxResourceLoadInfo->FormatInfo.Flags)
    {
        case PXFileFormatVarriantBinary:
        {
            pxLoadResult = PXR3D2ParseBinary(&pxR3D2, pxResourceLoadInfo);
            break;
        }
        case PXFileFormatVarriantText:
        {
            pxLoadResult = PXR3D2ParseText(&pxR3D2, pxResourceLoadInfo);
            break;
        }
        default:
            return PXResultInvalid;
    }

    if(PXResultOK != pxLoadResult)
    {
        return pxLoadResult;
    }





#if 0

    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;
    char name[260];

    PXTextPrintA(name, 260, "%s.%s", pxFile->FilePath.A, "obj");



    FILE* file = fopen(name, "wb");

    fprintf
    (
        file,
        "# [PX] Wavefront (OBJ) Writer\n"
       // "o MODEL\n",
      //  pxFile->FilePathData
    );

    fprintf(file, "\n");

    // Write normals
    for(size_t i = 0; i < pxR3D2.Header.VertexCount; i++)
    {
        PXVector3F32* vertex = &pxR3D2.VertexData[i];

        fprintf(file, "v %.6f %.6f %.6f\n", vertex->X, vertex->Y, vertex->Z);
    }

    fprintf(file, "\n");

    // Write texture positions
    for(size_t i = 0; i < pxR3D2.Header.IndexCount; i++)
    {
        R3D2MeshStaticFace PXREF r3D2MeshStaticFace = &pxR3D2.StaticFaceList[i];

        fprintf
        (
            file, 
            "vt %.6f %.6f\n"
            "vt %.6f %.6f\n"
            "vt %.6f %.6f\n",
            r3D2MeshStaticFace->UV[0].X, 
            r3D2MeshStaticFace->UV[0].Y,

            r3D2MeshStaticFace->UV[1].X,
            r3D2MeshStaticFace->UV[1].Y,

            r3D2MeshStaticFace->UV[2].X,
            r3D2MeshStaticFace->UV[2].Y
        );
    }

    fprintf(file, "\ns 0\n");

    for(size_t i = 0; i < pxR3D2.Header.IndexCount; i++)
    {
        R3D2MeshStaticFace PXREF r3D2MeshStaticFace = &pxR3D2.StaticFaceList[i];

        //fprintf(file, "f %i %i %i\n", r3D2MeshStaticFace->VertexID[0], r3D2MeshStaticFace->VertexID[1], r3D2MeshStaticFace->VertexID[2]);

        fprintf
        (
            file, 
            "f %i/%i %i/%i %i/%i\n", 
            1 + (r3D2MeshStaticFace->VertexID[0]),
            1 + (i * 3 + 0),
            1 + (r3D2MeshStaticFace->VertexID[1]),
            1 + (i * 3 + 1),
            1 + (r3D2MeshStaticFace->VertexID[2]),
            1 + (i * 3 + 2)
        );
    }


    fclose(file);
#endif

    return PXResultOK;
}

PXResult PXAPI PXR3D2ParseBinary(PXR3D2 PXREF pxR3D2, PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

    PXCopyList(char, PXR3D2HeaderSignatureLength, PXR3D2HeaderSignature, pxR3D2->Header.Signature);

    const PXSize amount = PXFileBinding
    (
        pxFile,
        &pxR3D2->Header,
        PXR3D2HeaderList,
        PXR3D2HeaderListSize,
        PXFileBindingRead
    );

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXR3D2Text,
        "Load",
        "Header\n"
        "%20s : %i.%i\n"
        "%20s : %i\n"
        "%20s : %i\n"
        "%20s : %s",
        "Version", pxR3D2->Header.VersionMajor, pxR3D2->Header.VersionMinor,
        "VertexCount", pxR3D2->Header.VertexCount,
        "IndexCount", pxR3D2->Header.IndexCount,
        "SourcePath", pxR3D2->Header.SourcePath
    );
#endif

    if(0 == pxR3D2->Header.VersionMajor || pxR3D2->Header.VersionMajor >= 4)
    {
        return PXActionFailedFormatNotAsExpected;
    }


    const PXBool doCheckForVertexColors = pxR3D2->Header.VersionMajor >= 3 && pxR3D2->Header.VersionMinor >= 2;
    const PXBool hasVertexColors = PXFalse;

    if(doCheckForVertexColors)
    {
        PXFileReadI32U(pxFile, &hasVertexColors);
    }


    // Read vertices
    pxR3D2->VertexData = PXMemoryHeapCallocT(PXVector3F32, pxR3D2->Header.VertexCount);
    PXFileReadB(pxFile, pxR3D2->VertexData, pxR3D2->Header.VertexCount * sizeof(PXVector3F32));

    // Read vertex colors
    if(hasVertexColors)
    {
        pxR3D2->VertexColors = PXMemoryHeapCallocT(PXVector3F32, pxR3D2->Header.VertexCount);

        PXFileReadB(pxFile, pxR3D2->VertexColors, pxR3D2->Header.VertexCount * sizeof(PXColorRGBAI8));
    }

    PXFileReadB(pxFile, &pxR3D2->CentralPoint, sizeof(PXVector3F32));


    // Read faces
    pxR3D2->StaticFaceList = PXMemoryHeapCallocT(R3D2MeshStaticFace, pxR3D2->Header.IndexCount);


    for(PXI32U i = 0; i < pxR3D2->Header.IndexCount; i++)
    {
        R3D2MeshStaticFace PXREF r3D2MeshStaticFace = &pxR3D2->StaticFaceList[i];

        PXFileReadB(pxFile, r3D2MeshStaticFace, sizeof(R3D2MeshStaticFace) - 3 * sizeof(PXColorRGBAI8));

#if PXLogEnable && 0
        PXLogPrint
        (
            PXLoggingInfo,
            PXR3D2Text,
            "Load",
            "Face\n"
            "%20s : %9i %9i %9i\n"
            "%20s : %s\n"
            "%20s : %9.6f %9.6f\n"
            "%20s : %9.6f %9.6f %9.6f",
            "VertexID", r3D2MeshStaticFace->VertexID[0], r3D2MeshStaticFace->VertexID[1], r3D2MeshStaticFace->VertexID[2],
            "Material", r3D2MeshStaticFace->Material,
            "UV", r3D2MeshStaticFace->UV[0], r3D2MeshStaticFace->UV[1],
            "Color", r3D2MeshStaticFace->Color[0], r3D2MeshStaticFace->Color[1], r3D2MeshStaticFace->Color[2]
        );
#endif
    }

    if(PXR3D2HeaderFlagHasVcp & pxR3D2->Header.Flags)
    {
        for(PXI32U i = 0; i < pxR3D2->Header.IndexCount; i++)
        {
            R3D2MeshStaticFace PXREF r3D2MeshStaticFace = &pxR3D2->StaticFaceList[i];

            PXFileReadB(pxFile, r3D2MeshStaticFace->Color, 4 * sizeof(PXColorRGBAI8));
        }
    }


    // Do a check for index data. This is found to be NULL of all bytes!
    // A triangle of 0,0,0 is invalid.

    return PXResultOK;
}

const char PXR3D2ObjectBegin[11] = "ObjectBegin";
const PXI8U PXR3D2ObjectBeginSize = sizeof(PXR3D2ObjectBegin);

const char PXR3D2ObjectEnd[9] = "ObjectEnd";
const PXI8U PXR3D2ObjectEndSize = sizeof(PXR3D2ObjectEnd);


const char PXR3D2Name[4] = "Name";
const PXI8U PXR3D2NameSize = sizeof(PXR3D2Name);

const char PXR3D2CentralPoint[12] = "CentralPoint";
const PXI8U PXR3D2CentralPointSize = sizeof(PXR3D2CentralPoint);

const char PXR3D2PivotPoint[10] = "PivotPoint";
const PXI8U PXR3D2PivotPointSize = sizeof(PXR3D2PivotPoint);

const char PXR3D2Verts[5] = "Verts";
const PXI8U PXR3D2VertsSize = sizeof(PXR3D2Verts);


const char PXR3D2Faces[5] = "Faces";
const PXI8U PXR3D2FacesSize = sizeof(PXR3D2Faces);


const char* PXR3D2TextNameList[5] =
{
    PXR3D2Name,
    PXR3D2CentralPoint,
    PXR3D2PivotPoint,
    PXR3D2Verts,
    PXR3D2Faces
};

const PXI8U PXR3D2TextNameLengthList[5] =
{
    sizeof(PXR3D2Name),
    sizeof(PXR3D2CentralPoint),
    sizeof(PXR3D2PivotPoint),
    sizeof(PXR3D2Verts),
    sizeof(PXR3D2Faces)
};

const PXI8U PXR3D2TextNameListAmount = sizeof(PXR3D2TextNameLengthList) / sizeof(PXI8U);






PXResult PXAPI PXR3D2ParseText(PXR3D2 PXREF pxR3D2, PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    PXFile PXREF pxFile = pxResourceLoadInfo->FileReference;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXR3D2TextText,
        "Load",
        "---Start---"
    );
#endif

    PXFile* compiledSteam = PXFileCreate();

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = compiledSteam;
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXR3D2TextText,
        "Load",
        "1/4 - Lexer"
    );
#endif

    // Lexer - Level I
    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXR3D2TextText,
        "Load",
        "2/4 - Analyse"
    );
#endif

    PXBool isValid = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);

    if(!isValid)
    {
        return PXActionFailedFormatNotAsExpected;
    }

    PXCompilerSymbolEntryForward(&pxCompiler);
    isValid = PXCompilerEnsureAndCompare(&pxCompiler, PXR3D2ObjectBegin, PXR3D2ObjectBeginSize);

    if(!isValid)
    {
        return PXActionFailedFormatNotAsExpected;
    }

    PXCompilerSymbolEntryForward(&pxCompiler);
    isValid = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerBracketSquareClose);

    if(!isValid)
    {
        return PXActionFailedFormatNotAsExpected;
    }

    PXCompilerSymbolEntryForward(&pxCompiler);


    for(;;)
    {
        PXCompilerSymbolEntryPeek(&pxCompiler);

        const PXBool exit =
            PXCompilerSymbolLexerEndOfFile == pxCompiler.ReadInfo.SymbolEntryCurrent.ID ||
            PXCompilerSymbolLexerBracketSquareOpen == pxCompiler.ReadInfo.SymbolEntryCurrent.ID;

        if(exit)
        {
            break;
        }

        const PXI8U index = PXCompilerEnsureTextListAndCompare(&pxCompiler, PXR3D2TextNameList, PXR3D2TextNameLengthList, PXR3D2TextNameListAmount);

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXCompilerSymbolEntryPeek(&pxCompiler);
        isValid = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerEqual);

        if(-1 == isValid)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        PXCompilerSymbolEntryPeek(&pxCompiler);

        switch(index)
        {
            case 0:
            {
                PXCompilerParseText(&pxCompiler, pxR3D2->Name, 64, PXCompilerParseTextDetectUntilNextLine);
                break;
            }
            case 1:
            {
                PXSize uhh = 0;
                PXCompilerParseF32V(&pxCompiler, pxR3D2->CentralPoint.Data, 3, &uhh);
                break;
            }
            case 2:
            {
                PXSize uhh = 0;
                PXCompilerParseF32V(&pxCompiler, pxR3D2->PivotPoint.Data, 3, &uhh);
                break;
            }
            case 3:
            {
                pxR3D2->Header.VertexCount = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                pxR3D2->VertexData = PXMemoryHeapMallocT(PXVector3F32, pxR3D2->Header.VertexCount);

                PXCompilerSymbolEntryForward(&pxCompiler);

                PXSize uhh = 0;
                PXCompilerParseF32V(&pxCompiler, pxR3D2->VertexData->Data, pxR3D2->Header.VertexCount*3, &uhh);

                break;
            }
            case 4:
            {
                pxR3D2->Header.IndexCount = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U;
                pxR3D2->StaticFaceList = PXMemoryHeapMallocT(R3D2MeshStaticFace, pxR3D2->Header.IndexCount);

                PXCompilerSymbolEntryForward(&pxCompiler);

                for(size_t i = 0; i < pxR3D2->Header.IndexCount; i++)
                {
                    R3D2MeshStaticFace PXREF r3D2MeshStaticFace = &pxR3D2->StaticFaceList[i];
                  
                    const PXBool isNumeric = PXCompilerSymbolEntryPeekCheck(&pxCompiler, PXCompilerSymbolLexerNumeric);

                    if(!isNumeric)
                    {
                        break;
                    }

                    const PXI8U amount = pxCompiler.ReadInfo.SymbolEntryCurrent.I32U; // ??? vertex size

                    PXCompilerSymbolEntryForward(&pxCompiler);

                    PXSize x = 0;

                    PXCompilerParseI32V(&pxCompiler, r3D2MeshStaticFace->VertexID, 3);
                    PXCompilerParseText(&pxCompiler, r3D2MeshStaticFace->Material, 64, 0);
                    PXCompilerParseF32V(&pxCompiler, r3D2MeshStaticFace->UV->Data, 6, &x);
                } 

                break;
            }
        }
    }

    // Expect "[ObjectEnd]"
    {
        PXBool isValid = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);

        if(!isValid)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        isValid = PXCompilerEnsureAndCompare(&pxCompiler, PXR3D2ObjectEnd, PXR3D2ObjectEndSize);

        if(!isValid)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
        isValid = PXCompilerSymbolEntryPeekEnsure(&pxCompiler, PXCompilerSymbolLexerBracketSquareClose);

        if(!isValid)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        PXCompilerSymbolEntryForward(&pxCompiler);
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXR3D2TextText,
        "Load",
        "---Done---"
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXR3D2SaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}