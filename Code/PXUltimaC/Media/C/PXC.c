#include "PXC.h"

#include <Compiler/PXCompiler.h>
#include <Media/PXText.h>

#include <OS/Console/PXConsole.h>

#define PXCDebugOutput 0

PXBool PXCElementHasName(PXCElement* const pxCElement)
{
    const PXBool hasName = pxCElement->Name && pxCElement->NameSizeCurrent;
   // const PXBool hasAlias = pxCElement->NameAlias && pxCStructure->NameAliasSizeCurrent;

    return hasName;
}

void PXCElementClear(PXCElement* const pxCElement)
{
    char* name = pxCElement->Name;
    PXInt8U nameSize = pxCElement->NameSizeMaximal;

    char* nameSecound = pxCElement->ElementStructure.NameAlias;
    PXInt8U nameSizeSecound = pxCElement->ElementStructure.NameAliasSizeMaximal;

    PXMemoryClear(name, nameSize);
    PXMemoryClear(nameSecound, nameSizeSecound);

    PXMemoryClear(pxCElement, sizeof(PXCElement));

    pxCElement->Name = name;
    pxCElement->NameSizeMaximal = nameSize;

    pxCElement->ElementStructure.NameAlias = nameSecound;
    pxCElement->ElementStructure.NameAliasSizeMaximal = nameSizeSecound;
}

CKeyWord PXCFileAnalyseElement(const char* name, const PXSize nameSize)
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

PXBool PXCFileParseTypedef(PXFile* const inputStream, PXFile* const outputStream)
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
        case PXCompilerSymbolLexerGenericElement:
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

PXBool PXCFileParseStructure(PXFile* const inputStream, PXFile* const outputStream, const CKeyWord structureType, const PXBool isTypeDefitinition)
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
        case PXCompilerSymbolLexerGenericElement: // We have a 'union' name
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

            printf
            (
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
                case PXCompilerSymbolLexerGenericElement:
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

            printf
            (
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

            const PXBool isExpectedComma = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry.ID;

            if (isExpectedComma) // Get alias name
            {
#if PXCDebugOutput
                char buffer[64];
                PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                printf
                (
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
                    printf("Missing ';' in structure\n");
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

                printf("| - %-54s |\n", buffer);
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

PXBool PXCFileParseDeclaration(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{
    PXBool finished = 0;
    PXInt8U flagList = MemberFieldFlagIsSigned;
    PXInt16U sizeOfType = 0;

    const PXBool isTypeName = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry->ID;

    if (!isTypeName)
    {
#if PXCDebugOutput
        printf("type expected but got something else\n");
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
            printf("| MemberField: %s Type, Name %-20s |\n", type, name);
            printf("| - %-17s : %-20s |\n", "Const", PXFlagIsSet(flagList, MemberFieldFlagIsConstType) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Singed", PXFlagIsSet(flagList, MemberFieldFlagIsSigned) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Adress", PXFlagIsSet(flagList, MemberFieldFlagIsAdress) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Readonly Adress", PXFlagIsSet(flagList, MemberFieldFlagIsAdressConst) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Volatile", PXFlagIsSet(flagList, MemberFieldFlagVolatile) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Register", PXFlagIsSet(flagList, MemberFieldFlagRegister) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Resticted", PXFlagIsSet(flagList, MemberFieldFlagResticted) ? "Yes" : "No");
            printf("| - %-17s : %-20s |\n", "Primitive", PXFlagIsSet(flagList, MemberFieldFlagIsKnownPrimitive) ? "Yes" : "No");

#endif // 1



            return PXTrue;
        }

        switch (compilerSymbolEntry->ID)
        {
            case PXCompilerSymbolLexerGenericElement: // Is element
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

PXBool PXCFileParseFunctionPrototype(PXFile* const inputStream, PXFile* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{


    PXFileWriteI8U(outputStream, PXCStructureTypeFuntion);

    // Name

    // Amount



#if PXCDebugOutput
    char buffer[64]; // calling convention?

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    printf("| [Function]\n");
    printf("| Element    : %s |\n", buffer);

#endif // 0


    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

#if PXCDebugOutput

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    printf("| Return Type : %s |\n", buffer);

#endif // 0


    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry);

    PXFileWriteI8U(outputStream, compilerSymbolEntry->Size);
    PXFileWriteB(outputStream, compilerSymbolEntry->Source, compilerSymbolEntry->Size);

#if PXCDebugOutput

    PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, buffer, 64);

    printf("| FuntionName : %s |\n", buffer);

#endif // 0

    PXCompilerSymbolEntryExtract(inputStream, compilerSymbolEntry); // Expect '('

    const PXBool isFunction = PXCompilerSymbolLexerBrackedRoundOpen == compilerSymbolEntry->ID;

    if (!isFunction)
    {
#if PXCDebugOutput
        printf("Expected funtion but didnt\n");
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

        printf("|     Parameter : %s |\n", buffer);

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
        printf("Colon missing\n");
#endif

    }

    return PXActionSuccessful;
}

PXActionResult PXCFileLexicalAnalysis(PXFile* const inputStream, PXFile* const outputStream)
{
    PXCompilerSettings compilerSettings;

    PXCompilerSettingsConstruct(&compilerSettings);

    compilerSettings.TryAnalyseTypes = PXYes;
    compilerSettings.CommentsKeep = PXYes;
    compilerSettings.CommentSingleLineSize = 2;
    compilerSettings.CommentSingleLine = "//";
    compilerSettings.CommentMultibleLineBegin = "/*";
    compilerSettings.CommentMultibleLineBeginSize = 2;
    compilerSettings.CommentMultibleLineEnd = "*/";
    compilerSettings.CommentMultibleLineEndSize = 2;

    PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings);

    return PXActionSuccessful;
}

PXActionResult PXCFileCompile(PXFile* const inputStream, PXFile* const outputStream)
{
    PXSize errorCounter = 0;
    PXFile tokenSteam;

    // Lexer - Level I
    {
        PXCFileLexicalAnalysis(inputStream, outputStream);

        PXFileBufferExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    while (!PXFileIsAtEnd(&tokenSteam))
    {
        PXCompilerSymbolEntry compilerSymbolEntry;

        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

        switch (compilerSymbolEntry.ID)
        {
            case PXCompilerSymbolLexerHash: // Probably a preprocessor definition
            {
                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // move from '#' to next one

                const CKeyWord keyWord = PXCFileAnalyseElement(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (keyWord)
                {
                    case CKeyWordDefine:
                    {
                        const PXSize defineTypePosition = outputStream->DataCursor;

                        PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

                        // Fetch name [Required]
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                            const PXBool isExpectedText = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry.ID;

                            if (!isExpectedText)
                            {
#if PXCDebugOutput
                                printf("Makro define has invalid name\n");
#endif
                            }

                            // Write define name
                            PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                            PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

#if PXCDebugOutput
                            printf("[C] Makro define name : ");
                            PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif
                        }


                        PXInt16U currentLineA = compilerSymbolEntry.Line;

                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry);

                        PXInt16U currentLineB = compilerSymbolEntry.Line;

                        PXBool isNewLine = currentLineA != currentLineB;

                        if (isNewLine)
                        {
                            // Is Makro-Flag

                            PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFlag, defineTypePosition);
                        }
                        else
                        {
                            switch (compilerSymbolEntry.ID)
                            {
                                case PXCompilerSymbolLexerBrackedRoundOpen:
                                {
                                    // Is Makro-Function

                                    const PXSize position = outputStream->DataCursor;
                                    PXInt8U parameterListLength = 0;

                                    PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroFunction, defineTypePosition);
                                    PXFileWriteI8U(outputStream, 0xFF);

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Remove the '('

                                    while (1)
                                    {
                                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                                        const PXBool isClosingBreaked = PXCompilerSymbolLexerBrackedRoundClose == compilerSymbolEntry.ID;

                                        if (isClosingBreaked)
                                        {
                                            break;
                                        }

                                        ++parameterListLength;

                                        PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                                        PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                        PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry);

                                        switch (compilerSymbolEntry.ID)
                                        {
                                            case PXCompilerSymbolLexerComma:
                                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Extract comma
                                                break;
                                            case PXCompilerSymbolLexerBrackedRoundClose:
                                                // done, dont do anything
                                                break;

                                            default: // Error
                                                break;
                                        }
                                    }

                                    PXFileWriteAtI8U(outputStream, parameterListLength, position);

                                    break;
                                }
                                default:
                                {
                                    // Is makro-Value

                                    PXFileWriteAtI8U(outputStream, PXCStructureTypeMakroValue, defineTypePosition);
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                                    PXFileWriteI8U(outputStream, compilerSymbolEntry.Size);
                                    PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                    break;
                                }
                            }
                        }

                        break;
                    }
                    case CKeyWordIfNotDefined:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                        if (!isExpectedText)
                        {
#if PXCDebugOutput
                            printf("Makro 'if not defined' has invalid name\n");
#endif
                        }

#if PXCDebugOutput

                        printf("Is makro not defined? : ");
                        PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                        break;
                    }
                    case CKeyWordIfDefined:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                        if (!isExpectedText)
                        {
#if PXCDebugOutput
                            printf("Makro 'if defined' has invalid name\n");
#endif
                        }

#if PXCDebugOutput

                        printf("Is makro if defined? : ");
                        PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                        break;
                    }
                    case CKeyWordInclude:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXSize defineTypePosition = outputStream->DataCursor;

                        PXFileWriteI8U(outputStream, 0xFF); // Write dummy define type

                        switch (compilerSymbolEntry.ID)
                        {
                            case PXCompilerSymbolLexerBracketAngleOpen:
                            {
                                // case A) #include <xxxxxxx>

#if PXCDebugOutput
                                printf("[C] Makro include libary : (global) ");
#endif

                                PXSize stringLength = 0;
                                PXSize stringLengthPosition = 0;

                                stringLengthPosition = outputStream->DataCursor;

                                PXFileWriteI8U(outputStream, 0xFF);

                                while (1)
                                {
                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                                    const PXBool isString = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                                    if (isString)
                                    {
#if 1
                                        PXLogPrintString(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                                        stringLength += compilerSymbolEntry.Size;

                                        PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                    }

                                    const PXBool isEnd = compilerSymbolEntry.ID == PXCompilerSymbolLexerBracketAngleClose;

                                    if (isEnd)
                                    {
#if PXCDebugOutput
                                        printf("\n");
#endif // 0
                                        break;
                                    }

                                    const PXBool isSlash = compilerSymbolEntry.ID == PXCompilerSymbolLexerSlash || compilerSymbolEntry.ID == PXCompilerSymbolLexerSlashBack;

                                    if (isSlash)
                                    {
                                        stringLength += 1;

                                        PXFileWriteB(outputStream, "/", 1u);

#if PXCDebugOutput
                                        printf("/");
#endif // 0
                                    }
                                    else
                                    {
#if PXCDebugOutput
                                        printf("Failure, include wrong");
#endif
                                        break;
                                    }
                                }

                                PXFileWriteAtI8U(outputStream, stringLength, stringLengthPosition);
                                PXFileWriteI8U(outputStream, PXCLibraryPathTypeGlobal);

                                break;
                            }
                            case PXCompilerSymbolLexerString:
                            {
                                // Case B) #include "xxxxxxxx"

                                const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                                if (!isExpectedText)
                                {
#if PXCDebugOutput
                                    printf("Makro include has invalid name\n");
#endif
                                }


                                PXFileWriteI8U(outputStream, PXCLibraryPathTypeGlobal);
                                PXFileWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


#if PXCDebugOutput

                                printf("Makro include : ");
                                PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                                PXFileWriteI8U(outputStream, PXCLibraryPathTypeLocale);

                                break;
                            }

                            default:
                            {
                                break;
                            }
                        }

                        break;
                    }
                    case CKeyWordPragma:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                        if (!isExpectedText)
                        {
#if PXCDebugOutput
                            printf("Makro pragma has invalid name\n");
#endif
                        }

#if PXCDebugOutput

                        printf("Makro pragma : ");
                        PXLogPrintString(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                        break;
                    }
                    case CKeyWordDefinitionEnd:
                    {
#if PXCDebugOutput
                        printf("End definition\n");
#endif

                        // OK?
                        break;
                    }

                    default:
                        break;
                }

                break;
            }
            case PXCompilerSymbolLexerGenericElement:
            {
                const CKeyWord keyWord = PXCFileAnalyseElement(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                switch (keyWord)
                {
                    case CKeyWordTypeDefinition:
                    {
                        PXCFileParseTypedef(&tokenSteam, outputStream);
                        break;
                    }
                    case CKeyWordUnion:
                    case CKeyWordEnum:
                    case CKeyWordStruct:
                    {
                        PXCFileParseStructure(&tokenSteam, outputStream, keyWord, PXFalse);
                        break;
                    }
                    case CKeyWordExtern:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        switch (compilerSymbolEntry.ID)
                        {
                            case PXCompilerSymbolLexerString: // Need to me the extern "C" thing
                            {
                                const PXBool isNameMangeling = (compilerSymbolEntry.Source[0] == 'C') && (compilerSymbolEntry.Size == 1);

                                if (isNameMangeling)
                                {
                                    // Invalid extern spesifieer
                                }

                                PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry);

                                const PXBool isWrapping = PXCompilerSymbolLexerBracketCurlyOpen == compilerSymbolEntry.ID;

                                if (isWrapping)
                                {
                                    // everything is C-Style now

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Consome '{'

#if PXCDebugOutput
                                    printf("[C] Disable name mangeling in C++ for following block\n");
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


                        break;
                    }
                    case CKeyWordUnkown: // Might declaration of variable or function.
                    {
                        // as we have an unkown type, lets look it up.

                        // There is no result, so we take this as an extra unrsolved paramater

                        const PXSize index = PXTextFindFirstCharacterBeforeA(compilerSymbolEntry.Source, 260, '(', ';');
                        const PXBool found = index != (PXSize)-1;

                        if (found)
                        {
                            // Probably a function

                            PXCFileParseFunctionPrototype(&tokenSteam, outputStream, &compilerSymbolEntry);
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
                printf("[C] Invalid code on this posision\n");
#endif
                break;
            }
        }
    }

    return PXActionSuccessful;
}

void PXCElementExtract(PXFile* const inputStream, PXCElement* const pxCElement)
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
