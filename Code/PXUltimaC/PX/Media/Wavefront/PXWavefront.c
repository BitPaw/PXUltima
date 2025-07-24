#include "PXWavefront.h"

#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>

#define PXWavefrontDetectMaterial 0

const char PXWaveFrontText[] = "WaveFront";
const char PXWaveFrontTextParsing[] = "Parsing";

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

PXActionResult PXAPI PXWavefrontLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXModel* const pxModel = (PXModel*)pxResourceLoadInfo->ResourceTarget;
    PXMesh* const mesh = &pxModel->Mesh;

    PXFile tokenSteam;

    struct PXWaveFrontCounter
    {
        PXInt32U MaterialInlcude;
        PXInt32U MaterialUse;

        // Vertex Data
        PXInt32U Position;
        PXInt32U Texture;
        PXInt32U Normal;
        PXInt32U Face;

        PXSize EntrysPosition;
        PXSize EntrysTexture;
        PXSize EntrysNormal;
        PXSize EntrysTotal;

        PXInt32U FaceLast;
        PXInt32U FaceTotal;

        // Index data
        PXInt32U VertexMaxID;

        PXInt32U ExpetcedInterleavedSizeInByte;
        PXInt32U ExpetcedNonInterleavedSizeInByte;

        void* DataVertex;
        void* DataNormal;
        void* DataTexture;

        void* IndexVertex;
        void* IndexNormal;
        void* IndexTexture;

        PXBool CanUseF16;
        PXInt8U SizeOfVertexData;
    }
    counter;

    PXMemoryClear(&counter, sizeof(counter));

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = &tokenSteam;
    pxCompiler.Flags = PXCompilerKeepAnalyseTypes;
    pxCompiler.CommentSingleLineSize = 1u;
    pxCompiler.CommentSingleLine = "#";

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

    while(!PXFileIsAtEnd(&tokenSteam))
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
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data

                    counter.VertexMaxID = PXMathMaximumIU(counter.VertexMaxID, vertexData[0]);
                    ++counter.Face;

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

   // counter.CanUseF16 = PXTrue;


    const PXInt32U type = PXTypeIntFitting(counter.VertexMaxID);

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
        "Material-Include", counter.MaterialInlcude,
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
        PXBufferLayout pxVertexBufferLayout[3];
        PXClearList(PXBufferLayout, pxVertexBufferLayout, 3);
        PXSize pxVertexBufferLayoutAmount = 0;
     
        // TODO: we can check if we can use 16-bit floats here

        // Add positions
        {
            //pxVertexBufferLayout[0].AmountOfValues = counter.SizeOfVertexData * counter.EntrysPosition;
            pxVertexBufferLayout[0].Format = PXTypeF32;
            pxVertexBufferLayout[0].AmountOfElements = 3;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypePosition;

            ++pxVertexBufferLayoutAmount;
        }

        // Add normals
        if(counter.EntrysNormal > 0)
        {
          //  pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfValues = counter.SizeOfVertexData * counter.EntrysNormal;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfElements = 3;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeNormal;

            ++pxVertexBufferLayoutAmount;
        }


        // Add texture units
        if(counter.EntrysTexture > 0)
        {
           // pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfValues = counter.SizeOfVertexData * counter.EntrysTexture;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].Format = PXTypeF32;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].AmountOfElements = 2;
            pxVertexBufferLayout[pxVertexBufferLayoutAmount].Type = PXVertexBufferLayoutTypeTexturePos;

            ++pxVertexBufferLayoutAmount;
        }

        PXMeshVertexLayout(mesh, 0, pxVertexBufferLayout, pxVertexBufferLayoutAmount);
        PXMeshIndexLayout(mesh, counter.Face, counter.MaterialUse);
     

        // Reset all size values 
        counter.DataVertex = PXMeshVertexInsert(mesh, PXVertexBufferLayoutTypePosition);
        counter.DataNormal = PXMeshVertexInsert(mesh, PXVertexBufferLayoutTypeNormal);
        counter.DataTexture = PXMeshVertexInsert(mesh, PXVertexBufferLayoutTypeTexturePos);

        counter.IndexVertex = PXMeshIndexInsert(mesh, PXVertexBufferLayoutTypePosition);
        counter.IndexNormal = PXMeshIndexInsert(mesh, PXVertexBufferLayoutTypeNormal);
        counter.IndexTexture = PXMeshIndexInsert(mesh, PXVertexBufferLayoutTypeTexturePos);

        counter.Position = 0;
        counter.Normal = 0;
        counter.Texture = 0;
        counter.Face = 0;
        counter.MaterialUse = 0;
        counter.MaterialInlcude = 0;

        PXFileCursorToBeginning(&tokenSteam);
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

    PXInt8U typeSize = PXTypeSizeGet(mesh->IndexBuffer.DataType);

    for(;;)
    {
        const PXBool isAtEnd = PXFileIsAtEnd(&tokenSteam);

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
                PXCompilerSymbolEntryExtract(&pxCompiler); // Expect a name .

                //PXFileWriteI8U(outputStream, PXCompilerSymbolLexerNumeric);
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
                        PXMaterialContainer* const pxMaterialContaier = &mesh->MaterialContaierList[counter.MaterialInlcude++];
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
                        PXIndexSegment* const pxIndexSegmentCurrent = &pxModel->Mesh.IndexBuffer.SegmentList[counter.MaterialUse];
                       
                        pxIndexSegmentCurrent->Material = PXMaterialContainerFind
                        (
                            mesh->MaterialContaierList,
                            &materialFileName
                        );

                        if(counter.MaterialUse > 0)
                        {
                            PXIndexSegment* const pxIndexSegmentPrevious = &pxModel->Mesh.IndexBuffer.SegmentList[counter.MaterialUse - 1];
                            
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
                        const PXBool listParsed = PXCompilerParseF32V(&pxCompiler, (PXF32*)insertPosition, valuesExpected, &valuesDetected);
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

          

                void* fp = counter.IndexVertex;
                void* ft = counter.IndexTexture;
                void* fn = counter.IndexNormal;

                do
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    PXWavefrontFaceLineParse(&pxCompiler, vertexData); // Get the data

                    switch(typeSize)
                    {
                        case PXTypeSize08:
                        {
                            ((PXInt8U*)fp)[counter.Face] = vertexData[0];
                            ((PXInt8U*)ft)[counter.Face] = vertexData[1];
                            ((PXInt8U*)fn)[counter.Face] = vertexData[2];

                            break;
                        }
                        case PXTypeSize16:
                        {
                            ((PXInt16U*)fp)[counter.Face] = vertexData[0];
                            ((PXInt16U*)ft)[counter.Face] = vertexData[1];
                            ((PXInt16U*)fn)[counter.Face] = vertexData[2];
                            break;
                        }
                        case PXTypeSize32:
                        {
                            ((PXInt32U*)fp)[counter.Face] = vertexData[0];
                            ((PXInt32U*)ft)[counter.Face] = vertexData[1];
                            ((PXInt32U*)fn)[counter.Face] = vertexData[2];
                            break;
                        }
                        case PXTypeSize64:
                        {
                            ((PXInt64U*)fp)[counter.Face] = vertexData[0];
                            ((PXInt64U*)ft)[counter.Face] = vertexData[1];
                            ((PXInt64U*)fn)[counter.Face] = vertexData[2];
                            break;
                        }
                    }

                    ++counter.Face;
                    ++cornerPoints;

                    PXCompilerSymbolEntryPeek(&pxCompiler); // check what line we are in

                    lineCurrent = pxCompiler.ReadInfo.SymbolEntryCurrent.Line; // Update current line

                    isDone = PXFileIsAtEnd(&tokenSteam) || (lineStart != lineCurrent);
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

    PXFileClose(&tokenSteam);

#if PXLogEnable
    for(PXSize i = 0; i < mesh->IndexBuffer.SegmentListAmount; ++i)
    {
        PXIndexSegment* const pxIndexSegment = &mesh->IndexBuffer.SegmentList[i];

        char materialText[256];

        if(pxIndexSegment->Material)
        {
            char* name = 0;

            PXResourceFetchName(&pxIndexSegment->Material->Info, &name, PXNull);

            PXTextPrintA(materialText, 256, "PXID:%i, Name:%s", pxIndexSegment->Material->Info.ID, name);
        }
        else
        {
            PXTextPrintA(materialText, 256, "**No material**");
        }

        PXLogPrint
        (
            PXLoggingInfo,
            PXWaveFrontText,
            "Segment",
            "[%2i/%2i] %6i - %s",
            i+1,
            mesh->IndexBuffer.SegmentListAmount,
            pxIndexSegment->DataRange,
            materialText
        );
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWavefrontSaveFromFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}