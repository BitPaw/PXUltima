#include "PXCS.h"

#include <Media/C/PXC.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

#include <Compiler/PXCompiler.h>
#include <Media/PXDocument.h>


char PXCSharpKeyWordPublic[] = "public";
char PXCSharpKeyWordPrivate[] = "private";
char PXCSharpKeyWordStruct[] = "struct";
char PXCSharpKeyWordClass[] = "class";
char PXCSharpKeyWordEnum[] = "enum";
char PXCSharpKeyWordVoid[] = "void";
char PXCSharpKeyWordExpliciLayout[] = "[StructLayout(LayoutKind.Explicit)]";

//char PXJavaKeyWordByte[] = "byte";
//char PXJavaKeyWordInt[] = "int";
//char PXJavaKeyWordShort[] = "short";
//char PXJavaKeyWordLong[] = "long";
//char PXJavaKeyWordFloat[] = "float";
//char PXJavaKeyWordDouble[] = "double";

#define PXJavaTABSize 2

PXCSKeyWord PXCSFetchNext(PXFile* const inputSteam)
{
	PXCSKeyWord keyWord = PXCSKeyWordInvalid;

	{
		PXInt8U keyID = 0;

		PXFileReadI8U(inputSteam, &keyID);

		keyWord = (PXCSKeyWord)keyID;
	}

	return keyWord;
}

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

void PXAPI PXCSharpComment(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	if(pxCodeDocumentElement->CommentSize > 0)
	{
		PXFileWriteA(pxFile, "// ", 3);
		PXFileWriteA(pxFile, pxCodeDocumentElement->CommentAdress, pxCodeDocumentElement->CommentSize);
		PXFileWriteNewLine(pxFile);
	}
}

void PXAPI PXCSharpIncludeWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteA(pxFile, "// depends on file ", 19);
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
	//PXFileWriteNewLine(pxFile);
}

void PXAPI PXCSharpContainerWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	PXCSharpComment(pxCodeDocumentElement, pxFile);

	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);
	PXFileWriteA(pxFile, PXCSharpKeyWordPublic, sizeof(PXCSharpKeyWordPublic) - 1);
	PXFileWriteC(pxFile, ' ');

	switch(pxCodeDocumentElement->Type)
	{
		case PXDocumentElementTypeEnum:
		{
			PXFileWriteA(pxFile, PXCSharpKeyWordEnum, sizeof(PXCSharpKeyWordEnum) - 1);
			break;
		}
		case PXDocumentElementTypeFile:
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
				//PXJavaDefinitionWrite(child, pxFile);
				PXFileWriteC(pxFile, ';');
				break;
			}
			default:
			//	PXJavaElementWrite(child, pxFile);
				break;
		}

		if(child->ElementSibling)
		{
			PXFileWriteNewLine(pxFile);
		}
	}

	PXFileWriteNewLine(pxFile);
	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);
	PXFileWriteC(pxFile, '}');
	PXFileWriteNewLine(pxFile);
}

void PXAPI PXCSharpFunctionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	// Cleave function name?	

	PXCSharpComment(pxCodeDocumentElement, pxFile);

	PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	//PXFileWriteA(pxFile, PXJavaKeyWordPublic, 6);
	PXFileWriteC(pxFile, ' ');

	if(1) // Is native?
	{
		//PXFileWriteA(pxFile, PXJavaKeyWordNative, 6);
		PXFileWriteC(pxFile, ' ');
	}

	//PXFileWriteA(pxFile, PXJavaKeyWordVoid, 4);
	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameShortAdress, pxCodeDocumentElement->NameShortSize);

//	PXJavaParameterList(pxCodeDocumentElement, pxFile);

	PXFileWriteC(pxFile, ';');
	//PXFileWriteNewLine(pxFile);
}

void PXAPI PXCSharpDefinitionWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXCSharpComment(pxCodeDocumentElement, pxFile);

	//PXFileWriteFill(pxFile, ' ', PXJavaTABSize * pxCodeDocumentElement->Depth);

	// const does not exists in this context. const in C# does mean another thing

	// type
	if(pxCodeDocumentElement->DataTypeIsBuildIn)
	{
		switch(pxCodeDocumentElement->DataType & PXDataTypeSizeMask)
		{
			case PXDataTypeSize08:
				//PXFileWriteA(pxFile, PXJavaKeyWordByte, 4);
				break;

			case PXDataTypeSize16:
				//PXFileWriteA(pxFile, PXJavaKeyWordShort, 5);
				break;

			case PXDataTypeSize32:
			//	PXFileWriteA(pxFile, PXJavaKeyWordInt, 3);
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
		PXFileWriteA(pxFile, pxCodeDocumentElement->TypeNameAdress, pxCodeDocumentElement->TypeNameSize);
	}

	// Adress?


	PXFileWriteC(pxFile, ' ');
	PXFileWriteA(pxFile, pxCodeDocumentElement->NameAdress, pxCodeDocumentElement->NameSize);
}

void PXAPI PXCSharpParameterList(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	PXFileWriteC(pxFile, '(');

	for(PXCodeDocumentElement* i = pxCodeDocumentElement->ElementChildFirstBorn; i; i = i->ElementSibling)
	{
		PXCSharpDefinitionWrite(i, pxFile);
	}

	PXFileWriteC(pxFile, ')');
}

void PXAPI PXCSharpElementWrite(PXCodeDocumentElement* pxCodeDocumentElement, PXFile* pxFile)
{
	switch(pxCodeDocumentElement->Type)
	{
		case PXDocumentElementTypeFile:
		{
			PXCSharpContainerWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeInclude:
		{
			PXCSharpIncludeWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeEnum:
		case PXDocumentElementTypeClass:
		case PXDocumentElementTypeStruct:
		{
			PXCSharpContainerWrite(pxCodeDocumentElement, pxFile);
			break;
		}
		case PXDocumentElementTypeFunction:
		{
			PXCSharpFunctionWrite(pxCodeDocumentElement, pxFile);
			break;
		}

		default:
			break;
	}
}

PXActionResult PXAPI PXCSLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCSSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
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

	PXCSharpElementWrite(pxCodeDocumentCurrent, pxFile);

	return PXActionRefusedNotImplemented;
}