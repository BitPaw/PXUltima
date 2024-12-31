#include "PXDocument.h"

#include <OS/Console/PXConsole.h>

PXDocumentElementType PXAPI PXDocumentElementAnalyseElement(const char* name, const PXSize nameSize)
{
    switch(nameSize)
    {
    case 2:
    {
        const PXInt16U key = PXInt16FromAdress(name);

        switch(key)
        {
        case PXInt16Make('i', 'f'):
            return PXDocumentElementTypeIf;

        case PXInt16Make('d', 'o'):
            return PXDocumentElementTypeDo;
        }

        break;
    }
    case 3:
    {
        const PXInt32U key = PXInt24FromAdress(name);

        switch(key)
        {
        case PXInt24Make('i', 'n', 't'):
            return PXDocumentElementTypeInt;

        case PXInt24Make('f', 'o', 'r'):
            return PXDocumentElementTypeFor;
        }

        break;
    }
    case 4:
    {
        const PXInt32U key = PXInt32FromAdress(name);

        switch(key)
        {
        case PXInt32Make('c', 'a', 's', 'e'):
            return PXDocumentElementTypeCase;
        case PXInt32Make('t', 'r', 'u', 'e'):
            return PXDocumentElementTypeTrue;
        case PXInt32Make('b', 'o', 'o', 'l'):
            return PXDocumentElementTypeBool;
        case PXInt32Make('c', 'h', 'a', 'r'):
            return PXDocumentElementTypeChar;
        case PXInt32Make('l', 'o', 'n', 'g'):
            return PXDocumentElementTypeLong;
        case PXInt32Make('a', 'u', 't', 'o'):
            return PXDocumentElementTypeAutomaticType;
        case PXInt32Make('e', 'l', 's', 'e'):
            return PXDocumentElementTypeElse;
        case PXInt32Make('g', 'o', 't', 'o'):
            return PXDocumentElementTypeGoTo;
        case PXInt32Make('e', 'n', 'u', 'm'):
            return PXDocumentElementTypeEnum;
        case PXInt32Make('v', 'o', 'i', 'd'):
            return PXDocumentElementTypeVoid;
        }

        break;
    }
    case 5u:
    {
        const PXInt64U key = PXInt40FromAdress(name);

        switch(key)
        {
        case PXInt40Make('f', 'i', 'n', 'a', 'l'):
            return PXDocumentElementTypeFinal;
        case PXInt40Make('s', 'h', 'o', 'r', 't'):
            return PXDocumentElementTypeShort;
        case PXInt40Make('u', 'n', 'i', 'o', 'n'):
            return PXDocumentElementTypeUnion;
        case PXInt40Make('w', 'h', 'i', 'l', 'e'):
            return PXDocumentElementTypeWhile;
        case PXInt40Make('f', 'a', 'l', 's', 'e'):
            return PXDocumentElementTypeFalse;
        case PXInt40Make('f', 'l', 'o', 'a', 't'):
            return PXDocumentElementTypeFloat;
        case PXInt40Make('c', 'o', 'n', 's', 't'):
            return PXDocumentElementTypeConst;
        case PXInt40Make('b', 'r', 'e', 'a', 'k'):
            return PXDocumentElementTypeBreak;
        case PXInt40Make('i', 'f', 'd', 'e', 'f'):
            return PXDocumentElementTypePreprocessorIfDefined;
        case PXInt40Make('e', 'n', 'd', 'i', 'f'):
            return PXDocumentElementTypePreprocessorDefinitionEnd;
        }

        break;
    }
    case 6u:
    {
        const PXInt64U key = PXInt48FromAdress(name);

        switch(key)
        {
        case PXInt48Make('s', 'i', 'z', 'e', 'o', 'f'):
            return PXDocumentElementTypeSizeOfType;
        case PXInt48Make('r', 'e', 't', 'u', 'r', 'n'):
            return PXDocumentElementTypeReturn;
        case PXInt48Make('e', 'x', 't', 'e', 'r', 'n'):
            return PXDocumentElementTypeExtern;
        case PXInt48Make('s', 't', 'a', 't', 'i', 'c'):
            return PXDocumentElementTypeStatic;
        case PXInt48Make('s', 't', 'r', 'u', 'c', 't'):
            return PXDocumentElementTypeStruct;
        case PXInt48Make('d', 'o', 'u', 'b', 'l', 'e'):
            return PXDocumentElementTypeDouble;
        case PXInt48Make('s', 'i', 'g', 'n', 'e', 'd'):
            return PXDocumentElementTypeSigned;
        case PXInt48Make('s', 'w', 'i', 't', 'c', 'h'):
            return PXDocumentElementTypeSwitch;
        case PXInt48Make('i', 'f', 'n', 'd', 'e', 'f'):
            return PXDocumentElementTypePreprocessorIfNotDefined;
        case PXInt48Make('d', 'e', 'f', 'i', 'n', 'e'):
            return PXDocumentElementTypePreprocessorDefine;
        case PXInt48Make('p', 'r', 'a', 'g', 'm', 'a'):
            return PXDocumentElementTypePreprocessorPragma;
        }

        break;
    }
    case 7u:
    {
        const PXInt64U key = PXInt56FromAdress(name);

        switch(key)
        {
        case PXInt56Make('d', 'e', 'f', 'a', 'u', 'l', 't'):
            return PXDocumentElementTypeDefault;
        case PXInt56Make('t', 'y', 'p', 'e', 'd', 'e', 'f'):
            return PXDocumentElementTypeTypeDefinition;
        case PXInt56Make('i', 'n', 'c', 'l', 'u', 'd', 'e'):
            return PXDocumentElementTypePreprocessorInclude;
        }

        break;
    }
    case 8u:
    {
        const PXInt64U key = PXInt64FromAdress(name);

        switch(key)
        {
        case PXInt64Make('c', 'o', 'n', 't', 'i', 'n', 'u', 'e'):
            return PXDocumentElementTypeContinue;
        case PXInt64Make('u', 'n', 's', 'i', 'g', 'n', 'e', 'd'):
            return PXDocumentElementTypeUnsigned;
        case PXInt64Make('r', 'e', 's', 't', 'r', 'i', 'c', 't'):
            return PXDocumentElementTypeRestrict;
        case PXInt64Make('v', 'o', 'l', 'a', 't', 'i', 'l', 'e'):
            return PXDocumentElementTypeVolitile;
        case PXInt64Make('r', 'e', 'g', 'i', 's', 't', 'e', 'r'):
            return PXDocumentElementTypeRegister;

        }

        break;
    }
    }

    return PXDocumentElementTypeUnkown;
}

const char* PXAPI PXDocumentElementTypeToString(const PXDocumentElementType pxDocumentElementType)
{
    switch(pxDocumentElementType)
    {
    case PXDocumentElementTypePreprocessorInclude:
        return "include";
    case PXDocumentElementTypeNamespace:
        return "namespace";
    case PXDocumentElementTypeStruct:
        return "struct";
    case PXDocumentElementTypeUnion:
        return "union";
    case PXDocumentElementTypeEnum:
        return "enum";
    case PXDocumentElementTypeClass:
        return "class";
    case PXDocumentElementTypeFunction:
        return "function";
    case PXDocumentElementTypeClassMember:
        return "class-member";
    case PXDocumentElementTypeClassAttribute:
        return "attribute";
    case PXDocumentElementTypeEnumMember:
        return "enum-member";
    case PXDocumentElementTypeFunctionParameter:
        return "fuction-parameter";
    case PXDocumentElementTypeFile:
        return "file";

    default:
        return PXNull;
    }
}

PXActionResult PXAPI PXDocumentElementRoot(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementChildGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementSiblingGet(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    return PXActionSuccessful;
}

PXCodeDocumentElement* PXAPI PXCodeDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    if(pxDocumentElement->ID == 0)
    {
        // Generate ID to make dynamic updates later
        pxDocumentElement->ID = pxDocument->LastCounter++;
    }

    // Make sure we have enough space
    if(pxDocument->ElementListAllocated <= (pxDocument->ElementListUsed +1))
    {
        pxDocument->ElementList = PXMemoryRealloc(pxDocument->ElementList, sizeof(PXCodeDocumentElement) * (pxDocument->ElementListAllocated + 10));

        pxDocument->ElementListAllocated += 10;
    }

    PXCodeDocumentElement* newTarget = &pxDocument->ElementList[pxDocumentElement->ID];


    PXCopy(PXCodeDocumentElement, pxDocumentElement, newTarget);

    //PXCodeDocumentElementPrintSingle(pxDocumentElement, newTarget);

    /*



        // This is not an attribute but a member.
        // We need to get the parent object and use that name to merge it into a data entry


        PXFileCursorMoveTo(&pxDocument->Data, pxDocument->LastEntryOffset); // Move to prev entry

        PXCodeDocumentElement pxDocumentElementClass;
        PXCodeDocumentElement pxDocumentElementMemberName;

        {
            const PXSize readBytes = PXDocumentElementIO(pxDocument, &pxDocumentElementMemberName, PXFileReadMultible); // Read parent entry

            PXFileCursorRewind(&pxDocument->Data, readBytes);

            pxDocumentElement->Type = PXDocumentElementTypeClassAttribute;

            if (!pxDocumentElement->NameAdress)
            {
                pxDocumentElement->NameAdress = pxDocumentElementMemberName.NameAdress;
                pxDocumentElement->NameSize = pxDocumentElementMemberName.NameSize;
                pxDocumentElement->Type = PXDocumentElementTypeClassMember;
            }
        }

        {
            // Go even further to resize
            const PXSize currentPosition = pxDocument->Data.DataCursor;

            PXFileCursorMoveTo(&pxDocument->Data, pxDocumentElementMemberName.ParaentDataPosition);

            const PXSize readBytes = PXDocumentElementIO(pxDocument, &pxDocumentElementClass, PXFileReadMultible); // Read parent entry
            PXFileCursorRewind(&pxDocument->Data, readBytes);

            if (pxDocumentElement->NameAdress != 0)
            {
                ++(pxDocumentElementClass.AttribtesAmount);
            }
            else
            {
                ++(pxDocumentElementClass.MemberAmount);
            }

            pxDocumentElement->ParaentDataPosition = pxDocument->Data.DataCursor;

            const PXSize writtenBytes = PXDocumentElementIO(pxDocument, &pxDocumentElementClass, PXFileWriteMultible);

            PXFileCursorMoveTo(&pxDocument->Data, currentPosition); // Reset

    #if 1
            {
                char buffer[64];
                PXTextCopyA(pxDocumentElementClass.NameAdress, pxDocumentElementClass.NameSize, buffer, 64);

    #if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Document",
                    "<%s:0x%p> updated. C:%i A:%i M:%i",
                    buffer,
                    pxDocumentElement->ParaentDataPosition,
                    pxDocumentElementClass.ElementChildrenAmount,
                    pxDocumentElementClass.AttribtesAmount,
                    pxDocumentElementClass.MemberAmount
                );
    #endif
            }
    #endif
        }


    PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileWriteMultible); // Write change

    PXDocumentPrintNode(pxDocumentElement);
    */

    return newTarget;
}

PXActionResult PXAPI PXCodeDocumentElementGenerateChild
(
    PXCodeDocument* const pxDocument,
    PXDocumentElementType pxDocumentElementType,
    const PXSize depth,
    PXCodeDocumentElement** pxDocumentElement,
    PXCodeDocumentElement* const pxDocumentElementParent
)
{
    PXCodeDocumentElement pxCodeDocumentElementTemp;
    PXClear(PXCodeDocumentElement, &pxCodeDocumentElementTemp);
    pxCodeDocumentElementTemp.Type = pxDocumentElementType;
    // pxCodeDocumentElementTemp.ElementParent = pxDocumentElementParent;
    pxCodeDocumentElementTemp.Depth = depth;

    PXCodeDocumentElement* pxCodeDocumentElement = PXCodeDocumentElementAdd(pxDocument, &pxCodeDocumentElementTemp); // Use to pre-register space. aka. allocate object



    //PXHierarchicalNodeEntryAdd();

    /*

    if(pxDocumentElementParent)
    {


        if(!pxDocumentElementParent->ElementChildFirstBorn) // if we dont have a child yet, we are the first
        {
            pxDocumentElementParent->ElementChildFirstBorn = pxCodeDocumentElement;
        }
        else
        {
            PXCodeDocumentElement* insertChild = pxDocumentElementParent->ElementChildFirstBorn;

            while(insertChild)
            {
                if(!insertChild->ElementSibling) // if has no sibling
                {
                    insertChild->ElementSibling = pxCodeDocumentElement; // this is not the last sibling
                    break;
                }

                insertChild = insertChild->ElementSibling;
            }
        }
    }
    */

    *pxDocumentElement = pxCodeDocumentElement;
}

void PXAPI PXCodeDocumentElementPrintSingle(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
#if PXLogEnable
    const char* typeName = PXDocumentElementTypeToString(pxDocumentElement->Type);

    char commentBuffer[128];
    PXMemoryClear(commentBuffer, 128);
    PXTextCopyA(pxDocumentElement->CommentAdress, pxDocumentElement->CommentSize, commentBuffer, 128);

    char nameBuffer[64];
    PXMemoryClear(nameBuffer, 64);
    PXTextCopyA(pxDocumentElement->NameAdress, pxDocumentElement->NameSize, nameBuffer, 64);


    char nameSpaceBuffer[64];

    {
        char AAAAAAAAAAAAAAA[64];
        PXMemoryClear(AAAAAAAAAAAAAAA, 64);
        PXTextCopyA(pxDocumentElement->NameSpaceAdress, pxDocumentElement->NameSpaceSize, AAAAAAAAAAAAAAA, 64);

        char BBBBBBBBBB[64];
        PXMemoryClear(BBBBBBBBBB, 64);
        PXTextCopyA(pxDocumentElement->NameClassAdress, pxDocumentElement->NameClassSize, BBBBBBBBBB, 64);

        char CCCCCCCCCCCC[64];
        PXMemoryClear(CCCCCCCCCCCC, 64);
        PXTextCopyA(pxDocumentElement->NameShortAdress, pxDocumentElement->NameShortSize, CCCCCCCCCCCC, 64);

        PXTextPrintA(nameSpaceBuffer, 64, "S:%s, C:%s, N:%s", AAAAAAAAAAAAAAA, BBBBBBBBBB, CCCCCCCCCCCC);
    }




    int parrentID = 0;//pxDocumentElement->ElementParent ? pxDocumentElement->ElementParent->ID : -1;
    int siblingID = 0;//pxDocumentElement->ElementSibling ? pxDocumentElement->ElementSibling->ID : -1;
    int childID = 0;// pxDocumentElement->ElementChildFirstBorn ? pxDocumentElement->ElementChildFirstBorn->ID : -1;

    char identification[515];
    PXTextPrintA
    (
        identification,
        515,
        "\n"
        "%10s : %i\n"
        "%10s : %i\n"
        "%10s : %s\n"
        "%10s : %i\n"
        "%10s : %i\n"
        "%10s : %i\n"
        "%10s : %s\n"
        "%10s : %s\n"
        "%10s : %s",
        "ID", pxDocumentElement->ID,
        "Depth", pxDocumentElement->Depth,
        "Type", typeName,
        "Parrent", parrentID,
        "Sibling", siblingID,
        "Child", childID,
        "Name", nameBuffer,
        "--->", nameSpaceBuffer,
        "Comment", commentBuffer
    );

    switch(pxDocumentElement->Type)
    {
    case PXDocumentElementTypePreprocessorInclude:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n"
            "%10s : %s",
            identification,
            "Global", pxDocumentElement->IsGlobal ? "Yes" : "No"
        );

        break;
    }
    case PXDocumentElementTypeStruct:
    case PXDocumentElementTypeClass:
    case PXDocumentElementTypeUnion:
    case PXDocumentElementTypeEnum:
    {
        char aliasBuffer[64];
        PXMemoryClear(aliasBuffer, 64);
        PXTextCopyA(pxDocumentElement->AliasAdress, pxDocumentElement->AliasSize, aliasBuffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n"
            "%10s : %s\n"
            "%10s : %i",
            identification,
            "Alias", aliasBuffer,
            "Members", pxDocumentElement->MemberAmount
        );

        break;
    }
    case PXDocumentElementTypeClassAttribute:
    case PXDocumentElementTypeClassMember:
    case PXDocumentElementTypeFunctionParameter:
    {
        char buffer[64];

        PXTypeToString(pxDocumentElement->DataType, buffer);

        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n"
            "%10s : %s",
            identification,
            "DataType", buffer
        );

        break;
    }
    case PXDocumentElementTypeEnumMember:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n",
            identification
        );

        break;
    }
    case PXDocumentElementTypeFunction:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n"
            "%10s : %s",
            identification,
            "Call", "stst"
        );

        break;
    }
    case PXDocumentElementTypeFile:
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "CodeDocument",
            "Entry",
            "%s\n"
            "File",
            identification
        );

        break;
    }
    default:
        break;
    }
#endif
}

void PXAPI PXCodeDocumentElementPrintAll(PXCodeDocument* const pxDocument)
{
    for(size_t i = 0; i < pxDocument->LastCounter; i++)
    {
        PXCodeDocumentElement* pxCodeDocumentElement = &pxDocument->ElementList[i];

        PXCodeDocumentElementPrintSingle(pxDocument, pxCodeDocumentElement);
    }
}

PXSize PXAPI PXDocumentElementIO(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction)
{
#if 0
    const PXInt8U symbolID = pxDocumentElement->Type;
    PXInt32U amount = 0;

    PXTypeEntry pxFileDataElementTypeList[] =
    {
        //&pxDocumentElement->ID, PXTypeSize,
        &pxDocumentElement->Depth, PXTypeSize,
        &pxDocumentElement->NameSize, PXTypeSize,
        &pxDocumentElement->NameAdress, PXTypeAdress,
        &pxDocumentElement->AttribtesAmount, PXTypeSize,
        &pxDocumentElement->MemberAmount, PXTypeSize,
        &pxDocumentElement->ElementChildrenAmount, PXTypeSize,
        &pxDocumentElement->ElementSiblingsAmount, PXTypeSize,
        &pxDocumentElement->ParaentDataPosition, PXTypeSize,
        &symbolID, PXTypeInt08U,
        &amount, PXTypeInt32U
    };

    const PXSize bytesTransphered = pxFileIOMultibleFunction(&pxDocument->Data, pxFileDataElementTypeList, sizeof(pxFileDataElementTypeList));


    return bytesTransphered;
#endif

    return 0;
}

PXActionResult PXAPI PXDocumentElementWrite(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{

#if 0
    const PXBool isNotRoot = pxDocumentElement->Depth == (pxDocument->LastEntryDepth + 1);

    PXSize parrentPosition = 0;

    if (isNotRoot)
    {
        // Update current Entry

        PXFileCursorMoveTo(&pxDocument->Data, pxDocument->LastEntryOffset); // Move to prev entry

        PXCodeDocumentElement pxDocumentElementTemp;

        const PXSize readBytes = PXDocumentElementIO(pxDocument, &pxDocumentElementTemp, PXFileReadMultible); // Read parent entry

        PXFileCursorRewind(&pxDocument->Data, readBytes);

        ++(pxDocumentElementTemp.ElementChildrenAmount); // Update amount of sub-objects

        parrentPosition = pxDocument->Data.DataCursor;

        PXDocumentElementIO(pxDocument, &pxDocumentElementTemp, PXFileWriteMultible); // Write change

#if PXLogEnable
        {
            char buffer[64];
            PXTextCopyA(pxDocumentElementTemp.NameAdress, pxDocumentElementTemp.NameSize, buffer, 64);

            PXLogPrint
            (
                PXLoggingInfo,
                "Document",
                "<%s> updated. Childs:%i",
                buffer,
                pxDocumentElementTemp.ElementChildrenAmount
            );
        }
#endif
    }

    //++(pxDocument->ElementListAmount);
    pxDocumentElement->ParaentDataPosition = parrentPosition;
    pxDocument->LastEntryOffset = pxDocument->Data.DataCursor;
    pxDocument->LastEntryDepth = pxDocumentElement->Depth;

    PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileWriteMultible); // Write node

#if PXLogEnable
    {
        char buffer[64];
        PXTextCopyA(pxDocumentElement->NameAdress, pxDocumentElement->NameSize, buffer, 64);

        PXLogPrint
        (
            PXLoggingInfo,
            "Document",
            "<%s> added",
            buffer,
            pxDocumentElement->ElementChildrenAmount
        );
    }
#endif


#endif

    return 0;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementRead(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    return PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileReadMultible);;
}

PXActionResult PXAPI PXDocumentElementAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
    switch (pxDocumentElement->Type)
    {
    case PXDocumentElementTypeNamespace:
    {
        break;
    }
    case PXDocumentElementTypeStruct:
    {
        break;
    }
    case PXDocumentElementTypeClass:
    {
        if (pxDocumentElement->ElementClass.IsTerminateSignal)
        {

            return;
        }

        PXDocumentElementWrite(pxDocument, pxDocumentElement);

        break;
    }
    case PXDocumentElementTypeFunction:
    {
        break;
    }
    case PXDocumentElementTypeClassMember:
    {
        PXCodeDocumentElementAdd(pxDocument, pxDocumentElement);
        break;
    }
    default:
    {
        break;
    }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentPrintNode(PXCodeDocumentElement* const pxDocumentElement)
{
#if PXLogEnable
    char elementType[64];
    PXMemoryClear(elementType, 64);
    char nameBuffer[64];
    PXTextCopyA(pxDocumentElement->NameAdress, pxDocumentElement->NameSize, nameBuffer, 64);
    char typeBuffer[64];
    PXMemoryClear(typeBuffer, 64);
    char valueBuffer[64];
    PXMemoryClear(valueBuffer, 64);

    switch (pxDocumentElement->ElementMember.ValueType)
    {
    case PXTypeBool:
    {
        PXBool value;
        PXCopy(PXBool, &pxDocumentElement->ElementMember.ValueAdress, &value);

        PXTextPrintA(typeBuffer, 64, "bool");
        PXTextPrintA(valueBuffer, 64, "%s", value ? "true" : "false");
        break;
    }
    case PXTypeFloat:
    {
        float value;
        PXCopy(float, &pxDocumentElement->ElementMember.ValueAdress, &value);

        PXTextPrintA(typeBuffer, 64, "float");
        PXTextPrintA(valueBuffer, 64, "%-5.2f", value);
        break;
    }
    case PXTypeIntS:
    {
        int value;
        PXCopy(int, &pxDocumentElement->ElementMember.ValueAdress, &value);

        PXTextPrintA(typeBuffer, 64, "int");
        PXTextPrintA(valueBuffer, 64, "%i", value);
        break;
    }
    case PXTypeString:
    {
        PXTextPrintA(typeBuffer, 64, "string");
        PXTextCopyA(pxDocumentElement->ElementMember.ValueAdress, pxDocumentElement->ElementMember.ValueSize, valueBuffer, 63);
        break;
    }
    default:
        break;
    }

    switch (pxDocumentElement->Type)
    {
    case PXDocumentElementTypeNamespace:
    {
        break;
    }
    case PXDocumentElementTypeStruct:
    {
        break;
    }
    case PXDocumentElementTypeClass:
    {
        PXTextPrintA(elementType, 64, "class");
        break;
    }
    case PXDocumentElementTypeFunction:
    {
        PXTextPrintA(elementType, 64, "function");
        break;
    }
    case PXDocumentElementTypeClassMember:
    {
        PXTextPrintA(elementType, 64, "member");
        break;
    }
    case PXDocumentElementTypeClassAttribute:
    {
        PXTextPrintA(elementType, 64, "attribute");
        break;
    }
    default:
    {
        break;
    }
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "Document",
        "| %-10s | %-15s | %-10s | %-30s |",
        elementType,
        nameBuffer,
        typeBuffer,
        valueBuffer
    );
#endif
}

PXActionResult PXAPI PXDocumentPrint(PXCodeDocument* const pxDocument)
{
#if 0
    PXCodeDocumentElement pxDocumentElement;

    PXDocumentElementIO(pxDocument, &pxDocumentElement, PXFileReadMultible);// Get root

    for (PXSize i = 0; i < pxDocumentElement.ElementChildrenAmount; ++i) // loop over all roots children
    {
        PXDocumentPrintNode(&pxDocumentElement);

        // Get all
        //printf("");
    }
#endif

    return PXActionSuccessful;
}
