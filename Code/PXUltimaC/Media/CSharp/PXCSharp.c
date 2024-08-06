#include "PXCSharp.h"

#include <Media/C/PXC.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

#include <Compiler/PXCompiler.h>
#include <Media/PXDocument.h>


char PXCSharpKeyWordPublic[] = "public";
char PXCSharpKeyWordPrivate[] = "private";
char PXCSharpKeyWordStruct[] = "struct";
char PXCSharpKeyWordNamespace[] = "namespace";
char PXCSharpKeyWordClass[] = "class";
char PXCSharpKeyWordEnum[] = "enum";
char PXCSharpKeyWordVoid[] = "void";
char PXCSharpKeyWordExpliciLayout[] = "[StructLayout(LayoutKind.Explicit)]";

char PXCSharpFunctionCallCdecl[] = "Cdecl";
char PXCSharpFunctionCallStdCall[] = "StdCall";
char PXCSharpFunctionCallFastCall[] = "FastCall";
char PXCSharpFunctionCallThisCall[] = "ThisCall";

//char PXJavaKeyWordByte[] = "byte";
//char PXJavaKeyWordInt[] = "int";
//char PXJavaKeyWordShort[] = "short";
//char PXJavaKeyWordLong[] = "long";
//char PXJavaKeyWordFloat[] = "float";
//char PXJavaKeyWordDouble[] = "double";

/*

void PXCSSerialize(PXFile* const inputSteam, PXFile* const outputStream, PXCTranslateStruct* const pxCTranslateStruct)
{
    PXCElementExtract(inputSteam, &pxCTranslateStruct->Element);

    switch (pxCTranslateStruct->Element.Type)
    {
        case PXCStructureTypeEnum:
        {
            PXCStructure* const pxCStructure = &pxCTranslateStruct->Element.ElementStructure;

            PXFileWriteB(outputStream, "\tpublic enum ", 13);
            PXFileWriteB(outputStream, pxCStructure->NameAlias, pxCStructure->NameAliasSizeCurrent);
            PXFileWriteB(outputStream, "\n\t{\n", 4);

            for (PXSize i = 0; i < pxCStructure->MemberAmount; ++i)
            {
                PXInt8U enumElementBufferLength = 0;
                char enumElementBuffer[64];

                PXFileReadI8U(inputSteam, &enumElementBufferLength);
                PXFileReadB(inputSteam, enumElementBuffer, enumElementBufferLength);

                // if the name contains the same name as the enum, remove it

                PXBool minLength = PXMathMinimumIU(pxCStructure->NameAliasSizeCurrent, enumElementBufferLength);
                PXBool isSame = PXTextCompareA(pxCStructure->NameAlias, minLength, enumElementBuffer, minLength);
                PXSize offset = 0;

                if (isSame)
                {
                    offset += pxCStructure->NameAliasSizeCurrent;
                }

                PXFileWriteB(outputStream, "\t\t", 2);
                PXFileWriteB(outputStream, enumElementBuffer + offset, enumElementBufferLength - offset);

                if (i < (pxCStructure->MemberAmount - 1))
                {
                    PXFileWriteB(outputStream, ",\n", 2);
                }
                else
                {
                    PXFileWriteB(outputStream, "\n", 1);
                }
            }

            PXFileWriteB(outputStream, "\t}\n\n", 4);

            break;
        }
        case PXCStructureTypeStruct:
        case PXCStructureTypeUnion:
        {
            PXCStructure* const pxCStructure = &pxCTranslateStruct->Element.ElementStructure;

            const PXBool isUnnamed = !PXCElementHasName(&pxCTranslateStruct->Element);

            PXSize dataPointOffset = 0;

            PXBool isUnion = PXCStructureTypeUnion == pxCTranslateStruct->Element.Type;

            pxCTranslateStruct->AreInUnion = isUnion;

            if (isUnion)
            {
                pxCTranslateStruct->UnionStartOffset = pxCTranslateStruct->StructureOffsetTotal;
                pxCTranslateStruct->UnionWidthOffset = 0;
            }

            if (!isUnnamed)
            {
                pxCTranslateStruct->StructureOffsetTotal = 0;

                PXFileWriteB(outputStream, "\t[StructLayout(LayoutKind.Explicit)]\n", 37);
                PXFileWriteB(outputStream, "\tpublic unsafe struct ", 22);
                PXFileWriteB(outputStream, pxCStructure->NameAlias, pxCStructure->NameAliasSizeCurrent);
                PXFileWriteB(outputStream, "\n\t{\n", 4);
            }

            const PXSize amountOfMembers = pxCStructure->MemberAmount;
            for (PXSize i = 0; i < amountOfMembers; ++i)
            {
                PXCSSerialize(inputSteam, outputStream, pxCTranslateStruct);
            }

            if (isUnion)
            {
                pxCTranslateStruct->StructureOffsetTotal += pxCTranslateStruct->UnionWidthOffset;
            }

            pxCTranslateStruct->AreInUnion = PXCStructureTypeUnion == pxCTranslateStruct->Element.Type;
        
            if (!isUnnamed)
            {
                PXFileWriteB(outputStream, "\t}\n", 3);
            }


            break;
        }
        case PXCStructureTypeStructElement:
        {
            PXCStructureVariable* const pxCStructureVariable = &pxCTranslateStruct->Element.ElementVariable;

            PXText pxText;
            PXTextConstructNamedBufferA(&pxText, numberText, 64);

            pxText.SizeUsed += PXTextFromInt(&pxText, pxCTranslateStruct->StructureOffsetTotal);

            if (!pxCTranslateStruct->AreInUnion)
            {
                pxCTranslateStruct->StructureOffsetTotal += pxCTranslateStruct->Element.ElementVariable.SizeOfType;
            }
            else
            {
                pxCTranslateStruct->UnionWidthOffset = PXMathMaximum(pxCTranslateStruct->Element.ElementVariable.SizeOfType, pxCTranslateStruct->UnionWidthOffset);
            }

            PXFileWriteB(outputStream, "\t\t[FieldOffset(", 15);
            PXFileWriteB(outputStream, pxText.TextA, pxText.SizeUsed);
            PXFileWriteB(outputStream, ")] public ", 10);

            //pxCStructure->MemberOffsetCurrent += (typeSize * !(pxCStructure->Type == PXCStructureTypeUnion && PXCStructureHasName(pxCStructure)));


            //----------

            char volatileText[] = "volatile";
            char charText[] = "char";
            char byteText[] = "byte";
            char shortText[] = "short";
            char ushortText[] = "ushort";
            char intText[] = "int";
            char uintText[] = "uint";
            char longText[] = "long";
            char ulongText[] = "ulong";
            char floatText[] = "float";
            char doubleText[] = "double";
            char boolText[] = "bool";

            if (pxCStructureVariable->IsAdressVolitile)
            {
                PXFileWriteB(outputStream, volatileText, sizeof(volatileText) - 1);
            }

            PXInt8U sizeofKey = 0;
            const char* keyText = 0;

            if (pxCStructureVariable->IsKnownPrimitve)
            {
                if (!pxCStructureVariable->IsSigned)
                {
                    switch (pxCStructureVariable->PrimitiveType)
                    {
                        case CKeyWordChar:
                        {
                            keyText = byteText;
                            sizeofKey = sizeof(byteText) - 1;
                            break;
                        }
                        case CKeyWordShort:
                        {
                            keyText = ushortText;
                            sizeofKey = sizeof(ushortText) - 1;
                            break;
                        }
                        case CKeyWordInt:
                        {
                            keyText = uintText;
                            sizeofKey = sizeof(uintText) - 1;
                            break;
                        }
                        case CKeyWordLong:
                        {
                            keyText = ulongText;
                            sizeofKey = sizeof(ulongText) - 1;
                            break;
                        }
                    }
                }
                else
                {
                    switch (pxCStructureVariable->PrimitiveType)
                    {
                        case CKeyWordChar:
                        {
                            keyText = charText;
                            sizeofKey = sizeof(charText) - 1;
                            break;
                        }
                        case CKeyWordShort:
                        {
                            keyText = shortText;
                            sizeofKey = sizeof(shortText) - 1;
                            break;
                        }
                        case CKeyWordInt:
                        {
                            keyText = intText;
                            sizeofKey = sizeof(intText) - 1;
                            break;
                        }
                        case CKeyWordLong:
                        {
                            keyText = longText;
                            sizeofKey = sizeof(longText) - 1;
                            break;
                        }
                    }
                }

                switch (pxCStructureVariable->PrimitiveType)
                {
                    case CKeyWordFloat:
                    {
                        keyText = floatText;
                        sizeofKey = sizeof(floatText) - 1;
                        break;
                    }
                    case CKeyWordDouble:
                    {
                        keyText = doubleText;
                        sizeofKey = sizeof(doubleText) - 1;
                        break;
                    }
                    case CKeyWordBool:
                    {
                        keyText = boolText;
                        sizeofKey = sizeof(boolText) - 1;
                        break;
                    }
                }

                PXFileWriteB(outputStream, keyText, sizeofKey);

                if (pxCStructureVariable->IsAdress)
                {
                    PXFileWriteB(outputStream, "*", 1);
                }

                PXFileWriteB(outputStream, " ", 1);
            }
            else
            {
                // Primitiv
                PXFileWriteB(outputStream, pxCStructureVariable->NameOfType, pxCStructureVariable->NameOfTypeSizeCurrent);
                PXFileWriteB(outputStream, " ", 1);
            }
            //------------

            // Variable Name
            PXFileWriteB(outputStream, pxCTranslateStruct->Element.Name, pxCTranslateStruct->Element.NameSizeCurrent);
            PXFileWriteB(outputStream, ";\n", 2u);

            break;
        }
        case PXCStructureTypeFuntion:
        {
            PXCFunction* const pxCFunction = &pxCTranslateStruct->Element.ElementFunction;

            // Function

            PXFileWriteB(outputStream, "\t[DllImport(\"PXUltima.dll\")]\n\tprivate static unsafe extern \n", 60);
            PXFileWriteB(outputStream, pxCTranslateStruct->Element.Name, pxCTranslateStruct->Element.NameSizeCurrent);
            PXFileWriteB(outputStream, "(", 1);

            for (PXSize i = 0; i < pxCFunction->ParameterListSize; ++i)
            {
                
            }
        
            PXFileWriteB(outputStream, ");\n", 3);
            break;
        }
        default:
            break;
    }
}

void PXCSCreateWrapperFromCSource(PXFile* const inputSteam, PXFile* const outputStream)
{
    PXFileWriteB(outputStream, "using System;\n", 14);
    PXFileWriteB(outputStream, "using System.Runtime.InteropServices;\n\n", 39);

    PXFileWriteB(outputStream, "namespace PX.Wrapper\n{\n", 23);

    char name[64];
    char bufferName[64];

    PXCTranslateStruct pxCTranslateStruct;
    pxCTranslateStruct.Element.Name = name;
    pxCTranslateStruct.Element.NameSizeMaximal = 64;
    pxCTranslateStruct.Element.ElementStructure.NameAlias = bufferName;
    pxCTranslateStruct.Element.ElementStructure.NameAliasSizeMaximal = 64;

    pxCTranslateStruct.AreInUnion = 0;
    pxCTranslateStruct.StructureOffsetTotal = 0;
    pxCTranslateStruct.UnionWidthOffset = 0;
    pxCTranslateStruct.UnionStartOffset = 0;

    while (!PXFileIsAtEnd(inputSteam))
    {
        PXCSSerialize(inputSteam, outputStream, &pxCTranslateStruct);
    }

    PXFileWriteB(outputStream, "}", 1);

    outputStream->DataSize = outputStream->DataCursor;
    outputStream->DataCursor = 0;
}
*/


void PXAPI PXCSharpWriteFile(PXCompiler* const pxCompiler)
{
    PXCSharpContainerWrite(pxCompiler);
}

void PXAPI PXCSharpContainerWrite(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const symbol = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    if(symbol->Type != PXDocumentElementTypeFile)
    {
        PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * symbol->Depth);
        PXFileWriteA(pxFile, PXCSharpKeyWordPublic, sizeof(PXCSharpKeyWordPublic) - 1);
        PXFileWriteC(pxFile, ' ');
    }



    switch(symbol->Type)
    {
        case PXDocumentElementTypeEnum:
        {
            PXFileWriteA(pxFile, PXCSharpKeyWordEnum, sizeof(PXCSharpKeyWordEnum) - 1);
            break;
        }
        case PXDocumentElementTypeFile:
        {
            PXFileWriteA(pxFile, PXCSharpKeyWordNamespace, sizeof(PXCSharpKeyWordNamespace) - 1);
            break;
        }
        case PXDocumentElementTypeStruct:
        {
            PXFileWriteA(pxFile, PXCSharpKeyWordStruct, sizeof(PXCSharpKeyWordStruct) - 1);
            break;
        }
        case PXDocumentElementTypeClass:
        {
            PXFileWriteA(pxFile, PXCSharpKeyWordClass, sizeof(PXCSharpKeyWordClass) - 1);
            break;
        }

        default:
            break;
    }

    PXFileWriteC(pxFile, ' ');

    if(symbol->NameShortAdress)
    {
        PXFileWriteA(pxFile, symbol->NameShortAdress, symbol->NameShortSize);
    }
    else
    {
        PXFileWriteA(pxFile, symbol->NameAdress, symbol->NameSize);
    }

    PXFileWriteNewLine(pxFile);
    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * symbol->Depth);
    PXFileWriteC(pxFile, '{');
    PXFileWriteNewLine(pxFile);

    for
        (
        PXCodeDocumentElement* child = symbol->ElementChildFirstBorn;
        child;
        child = child->ElementSibling
        )
    {
        switch(symbol->Type)
        {
            case PXDocumentElementTypeEnum:
            {
                PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * child->Depth);
                PXFileWriteA(pxFile, child->NameShortAdress, child->NameShortSize);

                if(child->ElementSibling) // if not last
                {
                    PXFileWriteC(pxFile, ',');
                }

                break;
            }
            case PXDocumentElementTypeStruct:
            case PXDocumentElementTypeUnion:
            case PXDocumentElementTypeClass:
            {
                pxCompiler->WriteInfo.CodeElementCurrent = child;
                pxCompiler->WriteInfo.WriteDefinition(pxCompiler);
                PXFileWriteC(pxFile, ';');
                break;
            }
            default:
            {
                pxCompiler->WriteInfo.CodeElementCurrent = child;
                pxCompiler->WriteInfo.WriteNode(pxCompiler);
                break;
            }
        }

        if(child->ElementSibling)
        {
            PXFileWriteNewLine(pxFile);
        }
    }

    PXFileWriteNewLine(pxFile);
    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * symbol->Depth);
    PXFileWriteC(pxFile, '}');
    PXFileWriteNewLine(pxFile);
}

void PXAPI PXCSharpFunctionWrite(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const symbol = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * symbol->Depth);

    //PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
    PXFileWriteC(pxFile, ' ');

    if(1) // Is native?
    {
        PXFileWriteA(pxFile, "[DllImport(\"", 12);
        PXFileWriteA(pxFile, "PXUltima.dll", 12); // DLL NAME
        PXFileWriteA(pxFile, "\", CallingConvention = CallingConvention.", 41);

        switch(symbol->FunctionCallingConvention)
        {
            default:
            case PXDocumentCallingConventionCDeclaration:
                PXFileWriteA(pxFile, PXCSharpFunctionCallCdecl, sizeof(PXCSharpFunctionCallCdecl) - 1);
                break;
            case PXDocumentCallingConventionStandardCall:
                PXFileWriteA(pxFile, PXCSharpFunctionCallStdCall, sizeof(PXCSharpFunctionCallStdCall) - 1);
                break;
            case PXDocumentCallingConventionFastCall:
                PXFileWriteA(pxFile, PXCSharpFunctionCallFastCall, sizeof(PXCSharpFunctionCallFastCall) - 1);
                break;
            case PXDocumentCallingConventionThisCall:
                PXFileWriteA(pxFile, PXCSharpFunctionCallThisCall, sizeof(PXCSharpFunctionCallThisCall) - 1);
                break;

            //default:
                // ERROR
                //break;
        }

        PXFileWriteA(pxFile, ")] private static extern ", 25);
    }

    //PXFileWriteA(pxFile, PXJavaKeyWordVoid, 4);
    //PXFileWriteC(pxFile, ' ');
    PXFileWriteA(pxFile, symbol->NameShortAdress, symbol->NameShortSize);

    pxCompiler->WriteInfo.WriteParameter(pxCompiler);

    PXFileWriteC(pxFile, ';');
    //PXFileWriteNewLine(pxFile);
}

void PXAPI PXCSharpDefinitionWrite(PXCompiler* const pxCompiler)
{
    PXFile* const pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement* const symbol = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    //PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

    // const does not exists in this context. const in C# does mean another thing

    // type
    if(symbol->DataTypeIsBuildIn)
    {
        switch(symbol->DataType & PXDataTypeSizeMask)
        {
            case PXDataTypeSize08:
                //PXFileWriteA(pxFile, PXJavaKeyWordByte, 4);
                break;

            case PXDataTypeSize16:
                //PXFileWriteA(pxFile, PXJavaKeyWordShort, 5);
                break;

            case PXDataTypeSize32:
            //    PXFileWriteA(pxFile, PXJavaKeyWordInt, 3);
                break;

            case PXDataTypeSize64:
                //PXFileWriteA(pxFile, PXJavaKeyWordLong, 4);
                break;

            default:
                PXFileWriteA(pxFile, "Error-Type", 10);
                break;
        }
    }
    else
    {
        // Custom type
        PXFileWriteA(pxFile, symbol->TypeNameAdress, symbol->TypeNameSize);
    }

    // Adress?


    PXFileWriteC(pxFile, ' ');
    PXFileWriteA(pxFile, symbol->NameAdress, symbol->NameSize);
}

PXActionResult PXAPI PXCSharpLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCSharpSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
    PXFile* pxFile = pxResourceSaveInfo->FileReference;

    if(!pxResourceSaveInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    if(PXResourceTypeCodeDocument != pxResourceSaveInfo->Type)
    {
        return PXActionRefusedArgumentInvalid;
    }

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.CommentSingleLine = "//";
    pxCompiler.CommentSingleLineSize = 2;
    pxCompiler.CommentMultibleLineBegin = "/*";
    pxCompiler.CommentMultibleLineBeginSize = 2;
    pxCompiler.CommentMultibleLineEnd = "*/";
    pxCompiler.CommentMultibleLineEndSize = 2;
    pxCompiler.CodeDocument = (PXCodeDocument*)pxResourceSaveInfo->Target;
    pxCompiler.WriteInfo.FileOutput = pxResourceSaveInfo->FileReference;
    pxCompiler.WriteInfo.WriteFile = PXCSharpWriteFile;
    pxCompiler.WriteInfo.WriteContainer = PXCSharpContainerWrite;
    pxCompiler.WriteInfo.WriteFunction = PXCSharpFunctionWrite;
    pxCompiler.WriteInfo.WriteDefinition = PXCSharpDefinitionWrite;

    PXCompilerWrite(&pxCompiler);

    return PXActionRefusedNotImplemented;
}