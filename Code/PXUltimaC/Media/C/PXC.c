#include "PXC.h"

#include <Compiler/PXCompiler.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>
#include <Media/PXDocument.h>

#define PXCDebugOutput 1

PXBool PXAPI PXCElementHasName(PXCElement* const pxCElement)
{
    const PXBool hasName = pxCElement->Name && pxCElement->NameSizeCurrent;
   // const PXBool hasAlias = pxCElement->NameAlias && pxCStructure->NameAliasSizeCurrent;

    return hasName;
}

void PXAPI PXCElementClear(PXCElement* const pxCElement)
{
    char* name = pxCElement->Name;
    PXInt8U nameSize = pxCElement->NameSizeMaximal;

    char* nameSecound = pxCElement->ElementStructure.NameAlias;
    PXInt8U nameSizeSecound = pxCElement->ElementStructure.NameAliasSizeMaximal;

    PXMemoryClear(name, nameSize);
    PXMemoryClear(nameSecound, nameSizeSecound);

    PXClear(PXCElement, pxCElement);

    pxCElement->Name = name;
    pxCElement->NameSizeMaximal = nameSize;

    pxCElement->ElementStructure.NameAlias = nameSecound;
    pxCElement->ElementStructure.NameAliasSizeMaximal = nameSizeSecound;
}

CKeyWord PXAPI PXCFileAnalyseElement(const char* name, const PXSize nameSize)
{
    switch (nameSize)
    {
        case 2:
        {
            const PXInt16U key = PXInt16FromAdress(name);

            switch (key)
            {
                case PXInt16Make('i', 'f'):
                    return CKeyWordIf;

                case PXInt16Make('d', 'o'):
                    return CKeyWordDo;
            }

            break;
        }
        case 3:
        {
            const PXInt32U key = PXInt24FromAdress(name);

            switch (key)
            {
                case PXInt24Make('i', 'n', 't'):
                    return CKeyWordInt;

                case PXInt24Make('f', 'o', 'r'):
                    return CKeyWordFor;
            }

            break;
        }
        case 4:
        {
            const PXInt32U key = PXInt32FromAdress(name);

            switch (key)
            {
                case PXInt32Make('c', 'a', 's', 'e'): return CKeyWordCase;
                case PXInt32Make('t', 'r', 'u', 'e'): return CKeyWordTrue;
                case PXInt32Make('b', 'o', 'o', 'l'): return CKeyWordBool;
                case PXInt32Make('c', 'h', 'a', 'r'): return CKeyWordChar;
                case PXInt32Make('l', 'o', 'n', 'g'): return CKeyWordLong;
                case PXInt32Make('a', 'u', 't', 'o'): return CKeyWordAutomaticType;
                case PXInt32Make('e', 'l', 's', 'e'): return CKeyWordElse;
                case PXInt32Make('g', 'o', 't', 'o'): return CKeyWordGoTo;
                case PXInt32Make('e', 'n', 'u', 'm'): return CKeyWordEnum;
                case PXInt32Make('v', 'o', 'i', 'd'): return CKeyWordVoid;
            }

            break;
        }
        case 5u:
        {
            const PXInt64U key = PXInt40FromAdress(name);

            switch (key)
            {
                case PXInt40Make('s', 'h', 'o', 'r', 't'): return CKeyWordShort;
                case PXInt40Make('u', 'n', 'i', 'o', 'n'): return CKeyWordUnion;
                case PXInt40Make('w', 'h', 'i', 'l', 'e'): return CKeyWordWhile;
                case PXInt40Make('f', 'a', 'l', 's', 'e'): return CKeyWordFalse;
                case PXInt40Make('f', 'l', 'o', 'a', 't'): return CKeyWordFloat;
                case PXInt40Make('c', 'o', 'n', 's', 't'): return CKeyWordConst;
                case PXInt40Make('b', 'r', 'e', 'a', 'k'): return CKeyWordBreak;
                case PXInt40Make('i', 'f', 'd', 'e', 'f'): return CKeyWordIfDefined;
                case PXInt40Make('e', 'n', 'd', 'i', 'f'): return CKeyWordDefinitionEnd;
            }

            break;
        }
        case 6u:
        {
            const PXInt64U key = PXInt48FromAdress(name);

            switch (key)
            {
                case PXInt48Make('s', 'i', 'z', 'e', 'o', 'f'): return CKeyWordTypeSize;
                case PXInt48Make('r', 'e', 't', 'u', 'r', 'n'): return CKeyWordReturn;
                case PXInt48Make('e', 'x', 't', 'e', 'r', 'n'): return CKeyWordExtern;
                case PXInt48Make('s', 't', 'a', 't', 'i', 'c'): return CKeyWordStatic;
                case PXInt48Make('s', 't', 'r', 'u', 'c', 't'): return CKeyWordStruct;
                case PXInt48Make('d', 'o', 'u', 'b', 'l', 'e'): return CKeyWordDouble;
                case PXInt48Make('s', 'i', 'g', 'n', 'e', 'd'): return CKeyWordSigned;
                case PXInt48Make('s', 'w', 'i', 't', 'c', 'h'): return CKeyWordSwitch;
                case PXInt48Make('i', 'f', 'n', 'd', 'e', 'f'): return CKeyWordIfNotDefined;
                case PXInt48Make('d', 'e', 'f', 'i', 'n', 'e'): return CKeyWordDefine;
                case PXInt48Make('p', 'r', 'a', 'g', 'm', 'a'): return CKeyWordPragma;
            }

            break;
        }
        case 7u:
        {
            const PXInt64U key = PXInt56FromAdress(name);

            switch (key)
            {
                case PXInt56Make('d', 'e', 'f', 'a', 'u', 'l', 't'): return CKeyWordDefault;
                case PXInt56Make('t', 'y', 'p', 'e', 'd', 'e', 'f'): return CKeyWordTypeDefinition;
                case PXInt56Make('i', 'n', 'c', 'l', 'u', 'd', 'e'): return CKeyWordInclude;
            }

            break;
        }
        case 8u:
        {
            const PXInt64U key = PXInt64FromAdress(name);

            switch (key)
            {
                case PXInt64Make('c', 'o', 'n', 't', 'i', 'n', 'u', 'e'): return CKeyWordContinue;
                case PXInt64Make('u', 'n', 's', 'i', 'g', 'n', 'e', 'd'): return CKeyWordUnsigned;
                case PXInt64Make('r', 'e', 's', 't', 'r', 'i', 'c', 't'): return CKeyWordRegister;
                case PXInt64Make('v', 'o', 'l', 'a', 't', 'i', 'l', 'e'): return CKeyWordVolitile;
                case PXInt64Make('r', 'e', 'g', 'i', 's', 't', 'e', 'r'): return CKeyWordRestrict;

            }

            break;
        }
    }

    return CKeyWordUnkown;
}

PXBool PXAPI PXCFileParseDeclaration(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{
    PXBool finished = 0;
    PXInt8U flagList = MemberFieldFlagIsSigned;
    PXInt16U sizeOfType = 0;

    const PXBool isTypeName = PXCompilerSymbolLexerGeneric == compilerSymbolEntry->ID;

    if (!isTypeName)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingInfo,
            "C",
            "Parse-Enum",
            "type expected but got something else"
        );
#endif
    }

    // Try fetch type
    const CKeyWord fieldType = PXCFileAnalyseElement(compilerSymbolEntry->Source, compilerSymbolEntry->Size);

    const PXBool isStructure =
        CKeyWordEnum == fieldType ||
        CKeyWordUnion == fieldType ||
        CKeyWordStruct == fieldType;

    if (isStructure)
    {
        //PXCFileParseStructure(inputStream, outputStream, fieldType, PXFalse);
        return PXTrue;
    }


    PXFileWriteI8U(outputStream, PXCStructureTypeStructElement);


    PXBool doesKnowCustomType = PXFalse;
    CKeyWord variableType = CKeyWordInvalid;


    PXSize variableNameSize = 0;
    char variableName[64];

    PXSize variableTypeNameSize = 0;
    char variableTypeName[64];


    do
    {
        const PXBool isFinished = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry->ID;

        if (isFinished)
        {
            const PXBool isKnownPrimitive = flagList & MemberFieldFlagIsKnownPrimitive;

            // if we have a primitive type, we only have one 'unkown' name.
            // If we dont have a primitive, we have two.


            if (PXFlagIsSet(flagList, MemberFieldFlagIsAdress))
            {
                sizeOfType = sizeof(void*);
            }

                     // Write name
            PXFileWriteI8U(outputStream, variableNameSize);
            PXFileWriteB(outputStream, variableName, variableNameSize);


            PXFileWriteI8U(outputStream, flagList); // Write flags
            PXFileWriteI16U(outputStream, sizeOfType); // Write size of Type

            if (isKnownPrimitive)
            {
                PXFileWriteI8U(outputStream, variableType); // Write primitive type
            }
            else
            {
                // We have a 2nd name, so its a custom type. Write name of custom variable..
                PXFileWriteI8U(outputStream, variableTypeNameSize);
                PXFileWriteB(outputStream, variableTypeName, variableTypeNameSize);
            }


#if PXCDebugOutput
            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parse",
                "| MemberField: %s Type, Name %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n"
                "| - %-17s : %-20s |\n",
                variableType, variableName,
                "Const", PXFlagIsSet(flagList, MemberFieldFlagIsConstType) ? "Yes" : "No",
                "Singed", PXFlagIsSet(flagList, MemberFieldFlagIsSigned) ? "Yes" : "No",
                "Adress", PXFlagIsSet(flagList, MemberFieldFlagIsAdress) ? "Yes" : "No",
                "Readonly Adress", PXFlagIsSet(flagList, MemberFieldFlagIsAdressConst) ? "Yes" : "No",
                "Volatile", PXFlagIsSet(flagList, MemberFieldFlagVolatile) ? "Yes" : "No",
                "Resticted", PXFlagIsSet(flagList, MemberFieldFlagResticted) ? "Yes" : "No",
                "Primitive", PXFlagIsSet(flagList, MemberFieldFlagIsKnownPrimitive) ? "Yes" : "No"
            );
#endif 


            return PXTrue;
        }

        switch (compilerSymbolEntry->ID)
        {
            case PXCompilerSymbolLexerGeneric: // Is element
            {
                const CKeyWord typeKey = PXCFileAnalyseElement(compilerSymbolEntry->Source, compilerSymbolEntry->Size);

                switch (typeKey)
                {
                    case CKeyWordTypeDefinition:
                    {
                        //PXCFileParseTypedef(inputStream, outputStream);
                        break;
                    }
                    case CKeyWordUnion:
                    case CKeyWordEnum:
                    case CKeyWordStruct:
                        //PXCFileParseStructure(inputStream, outputStream, typeKey, PXFalse);
                        break;

                    case CKeyWordChar:
                    {
                        variableType = CKeyWordChar;
                        sizeOfType = sizeof(char);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordShort:
                    {
                        variableType = CKeyWordShort;
                        sizeOfType = sizeof(short);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordInt:
                    {
                        variableType = CKeyWordInt;
                        sizeOfType = sizeof(int);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordLong:
                    {
                        variableType = CKeyWordLong;
                        sizeOfType = sizeof(long);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordFloat:
                    {
                        variableType = CKeyWordFloat;
                        sizeOfType = sizeof(float);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordDouble:
                    {
                        variableType = CKeyWordDouble;
                        sizeOfType = sizeof(double);
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordBool:
                    {
                        variableType = CKeyWordBool;
                        sizeOfType = 1u;
                        flagList |= MemberFieldFlagIsKnownPrimitive;
                        break;
                    }
                    case CKeyWordSigned:
                    {
                        flagList |= MemberFieldFlagIsSigned;
                        break;
                    }
                    case CKeyWordUnsigned:
                    {
                        flagList ^= MemberFieldFlagIsSigned;
                        break;
                    }
                    case CKeyWordConst:
                    {
                        flagList |= MemberFieldFlagIsConstType;
                        break;
                    }
                    case CKeyWordRegister:
                    {
                        flagList |= MemberFieldFlagRegister;
                        break;
                    }
                    case CKeyWordVolitile:
                    {
                        flagList |= MemberFieldFlagVolatile;
                        break;
                    }
                    case CKeyWordRestrict:
                    {
                        flagList |= MemberFieldFlagResticted;
                        break;
                    }
                    case CKeyWordUnkown: // could be the name
                    {
                        const PXBool isTypeKnown = PXFlagIsSet(flagList, MemberFieldFlagIsKnownPrimitive) || doesKnowCustomType;

                        if (isTypeKnown)
                        {
                            // Name
                            variableNameSize = PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, variableName, 64);
                        }
                        else
                        {
                            variableTypeNameSize = PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, variableTypeName, 64);

                            doesKnowCustomType = PXTrue;
                        }

                        break;
                    }
                    default:
                        return PXFalse;
                }



                break;
            }
            case PXCompilerSymbolLexerAsterisk: // Can be pointer
            {
                flagList |= MemberFieldFlagIsAdress;

                break;
            }

            default:
                return PXFalse;
        }






        //PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

        if (compilerSymbolEntry->ID == PXCompilerSymbolLexerComment)
        {
           // PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);
        }
    } while (0);

    // printf("type name expected but got something else\n");

        // printf("';' expected but got something else\n");

}

PXActionResult PXAPI PXCParsePreprocessorCondition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXBool isExpectedText = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

    if (!isExpectedText)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingError,
            "C",
            "Preprocessor",
            "Makro 'if not defined' has invalid name"
        );
#endif
    }

#if PXCDebugOutput
    char buffer[64];

    PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 63);

    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Preprocessor",
        "If not defined : %s",
        buffer
    );
#endif

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParsePreprocessorDefine(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    const PXSize defineTypePosition = 0;// pxCompiler->DataCursor;

    // PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

     // Fetch name [Required]
    {
        PXCompilerSymbolEntryExtract(pxCompiler);

        const PXBool isExpectedText = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

        if (!isExpectedText)
        {
#if PXCDebugOutput
            PXLogPrint
            (
                PXLoggingInfo,
                "C",
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

        PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            "C",
            "Preprocessor",
            "Makro define name : %s",
            buffer
        );
#endif
    }


    PXInt16U currentLineA = pxCompiler->SymbolEntryCurrent.Line;

    PXCompilerSymbolEntryPeek(pxCompiler);

    PXInt16U currentLineB = pxCompiler->SymbolEntryCurrent.Line;

    PXBool isNewLine = currentLineA != currentLineB;

    if (isNewLine)
    {
        // Is Makro-Flag

       // PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFlag, defineTypePosition);
    }
    else
    {
        switch (pxCompiler->SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerBrackedRoundOpen:
            {
                // Is Makro-Function

                const PXSize position = 0;// outputStream->DataCursor;
                PXInt8U parameterListLength = 0;

                //PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFunction, defineTypePosition);
               // PXFileWriteI8U(outputStream, 0xFF);

                PXCompilerSymbolEntryExtract(pxCompiler); // Remove the '('

                while (1)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);

                    const PXBool isClosingBreaked = PXCompilerSymbolLexerBrackedRoundClose == pxCompiler->SymbolEntryCurrent.ID;

                    if (isClosingBreaked)
                    {
                        break;
                    }

                    ++parameterListLength;

                    // PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                    // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                    PXCompilerSymbolEntryPeek(pxCompiler);

                    switch (pxCompiler->SymbolEntryCurrent.ID)
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

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParsePreprocessorInclude(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeInclude, pxCompiler->Depth, &pxCodeDocumentElement, parrent);

    PXCompilerSymbolEntryExtract(pxCompiler);

    char* defineTypePosition = 0;
    PXSize libraryNameLength = 0;

    //PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

    switch (pxCompiler->SymbolEntryCurrent.ID)
    {
        case PXCompilerSymbolLexerBracketAngleOpen:
        {
            // case A) #include <xxxxxxx>



           // PXFileWriteI8U(outputStream, 0xFF);

            while (1)
            {
                PXCompilerSymbolEntryExtract(pxCompiler);

                const PXBool isString = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

                if (isString)
                {
                    if (!defineTypePosition)
                    {
                        defineTypePosition = pxCompiler->SymbolEntryCurrent.Source;
                    }
#if 1

                    libraryNameLength += pxCompiler->SymbolEntryCurrent.Size;

                    //PXConsoleWrite(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);
#endif



                    //  PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                    PXCompilerSymbolEntryExtract(pxCompiler);
                }

                const PXBool isEnd = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerBracketAngleClose;

                if (isEnd)
                {
#if PXCDebugOutput
                    //printf("\n");
#endif // 0
                    break;
                }

                const PXBool isSlash = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerSlash || pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerSlashBack;

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
                        "C",
                        "Parsing",
                        "Invalid include"
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
                "C",
                "Parsing",
                "Include library (global) : %s",
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



            const PXBool isExpectedText = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

            if (!isExpectedText)
            {
#if PXCDebugOutput
                PXLogPrint
                (
                    PXLoggingError,
                    "C",
                    "Include",
                    "Makro include has invalid name"
                );
#endif
            }

            defineTypePosition = (char*)0;// PXFileCursorPosition(pxFile);

            // PXFileWriteI8U(outputStream, PXCLibraryPathTypeGlobal);
            // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


#if PXCDebugOutput
            char includeName[64];

            PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, includeName, 64);

            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Include",
                "Makro define name : %s",
                includeName
            );
#endif

            pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;
            pxCodeDocumentElement->IsGlobal = PXFalse;

            PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

            break;
        }

        default:
        {
            break;
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParsePreprocessorPragma(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXBool isExpectedText = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

    if (!isExpectedText)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingInfo,
            "C",
            "Parsing",
            "Makro pragma has invalid name"
        );
#endif
    }

#if PXCDebugOutput
    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "Makro pragma ..."
    );
   // PXConsoleWrite(pxCompilerSymbolEntry.Source, pxCompilerSymbolEntry.Size);
#endif

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParseTypeDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeInvalid, pxCompiler->Depth, &pxCodeDocumentElement, parrent);

    pxCodeDocumentElement->IsTypeDefinition = PXTrue;

    if(pxCompiler->CommentData)
    {
        pxCodeDocumentElement->CommentAdress = pxCompiler->CommentData;
        pxCodeDocumentElement->CommentSize = pxCompiler->CommentSize;

        pxCompiler->CommentLine = 0;
        pxCompiler->CommentData = 0;
        pxCompiler->CommentSize = 0;
    }

    PXCompilerSymbolEntryExtract(pxCompiler);

    const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "type definition"
    );
#endif

    switch (keyWord)
    {
        case CKeyWordStruct:
        {
            pxCodeDocumentElement->Type = PXDocumentElementTypeStruct;
            PXCParseTypeContainer(pxCompiler, pxCodeDocumentElement);
            break;
        }
        case CKeyWordUnion:
        {
            pxCodeDocumentElement->Type = PXDocumentElementTypeUnion;
            PXCParseTypeEnum(pxCompiler, pxCodeDocumentElement);
            break;
        }
        case CKeyWordEnum:
        {
            pxCodeDocumentElement->Type = PXDocumentElementTypeEnum;
            PXCParseTypeContainer(pxCompiler, pxCodeDocumentElement);
            break;
        }
        default:
        case CKeyWordUnkown:
        {
            // unkown Return type

            PXCompilerSymbolEntryPeek(pxCompiler);

            // Parse possible attributes

            // Is function pointer?
            const PXBool isOpenBreaked = PXCompilerSymbolLexerBrackedRoundOpen == pxCompiler->SymbolEntryCurrent.ID;

            if (isOpenBreaked)
            {            
                PXCompilerSymbolEntryExtract(pxCompiler);
                PXCompilerSymbolEntryPeek(pxCompiler);

                // check calling convention
                {
                    const PXBool isText = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;
                 
                    if (isText)
                    {
                        PXCompilerSymbolEntryExtract(pxCompiler);
                        PXCompilerSymbolEntryPeek(pxCompiler);
                    }             
                }

                PXBool isAsterics = PXCompilerSymbolLexerAsterisk == pxCompiler->SymbolEntryCurrent.ID;

                if (isAsterics)
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // remove '*'
                    PXCompilerSymbolEntryExtract(pxCompiler); // Get new

                    pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
                    pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;
#if 0
                    char buffer[64];

                    PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 64);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "C",
                        "Parsing",
                        "typedef Funtion : %s",
                        buffer
                    );
#endif


                    PXCompilerSymbolEntryExtract(pxCompiler);
     
                    const PXBool isAsterics = PXCompilerSymbolLexerBrackedRoundClose == pxCompiler->SymbolEntryCurrent.ID;


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

PXActionResult PXAPI PXCParseEndOfCommand(PXCompiler* const pxCompiler)
{
    const PXBool isSemicolon = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerSemiColon);

    if (!isSemicolon)
    {
        return PXActionRefusedSymbolInvalid;
    }

    PXCompilerSymbolEntryForward(pxCompiler);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParseTypeParameterList(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
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

        switch (pxCompiler->SymbolEntryCurrent.ID)
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

PXActionResult PXAPI PXCParseFunctionDefinition(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeFunction, pxCompiler->Depth, &pxCodeDocumentElement, parrent);

    PXCompilerSymbolEntryExtract(pxCompiler);

    // Return type
    const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);
    const PXBool isBuildIn = CKeyWordUnkown != keyWord;

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
        PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, bufferReturnName, 64);
    }

    // check calling convention
    PXCompilerSymbolEntryExtract(pxCompiler);


    // Check name
    PXCompilerSymbolEntryExtract(pxCompiler);

    {
        pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
        pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;

        PXCNameCleave(pxCompiler, pxCodeDocumentElement);

        PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, bufferFunctionName, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            "C",
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

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParseTypeDeclarationElement(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parent)
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

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, pxDocumentElementType, pxCompiler->Depth, &pxCodeDocumentElement, parent);

    // check if const
    {
        PXBool done = 0;

        do
        {
            PXCompilerSymbolEntryPeek(pxCompiler);

            if(pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerComment)
            {
                pxCodeDocumentElement->CommentAdress = pxCompiler->SymbolEntryCurrent.Source;
                pxCodeDocumentElement->CommentSize = pxCompiler->SymbolEntryCurrent.Size;

                PXCompilerSymbolEntryForward(pxCompiler);
                PXCompilerSymbolEntryPeek(pxCompiler);
            }

            const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);

            switch (keyWord)
            {

                case CKeyWordChar:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "char"

                    pxCodeDocumentElement->DataType |= PXDataTypeInt08U;

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    done = 1;

                    break;
                }
                case CKeyWordShort:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "short"

                    pxCodeDocumentElement->DataType |= PXDataTypeInt16U;

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    done = 1;

                    break;
                }
                case CKeyWordInt:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataType |= PXDataTypeInt32U;

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    done = 1;

                    break;
                }
                case CKeyWordLong:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                   // pxCo
                   // deDocumentElement->DataType |= PXDataTypeFloat;

                    done = 1;

                    break;
                }
                case  CKeyWordFloat:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataType |= PXDataTypeFloat;

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    done = 1;

                    break;
                }
                case  CKeyWordDouble:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataType |= PXDataTypeDouble;

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    done = 1;

                    break;
                }
                case  CKeyWordBool:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataTypeIsBuildIn = PXTrue;

                    pxCodeDocumentElement->DataType |= PXDataTypeInt08U;

                    done = 1;

                    break;
                }

                case CKeyWordConst:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    pxCodeDocumentElement->DataType |= PXDataTypeReadOnly;

                    break;
                }
                case CKeyWordUnsigned:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "unsigned"

                    pxCodeDocumentElement->DataType |= PXDataTypeUnsigned;

                    break;
                }

                case CKeyWordUnion: // might be a union inside a struct
                {
                    PXCodeDocumentElement pxCodeDocumentElementChild;
                    PXClear(PXCodeDocumentElement, &pxCodeDocumentElementChild);
                    pxCodeDocumentElementChild.Type = PXDocumentElementTypeUnion;
                    pxCodeDocumentElementChild.ElementParent = pxCodeDocumentElement;

                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    PXCParseTypeContainer(pxCompiler, &pxCodeDocumentElementChild);

                    //PXCompilerSymbolEntryExtract(pxCompiler); // consume ";" 

                    done = 1;

                   // break;

                    return PXActionSuccessful;

                   
                }
                case PXCompilerSymbolLexerAsterisk:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);

                    pxCodeDocumentElement->DataType |= PXDataTypeAdressMask;

                    break;
                }
                default:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler);

                    const PXBool isTypeName = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

                    pxCodeDocumentElement->TypeNameAdress = pxCompiler->SymbolEntryCurrent.Source;
                    pxCodeDocumentElement->TypeNameSize = pxCompiler->SymbolEntryCurrent.Size;

                    done = 1;
                    break;
                }             
            }
        }
        while (!done);         
    }

    // Get data entry
    //PXCompilerSymbolEntryExtract(pxCompiler);
    //PXBool aa = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

    PXBool isPointer = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerAsterisk);

    if (isPointer)
    {
        pxCodeDocumentElement->DataType |= PXDataTypeAdressMask;

        // Throw away '*'
        PXCompilerSymbolEntryExtract(pxCompiler);

        // Get new element
        PXCompilerSymbolEntryPeek(pxCompiler);

        const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);
        const PXBool isConst = CKeyWordConst == keyWord;

        if (isConst)
        {
            // Throw away 'const'
            PXCompilerSymbolEntryExtract(pxCompiler);
        }
    }

    // Name

    PXCompilerSymbolEntryExtract(pxCompiler);
    const PXBool isName = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;


    pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
    pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;



    
    const PXBool isArray = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerBracketSquareOpen);

    if(isArray)
    {
        PXCompilerSymbolEntryForward(pxCompiler); // Consume 

        const PXBool isInt = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerInteger);

        if(!isInt)
        {
            return PXActionInvalid;
        }

        PXCompilerSymbolEntryForward(pxCompiler); // Consume 

        pxCodeDocumentElement->ArrayAmount = pxCompiler->SymbolEntryCurrent.DataI32U;

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
                    const PXSize currentLine = pxCompiler->SymbolEntryCurrent.Line;

                    PXCompilerSymbolEntryForward(pxCompiler);

                    // Check for comment, often we put these at the left on the member

                    PXCompilerSymbolEntryPeek(pxCompiler);

                    const PXBool hasCommentAttached =
                        (PXCompilerSymbolLexerComment == pxCompiler->SymbolEntryCurrent.ID) &&
                        (pxCompiler->SymbolEntryCurrent.Line == currentLine);

                    if(hasCommentAttached)
                    {
                        PXCompilerSymbolEntryExtract(pxCompiler); // Consume comment

                        pxCodeDocumentElement->CommentAdress = pxCompiler->SymbolEntryCurrent.Source;
                        pxCodeDocumentElement->CommentSize = pxCompiler->SymbolEntryCurrent.Size;
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
        "C",
        "Parsing",
        "Member : %s",
        nameBuffer
    );
#endif

    PXCodeDocumentElementAdd(pxCompiler->CodeDocument, pxCodeDocumentElement);

    return PXActionInvalid;
}

void PXAPI PXCNameCleave(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement)
{
    char* name = pxCodeDocumentElement->AliasAdress ? pxCodeDocumentElement->AliasAdress : pxCodeDocumentElement->NameAdress;
    PXSize nameSize = pxCodeDocumentElement->AliasSize ? pxCodeDocumentElement->AliasSize : pxCodeDocumentElement->NameSize;

    if(pxCodeDocumentElement->ElementParent)
    {
        PXCodeDocumentElement* parent = pxCodeDocumentElement->ElementParent;      

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

PXActionResult PXAPI PXCParseEnumMember(PXCompiler* const pxCompiler, PXCodeDocumentElement* const parrent)
{
    PXCodeDocumentElement* pxCodeDocumentElement = PXNull;

    PXCodeDocumentElementGenerateChild(pxCompiler->CodeDocument, PXDocumentElementTypeEnumMember, pxCompiler->Depth, &pxCodeDocumentElement, parrent);

    const PXBool isValidText = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric);

    if(!isValidText)
    {
        return PXActionSuccessful;
    }

    PXCompilerSymbolEntryExtract(pxCompiler);

    pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
    pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;
        
    PXCNameCleave(pxCompiler, pxCodeDocumentElement);

    // TODO: what if an enum has a predefined value!?
    PXCompilerSymbolEntryPeek(pxCompiler);

    if(pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerComment)
    {
        pxCodeDocumentElement->CommentAdress = pxCompiler->SymbolEntryCurrent.Source;
        pxCodeDocumentElement->CommentSize = pxCompiler->SymbolEntryCurrent.Size;

        PXCompilerSymbolEntryExtract(pxCompiler);
        PXCompilerSymbolEntryPeek(pxCompiler);
    }

    switch(pxCompiler->SymbolEntryCurrent.ID)
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParseTypeContainer(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElement)
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
            return PXActionSuccessful;
        }    

        const PXBool isName = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

        if(isName)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);

            pxCodeDocumentElement->NameAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement->NameSize = pxCompiler->SymbolEntryCurrent.Size;

            PXCNameCleave(pxCompiler, pxCodeDocumentElement);

#if PXLogEnable
            char buffer[64];

            PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 64);

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
            ++pxCompiler->Depth;

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

            --pxCompiler->Depth;
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
            pxCodeDocumentElement->AliasAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement->AliasSize = pxCompiler->SymbolEntryCurrent.Size;

            PXCNameCleave(pxCompiler, pxCodeDocumentElement);

#if PXLogEnable
            char buffer[64];

            PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 64);


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

    for(PXCodeDocumentElement* i = pxCodeDocumentElement->ElementChildFirstBorn; i ; i = i->ElementSibling)
    {
        PXCNameCleave(pxCompiler, i);
        PXCodeDocumentElementAdd(pxCompiler->CodeDocument, i);
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCParseTypeEnum(PXCompiler* const pxCompiler, PXCodeDocumentElement* const pxCodeDocumentElementParent)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    char buffer[64];

    PXTextCopyA(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size, buffer, 64);

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

PXActionResult PXAPI PXCLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
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

    PXCodeDocument* pxDocument = (PXCodeDocument*)pxResourceLoadInfo->Target;
    PXClear(PXCodeDocument, pxDocument);

    PXFile tokenSteam;
    PXClear(PXFile, &tokenSteam);

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.FileInput = pxResourceLoadInfo->FileReference;
    pxCompiler.FileCache = &tokenSteam;

    PXCodeDocumentElement* pxCodeDocumentElementRoot = PXNull;

    char mainNodeName[64];

    //-----------------------------------------------------
    // Lexer - Level I
    //-----------------------------------------------------
    {
        PXCompilerSettings compilerSettings;
        PXClear(PXCompilerSettings, &compilerSettings);

        compilerSettings.TryAnalyseTypes = PXYes;
        compilerSettings.CommentsKeep = PXYes;
        compilerSettings.CommentSingleLineSize = 2;
        compilerSettings.CommentSingleLine = "//";
        compilerSettings.CommentMultibleLineBegin = "/*";
        compilerSettings.CommentMultibleLineBeginSize = 2;
        compilerSettings.CommentMultibleLineEnd = "*/";
        compilerSettings.CommentMultibleLineEndSize = 2;

        PXCompilerLexicalAnalysis(&pxCompiler, &compilerSettings);
    }



    pxCompiler.CodeDocument = pxDocument;
    pxCompiler.TokenStream = &tokenSteam;

    PXCodeDocumentElementGenerateChild(pxDocument, PXDocumentElementTypeFile, 0, &pxCodeDocumentElementRoot, PXNull);


    {
        PXText filePath;
        PXTextConstructBufferA(&filePath, PXPathSizeMax);

        PXFilePathGet(pxResourceLoadInfo->FileReference, &filePath);

        PXFilePathStructure pxFilePathStructure;

        PXFilePathSplitt(&filePath, &pxFilePathStructure);

        PXNewList(char, pxFilePathStructure.FileName.SizeUsed, &pxCodeDocumentElementRoot->NameAdress, &pxCodeDocumentElementRoot->NameSize);
        PXTextCopyA(pxFilePathStructure.FileName.TextA, pxFilePathStructure.FileName.SizeUsed, pxCodeDocumentElementRoot->NameAdress, pxCodeDocumentElementRoot->NameSize);

        PXCNameCleave(&pxCompiler, pxCodeDocumentElementRoot);

        PXCodeDocumentElementAdd(pxCompiler.CodeDocument, pxCodeDocumentElementRoot);
    }

    ++pxCompiler.Depth;

    // filename
    //pxCodeDocumentElementRoot->NameAdress = 


    //-----------------------------------------------------
    // Precompiler
    //-----------------------------------------------------



    //-----------------------------------------------------
    // Compile 
    //-----------------------------------------------------

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntryExtract(&pxCompiler);

        switch (pxCompiler.SymbolEntryCurrent.ID)
        {
            case PXCompilerSymbolLexerComment:
            {
                // Do nothing
                pxCompiler.CommentLine = pxCompiler.SymbolEntryCurrent.Line;
                pxCompiler.CommentData = pxCompiler.SymbolEntryCurrent.Source;
                pxCompiler.CommentSize = pxCompiler.SymbolEntryCurrent.Size;
                break;
            }
            case PXCompilerSymbolLexerHash: // Probably a preprocessor definition
            {
                PXCompilerSymbolEntryExtract(&pxCompiler); // move from '#' to next one

                const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler.SymbolEntryCurrent.Source, pxCompiler.SymbolEntryCurrent.Size);

                switch (keyWord)
                {
                    case CKeyWordDefine:
                    {
                        PXCParsePreprocessorDefine(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordIfDefined:
                    {
                        PXCParsePreprocessorCondition(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordIfNotDefined:
                    {
                        PXCParsePreprocessorCondition(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordInclude:
                    {
                        PXCParsePreprocessorInclude(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordPragma:
                    {
                        PXCParsePreprocessorPragma(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordDefinitionEnd:
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
                const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler.SymbolEntryCurrent.Source, pxCompiler.SymbolEntryCurrent.Size);

                switch (keyWord)
                {
                    case CKeyWordTypeDefinition:
                    {
                        PXCParseTypeDefinition(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }             
                    case CKeyWordEnum:
                    {                      
                        // && pxCodeDocumentElement.Type = PXDocumentElementTypeEnum;

                       // PXCParseTypeEnum(&pxCompiler, &pxCodeDocumentElement);
                        break;
                    }
                    case CKeyWordUnion:
                    {
                       // pxCodeDocumentElement.Type = PXDocumentElementTypeUnion;

                       // PXCParseTypeContainer(&pxCompiler, &pxCodeDocumentElement);
                        break;
                    }
                    case CKeyWordStruct:
                    {
                        // pxCodeDocumentElement.Type = PXDocumentElementTypeStruct;

                       // PXCParseTypeContainer(&pxCompiler, &pxCodeDocumentElement);
                        break;
                    }
                    case CKeyWordExtern:
                    {
                        PXCompilerSymbolEntryExtract(&pxCompiler);

                        switch (pxCompiler.SymbolEntryCurrent.ID)
                        {
                            case PXCompilerSymbolLexerString: // Need to me the extern "C" thing
                            {
                                const PXBool isNameMangeling = (pxCompiler.SymbolEntryCurrent.Source[0] == 'C') && (pxCompiler.SymbolEntryCurrent.Size == 1);

                                if (isNameMangeling)
                                {
                                    // Invalid extern spesifieer
                                }

                                PXCompilerSymbolEntryPeek(&pxCompiler);

                                const PXBool isWrapping = PXCompilerSymbolLexerBracketCurlyOpen == pxCompiler.SymbolEntryCurrent.ID;

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
                    case CKeyWordVoid:
                    {
                        PXCParseFunctionDefinition(&pxCompiler, pxCodeDocumentElementRoot);
                        break;
                    }
                    case CKeyWordUnkown: // Might declaration of variable or function.
                    {
                        // as we have an unkown type, lets look it up.

                        // There is no result, so we take this as an extra unrsolved paramater

                        const PXSize index = PXTextFindFirstCharacterBeforeA(pxCompiler.SymbolEntryCurrent.Source, 260, '(', ';');
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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}

void PXAPI PXCElementExtract(PXFile* const inputStream, PXCElement* const pxCElement)
{
    PXCElementClear(pxCElement);

    {
        PXInt8U keyID = 0;

        PXFileReadI8U(inputStream, &keyID);

        pxCElement->Type = (PXCStructureType)keyID;
    }

    // Fake name
    PXFileReadI8U(inputStream, &pxCElement->NameSizeCurrent);
    pxCElement->Name = (char*)PXFileCursorPosition(inputStream);
    PXFileCursorAdvance(inputStream, pxCElement->NameSizeCurrent);

    switch (pxCElement->Type)
    {
        case PXCStructureTypeMakroFlag:
        {


            break;
        }
        case PXCStructureTypeMakroValue:
        {


            break;
        }
        case PXCStructureTypeMakroInlcude:
        {
            PXCLibraryPathType pxCLibraryPathType = PXCLibraryPathTypeInvalid;

            {
                PXInt8U inlcudeType = 0;

                PXFileReadI8U(inputStream, &inlcudeType);

                inlcudeType = (PXCLibraryPathType)inlcudeType;
            }

            break;
        }
        case PXCStructureTypeEnum:
        case PXCStructureTypeStruct:
        case PXCStructureTypeUnion:
        {
            // Alias
            PXFileReadI8U(inputStream, &pxCElement->ElementStructure.NameAliasSizeCurrent);
            pxCElement->ElementStructure.NameAlias = (char*)PXFileCursorPosition(inputStream);
            PXFileCursorAdvance(inputStream, pxCElement->ElementStructure.NameAliasSizeCurrent);

            PXFileReadI16U(inputStream, &pxCElement->ElementStructure.MemberAmount);

            PXFileReadI8U(inputStream, &pxCElement->IsTypeDefinition);

            break;
        }
        case PXCStructureTypeStructElement:
        {
            PXInt8U variableInfoFlags = 0;

            PXFileReadI8U(inputStream, &variableInfoFlags); // Write flags
            PXFileReadI16U(inputStream, &pxCElement->ElementVariable.SizeOfType); // Write size of Type

            pxCElement->ElementVariable.IsKnownPrimitve = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsKnownPrimitive);
            pxCElement->ElementVariable.IsConstantType = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsConstType);
            pxCElement->ElementVariable.IsInRegister = PXFlagIsSet(variableInfoFlags, MemberFieldFlagRegister);
            pxCElement->ElementVariable.IsSigned = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsSigned);
            pxCElement->ElementVariable.IsAdress = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsAdress);
            pxCElement->ElementVariable.IsAdressConstant = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsAdressConst);
            pxCElement->ElementVariable.IsAdressVolitile = PXFlagIsSet(variableInfoFlags, MemberFieldFlagVolatile);
            pxCElement->ElementVariable.IsAdressRestricted = PXFlagIsSet(variableInfoFlags, MemberFieldFlagResticted);

            if (pxCElement->ElementVariable.IsKnownPrimitve)
            {
                // Get Type
                PXInt8U keyID = 0;

                PXFileReadI8U(inputStream, &keyID);

                pxCElement->ElementVariable.PrimitiveType = (CKeyWord)keyID;
            }
            else
            {
                PXFileReadI8U(inputStream, &pxCElement->ElementVariable.NameOfTypeSizeCurrent);
                pxCElement->ElementVariable.NameOfType = (char*)PXFileCursorPosition(inputStream);
                PXFileCursorAdvance(inputStream, pxCElement->ElementVariable.NameOfTypeSizeCurrent);
            }

            break;
        }
        case PXCStructureTypeFuntion:
        {
            PXCFunction* const pxCFunction = &pxCElement->ElementFunction;

            pxCFunction->AccessModifier = PXCAccessModifierPublic;
            pxCFunction->CallingConvention = PXCCallingConventionCDeclaration;

            PXFileReadI8U(inputStream, &pxCFunction->ParameterListSize);

            break;
        }
        default:
            break;
    }
}
