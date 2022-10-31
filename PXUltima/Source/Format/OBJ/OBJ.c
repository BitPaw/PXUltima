#include "OBJ.h"

#include <Compiler/Compiler.h>
#include <File/DataStream.h>
#include <Memory/PXMemory.h>
#include <Container/ClusterValue.h>
#include <Text/Text.h>
#include <File/File.h>
#include <Math/PXMath.h>

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

    size_t drawoffsetCounter = 0;
    size_t drawCurrentCounter = 0;
    size_t drawCurrentIndex = 0;

    unsigned char drawSize[256];
    size_t drawOrder[256];

    MemorySet(drawSize, sizeof(drawSize), 0);
    MemorySet(drawOrder, sizeof(drawOrder), 0);

    const size_t headerOffset = 256;

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
            case OBJLineMaterialLibraryInclude:
            case OBJLineMaterialLibraryUse:
            case OBJLineObjectName:
            case OBJLineSmoothShading:
            case OBJLineObjectGroup:
            {
                char namedElement[128];
                size_t namedElementSize = 0;

                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.    

                switch (compilerSymbolEntry.ID)
                {
                    case PXCompilerSymbolLexerInteger:
                    {
                        DataStreamWriteCU(outputStream, PXCompilerSymbolLexerInteger);
                        DataStreamWriteIU(outputStream, compilerSymbolEntry.DataI, EndianLittle);
                        break;
                    }

                    case PXCompilerSymbolLexerGenericElement:
                    {
                        namedElementSize = TextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, namedElement, 128);

                        char* nameAdressStart = compilerSymbolEntry.Source;

                        while (1u)
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

                            const PXBool isElement = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                            if (isElement)
                            {
                                const size_t namelength = ((size_t)compilerSymbolEntry.Source + (size_t)compilerSymbolEntry.Size) - (size_t)nameAdressStart;

                                namedElementSize = TextCopyA(nameAdressStart, namelength, namedElement, 128);
                            }
                            else // Should be expected new line Error
                            {
                                break;
                            }
                        }

                        DataStreamWriteCU(outputStream, PXCompilerSymbolLexerString);
                        DataStreamWriteSU(outputStream, namedElementSize, EndianLittle);
                        DataStreamWriteP(outputStream, namedElement, namedElementSize);

                        break;
                    }
                    default:
                    {
                        ++errorCounter;
                        OBJCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                        break;
                    }                      
                }          

                if (objPeekLine == OBJLineMaterialLibraryUse)
                {
                    drawOrder[drawCurrentIndex++] = drawCurrentCounter;
                }

                break; // [OK]
            }    
            case OBJLineVertexTexture:
            case OBJLineVertexGeometric:            
            case OBJLineVertexNormal:
            case OBJLineVertexParameter:
            {
                size_t valuesDetected = 0;
                PXBool isNumber[4] = { 0u, 0u, 0u, 0u };
                float vector[4] = { -1, -1, -1, -1 };

                while (1u)
                {
                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                    // [symbol check] Check if symbol is expected name
                    {
                       const PXBool isFloat = compilerSymbolEntry.ID == PXCompilerSymbolLexerFloat;

                        if (!isFloat)
                        {
                            // we are at the line end

                            break; // finished
                        }
                    }

                    vector[valuesDetected] = compilerSymbolEntry.DataF;
                    isNumber[valuesDetected] = PXYes;

                    ++valuesDetected;
                }          


                // Export
                {
                    switch (objPeekLine)
                    {
                        case OBJLineVertexGeometric:
                            vertexListSize += valuesDetected;
                           // printf("position: ");
                            break;

                        case OBJLineVertexNormal:
                            normalListSize += valuesDetected;
                            //printf("norm: ");
                            break;

                        case OBJLineVertexParameter:
                            parameterListSize += valuesDetected;
                            break;

                        case OBJLineVertexTexture:
                            textureListSize += valuesDetected;
                           // printf("text: ");
                            break;
                    }

                    DataStreamWriteCU(outputStream, valuesDetected);
                    DataStreamWriteP(outputStream, vector, sizeof(float) * valuesDetected);

#if 0
                    for (size_t i = 0; i < valuesDetected; ++i)
                    {
                        printf("%f, ", vector[i]);
                    }

                    printf("\n");
#endif
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

                ++drawCurrentCounter;

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

    drawOrder[drawCurrentIndex] = drawCurrentCounter;



    size_t offset = outputStream->DataCursor;

    outputStream->DataCursor = 0;

    DataStreamWriteCU(outputStream, drawCurrentIndex+1);

    for (size_t i = 0; i < drawCurrentIndex + 1; ++i)
    {
        DataStreamWriteCU(outputStream, drawSize[i]);
        DataStreamWriteIU(outputStream, MathAbsoluteI((int)drawOrder[i+1] - (int)drawOrder[i]), EndianLittle);
    }  

    DataStreamWriteIU(outputStream, vertexListSize, EndianLittle);
    DataStreamWriteIU(outputStream, normalListSize, EndianLittle);
    DataStreamWriteIU(outputStream, textureListSize, EndianLittle);
    DataStreamWriteIU(outputStream, parameterListSize, EndianLittle);
    DataStreamWriteIU(outputStream, indexListSize, EndianLittle);

    outputStream->DataCursor = offset;


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


    DataStreamReadCU(inputStream, &numberOfMeshes);

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

    inputStream->DataCursor = 256;

    size_t infoHeader = sizeof(unsigned char) + numberOfMeshes * (sizeof(unsigned char) + sizeof(unsigned int));

    size_t expectedSize = 5*sizeof(float) * (vertexListSize + normalListSize + textureListSize + parameterListSize) + infoHeader;// +indexListSize;



    model->Data = MemoryAllocateClear(expectedSize * 20);

    MemorySet(model->Data, expectedSize * 20, 0xFF);

    // Format: Ver Nor Tx Colo
    //         XYZ XYZ XY RGBA
    //          3   3  2  4     = 12
    //      


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

    model->DataVertex = (unsigned char*)model->Data + infoHeader;

    DataStream vertexArrayData;

    DataStreamFromExternal(&vertexArrayData, model->DataVertex, expectedSize*20);




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
        OBJLineType objLineType;     

        {
            unsigned char lineTypeID = 0;

            DataStreamReadCU(inputStream, &lineTypeID); // Line Type     

            objLineType = lineTypeID;
        }
   
        switch (objLineType)
        {
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

                        DataStreamReadSU(inputStream, &size, EndianLittle);
                        DataStreamReadP(inputStream, name, size);

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
                       // printf("text ");
                        break;

                    case OBJLineVertexGeometric:
                        adress = vertexValueList;
                        offset = &vertexValueListOffset;
                      //  printf("psotion ");
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

#if 0
                for (size_t i = 0; i < amountofValues; i++)
                {
                    printf("%f, ", adress[i]);

                }

                printf("\n");
#endif

                *offset += amountofValues;

                break;
            }

            case OBJLineFaceElement:
            {

 // Format: Ver Nor Tx Colo
 //         XYZ XYZ XY RGBA
 //          3   3  2  4     = 12
 // 

                unsigned char amountofValues = 0;            

                DataStreamReadCU(inputStream, &amountofValues);

                for (size_t i = 0; i < amountofValues; ++i)
                {
                    unsigned int indexList[3] = { -1, -1, -1, };

                    DataStreamReadIU(inputStream, &indexList[0], EndianLittle);
                    DataStreamReadIU(inputStream, &indexList[1], EndianLittle);
                    DataStreamReadIU(inputStream, &indexList[2], EndianLittle);

                    const float* vertexValueIndex = &vertexValueList[indexList[0] * 3u + i];                   
                    const float* textureValueIndex = &textureList[indexList[1] * 2u + i];
                    const float* normalValueIndex = &normalList[indexList[2] * 3u + i];

                    //printf("Face _> %i, %i, %i\n", indexList[0], indexList[1], indexList[2]);

                    float* xDat = (float*)DataStreamCursorPosition(&vertexArrayData);
                    size_t xDatSize = 0;

                    DataStreamWriteP(&vertexArrayData, vertexValueIndex, sizeof(float) * 3u);
                    DataStreamWriteP(&vertexArrayData, normalValueIndex, sizeof(float) * 3u);
                    DataStreamWriteP(&vertexArrayData, textureValueIndex, sizeof(float) * 2u);
                              
                    xDatSize = vertexArrayData.DataCursor;

#if 0
                    printf("---\n");

                    for (size_t i = 0; i < 8u; i++)
                    {
                       printf("[%i] %f\n", i, xDat[i]);
                    }
                    printf("---\n");
#endif
                } 

                break;
            }
        }
    }

    return ActionSuccessful;
}

ActionResult OBJParseEEE(OBJ* obj, const void* data, const size_t dataSize, size_t* dataRead, const wchar_t* fileName)
{
    DataStream dataStream;

    DataStreamConstruct(&dataStream);
    DataStreamFromExternal(&dataStream, (void*)data, dataSize);
    OBJConstruct(obj);
    *dataRead = 0;

    unsigned char isFirstVertex = 0;

    obj->ElementListSize = 1;
    obj->ElementList = (OBJElement*)MemoryAllocate(sizeof(OBJElement) * 1u);

    typedef struct OBJSegmentData_
    {
        size_t Position;
        size_t Texture;
        size_t Normal;
        size_t Parameter;
        size_t Face;
        size_t Material;
    }
    OBJSegmentData;

    //---<Cound needed Space and allocate>----------------------------------
    {
        OBJSegmentData segmentData[128];
        size_t segmentAmount = 0;
        size_t materialCounter = 0;

        OBJElement* currentSegment = obj->ElementList;
        unsigned char isInMesh = 0;

        MemorySet(segmentData, sizeof(OBJSegmentData) * 128, 0);

        do
        {
            const char* currentLine = (const char*)DataStreamCursorPosition(&dataStream);
            const OBJLineType command = OBJPeekLine(currentLine, 0);

            OBJSegmentData* currentSegmentData = &segmentData[segmentAmount];

            switch (command)
            {
            case OBJLineMaterialLibraryInclude:
                ++materialCounter;
                break;

            case OBJLineMaterialLibraryUse:
                ++currentSegmentData->Material;
                break;

            case OBJLineObjectName:
                ++segmentAmount;
                break;

            case OBJLineVertexGeometric:
                ++currentSegmentData->Position;
                break;

            case OBJLineVertexTexture:
                ++currentSegmentData->Texture;
                break;

            case OBJLineVertexNormal:
                ++currentSegmentData->Normal;
                break;

            case OBJLineVertexParameter:
                ++currentSegmentData->Parameter;
                break;

            case OBJLineFaceElement:
            {
                const size_t amount = TextCountUntilA(currentLine + 2, DataStreamRemainingSize(&dataStream), '/', '\n') / 2;

                currentSegmentData->Face += amount;

                isInMesh = 1u;

                if (obj->VertexStructureSize < amount)
                    obj->VertexStructureSize = amount;

                break;
            }
            }
        } while (DataStreamSkipLine(&dataStream));

        DataStreamCursorToBeginning(&dataStream);

        obj->MaterialFileListSize = materialCounter;
        obj->MaterialFileList = (MTL*)MemoryAllocate(sizeof(MTL) * materialCounter);

        MemorySet(obj->MaterialFileList, sizeof(MTL) * materialCounter, 0);

        for (size_t i = 0; i < segmentAmount; i++)
        {
            const OBJSegmentData* currentSegmentData = &segmentData[segmentAmount];

            OBJElement* segment = currentSegment;

            OBJElementConstruct(segment);

            segment->VertexPositionListSize = currentSegmentData->Position * 3u;
            segment->VertexPositionList = (float*)MemoryAllocate(sizeof(float) * segment->VertexPositionListSize);

            segment->TextureCoordinateListSize = currentSegmentData->Texture * 2u;
            segment->TextureCoordinateList = (float*)MemoryAllocate(sizeof(float) * segment->TextureCoordinateListSize);

            segment->VertexNormalPositionListSize = currentSegmentData->Normal * 3u;
            segment->VertexNormalPositionList = (float*)MemoryAllocate(sizeof(float) * segment->VertexNormalPositionListSize);

            segment->VertexParameterListSize = currentSegmentData->Parameter * 3u;
            segment->VertexParameterList = (float*)MemoryAllocate(sizeof(float) * segment->VertexParameterListSize);

            segment->FaceElementListSize = currentSegmentData->Face * 3u;
            segment->FaceElementList = (unsigned int*)MemoryAllocate(sizeof(unsigned int) * segment->FaceElementListSize);

            segment->MaterialInfoSize = currentSegmentData->Material;
            segment->MaterialInfo = (OBJElementMaterialInfo*)MemoryAllocate(sizeof(OBJElementMaterialInfo) * segment->MaterialInfoSize);

#if OBJDebug

            printf
            (
                "[OBJ][Segment (%li/%li)] V:%li T:%li N:%li P:%li F:%li M:%i\n",
                i + 1,
                segmentAmount,
                currentSegmentData->Position,
                currentSegmentData->Texture,
                currentSegmentData->Normal,
                currentSegmentData->Parameter,
                currentSegmentData->Face,
                currentSegmentData->Material
            );
#endif
        }
    }
    //--------------------------------------------------------------------

    //assert(VertexStructureSize == 3 || VertexStructureSize == 4);

    // Exact Parse
    {
        size_t currentPositionElement = 0;
        size_t currentTextureElement = 0;
        size_t currentNormalElement = 0;
        size_t currentParameterElement = 0;
        size_t currentFaceElement = 0;
        size_t materialIndex = 0;
        size_t materialIDCounter = 0;

        size_t elementIndex = -1;
        OBJElement* elemtentAdress = obj->ElementList;
        unsigned char isInMesh = 0;

        size_t materialInfoIndex = 0;
        size_t materialFaceCounter = 0;
        size_t materialFaceCounterMAX = 0;

        // Parse
        do
        {
            const char* const currentLine = (const char* const)DataStreamCursorPosition(&dataStream);
            const unsigned short lineTagID = MakeShort(currentLine[0], currentLine[1]);
            const OBJLineType command = OBJPeekLine(currentLine, 0);

            DataStreamSkipBlock(&dataStream);

            const char* const dataPoint = (const char* const)DataStreamCursorPosition(&dataStream);
            const size_t maximalSize = DataStreamRemainingSize(&dataStream);
            const size_t currentLineLength = TextLengthUntilA(dataPoint, maximalSize, '\n');

            OBJElement* currentElemtent = elemtentAdress;

            switch (command)
            {
            case OBJLineMaterialLibraryInclude:
            {
                char materialFilePathA[PathMaxSize];
                wchar_t materialFilePathW[PathMaxSize];
                wchar_t materialFilePathFullW[PathMaxSize];
                MTL* material = &obj->MaterialFileList[materialIndex++];

                // Parse materialPath
                {
                    TextParseA
                    (
                        dataPoint,
                        currentLineLength,
                        "s",
                        materialFilePathA
                    );
                }

                TextCopyAW(materialFilePathA, PathMaxSize, materialFilePathW, PathMaxSize);

                FilePathSwapFile(fileName, materialFilePathFullW, materialFilePathW);

                {

                    DataStream dataStreamMTL;

                    DataStreamConstruct(&dataStreamMTL);

                    {
                        const ActionResult fileLoadingResult = DataStreamMapToMemoryW(&dataStreamMTL, materialFilePathFullW, 0, MemoryReadOnly);
                        const unsigned char sucessful = fileLoadingResult == ActionSuccessful;

                        // if(!sucessful)
                        // {
                        //     return ResultInvalid;
                        // }
                    }
                    size_t readBytes = 0;
                    const ActionResult actionResult = MTLParse(material, dataStreamMTL.Data, dataStreamMTL.DataSize, &readBytes);
                    const unsigned char sucessful = actionResult == ActionSuccessful;

#if OBJDebug

                    if (sucessful)
                    {
                        printf("[+][MTL] Material (.mtl) file loaded <%ls>\n", materialFilePathFullW);
                    }
                    else
                    {
                        printf("[Warning] Material (.mtl) file is missing at path <%ls>\n", materialFilePathFullW);
                    }
#endif

                    DataStreamDestruct(&dataStreamMTL);
                }

                break;
            }
            case OBJLineMaterialLibraryUse:
            {
                char usedMaterialName[MTLNameSize];
                unsigned int materialID = -1;

                TextParseA
                (
                    dataPoint,
                    currentLineLength,
                    "s",
                    usedMaterialName
                );

                for (size_t i = 0; i < obj->MaterialFileListSize; ++i)
                {
                    const MTL* mtl = &obj->MaterialFileList[i];
                    const size_t materialListSize = mtl->MaterialListSize;

                    for (size_t j = 0; j < materialListSize; ++j)
                    {
                        const MTLMaterial* material = &mtl->MaterialList[j];
                        const unsigned char isSameName = TextCompareA(material->Name, MTLNameSize, usedMaterialName, MTLNameSize);

                        if (isSameName)
                        {
                            materialID = j;
                            break;
                        }
                    }
                }

                if (materialInfoIndex)
                {
                    OBJElementMaterialInfo* infoBefore = &currentElemtent->MaterialInfo[materialInfoIndex - 1];
                    OBJElementMaterialInfo* infoNew = &currentElemtent->MaterialInfo[materialInfoIndex++];

                    infoNew->MaterialIndex = materialID;
                    infoNew->Size = -1;

                    size_t newSize = (currentFaceElement * 3);

                    infoBefore->Size = materialFaceCounter;

                    materialFaceCounterMAX += materialFaceCounter;
                    materialFaceCounter = 0;

                    if (materialInfoIndex == currentElemtent->MaterialInfoSize)
                    {
                        // Last entry
                        infoNew->Size = currentElemtent->FaceElementListSize - materialFaceCounterMAX;
                    }
                }
                else
                {
                    OBJElementMaterialInfo* info = &currentElemtent->MaterialInfo[materialInfoIndex++];

                    info->MaterialIndex = materialID;
                    info->Size = -1;
                }

                break;
            }

            case OBJLineObjectName:
            {
                OBJElement* elemtentAdress = &obj->ElementList[elementIndex++];

                materialInfoIndex = 0;

                TextCopyA(dataPoint, currentLineLength, currentElemtent->Name, OBJElementNameLength);
                break;
            }

            case OBJLineVertexParameter:
            case OBJLineVertexNormal:
            case OBJLineVertexGeometric:
            {
                float* data = 0;

                switch (command)
                {
                case OBJLineVertexParameter:
                    data = &currentElemtent->VertexParameterList[currentParameterElement];
                    currentParameterElement += 3;
                    break;

                case OBJLineVertexNormal:
                    data = &currentElemtent->VertexNormalPositionList[currentNormalElement];
                    currentNormalElement += 3;
                    break;

                case OBJLineVertexGeometric:
                    data = &currentElemtent->VertexPositionList[currentPositionElement];
                    currentPositionElement += 3;
                    break;
                }

                //assert(currentVectorValue);

                TextParseA
                (
                    dataPoint,
                    currentLineLength,
                    "fff",
                    &data[0], // x
                    &data[1], // y
                    &data[2] // z
                );
                break;
            }

            case OBJLineVertexTexture:
            {
                float* data = &currentElemtent->TextureCoordinateList[currentTextureElement];

                currentTextureElement += 2u;

                TextParseA
                (
                    dataPoint,
                    currentLineLength,
                    "ff",
                    &data[0], // x
                    &data[1] // y
                );

                break;
            }
            case OBJLineSmoothShading:
                break;

            case OBJLineFaceElement:
            {
                unsigned int* data = &currentElemtent->FaceElementList[currentFaceElement];

                currentFaceElement += obj->VertexStructureSize * 3;
                materialFaceCounter += obj->VertexStructureSize * 3;

                switch (obj->VertexStructureSize)
                {
                case 3:
                {
                    TextParseA
                    (
                        dataPoint,
                        currentLineLength,
                        "u§u§uu§u§uu§u§u",
                        &data[0], &data[1], &data[2], // x, y, z (A)
                        &data[3], &data[4], &data[5], // x, y, z (B)
                        &data[6], &data[7], &data[8]  // x, y, z (C)
                    );
                    break;
                }
                case 4:
                {
                    TextParseA
                    (
                        dataPoint,
                        currentLineLength,
                        "u§u§u§uu§u§u§uu§u§u§uu§u§u§u",
                        &data[0], &data[1], &data[2], // x, y, z (A)
                        &data[3], &data[4], &data[5], // x, y, z (B)
                        &data[6], &data[7], &data[8],  // x, y, z (C)
                        &data[9], &data[10], &data[11]  // x, y, z (D)
                    );
                    break;
                }

                default:
                    break;
                }

                isInMesh = 1;

                break;
            }

            case OBJLineInvalid:
            case OBJLineNone:
            case OBJLineComment:
            default:
                break;
            }
        } while (DataStreamSkipLine(&dataStream));
    }

    return ActionSuccessful;
}