#include "PXC.h"

#include <Compiler\PXCompiler.h>
#include <Container/ClusterValue.h>
#include <Text/PXText.h>

#include <Log/PXLog.h>

CKeyWord PXCFileAnalyseElement(const char* name, const PXSize nameSize)
{
    switch (nameSize)
    {
        case 2:
        {
            const PXInt16U key = PXInt16FromAdress(name);

            switch (key)
            {
                case MakeShort('i', 'f'):
                    return CKeyWordIf;

                case MakeShort('d', 'o'):
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
/*
PXBool PXCFileParseEnum(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXCompilerSymbolEntry compilerSymbolEntry;

    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

    const PXBool isExpectedName = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry.ID;

    if (isExpectedName)
    {
#if 1
        char buffer[64];
        PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

        printf
        (
            "|+------+-------------------------------------------------+|\n"
            "|| Enum | %-47s ||\n"
            "|+------+-------------------------------------------------+|\n",
            buffer
        );
#endif

        PXDataStreamWriteI8U(outputStream, 'N');
        PXDataStreamWriteI8U(outputStream, compilerSymbolEntry.Size);
        PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);
    }

    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);
    const PXBool isExpectedBrace = PXCompilerSymbolLexerBracketCurlyOpen == compilerSymbolEntry.ID;

    if (!isExpectedBrace)
    {
        // Expected '{' but did not found.
    }

    PXBool finishedWithGrace = 0;

    while (1)
    {
        PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

        while (PXCompilerSymbolLexerComment == compilerSymbolEntry.ID)
        {
            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);
        }

        const PXBool isFinished = PXCompilerSymbolLexerBracketCurlyClose == compilerSymbolEntry.ID;

        if (isFinished)
        {
#if 1
            printf("|+------+-------------------------------------------------+|\n");
#endif
            finishedWithGrace = 1;
            break;
        }



    }

    if (!finishedWithGrace)
    {
        // Error, we seem to not have a valid ending
        return PXFalse;
    }


    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

    switch (compilerSymbolEntry.ID)
    {
        case PXCompilerSymbolLexerGenericElement:
        {
            PXDataStreamWriteI8U(outputStream, 'T');
            PXDataStreamWriteI8U(outputStream, compilerSymbolEntry.Size);
            PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


#if 1
            char buffer[64];
            PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

            printf
            (
                "+----------------------------------------------------------+\n"
                "| Enum alias : %-30s |\n"
                "+----------------------------------------------------------+\n",
                buffer
            );
#endif

            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

            const PXBool su = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry.ID;

            if (!su)
            {
                // Error, expected ; But got something else.
                return PXFalse;
            }

            return PXTrue; // OK
        }
        case PXCompilerSymbolLexerSemiColon:
        {
            // No Name
            return PXTrue;
        }
        default:
            return PXFalse;
    }
}*/

PXBool PXCFileParseTypedef(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXCompilerSymbolEntry compilerSymbolEntry;

#if 1
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
                    return PXCFileParseStructure(inputStream, outputStream, typeKey);
            }
            break;
        }
    }

    return PXFalse;
}

PXBool PXCFileParseStructure(PXDataStream* const inputStream, PXDataStream* const outputStream, const CKeyWord structureType)
{
    PXCompilerSymbolEntry compilerSymbolEntry;

    PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry); // Fetch element after 'union' keyword

    switch (compilerSymbolEntry.ID)
    {
        case PXCompilerSymbolLexerGenericElement: // We have a 'union' name
        {
#if 1
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

            PXDataStreamWriteI8U(outputStream, 'N');
            PXDataStreamWriteI8U(outputStream, compilerSymbolEntry.Size);
            PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);

            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

            const PXBool isPrototype = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry.ID;

            if (isPrototype)
            {
                return PXTrue; // finished
            }

            const PXBool isOpenBrace = PXCompilerSymbolLexerBracketCurlyOpen == compilerSymbolEntry.ID; // Do we have '{'

            if (!isOpenBrace)
            {
                return PXFalse; // We need a '{', but got something else
            }

            break;
        }

        case PXCompilerSymbolLexerBracketCurlyOpen: // We dont have a name
        {

#if 1
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
            PXCompilerSymbolEntryExtract(inputStream, &compilerSymbolEntry);

            const PXBool isExpectedComma = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry.ID;

            if (isExpectedComma) // Get alias name
            {
#if 1
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
#if 1
                char buffer[64];
                PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                printf("| - %-54s |\n", buffer);
#endif 


                PXDataStreamWriteI8U(outputStream, compilerSymbolEntry.Size);
                PXDataStreamWriteB(outputStream, compilerSymbolEntry.Source, compilerSymbolEntry.Size);


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
                PXBool result = PXCFileParseDeclaration(inputStream, outputStream, &compilerSymbolEntry);
                break;
            }

            default:
                break;
        }
    }

    return PXFalse;
}

#define PXFlagIsSet(value, flag)  ((value & flag) && 1u)

PXBool PXCFileParseDeclaration(PXDataStream* const inputStream, PXDataStream* const outputStream, PXCompilerSymbolEntry* compilerSymbolEntry)
{
    PXBool finished = 0;
    PXInt8U flagList = MemberFieldFlagIsSigned;

    const PXBool isTypeName = PXCompilerSymbolLexerGenericElement == compilerSymbolEntry->ID;

    if (!isTypeName)
    {
        printf("type expected but got something else\n");
    }

    // Try fetch type
    const CKeyWord fieldType = PXCFileAnalyseElement(compilerSymbolEntry->Source, compilerSymbolEntry->Size);

    const PXBool isStructure =
        CKeyWordEnum == fieldType ||
        CKeyWordUnion == fieldType ||
        CKeyWordStruct == fieldType;

    if (isStructure)
    {
        PXCFileParseStructure(inputStream, outputStream, fieldType);
        return PXTrue;
    }

#if 1
    char type[64];
    char name[64];

    PXTextCopyA("N/A", 3, type, 64);
    PXTextCopyA("N/A", 3, name, 64);
#endif

    PXBool typeKnown = PXFalse;

    do
    {
        const PXBool isFinished = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry->ID;

        if (isFinished)
        {

#if 1
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
                        PXCFileParseStructure(inputStream, outputStream, typeKey);
                        break;

                    case CKeyWordChar:
                    case CKeyWordShort:
                    case CKeyWordInt:
                    case CKeyWordLong:
                    case CKeyWordFloat:
                    case CKeyWordDouble:
                    case CKeyWordBool:
                    {
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
                        const PXBool isKnownPrimitive = flagList & MemberFieldFlagIsKnownPrimitive || !typeKnown;

                        if (isKnownPrimitive)
                        {
                            typeKnown = PXTrue;

                            // This is our Name
                            PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, name, 64);
                        }
                        else
                        {
                            // Must be the a yet unkown custom type
                            PXTextCopyA(compilerSymbolEntry->Source, compilerSymbolEntry->Size, type, 64);
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

PXActionResult PXCFileLexicalAnalysis(PXDataStream* const inputStream, PXDataStream* const outputStream)
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

PXActionResult PXCFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
    PXSize errorCounter = 0;
    PXDataStream tokenSteam;

    // Lexer - Level I
    {
        PXCFileLexicalAnalysis(inputStream, outputStream);

        PXDataStreamFromExternal(&tokenSteam, outputStream->Data, outputStream->DataCursor);

        outputStream->DataCursor = 0;
    }

    while (!PXDataStreamIsAtEnd(&tokenSteam))
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
                        // Fetch name [Required]
                        {
                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                            const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                            if (!isExpectedText)
                            {
                                printf("Makro define has invalid name\n");
                            }

#if 1

                            printf("Makro define : ");
                            PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif
                        }

                        // Fetch [Optional]
                        {
                            //PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry);

                            //const PXBool isFlagDefine = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;



                            const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;
                        }

                        break;
                    }
                    case CKeyWordIfNotDefined:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                        if (!isExpectedText)
                        {
                            printf("Makro 'if not defined' has invalid name\n");
                        }

#if 1

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
                            printf("Makro 'if defined' has invalid name\n");
                        }

#if 1

                        printf("Is makro if defined? : ");
                        PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                        break;
                    }
                    case CKeyWordInclude:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        // case A) #include <xxxxxxx>
                        const PXBool isTypeA = compilerSymbolEntry.ID == PXCompilerSymbolLexerBracketAngleOpen;

                        if (isTypeA)
                        {
                            printf("Makro include libary : (global) ");


                            while (1)
                            {
                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                                const PXBool isString = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                                if (isString)
                                {
                                    PXLogPrintString(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

                                    PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);
                                }



                                const PXBool isEnd = compilerSymbolEntry.ID == PXCompilerSymbolLexerBracketAngleClose;

                                if (isEnd)
                                {
                                    printf("\n");
                                    break;
                                }

                                const PXBool isSlash = compilerSymbolEntry.ID == PXCompilerSymbolLexerSlash || compilerSymbolEntry.ID == PXCompilerSymbolLexerSlashBack;

                                if (isSlash)
                                {
                                    printf("/");
                                }
                                else
                                {
                                    printf("Failure, include wrong");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            // Case B) #include "xxxxxxxx"

                            const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                            if (!isExpectedText)
                            {
                                printf("Makro include has invalid name\n");
                            }

#if 1

                            printf("Makro include : ");
                            PXLogPrintStringLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif
                        }

                        break;
                    }
                    case CKeyWordPragma:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        const PXBool isExpectedText = compilerSymbolEntry.ID == PXCompilerSymbolLexerGenericElement;

                        if (!isExpectedText)
                        {
                            printf("Makro pragma has invalid name\n");
                        }

#if 1

                        printf("Makro pragma : ");
                        PXLogPrintString(compilerSymbolEntry.Source, compilerSymbolEntry.Size);
#endif

                        break;
                    }
                    case CKeyWordDefinitionEnd:
                    {
                        printf("End definition\n");

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
                        PXCFileParseStructure(&tokenSteam, outputStream, keyWord);
                        break;
                    }
                    case CKeyWordExtern:
                    {
                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

                        switch (compilerSymbolEntry.ID)
                        {
                            case PXCompilerSymbolLexerString: // Need to me the extern "C" thing
                            {
                                // extern "C"
                                printf("[C] Disable name mangeling for C++\n");
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

#if 1
                        char buffer[64];

                        PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                        printf("| [Function]\n");
                        printf("| Element    : %s |\n", buffer);

#endif // 0


                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

#if 1

                        PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                        printf("| Return Type : %s |\n", buffer);

#endif // 0


                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);


#if 1

                        PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                        printf("| FuntionName : %s |\n", buffer);

#endif // 0

                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Expect '('

                        const PXBool isFunction = PXCompilerSymbolLexerBrackedRoundOpen == compilerSymbolEntry.ID;

                        if (!isFunction)
                        {
                            printf("Expected funtion but didnt\n");
                        }

                        PXBool parameterDone = PXFalse;

                        while (1u)
                        {


                            PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // get next thing

                            const PXBool isEndOfParameterList = PXCompilerSymbolLexerBrackedRoundClose == compilerSymbolEntry.ID;

                            if (isEndOfParameterList)
                            {
                                break;
                            }

#if 1



                            PXTextCopyA(compilerSymbolEntry.Source, compilerSymbolEntry.Size, buffer, 64);

                            printf("|     Parameter : %s |\n", buffer);

#endif // 0





                            PXCompilerSymbolEntryPeek(&tokenSteam, &compilerSymbolEntry); // Soft Expect ','

                            const PXBool isColon = PXCompilerSymbolLexerComma == compilerSymbolEntry.ID;

                            if (isColon)
                            {
                                PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Remove the ','
                                // new variab

                                //printf("Missing semicolon");
                            }
                        }


                        PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry); // Soft Expect ';'

                        const PXBool isComma = PXCompilerSymbolLexerSemiColon == compilerSymbolEntry.ID;

                        if (!isComma)
                        {
                            printf("Colon missing\n");

                        }

                        printf("---------------------\n");

                        break;
                    }

                    break;
                }

                break;
            }
            default:
            {
                printf("[C] Invalid code on this posision\n");
                break;
            }
        }
    }
}