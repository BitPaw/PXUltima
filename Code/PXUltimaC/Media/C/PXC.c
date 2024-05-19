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

PXBool PXAPI PXCFileParseTypedef(PXFile* const inputStream, PXFile* const outputStream)
{
    PXCompilerSymbolEntry compilerSymbolEntry;

#if PXCDebugOutput
    printf
    (
        "|+--------------------------------------------------------+|\n"
        "|| Typedefinition                                         ||\n"
        "|+--------------------------------------------------------+|\n"
    );
#endif

    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Peek for next element

    switch (compilerSymbolEntry.ID)
    {
        case PXCompilerSymbolLexerGeneric:
        {
            const CKeyWord typeKey = PXCFileAnalyseElement(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

            switch (typeKey)
            {
                case CKeyWordEnum:
                case CKeyWordStruct:
                case CKeyWordUnion:
                    return PXCFileParseStructure(inputStream, outputStream, typeKey, PXTrue);
            }
            break;
        }
    }

    return PXFalse;
}

PXBool PXAPI PXCFileParseStructure(PXFile* const inputStream, PXFile* const outputStream, const CKeyWord structureType, const PXBool isTypeDefitinition)
{
    PXCompilerSymbolEntry compilerSymbolEntry;

    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Fetch element after 'union' keyword

    switch (structureType)
    {
        case CKeyWordEnum:
            PXFileWriteI8U(outputStream, PXCStructureTypeEnum);
            break;
        case CKeyWordUnion:
            PXFileWriteI8U(outputStream, PXCStructureTypeUnion);
            break;
        case CKeyWordStruct:
            PXFileWriteI8U(outputStream, PXCStructureTypeStruct);
            break;
    }

    switch (compilerSymbolEntry.ID)
    {
        case PXCompilerSymbolLexerGeneric: // We have a 'union' name
        {
#if PXCDebugOutput
            char buffer[64];
            PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

            char name[64];


            switch (structureType)
            {
                case CKeyWordEnum:
                {
                    PXTextCopyA("Enum", 4, name, 64);
                    break;
                }
                case CKeyWordUnion:
                {
                    PXTextCopyA("Union", 5, name, 64);
                    break;
                }
                case CKeyWordStruct:
                {
                    PXTextCopyA("Struct", 6, name, 64);
                    break;
                }

                default:
                    break;
            }

            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parse",
                "Object detected\n"
                "|+--------+-----------------------------------------------+|\n"
                "|| %-6s | %-45s ||\n"
                "|+--------+-----------------------------------------------+|\n",
                name,
                buffer
            );

#endif

            //---<Write name of structure>---
            PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
            PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
            //-------------------------------

            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Fetch next element after "name"

            switch (compilerSymbolEntry.ID)
            {
                case PXCompilerSymbolLexerSemiColon:
                {
                    return PXTrue; // finished, format is now "struct NAME;"
                }
                case PXCompilerSymbolLexerBracketCurlyOpen:
                {
                    // OK, proceed. format is now "struct NAME { ..."
                    break;
                }
                case PXCompilerSymbolLexerGeneric:
                {
                    // Format  "struct NAME ALIASNAME;"
                    // We have a direct alias

                    PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                    PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


                    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Element after alias

                    const PXBool isSemiColon = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry.ID; //

                    if (!isSemiColon)
                    {
                        // Error expected ;
                        return PXFalse;
                    }

                    return PXTrue;
                }
                default:
                {
                    return PXFalse;
                }
            }

            break;
        }
        case PXCompilerSymbolLexerBracketCurlyOpen: // We dont have a name
        {
            PXFileWriteI8U(outputStream, 0); // Name size, we dont have one so its always 0

#if PXCDebugOutput
            char buffer[64];
            PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

            PXLogPrint
            (
                PXLoggingInfo,
                "C",
                "Parse",
                "Alias detected\n"
                "|+--------+-----------------------------------------------+|\n"
                "|| Alias: %-45s ||\n"
                "|+--------+-----------------------------------------------+|\n",
                "**Unnamed**"
            );
#endif

            // OK
            break;
        }

        default:
            return PXFalse; // Illegal formating
    }


    // We are now here
    // "struct name { | ... };"


    // Problem: We need the alias name NOW. And the content of {...} is in the way.
    // Solution: We do a very far peek and try to check if one is readable.
    {
    PXSize range = PXFileRemainingSizeRelativeFromAddress(inputStream, compilerSymbolEntry.Source);
    char* start = compilerSymbolEntry.Source + 1u;

    PXBool foundInsert = PXFalse;

    while (1)
    {
        const PXSize index = PXTextFindFirstCharacterA(start, range, '}');
        const PXBool found = index != (PXSize)-1;

        if (!found)
        {
            // Error
        }

        const PXSize amountOfOpen = PXTextCountA(start, index, '{');

        start += index + 1u;
        range -= index + 1u;

        if (amountOfOpen == 0)
        {
            // We have found what we need
            foundInsert = PXTrue;
            break;
        }

        // We have one open more than a close, so we look for more
    }

    if (foundInsert)
    {
        // We should be where the } is

        const PXSize semicolonINdex = PXTextFindFirstCharacterA(start, range, ';');

        if (semicolonINdex > 1)
        {
            // We have found an alias
            char* name = start;
            PXSize range = semicolonINdex;

            // Remove empty space
            while (IsEmptySpace(name[0]) || IsEndOfLineCharacter(name[0]) || IsTab(name[0]))
            {
                ++name;
                --range;
            }


            PXFileWriteI8U(outputStream, range);
            PXFileWriteB(outputStream, name, range);

        }
        else
        {
            // No name
            PXFileWriteI8U(outputStream, 0);

        }
    }
    else
    {
        PXFileWriteI8U(outputStream, 0);
    }
    }






    PXInt16U memberCounter = 0;
    PXSize poition = outputStream->DataCursor;

    PXFileWriteI16U(outputStream, 0xFFFF);
    PXFileWriteI8U(outputStream, isTypeDefitinition);


    while (1)
    {
        PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Peek for possible '}'

        while (PXCompilerSymbolLexerComment == compilerSymbolEntry.ID)
        {
            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Fetch next
        }

        const PXBool isAtEnd = PXCompilerSymbolLexerBracketCurlyClose == compilerSymbolEntry.ID; // Do we have a '}'

        if (isAtEnd) // If so, we quit
        {
            PXFileWriteAtI16U(outputStream, memberCounter, poition); // Write hoe many elements we've written


            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

            const PXBool isExpectedComma = PXCompilerSymbolLexerGeneric == compilerSymbolEntry.ID;

            if (isExpectedComma) // Get alias name
            {
#if PXCDebugOutput
                char buffer[64];
                PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "C",
                    "Parse",
                    "Alias detected\n"
                    "|+--------+-----------------------------------------------+|\n"
                    "|| Alias: %-45s ||\n"
                    "|+--------+-----------------------------------------------+|\n",
                    buffer
                );
#endif

                // Don't write alias, we hacked that before.
                // PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                // PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


                PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Go to next
            }



            // Expect ';'
            {
                const PXBool isExpectedComma = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry.ID;

                if (!isExpectedComma)
                {
                    PXLogPrint
                    (
                        PXLoggingError,
                        "C",
                        "Parse",
                        "Missing ';' in structure"
                    );
                }

                return isExpectedComma;
            }
        }

        switch (structureType)
        {
            case CKeyWordEnum:
            {
#if PXCDebugOutput
                char buffer[64];
                PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "C",
                    "Parse-Enum",
                    "| - %-54s |\n",
                    buffer
                );
#endif

                ++memberCounter;

                PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


                // Look forward, we expect a ','
                {
                    PXCompilerSymbolEntryPeek(inputStream, &compilerSymbolEntry);

                    const PXBool isExpectedComma = PXCompilerSymbolLexerComma == compilerSymbolEntry.ID;

                    if (isExpectedComma)
                    {
                        PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Fetch next

                        //printf("Parsing error, enum value has no ending comma.\n");
                    }
                }

                break;
            }

            case CKeyWordUnion:
            case CKeyWordStruct:
            {
                // If we dont end, we must have fields to parse.
                const PXBool result = PXCFileParseDeclaration(inputStream, outputStream, &compilerSymbolEntry);

                if (result)
                {
                    ++memberCounter;
                }

                break;
            }

            default:
                break;
        }
    }

    return PXFalse;
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
        PXCFileParseStructure(inputStream, outputStream, fieldType, PXFalse);
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
                        PXCFileParseTypedef(inputStream, outputStream);
                        break;
                    }
                    case CKeyWordUnion:
                    case CKeyWordEnum:
                    case CKeyWordStruct:
                        PXCFileParseStructure(inputStream, outputStream, typeKey, PXFalse);
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






        PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

        if (compilerSymbolEntry->ID == PXCompilerSymbolLexerComment)
        {
            PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);
        }
    } while (1);

    // printf("type name expected but got something else\n");

        // printf("';' expected but got something else\n");

}

PXBool PXAPI PXCFileParseFunctionPrototype(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{


    PXFileWriteI8U(outputStream, PXCStructureTypeFuntion);

    // Name

    // Amount



#if PXCDebugOutput
    char buffer[64]; // calling convention?

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    PXConsoleWrite("| [Function]\n", 0);
    PXConsoleWrite("| Element    : %s |\n", buffer);

#endif // 0


    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

#if PXCDebugOutput

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    PXConsoleWrite("| Return Type : %s |\n", buffer);

#endif // 0


    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

    PXFileWriteI8U(outputStream, compilerSymbolEntry->Size);
    PXFileWriteB(outputStream, compilerSymbolEntry->Source, compilerSymbolEntry->Size);

#if PXCDebugOutput

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    PXConsoleWrite("| FuntionName : %s |\n", buffer);

#endif // 0

    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry); // Expect '('

    const PXBool isFunction = PXCompilerSymbolLexerBrackedRoundOpen == compilerSymbolEntry->ID;

    if (!isFunction)
    {
#if PXCDebugOutput
        PXConsoleWrite("Expected funtion but didnt\n", 0);
#endif
    }


    PXSize parameterListLengthDataPosition = outputStream->DataCursor;
    PXInt8U parameterListLength = 0;

    PXFileWriteI8U(outputStream, 0xFF); // How many parameters?



    PXBool parameterDone = PXFalse;

    while (1u)
    {
        PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry); // get next thing

        const PXBool isEndOfParameterList = PXCompilerSymbolLexerBrackedRoundClose == compilerSymbolEntry->ID;

        if (isEndOfParameterList)
        {
            PXFileWriteAtI8U(outputStream, parameterListLength, parameterListLength);

            break;
        }

        ++parameterListLength;


        // Get type


        // Get name
        PXFileWriteI8U(outputStream, compilerSymbolEntry->Size);
        PXFileWriteB(outputStream, compilerSymbolEntry->Source, compilerSymbolEntry->Size);

#if PXCDebugOutput

        PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

        PXConsoleWrite("|     Parameter : %s |\n", buffer);

#endif // 0





        PXCompilerSymbolEntryPeek(inputStream, compilerSymbolEntry); // Soft Expect ','

        const PXBool isColon = PXCompilerSymbolLexerComma == compilerSymbolEntry->ID;

        if (isColon)
        {
            PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry); // Remove the ','
            // new variab

            //printf("Missing semicolon");
        }
    }


    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry); // Soft Expect ';'

    const PXBool isComma = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry->ID;

    if (!isComma)
    {
#if PXCDebugOutput
        PXLogPrint
        (
            PXLoggingError,
            "C",
            "Parser",
            "Colon missing"
        );
#endif

    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParsePreprocessorCondition(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
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

PXActionResult PXAPI PXCParsePreprocessorDefine(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    const PXSize defineTypePosition = pxFile->DataCursor;

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

PXActionResult PXAPI PXCParsePreprocessorInclude(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    PXCodeDocumentElement pxCodeDocumentElement;
    PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
    pxCodeDocumentElement.Type = PXDocumentElementTypeInclude;


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

            pxCodeDocumentElement.NameAdress = defineTypePosition;
            pxCodeDocumentElement.NameSize = libraryNameLength;
            pxCodeDocumentElement.IsGlobal = PXTrue;

            PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);


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

            defineTypePosition = (char*)PXFileCursorPosition(pxFile);

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

            pxCodeDocumentElement.NameAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement.NameSize = pxCompiler->SymbolEntryCurrent.Size;
            pxCodeDocumentElement.IsGlobal = PXFalse;

            PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);

            break;
        }

        default:
        {
            break;
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParsePreprocessorPragma(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    PXCompilerSymbolEntryExtract(pxCompiler);

    const PXBool isExpectedText = pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerGeneric;

    if (!isExpectedText)
    {
#if PXCDebugOutput
        PXConsoleWrite("Makro pragma has invalid name", 0);
#endif
    }

#if PXCDebugOutput

    PXConsoleWrite("Makro pragma : ", 0);
   // PXConsoleWrite(pxCompilerSymbolEntry.Source, pxCompilerSymbolEntry.Size);
#endif

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParseTypeDefinition(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    PXCodeDocumentElement pxCodeDocumentElement;
    PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
    pxCodeDocumentElement.IsTypeDefinition = PXTrue;

    PXCompilerSymbolEntryExtract(pxCompiler);

    const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);

    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "type definition"
    );

    switch (keyWord)
    {
        case CKeyWordStruct:
        {
            pxCodeDocumentElement.Type = PXDocumentElementTypeStruct;
            PXCParseTypeContainer(&pxCodeDocumentElement, pxCompiler, pxDocument, pxFile);
            break;
        }
        case CKeyWordUnion:
        {
            pxCodeDocumentElement.Type = PXDocumentElementTypeUnion;
            PXCParseTypeEnum(&pxCodeDocumentElement, pxDocument, pxFile);
            break;
        }
        case CKeyWordEnum:
        {
            pxCodeDocumentElement.Type = PXDocumentElementTypeEnum;
            PXCParseTypeContainer(&pxCodeDocumentElement, pxCompiler, pxDocument, pxFile);
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


                    PXCompilerSymbolEntryExtract(pxCompiler);
     
                    const PXBool isAsterics = PXCompilerSymbolLexerBrackedRoundClose == pxCompiler->SymbolEntryCurrent.ID;

                    PXCParseTypeParameterList(pxCompiler, pxDocument, pxFile);

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

PXActionResult PXAPI PXCParseTypeParameterList(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
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
                PXCodeDocumentElement pxCodeDocumentElement;
                PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
                pxCodeDocumentElement.Type = PXDocumentElementTypeFunctionParameter;

                PXCParseTypeDeclarationElement(&pxCodeDocumentElement, pxCompiler, pxDocument, pxFile);

                PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);

                break;
            }
        }
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCParseFunctionDefinition(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    PXCodeDocumentElement pxCodeDocumentElement;
    PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
    pxCodeDocumentElement.Type = PXDocumentElementTypeFunction;

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
        pxCodeDocumentElement.NameAdress = pxCompiler->SymbolEntryCurrent.Source;
        pxCodeDocumentElement.NameSize = pxCompiler->SymbolEntryCurrent.Size;

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

    PXCParseTypeParameterList(pxCompiler, pxDocument, pxFile);    

    PXCParseEndOfCommand(pxCompiler);


    // ALL OK, update entry
    PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);

    return PXActionInvalid;
}

PXActionResult PXAPI PXCParseTypeDeclarationElement(PXCodeDocumentElement* const pxCodeDocumentElement, PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    // check if const
    {
        PXBool done = 0;

        do
        {
            PXCompilerSymbolEntryPeek(pxCompiler);

            const CKeyWord keyWord = PXCFileAnalyseElement(pxCompiler->SymbolEntryCurrent.Source, pxCompiler->SymbolEntryCurrent.Size);

            switch (keyWord)
            {
                case CKeyWordConst:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"
                    break;
                }
                case CKeyWordUnsigned:
                {
                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "unsigned"
                    break;
                }

                case CKeyWordUnion: // might be a union inside a struct
                {
                    PXCodeDocumentElement pxCodeDocumentElementChild;
                    PXClear(PXCodeDocumentElement, &pxCodeDocumentElementChild);
                    pxCodeDocumentElementChild.Type = PXDocumentElementTypeUnion;
                    pxCodeDocumentElementChild.ElementParent = pxCodeDocumentElement;

                    PXCompilerSymbolEntryExtract(pxCompiler); // consume "const"

                    PXCParseTypeContainer(&pxCodeDocumentElementChild, pxCompiler, pxDocument, pxFile);

                    //PXCompilerSymbolEntryExtract(pxCompiler); // consume ";" 

                    done = 1;

                   // break;

                    return PXActionSuccessful;

                   
                }
                default:
                    done = 1;
                    break;
            }
        }
        while (!done);         
    }

    // Get data entry
    PXCompilerSymbolEntryExtract(pxCompiler);
    PXBool aa = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

    PXBool isPointer = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerAsterisk);

    if (isPointer)
    {
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



#if PXLogEnable
    char nameBuffer[64];
    PXTextCopyA(pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize, nameBuffer, 64);

    PXLogPrint
    (
        PXLoggingInfo,
        "C",
        "Parsing",
        "Member : %s, ArraySize:%i",
        nameBuffer,
        pxCodeDocumentElement->ArrayAmount
    );
#endif



    return PXActionInvalid;
}

PXActionResult PXAPI PXCParseTypeDeclarationFull(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{   
    PXCodeDocumentElement pxCodeDocumentElement;
    PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
    pxCodeDocumentElement.Type = PXDocumentElementTypeClassMember;
    
    PXCParseTypeDeclarationElement(&pxCodeDocumentElement, pxCompiler, pxDocument, pxFile);

    // ';'   
    const PXBool isSemiColon = PXCompilerSymbolEntryPeekCheck(pxCompiler, PXCompilerSymbolLexerSemiColon);

    if (isSemiColon)
    {
        const PXSize currentLine = pxCompiler->SymbolEntryCurrent.Line;

        PXCompilerSymbolEntryExtract(pxCompiler);


        // Check for comment, often we put these at the left on the member

        PXCompilerSymbolEntryPeek(pxCompiler);

        const PXBool hasCommentAttached =
            (PXCompilerSymbolLexerComment == pxCompiler->SymbolEntryCurrent.ID) &&
            (pxCompiler->SymbolEntryCurrent.Line == currentLine);

        if(hasCommentAttached)
        {
            PXCompilerSymbolEntryExtract(pxCompiler); // Consume comment

            pxCodeDocumentElement.CommentAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement.CommentAdress = pxCompiler->SymbolEntryCurrent.Size;
        }
    }

    PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCParseEnumList(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
{
    PXCodeDocumentElement pxCodeDocumentElement;
    
  //  for(;;)
    {
        PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
        pxCodeDocumentElement.Type = PXDocumentElementTypeEnumMember;

        const PXBool isValidText = PXCompilerSymbolEntryPeekEnsure(pxCompiler, PXCompilerSymbolLexerGeneric);

        if(!isValidText)
        {
            return PXActionSuccessful;
        }

        PXCompilerSymbolEntryExtract(pxCompiler);

        pxCodeDocumentElement.NameAdress = pxCompiler->SymbolEntryCurrent.Source;
        pxCodeDocumentElement.NameSize = pxCompiler->SymbolEntryCurrent.Size;

        // TODO: what if an enum has a predefined value!?
        PXCompilerSymbolEntryPeek(pxCompiler);

        if(pxCompiler->SymbolEntryCurrent.ID == PXCompilerSymbolLexerComment)
        {
            pxCodeDocumentElement.CommentAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement.CommentSize = pxCompiler->SymbolEntryCurrent.Size;

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
    }

    PXDocumentElementAttributeAdd(pxDocument, &pxCodeDocumentElement);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCParseTypeContainer(PXCodeDocumentElement* const pxCodeDocumentElement, PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
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


    // expect '{' 
    {
        PXCompilerSymbolEntryExtract(pxCompiler);

        const PXBool isCurlyBrackedOpen = PXCompilerSymbolLexerBracketCurlyOpen == pxCompiler->SymbolEntryCurrent.ID;

        if (isCurlyBrackedOpen)
        {
            PXBool isCurlyBrackedClose = 0; 

            for(;;)
            {
                switch(pxCodeDocumentElement->Type)
                {
                    case PXDocumentElementTypeStruct:
                    case PXDocumentElementTypeUnion:
                    case PXDocumentElementTypeClass:
                    {
                         PXCParseTypeDeclarationFull(pxCompiler, pxDocument, pxFile);

                         break;
                    }
                    case PXDocumentElementTypeEnum:
                    {
                        PXCParseEnumList(pxCompiler, pxDocument, pxFile);
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
        }
        else
        {
            pxCodeDocumentElement->IsPreDefine = PXTrue;
        }
    }
   





    // Alias [optional]
    {
        PXCompilerSymbolEntryPeek(pxCompiler);

        const PXBool isText = PXCompilerSymbolLexerGeneric == pxCompiler->SymbolEntryCurrent.ID;

        if (isText)
        {
            PXCompilerSymbolEntryExtract(pxCompiler);

            // Alias
            pxCodeDocumentElement->AliasAdress = pxCompiler->SymbolEntryCurrent.Source;
            pxCodeDocumentElement->AliasSize = pxCompiler->SymbolEntryCurrent.Size;

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

    PXDocumentElementAttributeAdd(pxDocument, pxCodeDocumentElement);

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCParseTypeEnum(PXCompiler* const pxCompiler, PXCodeDocument* const pxDocument, PXFile* const pxFile)
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
                        PXCParsePreprocessorDefine(&pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordIfDefined:
                    {
                        PXCParsePreprocessorCondition(&pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordIfNotDefined:
                    {
                        PXCParsePreprocessorCondition(&pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordInclude:
                    {
                        PXCParsePreprocessorInclude(&pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordPragma:
                    {
                        PXCParsePreprocessorPragma(&pxCompiler, pxDocument, &tokenSteam);
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
                        PXCParseTypeDefinition(&pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }             
                    case CKeyWordEnum:
                    {
                        PXCodeDocumentElement pxCodeDocumentElement;
                        PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
                        pxCodeDocumentElement.Type = PXDocumentElementTypeEnum;

                        PXCParseTypeEnum(&pxCodeDocumentElement, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordUnion:
                    {
                        PXCodeDocumentElement pxCodeDocumentElement;
                        PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
                        pxCodeDocumentElement.Type = PXDocumentElementTypeUnion;

                        PXCParseTypeContainer(&pxCodeDocumentElement, &pxCompiler, pxDocument, &tokenSteam);
                        break;
                    }
                    case CKeyWordStruct:
                    {
                        PXCodeDocumentElement pxCodeDocumentElement;
                        PXClear(PXCodeDocumentElement, &pxCodeDocumentElement);
                        pxCodeDocumentElement.Type = PXDocumentElementTypeStruct;

                        PXCParseTypeContainer(&pxCodeDocumentElement, &pxCompiler, pxDocument, &tokenSteam);
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
                        PXCParseFunctionDefinition(&pxCompiler, pxDocument, &tokenSteam);
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
                            PXCParseFunctionDefinition(&pxCompiler, pxDocument, &tokenSteam);
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
