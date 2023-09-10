#include "PXWavefront.h"

#include <Compiler/PXCompiler.h>
#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

#define PXWavefrontDetectMaterial 0

void PXWavefrontElementConstruct(PXWavefrontElement* objElement)
{
    PXClear(PXWavefrontElement, objElement);
}

void PXWavefrontElementDestruct(PXWavefrontElement* objElement)
{
    //TODO: clear memeory
}

void PXWavefrontConstruct(PXWavefront* const obj)
{
    PXClear(PXWavefront, obj);
}

void PXWavefrontDestruct(PXWavefront* const obj)
{
    PXMemoryRelease(obj->ElementList, obj->ElementListSize);

    PXMemoryRelease(obj->MaterialFileList, obj->MaterialFileListSize);
}

PXWavefrontLineType PXWavefrontPeekLine(const void* line, const PXSize size)
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

PXActionResult PXWavefrontFileCompile(PXFile* const inputStream, PXFile* const outputStream)
{
    PXSize errorCounter = 0;
    PXFile tokenSteam;

    PXFileOpenTemporal(&tokenSteam, inputStream->DataSize * 2);

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

    PXMemoryClear(drawSize, sizeof(drawSize));
    PXMemoryClear(drawOrder, sizeof(drawOrder));

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

        PXCompilerLexicalAnalysis(inputStream, &tokenSteam, &compilerSettings); // Raw-File-Input -> Lexer tokens

        //PXFileBufferExternal(&tokenSteam, (PXAdress)outputStream->Data + headerOffset, outputStream->DataCursor - headerOffset);
    }

    // Write 0'ed data to later jump back to to change.
    PXFileCursorAdvance(outputStream, headerOffset);

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        PXFileWriteI8U(outputStream, objPeekLine);

        switch (objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerInteger);
                PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);

                break;
            }
            case PXWavefrontLineMaterialLibraryInclude:
            case PXWavefrontLineMaterialLibraryUse:
            case PXWavefrontLineObjectName:
            case PXWavefrontLineObjectGroup:
            {
                PXText elementName;
                PXTextConstructBufferA(&elementName, 256);

                const PXBool isString = PXCompilerParseStringUntilNewLine(&tokenSteam, &elementName);

                if (!isString)
                {
                    ++errorCounter;
                    PXWavefrontCompileError(&compilerSymbolEntry, PXCompilerSymbolLexerGenericElement);
                    break;
                }

                PXFileWriteI8U(outputStream, PXCompilerSymbolLexerString);
                PXFileWriteI16U(outputStream, elementName.SizeUsed);
                PXFileWriteA(outputStream, elementName.TextA, elementName.SizeUsed);

                switch (objPeekLine)
                {
                    case PXWavefrontLineMaterialLibraryInclude:
                    {
                        ++mtlInlclueesListSize;

#if PXWavefrontDetectMaterial

                        headerCacheOffset += PXFileWriteAtI16U(outputStream, namedElementSize, headerCacheOffset);
                        headerCacheOffset += PXFileWriteAtB(outputStream, namedElement, namedElementSize, headerCacheOffset);
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

                    PXFileWriteI8U(outputStream, valuesDetected);
                    PXFileWriteFV(outputStream, vector, valuesDetected);
                }

                break; // [OK]
            }
            case PXWavefrontLineFaceElement:
            {
                PXSize cornerPoints = 0;
                PXSize cursorPos = outputStream->DataCursor;

                PXFileWriteI8U(outputStream, 0xFF);

                while (!PXFileIsAtEnd(&tokenSteam))
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
                    vertexData[0] = compilerSymbolEntry.DataI32U;

                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek for next token

                    switch (compilerSymbolEntry.ID)
                    {
                        case PXCompilerSymbolLexerInteger: // is syntax A
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful skip to 2nd integer

                            vertexData[1] = compilerSymbolEntry.DataI32U; // Save 2nd value

                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Get 3rd integer

                            const PXBool isExpectedThridInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                            if (!isExpectedThridInteger)
                            {
                                // Error;
                            }

                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                            vertexData[2] = compilerSymbolEntry.DataI32U;

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

                                    vertexData[2] = compilerSymbolEntry.DataI32U; // Save value

                                    break;
                                }
                                case PXCompilerSymbolLexerInteger: // Is syntax B
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful, remove the secound value

                                    vertexData[1] = compilerSymbolEntry.DataI32U; // Save value

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

                                        vertexData[2] = compilerSymbolEntry.DataI32U;

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

                    PXFileWriteI32UV(outputStream, vertexData, 3u);

                  //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                    //----------------------------------

                    ++cornerPoints;
                }

                drawSize[drawCurrentIndex] = PXMathMaximum(drawSize[drawCurrentIndex], cornerPoints);

                indexListSize += cornerPoints;
                drawCurrentCounter += cornerPoints;

                PXFileWriteAtI8U(outputStream, cornerPoints, cursorPos);

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
        PXFile materialNameFetchStream;
        PXFile materialFileStream;

        PXFileBufferExternal(&materialNameFetchStream, outputStream->Data, outputStream->DataCursor);

        PXText currentFilePath;
        PXTextConstructNamedBufferA(&currentFilePath, currentFilePathBuffer, PathMaxSize);

        PXText currentMTLFilePath;
        PXTextConstructNamedBufferA(&currentMTLFilePath, currentMTLFilePathBuffer, PathMaxSize);

        const PXBool success = PXFilePathGet(inputStream, &currentFilePath); // Work PXWavefront file path

        for (PXSize i = 0; i < mtlInlclueesListSize; ++i)
        {
            unsigned short length = 0;

            PXFileWriteI8U(outputStream, PXWavefrontEmbeddedMTL);

            PXFileReadI16U(&materialNameFetchStream, &length);
            currentMTLFilePath.SizeUsed = length;
            PXFileReadB(&materialNameFetchStream, currentMTLFilePath.TextA, currentMTLFilePath.SizeUsed);

            PXFilePathSwapFileName(&currentMTLFilePath, &currentMTLFilePath, &currentFilePath);


            PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
            pxFileOpenFromPathInfo.Text = currentMTLFilePath;
            pxFileOpenFromPathInfo.FileSize = 0;
            pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
            pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
            pxFileOpenFromPathInfo.AllowMapping = PXTrue;
            pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
            pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

            {
                const PXActionResult materialFileLoadResult = PXFileOpenFromPath(&materialFileStream, &pxFileOpenFromPathInfo);
                const PXBool sucessful = PXActionSuccessful == materialFileLoadResult;

                if (!sucessful)
                {
                    continue;
                }

                const PXActionResult materialFileCompileResult = PXMTLFileCompile(&materialFileStream, outputStream);

                PXFileDestruct(&materialFileStream);
            }
        }
    }
#endif
    // MTL loading finished



    {
        PXFile headerInfoStream;

        PXFileBufferExternal(&headerInfoStream, outputStream->Data, outputStream->DataCursor);

        PXFileWriteI8U(&headerInfoStream, drawCurrentIndex);

        for (PXSize i = 0; i < drawCurrentIndex; ++i)
        {
            PXFileWriteI8U(&headerInfoStream, drawSize[i+1]);
            PXFileWriteI32U(&headerInfoStream, drawOrder[i+1]);
        }

        PXFileWriteI32U(&headerInfoStream, vertexListSize);
        PXFileWriteI32U(&headerInfoStream, normalListSize);
        PXFileWriteI32U(&headerInfoStream, textureListSize);
        PXFileWriteI32U(&headerInfoStream, parameterListSize);
        PXFileWriteI32U(&headerInfoStream, indexListSize);
        PXFileWriteI32U(&headerInfoStream, mtlInlclueesListSize);
        PXFileWriteI64U(&headerInfoStream, mtlEmbeddedDataOffset);
    }

    outputStream->DataSize = outputStream->DataCursor;

    PXFileDestruct(&tokenSteam);

    if (errorCounter)
    {
        return PXActionCompilingError;
    }

    return PXActionSuccessful;
}

PXActionResult PXWavefrontLoadFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile)
{
    PXFile tokenSteam;
    PXSize errorCounter = 0;

    PXSize drawoffsetCounter = 0;
    PXSize drawCurrentIndex = 0;


    PXInt32U counterVertex = 0;
    PXInt32U counterVertexMaxID = 0;
    PXInt32U counterNormal = 0;
    PXInt32U counterTexture = 0;
    PXInt32U counterIndex = 0;

    PXFileOpenTemporal(&tokenSteam, pxFile->DataSize * 6);

    // Lexer - Level I
    {
        PXCompilerSettings compilerSettings;

        PXCompilerSettingsConstruct(&compilerSettings);

        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;
        compilerSettings.CommentSingleLineSize = 1u;
        compilerSettings.CommentSingleLine = "#";

        PXCompilerLexicalAnalysis(pxFile, &tokenSteam, &compilerSettings); // Raw-File-Input -> Lexer tokens
    }

    // Stage - 1 - Analyse file

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
                        //  ++(pxModel->MaterialListSize);
                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
                        // Do nothing
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
                //PXSize cornerPoints = 0;
              //  PXSize cursorPos = outputStream->DataCursor;

              //  PXFileWriteI8U(outputStream, 0xFF);

                while (!PXFileIsAtEnd(&tokenSteam))
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    // allowed syntax is
                    // A: "f 1 2 3"
                    // B: "f 1/2/3"
                    // C: "f 1//3"
                    // D: "f 1/3"


                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    const PXBool isExpectedInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID; // is int`?

                    if (!isExpectedInteger) // If not int => Error
                    {
                        break;
                    }

                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    // Save 1st value
                    vertexData[0] = compilerSymbolEntry.DataI32U;

                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek for next token

                    switch (compilerSymbolEntry.ID)
                    {
                        case PXCompilerSymbolLexerInteger: // is syntax A
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful skip to 2nd integer

                            vertexData[1] = compilerSymbolEntry.DataI32U; // Save 2nd value

                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Get 3rd integer

                            const PXBool isExpectedThridInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                            if (!isExpectedThridInteger)
                            {
                                // Error;
                            }

                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                            vertexData[2] = compilerSymbolEntry.DataI32U;

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

                                    vertexData[2] = compilerSymbolEntry.DataI32U; // Save value

                                    break;
                                }
                                case PXCompilerSymbolLexerInteger: // Is syntax B
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful, remove the secound value

                                    vertexData[1] = compilerSymbolEntry.DataI32U; // Save value

                                    // Exptect 2nd '/'
                                    {
                                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek, expect '/'
                                        const PXBool isSlash = PXCompilerSymbolLexerSlash == compilerSymbolEntry.ID;

                                        if (isSlash)
                                        {
                                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // remove '/'

                                            // Try get 3nd integer
                                            {
                                                PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Next token

                                                const PXBool isSecoundToken = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                                                if (!isSecoundToken)
                                                {
                                                    // Error
                                                }

                                                vertexData[2] = compilerSymbolEntry.DataI32U;

                                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
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


                    // printf("|%6i| %3s | %12i | %12i | %12i |\n",  compilerSymbolEntry.Line, "f", vertexData[0], vertexData[1], vertexData[2]);

                    for (PXSize i = 0; i < 3u; ++i)
                    {
                        vertexData[i] -= 1u;
                    }

                    counterVertexMaxID = PXMathMaximumIU(counterVertexMaxID, vertexData[0]);

                    //PXFileWriteI32UV(outputStream, vertexData, 3u);

                    //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                      //----------------------------------

                    ++counterIndex;
                }

                //pxModel->DataIndexWidth = PXMathMaximum(pxModel->DataIndexWidth, cornerPoints);


               // counterIndex += cornerPoints;

              //  PXFileWriteAtI8U(outputStream, cornerPoints, cursorPos);

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

    //
    PXBool requireToCalculateNormals = PXFalse;

    // Stage - 2 - Allocate space
    {

        PXSize memoryDataNormalSize = 3 * counterNormal * sizeof(float);
        PXSize memoryDataTextureSize = 2 * counterTexture * sizeof(float);
        // PXSize memoryDataColorSize = pxModel->DataColorWidth * pxModel->DataColorSize * sizeof(float);


         /*
         pxModel->DataSize =
             memoryDataVertexSize +
             memoryDataNormalSize +
             memoryDataTextureSize +
             memoryDataColorSize +
             memoryDataIndexSize;*/

             // pxModel->Data = PXMemoryAllocate(pxModel->DataSize);

        if (counterVertex && memoryDataNormalSize && memoryDataTextureSize)
        {
            pxVertexStructure->VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;
        }
        else if (counterVertex && !memoryDataNormalSize && memoryDataTextureSize)
        {
            // We would set this in that format, but we want normals.
            // pxVertexStructure->VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;

            pxVertexStructure->VertexBuffer.Format = PXVertexBufferFormatT2F_N3F_XYZ;

            requireToCalculateNormals = PXTrue;
        }

        pxVertexStructure->VertexBuffer.VertexDataRowSize = PXVertexBufferFormatStrideSize(pxVertexStructure->VertexBuffer.Format);
        pxVertexStructure->VertexBuffer.VertexDataSize = pxVertexStructure->VertexBuffer.VertexDataRowSize * counterVertex * sizeof(float);
        pxVertexStructure->VertexBuffer.VertexData = PXMemoryHeapAllocateCleared(sizeof(float), pxVertexStructure->VertexBuffer.VertexDataSize / sizeof(float));


        // Setup index array

        pxVertexStructure->IndexBuffer.DrawModeID = PXDrawModeIDTriangle; // PXDrawModeIDPoint | PXDrawModeIDLineLoop;
        pxVertexStructure->IndexBuffer.DataType =
            PXDataTypeInt08U * (counterVertexMaxID <= 0xFF) +
            PXDataTypeInt16ULE * ((counterVertexMaxID > 0xFF) && (counterVertexMaxID <= 0xFFFF)) +
            PXDataTypeInt32ULE * (counterVertexMaxID > 0xFFFF);

        pxVertexStructure->IndexBuffer.IndexTypeSize = pxVertexStructure->IndexBuffer.DataType & PXDataTypeSizeMask;
        pxVertexStructure->IndexBuffer.IndexDataAmount = counterIndex;
        pxVertexStructure->IndexBuffer.IndexData = PXMemoryHeapAllocateCleared(pxVertexStructure->IndexBuffer.IndexTypeSize, counterIndex);
        pxVertexStructure->IndexBuffer.IndexDataSize = pxVertexStructure->IndexBuffer.IndexTypeSize * counterIndex;

        // pxModel->MaterialList = PXMemoryAllocateTypeCleared(PXMaterial, pxModel->MaterialListSize);


         //pxModel->DataVertexList = pxModel->Data;
         //pxModel->DataNormalList = (char*)pxModel->DataVertexList + memoryDataVertexSize;
         //pxModel->DataTextureList = (char*)pxModel->DataNormalList + memoryDataNormalSize;
         //pxModel->DataColorList = (char*)pxModel->DataTextureList + memoryDataTextureSize;
         //pxModel->DataIndexList = (char*)pxModel->DataColorList + memoryDataColorSize;


         // Reset all size values
        counterVertex = 0;
        counterNormal = 0;
        counterTexture = 0;
        counterIndex = 0;

        PXFileCursorToBeginning(&tokenSteam);
    }


    // Stage - 3 - Extract data

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        const PXWavefrontLineType objPeekLine = PXWavefrontPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

        switch (objPeekLine)
        {
            case PXWavefrontLineSmoothShading:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect a name.

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
                PXTextConstructNamedBufferA(&materialFileName, materialFileNameBuffer, PathMaxSize);

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
                        /*
                        PXMaterial* const pxMaterial = &pxModel->MaterialList[pxModel->MaterialListSize++];

                        PXFile materialFile;

                        // Open and load
                        {

                            //-------------------------------
                            PXText materialFilePathFull;
                            PXTextConstructNamedBufferA(&materialFilePathFull, materialFilePathFullBuffer, PathMaxSize);

                            {
                                //---<Get current path>----------------
                                PXText currentFilePath;
                                PXTextConstructNamedBufferA(&currentFilePath, currentFilePathBuffer, PathMaxSize);

                                const PXBool success = PXFilePathGet(pxFile, &currentFilePath); // Work PXWavefront file path
                                //-------------------------------

                                PXFilePathSwapFileName(&currentMTLFilePath, &currentMTLFilePath, &currentFilePath);
                            }
                            //-------------------------------


                            PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
                            pxFileOpenFromPathInfo.Text = currentMTLFilePath;
                            pxFileOpenFromPathInfo.FileSize = 0;
                            pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
                            pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                            pxFileOpenFromPathInfo.AllowMapping = PXTrue;
                            pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
                            pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

                            {
                                const PXActionResult materialFileLoadResult = PXFileOpenFromPath(&materialFilePathFull, &pxFileOpenFromPathInfo);
                                const PXBool sucessful = PXActionSuccessful == materialFileLoadResult;

                                if (!sucessful)
                                {

                                }
                            }

                        }

                        // compile
                        const PXActionResult materialFileCompileResult = PXMTLFileCompile(&materialFile, outputStream);

                        // Close
                        PXFileDestruct(&materialFile);
                        */

                        break;
                    }
                    case PXWavefrontLineMaterialLibraryUse:
                    {
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
                    switch (objPeekLine)
                    {
                        case PXWavefrontLineVertexGeometric:
                        {
                            float* const position = (float*)PXVertexBufferInsertionPoint(&pxVertexStructure->VertexBuffer, PXVertexBufferDataTypeNormal, counterVertex);

                            PXMemoryCopy(vector, sizeof(float) * valuesDetected, position, sizeof(float) * valuesDetected);
                            ++counterVertex;
                            break;
                        }
                        case PXWavefrontLineVertexNormal:
                            //PXMemoryCopy(vector, sizeof(float) * valuesDetected, &pxModel->DataNormalList[pxModel->DataNormalSize], sizeof(float) * pxModel->DataNormalWidth * valuesDetected);
                            counterNormal += valuesDetected;
                            break;

                        case PXWavefrontLineVertexParameter:

                            break;

                        case PXWavefrontLineVertexTexture:
                            //  float* position = &((float*)pxVertexStructure->VertexBuffer.VertexData)[counterVertex * 8 + 5];

                            //  PXMemoryCopy(vector, sizeof(float) * valuesDetected, position, sizeof(float) * pxModel->DataTextureWidth * valuesDetected);
                            counterTexture += valuesDetected;
                            break;
                    }
                }

                break; // [OK]
            }
            case PXWavefrontLineFaceElement:
            {
                PXSize cornerPoints = 0;

                while (!PXFileIsAtEnd(&tokenSteam))
                {
                    PXInt32U vertexData[3] = { 0, 0, 0 };

                    // allowed syntax is
                    // A: "f 1 2 3"
                    // B: "f 1/2/3"
                    // C: "f 1//3"
                    // D: "f 1/3"


                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    const PXBool isExpectedInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID; // is int`?

                    if (!isExpectedInteger) // If not int => Error
                    {
                        break;
                    }

                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // read token, expect int

                    // Save 1st value
                    vertexData[0] = compilerSymbolEntry.DataI32U;

                    PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek for next token

                    switch (compilerSymbolEntry.ID)
                    {
                        case PXCompilerSymbolLexerInteger: // is syntax A
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful skip to 2nd integer

                            vertexData[1] = compilerSymbolEntry.DataI32U; // Save 2nd value

                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Get 3rd integer

                            const PXBool isExpectedThridInteger = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                            if (!isExpectedThridInteger)
                            {
                                // Error;
                            }

                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                            vertexData[2] = compilerSymbolEntry.DataI32U;

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

                                    vertexData[2] = compilerSymbolEntry.DataI32U; // Save value

                                    break;
                                }
                                case PXCompilerSymbolLexerInteger: // Is syntax B
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Peek sucessful, remove the secound value

                                    vertexData[1] = compilerSymbolEntry.DataI32U; // Save value

                                    // Exptect 2nd '/'
                                    {
                                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Peek, expect '/'
                                        const PXBool isSlash = PXCompilerSymbolLexerSlash == compilerSymbolEntry.ID;

                                        if (isSlash)
                                        {
                                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // remove '/'

                                            // Try get 3nd integer
                                            {
                                                PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Next token

                                                const PXBool isSecoundToken = PXCompilerSymbolLexerInteger == compilerSymbolEntry.ID;

                                                if (!isSecoundToken)
                                                {
                                                    // Error
                                                }

                                                vertexData[2] = compilerSymbolEntry.DataI32U;

                                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
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

                    for (PXSize i = 0; i < 3u; ++i)
                    {
                        vertexData[i] -= 1u;
                    }


                    void* const input = (PXAdress)pxVertexStructure->IndexBuffer.IndexData + counterIndex * pxVertexStructure->IndexBuffer.IndexTypeSize;


#if 1
                    const PXSize dataSize = pxVertexStructure->IndexBuffer.IndexTypeSize;
                    PXMemoryCopy(vertexData, dataSize, input, dataSize);


#else
                    switch (pxVertexStructure->IndexBuffer.DataType)
                    {

                        case PXDataTypeInt8U:
                            *(PXInt8U*)input = vertexData[0];
                            break;

                        case PXDataTypeLEInt16U:
                            *(PXInt16U*)input = vertexData[0];
                            break;

                        case PXDataTypeLEInt32U:
                            *(PXInt32U*)input = vertexData[0];
                            break;
                    }
#endif





                    counterIndex++;


                    //  PXFileWriteI32UV(outputStream, vertexData, 3u);

                      //  printf("Face _> %i, %i, %i\n", vertexData[0], vertexData[1], vertexData[2]);

                        //----------------------------------

                    ++cornerPoints;
                }

                //drawSize[drawCurrentIndex] = PXMathMaximum(drawSize[drawCurrentIndex], cornerPoints);

               // indexListSize += cornerPoints;
               // drawCurrentCounter += cornerPoints;

              //  PXFileWriteAtI8U(outputStream, cornerPoints, cursorPos);

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

    if (errorCounter)
    {
        return PXActionCompilingError;
    }

    if (!requireToCalculateNormals)
    {
        return PXActionSuccessful;
    }


    // Calculate normals


    for (PXSize i = 0; i < counterVertex; ++i)
    {
        float* const positionData = (float*)PXVertexBufferInsertionPoint(&pxVertexStructure->VertexBuffer, PXVertexBufferDataTypeNormal, i);
        float* const normalData = (float*)PXVertexBufferInsertionPoint(&pxVertexStructure->VertexBuffer, PXVertexBufferDataTypeVertex, i);

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



    return PXActionSuccessful;
}

PXActionResult PXWavefrontSaveFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}
