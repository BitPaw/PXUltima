#include "PXCS.h"

#include <Media/C/PXC.h>
#include <Text/PXText.h>
#include <Math/PXMath.h>

PXCSKeyWord PXCSFetchNext(PXDataStream* const inputSteam)
{
	PXCSKeyWord keyWord = PXCSKeyWordInvalid;

	{
		PXInt8U keyID = 0;

		PXDataStreamReadI8U(inputSteam, &keyID);

		keyWord = (PXInt8U)keyID;
	}

	return keyWord;
}

void PXCSSerialize(PXDataStream* const inputSteam, PXDataStream* const outputStream, PXCTranslateStruct* const pxCTranslateStruct)
{
	PXCElementExtract(inputSteam, &pxCTranslateStruct->Element);

	switch (pxCTranslateStruct->Element.Type)
	{
		case PXCStructureTypeEnum:
		{
			PXCStructure* const pxCStructure = &pxCTranslateStruct->Element.ElementStructure;

			PXDataStreamWriteB(outputStream, "\tpublic enum ", 13);
			PXDataStreamWriteB(outputStream, pxCStructure->NameAlias, pxCStructure->NameAliasSizeCurrent);
			PXDataStreamWriteB(outputStream, "\n\t{\n", 4);

			for (PXSize i = 0; i < pxCStructure->MemberAmount; ++i)
			{
				PXInt8U enumElementBufferLength = 0;
				char enumElementBuffer[64];

				PXDataStreamReadI8U(inputSteam, &enumElementBufferLength);
				PXDataStreamReadB(inputSteam, enumElementBuffer, enumElementBufferLength);

				// if the name contains the same name as the enum, remove it

				PXBool minLength = MathMinimumIU(pxCStructure->NameAliasSizeCurrent, enumElementBufferLength);
				PXBool isSame = PXTextCompareA(pxCStructure->NameAlias, minLength, enumElementBuffer, minLength);
				PXSize offset = 0;

				if (isSame)
				{
					offset += pxCStructure->NameAliasSizeCurrent;
				}

				PXDataStreamWriteB(outputStream, "\t\t", 2);
				PXDataStreamWriteB(outputStream, enumElementBuffer + offset, enumElementBufferLength - offset);

				if (i < (pxCStructure->MemberAmount - 1))
				{
					PXDataStreamWriteB(outputStream, ",\n", 2);
				}
				else
				{
					PXDataStreamWriteB(outputStream, "\n", 1);
				}
			}

			PXDataStreamWriteB(outputStream, "\t}\n\n", 4);

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

				PXDataStreamWriteB(outputStream, "\t[StructLayout(LayoutKind.Explicit)]\n", 37);
				PXDataStreamWriteB(outputStream, "\tpublic unsafe struct ", 22);
				PXDataStreamWriteB(outputStream, pxCStructure->NameAlias, pxCStructure->NameAliasSizeCurrent);
				PXDataStreamWriteB(outputStream, "\n\t{\n", 4);
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
				PXDataStreamWriteB(outputStream, "\t}\n", 3);
			}


			break;
		}
		case PXCStructureTypeStructElement:
		{
			PXCStructureVariable* const pxCStructureVariable = &pxCTranslateStruct->Element.ElementVariable;

			PXSize offsetTextSize = 0;
			char numberText[64];

			offsetTextSize += PXTextFromIntA(pxCTranslateStruct->StructureOffsetTotal, numberText, 64);

			if (!pxCTranslateStruct->AreInUnion)
			{
				pxCTranslateStruct->StructureOffsetTotal += pxCTranslateStruct->Element.ElementVariable.SizeOfType;
			}
			else
			{
				pxCTranslateStruct->UnionWidthOffset = MathMaximum(pxCTranslateStruct->Element.ElementVariable.SizeOfType, pxCTranslateStruct->UnionWidthOffset);
			}

			PXDataStreamWriteB(outputStream, "\t\t[FieldOffset(", 15);
			PXDataStreamWriteB(outputStream, numberText, offsetTextSize);
			PXDataStreamWriteB(outputStream, ")] public ", 10);

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
				PXDataStreamWriteB(outputStream, volatileText, sizeof(volatileText) - 1);
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

				PXDataStreamWriteB(outputStream, keyText, sizeofKey);

				if (pxCStructureVariable->IsAdress)
				{
					PXDataStreamWriteB(outputStream, "*", 1);
				}

				PXDataStreamWriteB(outputStream, " ", 1);
			}
			else
			{
				// Primitiv
				PXDataStreamWriteB(outputStream, pxCStructureVariable->NameOfType, pxCStructureVariable->NameOfTypeSizeCurrent);
				PXDataStreamWriteB(outputStream, " ", 1);
			}
			//------------

			// Variable Name
			PXDataStreamWriteB(outputStream, pxCTranslateStruct->Element.Name, pxCTranslateStruct->Element.NameSizeCurrent);
			PXDataStreamWriteB(outputStream, ";\n", 2u);

			break;
		}
		case PXCStructureTypeFuntion:
		{
			PXCFunction* const pxCFunction = &pxCTranslateStruct->Element.ElementFunction;

			// Function

			PXDataStreamWriteB(outputStream, "\t[DllImport(\"PXUltima.dll\")]\n\tprivate static unsafe extern \n", 60);
			PXDataStreamWriteB(outputStream, pxCTranslateStruct->Element.Name, pxCTranslateStruct->Element.NameSizeCurrent);
			PXDataStreamWriteB(outputStream, "(", 1);

			for (PXSize i = 0; i < pxCFunction->ParameterListSize; ++i)
			{
				
			}
		
			PXDataStreamWriteB(outputStream, ");\n", 3);
			break;
		}
		default:
			break;
	}
}

void PXCSCreateWrapperFromCSource(PXDataStream* const inputSteam, PXDataStream* const outputStream)
{
	PXDataStreamWriteB(outputStream, "using System;\n", 14);
	PXDataStreamWriteB(outputStream, "using System.Runtime.InteropServices;\n\n", 39);

	PXDataStreamWriteB(outputStream, "namespace PX.Wrapper\n{\n", 23);

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

	while (!PXDataStreamIsAtEnd(inputSteam))
	{
		PXCSSerialize(inputSteam, outputStream, &pxCTranslateStruct);
	}

	PXDataStreamWriteB(outputStream, "}", 1);

	outputStream->DataSize = outputStream->DataCursor;
	outputStream->DataCursor = 0;
}