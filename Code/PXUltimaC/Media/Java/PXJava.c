#include "PXJava.h"

#include <Media/PXDocument.h>

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
char PXJavaKeyWordFloat[] = "float";
char PXJavaKeyWordDouble[] = "double";

#define PXJavaTABSize 2

void PXAPI PXJavaComment(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	if(pxCodeDocumentElement->CommentSize > 0)
	{
		PXFileWriteA(pxFile, "// ", 3);
		PXFileWriteA(pxFile, pxCodeDocumentElement->CommentAdress, pxCodeDocumentElement->CommentSize);
		PXFileWriteNewLine(pxFile);
	}
}

void PXAPI PXJavaIncludeWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteA(pxFile, "// depends on file ", 19);
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	//PXFileWriteNewLine(pxFile);
}

void PXAPI PXJavaContainerWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	PXJavaComment(pxCodeDocumentElement, pxFile);

	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);
	PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');

	switch(pxCodeDocumentElement->Type)
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

	if(pxCodeDocumentElement->NameShortAdress)
	{
		PXFileWriteA(pxFile, pxCodeDocumentElement->NameShortAdress, pxCodeDocumentElement->NameShortSize);
	}
	else
	{
		PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	}

	PXFileWriteNewLine(pxFile);
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);
	PXFileWriteC(pxFile, '{');
	PXFileWriteNewLine(pxFile);

	for
	(
		PXCodeDocumentElement* child = pxCodeDocumentElement->ElementChildFirstBorn;
		child; 
		child = child->ElementSibling
	)
	{
		switch(pxCodeDocumentElement->Type)
		{
			case PXDocumentElementTypeEnum:
			{
				PXFileWriteFill(pxFile, ' ', PXJavaTABSize * child->Depth);
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
				PXJavaDefinitionWrite(child, pxFile);
				PXFileWriteC(pxFile, ';');
				break;
			}
			default:
				PXJavaElementWrite(child, pxFile);
				break;
		}

		if(child->ElementSibling)
		{
			PXFileWriteNewLine(pxFile);
		}
	}

	PXFileWriteNewLine(pxFile);
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);
	PXFileWriteC(pxFile,  '}');
	PXFileWriteNewLine(pxFile);
}

void PXAPI PXJavaFunctionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	// Cleave function name?	

	PXJavaComment(pxCodeDocumentElement, pxFile);

	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');

	if(1) // Is native?
	{
		PXFileWriteA(pxFile, PXJavaKeyWordNative, 6);
		PXFileWriteC(pxFile, ' ');
	}

	PXFileWriteA(pxFile, PXJavaKeyWordVoid, 4);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameShortAdress, pxCodeDocumentElement->NameShortSize);

	PXJavaParameterList(pxCodeDocumentElement, pxFile);

	PXFileWriteC(pxFile, ';');
	//PXFileWriteNewLine(pxFile);

}

void PXAPI PXJavaDefinitionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXJavaComment(pxCodeDocumentElement, pxFile);

	//PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	// readonly
	if(pxCodeDocumentElement->DataType & PXDataTypeReadOnly)
	{
		PXFileWriteA(pxFile, PXJavaKeyWordReadOnly, 5);
		PXFileWriteC(pxFile, ' ');
	}

	// type
	if(pxCodeDocumentElement->DataTypeIsBuildIn)
	{
		switch(pxCodeDocumentElement->DataType & PXDataTypeSizeMask)
		{
			case PXDataTypeSize08:
				PXFileWriteA(pxFile, PXJavaKeyWordByte, 4);
				break;

			case PXDataTypeSize16:
				PXFileWriteA(pxFile, PXJavaKeyWordShort, 5);
				break;

			case PXDataTypeSize32:
				PXFileWriteA(pxFile, PXJavaKeyWordInt, 3);
				break;

			case PXDataTypeSize64:
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
		PXFileWriteA(pxFile, pxCodeDocumentElement->TypeNameAdress, pxCodeDocumentElement->TypeNameSize);
	}

	// Adress?


	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
}

void PXAPI PXJavaParameterList(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteC(pxFile, '(');

	for(PXCodeDocumentElement* i = pxCodeDocumentElement->ElementChildFirstBorn ; i ; i = i->ElementSibling)
	{
		PXJavaDefinitionWrite(i, pxFile);
	}

	PXFileWriteC(pxFile, ')');
}

void PXAPI PXJavaElementWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	if(!pxCodeDocumentElement)
	{
		return;
	}

#if 1
	switch(pxCodeDocumentElement->Type)
	{
		case PXDocumentElementTypeFile:
		{
			PXJavaContainerWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeInclude:
		{
			PXJavaIncludeWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeEnum:
		case PXDocumentElementTypeClass:
		case PXDocumentElementTypeStruct:
		{
			PXJavaContainerWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeFunction:
		{
			PXJavaFunctionWrite(pxCodeDocumentElement, pxFile);
			break;
		}

		default:
			break;
	}
#else
	PXCodeDocumentElement* sibling = pxCodeDocumentElement;

	for(size_t i = 0; sibling; ++i)
	{
		switch(sibling->Type)
		{
			case PXDocumentElementTypeFile:
			{
				PXJavaContainerWrite(sibling, pxFile);
				break;
			}
			case PXDocumentElementTypeInclude:
			{
				PXJavaIncludeWrite(sibling, pxFile);
				break;
			}
			case PXDocumentElementTypeEnum:
			case PXDocumentElementTypeClass:
			case PXDocumentElementTypeStruct:
			{
				PXJavaContainerWrite(sibling, pxFile);
				break;
			}
			case PXDocumentElementTypeFunction:
			{
				PXJavaFunctionWrite(sibling, pxFile);
				break;
			}
	

			default:
				break;
		}

		if(sibling->ElementChildFirstBorn)
		{
			PXJavaElementWrite(sibling->ElementChildFirstBorn, pxFile);
		}

		sibling = sibling->ElementSibling;
	}
#endif
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
	
	PXTime pxTime;
	PXTimeNow(&pxTime);

	PXFileWriteAF
	(
		pxFile, 
		"// This file is generated by PXUltima\n"
		"// Date: %02i.%02i.%04i\n"
		"// Time: %02i:%02i:%02i\n\n",
		(int)pxTime.Day, 
		(int)pxTime.Month,
		(int)pxTime.Year,
		(int)pxTime.Hour,
		(int)pxTime.Minute,
		(int)pxTime.Second
	);

	PXJavaElementWrite(pxCodeDocumentCurrent, pxFile);

    return PXActionRefusedNotImplemented;
}
