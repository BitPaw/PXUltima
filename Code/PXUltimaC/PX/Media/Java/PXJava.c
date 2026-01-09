#include "PXJava.h"

#include <PX/Media/PXDocument.h>
#include <PX/Compiler/PXCompiler.h>

char PXJavaKeyWordPublic[] = "public";
char PXJavaKeyWordPrivate[] = "private";
char PXJavaKeyWordClass[] = "class";
char PXJavaKeyWordReadOnly[] = "final";
char PXJavaKeyWordEnum[] = "enum";
char PXJavaKeyWordVoid[] = "void";
char PXJavaKeyWordNative[] = "native";

char PXJavaKeyWordByte[] = "byte";
char PXJavaKeyWordInt[] = "int";
char PXJavaKeyWordShort[] = "short";
char PXJavaKeyWordLong[] = "long";
char PXJavaKeyWordPXF32[] = "PXF32";
char PXJavaKeyWordDouble[] = "double";

void PXAPI PXJavaWriteFile(PXCompiler PXREF pxCompiler)
{
    PXJavaContainerWrite(pxCompiler);
}

void PXAPI PXJavaContainerWrite(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF entry = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * entry->Depth);
    PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
    PXFileWriteC(pxFile, ' ');

    switch(entry->Type)
    {
    case PXDocumentElementTypeEnum:
    {
        PXFileWriteA(pxFile, PXJavaKeyWordEnum, 4);
        break;
    }
    case PXDocumentElementTypeFile:
    case PXDocumentElementTypeStruct:
    case PXDocumentElementTypeClass:
    {
        PXFileWriteA(pxFile, PXJavaKeyWordClass, 5);
        break;
    }

    default:
        break;
    }

    PXFileWriteC(pxFile, ' ');

    if(entry->NameShortAdress)
    {
        PXFileWriteA(pxFile, entry->NameShortAdress, entry->NameShortSize);
    }
    else
    {
        PXFileWriteA(pxFile, entry->NameAdress, entry->NameSize);
    }

    PXFileWriteNewLine(pxFile);
    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * entry->Depth);
    PXFileWriteC(pxFile, '{');
    PXFileWriteNewLine(pxFile);

    PXCodeDocumentElement* prevSym = pxCompiler->WriteInfo.CodeElementCurrent;

    /*

    for
    (
        PXCodeDocumentElement* child = entry->ElementChildFirstBorn;
        child;
        child = child->ElementSibling
    )
    {
        switch(entry->Type)
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
    */

    pxCompiler->WriteInfo.CodeElementCurrent = prevSym;


    PXFileWriteNewLine(pxFile);
    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * entry->Depth);
    PXFileWriteC(pxFile, '}');
    PXFileWriteNewLine(pxFile);
}

void PXAPI PXJavaFunctionWrite(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF entry = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    PXFileWriteFill(pxFile, ' ', pxCompiler->WriteInfo.TABSize * entry->Depth);

    PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
    PXFileWriteC(pxFile, ' ');

    if(1) // Is native?
    {
        PXFileWriteA(pxFile, PXJavaKeyWordNative, 6);
        PXFileWriteC(pxFile, ' ');
    }

    PXFileWriteA(pxFile, PXJavaKeyWordVoid, 4);
    PXFileWriteC(pxFile, ' ');
    PXFileWriteA(pxFile, entry->NameShortAdress, entry->NameShortSize);

    pxCompiler->WriteInfo.WriteParameter(pxCompiler);

    PXFileWriteC(pxFile, ';');
    //PXFileWriteNewLine(pxFile);
}

void PXAPI PXJavaDefinitionWrite(PXCompiler PXREF pxCompiler)
{
    PXFile PXREF pxFile = pxCompiler->WriteInfo.FileOutput;
    PXCodeDocumentElement PXREF entry = pxCompiler->WriteInfo.CodeElementCurrent;

    pxCompiler->WriteInfo.WriteComment(pxCompiler);

    //PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

    // readonly
    if(entry->DataType & PXTypeReadOnly)
    {
        PXFileWriteA(pxFile, PXJavaKeyWordReadOnly, 5);
        PXFileWriteC(pxFile, ' ');
    }

    // type
    if(entry->DataTypeIsBuildIn)
    {
        switch(entry->DataType & PXTypeSizeMask)
        {
        case PXTypeSize08:
            PXFileWriteA(pxFile, PXJavaKeyWordByte, 4);
            break;

        case PXTypeSize16:
            PXFileWriteA(pxFile, PXJavaKeyWordShort, 5);
            break;

        case PXTypeSize32:
            PXFileWriteA(pxFile, PXJavaKeyWordInt, 3);
            break;

        case PXTypeSize64:
            PXFileWriteA(pxFile, PXJavaKeyWordLong, 4);
            break;

        default:
            PXFileWriteA(pxFile, "Error-Type", 10);
            break;
        }
    }
    else
    {
        // Custom type
        PXFileWriteA(pxFile, entry->TypeNameAdress, entry->TypeNameSize);
    }

    // Adress?


    PXFileWriteC(pxFile, ' ');
    PXFileWriteA(pxFile, entry->NameAdress, entry->NameSize);
}

PXResult PXAPI PXJavaLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXJavaSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo)
{
    if(!pxResourceSaveInfo)
    {
        return PXResultRefusedParameterNull;
    }

    if(PXResourceTypeCodeDocument != pxResourceSaveInfo->ResourceType)
    {
        return PXResultRefusedParameterInvalid;
    }

    PXCompiler pxCompiler;
    PXClear(PXCompiler, &pxCompiler);
    pxCompiler.CommentSingleLine = "//";
    pxCompiler.CommentSingleLineSize = 2;
    pxCompiler.CommentMultibleLineBegin = "/*";
    pxCompiler.CommentMultibleLineBeginSize = 2;
    pxCompiler.CommentMultibleLineEnd = "*/";
    pxCompiler.CommentMultibleLineEndSize = 2;
    pxCompiler.CodeDocument = (PXCodeDocument*)pxResourceSaveInfo->ResourceTarget;
    pxCompiler.WriteInfo.FileOutput = pxResourceSaveInfo->FileReference;
    pxCompiler.WriteInfo.WriteFile = PXJavaWriteFile;
    pxCompiler.WriteInfo.WriteContainer = PXJavaContainerWrite;
    pxCompiler.WriteInfo.WriteFunction = PXJavaFunctionWrite;
    pxCompiler.WriteInfo.WriteDefinition = PXJavaDefinitionWrite;

    PXCompilerWrite(&pxCompiler);

    return PXActionRefusedNotImplemented;
}
