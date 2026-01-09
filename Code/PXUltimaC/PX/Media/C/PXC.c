#include "PXC.h"

#include <PX/Compiler/PXCompiler.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/Media/PXDocument.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/PXECS.h>

const char PXCText[] = "C";

#define PXCDebugOutput 1

PXResult PXAPI PXCParsePreprocessorCondition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXBool isExpectedText = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    if (!isExpectedText)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingError,
            PXCText,
            "Preprocessor",
            "Makro 'if not defined' has invalid name"
        );
#endif
    }

#if PXCDebugOutput
    char buffer[64];

    PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source, pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 63);

    PXLogPrint
    (
        PXLoggingInfo,
        PXCText,
        "Preprocessor",
        "If not defined : %s",
        buffer
    );
#endif

    return PXResultInvalid;
}

PXResult PXAPI PXCParsePreprocessorDefine(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    const PXSize defineTypePosition = 0;// pxCompiler->DataCursor;

    // PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

    // Fetch name [Required]
    {
        PXCompilerSymbolEntryExtract(pxCompiler);

        const PXBool isExpectedText = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

        if (!isExpectedText)
        {
#if PXCDebugOutput
            PXLogPrint
            (
                PXLoggingInfo,
                PXCText,
                "Preprocessor",
                "Makro define has invalid name"
            );
#endif
        }

        // Write define name
        // PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
        // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

#if PXCDebugOutput
        char buffer[64];

        PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            PXCText,
            "Preprocessor",
            "Makro define name : %s",
            buffer
        );
#endif
    }


    PXI16U currentLineA =  pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    PXCompilerSymbolEntryPeek(pxCompiler);

    PXI16U currentLineB =  pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

    PXBool isNewLine = currentLineA != currentLineB;

    if (isNewLine)
    {
        // Is Makro-Flag

        // PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFlag, defineTypePosition);
    }
    else
    {
        switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
        case PXCompilerSymbolLexerBrackedRoundOpen:
        {
            // Is Makro-Function

            const PXSize position = 0;// outputStream->DataCursor;
            PXI8U parameterListLength = 0;

            //PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFunction, defineTypePosition);
            // PXFileWriteI8U(outputStream, 0xFF);

            PXCompilerSymbolEntryExtract(pxCompiler); // Remove the '('

            while (1)
            {
                PXCompilerSymbolEntryExtract(pxCompiler);

                const PXBool isClosingBreaked = PXCompilerSymbolLexerBrackedRoundClose ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

                if (isClosingBreaked)
                {
                    break;
                }

                ++parameterListLength;

                // PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                PXCompilerSymbolEntryPeek(pxCompiler);

                switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
                {
                case PXCompilerSymbolLexerComma:
                    PXCompilerSymbolEntryExtract(pxCompiler); // Extract comma
                    break;
                case PXCompilerSymbolLexerBrackedRoundClose:
                    // done, dont do anything
                    break;

                default: // Error
                    break;
                }
            }

            // PXFileWriteAtI8U(outputStream, parameterListLength, position);

            break;
        }
        default:
        {
            // Is makro-Value

            //  PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroValue, defineTypePosition);
            PXCompilerSymbolEntryExtract(pxCompiler);

            // PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
            //  PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

            break;
        }
        }
    }

    return PXResultInvalid;
}

PXResult PXAPI PXCParsePreprocessorIncluded(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypePreprocessorIncluded, pxCompiler->ReadInfo.DepthCurrent, &pxCodeDocumentElement, parrent);

    PXCompilerSymbolEntryExtract(pxCompiler);

    char* defineTypePosition = 0;
    PXSize libraryNameLength = 0;

    //PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

    switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
    {
    case PXCompilerSymbolLexerBracketAngleOpen:
    {
        // case A) #include <xxxxxxx>



        // PXFileWriteI8U(outputStream, 0xFF);

        while (1)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);

            const PXBool isString =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

            if (isString)
            {
                if (!defineTypePosition)
                {
                    defineTypePosition =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                }
#if 1

                libraryNameLength +=  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

                //PXConsoleWrite( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size);
#endif



                //  PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                PXCompilerSymbolEntryExtract(pxCompiler);
            }

            const PXBool isEnd =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerBracketAngleClose;

            if (isEnd)
            {
#if PXCDebugOutput
                //printf("\n");
#endif // 0
                break;
            }

            const PXBool isSlash =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerSlash ||  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerSlashBack;

            if (isSlash)
            {
                libraryNameLength += 1;

                //  PXFileWriteB(outputStream, "/", 1u);

#if PXCDebugOutput
                // printf("/");
#endif // 0
            }
            else
            {
#if PXCDebugOutput
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXCText,
                    "Parsing",
                    "Invalid Included"
                );
#endif
                break;
            }
        }

#if PXCDebugOutput
        char buffer[64];

        PXTextCopyA(defineTypePosition, libraryNameLength, buffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            PXCText,
            "Parsing",
            "Included library (global) : %s",
            buffer
        );
#endif

        pxCodeDocumentElement->NameAdress = defineTypePosition;
        pxCodeDocumentElement->NameSize = libraryNameLength;
        pxCodeDocumentElement->IsGlobal = PXTrue;

        PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);


        break;
    }
    case PXCompilerSymbolLexerString:
    {
        // Case B) #include "xxxxxxxx"



        const PXBool isExpectedText =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

        if (!isExpectedText)
        {
#if PXCDebugOutput
            PXLogPrint
            (
                PXLoggingError,
                PXCText,
                "Included",
                "Makro Included has invalid name"
            );
#endif
        }

        defineTypePosition = (char*)0;// PXFileCursorPosition(pxFile);

        // PXFileWriteI8U(outputStream, PXCLibraryPathTypeGlobal);
        // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


#if PXCDebugOutput
        char IncludedName[64];

        PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, IncludedName, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            PXCText,
            "Included",
            "Makro define name : %s",
            IncludedName
        );
#endif

        pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
        pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
        pxCodeDocumentElement->IsGlobal = PXFalse;

        PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

        break;
    }

    default:
    {
        break;
    }
    }

    return PXResultOK;
}

PXResult PXAPI PXCParsePreprocessorPragma(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXBool isExpectedText =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

    if (!isExpectedText)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingInfo,
            PXCText,
            "Parsing",
            "Makro pragma has invalid name"
        );
#endif
    }

#if PXCDebugOutput
    PXLogPrint
    (
        PXLoggingInfo,
        PXCText,
        "Parsing",
        "Makro pragma ..."
    );
    // PXConsoleWrite(pxCompilerSymbolEntry.Source, pxCompilerSymbolEntry.Size);
#endif

    return PXResultInvalid;
}

PXResult PXAPI PXCParseTypeDefinition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeInvalid, pxCompiler->ReadInfo.DepthCurrent, &pxCodeDocumentElement, parrent);

    pxCodeDocumentElement->IsTypeDefinition = PXTrue;

    if(pxCompiler->ReadInfo.Comment.Data)
    {
        pxCodeDocumentElement->CommentAdress = pxCompiler->ReadInfo.Comment.Data;
        pxCodeDocumentElement->CommentSize = pxCompiler->ReadInfo.Comment.Size;

        pxCompiler->ReadInfo.Comment.Line = 0;
        pxCompiler->ReadInfo.Comment.Data = 0;
        pxCompiler->ReadInfo.Comment.Size = 0;
    }

    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement(pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXCText,
        "Parsing",
        "type definition"
    );
#endif

    switch (keyWord)
    {
    case PXDocumentElementTypeStruct:
    {
        pxCodeDocumentElement->Type = PXDocumentElementTypeStruct;
        PXCParseTypeContainer(pxCompiler, pxCodeDocumentElement);
        break;
    }
    case PXDocumentElementTypeUnion:
    {
        pxCodeDocumentElement->Type = PXDocumentElementTypeUnion;
        PXCParseTypeEnum(pxCompiler, pxCodeDocumentElement);
        break;
    }
    case PXDocumentElementTypeEnum:
    {
        pxCodeDocumentElement->Type = PXDocumentElementTypeEnum;
        PXCParseTypeContainer(pxCompiler, pxCodeDocumentElement);
        break;
    }
    default:
    case PXDocumentElementTypeUnkown:
    {
        // unkown Return type

        PXCompilerSymbolEntryPeek(pxCompiler);

        // Parse possible attributes

        // Is function pointer?
        const PXBool isOpenBreaked = PXCompilerSymbolLexerBrackedRoundOpen ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

        if (isOpenBreaked)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);
            PXCompilerSymbolEntryPeek(pxCompiler);

            // check calling convention
            {
                const PXBool isText = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

                if (isText)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);
                    PXCompilerSymbolEntryPeek(pxCompiler);
                }
            }

            PXBool isAsterics = PXCompilerSymbolLexerAsterisk ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

            if (isAsterics)
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // remove '*'
                PXCompilerSymbolEntryExtract(pxCompiler); // Get new

                pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
#if 0
                char buffer[64];

                PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 64);

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXCText,
                    "Parsing",
                    "typedef Funtion : %s",
                    buffer
                );
#endif


                PXCompilerSymbolEntryExtract(pxCompiler);

                const PXBool isAsterics = PXCompilerSymbolLexerBrackedRoundClose ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;


                pxCodeDocumentElement->Type = PXDocumentElementTypeFunctionPointer;

                PXCParseTypeParameterList(pxCompiler, pxCodeDocumentElement);

            }
            else
            {
                isAsterics = 1;
            }

            //PXCParseEndOfCommand(pxDocument, pxFile);
        }

        // Unknown Type

        // can be function resturn type
        // can also be

    }
    }

    // Check for secoundary name
    {

    }

    PXCParseEndOfCommand(pxCompiler);

    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXCParseEndOfCommand(PXCompiler PXREF pxCompiler)
{
    const PXBool isSemicolon = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerSemiColon);

    if (!isSemicolon)
    {
        return PXActionRefusedParserSymbolNotAsExpected;
    }

    PXCompilerSymbolEntryForward(pxCompiler);

    return PXResultOK;
}

PXResult PXAPI PXCParseTypeParameterList(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    // expect '('
    const PXBool isOpenBracked = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBrackedRoundOpen);

    if (!isOpenBracked)
    {
        // Error
        //PXCompilerSymbolEntryExtract(pxFile, &pxCompilerSymbolEntry);
    }
    else
    {
        PXCompilerSymbolEntryExtract(pxCompiler);
    }

    for (PXBool doLoop = PXTrue; doLoop; )
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        switch ( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
        {
        case PXCompilerSymbolLexerBrackedRoundClose:
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            doLoop = PXFalse;
            break;
        }
        case PXCompilerSymbolLexerComma:
        {
            PXCompilerSymbolEntryForward(pxCompiler);
            // OK
            break;
        }
        default:
        {
            PXCParseTypeDeclarationElement(pxCompiler, parrent);
            break;
        }
        }
    }

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXCParseFunctionDefinition(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeFunction, pxCompiler->ReadInfo.DepthCurrent, &pxCodeDocumentElement, parrent);

    PXCompilerSymbolEntryExtract(pxCompiler);

    // Return type
    const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement(pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size);
    const PXBool isBuildIn = PXDocumentElementTypeUnkown != keyWord;

    {
        const PXBool isPointer = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerAsterisk);

        if (isPointer)
        {
            PXCompilerSymbolEntryForward(pxCompiler);
        }
    }


    char bufferReturnName[64];
    char bufferFunctionName[64];

    {
        PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, bufferReturnName, 64);
    }

    // check calling convention
    PXCompilerSymbolEntryExtract(pxCompiler);


    // Check name
    PXCompilerSymbolEntryExtract(pxCompiler);

    {
        pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
        pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

        PXCNameCleave(pxCompiler, pxCodeDocumentElement);

        PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, bufferFunctionName, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            PXCText,
            "Parsing",
            "Function : %6s %s",
            bufferReturnName,
            bufferFunctionName
        );
    }

    PXCParseTypeParameterList(pxCompiler, pxCodeDocumentElement);

    PXCParseEndOfCommand(pxCompiler);


    // ALL OK, update entry
    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    return PXResultInvalid;
}

PXResult PXAPI PXCParseTypeDeclarationElement(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXDocumentElementType pxDocumentElementType;


    switch(parent->Type)
    {
    case PXDocumentElementTypeUnion:
    case PXDocumentElementTypeStruct:
    case PXDocumentElementTypeClass:
        pxDocumentElementType = PXDocumentElementTypeClassMember;
        break;

    case PXDocumentElementTypeFunctionPointer:
    case PXDocumentElementTypeFunction:
        pxDocumentElementType = PXDocumentElementTypeFunctionParameter;
        break;

    default:
        break;
    }

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, pxDocumentElementType, pxCompiler->ReadInfo.DepthCurrent, &pxCodeDocumentElement, parent);

    // check if const
    {
        PXBool done = 0;

        do
        {
            PXCompilerSymbolEntryPeek(pxCompiler);

            if( pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerComment)
            {
                pxCodeDocumentElement->CommentAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                pxCodeDocumentElement->CommentSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeek(pxCompiler);
            }

            const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size);

            switch (keyWord)
            {
            case PXDocumentElementTypeChar:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "char"

                pxCodeDocumentElement->DataType |= PXTypeInt08U;

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                done = 1;

                break;
            }
            case PXDocumentElementTypeShort:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "short"

                pxCodeDocumentElement->DataType |= PXTypeInt16U;

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                done = 1;

                break;
            }
            case PXDocumentElementTypeInt:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataType |= PXTypeInt32U;

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                done = 1;

                break;
            }
            case PXDocumentElementTypeLong:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                // pxCo
                // deDocumentElement->DataType |= PXTypePXF32;

                done = 1;

                break;
            }
            case PXDocumentElementTypePXF32:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataType |= PXTypeF32;

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                done = 1;

                break;
            }
            case PXDocumentElementTypeDouble:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataType |= PXTypeF64;

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                done = 1;

                break;
            }
            case PXDocumentElementTypeBool:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                pxCodeDocumentElement->DataType |= PXTypeInt08U;

                done = 1;

                break;
            }

            case PXDocumentElementTypeConst:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                pxCodeDocumentElement->DataType |= PXTypeReadOnly;

                break;
            }
            case PXDocumentElementTypeUnsigned:
            {
                PXCompilerSymbolEntryExtract(pxCompiler); // consume "unsigned"

                pxCodeDocumentElement->DataType |= PXTypeUnsigned;

                break;
            }

            case PXDocumentElementTypeUnion: // might be a union inside a struct
            {
                PXCodeDocumentElement pxCodeDocumentElementChild;
                PXClear(PXCodeDocumentElement, &pxCodeDocumentElementChild);
                pxCodeDocumentElementChild.Type = PXDocumentElementTypeUnion;
                pxCodeDocumentElementChild.Hierachy.Parrent = pxCodeDocumentElement;

                PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                PXCParseTypeContainer(pxCompiler, &pxCodeDocumentElementChild);

                //PXCompilerSymbolEntryExtract(pxCompiler); // consume ";"

                done = 1;

                // break;

                return PXResultOK;


            }
            //  case PXCompilerSymbolLexerAsterisk:
            // {
            //    PXCompilerSymbolEntryExtract(pxCompiler);

            //     pxCodeDocumentElement->DataType |= PXTypeAdressMask;
//
            //     break;
            //  }
            default:
            {
                PXCompilerSymbolEntryExtract(pxCompiler);

                const PXBool isTypeName =  pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

                pxCodeDocumentElement->TypeNameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                pxCodeDocumentElement->TypeNameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

                done = 1;
                break;
            }
            }
        }
        while (!done);
    }

    // Get data entry
    //PXCompilerSymbolEntryExtract(pxCompiler);
    //PXBool aa = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

    PXBool isPointer = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerAsterisk);

    if (isPointer)
    {
        pxCodeDocumentElement->DataType |= PXTypeAdressMask;

        // Throw away '*'
        PXCompilerSymbolEntryExtract(pxCompiler);

        // Get new element
        PXCompilerSymbolEntryPeek(pxCompiler);

        const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size);
        const PXBool isConst = PXDocumentElementTypeConst == keyWord;

        if (isConst)
        {
            // Throw away 'const'
            PXCompilerSymbolEntryExtract(pxCompiler);
        }
    }

    // Name

    PXCompilerSymbolEntryExtract(pxCompiler);
    const PXBool isName = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;


    pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;




    const PXBool isArray = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);

    if(isArray)
    {
        PXCompilerSymbolEntryForward(pxCompiler); // Consume

        const PXBool isInt = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerNumeric);

        if(!isInt)
        {
            return PXResultInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler); // Consume

        pxCodeDocumentElement->ArrayAmount =  pxCompiler->ReadInfo.SymbolEntryCurrent.I32U;

        const PXBool isclosing = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerBracketSquareClose);
    }


    // Check for terminating character
    {
        switch(pxCodeDocumentElement->Type)
        {
        case PXDocumentElementTypeClassMember:
        {
            const PXBool isSemiColon = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerSemiColon);

            if(isSemiColon)
            {
                const PXSize currentLine =  pxCompiler->ReadInfo.SymbolEntryCurrent.Line;

                PXCompilerSymbolEntryForward(pxCompiler);

                // Check for comment, often we put these at the left on the member

                PXCompilerSymbolEntryPeek(pxCompiler);

                const PXBool hasCommentAttached =
                    (PXCompilerSymbolLexerComment ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID) &&
                    ( pxCompiler->ReadInfo.SymbolEntryCurrent.Line == currentLine);

                if(hasCommentAttached)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // Consume comment

                    pxCodeDocumentElement->CommentAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
                    pxCodeDocumentElement->CommentSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;
                }
            }

            break;
        }
        case PXDocumentElementTypeFunctionParameter:
        {
            const PXBool isComma = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerComma);



            break;
        }
        }
    }

#if PXLogEnable && 0
    char nameBuffer[64];
    PXTextCopyA(pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize, nameBuffer, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        PXCText,
        "Parsing",
        "Member : %s",
        nameBuffer
    );
#endif

    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    return PXResultInvalid;
}

void PXAPI PXCNameCleave(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElement)
{
    char* name = pxCodeDocumentElement->AliasAdress ? pxCodeDocumentElement->AliasAdress : pxCodeDocumentElement->NameAdress;
    PXSize nameSize = pxCodeDocumentElement->AliasSize ? pxCodeDocumentElement->AliasSize : pxCodeDocumentElement->NameSize;

    if(pxCodeDocumentElement->Hierachy.Parrent)
    {
        PXCodeDocumentElement* parent = (PXCodeDocumentElement*)pxCodeDocumentElement->Hierachy.Parrent;

        PXBool check = PXMemoryCompare
                       (
                           name,
                           nameSize,
                           parent->NameSpaceAdress,
                           parent->NameSpaceSize
                       );

        if(check)
        {
            pxCodeDocumentElement->NameSpaceAdress = name;
            pxCodeDocumentElement->NameSpaceSize = parent->NameSpaceSize;

            pxCodeDocumentElement->NameClassAdress = name + parent->NameSpaceSize;
            pxCodeDocumentElement->NameClassSize = nameSize - parent->NameSpaceSize;


            char* className = 0;
            PXSize classSize = 0;

            if(parent->NameClassAdress)
            {
                className = parent->NameClassAdress;
                classSize = parent->NameClassSize;
            }
            else
            {
                className = parent->NameShortAdress;
                classSize = parent->NameShortSize;
            }




            check = PXMemoryCompare
                    (
                        pxCodeDocumentElement->NameClassAdress,
                        pxCodeDocumentElement->NameClassSize,
                        className,
                        classSize
                    );

            if(check)
            {
                pxCodeDocumentElement->NameShortAdress = pxCodeDocumentElement->NameClassAdress + classSize;
                pxCodeDocumentElement->NameShortSize = pxCodeDocumentElement->NameClassSize - classSize;


                pxCodeDocumentElement->NameClassSize -= pxCodeDocumentElement->NameShortSize;
            }

            // For C enums, we have a very detailed name, we need to cleave this further
            if(pxCodeDocumentElement->Type == PXDocumentElementTypeEnumMember)
            {
                check = PXMemoryCompare
                        (
                            pxCodeDocumentElement->NameShortAdress,
                            pxCodeDocumentElement->NameShortSize,
                            parent->NameShortAdress,
                            parent->NameShortSize
                        );

                if(check)
                {
                    pxCodeDocumentElement->NameShortAdress += parent->NameShortSize;
                    pxCodeDocumentElement->NameShortSize -= parent->NameShortSize;
                }
            }
        }
    }
    else
    {
        pxCodeDocumentElement->NameSpaceAdress = name;
        pxCodeDocumentElement->NameSpaceSize = PXTextPascalCaseCleave(name, nameSize);

        pxCodeDocumentElement->NameClassAdress = 0;
        pxCodeDocumentElement->NameClassSize = 0;

        pxCodeDocumentElement->NameShortAdress = name + pxCodeDocumentElement->NameSpaceSize;
        pxCodeDocumentElement->NameShortSize = nameSize - pxCodeDocumentElement->NameSpaceSize;
    }
}

PXResult PXAPI PXCParseEnumMember(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeEnumMember, pxCompiler->ReadInfo.DepthCurrent, &pxCodeDocumentElement, parrent);

    const PXBool isValidText = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric);

    if(!isValidText)
    {
        return PXResultOK;
    }

    PXCompilerSymbolEntryExtract(pxCompiler);

    pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
    pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

    PXCNameCleave(pxCompiler, pxCodeDocumentElement);

    // TODO: what if an enum has a predefined value!?
    PXCompilerSymbolEntryPeek(pxCompiler);

    if( pxCompiler->ReadInfo.SymbolEntryCurrent.ID == PXCompilerSymbolLexerComment)
    {
        pxCodeDocumentElement->CommentAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
        pxCodeDocumentElement->CommentSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

        PXCompilerSymbolEntryExtract(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
    }

    switch( pxCompiler->ReadInfo.SymbolEntryCurrent.ID)
    {
    case PXCompilerSymbolLexerSemiColon: // End of all
        break;

    case PXCompilerSymbolLexerComma: // end of line
        PXCompilerSymbolEntryExtract(pxCompiler);
        break;

    default:
        break;
    }


    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    return PXResultOK;
}

PXResult PXAPI PXCParseTypeContainer(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElement)
{
    // Get name [optional]
    {
        const PXCompilerSymbolLexer pxCompilerSymbolLexer[2] =
        {
            PXCompilerSymbolLexerGeneric,
            PXCompilerSymbolLexerBracketCurlyOpen
        };

        const PXBool isValidToken = PXCompilerSymbolEntryEnsureCheckList(pxCompiler, pxCompilerSymbolLexer, 2);

        if(!isValidToken)
        {
            return PXResultOK;
        }

        const PXBool isName = PXCompilerSymbolLexerGeneric ==  pxCompiler->ReadInfo.SymbolEntryCurrent.ID;

        if(isName)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);

            pxCodeDocumentElement->NameAdress =  pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
            pxCodeDocumentElement->NameSize =  pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

            PXCNameCleave(pxCompiler, pxCodeDocumentElement);

#if PXLogEnable
            char buffer[64];

            PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 64);

            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parsing",
                "Container : %s",
                buffer
            );
#endif
        }
        else
        {
            // We dont have a name, it's an anonym container

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parsing",
                "Container : --- unnamed ---"
            );
#endif
        }
    }

    // Register element now
    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    // expect '{'
    {
        const PXBool isCurlyBrackedOpen = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketCurlyOpen);

        if (isCurlyBrackedOpen)
        {
            ++pxCompiler->ReadInfo.DepthCurrent;

            PXCompilerSymbolEntryForward(pxCompiler);

            PXBool isCurlyBrackedClose = 0;

            for(;;)
            {
                switch(pxCodeDocumentElement->Type)
                {
                case PXDocumentElementTypeStruct:
                case PXDocumentElementTypeUnion:
                case PXDocumentElementTypeClass:
                {
                    PXCParseTypeDeclarationElement(pxCompiler, pxCodeDocumentElement);

                    break;
                }
                case PXDocumentElementTypeEnum:
                {
                    PXCParseEnumMember(pxCompiler, pxCodeDocumentElement);
                    break;
                }

                default:
                    break;
                }

                ++pxCodeDocumentElement->MemberAmount;

                // expect '}'
                isCurlyBrackedClose = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketCurlyClose);

                if (isCurlyBrackedClose)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);
                    break;
                }
            }

            --pxCompiler->ReadInfo.DepthCurrent;
        }
        else
        {
            pxCodeDocumentElement->IsPreDefine = PXTrue;
        }
    }

    // Alias [optional]
    {
        const PXBool isText = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerGeneric);

        if (isText)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);

            // Alias
            pxCodeDocumentElement->AliasAdress = pxCompiler->ReadInfo.SymbolEntryCurrent.Source;
            pxCodeDocumentElement->AliasSize = pxCompiler->ReadInfo.SymbolEntryCurrent.Size;

            PXCNameCleave(pxCompiler, pxCodeDocumentElement);

#if PXLogEnable
            char buffer[64];

            PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 64);

            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parsing",
                "alias : %s",
                buffer
            );
#endif
        }
    }

    // Update value with new amount of members
    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    /*
    for(PXCodeDocumentElement* i = pxCodeDocumentElement->ElementChildFirstBorn; i ; i = i->ElementSibling)
    {
        PXCNameCleave(pxCompiler, i);
        PXCodeDocumentElementAdd(pxCompiler->CodeDocument, i);
    }
    */

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXCParseTypeEnum(PXCompiler PXREF pxCompiler, PXCodeDocumentElement PXREF pxCodeDocumentElementParent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    char buffer[64];

    PXTextCopyA( pxCompiler->ReadInfo.SymbolEntryCurrent.Source,  pxCompiler->ReadInfo.SymbolEntryCurrent.Size, buffer, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "enum : %s",
        buffer
    );

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXCLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "--- Begin ---"
    );
#endif

    PXCodeDocument* pxDocument = (PXCodeDocument*)pxResourceLoadInfo->ResourceTarget;
    PXClear(PXCodeDocument, pxDocument);

    PXFile* tokenSteam = PXFileCreate();

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.CodeDocument = pxDocument;
    pxCompiler.ReadInfo.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.ReadInfo.FileCache = tokenSteam;
    pxCompiler.Flags = PXCompilerKeepComments | PXCompilerKeepAnalyseTypes;
    pxCompiler.CommentSingleLineSize = 2;
    pxCompiler.CommentSingleLine = "//";
    pxCompiler.CommentMultibleLineBegin = "/*";
    pxCompiler.CommentMultibleLineBeginSize = 2;
    pxCompiler.CommentMultibleLineEnd = "*/";
    pxCompiler.CommentMultibleLineEndSize = 2;

    PXCodeDocumentElement* pxCodeDocumentElementRoot = PXNull;

    char mainNodeName[64];

    //-----------------------------------------------------
    // Lexer - Level I
    //-----------------------------------------------------
    PXCompilerLexicalAnalysis(&pxCompiler);

    PXCodeDocumentElementGenerateChild(pxDocument, PXDocumentElementTypeFile, 0, &pxCodeDocumentElementRoot, PXNull);


    {
        PXText filePath;
        PXTextConstructBufferA(&filePath, PXPathSizeMax);

        PXFilePath(pxResourceLoadInfo->FileReference, &filePath, PXFalse);

        PXFilePathStructure pxFilePathStructure;

        PXFilePathSplitt(&filePath, &pxFilePathStructure);

        pxCodeDocumentElementRoot->NameAdress = PXMemoryHeapCallocT(char, pxFilePathStructure.FileName.SizeUsed);
        pxCodeDocumentElementRoot->NameSize += PXTextCopyA(pxFilePathStructure.FileName.A, pxFilePathStructure.FileName.SizeUsed, pxCodeDocumentElementRoot->NameAdress, pxCodeDocumentElementRoot->NameSize);

        PXCNameCleave(&pxCompiler, pxCodeDocumentElementRoot);

        PXCodeDocumentElementAdd(pxCompiler.CodeDocument, pxCodeDocumentElementRoot);
    }

    ++pxCompiler.ReadInfo.DepthCurrent;

    // filename
    //pxCodeDocumentElementRoot->NameAdress =


    //-----------------------------------------------------
    // Precompiler
    //-----------------------------------------------------



    //-----------------------------------------------------
    // Compile
    //-----------------------------------------------------

    for (;;)
    {
        const PXBool isAtEnd = PXFileIsAtEnd(tokenSteam);

        if(isAtEnd)
        {
            break;
        }

        PXCompilerSymbolEntryExtract(&pxCompiler);

        switch (pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
        {
        case PXCompilerSymbolLexerComment:
        {
            // Do nothing
            pxCompiler.ReadInfo.Comment.Position = PXCommentPositionHeader;
            pxCompiler.ReadInfo.Comment.Line = pxCompiler.ReadInfo.SymbolEntryCurrent.Line;
            pxCompiler.ReadInfo.Comment.Data = pxCompiler.ReadInfo.SymbolEntryCurrent.Source;
            pxCompiler.ReadInfo.Comment.Size = pxCompiler.ReadInfo.SymbolEntryCurrent.Size;
            break;
        }
        case PXCompilerSymbolLexerHash: // Probably a preprocessor definition
        {
            PXCompilerSymbolEntryExtract(&pxCompiler); // move from '#' to next one

            const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

            switch (keyWord)
            {
            case PXDocumentElementTypePreprocessorDefine:
            {
                PXCParsePreprocessorDefine(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypePreprocessorIfDefined:
            {
                PXCParsePreprocessorCondition(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypePreprocessorIfNotDefined:
            {
                PXCParsePreprocessorCondition(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypePreprocessorIncluded:
            {
                PXCParsePreprocessorIncluded(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypePreprocessorPragma:
            {
                PXCParsePreprocessorPragma(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypePreprocessorDefinitionEnd:
            {
#if PXCDebugOutput
                PXLogPrint
                (
                    PXLoggingInfo,
                    "C",
                    "Parsing",
                    "Makro : End definition"
                );
#endif

                // OK?
                break;
            }

            default:
                break;
            }

            break;
        }
        case PXCompilerSymbolLexerGeneric:
        {
            const PXDocumentElementType keyWord = PXDocumentElementAnalyseElement(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, pxCompiler.ReadInfo.SymbolEntryCurrent.Size);

            switch (keyWord)
            {
            case PXDocumentElementTypeTypeDefinition:
            {
                PXCParseTypeDefinition(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypeEnum:
            {
                // && pxCodeDocumentElement.Type = PXDocumentElementTypeEnum;

                // PXCParseTypeEnum(&pxCompiler, &pxCodeDocumentElement);
                break;
            }
            case PXDocumentElementTypeUnion:
            {
                // pxCodeDocumentElement.Type = PXDocumentElementTypeUnion;

                // PXCParseTypeContainer(&pxCompiler, &pxCodeDocumentElement);
                break;
            }
            case PXDocumentElementTypeStruct:
            {
                // pxCodeDocumentElement.Type = PXDocumentElementTypeStruct;

                // PXCParseTypeContainer(&pxCompiler, &pxCodeDocumentElement);
                break;
            }
            case PXDocumentElementTypeExtern:
            {
                PXCompilerSymbolEntryExtract(&pxCompiler);

                switch (pxCompiler.ReadInfo.SymbolEntryCurrent.ID)
                {
                case PXCompilerSymbolLexerString: // Need to me the extern "C" thing
                {
                    const PXBool isNameMangeling = (pxCompiler.ReadInfo.SymbolEntryCurrent.Source[0] == 'C') && (pxCompiler.ReadInfo.SymbolEntryCurrent.Size == 1);

                    if (isNameMangeling)
                    {
                        // Invalid extern spesifieer
                    }

                    PXCompilerSymbolEntryPeek(&pxCompiler);

                    const PXBool isWrapping = PXCompilerSymbolLexerBracketCurlyOpen == pxCompiler.ReadInfo.SymbolEntryCurrent.ID;

                    if (isWrapping)
                    {
                        // everything is C-Style now

                        PXCompilerSymbolEntryExtract(&pxCompiler); // Consome '{'

#if PXCDebugOutput

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "C",
                            "Parsing",
                            "Disable name mangeling in C++ for following block"
                        );
#endif
                    }
                    else
                    {
                        // extern "C" xxxxxxxx

                        // Expect funtion
                    }

                    break;
                }

                default:
                    break;
                }

                break;
            }
            case PXDocumentElementTypeVoid:
            {
                PXCParseFunctionDefinition(&pxCompiler, pxCodeDocumentElementRoot);
                break;
            }
            case PXDocumentElementTypeUnkown: // Might declaration of variable or function.
            {
                // as we have an unkown type, lets look it up.

                // There is no result, so we take this as an extra unrsolved paramater

                const PXSize index = PXTextFindFirstCharacterBeforeA(pxCompiler.ReadInfo.SymbolEntryCurrent.Source, 260, '(', ';');
                const PXBool isFunxtion = index != (PXSize)-1;

                if (isFunxtion)
                {
                    PXCParseFunctionDefinition(&pxCompiler, pxCodeDocumentElementRoot);
                }

                break;
            }

            break;
            }

            break;
        }
        default:
        {
#if PXCDebugOutput
            PXLogPrint
            (
                PXLoggingError,
                "C",
                "Parsing",
                "Invalid code on this posision"
            );
#endif
            break;
        }
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "--- Finished ---"
    );
#endif

    PXCodeDocumentElementPrintAll(pxDocument);

    return PXResultOK;
}

PXResult PXAPI PXCSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}

/*

       pxCElement->ElementVariable.IsKnownPrimitve = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsKnownPrimitive);
            pxCElement->ElementVariable.IsConstantType = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsConstType);
            pxCElement->ElementVariable.IsInRegister = PXFlagIsSet(variableInfoFlags, MemberFieldFlagRegister);
            pxCElement->ElementVariable.IsSigned = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsSigned);
            pxCElement->ElementVariable.IsAdress = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsAdress);
            pxCElement->ElementVariable.IsAdressConstant = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsAdressConst);
            pxCElement->ElementVariable.IsAdressVolitile = PXFlagIsSet(variableInfoFlags, MemberFieldFlagVolatile);
            pxCElement->ElementVariable.IsAdressRestricted = PXFlagIsSet(variableInfoFlags, MemberFieldFlagResticted);

*/
