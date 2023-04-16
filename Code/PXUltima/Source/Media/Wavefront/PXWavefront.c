#include "PXWavefront.h"

#include <Compiler/PXCompiler.h>
#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

#define PXWavefrontDetectMaterial 1

#define PXCompilerSymbolLexerPXWavefrontMaterialLibraryIncludeID 'I'
#define PXCompilerSymbolLexerPXWavefrontMaterialLibraryUselID 'U'
#define PXCompilerSymbolLexerPXWavefrontObjectNameID 'O'
#define PXCompilerSymbolLexerPXWavefrontSmoothShadingID 'S'
#define PXCompilerSymbolLexerPXWavefrontObjectGroupID 'G'
#define PXCompilerSymbolLexerPXWavefrontVertexGerometricID 'v'
#define PXCompilerSymbolLexerPXWavefrontVertexNormalID 'n'
#define PXCompilerSymbolLexerPXWavefrontVertexParameterID 'p'
#define PXCompilerSymbolLexerPXWavefrontVertexTextureID 't'

void PXWavefrontElementConstruct(PXWavefrontElement* objElement)
{
    MemoryClear(objElement, sizeof(PXWavefrontElement));
}

void PXWavefrontElementDestruct(PXWavefrontElement* objElement)
{
    //TODO: clear memeory
}

void PXWavefrontConstruct(PXWavefront* const obj)
{
    MemoryClear(obj, sizeof(PXWavefront));
}

void PXWavefrontDestruct(PXWavefront* const obj)
{
    MemoryRelease(obj->ElementList, obj->ElementListSize);

    MemoryRelease(obj->MaterialFileList, obj->MaterialFileListSize);
}

PXWavefrontLineType PXWavefrontPeekLine(const void* line, const PXSize size)
{
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

void PXWavefrontCompileError(PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int expectedID)
{
    char textBuffer[32];

    PXTextCopyA(compilerSymbolEntry->Source, 20, textBuffer, 32);

    printf
    (
        "[PXWavefront][Error] At line <%i> at <%i>, unexpected symbol.\n"
        "          -> %s\n",
        compilerSymbolEntry->Line,
        compilerSymbolEntry->Coloum,
        textBuffer
    );
}

PXActionResult PXWavefrontFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXSize errorCounter = 0;
    PXDataStream tokenSteam;

    unsigned int vertexListSize = 0;
    unsigned int normalListSize = 0;
    unsigned int textureListSize = 0;
    unsigned int parameterListSize = 0;
    unsigned int indexListSize = 0;

    unsigned int mtlInlclueesListSize = 0;
    PXSize mtlEmbeddedDataOffset = 0;

    PXSize drawoffsetCounter = 0;
    PXSize drawCurrentCounter = 0;
    PXSize drawCurrentIndex = 0;

    unsigned char drawSize[256];
    PXSize drawOrder[256];

    MemoryClear(drawSize, sizeof(drawSize));
    MemoryClear(drawOrder, sizeof(drawOrder));

    const PXSize headerOffset = 1024;
    PXSize headerCacheOffset = 0;

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";

        outputStream->DataCursor += headerOffset;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        PXDataStreamFromExternal(&tokenSteam, (PXAdress)outputStream->Data + headerOffset, outputStream->DataCursor - headerOffset);

        outputStream->DataCursor = 0;
    }


    // Write 0'ed data to later jump back to to change.
    PXDataStreamCursorAdvance(outputStream, headerOffset);

    while (!PXDataStreamIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        switch (compilerSymbolEntry.ID)
        {
            case PXCompilerSymbolLexerComment:
            case PXCompilerSymbolLexerNewLine:
                continue;
        }

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        PXDataStreamWriteI8U(outputStream, objPeekLine);

        switch (objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

                PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                PXDataStreamWriteI32U(outputStream, compilerSymbolEntry.DataI);

                break;
            }
            case PXWavefrontLineMaterialLibraryInclude:
            case PXWavefrontLineMaterialLibraryUse:
            case PXWavefrontLineObjectName:
            case PXWavefrontLineObjectGroup:
            {
                char namedElement[256];
                PXSize namedElementSize = 0;

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, namedElement, 256, &namedElementSize);

                if (!isString)
                {
                    ++errorCounter;
                    PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerString);
                PXDataStreamWriteI16U(outputStream, namedElementSize);
                PXDataStreamWriteA(outputStream, namedElement, namedElementSize);

                switch (objPeekLine)
                {
                    case PXWavefrontLineMaterialLibraryInclude:
                    {
                        ++mtlInlclueesListSize;

#if PXWavefrontDetectMaterial

                        headerCacheOffset += PXDataStreamWriteAtI16U(outputStream, namedElementSize, headerCacheOffset);
                        headerCacheOffset += PXDataStreamWriteAtB(outputStream, namedElement, namedElementSize, headerCacheOffset);
#endif

                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        drawOrder[drawCurrentIndex++] = drawCurrentCounter;
                        drawCurrentCounter = 0;
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
                PXSize valuesExpected = 0;
                float vector[4] = { -1, -1, -1, -1 };

                switch (objPeekLine)
                {
                    case PXWavefrontLineVertexGeometric:
                        valuesExpected = 3;
                        break;

                    case PXWavefrontLineVertexNormal:
                        valuesExpected = 3;
                        break;

                    case PXWavefrontLineVertexParameter:
                        valuesExpected = 3;
                        break;

                    case PXWavefrontLineVertexTexture:
                        valuesExpected = 2;
                        break;
                }

                const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    switch (objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                            vertexListSize += valuesDetected;
                            break;

                        case PXWavefrontLineVertexNormal:
                            normalListSize += valuesDetected;
                            break;

                        case PXWavefrontLineVertexParameter:
                            parameterListSize += valuesDetected;
                            break;

                        case PXWavefrontLineVertexTexture:
                            textureListSize += valuesDetected;
                            break;
                    }

                    PXDataStreamWriteI8U(outputStream, valuesDetected);
                    PXDataStreamWriteFV(outputStream, vector, valuesDetected);
                }

                break; // [OK]
            }
            case PXWavefrontLineFaceElement:
            {
                PXSize cornerPoints = 0;
                PXSize cursorPos = outputStream->DataCursor;

                PXDataStreamWriteI8U(outputStream, 0xFF);

                while (!PXDataStreamIsAtEnd(&tokenSteam))
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    // allowed syntax is
                    // A: "f 1 2 3"
                    // B: "f 1/2/3"
                    // C: "f 1//3"


                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    const PXBool isExpectedInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID; // is int`?

                    if (!isExpectedInteger) // If not int => Error
                    {
                        break;
                    }

                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    // Save 1st value
                    vertexData[0] = compilerSymbolEntry.DataI;

                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek for next token

                    switch (compilerSymbolEntry.ID)
                    {
                        case PXCompilerSymbolLexerInteger: // is syntax A
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful skip to 2nd integer

                            vertexData[1] = compilerSymbolEntry.DataI; // Save 2nd value 

                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Get 3rd integer

                            const PXBool isExpectedThridInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                            if (!isExpectedThridInteger)
                            {
                                // Error;
                            }

                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                            vertexData[2] = compilerSymbolEntry.DataI;

                            break;
                        }
                        case PXCompilerSymbolLexerSlash: // Syntax B or C
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful, remove the first '/'

                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Next token

                            switch (compilerSymbolEntry.ID)
                            {
                                case PXCompilerSymbolLexerSlash: // is Syntax C
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Remove the '/'
                                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Next token, expect int
                                    const PXBool isThridToken = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID; // is int?

                                    if (!isThridToken) // if not int => error
                                    {
                                        // Error
                                    }

                                    vertexData[2] = compilerSymbolEntry.DataI; // Save value

                                    break;
                                }
                                case PXCompilerSymbolLexerInteger: // Is syntax B
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful, remove the secound value

                                    vertexData[1] = compilerSymbolEntry.DataI; // Save value

                                    // Exptect 2nd '/'
                                    {
                                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek, expect '/'
                                        const PXBool isSlash = PXCompilerSymbolLexerSlash == compilerSymbolEntry.ID;

                                        if (!isSlash)
                                        {
                                            // Error
                                        }

                                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                    }

                                    // Try get 3nd integer
                                    {
                                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Next token

                                        const PXBool isSecoundToken = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                                        if (!isSecoundToken)
                                        {
                                            // Error
                                        }

                                        vertexData[2] = compilerSymbolEntry.DataI;

                                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
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

                    for (PXSize i = 0; i < 3u; ++i)
                    {
                        vertexData[i] -= 1u;
                    }

                    PXDataStreamWriteI32UV(outputStream, vertexData, 3u);

                  //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                    //----------------------------------

                    ++cornerPoints;
                }

                drawSize[drawCurrentIndex] = MathMaximum(drawSize[drawCurrentIndex], cornerPoints);

                indexListSize += cornerPoints;
                drawCurrentCounter += cornerPoints;

                PXDataStreamWriteAtI8U(outputStream, cornerPoints, cursorPos);

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

    // End of PXWavefront parsing
    drawOrder[drawCurrentIndex] = drawCurrentCounter;
    mtlEmbeddedDataOffset = outputStream->DataCursor;

    // Reset for header fetching
    //const PXSize offset = outputStream->DataCursor; // Save old position
    //outputStream->DataCursor = 0; // Jump to beginning, parsing temp headerInfo

    // Begin loading MTL files
#if PXWavefrontDetectMaterial
    {
        PXDataStream materialNameFetchStream;
        PXDataStream materialFileStream;

        PXDataStreamFromExternal(&materialNameFetchStream, outputStream->Data, outputStream->DataCursor);

        wchar_t currentWorkPath[PathMaxSize];
        wchar_t currentMTLFileW[PathMaxSize];
        char currentMTLFileA[PathMaxSize];

        const PXBool succ = PXDataStreamFilePathGetW(inputStream, currentWorkPath, PathMaxSize); // Work PXWavefront file path

        for (PXSize i = 0; i < mtlInlclueesListSize; ++i)
        {
            unsigned short length = 0;

            PXDataStreamWriteI8U(outputStream, PXWavefrontEmbeddedMTL);

            PXDataStreamReadI16U(&materialNameFetchStream, &length);
            PXDataStreamReadB(&materialNameFetchStream, currentMTLFileA, PathMaxSize);

            PXTextCopyAW(currentMTLFileA, length, currentMTLFileW, PathMaxSize);

            FilePathSwapFileNameW(currentWorkPath, currentMTLFileW, currentMTLFileW);

            {
                const PXActionResult materialFileLoadResult = PXDataStreamMapToMemoryW(&materialFileStream, currentMTLFileW, 0, MemoryReadOnly);
                const PXBool sucessful = PXActionSuccessful == materialFileLoadResult;

                if (!sucessful)
                {
                    continue;
                }

                const PXActionResult materialFileCompileResult = MTLFileCompile(&materialFileStream, outputStream);

                PXDataStreamDestruct(&materialFileStream);
            }
        }
    }
#endif
    // MTL loading finished



    {
        PXDataStream headerInfoStream;

        PXDataStreamFromExternal(&headerInfoStream, outputStream->Data, outputStream->DataCursor);

        PXDataStreamWriteI8U(&headerInfoStream, drawCurrentIndex);

        for (PXSize i = 0; i < drawCurrentIndex; ++i)
        {
            PXDataStreamWriteI8U(&headerInfoStream, drawSize[i+1]);
            PXDataStreamWriteI32U(&headerInfoStream, drawOrder[i+1]);
        }

        PXDataStreamWriteI32U(&headerInfoStream, vertexListSize);
        PXDataStreamWriteI32U(&headerInfoStream, normalListSize);
        PXDataStreamWriteI32U(&headerInfoStream, textureListSize);
        PXDataStreamWriteI32U(&headerInfoStream, parameterListSize);
        PXDataStreamWriteI32U(&headerInfoStream, indexListSize);
        PXDataStreamWriteI32U(&headerInfoStream, mtlInlclueesListSize);
        PXDataStreamWriteI64U(&headerInfoStream, mtlEmbeddedDataOffset);
    }

    outputStream->DataSize = outputStream->DataCursor;

    if (errorCounter)
    {
        return PXActionCompilingError;
    }

    return PXActionSuccessful;
}

PXActionResult PXWavefrontParseToModel(PXDataStream* const inputStream, PXModel* const model)
{
    PXModelConstruct(model);

    inputStream->DataCursor = 0;

    unsigned char numberOfMeshes = 0;
    unsigned char renderMode[256];
    unsigned int renderSize[256];

    unsigned int vertexListSize = 0;
    unsigned int normalListSize = 0;
    unsigned int textureListSize = 0;
    unsigned int parameterListSize = 0;
    unsigned int indexListSize = 0;
    unsigned int mtlInlclueesListSize = 0;
    PXSize mtlEmbeddedDataOffset = 0;

    PXDataStreamReadI8U(inputStream, &numberOfMeshes);

    MemoryClear(renderMode, sizeof(renderMode));
    MemoryClear(renderSize, sizeof(renderSize));

    for (PXSize i = 0; i < numberOfMeshes; ++i)
    {
        PXDataStreamReadI8U(inputStream, &renderMode[i]);
        PXDataStreamReadI32U(inputStream, &renderSize[i]);
    }

    PXDataStreamReadI32U(inputStream, &vertexListSize);
    PXDataStreamReadI32U(inputStream, &normalListSize);
    PXDataStreamReadI32U(inputStream, &textureListSize);
    PXDataStreamReadI32U(inputStream, &parameterListSize);
    PXDataStreamReadI32U(inputStream, &indexListSize);
    PXDataStreamReadI32U(inputStream, &mtlInlclueesListSize);
    PXDataStreamReadI64U(inputStream, &mtlEmbeddedDataOffset);

    model->DataVertexListSize = sizeof(float) * (vertexListSize + normalListSize + textureListSize + parameterListSize);

    PXSize expectedMaterialSize = (sizeof(PXMaterial) + 256u) * 100u;
    PXSize infoHeaderSize = sizeof(unsigned char) + numberOfMeshes * (sizeof(unsigned char) + sizeof(unsigned int)) + expectedMaterialSize;
    PXSize expectedSize = 40 * model->DataVertexListSize + infoHeaderSize;// +indexListSize;


    model->Data = MemoryAllocate(expectedSize);
    //---<End header>----------------------------------------------------------



    //---<Lookup materials>----------------------------------------------------

    {
        PXDataStream modelHeaderStream;

        PXDataStreamFromExternal(&modelHeaderStream, model->Data, infoHeaderSize);

        PXDataStreamWriteI8U(&modelHeaderStream, numberOfMeshes);

        for (PXSize meshIndex = 0; meshIndex < numberOfMeshes; ++meshIndex)
        {
            PXDataStreamWriteI8U(&modelHeaderStream, renderMode[meshIndex]); // Draw mode
            PXDataStreamWriteI32U(&modelHeaderStream, renderSize[meshIndex]); // Draw amount
            PXDataStreamWriteI32U(&modelHeaderStream,(unsigned int)-1); // Material ID, set later
        }
#if PXWavefrontDetectMaterial
        //---<MTL to PXMaterial>-----------------------------------------------
        mtlEmbeddedDataOffset += 1; // ???

        void* data = (PXAdress)inputStream->Data + mtlEmbeddedDataOffset;
        const PXSize size = inputStream->DataSize - mtlEmbeddedDataOffset;

        PXDataStreamWriteI32U(&modelHeaderStream, 0);

        if (mtlInlclueesListSize > 0)
        {
            const PXSize amount = MTLFetchAmount(data, size);

            model->MaterialList = PXDataStreamCursorPosition(&modelHeaderStream);

            PXDataStreamWriteI32U(&modelHeaderStream, amount);

            for (PXSize i = 0; i < amount; ++i)
            {
                MTLMaterial mtlMaterial;

                const PXBool succ = MTLFetchMaterial(data, size, i, &mtlMaterial);

                // Write PXMaterial format
                {
                    const PXSize positionSizeData = modelHeaderStream.DataCursor;

                    PXDataStreamWriteI16U(&modelHeaderStream, (unsigned short)-1); // Total size
                    PXDataStreamWriteI16U(&modelHeaderStream, mtlMaterial.NameSize); // Size of name
                    PXDataStreamWriteA(&modelHeaderStream, mtlMaterial.Name, mtlMaterial.NameSize); // Name

                    PXDataStreamWriteI16U(&modelHeaderStream, mtlMaterial.DiffuseTexturePathSize); // Size of filepath
                    PXDataStreamWriteA(&modelHeaderStream, mtlMaterial.DiffuseTexturePath, mtlMaterial.DiffuseTexturePathSize); // filepath

                    PXDataStreamWriteFV(&modelHeaderStream, mtlMaterial.Ambient, 3u);
                    PXDataStreamWriteFV(&modelHeaderStream, mtlMaterial.Diffuse, 3u);
                    PXDataStreamWriteFV(&modelHeaderStream, mtlMaterial.Specular, 3u);
                    PXDataStreamWriteFV(&modelHeaderStream, mtlMaterial.Emission, 3u);

                    {
                        const PXSize pxMaterialSize = modelHeaderStream.DataCursor - positionSizeData;

                        PXDataStreamWriteAtI16U(&modelHeaderStream, pxMaterialSize, positionSizeData);
                    }
                }
            }
        }
#endif
    }

    //-------------------------------------------------------------------------





    //---<Start parsing Data>-------------------------------------------------
    PXDataStream materialIDLookupStream;
    PXDataStream vertexArrayData;

    inputStream->DataCursor = 1024;
    model->DataVertexList = (PXAdress)model->Data + infoHeaderSize;

    PXDataStreamFromExternal(&materialIDLookupStream, (PXAdress)model->Data+1, infoHeaderSize);
    PXDataStreamFromExternal(&vertexArrayData, model->DataVertexList, expectedSize);


    // Format: Ver Nor Tx Colo
    //         XYZ XYZ XY RGBA
    //          3   3  2  4     = 12

    model->DataVertexWidth = 3u;
    model->DataNormalWidth = 3u;
    model->DataTextureWidth = 2u;
    model->DataColorWidth = 0u;
    model->DataIndexWidth = renderMode;

    model->DataVertexSize = vertexListSize;
    model->DataNormalSize = normalListSize;
    model->DataTextureSize = textureListSize;
    model->DataColorSize = 0;
    model->DataIndexSize = indexListSize;








    float* buffer = MemoryAllocateClear(expectedSize);
    //MemorySet(buffer, expectedSize, 0xFF);

    float* vertexValueList = buffer;
    PXSize vertexValueListOffset = 0;
    //MemorySet(vertexValueList, vertexListSize * sizeof(float), 0xAA);

    float* normalList = &vertexValueList[vertexListSize];
    PXSize normalListOffset = 0;
    //MemorySet(normalList, normalListSize * sizeof(float), 0xBB);

    float* textureList = &normalList[normalListSize];
    PXSize textureListOffset = 0;
    //MemorySet(textureList, textureListSize * sizeof(float), 0xCC);

    float* parameterList = &textureList[textureListSize];
    PXSize parameterListOffset = 0;
    //MemorySet(parameterList, 0 * sizeof(float), 0xDD);



    while (!PXDataStreamIsAtEnd(inputStream))
    {
        PXWavefrontLineType objLineType = PXWavefrontLineInvalid;

        {
            PXInt8U lineTypeID = 0;

            PXDataStreamReadI8U(inputStream, &lineTypeID); // Line Type

            objLineType = lineTypeID;
        }

        switch (objLineType)
        {
            //case PXWavefrontEmbeddedMTL:
            case PXWavefrontLineMaterialLibraryInclude:
            case PXWavefrontLineMaterialLibraryUse:
            case PXWavefrontLineObjectName:
            case PXWavefrontLineSmoothShading:
            case PXWavefrontLineObjectGroup:
            {
                PXCompilerSymbolLexer compilerSymbolLexer;

                {
                    PXInt8U compilerSymbolLexerID = 0;

                    PXDataStreamReadI8U(inputStream, &compilerSymbolLexerID); // following datatype

                    compilerSymbolLexer = compilerSymbolLexerID;
                }

                switch (compilerSymbolLexer)
                {
                    case PXCompilerSymbolLexerInteger:
                    {
                        unsigned int value = 0;
                        PXDataStreamReadI32U(inputStream, &value);
                        break;
                    }

                    case PXCompilerSymbolLexerString:
                    {
                        unsigned short size = 0;
                        char* name = model->Data;

                        if (PXWavefrontLineMaterialLibraryUse == objLineType)
                        {
#if PXWavefrontDetectMaterial
                            char materialName[256];

                            void* data = (PXAdress)inputStream->Data + mtlEmbeddedDataOffset;
                            const PXSize sizeEE = inputStream->DataSize - mtlEmbeddedDataOffset;
                            const PXSize amount = MTLFetchAmount(data, sizeEE);

                            PXDataStreamReadI16U(inputStream, &size);
                            PXDataStreamReadTextA(inputStream, materialName, size);

                            PXDataStreamCursorAdvance(&materialIDLookupStream, sizeof(unsigned char) + sizeof(unsigned int));

                            // Lookup material
                            for (PXSize i = 0; i < amount; ++i)
                            {
                                PXMaterial pxMaterial;

                                PXModelMaterialGet(model, i, &pxMaterial);

                                const PXBool isValid = PXTextCompareA(materialName, size, pxMaterial.Name, pxMaterial.NameSize); // is found?

                                if (isValid)
                                {
                                    PXDataStreamWriteI32U(&materialIDLookupStream, i); // Material ID, set later
                                    break;
                                }

                            }
#else
                            PXDataStreamReadI16U(inputStream, &size);
                            PXDataStreamReadTextA(inputStream, name, size);
#endif
                        }
                        else
                        {
                           // unsigned char tttt[260];

                            PXDataStreamReadI16U(inputStream, &size);
                            PXDataStreamCursorAdvance(inputStream, size);
                            //PXDataStreamReadTextA(inputStream, tttt, size);
                        }

                        break;
                    }

                    default:
                        break;
                }

                break; // [OK]
            }
            case PXWavefrontLineVertexTexture:
            case PXWavefrontLineVertexGeometric:
            case PXWavefrontLineVertexNormal:
            case PXWavefrontLineVertexParameter:
            {
                PXInt8U amountofValues;
                float* adress;
                PXSize* offset;

                switch (objLineType)
                {
                    case PXWavefrontLineVertexTexture:
                        adress = textureList;
                        offset = &textureListOffset;
                        //printf("text ");
                        break;

                    case PXWavefrontLineVertexGeometric:
                        adress = vertexValueList;
                        offset = &vertexValueListOffset;
                        //printf("psotion ");
                        break;

                    case PXWavefrontLineVertexNormal:
                        adress = normalList;
                        offset = &normalListOffset;
                      // printf("norm ");
                        break;

                    case PXWavefrontLineVertexParameter:
                        adress = parameterList;
                        offset = &parameterListOffset;
                        break;

                    default:
                        adress = 0;
                        offset = 0;
                        break;
                }

                adress = &adress[*offset];

                PXDataStreamReadI8U(inputStream, &amountofValues);
                PXDataStreamReadFV(inputStream, adress, amountofValues);

                *offset += amountofValues;

                break;
            }

            case PXWavefrontLineFaceElement:
            {
                PXInt8U amountofValues = 0;

                PXDataStreamReadI8U(inputStream, &amountofValues);

                for (PXSize nodeIndex = 0; nodeIndex < amountofValues; ++nodeIndex)
                {
                    PXInt32U indexList[3] = { -1, -1, -1 };

                    PXDataStreamReadI32UV(inputStream, indexList, 3u);

                    const float* const vertexValueIndex = &vertexValueList[indexList[0] * (3u)];
                    const float* const textureValueIndex = &textureList[indexList[1] * (2u)];
                    const float* const normalValueIndex = &normalList[indexList[2] * (3u)];


#if 0
                    printf("[F] %i, %i, %i\n",indexList[0], indexList[1], indexList[2]);
#endif


#if 0
                    printf
                    (
                        "[F] V:%i,%i,%i T:%i,%i N:%i,%i,%i\n", 
                        vertexValueIndex[0],
                        vertexValueIndex[1],
                        vertexValueIndex[2],
                        textureValueIndex[0],
                        textureValueIndex[1],
                        normalValueIndex[0],
                        normalValueIndex[1],
                        normalValueIndex[2]
                    );
#endif

                    PXDataStreamWriteFV(&vertexArrayData, vertexValueIndex, 3u);
                    PXDataStreamWriteFV(&vertexArrayData, normalValueIndex, 3u);
                    PXDataStreamWriteFV(&vertexArrayData, textureValueIndex, 2u);
                }

                break;
            }
        }
    }

    return PXActionSuccessful;
}