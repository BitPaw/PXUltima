#include "OBJ.h"

#include <Compiler/PXCompiler.h>
#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>
#include <Container/ClusterValue.h>
#include <Text/Text.h>
#include <File/File.h>
#include <Math/PXMath.h>

#define OBJDetectMaterial 1

#define PXCompilerSymbolLexerOBJMaterialLibraryIncludeID 'I'
#define PXCompilerSymbolLexerOBJMaterialLibraryUselID 'U'
#define PXCompilerSymbolLexerOBJObjectNameID 'O'
#define PXCompilerSymbolLexerOBJSmoothShadingID 'S'
#define PXCompilerSymbolLexerOBJObjectGroupID 'G'
#define PXCompilerSymbolLexerOBJVertexGerometricID 'v'
#define PXCompilerSymbolLexerOBJVertexNormalID 'n'
#define PXCompilerSymbolLexerOBJVertexParameterID 'p'
#define PXCompilerSymbolLexerOBJVertexTextureID 't'

void OBJElementConstruct(OBJElement* objElement)
{
    MemoryClear(objElement, sizeof(OBJElement));
}

void OBJElementDestruct(OBJElement* objElement)
{
    //TODO: clear memeory
}

void OBJConstruct(OBJ* const obj)
{
    MemoryClear(obj, sizeof(OBJ));
}

void OBJDestruct(OBJ* const obj)
{
    MemoryRelease(obj->ElementList, obj->ElementListSize);

    MemoryRelease(obj->MaterialFileList, obj->MaterialFileListSize);
}

OBJLineType OBJPeekLine(const void* line, const PXSize size)
{
    const char* const text = (const char* const)line;

    switch (size)
    {
        case 1:
        {
            switch (text[0])
            {
                case 'v': return OBJLineVertexGeometric;
                case 'f': return OBJLineFaceElement;
                case '#': return OBJLineComment;
                case 'o': return OBJLineObjectName;
                case 's': return OBJLineSmoothShading;
                case 'g': return OBJLineObjectGroup;
            }

            break;
        }
        case 2:
        {
            const unsigned short lineTagID = MakeShort(text[0], text[1]);

            switch (lineTagID)
            {
                case MakeShort('v', 't'): return OBJLineVertexTexture;
                case MakeShort('v', 'n'): return OBJLineVertexNormal;
                case MakeShort('v', 'p'): return OBJLineVertexParameter;
            }

            break;
        }

        case 6:
        {
            const unsigned long long lineTagID = MakeInt(text[0], text[1], text[2], text[3]);

            switch (lineTagID)
            {
                case MakeInt('m', 't', 'l', 'l'):
                case MakeInt('u', 's', 'e', 'm'):
                {
                    const unsigned short lineTagID = MakeShort(text[4], text[5]);

                    switch (lineTagID)
                    {
                        case MakeShort('i', 'b'): return OBJLineMaterialLibraryInclude;
                        case MakeShort('t', 'l'): return OBJLineMaterialLibraryUse;
                    }

                    break;
                }
            }

            break;
        }
    }

    return OBJLineInvalid;
}

void OBJCompileError(PXCompilerSymbolEntry* const compilerSymbolEntry, unsigned int expectedID)
{
    char textBuffer[32];

    TextCopyA(compilerSymbolEntry->Source, 20, textBuffer, 32);

    printf
    (
        "[OBJ][Error] At line <%i> at <%i>, unexpected symbol.\n"
        "          -> %s\n",
        compilerSymbolEntry->Line,
        compilerSymbolEntry->Coloum,
        textBuffer
    );
}

PXActionResult OBJFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
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
        compilerSettings.KeepWhiteSpace = PXNo;
        compilerSettings.KeepWhiteSpaceIndentationLeft = PXNo;
        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.IntrepredNewLineAsWhiteSpace = PXNo;
        compilerSettings.KeepTabs = PXNo;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";
        compilerSettings.CommentMultibleLineSize = 0;
        compilerSettings.CommentMultibleLine = 0;

        outputStream->DataCursor += headerOffset;

        PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

        PXDataStreamFromExternal(&tokenSteam, (unsigned char*)outputStream->Data + headerOffset, outputStream->DataCursor- headerOffset);

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

        const OBJLineType objPeekLine = OBJPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        PXDataStreamWriteI8U(outputStream, objPeekLine);

        switch (objPeekLine)
        {
            case OBJLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

                PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                PXDataStreamWriteI32U(outputStream, compilerSymbolEntry.DataI);

                break;
            }
            case OBJLineMaterialLibraryInclude:
            case OBJLineMaterialLibraryUse:
            case OBJLineObjectName:
            case OBJLineObjectGroup:
            {
                char namedElement[256];
                PXSize namedElementSize = 0;

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, &compilerSymbolEntry, namedElement, 256, &namedElementSize);

                if (!isString)
                {
                    ++errorCounter;
                    OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                PXDataStreamWriteI8U(outputStream, PXCompilerSymbolLexerString);
                PXDataStreamWriteI16U(outputStream, namedElementSize);
                PXDataStreamWriteA(outputStream, namedElement, namedElementSize);

                switch (objPeekLine)
                {
                    case OBJLineMaterialLibraryInclude:
                    {
                        ++mtlInlclueesListSize;

#if OBJDetectMaterial

                        headerCacheOffset += PXDataStreamWriteAtI16U(outputStream, namedElementSize, headerCacheOffset);
                        headerCacheOffset += PXDataStreamWriteAtB(outputStream, namedElement, namedElementSize, headerCacheOffset);
#endif

                        break;
                    }
                    case OBJLineMaterialLibraryUse:
                    {
                        drawOrder[drawCurrentIndex++] = drawCurrentCounter;
                        drawCurrentCounter = 0;
                        break;
                    }
                }

                break; // [OK]
            }
            case OBJLineVertexTexture:
            case OBJLineVertexGeometric:
            case OBJLineVertexNormal:
            case OBJLineVertexParameter:
            {
                PXSize valuesDetected = 0;
                float vector[4] = { -1, -1, -1, -1 };

                const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, &compilerSymbolEntry, vector, 4u, &valuesDetected);

                // Export
                {
                    switch (objPeekLine)
                    {
                        case OBJLineVertexGeometric:
                            vertexListSize += valuesDetected;
                            break;

                        case OBJLineVertexNormal:
                            normalListSize += valuesDetected;
                            break;

                        case OBJLineVertexParameter:
                            parameterListSize += valuesDetected;
                            break;

                        case OBJLineVertexTexture:
                            textureListSize += valuesDetected;
                            break;
                    }

                    PXDataStreamWriteI8U(outputStream, valuesDetected);
                    PXDataStreamWriteFV(outputStream, vector, valuesDetected);
                }

                break; // [OK]
            }
            case OBJLineFaceElement:
            {
                PXSize cornerPoints = 0;
                PXSize cursorPos = outputStream->DataCursor;

                PXDataStreamWriteI8U(outputStream, 0xFF);

                while (!PXDataStreamIsAtEnd(&tokenSteam))
                {
                    PXInt32U vertexData[3] = { -1, -1, -1 };

                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                    if (compilerSymbolEntry.ID != PXCompilerSymbolLexerGenericElement)
                    {
                        break;
                    }

                    PXSize offsetA = TextToIntA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, &vertexData[0]);

                    const char* expectedSlash = compilerSymbolEntry.Source + offsetA;
                    const PXBool isSlashA = expectedSlash[0] == '/';

                    if (isSlashA) // not "f 1 2 3"
                    {
                        const char* nextValueAdress = expectedSlash + 1u; // Offset to possible next /
                        const PXBool isSlashB = nextValueAdress[0] == '/';

                        if (isSlashB) // 7//1  ->   // texture is not set
                        {
                            ++nextValueAdress;
                        }
                        else
                        {
                            PXSize offsetB = TextToIntA(nextValueAdress, compilerSymbolEntry.Size, &vertexData[1]);
                            nextValueAdress = nextValueAdress + offsetB;
                            const PXBool isSlashC = nextValueAdress[0] == '/';

                            if (!isSlashC)
                            {
                                // error
                            }
                            else
                            {
                                ++nextValueAdress; // Skip the '/'
                            }
                            //else ok
                        }

                        PXSize offsetB = TextToIntA(nextValueAdress, compilerSymbolEntry.Size, &vertexData[2]); // read

                        if (!offsetB)
                        {
                            ++errorCounter;
                            OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                        }

                        for (PXSize i = 0; i < 3u; ++i)
                        {
                            vertexData[i] -= 1u;
                        }
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
                OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);

                do
                {
                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                }
                while (compilerSymbolEntry.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }

    // End of OBJ parsing
    drawOrder[drawCurrentIndex] = drawCurrentCounter;
    mtlEmbeddedDataOffset = outputStream->DataCursor;

    // Reset for header fetching
    //const PXSize offset = outputStream->DataCursor; // Save old position
    //outputStream->DataCursor = 0; // Jump to beginning, parsing temp headerInfo

    // Begin loading MTL files
#if OBJDetectMaterial
    {
        PXDataStream materialNameFetchStream;
        PXDataStream materialFileStream;

        PXDataStreamFromExternal(&materialNameFetchStream, outputStream->Data, outputStream->DataCursor);

        wchar_t currentWorkPath[PathMaxSize];
        wchar_t currentMTLFileW[PathMaxSize];
        char currentMTLFileA[PathMaxSize];

        const PXBool succ = PXDataStreamFilePathGetW(inputStream, currentWorkPath, PathMaxSize); // Work OBJ file path

        for (PXSize i = 0; i < mtlInlclueesListSize; ++i)
        {
            unsigned short length = 0;

            PXDataStreamWriteI8U(outputStream, OBJEmbeddedMTL);

            PXDataStreamReadI16U(&materialNameFetchStream, &length);
            PXDataStreamReadB(&materialNameFetchStream, currentMTLFileA, PathMaxSize);

            TextCopyAW(currentMTLFileA, length, currentMTLFileW, PathMaxSize);

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

PXActionResult OBJParseToModel(PXDataStream* const inputStream, PXModel* const model)
{
    ModelConstruct(model);

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
#if OBJDetectMaterial
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
        OBJLineType objLineType = OBJLineInvalid;

        {
            PXInt8U lineTypeID = 0;

            PXDataStreamReadI8U(inputStream, &lineTypeID); // Line Type

            objLineType = lineTypeID;
        }

        switch (objLineType)
        {
            //case OBJEmbeddedMTL:
            case OBJLineMaterialLibraryInclude:
            case OBJLineMaterialLibraryUse:
            case OBJLineObjectName:
            case OBJLineSmoothShading:
            case OBJLineObjectGroup:
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

                        if (OBJLineMaterialLibraryUse == objLineType)
                        {
#if OBJDetectMaterial
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

                                const PXBool isValid = TextCompareA(materialName, size, pxMaterial.Name, pxMaterial.NameSize); // is found?

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
            case OBJLineVertexTexture:
            case OBJLineVertexGeometric:
            case OBJLineVertexNormal:
            case OBJLineVertexParameter:
            {
                PXInt8U amountofValues;
                float* adress;
                PXSize* offset;

                switch (objLineType)
                {
                    case OBJLineVertexTexture:
                        adress = textureList;
                        offset = &textureListOffset;
                        //printf("text ");
                        break;

                    case OBJLineVertexGeometric:
                        adress = vertexValueList;
                        offset = &vertexValueListOffset;
                        //printf("psotion ");
                        break;

                    case OBJLineVertexNormal:
                        adress = normalList;
                        offset = &normalListOffset;
                      // printf("norm ");
                        break;

                    case OBJLineVertexParameter:
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

            case OBJLineFaceElement:
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