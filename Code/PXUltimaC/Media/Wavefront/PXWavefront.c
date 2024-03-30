#include "PXWavefront.h"

#include <Compiler/PXCompiler.h>
#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>

#define PXWavefrontDetectMaterial 0

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
    if (!line || size == 0)
    {
        return PXWavefrontLineInvalid;
    }

    const char* const text = (const char* const)line;

    switch (size)
    {
        case 1:
        {
            switch (text[0])
            {
                case 'v': return PXWavefrontLineVertexGeometric;
                case 'f': return PXWavefrontLineFaceElement;
                case '#': return PXWavefrontLineComment;
                case 'o': return PXWavefrontLineObjectName;
                case 's': return PXWavefrontLineSmoothShading;
                case 'g': return PXWavefrontLineObjectGroup;
            }

            break;
        }
        case 2:
        {
            const PXInt16U lineTagID = PXInt16Make(text[0], text[1]);

            switch (lineTagID)
            {
                case PXInt16Make('v', 't'): return PXWavefrontLineVertexTexture;
                case PXInt16Make('v', 'n'): return PXWavefrontLineVertexNormal;
                case PXInt16Make('v', 'p'): return PXWavefrontLineVertexParameter;
            }

            break;
        }

        case 6:
        {
            const unsigned long long lineTagID = PXInt32Make(text[0], text[1], text[2], text[3]);

            switch (lineTagID)
            {
                case PXInt32Make('m', 't', 'l', 'l'):
                case PXInt32Make('u', 's', 'e', 'm'):
                {
                    const unsigned short lineTagID = PXInt16Make(text[4], text[5]);

                    switch (lineTagID)
                    {
                        case PXInt16Make('i', 'b'): return PXWavefrontLineMaterialLibraryInclude;
                        case PXInt16Make('t', 'l'): return PXWavefrontLineMaterialLibraryUse;
                    }

                    break;
                }
            }

            break;
        }
    }

    return PXWavefrontLineInvalid;
}

void PXAPI PXWavefrontCompileError(PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int expectedID)
{
    char textBuffer[32];

    PXTextCopyA(compilerSymbolEntry->Source, 20, textBuffer, 32);

    PXLogPrint
    (
        PXLoggingError,
        "Wavefront",
        "Compile",
        "At line <%i> at <%i>, unexpected symbol.\n%s",
        compilerSymbolEntry->Line,
        compilerSymbolEntry->Coloum,
        textBuffer        
    );
}

void PXAPI PXWavefrontFaceLineParse(PXFile* const tokenSteam, PXInt32U* const vertexData)
{
    PXCompilerSymbolEntry pxCompilerSymbolEntry;

    PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // read token, expect int

    const PXBool isExpectedInteger = PXCompilerSymbolLexerInteger == pxCompilerSymbolEntry.ID; // is int`?

    if(!isExpectedInteger) // If not int => Error
    {
        PXLogPrint
        (
            PXLoggingError,
            "Wavefront",
            "Parse",
            "Faceline element invalid. Line <%i>",
            pxCompilerSymbolEntry.Line
        );

        return;
    }

    PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // read token, expect int

    // Save 1st value
    vertexData[0] = pxCompilerSymbolEntry.DataI32U;

    PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Peek for next token

    switch(pxCompilerSymbolEntry.ID)
    {
        case PXCompilerSymbolLexerInteger: // is syntax A
        {
            PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // Peek sucessful skip to 2nd integer

            vertexData[1] = pxCompilerSymbolEntry.DataI32U; // Save 2nd value

            PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Get 3rd integer

            const PXBool isExpectedThridInteger = PXCompilerSymbolLexerInteger == pxCompilerSymbolEntry.ID;

            if(!isExpectedThridInteger)
            {
                // Error;
            }

            PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry);
            vertexData[2] = pxCompilerSymbolEntry.DataI32U;

            break;
        }
        case PXCompilerSymbolLexerSlash: // Syntax B or C
        {
            PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // Peek sucessful, remove the first '/'

            PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Next token

            switch(pxCompilerSymbolEntry.ID)
            {
                case PXCompilerSymbolLexerSlash: // Syntax: "xx//xx"
                {
                    PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // Remove the '/'
                    PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Next token, expect int
                    const PXBool isThridToken = PXCompilerSymbolLexerInteger == pxCompilerSymbolEntry.ID; // is int?

                    if(!isThridToken) // if not int => error
                    {
                        // Error
                    }
                 
                    PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry);    // Consume 3rd number
               
                    vertexData[2] = pxCompilerSymbolEntry.DataI32U;      // Store 3rd number

                    break;
                }
                case PXCompilerSymbolLexerInteger: // Is syntax B
                {
                    PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // Peek sucessful, remove the secound value

                    vertexData[1] = pxCompilerSymbolEntry.DataI32U; // Save value

                    // Exptect 2nd '/'
                    {
                        PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Peek, expect '/'
                        const PXBool isSlash = PXCompilerSymbolLexerSlash == pxCompilerSymbolEntry.ID;

                        if(isSlash)
                        {
                            PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry); // remove '/'

                            // Try get 3nd integer
                            {
                                PXCompilerSymbolEntryPeek(tokenSteam, &pxCompilerSymbolEntry); // Next token

                                const PXBool isSecoundToken = PXCompilerSymbolLexerInteger == pxCompilerSymbolEntry.ID;

                                if(!isSecoundToken)
                                {
                                    // Error
                                }

                                vertexData[2] = pxCompilerSymbolEntry.DataI32U;

                                PXCompilerSymbolEntryExtract(tokenSteam, &pxCompilerSymbolEntry);
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
        default:
        {
            // Invalid syntax
            break;
        }
    }

    for(PXSize i = 0; i < 3u; ++i)
    {
        vertexData[i] -= 1u;
    }   
}

PXActionResult PXAPI PXWavefrontLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    PXModel* const pxModel = (PXModel*)pxResourceLoadInfo->Target;

    PXFile tokenSteam;
    PXSize errorCounter = 0;

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

    PXFileOpenTemporal(&tokenSteam, pxResourceLoadInfo->FileReference->DataSize * 16);

    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Start"
    );


    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Step 1, Lexer..."
    );

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";

        PXCompilerLexicalAnalysis(pxResourceLoadInfo->FileReference, &tokenSteam, &compilerSettings); // Raw-File-Input -> Lexer tokens
    }

    // Stage - 1 - Analyse file

    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Step 2, analyse file..."
    );

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // First in line token

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        switch (objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

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

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, &pxTextElement);

                if (!isString)
                {
                    ++errorCounter;
                    PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                //PXFileWriteI8U(outputStream, PXCompilerSymbolLexerString);
               // PXFileWriteI16U(outputStream, namedElementSize);
               // PXFileWriteA(outputStream, namedElement, namedElementSize);

                switch (objPeekLine)
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

                const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    switch (objPeekLine)
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
                const PXSize lineStart = compilerSymbolEntry.Line;
                PXSize lineCurrent = lineStart;

                PXBool isDone = PXFalse;
                
                do
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&tokenSteam, vertexData); // Get the data

                    counterVertexMaxID = PXMathMaximumIU(counterVertexMaxID, vertexData[0]);
                    ++counterIndex;

                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // check what line we are in

                    lineCurrent = compilerSymbolEntry.Line; // Update current line

                    isDone = PXFileIsAtEnd(&tokenSteam) || (lineStart != lineCurrent);
                } 
                while(!isDone);

                break; // [OK]
            }
            default: // Error
            {
                ++errorCounter;
                PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);

                do
                {
                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                }
                while (compilerSymbolEntry.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }


    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Step 3, prealocate memory..."
    );

    //
    PXBool requireToCalculateNormals = PXFalse;

    // Stage - 2 - Allocate space
    {
        const PXBool isVNT = counterVertex && counterNormal && counterTexture;
        const PXBool isVT = counterVertex && !counterNormal && counterTexture;
        const PXBool isVN = counterVertex && counterNormal && !counterTexture;

        if (isVNT)
        {
            pxModel->VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;
        }
        else if (isVT)
        {
            // We would set this in that format, but we want normals.
            // pxModel->VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;

            pxModel->VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;

            requireToCalculateNormals = PXTrue;
        }
        else if(isVN)
        {
            pxModel->VertexBuffer.Format = PXVertexBufferFormatN3F_XYZ;
        }

        // Allocate temp cache to use it for later
        vertexPositionDataCacheSize = counterVertex * 3u;
        vertexTextureDataCacheSize = counterTexture * 2u;
        vertexNormalDataCacheSize = counterNormal * 3u;

        vertexDataCacheSize = vertexPositionDataCacheSize + vertexTextureDataCacheSize + vertexNormalDataCacheSize;
        PXNewList(float, vertexDataCacheSize, &vertexDataCache, PXNull);

        vertexTextureDataCache = vertexDataCache;    
        vertexNormalDataCache = &vertexTextureDataCache[vertexNormalDataCacheSize];
        vertexPositionDataCache = &vertexNormalDataCache[vertexTextureDataCacheSize];     

        indexDataCacheSize = counterIndex;

        pxModel->VertexBuffer.VertexDataRowSize = PXVertexBufferFormatStrideSize(pxModel->VertexBuffer.Format);

        pxModel->VertexBuffer.VertexDataAmount = pxModel->VertexBuffer.VertexDataRowSize * counterIndex;
        pxModel->IndexBuffer.SegmentListAmount = materialUseIndex;

        PXNewListZerod(float, pxModel->VertexBuffer.VertexDataAmount, &pxModel->VertexBuffer.VertexData, &pxModel->VertexBuffer.VertexDataSize);
        PXNewListZerod(PXIndexSegment, materialUseIndex, &pxModel->IndexBuffer.SegmentList, &pxModel->IndexBuffer.SegmentListSize);


        materialUseIndex = 0;


        // Setup index array
#if 0
        pxModel->IndexBuffer.DrawModeID = PXDrawModeIDTriangle; // PXDrawModeIDPoint | PXDrawModeIDLineLoop;
        pxModel->IndexBuffer.DataType = PXDataTypeIntFitting(counterVertexMaxID);
        pxModel->IndexBuffer.IndexTypeSize = pxModel->IndexBuffer.DataType & PXDataTypeSizeMask;
        pxModel->IndexBuffer.IndexDataAmount = counterIndex;
        pxModel->IndexBuffer.IndexData = PXMemoryHeapAllocateCleared(pxModel->IndexBuffer.IndexTypeSize, counterIndex);
        pxModel->IndexBuffer.IndexDataSize = pxModel->IndexBuffer.IndexTypeSize * counterIndex;
#else
        pxModel->IndexBuffer.DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;
        pxModel->IndexBuffer.DataType = 0;
        pxModel->IndexBuffer.IndexTypeSize = 0;
        pxModel->IndexBuffer.IndexDataAmount = counterIndex;
        pxModel->IndexBuffer.IndexData = 0;
        pxModel->IndexBuffer.IndexDataSize = 0;
#endif

        pxModel->MaterialContaierListAmount = materialInlcudeIndex;
        PXNewListZerod(PXMaterialContainer, materialInlcudeIndex, &pxModel->MaterialContaierList, PXNull);


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
        "WaveFront",
        "Parsing",
        "Step 4, extract data..."
    );
#endif
    
    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        switch (objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name .

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

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, &materialFileName);

                if (!isString)
                {
                    ++errorCounter;
                    PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                switch (objPeekLine)
                {
                    case PXWavefrontLineMaterialLibraryInclude:
                    {
                        PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialInlcudeIndex++];
                        PXFile materialFile; 

                        // Open and load
                        {
                            //-------------------------------
                            PXText materialFilePathFull;
                            PXTextConstructNamedBufferA(&materialFilePathFull, materialFilePathFullBuffer, PXPathSizeMax);

                            PXFilePathRelativeFromFile(pxResourceLoadInfo->FileReference, &materialFileName, &materialFilePathFull);
     
                            PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
                            PXFileOpenFromPathInfoMakeLoadOneshot(&pxFileOpenFromPathInfo, materialFilePathFull);

                            {
                                const PXActionResult materialFileLoadResult = PXFileOpenFromPath(&materialFile, &pxFileOpenFromPathInfo);
                                const PXBool sucessful = PXActionSuccessful == materialFileLoadResult;

                                if (sucessful)
                                {
                                    const PXActionResult materialFileCompileResult = PXMTLLoadFromFile(pxMaterialContaier, &materialFile);
                                }
                            }
                        }                   
                      
                        PXFileDestruct(&materialFile);   

                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        PXIndexSegment* const pxIndexSegmentLast = &pxModel->IndexBuffer.SegmentList[pxModel->IndexBuffer.SegmentListAmount - 1];
                        PXIndexSegment* const pxIndexSegmentCurrent = &pxModel->IndexBuffer.SegmentList[materialUseIndex];
                        PXSize offset = counterIndex - currentTotalOffset;

                        pxIndexSegmentCurrent->Material = PXMaterialContainerFind(pxModel->MaterialContaierList, &materialFileName);

                        if (offset != 0) 
                        {
                            PXIndexSegment* const pxIndexSegmentPrevious = &pxModel->IndexBuffer.SegmentList[materialUseIndex - 1];

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

                const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    // We dont write the data directly into the vertex buffer.
                    // As this data is compressed and is not directly useable for the rendering API.

                    switch (objPeekLine)
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


                const PXSize lineStart = compilerSymbolEntry.Line;
                PXSize lineCurrent = lineStart;

                PXBool isDone = PXFalse;

                do
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&tokenSteam, vertexData); // Get the data

       
                    const PXSize dataSize = pxModel->IndexBuffer.IndexTypeSize;
                    // PXMemoryCopy(vertexData, dataSize, input, dataSize);


                     //-------------------------------------
                     // Fill vertex element from index data
                     //-------------------------------------                              
                    float* vertexTextureDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeTexture, counterIndex);
                    float* vertexNormalDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeNormal, counterIndex);
                    float* vertexPositionDataTarget = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeVertex, counterIndex);

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




                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // check what line we are in

                    lineCurrent = compilerSymbolEntry.Line; // Update current line

                    isDone = PXFileIsAtEnd(&tokenSteam) || (lineStart != lineCurrent);
                } 
                while(!isDone);


                break; // [OK]
            }
            default: // Error
            {
                ++errorCounter;
                PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);

                do
                {
                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                }
                while (compilerSymbolEntry.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }


    PXFileDestruct(&tokenSteam);

    PXDeleteList(float, vertexDataCacheSize, &vertexDataCache, PXNull); // Delete cached vertex data


    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Done!"
    );


    if (errorCounter)
    {
        return PXActionCompilingError;
    }

    if (!requireToCalculateNormals)
    {
        return PXActionSuccessful;
    }


    // Calculate normals

    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Generating missing normals..."
    );

    for (PXSize i = 0; i < counterVertex; ++i)
    {
        float* const positionData = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeNormal, i);
        float* const normalData = (float*)PXVertexBufferInsertionPoint(&pxModel->VertexBuffer, PXVertexBufferDataTypeVertex, i);

        PXVector3F normalVector;
        PXVector3F positionVector =
        {
            positionData[0],
            positionData[1],
            positionData[2]
        };
        const PXVector3F positionVectorConst = {1,1,1};

        float normalFactor = PXVector3FDotProduct(&positionVectorConst, &positionVector);

        if (normalFactor != 0.0)
        {
            normalFactor = 1.0f / PXMathSquareRoot(normalFactor);
        }
        else
        {
            normalFactor = 0.0;
        }

        normalData[0] = normalFactor;
        normalData[1] = normalFactor;
        normalData[2] = normalFactor;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "WaveFront",
        "Parsing",
        "Generated missing normals!"
    );


    return PXActionSuccessful;
}

PXActionResult PXAPI PXWavefrontSaveFromFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
