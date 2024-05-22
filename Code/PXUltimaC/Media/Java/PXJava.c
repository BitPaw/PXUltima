#include "PXJava.h"

#include <Media/PXDocument.h>

char PXJavaKeyWordPublic[] = "public";
char PXJavaKeyWordPrivate[] = "private";
char PXJavaKeyWordClass[] = "class";
char PXJavaKeyWordReadOnly[] = "final";
char PXJavaKeyWordEnum[] = "enum";
char PXJavaKeyWordVoid[] = "void";

void PXAPI PXJavaIncludeWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteA(pxFile, "// depends on file ", 19);
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	PXFileWriteNewLine(pxFile);
}

void PXAPI PXJavaEnumWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, PXJavaKeyWordEnum, 4);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	PXFileWriteNewLine(pxFile);
	PXFileWriteA(pxFile, "{", 1);

	for(size_t i = 0; i < pxCodeDocumentElement->MemberAmount; i++)
	{
		PXCodeDocumentElement* pxCodeDocumentChild = pxCodeDocumentElement->ElementChildFirstBorn;
		
		PXFileWriteA(pxFile, PXJavaKeyWordEnum, 4);

		if(pxCodeDocumentElement->MemberAmount != i) // if not last
		{
			PXFileWriteC(pxFile, ',');
		}

		PXFileWriteNewLine(pxFile);
	}

	PXFileWriteA(pxFile, "}", 1);
}

void PXAPI PXJavaContainerWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, PXJavaKeyWordClass, 6);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	PXFileWriteNewLine(pxFile);
	PXFileWriteA(pxFile, "{", 1);

	for(size_t i = 0; i < pxCodeDocumentElement->MemberAmount; i++)
	{
		PXCodeDocumentElement* pxCodeDocumentChild = pxCodeDocumentElement->ElementChildFirstBorn;

		PXJavaElementWrite(pxCodeDocumentChild, pxFile);
	}

	PXFileWriteA(pxFile, "}", 1);
}

void PXAPI PXJavaFunctionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	// Cleave function name?

	PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, PXJavaKeyWordVoid, 4);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	PXFileWriteNewLine(pxFile);

	PXJavaParameterList(pxCodeDocumentElement, pxFile);

	PXFileWriteC(pxFile, ';');
	PXFileWriteNewLine(pxFile);

}

void PXAPI PXJavaDefinitionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	// readonly
	if(pxCodeDocumentElement->DataType & PXDataTypeReadOnly)
	{
		PXFileWriteA(pxFile, PXJavaKeyWordReadOnly, 5);
		PXFileWriteC(pxFile, ' ');
	}

	// type
	PXFileWriteA(pxFile, "***TYPE***", 10);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
}

void PXAPI PXJavaParameterList(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteC(pxFile, '(');

	for(PXCodeDocumentElement* i = pxCodeDocumentElement->ElementSibling ; i ; i = pxCodeDocumentElement->ElementSibling)
	{
		PXJavaDefinitionWrite(pxCodeDocumentElement, pxFile);

		if(pxCodeDocumentElement->ElementSibling) // is not last entry
		{
			PXFileWriteC(pxFile, ',');
		}
	}

	PXFileWriteC(pxFile, ')');
}

void PXAPI PXJavaElementWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXCodeDocumentElement* pxCodeDocumentElementSibling = pxCodeDocumentElement;

	for(size_t i = 0; pxCodeDocumentElementSibling; ++i)
	{
		switch(pxCodeDocumentElementSibling->Type)
		{
			case PXDocumentElementTypeInclude:
			{
				PXJavaIncludeWrite(pxCodeDocumentElementSibling, pxFile);
				break;
			}
			case PXDocumentElementTypeEnum:
			case PXDocumentElementTypeClass:
			{
				PXJavaContainerWrite(pxCodeDocumentElementSibling, pxFile);
				break;
			}
			case PXDocumentElementTypeFunction:
			{
				PXJavaFunctionWrite(pxCodeDocumentElementSibling, pxFile);
				break;
			}

			default:
				break;
		}

		pxCodeDocumentElementSibling = pxCodeDocumentElementSibling->ElementSibling;
	}
}

PXActionResult PXAPI PXJavaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXJavaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
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

	PXCodeDocument* const pxCodeDocument = (PXCodeDocument*)pxResourceSaveInfo->Target;

	PXCodeDocumentElement* const pxCodeDocumentCurrent = &pxCodeDocument->ElementList[0];
	
	PXJavaElementWrite(pxCodeDocumentCurrent, pxFile);

    return PXActionRefusedNotImplemented;
}
