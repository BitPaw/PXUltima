#include "OBJ.h"

#include <Compiler/Compiler.h>
#include <File/DataStream.h>
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

OBJLineType OBJPeekLine(const void* line, const size_t size)
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

ActionResult OBJFileCompile(DataStream* const inputStream, DataStream* const outputStream)
{
    size_t errorCounter = 0;
    DataStream tokenSteam;   

    unsigned int vertexListSize = 0;
    unsigned int normalListSize = 0;
    unsigned int textureListSize = 0;
    unsigned int parameterListSize = 0;
    unsigned int indexListSize = 0;

    unsigned int mtlInlclueesListSize = 0;
    size_t mtlEmbeddedDataOffset = 0;

    size_t drawoffsetCounter = 0;
    size_t drawCurrentCounter = 0;
    size_t drawCurrentIndex = 0;

    unsigned char drawSize[256];
    size_t drawOrder[256];

    MemorySet(drawSize, sizeof(drawSize), 0);
    MemorySet(drawOrder, sizeof(drawOrder), 0);

    const size_t headerOffset = 1024;
    size_t headerCacheOffset = 0;

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

        DataStreamFromExternal(&tokenSteam, (unsigned char*)outputStream->Data + headerOffset, outputStream->DataCursor- headerOffset);

        outputStream->DataCursor = 0;
    }
       

    // Write 0'ed data to later jump back to to change.
    DataStreamCursorAdvance(outputStream, headerOffset);

    while (!DataStreamIsAtEnd(&tokenSteam))
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

        DataStreamWriteCU(outputStream, objPeekLine);

        switch (objPeekLine)
        {
            case OBJLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.    

                DataStreamWriteCU(outputStream, PXCompilerSymbolLexerInteger);
                DataStreamWriteIU(outputStream, compilerSymbolEntry.DataI, EndianLittle);

                break;
            }
            case OBJLineMaterialLibraryInclude:
            case OBJLineMaterialLibraryUse:
            case OBJLineObjectName:  
            case OBJLineObjectGroup:
            {
                char namedElement[256];
                size_t namedElementSize = 0;

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, &compilerSymbolEntry, namedElement, 256, &namedElementSize);

                if (!isString)
                {
                    ++errorCounter;
                    OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                DataStreamWriteCU(outputStream, PXCompilerSymbolLexerString);
                DataStreamWriteSU(outputStream, namedElementSize, EndianLittle);
                DataStreamWriteA(outputStream, namedElement, namedElementSize);

                switch (objPeekLine)
                {
                    case OBJLineMaterialLibraryInclude:
                    {
                        ++mtlInlclueesListSize;

#if OBJDetectMaterial

                        headerCacheOffset += DataStreamWriteAtSU(outputStream, namedElementSize, EndianLittle, headerCacheOffset);
                        headerCacheOffset += DataStreamWriteAtP(outputStream, namedElement, namedElementSize, headerCacheOffset);
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
                size_t valuesDetected = 0;
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

                    DataStreamWriteCU(outputStream, valuesDetected);
                    DataStreamWriteP(outputStream, vector, sizeof(float) * valuesDetected);
                }
           
                break; // [OK]
            }          
            case OBJLineFaceElement:
            {
                size_t cornerPoints = 0;
                size_t cursorPos = outputStream->DataCursor;

                DataStreamWriteCU(outputStream, 0xFF);

                while (!DataStreamIsAtEnd(&tokenSteam))
                {
                    unsigned int vertexData[3] = { -1, -1, -1 };

                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                    if (compilerSymbolEntry.ID != PXCompilerSymbolLexerGenericElement)
                    {
                        break;
                    }

                    size_t offsetA = TextToIntA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, &vertexData[0]);

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
                            size_t offsetB = TextToIntA(nextValueAdress, compilerSymbolEntry.Size, &vertexData[1]);
                            nextValueAdress = nextValueAdress + offsetB + 1u;
                            const PXBool isSlashC = nextValueAdress[0] == '/';

                            if (!isSlashC)
                            {
                                // error
                            } 
                            //else ok
                        }
                       
                        size_t offsetB = TextToIntA(nextValueAdress, compilerSymbolEntry.Size, &vertexData[2]); // read             
                        
                        if (!offsetB)
                        {
                            ++errorCounter;
                            OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                        }

                        for (size_t i = 0; i < 3u; ++i)
                        {
                            vertexData[i] -= 1u;
                        }
                    }

                    DataStreamWriteP(outputStream, vertexData, sizeof(unsigned int) * 3u);

                  //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                    //----------------------------------             
                    
                    ++cornerPoints;
                }

                drawSize[drawCurrentIndex] = MathMaximum(drawSize[drawCurrentIndex], cornerPoints);

                indexListSize += cornerPoints;
                drawCurrentCounter += cornerPoints;

                DataStreamWriteAtCU(outputStream, cornerPoints, cursorPos);

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
    //const size_t offset = outputStream->DataCursor; // Save old position
    //outputStream->DataCursor = 0; // Jump to beginning, parsing temp headerInfo

    // Begin loading MTL files    
#if OBJDetectMaterial
    {        
        DataStream materialNameFetchStream;
        DataStream materialFileStream;

        DataStreamFromExternal(&materialNameFetchStream, outputStream->Data, outputStream->DataCursor);
            
        wchar_t currentWorkPath[PathMaxSize];
        wchar_t currentMTLFileW[PathMaxSize];
        char currentMTLFileA[PathMaxSize];

        const PXBool succ = DataStreamFilePathGetW(inputStream, currentWorkPath, PathMaxSize); // Work OBJ file path

        for (size_t i = 0; i < mtlInlclueesListSize; ++i)
        {
            unsigned short length = 0;

            DataStreamWriteCU(outputStream, OBJEmbeddedMTL);

            DataStreamReadSU(&materialNameFetchStream, &length, EndianLittle);
            DataStreamReadP(&materialNameFetchStream, currentMTLFileA, PathMaxSize);

            TextCopyAW(currentMTLFileA, length, currentMTLFileW, PathMaxSize);

            FilePathSwapFileNameW(currentWorkPath, currentMTLFileW, currentMTLFileW);

            {
                const ActionResult materialFileLoadResult = DataStreamMapToMemoryW(&materialFileStream, currentMTLFileW, 0, MemoryReadOnly);
                const PXBool sucessful = ActionSuccessful == materialFileLoadResult;

                if (!sucessful)
                {
                    continue;
                }

                const ActionResult materialFileCompileResult = MTLFileCompile(&materialFileStream, outputStream);

                DataStreamDestruct(&materialFileStream);
            }
        }    
    }
#endif
    // MTL loading finished
    


    {
        DataStream headerInfoStream;

        DataStreamFromExternal(&headerInfoStream, outputStream->Data, outputStream->DataCursor);

        DataStreamWriteCU(&headerInfoStream, drawCurrentIndex + 1);

        for (size_t i = 0; i < drawCurrentIndex + 1; ++i)
        {
            DataStreamWriteCU(&headerInfoStream, drawSize[i+1]);
            DataStreamWriteIU(&headerInfoStream, MathAbsoluteI(drawOrder[i+1]), EndianLittle);
        }

        DataStreamWriteIU(&headerInfoStream, vertexListSize, EndianLittle);
        DataStreamWriteIU(&headerInfoStream, normalListSize, EndianLittle);
        DataStreamWriteIU(&headerInfoStream, textureListSize, EndianLittle);
        DataStreamWriteIU(&headerInfoStream, parameterListSize, EndianLittle);
        DataStreamWriteIU(&headerInfoStream, indexListSize, EndianLittle);
        DataStreamWriteIU(&headerInfoStream, mtlInlclueesListSize, EndianLittle);
        DataStreamWriteLLU(&headerInfoStream, mtlEmbeddedDataOffset, EndianLittle);
    }

    // outputStream->DataCursor = offset;

    if (errorCounter)
    {
        return ActionCompilingError;
    }

    return ActionSuccessful;
}

ActionResult OBJParseToModel(DataStream* const inputStream, Model* const model)
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
    size_t mtlEmbeddedDataOffset = 0;

    DataStreamReadCU(inputStream, &numberOfMeshes);

    MemorySet(renderMode, sizeof(renderMode), 0);
    MemorySet(renderSize, sizeof(renderSize), 0);

    for (size_t i = 0; i < numberOfMeshes; ++i)
    {
        DataStreamReadCU(inputStream, &renderMode[i]);
        DataStreamReadIU(inputStream, &renderSize[i], EndianLittle);
    }
    
    DataStreamReadIU(inputStream, &vertexListSize, EndianLittle);
    DataStreamReadIU(inputStream, &normalListSize, EndianLittle);
    DataStreamReadIU(inputStream, &textureListSize, EndianLittle);
    DataStreamReadIU(inputStream, &parameterListSize, EndianLittle);
    DataStreamReadIU(inputStream, &indexListSize, EndianLittle);
    DataStreamReadIU(inputStream, &mtlInlclueesListSize, EndianLittle);
    DataStreamReadLLU(inputStream, &mtlEmbeddedDataOffset, EndianLittle);

    size_t expectedMaterialSize = (sizeof(PXMaterial) + 256)* 40;
    size_t infoHeaderSize = sizeof(unsigned char) + numberOfMeshes * (sizeof(unsigned char) + sizeof(unsigned int)) + expectedMaterialSize;
    size_t expectedSize = 40 * sizeof(float) * (vertexListSize + normalListSize + textureListSize + parameterListSize) + infoHeaderSize;// +indexListSize;

    model->Data = MemoryAllocate(expectedSize);
    //---<End header>----------------------------------------------------------



    //---<Lookup materials>----------------------------------------------------

    {
        DataStream modelHeaderStream;      

        DataStreamFromExternal(&modelHeaderStream, model->Data, infoHeaderSize);

        DataStreamWriteCU(&modelHeaderStream, numberOfMeshes);

        for (size_t meshIndex = 0; meshIndex < numberOfMeshes; ++meshIndex)
        {
            DataStreamWriteCU(&modelHeaderStream, renderMode[meshIndex]); // Draw mode
            DataStreamWriteIU(&modelHeaderStream, renderSize[meshIndex], EndianLittle); // Draw amount
            DataStreamWriteIU(&modelHeaderStream,(unsigned int)-1, EndianLittle); // Material ID, set later   
        }
#if OBJDetectMaterial
        //---<MTL to PXMaterial>-----------------------------------------------
        mtlEmbeddedDataOffset += 1; // ???

        void* data = (unsigned char*)inputStream->Data + mtlEmbeddedDataOffset;
        const size_t size = inputStream->DataSize - mtlEmbeddedDataOffset;

        DataStreamWriteIU(&modelHeaderStream, 0, EndianLittle);

        if (mtlInlclueesListSize > 0)
        {
            const size_t amount = MTLFetchAmount(data, size);

            model->MaterialList = DataStreamCursorPosition(&modelHeaderStream);

            DataStreamWriteIU(&modelHeaderStream, amount, EndianLittle);

            for (size_t i = 0; i < amount; ++i)
            {
                MTLMaterial mtlMaterial;

                const PXBool succ = MTLFetchMaterial(data, size, i, &mtlMaterial);

                // Write PXMaterial format
                {
                    const size_t positionSizeData = modelHeaderStream.DataCursor;

                    DataStreamWriteSU(&modelHeaderStream, (unsigned short)-1, EndianLittle); // Total size
                    DataStreamWriteSU(&modelHeaderStream, mtlMaterial.NameSize, EndianLittle); // Size of name
                    DataStreamWriteA(&modelHeaderStream, mtlMaterial.Name, mtlMaterial.NameSize); // Name

                    DataStreamWriteSU(&modelHeaderStream, mtlMaterial.DiffuseTexturePathSize, EndianLittle); // Size of filepath
                    DataStreamWriteA(&modelHeaderStream, mtlMaterial.DiffuseTexturePath, mtlMaterial.DiffuseTexturePathSize); // filepath

                    DataStreamWriteP(&modelHeaderStream, mtlMaterial.Ambient, sizeof(float) * 3u);
                    DataStreamWriteP(&modelHeaderStream, mtlMaterial.Diffuse, sizeof(float) * 3u);
                    DataStreamWriteP(&modelHeaderStream, mtlMaterial.Specular, sizeof(float) * 3u);
                    DataStreamWriteP(&modelHeaderStream, mtlMaterial.Emission, sizeof(float) * 3u);

                    {
                        const size_t pxMaterialSize = modelHeaderStream.DataCursor - positionSizeData;

                        DataStreamWriteAtSU(&modelHeaderStream, pxMaterialSize, EndianLittle, positionSizeData);
                    }
                }
            }
        }    
#endif
    }

    //-------------------------------------------------------------------------





    //---<Start parsing Data>-------------------------------------------------
    DataStream materialIDLookupStream;
    DataStream vertexArrayData;

    inputStream->DataCursor = 1024;
    model->DataVertex = (unsigned char*)model->Data + infoHeaderSize;

    DataStreamFromExternal(&materialIDLookupStream, (PXAdress)model->Data+1, infoHeaderSize);
    DataStreamFromExternal(&vertexArrayData, model->DataVertex, expectedSize);
     

    // Format: Ver Nor Tx Colo
    //         XYZ XYZ XY RGBA
    //          3   3  2  4     = 12

    model->DataVertexWidth = 3u;
    model->DataNormalWidth = 3u;
    model->DataTextureWidth = 2u;
    model->DataColorWidth = 0u;
    model->DataIndexWidth = renderMode;

    model->DataVertexStride = sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
    model->DataNormalStride = sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
    model->DataTextureStride = sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
    model->DataColorStride = sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
    model->DataIndexStride = renderMode;

    model->DataVertexSize = vertexListSize;
    model->DataNormalSize = normalListSize;
    model->DataTextureSize = textureListSize;
    model->DataColorSize = 0;
    model->DataIndexSize = indexListSize;








    void* buffer = MemoryAllocateClear(expectedSize);
    MemorySet(buffer, expectedSize, 0xFF);

    float* vertexValueList = buffer;
    size_t vertexValueListOffset = 0;
    MemorySet(vertexValueList, vertexListSize * sizeof(float), 0xAA);

    float* normalList = &vertexValueList[vertexListSize];
    size_t normalListOffset = 0;
    MemorySet(normalList, normalListSize * sizeof(float), 0xBB);

    float* textureList = &normalList[normalListSize];
    size_t textureListOffset = 0;
    MemorySet(textureList, textureListSize * sizeof(float), 0xCC);

    float* parameterList = &textureList[textureListSize];
    size_t parameterListOffset = 0;
    MemorySet(parameterList, 0 * sizeof(float), 0xDD);



    while (!DataStreamIsAtEnd(inputStream))
    {
        OBJLineType objLineType = OBJLineInvalid;

        {
            unsigned char lineTypeID = 0;

            DataStreamReadCU(inputStream, &lineTypeID); // Line Type     

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

                    DataStreamReadCU(inputStream, &compilerSymbolLexerID); // following datatype

                    compilerSymbolLexer = compilerSymbolLexerID;
                }

                switch (compilerSymbolLexer)
                {
                    case PXCompilerSymbolLexerInteger:
                    {
                        unsigned int value = 0;
                        DataStreamReadIU(inputStream, &value, EndianLittle);
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
                            const size_t sizeEE = inputStream->DataSize - mtlEmbeddedDataOffset;
                            const size_t amount = MTLFetchAmount(data, sizeEE);

                            DataStreamReadSU(inputStream, &size, EndianLittle);
                            DataStreamReadA(inputStream, materialName, size);

                            DataStreamCursorAdvance(&materialIDLookupStream, sizeof(unsigned char) + sizeof(unsigned int));

                            // Lookup material
                            for (size_t i = 0; i < amount; ++i)
                            {
                                PXMaterial pxMaterial;

                                PXModelMaterialGet(model, i, &pxMaterial);

                                const PXBool isValid = TextCompareA(materialName, size, pxMaterial.Name, pxMaterial.NameSize); // is found?

                                if (isValid)
                                {
                                    DataStreamWriteIU(&materialIDLookupStream, i, EndianLittle); // Material ID, set later   
                                    break;
                                }
      
                            }
#else
                            DataStreamReadSU(inputStream, &size, EndianLittle);
                            DataStreamReadA(inputStream, name, size);
#endif
                        }
                        else
                        {
                            unsigned char tttt[260];

                            DataStreamReadSU(inputStream, &size, EndianLittle);
                            DataStreamReadA(inputStream, tttt, size);
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
                size_t* offset;

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

                DataStreamReadCU(inputStream, &amountofValues);
                DataStreamReadP(inputStream, adress, sizeof(float) * amountofValues);            

                *offset += amountofValues;

                break;
            }

            case OBJLineFaceElement:
            {
                unsigned char amountofValues = 0;            

                DataStreamReadCU(inputStream, &amountofValues);

                for (size_t nodeIndex = 0; nodeIndex < amountofValues; ++nodeIndex)
                {
                    unsigned int indexList[3] = { -1, -1, -1 };

                    DataStreamReadIU(inputStream, &indexList[0], EndianLittle);
                    DataStreamReadIU(inputStream, &indexList[1], EndianLittle);
                    DataStreamReadIU(inputStream, &indexList[2], EndianLittle);

                    const float* vertexValueIndex = &vertexValueList[indexList[0] * (3u)];
                    const float* textureValueIndex = &textureList[indexList[1] * (2u)];
                    const float* normalValueIndex = &normalList[indexList[2] * (3u)];

                    DataStreamWriteP(&vertexArrayData, vertexValueIndex, sizeof(float) * 3u);
                    DataStreamWriteP(&vertexArrayData, normalValueIndex, sizeof(float) * 3u);
                    DataStreamWriteP(&vertexArrayData, textureValueIndex, sizeof(float) * 2u);
                } 

                break;
            }
        }
    }

    return ActionSuccessful;
}