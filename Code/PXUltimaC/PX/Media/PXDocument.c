#include "PXDocument.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

PXDocumentElementType PXAPI PXDocumentElementAnalyseElement(const char* name, const PXSize nameSize)
{
    switch(nameSize)
    {
    case 2:
    {
        const PXI16U key = PXI16FromAdress(name);

        switch(key)
        {
        case PXI16Make('i', 'f'):
            return PXDocumentElementTypeIf;

        case PXI16Make('d', 'o'):
            return PXDocumentElementTypeDo;
        }

        break;
    }
    case 3:
    {
        const PXI32U key = PXI24FromAdress(name);

        switch(key)
        {
        case PXI24Make('i', 'n', 't'):
            return PXDocumentElementTypeInt;

        case PXI24Make('f', 'o', 'r'):
            return PXDocumentElementTypeFor;
        }

        break;
    }
    case 4:
    {
        const PXI32U key = PXI32FromAdress(name);

        switch(key)
        {
        case PXI32Make('c', 'a', 's', 'e'):
            return PXDocumentElementTypeCase;
        case PXI32Make('t', 'r', 'u', 'e'):
            return PXDocumentElementTypeTrue;
        case PXI32Make('b', 'o', 'o', 'l'):
            return PXDocumentElementTypeBool;
        case PXI32Make('c', 'h', 'a', 'r'):
            return PXDocumentElementTypeChar;
        case PXI32Make('l', 'o', 'n', 'g'):
            return PXDocumentElementTypeLong;
        case PXI32Make('a', 'u', 't', 'o'):
            return PXDocumentElementTypeAutomaticType;
        case PXI32Make('e', 'l', 's', 'e'):
            return PXDocumentElementTypeElse;
        case PXI32Make('g', 'o', 't', 'o'):
            return PXDocumentElementTypeGoTo;
        case PXI32Make('e', 'n', 'u', 'm'):
            return PXDocumentElementTypeEnum;
        case PXI32Make('v', 'o', 'i', 'd'):
            return PXDocumentElementTypeVoid;
        }

        break;
    }
    case 5u:
    {
        const PXI64U key = PXI40FromAdress(name);

        switch(key)
        {
        case PXI40Make('f', 'i', 'n', 'a', 'l'):
            return PXDocumentElementTypeFinal;
        case PXI40Make('s', 'h', 'o', 'r', 't'):
            return PXDocumentElementTypeShort;
        case PXI40Make('u', 'n', 'i', 'o', 'n'):
            return PXDocumentElementTypeUnion;
        case PXI40Make('w', 'h', 'i', 'l', 'e'):
            return PXDocumentElementTypeWhile;
        case PXI40Make('f', 'a', 'l', 's', 'e'):
            return PXDocumentElementTypeFalse;
        case PXI40Make('f', 'l', 'o', 'a', 't'):
            return PXDocumentElementTypePXF32;
        case PXI40Make('c', 'o', 'n', 's', 't'):
            return PXDocumentElementTypeConst;
        case PXI40Make('b', 'r', 'e', 'a', 'k'):
            return PXDocumentElementTypeBreak;
        case PXI40Make('i', 'f', 'd', 'e', 'f'):
            return PXDocumentElementTypePreprocessorIfDefined;
        case PXI40Make('e', 'n', 'd', 'i', 'f'):
            return PXDocumentElementTypePreprocessorDefinitionEnd;
        }

        break;
    }
    case 6u:
    {
        const PXI64U key = PXI48FromAdress(name);

        switch(key)
        {
        case PXI48Make('s', 'i', 'z', 'e', 'o', 'f'):
            return PXDocumentElementTypeSizeOfType;
        case PXI48Make('r', 'e', 't', 'u', 'r', 'n'):
            return PXDocumentElementTypeReturn;
        case PXI48Make('e', 'x', 't', 'e', 'r', 'n'):
            return PXDocumentElementTypeExtern;
        case PXI48Make('s', 't', 'a', 't', 'i', 'c'):
            return PXDocumentElementTypeStatic;
        case PXI48Make('s', 't', 'r', 'u', 'c', 't'):
            return PXDocumentElementTypeStruct;
        case PXI48Make('d', 'o', 'u', 'b', 'l', 'e'):
            return PXDocumentElementTypeDouble;
        case PXI48Make('s', 'i', 'g', 'n', 'e', 'd'):
            return PXDocumentElementTypeSigned;
        case PXI48Make('s', 'w', 'i', 't', 'c', 'h'):
            return PXDocumentElementTypeSwitch;
        case PXI48Make('i', 'f', 'n', 'd', 'e', 'f'):
            return PXDocumentElementTypePreprocessorIfNotDefined;
        case PXI48Make('d', 'e', 'f', 'i', 'n', 'e'):
            return PXDocumentElementTypePreprocessorDefine;
        case PXI48Make('p', 'r', 'a', 'g', 'm', 'a'):
            return PXDocumentElementTypePreprocessorPragma;
        }

        break;
    }
    case 7u:
    {
        const PXI64U key = PXI56FromAdress(name);

        switch(key)
        {
        case PXI56Make('d', 'e', 'f', 'a', 'u', 'l', 't'):
            return PXDocumentElementTypeDefault;
        case PXI56Make('t', 'y', 'p', 'e', 'd', 'e', 'f'):
            return PXDocumentElementTypeTypeDefinition;
        case PXI56Make('i', 'n', 'c', 'l', 'u', 'd', 'e'):
            return PXDocumentElementTypePreprocessorIncluded;
        }

        break;
    }
    case 8u:
    {
        const PXI64U key = PXI64FromAdress(name);

        switch(key)
        {
        case PXI64Make('c', 'o', 'n', 't', 'i', 'n', 'u', 'e'):
            return PXDocumentElementTypeContinue;
        case PXI64Make('u', 'n', 's', 'i', 'g', 'n', 'e', 'd'):
            return PXDocumentElementTypeUnsigned;
        case PXI64Make('r', 'e', 's', 't', 'r', 'i', 'c', 't'):
            return PXDocumentElementTypeRestrict;
        case PXI64Make('v', 'o', 'l', 'a', 't', 'i', 'l', 'e'):
            return PXDocumentElementTypeVolitile;
        case PXI64Make('r', 'e', 'g', 'i', 's', 't', 'e', 'r'):
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
    case PXDocumentElementTypePreprocessorIncluded:
        return "Included";
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

PXResult PXAPI PXDocumentElementRoot(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
{
    return PXResultOK;
}

PXResult PXAPI PXDocumentElementChildGet(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
{
    return PXResultOK;
}

PXResult PXAPI PXDocumentElementSiblingGet(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
{
    return PXResultOK;
}

PXCodeDocumentElement* PXAPI PXCodeDocumentElementAdd(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
{
    if(pxDocumentElement->ID == 0)
    {
        // Generate ID to make dynamic updates later
        pxDocumentElement->ID = pxDocument->LastCounter++;
    }

    // Make sure we have enough space
    if(pxDocument->ElementListAllocated <= (pxDocument->ElementListUsed +1))
    {
        pxDocument->ElementListAllocated += 10;
        pxDocument->ElementList = PXMemoryHeapReallocT(PXCodeDocumentElement, pxDocument->ElementList, pxDocument->ElementListAllocated);    
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

PXResult PXAPI PXCodeDocumentElementGenerateChild
(
    PXCodeDocument PXREF pxDocument,
    PXDocumentElementType pxDocumentElementType,
    const PXSize depth,
    PXCodeDocumentElement** pxDocumentElement,
    PXCodeDocumentElement PXREF pxDocumentElementParent
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

void PXAPI PXCodeDocumentElementPrintSingle(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
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
    case PXDocumentElementTypePreprocessorIncluded:
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

void PXAPI PXCodeDocumentElementPrintAll(PXCodeDocument PXREF pxDocument)
{
    for(size_t i = 0; i < pxDocument->LastCounter; i++)
    {
        PXCodeDocumentElement* pxCodeDocumentElement = &pxDocument->ElementList[i];

        PXCodeDocumentElementPrintSingle(pxDocument, pxCodeDocumentElement);
    }
}

PXSize PXAPI PXDocumentElementIO(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction)
{
#if 0
    const PXI8U symbolID = pxDocumentElement->Type;
    PXI32U amount = 0;

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
        &symbolID, PXTypeI08U,
        &amount, PXTypeI32U
    };

    const PXSize bytesTransphered = pxFileIOMultibleFunction(&pxDocument->Data, pxFileDataElementTypeList, sizeof(pxFileDataElementTypeList));


    return bytesTransphered;
#endif

    return 0;
}

PXResult PXAPI PXDocumentElementWrite(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
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

    return PXResultOK;
}

PXResult PXAPI PXDocumentElementRead(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
{
    return PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileReadMultible);;
}

PXResult PXAPI PXDocumentElementAdd(PXCodeDocument PXREF pxDocument, PXCodeDocumentElement PXREF pxDocumentElement)
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

            return PXResultInvalid;
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

    return PXResultOK;
}

PXResult PXAPI PXDocumentPrintNode(PXCodeDocumentElement PXREF pxDocumentElement)
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
    case PXTypeF32:
    {
        PXF32 value;
        PXCopy(PXF32, &pxDocumentElement->ElementMember.ValueAdress, &value);

        PXTextPrintA(typeBuffer, 64, "PXF32");
        PXTextPrintA(valueBuffer, 64, "%-5.2f", value);
        break;
    }
    case PXTypeIS:
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

PXResult PXAPI PXDocumentPrint(PXCodeDocument PXREF pxDocument)
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

    return PXResultOK;
}
