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
    MemorySet(objElement, sizeof(OBJElement), 0);
}

void OBJElementDestruct(OBJElement* objElement)
{
    //TODO: clear memeory
}

void OBJConstruct(OBJ* const obj)
{
    MemorySet(obj, sizeof(OBJ), 0);
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

    MemorySet(drawSize, sizeof(drawSize), 0);
    MemorySet(drawOrder, sizeof(drawOrder), 0);

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

        PXDataStreamWriteCU(outputStream, objPeekLine);

        switch (objPeekLine)
        {
            case OBJLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

                PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerInteger);
                PXDataStreamWriteIU(outputStream, compilerSymbolEntry.DataI, EndianLittle);

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

                PXDataStreamWriteCU(outputStream, PXCompilerSymbolLexerString);
                PXDataStreamWriteSU(outputStream, namedElementSize, EndianLittle);
                PXDataStreamWriteA(outputStream, namedElement, namedElementSize);

                switch (objPeekLine)
                {
                    case OBJLineMaterialLibraryInclude:
                    {
                        ++mtlInlclueesListSize;

#if OBJDetectMaterial

                        headerCacheOffset += PXDataStreamWriteAtSU(outputStream, namedElementSize, EndianLittle, headerCacheOffset);
                        headerCacheOffset += PXDataStreamWriteAtP(outputStream, namedElement, namedElementSize, headerCacheOffset);
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

                    PXDataStreamWriteCU(outputStream, valuesDetected);
                    PXDataStreamWriteP(outputStream, vector, sizeof(float) * valuesDetected);
                }

                break; // [OK]
            }
            case OBJLineFaceElement:
            {
                PXSize cornerPoints = 0;
                PXSize cursorPos = outputStream->DataCursor;

                PXDataStreamWriteCU(outputStream, 0xFF);

                while (!PXDataStreamIsAtEnd(&tokenSteam))
                {
                    unsigned int vertexData[3] = { -1, -1, -1 };

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
                        const char* nextValueAdress = expectedSlash + 1u;
                        const PXBool isSlashB = nextValueAdress[0] == '/';

                        if (isSlashB) // 7//1  ->   // texture is not set
                        {
                            ++nextValueAdress;
                        }
                        else
                        {
                            PXSize offsetB = TextToIntA(nextValueAdress, compilerSymbolEntry.Size, &vertexData[1]);
                            nextValueAdress = nextValueAdress + offsetB + 1u;
                            const PXBool isSlashC = nextValueAdress[0] == '/';

                            if (!isSlashC)
                            {
                                // error
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

                    PXDataStreamWriteP(outputStream, vertexData, sizeof(unsigned int) * 3u);

                  //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                    //----------------------------------

                    ++cornerPoints;
                }

                drawSize[drawCurrentIndex] = MathMaximum(drawSize[drawCurrentIndex], cornerPoints);

                indexListSize += cornerPoints;
                drawCurrentCounter += cornerPoints;

                PXDataStreamWriteAtCU(outputStream, cornerPoints, cursorPos);

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

            PXDataStreamWriteCU(outputStream, OBJEmbeddedMTL);

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

        PXDataStreamWriteCU(&headerInfoStream, drawCurrentIndex + 1);

        for (PXSize i = 0; i < drawCurrentIndex + 1; ++i)
        {
            PXDataStreamWriteCU(&headerInfoStream, drawSize[i+1]);
            PXDataStreamWriteIU(&headerInfoStream, MathAbsoluteI(drawOrder[i+1]), EndianLittle);
        }

        PXDataStreamWriteIU(&headerInfoStream, vertexListSize, EndianLittle);
        PXDataStreamWriteIU(&headerInfoStream, normalListSize, EndianLittle);
        PXDataStreamWriteIU(&headerInfoStream, textureListSize, EndianLittle);
        PXDataStreamWriteIU(&headerInfoStream, parameterListSize, EndianLittle);
        PXDataStreamWriteIU(&headerInfoStream, indexListSize, EndianLittle);
        PXDataStreamWriteIU(&headerInfoStream, mtlInlclueesListSize, EndianLittle);
        PXDataStreamWriteLLU(&headerInfoStream, mtlEmbeddedDataOffset, EndianLittle);
    }

    // outputStream->DataCursor = offset;

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

    PXSize expectedMaterialSize = (sizeof(PXMaterial) + 256)* 40;
    PXSize infoHeaderSize = sizeof(unsigned char) + numberOfMeshes * (sizeof(unsigned char) + sizeof(unsigned int)) + expectedMaterialSize;
    PXSize expectedSize = 40 * model->DataVertexListSize + infoHeaderSize;// +indexListSize;


    model->Data = MemoryAllocate(expectedSize);
    //---<End header>----------------------------------------------------------



    //---<Lookup materials>----------------------------------------------------

    {
        PXDataStream modelHeaderStream;

        PXDataStreamFromExternal(&modelHeaderStream, model->Data, infoHeaderSize);

        PXDataStreamWriteCU(&modelHeaderStream, numberOfMeshes);

        for (PXSize meshIndex = 0; meshIndex < numberOfMeshes; ++meshIndex)
        {
            PXDataStreamWriteCU(&modelHeaderStream, renderMode[meshIndex]); // Draw mode
            PXDataStreamWriteIU(&modelHeaderStream, renderSize[meshIndex], EndianLittle); // Draw amount
            PXDataStreamWriteIU(&modelHeaderStream,(unsigned int)-1, EndianLittle); // Material ID, set later
        }
#if OBJDetectMaterial
        //---<MTL to PXMaterial>-----------------------------------------------
        mtlEmbeddedDataOffset += 1; // ???

        void* data = (PXAdress)inputStream->Data + mtlEmbeddedDataOffset;
        const PXSize size = inputStream->DataSize - mtlEmbeddedDataOffset;

        PXDataStreamWriteIU(&modelHeaderStream, 0, EndianLittle);

        if (mtlInlclueesListSize > 0)
        {
            const PXSize amount = MTLFetchAmount(data, size);

            model->MaterialList = PXDataStreamCursorPosition(&modelHeaderStream);

            PXDataStreamWriteIU(&modelHeaderStream, amount, EndianLittle);

            for (PXSize i = 0; i < amount; ++i)
            {
                MTLMaterial mtlMaterial;

                const PXBool succ = MTLFetchMaterial(data, size, i, &mtlMaterial);

                // Write PXMaterial format
                {
                    const PXSize positionSizeData = modelHeaderStream.DataCursor;

                    PXDataStreamWriteSU(&modelHeaderStream, (unsigned short)-1, EndianLittle); // Total size
                    PXDataStreamWriteSU(&modelHeaderStream, mtlMaterial.NameSize, EndianLittle); // Size of name
                    PXDataStreamWriteA(&modelHeaderStream, mtlMaterial.Name, mtlMaterial.NameSize); // Name

                    PXDataStreamWriteSU(&modelHeaderStream, mtlMaterial.DiffuseTexturePathSize, EndianLittle); // Size of filepath
                    PXDataStreamWriteA(&modelHeaderStream, mtlMaterial.DiffuseTexturePath, mtlMaterial.DiffuseTexturePathSize); // filepath

                    PXDataStreamWriteP(&modelHeaderStream, mtlMaterial.Ambient, sizeof(float) * 3u);
                    PXDataStreamWriteP(&modelHeaderStream, mtlMaterial.Diffuse, sizeof(float) * 3u);
                    PXDataStreamWriteP(&modelHeaderStream, mtlMaterial.Specular, sizeof(float) * 3u);
                    PXDataStreamWriteP(&modelHeaderStream, mtlMaterial.Emission, sizeof(float) * 3u);

                    {
                        const PXSize pxMaterialSize = modelHeaderStream.DataCursor - positionSizeData;

                        PXDataStreamWriteAtSU(&modelHeaderStream, pxMaterialSize, EndianLittle, positionSizeData);
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
            unsigned char lineTypeID = 0;

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
                    unsigned char compilerSymbolLexerID = 0;

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
                                    PXDataStreamWriteIU(&materialIDLookupStream, i, EndianLittle); // Material ID, set later
                                    break;
                                }

                            }
#else
                            PXDataStreamReadI16U(inputStream, &size, EndianLittle);
                            PXDataStreamReadA(inputStream, name, size);
#endif
                        }
                        else
                        {
                            unsigned char tttt[260];

                            PXDataStreamReadI16U(inputStream, &size);
                            PXDataStreamReadTextA(inputStream, tttt, size);
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
                unsigned char amountofValues;
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
                PXDataStreamReadB(inputStream, adress, sizeof(float) * amountofValues);

                *offset += amountofValues;

                break;
            }

            case OBJLineFaceElement:
            {
                unsigned char amountofValues = 0;

                PXDataStreamReadI8U(inputStream, &amountofValues);

                for (PXSize nodeIndex = 0; nodeIndex < amountofValues; ++nodeIndex)
                {
                    PXInt32U indexList[3] = { -1, -1, -1 };

                    PXDataStreamReadI32UV(inputStream, indexList, 3u);

                    const float* vertexValueIndex = &vertexValueList[indexList[0] * (3u)];
                    const float* textureValueIndex = &textureList[indexList[1] * (2u)];
                    const float* normalValueIndex = &normalList[indexList[2] * (3u)];

                    PXDataStreamWriteP(&vertexArrayData, vertexValueIndex, sizeof(float) * 3u);
                    PXDataStreamWriteP(&vertexArrayData, normalValueIndex, sizeof(float) * 3u);
                    PXDataStreamWriteP(&vertexArrayData, textureValueIndex, sizeof(float) * 2u);
                }

                break;
            }
        }
    }

    return PXActionSuccessful;
}
