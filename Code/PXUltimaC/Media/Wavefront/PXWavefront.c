#include "PXWavefront.h"

#include <Compiler/PXCompiler.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define PXWavefrontDetectMaterial 0

const char PXWaveFrontText[] = "WaveFront";

void PXAPI PXWavefrontElementConstruct(PXWavefrontElement* objElement)
{
    PXClear(PXWavefrontElement, objElement);
}

void PXAPI PXWavefrontElementDestruct(PXWavefrontElement* objElement)
{
    //TODO: clear memeory
}

PXWavefrontLineType PXAPI PXWavefrontPeekLine(const void* line, const PXSize size)
{
    if(!line || size == 0)
    {
        return PXWavefrontLineInvalid;
    }

    const char* const text = (const char* const)line;

    switch(size)
    {
        case 1:
        {
            switch(text[0])
            {
                case 'v':
                    return PXWavefrontLineVertexGeometric;
                case 'f':
                    return PXWavefrontLineFaceElement;
                case '#':
                    return PXWavefrontLineComment;
                case 'o':
                    return PXWavefrontLineObjectName;
                case 's':
                    return PXWavefrontLineSmoothShading;
                case 'g':
                    return PXWavefrontLineObjectGroup;
            }

            break;
        }
        case 2:
        {
            const PXInt16U lineTagID = PXInt16Make(text[0], text[1]);

            switch(lineTagID)
            {
                case PXInt16Make('v', 't'):
                    return PXWavefrontLineVertexTexture;
                case PXInt16Make('v', 'n'):
                    return PXWavefrontLineVertexNormal;
                case PXInt16Make('v', 'p'):
                    return PXWavefrontLineVertexParameter;
            }

            break;
        }

        case 6:
        {
            const unsigned long long lineTagID = PXInt32Make(text[0], text[1], text[2], text[3]);

            switch(lineTagID)
            {
                case PXInt32Make('m', 't', 'l', 'l'):
                case PXInt32Make('u', 's', 'e', 'm'):
                {
                    const unsigned short lineTagID = PXInt16Make(text[4], text[5]);

                    switch(lineTagID)
                    {
                        case PXInt16Make('i', 'b'):
                            return PXWavefrontLineMaterialLibraryInclude;
                        case PXInt16Make('t', 'l'):
                            return PXWavefrontLineMaterialLibraryUse;
                    }

                    break;
                }
            }

            break;
        }
    }

    return PXWavefrontLineInvalid;
}

void PXAPI PXWavefrontFaceLineParse(PXCompiler* const pxCompiler, PXInt32U* const vertexData)
{
    const PXBool isExpectedInteger = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInteger);

    if(!isExpectedInteger) // If not int => Error
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler); // Consume int

    // Save 1st value
    vertexData[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U;


    // Check
    {
        const PXCompilerSymbolLexer pxCompilerSymbolLexerList[2] =
        {
            PXCompilerSymbolLexerInteger,
            PXCompilerSymbolLexerSlash
        };

        const PXBool isValid = PXCompilerSymbolEntryEnsureCheckList(pxCompiler, pxCompilerSymbolLexerList, 2);

        if(!isValid)
        {
            return;
        }
    }

    PXCompilerSymbolEntryPeek(pxCompiler); // Peek for next token

    switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
    {
        case PXCompilerSymbolLexerInteger: // is syntax A
        {
            PXCompilerSymbolEntryForward(pxCompiler); // Peek sucessful skip to 2nd integer

            vertexData[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U; // Save 2nd value

            const PXBool isExpectedThridInteger = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInteger); // Get 3rd integer

            if(!isExpectedThridInteger)
            {
                // Error;
                return;
            }

            PXCompilerSymbolEntryForward(pxCompiler);

            vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U;

            break;
        }
        case PXCompilerSymbolLexerSlash: // Syntax B or C
        {
            PXCompilerSymbolEntryForward(pxCompiler); // Peek sucessful, remove the first '/'

            PXCompilerSymbolEntryPeek(pxCompiler); // Next token

            switch(pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
            {
                case PXCompilerSymbolLexerSlash: // Syntax: "xx//xx"
                {
                    PXCompilerSymbolEntryForward(pxCompiler);  // Remove the '/'

                    const PXBool isThridToken = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInteger); // Next token, expect int

                    if(!isThridToken) // if not int => error
                    {
                        // Error
                        return;
                    }

                    PXCompilerSymbolEntryForward(pxCompiler); // Consume 3rd number

                    vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U;      // Store 3rd number

                    break;
                }
                case PXCompilerSymbolLexerInteger: // Is syntax B
                {
                    PXCompilerSymbolEntryForward(pxCompiler); // Peek sucessful, remove the secound value

                    vertexData[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U; // Save value

                    // Exptect 2nd '/'
                    {
                        const PXBool isSlash = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerSlash); // Peek, expect '/'

                        if(isSlash)
                        {
                            PXCompilerSymbolEntryForward(pxCompiler);  // remove '/'

                            // Try get 3nd integer
                            {
                                const PXBool isSecoundToken = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerInteger);

                                if(!isSecoundToken)
                                {
                                    // Error
                                    return;
                                }

                                vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.DataI32U;

                                PXCompilerSymbolEntryForward(pxCompiler);
                            }
                        }
                    }

                    break;
                }
                default:
                {
                    // Invalid
                    break;
                }
            }

            break;
        }
    }

    for(PXSize i = 0; i < 3u; ++i)
    {
        vertexData[i] -= 1u;
    }
}

#pragma optimize("", off)
PXActionResult PXAPI PXWavefrontLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXModel* const pxModel = (PXModel*)pxResourceLoadInfo->ResourceTarget;

    PXFile tokenSteam;

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = &tokenSteam;
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
    pxCompiler.CommentSingleLineSize = 1u;
    pxCompiler.CommentSingleLine = "#";

    PXSize drawoffsetCounter = 0;
    PXSize drawCurrentIndex = 0;

    float* vertexDataCache = PXNull;
    PXSize vertexDataCacheSize = 0;

    float* vertexPositionDataCache = PXNull;
    PXSize vertexPositionDataCacheSize = 0;
    float* vertexTextureDataCache = PXNull;
    PXSize vertexTextureDataCacheSize = 0;
    float* vertexNormalDataCache = PXNull;
    PXSize vertexNormalDataCacheSize = 0;

    PXSize indexDataCacheSize = 0;


    PXInt32U counterVertex = 0;
    PXInt32U counterVertexMaxID = 0;
    PXInt32U counterNormal = 0;
    PXInt32U counterTexture = 0;
    PXInt32U counterIndex = 0;

    PXInt32U materialInlcudeIndex = 0;
    PXInt32U materialUseIndex = 0;


    PXInt32U currentTotalOffset = 0;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Start"
    );
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Step 1, Lexer..."
    );
#endif

    // Lexer - Level I
    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens


    // Stage - 1 - Analyse file

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Step 2, analyse file..."
    );
#endif

#if 1
    while(!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntryExtract(&pxCompiler); // First in line token

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

        switch(objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&pxCompiler); // Expect a name.

                // PXFileWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                // PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);

                break;
            }
            case PXWavefrontLineMaterialLibraryInclude:
            case PXWavefrontLineMaterialLibraryUse:
            case PXWavefrontLineObjectName:
            case PXWavefrontLineObjectGroup:
            {
                PXText pxTextElement;
                PXTextConstructBufferA(&pxTextElement, 260);

                const PXBool isString = PXCompilerParseStringUntilNewLine(&pxCompiler, &pxTextElement);

                if(!isString)
                {
                    //++errorCounter;
                    break;
                }

                // PXCompilerSymbolEntryMergeCurrentUntilNextLine();

                switch(objPeekLine)
                {
                    case PXWavefrontLineMaterialLibraryInclude:
                    {
                        ++materialInlcudeIndex;
                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        ++materialUseIndex;
                        break;
                    }
                }

                break; // [OK]
            }
            case PXWavefrontLineVertexTexture:
            case PXWavefrontLineVertexGeometric:
            case PXWavefrontLineVertexNormal:
            case PXWavefrontLineVertexParameter:
            {
                PXSize valuesDetected = 0;
                PXSize valuesExpected = 16;
                float vector[4] = { -1, -1, -1, -1 };

                const PXBool listParsed = PXCompilerParseFloatList(&pxCompiler, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    switch(objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                            ++counterVertex;
                            //printf("|%6i| %3s | %12.6f | %12.6f | %12.6f |\n", compilerSymbolEntry.Line, "v", vector[0], vector[1], vector[2]);
                            //pxModel->DataVertexWidth = PXMathMaximum(pxModel->DataVertexWidth, valuesDetected);
                            break;

                        case PXWavefrontLineVertexNormal:
                            ++counterNormal;
                            // printf("|%6i| %3s | %12.6f | %12.6f | %12.6f |\n", compilerSymbolEntry.Line, "vn", vector[0], vector[1], vector[2]);
                            //pxModel->DataNormalWidth = PXMathMaximum(pxModel->DataNormalWidth, valuesDetected);
                            break;

                        case PXWavefrontLineVertexParameter:
                            //pxModel-> += valuesDetected;
                            break;

                        case PXWavefrontLineVertexTexture:
                            ++counterTexture;
                            //  printf("|%6i| %3s | %12.6f | %12.6f | %12.6f |\n", compilerSymbolEntry.Line, "vt", vector[0], vector[1], vector[2]);
                            //pxModel->DataTextureWidth = PXMathMaximum(pxModel->DataTextureWidth, valuesDetected);
                            break;
                    }
                }

                break; // [OK]
            }
            case PXWavefrontLineFaceElement:
            {
                const PXSize lineStart = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;
                PXSize lineCurrent = lineStart;

                PXBool isDone = PXFalse;

                do
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data

                    counterVertexMaxID = PXMathMaximumIU(counterVertexMaxID, vertexData[0]);
                    ++counterIndex;

                    PXCompilerSymbolEntryPeek(&pxCompiler); // check what line we are in

                    lineCurrent = pxCompiler.ReadInfo.SymbolEntryCurrent.Line; // Update current line

                    isDone = PXFileIsAtEnd(&tokenSteam) || (lineStart != lineCurrent);
                } 
                while(!isDone);

                break; // [OK]
            }
            default: // Error
            {
                // ++errorCounter;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXWaveFrontText,
                    "Parsing",
                    "Unexpected"
                );
#endif


                do
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                } 
                while(pxCompiler.ReadInfo.SymbolEntryCurrent.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Step 3, prealocate memory..."
    );
#endif

    //
    PXBool requireToCalculateNormals = PXFalse;

    // Stage - 2 - Allocate space
    {
        const PXBool isVNT = counterVertex && counterNormal && counterTexture;
        const PXBool isVT = counterVertex && !counterNormal && counterTexture;
        const PXBool isVN = counterVertex && counterNormal && !counterTexture;

        if(isVNT)
        {
            pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;
        }
        else if(isVT)
        {
            // We would set this in that format, but we want normals.
            // pxModel->VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;

            pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;

            requireToCalculateNormals = PXTrue;
        }
        else if(isVN)
        {
            pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatN3F_XYZ;
        }

        // Allocate temp cache to use it for later
        vertexPositionDataCacheSize = counterVertex * 3u;
        vertexTextureDataCacheSize = counterTexture * 2u;
        vertexNormalDataCacheSize = counterNormal * 3u;

        vertexDataCacheSize = sizeof(float) * (vertexPositionDataCacheSize + vertexTextureDataCacheSize + vertexNormalDataCacheSize);
        vertexDataCache = PXMemoryHeapCallocT(float, vertexDataCacheSize);

        vertexTextureDataCache = vertexDataCache;
        vertexNormalDataCache = &vertexTextureDataCache[vertexNormalDataCacheSize];
        vertexPositionDataCache = &vertexNormalDataCache[vertexTextureDataCacheSize];

        indexDataCacheSize = counterIndex;

        //pxModel->VertexBuffer.VertexDataRowSize = PXVertexBufferFormatStrideSize(pxModel->VertexBuffer.Format);
        //pxModel->VertexBuffer.VertexDataAmount = pxModel->VertexBuffer.VertexDataRowSize * counterIndex;
        pxModel->Mesh.IndexBuffer.SegmentListAmount = materialUseIndex;

        const PXSize vertexDataStride = PXVertexBufferFormatStrideSize(pxModel->Mesh.VertexBuffer.Format);
        const PXSize vertexDataAmount = vertexDataStride * counterIndex;


        pxModel->Mesh.VertexBuffer.VertexDataSize = sizeof(float) * vertexDataAmount;
        pxModel->Mesh.VertexBuffer.VertexData = PXMemoryHeapCallocT(float, vertexDataAmount);    

        pxModel->Mesh.IndexBuffer.SegmentListSize = sizeof(PXIndexSegment) * materialUseIndex;
        pxModel->Mesh.IndexBuffer.SegmentList = PXMemoryHeapCallocT(PXIndexSegment, materialUseIndex);


        materialUseIndex = 0;


        // Setup index array
#if 0
        pxModel->IndexBuffer.DrawModeID = PXDrawModeIDTriangle; // PXDrawModeIDPoint | PXDrawModeIDLineLoop;
        pxModel->IndexBuffer.DataType = PXTypeIntFitting(counterVertexMaxID);
        pxModel->IndexBuffer.IndexTypeSize = pxModel->IndexBuffer.DataType & PXTypeSizeMask;
        pxModel->IndexBuffer.IndexDataAmount = counterIndex;
        pxModel->IndexBuffer.IndexData = PXMemoryHeapAllocateCleared(pxModel->IndexBuffer.IndexTypeSize, counterIndex);
        pxModel->IndexBuffer.IndexDataSize = pxModel->IndexBuffer.IndexTypeSize * counterIndex;
#else
        pxModel->Mesh.IndexBuffer.DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;
        pxModel->Mesh.IndexBuffer.IndexDataType = 0;
        //pxModel->IndexBuffer.IndexDataAmount = counterIndex;
        pxModel->Mesh.IndexBuffer.IndexData = 0;
        pxModel->Mesh.IndexBuffer.IndexDataSize = 0;
#endif

        pxModel->MaterialContaierListAmount = sizeof(PXMaterialContainer) * materialInlcudeIndex;
        pxModel->MaterialContaierList = PXMemoryHeapCallocT(PXMaterialContainer, materialInlcudeIndex);


        // Reset all size values
        counterVertex = 0;
        counterNormal = 0;
        counterTexture = 0;
        counterIndex = 0;
        materialInlcudeIndex = 0;
        materialUseIndex = 0;

        PXFileCursorToBeginning(&tokenSteam);
    }


    // Stage - 3 - Extract data

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Step 4, extract data..."
    );
#endif

    while(!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntryExtract(&pxCompiler);

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

        switch(objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&pxCompiler); // Expect a name .

                //PXFileWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                //PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);

                break;
            }
            case PXWavefrontLineMaterialLibraryInclude:
            case PXWavefrontLineMaterialLibraryUse:
            case PXWavefrontLineObjectName:
            case PXWavefrontLineObjectGroup:
            {
                PXText materialFileName;
                PXTextConstructNamedBufferA(&materialFileName, materialFileNameBuffer, PXPathSizeMax);

                const PXBool isString = PXCompilerParseStringUntilNewLine(&pxCompiler, &materialFileName);

                if(!isString)
                {
                    // ++errorCounter;
                    break;
                }

                switch(objPeekLine)
                {
                    case PXWavefrontLineMaterialLibraryInclude:
                    {
                        PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialInlcudeIndex++];
                        PXFile materialFile;
                        PXClear(PXFile, &materialFile);

                        // Open and load
                        {
                            //-------------------------------
                            PXText materialFilePathFull;
                            PXTextConstructNamedBufferA(&materialFilePathFull, materialFilePathFullBuffer, PXPathSizeMax);

                            PXFilePathRelativeFromFile(pxResourceLoadInfo->FileReference, &materialFileName, &materialFilePathFull);

                            PXResourceTransphereInfo pxMaterialLoadInfo;
                            PXClear(PXResourceTransphereInfo, &pxMaterialLoadInfo);
                            pxMaterialLoadInfo.ResourceTarget = pxMaterialContaier;
                            pxMaterialLoadInfo.FileReference = &materialFile;
                            pxMaterialLoadInfo.ResourceType = PXResourceTypeMaterialList;
                            pxMaterialLoadInfo.Manager = pxResourceLoadInfo->Manager;

                            PXResourceLoad(&pxMaterialLoadInfo, &materialFilePathFull);
                        }

                        PXFileClose(&materialFile);

                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        PXIndexSegment* const pxIndexSegmentLast = &pxModel->Mesh.IndexBuffer.SegmentList[pxModel->Mesh.IndexBuffer.SegmentListAmount - 1];
                        PXIndexSegment* const pxIndexSegmentCurrent = &pxModel->Mesh.IndexBuffer.SegmentList[materialUseIndex];
                        PXSize offset = counterIndex - currentTotalOffset;

                        pxIndexSegmentCurrent->Material = PXMaterialContainerFind
                        (
                            pxModel->MaterialContaierList,
                            &materialFileName
                        );

                        if(offset != 0)
                        {
                            PXIndexSegment* const pxIndexSegmentPrevious = &pxModel->Mesh.IndexBuffer.SegmentList[materialUseIndex - 1];

                            pxIndexSegmentPrevious->DataRange = offset;
                        }

                        currentTotalOffset = counterIndex;

                        pxIndexSegmentLast->DataRange = indexDataCacheSize - currentTotalOffset;

                        ++materialUseIndex;

                        break;
                    }
                }

                break; // [OK]
            }
            case PXWavefrontLineVertexTexture:
            case PXWavefrontLineVertexGeometric:
            case PXWavefrontLineVertexNormal:
            case PXWavefrontLineVertexParameter:
            {
                PXSize valuesDetected = 0;
                PXSize valuesExpected = 16;
                float vector[16] = { -1, -1, -1, -1 };

                const PXBool listParsed = PXCompilerParseFloatList(&pxCompiler, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    // We dont write the data directly into the vertex buffer.
                    // As this data is compressed and is not directly useable for the rendering API.

                    switch(objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                        {
                            float* const insertPosition = &vertexPositionDataCache[counterVertex * 3u];

                            PXCopyList(float, 3u, vector, insertPosition);

                            //float* const position = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeVertex, counterVertex);
                            //PXMemoryCopy(vector, sizeof(float) * valuesDetected, position, sizeof(float) * valuesDetected);

                            ++counterVertex;
                            break;
                        }
                        case PXWavefrontLineVertexNormal:
                        {
                            float* const insertPosition = &vertexNormalDataCache[counterNormal * 3u];

                            PXCopyList(float, 3u, vector, insertPosition);

                            ++counterNormal;
                            break;
                        }
                        case PXWavefrontLineVertexParameter:

                            break;

                        case PXWavefrontLineVertexTexture:
                        {
                            float* const insertPosition = &vertexTextureDataCache[counterTexture * 2u];

                            PXCopyList(float, 2u, vector, insertPosition);

                            ++counterTexture;
                            break;
                        }
                    }
                }

                break; // [OK]
            }
            case PXWavefrontLineFaceElement:
            {
                PXSize cornerPoints = 0;


                const PXSize lineStart = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;
                PXSize lineCurrent = lineStart;

                PXBool isDone = PXFalse;

                do
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data




                    //   const PXSize dataSize = pxModel->IndexBuffer.IndexTypeSize;
                    // PXMemoryCopy(vertexData, dataSize, input, dataSize);


                    //-------------------------------------
                    // Fill vertex element from index data
                    //-------------------------------------
                    float* vertexTextureDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeTexture, counterIndex);
                    float* vertexNormalDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeNormal, counterIndex);
                    float* vertexPositionDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeVertex, counterIndex);

                    if(vertexData[1] != -1)
                    {
                        float* vertexTextureDataSource = &vertexTextureDataCache[vertexData[1] * 2u];
                        PXCopyList(float, 2, vertexTextureDataSource, vertexTextureDataTarget);
                    }

                    if(vertexData[2] != -1)
                    {
                        float* vertexNormalDataSource = &vertexNormalDataCache[vertexData[2] * 3u];
                        PXCopyList(float, 3, vertexNormalDataSource, vertexNormalDataTarget);
                    }

                    if(vertexData[0] != -1)
                    {
                        float* vertexPositionDataSource = &vertexPositionDataCache[vertexData[0] * 3u];
                        PXCopyList(float, 3, vertexPositionDataSource, vertexPositionDataTarget);
                    }
                    //-------------------------------------



                    counterIndex++;


                    //  PXFileWriteI32UV(outputStream, vertexData, 3u);

                    //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                    //----------------------------------

                    ++cornerPoints;




                    PXCompilerSymbolEntryPeek(&pxCompiler); // check what line we are in

                    lineCurrent = pxCompiler.ReadInfo.SymbolEntryCurrent.Line; // Update current line

                    isDone = PXFileIsAtEnd(&tokenSteam) || (lineStart != lineCurrent);
                } while(!isDone);


                break; // [OK]
            }
            default: // Error
            {
                //  ++errorCounter;

                do
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                } while(pxCompiler.ReadInfo.SymbolEntryCurrent.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }


    PXFileClose(&tokenSteam);

    PXMemoryHeapFree(PXNull, vertexDataCache); // Delete cached vertex data


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Done!"
    );
#endif


    // if (errorCounter)
    // {
    //     return PXActionCompilingError;
    // }

    if(!requireToCalculateNormals)
    {
        return PXActionSuccessful;
    }


    // Calculate normals

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Generating missing normals..."
    );
#endif

    for(PXSize i = 0; i < counterVertex; ++i)
    {
        float* const positionData = (float*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeNormal, i);
        float* const normalData = (float*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeVertex, i);

        PXVector3F normalVector;
        PXVector3F positionVector =
        {
            positionData[0],
            positionData[1],
            positionData[2]
        };
        const PXVector3F positionVectorConst = { 1,1,1 };

        float normalFactor = PXVector3FDotProduct(&positionVectorConst, &positionVector);

        if(normalFactor != 0.0)
        {
            normalFactor = 1.0f / PXMathRootSquare(normalFactor);
        }
        else
        {
            normalFactor = 0.0;
        }

        normalData[0] = normalFactor;
        normalData[1] = normalFactor;
        normalData[2] = normalFactor;
    }

#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        "Parsing",
        "Generated missing normals!"
    );
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXWavefrontSaveFromFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}