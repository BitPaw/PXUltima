#include "PXYAML.h"

#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

PXResult PXAPI PXYAMLLoadFromFile(PXDOM PXREF pxDOM, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXSize errorCounter = 0;


    PXCompiler pxCompiler;

    PXClear(PXCompiler, &pxCompiler);
    //pxCompiler.CodeDocument = (PXCodeDocument*)pxResourceLoadInfo->ResourceTarget;
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileCurrent;
    pxCompiler.Flags = 
        PXCompilerKeepAnalyseTypes | 
        PXCompilerDOMGenerate |
        PXCompilerKeepNewLine |
        PXCompilerKeepWhiteSpace |
        PXCompilerKeepComments;
    //pxCompiler.DOM = pxDOM;
    PXTextFromAdressA(&pxCompiler.CommentSingleLine, "#", 1, 1);

    // Lexer - Level I
    PXCompilerLexicalAnalysis(&pxCompiler); // Raw-File-Input -> Lexer tokens



    while(PXCompilerSymbolEntryExtract(&pxCompiler))
    {
        PXCompilerSymbolEntry PXREF symbolCurrent = PXCompilerSymbolEntryCurrent(&pxCompiler);

        switch(symbolCurrent->ID)
        {
            case PXCompilerSymbolLexerGeneric:
            {
                // Store generic symbol
                PXCompilerSymbolEntry pxSymbolSliceKey = *symbolCurrent;  

                PXBool isColonNext = PXCompilerSymbolEntryPeekCheck(&pxCompiler, PXCompilerSymbolLexerColon);

                if(!isColonNext)
                {
                    break;
                }         

                // remove ":"
                PXCompilerSymbolEntryForward(&pxCompiler);

                // Possible whitespace
                PXBool iswhiteSpace = PXCompilerSymbolEntryPeekCheck(&pxCompiler, PXCompilerSymbolLexerWhiteSpace);

                if(iswhiteSpace)
                {
                    PXCompilerSymbolEntryForward(&pxCompiler);
                }

                PXBool isNewLine = PXCompilerSymbolEntryPeekCheck(&pxCompiler, PXCompilerSymbolLexerNewLine);

                if(isNewLine)
                {
                    // nested key: "pad:"

                    // Store class
                    PXDOMObjectAdd(pxDOM, &pxCompiler.DOMPrefixStack, &pxSymbolSliceKey);

                    // remove new line
                    PXCompilerSymbolEntryForward(&pxCompiler);
                }
                else
                {
                    // key-value: "bottom: 750"
                    PXCompilerSymbolEntry pxSymbolSliceValue = *symbolCurrent; 

                    PXDOMPropertyAdd
                    (
                        pxDOM,
                        &pxCompiler.DOMPrefixStack,
                        &pxSymbolSliceKey,
                        &pxSymbolSliceValue
                    );

                    // Remove value
                    PXCompilerSymbolEntryForward(&pxCompiler);
                }

                break;
            }
            case PXCompilerSymbolLexerNewLine:
            {
               

                break;
            }
            case PXCompilerSymbolLexerWhiteSpace:
            {
                PXI16U newIndent = symbolCurrent->Size;
                PXI16U oldIndent = PXDOMPrefixIndentCurrent(&pxCompiler.DOMPrefixStack);

                PXBool isSame = newIndent == oldIndent;
                PXBool isDeeper = newIndent > oldIndent;

                if(isSame)
                {
                    break;
                }

                if(isDeeper) 
                {
                    pxCompiler.DOMPrefixStack.indentDepth++;
                    pxCompiler.DOMPrefixStack.indentStack[pxCompiler.DOMPrefixStack.indentDepth] = newIndent;
                }
                else if(newIndent < oldIndent) 
                {
                    while(pxCompiler.DOMPrefixStack.indentDepth > 0 && pxCompiler.DOMPrefixStack.indentStack[pxCompiler.DOMPrefixStack.indentDepth] > newIndent)
                    {
                        pxCompiler.DOMPrefixStack.indentDepth--;
                        pxCompiler.DOMPrefixStack.prefixDepth--;
                        //pxCompiler.DOMPrefixStack.objectDepth--;
                    }

                    if(pxCompiler.DOMPrefixStack.indentStack[pxCompiler.DOMPrefixStack.indentDepth] != newIndent) 
                    {
                        // This means indentation is invalid (YAML requires matching sibling indent)
                        // You can error or normalize depending on your design
                        DebugBreak();
                    }
                }              

                break;
            }
            default:
                break;
        }
    }


    PXFile* tokenSteam = pxCompiler.ReadInfo.FileCache;
#if 0

    unsigned int indentCounter = 0;

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        switch (compilerSymbolEntry.ID)
        {
        case PXCompilerSymbolLexerWhiteSpace:
            indentCounter = compilerSymbolEntry.Size;
            break;

        case PXCompilerSymbolLexerNewLine:
            indentCounter = 0;
            break;

        default:
        {
            break;
        }
        case PXCompilerSymbolLexerSingleCharacter:
        case PXCompilerSymbolLexerGenericElement:
        {
            const PXYAMLLineType lineType = PXYAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

            switch (lineType)
            {
            case PXYAMLLineTypeNotationStyleExtendedLine:
            {
                // interpret a new line as a whitespace.
                // Line ends after new symbol declaration
            }

            case PXYAMLLineTypeUnkown:
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                const PXYAMLLineType lineType = PXYAMLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
                const PXBool isColonSymbol = PXYAMLLineTypeKeyValueSeperator == lineType;

                if (!isColonSymbol) // Format: xxxx :
                {
                    // Error
                    continue;
                }

                // Fall though
            }
            case PXYAMLLineTypeKeyDeclare: // xxxx:
            {
                char* declname = compilerSymbolEntry.Source;
                unsigned short declSize = compilerSymbolEntry.Size - 1;

                PXFileWriteI8U(outputStream, PXYAMLLineTypeKeyValueDeclare);
                PXFileWriteI8U(outputStream, indentCounter);
                PXFileWriteI16U(outputStream, declSize);
                PXFileWriteB(outputStream, declname, declSize);


                // Fetch next value
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // consume "xxxxx: "
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // sonsume empty space or number


                switch (compilerSymbolEntry.ID)
                {

                case PXCompilerSymbolLexerNewLine:
                case PXCompilerSymbolLexerWhiteSpace:
                    PXFileWriteI16U(outputStream, 0);
                    indentCounter = compilerSymbolEntry.Size;
                    break;

                case PXCompilerSymbolLexerBool:
                {
                    PXFileWriteI16U(outputStream, sizeof(unsigned char));
                    PXFileWriteI8U(outputStream, PXCompilerSymbolLexerBool);
                    PXFileWriteI8U(outputStream, compilerSymbolEntry.DataC);
                    break;
                }

                case PXCompilerSymbolLexerNumeric:
                {
                    PXFileWriteI16U(outputStream, sizeof(unsigned int));
                    PXFileWriteI8U(outputStream, PXCompilerSymbolLexerNumeric);
                    PXFileWriteI32U(outputStream, compilerSymbolEntry.DataI32U);
                    break;
                }

                case PXCompilerSymbolLexerReal:
                {
                    PXFileWriteI16U(outputStream, sizeof(PXF32));
                    PXFileWriteI8U(outputStream, PXCompilerSymbolLexerReal);
                    PXFileWriteF(outputStream, compilerSymbolEntry.DataF);
                    break;
                }
                case PXCompilerSymbolLexerGenericElement:
                case PXCompilerSymbolLexerString:
                {
                    PXFileWriteI16U(outputStream, compilerSymbolEntry.Size);
                    PXFileWriteI8U(outputStream, PXCompilerSymbolLexerString);
                    PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
                    break;
                }
                }

                break;
            }

            }

            break;
        }
        }
    }


    // Comment
    // # Text

    // List
    // - Text


    // [milk, pumpkin pie, eggs, juice]

    PXSize oldpos = outputStream->DataCursor;

    outputStream->DataCursor = 0;

    while (!PXFileIsAtEnd(outputStream))
    {
        PXI8U depth = 0;
        PXI8U lineTypeID = 0;

        PXYAMLLineType lineType = PXYAMLLineTypeInvalid;

        PXFileReadI8U(outputStream, &lineTypeID);
        PXFileReadI8U(outputStream, &depth);

        lineType = (PXYAMLLineType)lineTypeID;


        switch (lineType)
        {
        case PXYAMLLineTypeKeyValueDeclare:
        {
            unsigned short ASize = 0;
            char A[256];
            unsigned short textBSize = 0;
            char textB[256];

            PXText pxTextBuffer;
            PXTextConstructFromAdressA(&pxTextBuffer, textB, 0, 256);

            char emotySpace[25];

            PXMemoryClear(A, 256u);
            PXMemoryClear(textB, 256u);
            PXMemoryClear(emotySpace, 25u);

            PXFileReadI16U(outputStream, &ASize);
            PXFileReadB(outputStream, A, ASize);
            PXFileReadI16U(outputStream, &textBSize);

            for (PXSize i = 0; i < depth; i++)
            {
                emotySpace[i] = ' ';
            }

            if (textBSize > 0)
            {
                PXCompilerSymbolLexer lexer;

                {
                    PXI8U lx = 0;

                    PXFileReadI8U(outputStream, &lx);

                    lexer = (PXCompilerSymbolLexer)lx;
                }

                switch (lexer)
                {
                case PXCompilerSymbolLexerBool:
                {
                    PXBool x = 0;

                    PXFileReadI8U(outputStream, &x);

                    PXTextFromBool(&pxTextBuffer, x);

                    break;
                }

                case PXCompilerSymbolLexerNumeric:
                {
                    PXI32U x = 0;

                    PXFileReadI32U(outputStream, &x);

                    PXTextFromInt(&pxTextBuffer, x);

                    break;
                }

                case PXCompilerSymbolLexerReal:
                {
                    PXF32 x = 0;

                    PXFileReadF(outputStream, &x);

                    PXTextFromInt(&pxTextBuffer, x);

                    break;
                }

                case PXCompilerSymbolLexerString:
                {
                    PXFileReadB(outputStream, textB, textBSize);
                    break;
                }
                }
            }

            printf("[YAML][%i] %s %s:%s\n", depth, emotySpace, A, textB);
        }

        default:
            break;
        }
    }
    outputStream->DataCursor = oldpos;

#endif

    return PXResultOK;
}

PXResult PXAPI PXYAMLSaveToFile(PXDOM PXREF pxDOM, PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
