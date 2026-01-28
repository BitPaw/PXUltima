#include "PXWavefront.h"

#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Mesh/PXMesh.h>
#include <PX/Engine/ECS/Resource/Mesh/PXIndexBuffer.h>

#include <PX/Engine/ECS/Resource/Mesh/PXMeshGeometry.h>

#define PXWavefrontDetectMaterial 0

const char PXWaveFrontText[] = "WaveFront";
const char PXWaveFrontTextParsing[] = "Parsing";

PXWavefrontLineType PXAPI PXWavefrontPeekLine(const void* line, const PXSize size)
{
    if(!line || size == 0)
    {
        return PXWavefrontLineInvalid;
    }

    const char PXREF text = (const char PXREF)line;

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
            const PXI16U lineTagID = PXI16Make(text[0], text[1]);

            switch(lineTagID)
            {
                case PXI16Make('v', 't'):
                    return PXWavefrontLineVertexTexture;
                case PXI16Make('v', 'n'):
                    return PXWavefrontLineVertexNormal;
                case PXI16Make('v', 'p'):
                    return PXWavefrontLineVertexParameter;
            }

            break;
        }

        case 6:
        {
            const unsigned long long lineTagID = PXI32Make(text[0], text[1], text[2], text[3]);

            switch(lineTagID)
            {
                case PXI32Make('m', 't', 'l', 'l'):
                case PXI32Make('u', 's', 'e', 'm'):
                {
                    const unsigned short lineTagID = PXI16Make(text[4], text[5]);

                    switch(lineTagID)
                    {
                        case PXI16Make('i', 'b'):
                            return PXWavefrontLineMaterialLibraryIncluded;
                        case PXI16Make('t', 'l'):
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

void PXAPI PXWavefrontFaceLineParse(PXCompiler PXREF pxCompiler, PXI32U PXREF vertexData)
{
    const PXBool isExpectedInteger = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);

    if(!isExpectedInteger) // If not int => Error
    {
        return;
    }

    PXCompilerSymbolEntryForward(pxCompiler); // Consume int

    // Save 1st value
    vertexData[0] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;


    // Check
    {
        const PXCompilerSymbolLexer pxCompilerSymbolLexerList[2] =
        {
            PXCompilerSymbolLexerNumeric,
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
        case PXCompilerSymbolLexerNumeric: // is syntax A
        {
            PXCompilerSymbolEntryForward(pxCompiler); // Peek sucessful skip to 2nd integer

            vertexData[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U; // Save 2nd value

            const PXBool isExpectedThridInteger = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerNumeric); // Get 3rd integer

            if(!isExpectedThridInteger)
            {
                // Error;
                return;
            }

            PXCompilerSymbolEntryForward(pxCompiler);

            vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;

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

                    const PXBool isThridToken = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric); // Next token, expect int

                    if(!isThridToken) // if not int => error
                    {
                        // Error
                        return;
                    }

                    PXCompilerSymbolEntryForward(pxCompiler); // Consume 3rd number

                    vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;      // Store 3rd number

                    break;
                }
                case PXCompilerSymbolLexerNumeric: // Is syntax B
                {
                    PXCompilerSymbolEntryForward(pxCompiler); // Peek sucessful, remove the secound value

                    vertexData[1] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U; // Save value

                    // Exptect 2nd '/'
                    {
                        const PXBool isSlash = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerSlash); // Peek, expect '/'

                        if(isSlash)
                        {
                            PXCompilerSymbolEntryForward(pxCompiler);  // remove '/'

                            // Try get 3nd integer
                            {
                                const PXBool isSecoundToken = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);

                                if(!isSecoundToken)
                                {
                                    // Error
                                    return;
                                }

                                vertexData[2] = pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;

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

PXResult PXAPI PXWavefrontLoadFromFile(PXMesh PXREF pxMesh, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXFile* tokenSteam = PXFileCreate();

    struct PXWaveFrontCounter
    {
        PXI32U MaterialInlcude;
        PXI32U MaterialUse;

        // Vertex Data
        PXI32U Position;
        PXI32U Texture;
        PXI32U Normal;
        PXI32U Face;

        PXSize EntrysPosition;
        PXSize EntrysTexture;
        PXSize EntrysNormal;
        PXSize EntrysTotal;

        PXI32U FaceLast;
        PXI32U FaceTotal;

        // Index data
        PXI32U VertexMaxID;

        PXI32U ExpetcedInterleavedSizeInByte;
        PXI32U ExpetcedNonInterleavedSizeInByte;

        void* DataVertex;
        void* DataNormal;
        void* DataTexture;

        //void* IndexVertex;
        //void* IndexNormal;
        //void* IndexTexture;

        PXBool CanUseF16;
        PXI8U SizeOfVertexData;
    }
    counter;

    PXMemoryClear(&counter, sizeof(counter));

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileCurrent;
    pxCompiler.ReadInfo.FileCache = tokenSteam;
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes;

    PXTextFromAdressA(&pxCompiler.CommentSingleLine, "#", 1, 1);

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

    while(!PXFileIsAtEnd(tokenSteam))
    {
        PXCompilerSymbolEntryExtract(&pxCompiler); // First in line token

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

        switch(objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&pxCompiler); // Expect a name.

                // PXFileWriteI8U(outputStream, PXCompilerSymbolLexerNumeric);
                // PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);

                break;
            }
            case PXWavefrontLineMaterialLibraryIncluded:
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
                    case PXWavefrontLineMaterialLibraryIncluded:
                    {
                        ++counter.MaterialInlcude;
                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        ++counter.MaterialUse;
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
                PXF32 vector[4] = { -1, -1, -1, -1 };

                const PXBool listParsed = PXCompilerParseF32V(&pxCompiler, vector, valuesExpected, &valuesDetected);

                // Export
                {
                    switch(objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                            ++counter.Position;
                            //printf("|%6i| %3s | %12.6f | %12.6f | %12.6f |\n", compilerSymbolEntry.Line, "v", vector[0], vector[1], vector[2]);
                            //pxModel->DataVertexWidth = PXMathMaximum(pxModel->DataVertexWidth, valuesDetected);
                            break;

                        case PXWavefrontLineVertexNormal:
                            ++counter.Normal;
                            // printf("|%6i| %3s | %12.6f | %12.6f | %12.6f |\n", compilerSymbolEntry.Line, "vn", vector[0], vector[1], vector[2]);
                            //pxModel->DataNormalWidth = PXMathMaximum(pxModel->DataNormalWidth, valuesDetected);
                            break;

                        case PXWavefrontLineVertexParameter:
                            //pxModel-> += valuesDetected;
                            break;

                        case PXWavefrontLineVertexTexture:
                            ++counter.Texture;
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
                    PXI32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data

                    counter.VertexMaxID = PXMathMaximumIU(counter.VertexMaxID, vertexData[0]);
                    ++counter.Face;

                    PXCompilerSymbolEntryPeek(&pxCompiler); // check what line we are in

                    lineCurrent = pxCompiler.ReadInfo.SymbolEntryCurrent.Line; // Update current line

                    isDone = PXFileIsAtEnd(tokenSteam) || (lineStart != lineCurrent);
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

   // counter.CanUseF16 = PXTrue;


    const PXI32U type = PXTypeIFitting(counter.VertexMaxID);

    if(counter.CanUseF16)
    {
        counter.SizeOfVertexData = sizeof(PXF16);
    }
    else
    {
        counter.SizeOfVertexData = sizeof(PXF32);
    }

    counter.EntrysPosition = counter.Position * 3u;
    counter.EntrysNormal = counter.Normal * 3u;
    counter.EntrysTexture = counter.Texture * 2u;
    counter.EntrysTotal = counter.EntrysPosition + counter.EntrysTexture + counter.EntrysNormal;
    counter.FaceTotal = counter.Face;

    counter.ExpetcedInterleavedSizeInByte = counter.SizeOfVertexData * (counter.Face * (3 + 2 + 3));
    counter.ExpetcedNonInterleavedSizeInByte =
        (counter.SizeOfVertexData * counter.EntrysTotal) +
        PXTypeSizeGet(type) * counter.Face;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        PXWaveFrontTextParsing,
        "Analysed following sizes:\n"
        "%20s : x %i\n"
        "%20s : x %i\n"
        "%20s : x %i\n"
        "%20s : x %i\n"
        "%20s : x %i\n"
        "%20s : x %i (%i max)\n"
        "%20s : B %i\n"
        "%20s : B %i",
        "Material-Included", counter.MaterialInlcude,
        "Material-Use", counter.MaterialUse,
        "Position", counter.Position,
        "Texture", counter.Texture,
        "Normal", counter.Normal,
        "Face", counter.Face, counter.VertexMaxID,
        "Interleaved", counter.ExpetcedInterleavedSizeInByte,
        "Non-Interleaved", counter.ExpetcedNonInterleavedSizeInByte
    );
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        PXWaveFrontTextParsing,
        "Step 3, prealocate memory..."
    );
#endif

    //
    PXBool requireToCalculateNormals = PXFalse;

    // Stage - 2 - Allocate space
    {
        PXBufferLayoutEntry pxBufferLayoutEntry[6];
        PXClearList(PXBufferLayoutEntry, pxBufferLayoutEntry, 6);
        PXSize pxVertexBufferLayoutAmount = 0;
        PXSize pxIndexBufferLayoutAmount = 0;

        // TODO: we can check if we can use 16-bit floats here

        // Add positions
        {
            //pxVertexBufferLayout[0].AmountOfValues = counter.SizeOfVertexData * counter.EntrysPosition;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 3;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypePosition;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Position;

            ++pxVertexBufferLayoutAmount;
        }

        // Add normals
        if(counter.EntrysNormal > 0)
        {
          //  pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfValues = counter.SizeOfVertexData * counter.EntrysNormal;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 3;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeNormal;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Normal;

            ++pxVertexBufferLayoutAmount;
        }


        // Add texture units
        if(counter.EntrysTexture > 0)
        {
           // pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfValues = counter.SizeOfVertexData * counter.EntrysTexture;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 2;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeTexturePos;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Texture;

            ++pxVertexBufferLayoutAmount;
        }


        // Index buffer
        if(1)
        {
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 1;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypePosition;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Face;

            ++pxIndexBufferLayoutAmount;
        }
        if(1)
        {
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 1;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeTexturePos;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Face;

            ++pxIndexBufferLayoutAmount;
        }
        if(1)
        {
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].AmountOfElements = 1;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeNormal;
            pxBufferLayoutEntry[pxVertexBufferLayoutAmount].Entrys = counter.Face;

            ++pxIndexBufferLayoutAmount;
        }




        PXMeshGeometryCreateInfo pxMeshGeometryCreateInfo;
        PXClear(PXMeshGeometryCreateInfo, &pxMeshGeometryCreateInfo);
        pxMeshGeometryCreateInfo.VertexBufferLayoutAmount = pxVertexBufferLayoutAmount;
        pxMeshGeometryCreateInfo.VertexBufferLayoutList = pxBufferLayoutEntry;
        pxMeshGeometryCreateInfo.IndexBufferLayoutAmount = pxIndexBufferLayoutAmount;
        pxMeshGeometryCreateInfo.IndexBufferLayoutList = &pxBufferLayoutEntry[3];
        pxMeshGeometryCreateInfo.PrimitveAmount = counter.Face;
        pxMeshGeometryCreateInfo.SegmentAmount = counter.MaterialUse;

        PXMeshGeometryCreate(&pxMesh->Geometry, &pxMeshGeometryCreateInfo);

        pxMesh->MaterialContaierListAmount = counter.MaterialUse;
        pxMesh->MaterialContaierList = PXMemoryHeapCallocT(PXMaterialContainer, counter.MaterialUse);
     

        // Reset all size values 
       // counter.DataVertex = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
       // counter.DataNormal = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
       // counter.DataTexture = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

        counter.DataVertex = pxMesh->Geometry->VertexBufferPrime[0]->VertexData.Data;
        counter.DataNormal = pxMesh->Geometry->VertexBufferPrime[1]->VertexData.Data;
        counter.DataTexture = pxMesh->Geometry->VertexBufferPrime[2]->VertexData.Data;

        //counter.IndexVertex = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
        //counter.IndexNormal = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
        //counter.IndexTexture = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

        counter.Position = 0;
        counter.Normal = 0;
        counter.Texture = 0;
        counter.Face = 0;
        counter.MaterialUse = 0;
        counter.MaterialInlcude = 0;

        PXFileCursorToBeginning(tokenSteam);
    }

    // Stage - 3 - Extract data
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        PXWaveFrontTextParsing,
        "Step 4, extract data..."
    );
#endif

    PXI8U typeSize = PXTypeSizeGet(pxMesh->Geometry->IndexBuffer->DataType);

    const PXIndexBufferIndexSET pxIndexBufferIndexSET = PXIndexBufferIndexFunc(pxMesh->Geometry->IndexBuffer);

    for(;;)
    {
        const PXBool isAtEnd = PXFileIsAtEnd(tokenSteam);

        if(isAtEnd)
        {
            break;
        }

        PXCompilerSymbolEntryExtract(&pxCompiler);

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

        switch(objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&pxCompiler); // Expect a name
                break;
            }
            case PXWavefrontLineMaterialLibraryIncluded:
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
                    case PXWavefrontLineMaterialLibraryIncluded:
                    {
                        PXMaterialContainer PXREF pxMaterialContaier = &pxMesh->MaterialContaierList[counter.MaterialInlcude++];
                       
#if 0
                        PXFile* materialFile = PXFileCreate();

                        // Open and load
                        {
                            //-------------------------------
                            PXText materialFilePathFull;
                            PXTextConstructNamedBufferA(&materialFilePathFull, materialFilePathFullBuffer, PXPathSizeMax);

                            PXFilePathRelativeFromFile(pxResourceLoadInfo->FileCurrent, &materialFileName, &materialFilePathFull);

                            PXECSCreateInfo pxMaterialLoadInfo;
                            PXClear(PXECSCreateInfo, &pxMaterialLoadInfo);
                            pxMaterialLoadInfo.ResourceTarget = pxMaterialContaier;
                            pxMaterialLoadInfo.FileReference = materialFile;
                            pxMaterialLoadInfo.ResourceType = PXResourceTypeMaterialList;


                            PXECSResourceLoad(&pxMaterialLoadInfo, &materialFilePathFull);
                        }

                        PXFileClose(materialFile);
#endif

                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        PXIndexSegmentEntry PXREF pxIndexSegmentLast = 0;// &pxMesh->IndexBuffer->SegmentList[pxMesh->IndexBuffer->SegmentListAmount - 1];
                        PXIndexSegmentEntry PXREF pxIndexSegmentCurrent = 0;//&pxMesh->IndexBuffer->SegmentList[counter.MaterialUse];
                       
                        /*
                        pxIndexSegmentCurrent->Material = PXMaterialContainerFind
                        (
                            mesh->MaterialContaierList,
                            &materialFileName
                        );*/

                        if(counter.MaterialUse > 0)
                        {
                            PXIndexSegmentEntry PXREF pxIndexSegmentPrevious = 0;//&pxMesh->IndexBuffer->SegmentList[counter.MaterialUse - 1];
                            
                            pxIndexSegmentPrevious->DataRange = counter.Face - counter.FaceLast;
                            pxIndexSegmentLast->DataRange = PXMathAbsoluteI32(counter.Face - counter.FaceTotal);

                           counter.FaceLast = counter.Face;
                        }
                        else
                        {
                            pxIndexSegmentCurrent->DataRange = counter.Face;
                            counter.FaceLast = counter.Face;
                        }

                        ++counter.MaterialUse;

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
                void* insertPosition = PXNull;
                PXSize valuesDetected = 0;
                PXSize valuesExpected = 0;

                // Export
                {
                    switch(objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                        {
                            insertPosition = (PXByte*)counter.DataVertex + (counter.SizeOfVertexData * counter.Position * 3u);
                            valuesExpected = 3;
                            ++counter.Position;
                            break;
                        }
                        case PXWavefrontLineVertexNormal:
                        {
                            insertPosition = (PXByte*)counter.DataNormal + (counter.SizeOfVertexData * counter.Normal * 3u);
                            valuesExpected = 3;
                            ++counter.Normal;
                            break;
                        }
                        case PXWavefrontLineVertexParameter:
                            break;

                        case PXWavefrontLineVertexTexture:
                        {
                            insertPosition = (PXByte*)counter.DataTexture + (counter.SizeOfVertexData * counter.Texture * 2u);
                            valuesExpected = 2;
                            ++counter.Texture;
                            break;
                        }
                    }

                    if(counter.CanUseF16)
                    {
                        const PXBool listParsed = PXCompilerParseF16V(&pxCompiler, (PXF16*)insertPosition, valuesExpected, &valuesDetected);
                   
                    
                    }
                    else
                    {
                        PXF32* data = (PXF32*)insertPosition;

                        const PXBool listParsed = PXCompilerParseF32V(&pxCompiler, data, valuesExpected, &valuesDetected);
                                      
#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "EEE",
                            "Color-Set",
                            "%6.2f %6.2f %6.2f",
                            data[0],
                            data[1],
                            data[2]
                        );
#endif 
                    
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
                    PXI32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data

#if 1
                    pxIndexBufferIndexSET
                    (
                        pxMesh->Geometry->IndexBuffer,
                        counter.Face,
                        3,
                        vertexData
                    );
#endif

                    pxMesh->Geometry->IndexBuffer->Data.CursorOffsetByte +=
                        PXTypeSizeGet(pxMesh->Geometry->IndexBuffer->DataType) * 3;

                    ++counter.Face;
                    ++cornerPoints;

                    PXCompilerSymbolEntryPeek(&pxCompiler); // check what line we are in

                    lineCurrent = pxCompiler.ReadInfo.SymbolEntryCurrent.Line; // Update current line

                    isDone = PXFileIsAtEnd(tokenSteam) || (lineStart != lineCurrent);
                } 
                while(!isDone);

                break; // [OK]
            }
            default: // Error
            {
                //  ++errorCounter;
                do
                {
                    PXCompilerSymbolEntryExtract(&pxCompiler);
                } 
                while(pxCompiler.ReadInfo.SymbolEntryCurrent.ID != PXCompilerSymbolLexerNewLine);

                break;
            }
        }
    }

    PXFileClose(tokenSteam);

#if PXLogEnable && 0
    for(PXSize i = 0; i < pxMesh->IndexBuffer->SegmentListAmount; ++i)
    {
        PXIndexSegmentEntry PXREF pxIndexSegment = &pxMesh->IndexBuffer->SegmentList[i];
        PXECSProperty pxECSProperty;
        PXClear(PXECSProperty, &pxECSProperty);

        if(pxIndexSegment->Material)
        {
            //PXResourcePropertyIO(&pxIndexSegment->Material->Info, &pxECSProperty, PXResourcePropertyName, PXResourcePropertyFetch);

            PXLogPrint
            (
                PXLoggingInfo,
                PXWaveFrontText,
                "Segment",
                "[%2i/%2i] %6i - PXID:%i, Name:%s",
                i + 1,
                pxMesh->IndexBuffer->SegmentListAmount,
                pxIndexSegment->DataRange,
                pxIndexSegment->Material->Info.ID,
                "---"
            );
        }
        else
        {
            PXLogPrint
            (
                PXLoggingInfo,
                PXWaveFrontText,
                "Segment",
                "[%2i/%2i] %6i - **No material**",
                i + 1,
                pxMesh->IndexBuffer->SegmentListAmount,
                pxIndexSegment->DataRange
                );
        } 
    }
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWaveFrontText,
        PXWaveFrontTextParsing,
        "Done!"
    );
#endif


    pxMesh->Geometry->VertexBufferPrime[0]->VertexData.CursorOffsetByte = counter.Position * sizeof(float) * 3;
    pxMesh->Geometry->VertexBufferPrime[1]->VertexData.CursorOffsetByte = counter.Normal * sizeof(float) * 3;
    pxMesh->Geometry->VertexBufferPrime[2]->VertexData.CursorOffsetByte = counter.Texture * sizeof(float) * 2;



    // Mesh data right now has multible 
    // index attributes, this does not work with graphic APIs
    // We need to convert it.
    PXMeshGeometry* pxMeshGeometryNEW;


    PXMeshGeometry PXREF pxMeshGeometryOld = pxMesh->Geometry;
    PXVertexBuffer** pxVertexBufferOLD = PXMeshGeometryVertexBufferListGET(pxMeshGeometryOld);
    PXIndexBuffer* pxIndexBufferOLD = pxMeshGeometryOld->IndexBuffer;

    const PXSize vertexCounter = PXIndexBufferAmount(pxIndexBufferOLD);

    // CREATE
    {
        PXBufferLayoutEntry pxBufferLayoutEntry[4];
        PXClearList(PXBufferLayoutEntry, pxBufferLayoutEntry, 4);

        pxBufferLayoutEntry[0].Format = PXTypeF32;
        pxBufferLayoutEntry[0].AmountOfElements = 3;
        pxBufferLayoutEntry[0].Type = PXVertexBufferLayoutTypePosition;
        pxBufferLayoutEntry[0].Entrys = vertexCounter;

        pxBufferLayoutEntry[1].Format = PXTypeF32;
        pxBufferLayoutEntry[1].AmountOfElements = 3;
        pxBufferLayoutEntry[1].Type = PXVertexBufferLayoutTypeNormal;
        pxBufferLayoutEntry[1].Entrys = vertexCounter;

        pxBufferLayoutEntry[2].Format = PXTypeF32;
        pxBufferLayoutEntry[2].AmountOfElements = 2;
        pxBufferLayoutEntry[2].Type = PXVertexBufferLayoutTypeTexturePos;
        pxBufferLayoutEntry[2].Entrys = vertexCounter;

        // Index buffer
        pxBufferLayoutEntry[3].Format = PXTypeI16U;
        pxBufferLayoutEntry[3].AmountOfElements = 1;
        pxBufferLayoutEntry[3].Type = PXVertexBufferLayoutTypePosition;
        pxBufferLayoutEntry[3].Entrys = vertexCounter;


        PXMeshGeometryCreateInfo pxMeshGeometryCreateInfo;
        PXClear(PXMeshGeometryCreateInfo, &pxMeshGeometryCreateInfo);
        pxMeshGeometryCreateInfo.VertexBufferLayoutAmount = 3;
        pxMeshGeometryCreateInfo.VertexBufferLayoutList = pxBufferLayoutEntry;
        pxMeshGeometryCreateInfo.IndexBufferLayoutAmount = 1;
        pxMeshGeometryCreateInfo.IndexBufferLayoutList = &pxBufferLayoutEntry[3];
        pxMeshGeometryCreateInfo.PrimitveAmount = vertexCounter;
        pxMeshGeometryCreateInfo.SegmentAmount = pxIndexBufferOLD->Segment.SegmentListAmount;

        PXMeshGeometryCreate(&pxMeshGeometryNEW, &pxMeshGeometryCreateInfo);

        // update reference
        pxMesh->Geometry = pxMeshGeometryNEW;
    }

    PXVertexBuffer** pxVertexBufferNEW = PXMeshGeometryVertexBufferListGET(pxMeshGeometryNEW);
    PXIndexBuffer* pxIndexBufferNEW = pxMeshGeometryNEW->IndexBuffer;

    switch(pxIndexBufferOLD->DataType)
    {
        case PXTypeI16U:
        {
            PXSize amount = PXIndexBufferAmount(pxIndexBufferOLD) /3;

            for(size_t i = 0; i < amount; ++i)
            {
                PXI16U* dataIN = pxIndexBufferOLD->Data.Data;
                PXSize indexPosition = dataIN[i*3+0];
                PXSize indexNormal = dataIN[i * 3 + 1];
                PXSize indexTexture = dataIN[i * 3 + 2];

                pxVertexBufferNEW[0]->VertexData.F32V3[i] = pxVertexBufferOLD[0]->VertexData.F32V3[indexPosition];
                pxVertexBufferNEW[1]->VertexData.F32V3[i] = pxVertexBufferOLD[1]->VertexData.F32V3[indexPosition];
                pxVertexBufferNEW[2]->VertexData.F32V3[i] = pxVertexBufferOLD[2]->VertexData.F32V3[indexPosition];
                pxIndexBufferNEW->Data.I16U[i] = i;
            }

            pxIndexBufferNEW->Data.CursorOffsetByte = amount * 2;

            pxIndexBufferNEW->Segment = pxIndexBufferOLD->Segment;

            break;
        }

        default:
            break;
    }

    pxVertexBufferNEW[0]->VertexData.CursorOffsetByte = vertexCounter * sizeof(float) * 3;
    pxVertexBufferNEW[1]->VertexData.CursorOffsetByte = vertexCounter * sizeof(float) * 3;
    pxVertexBufferNEW[2]->VertexData.CursorOffsetByte = vertexCounter * sizeof(float) * 2;


    return PXResultOK;
}

PXResult PXAPI PXWavefrontSaveFromFile(PXMesh PXREF pxMesh, PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}