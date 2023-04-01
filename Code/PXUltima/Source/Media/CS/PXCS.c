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

void PXCSSerialize(PXDataStream* const inputSteam, PXDataStream* const outputStream)
{
	while (!PXDataStreamIsAtEnd(inputSteam))
	{
		PXCSKeyWord keyword = PXCSFetchNext(inputSteam);

		switch (keyword)
		{
			case PXCSKeyWordNameSpace:
			{
				PXDataStreamWriteB(outputStream, "namespace", 9);
				PXDataStreamWriteB(outputStream, "namespace", 9);
				PXDataStreamWriteB(outputStream, "{", 1);
			}

			default:
				break;
		}
	}
}

void PXCSGenerateVariableNameFromCSource(const CKeyWord keyword, const PXInt8U variableInfoFlags, char* const text)
{
	PXDataStream textSteam;

	PXDataStreamFromExternal(&textSteam, text, 64);




}

void PXCSSerializeStruct(PXDataStream* const inputSteam, PXDataStream* const outputStream, char* name, int size, int amount)
{
	const PXBool isUnnamed = !((name) && (size > 0));

	if (!isUnnamed)
	{
		PXDataStreamWriteB(outputStream, "\t[StructLayout(LayoutKind.Explicit)]\n", 37);
		PXDataStreamWriteB(outputStream, "\tpublic struct ", 15);
		PXDataStreamWriteB(outputStream, name, size);
		PXDataStreamWriteB(outputStream, "\n\t{\n", 4);
	}

	for (PXSize i = 0; i < amount; ++i)
	{
		PXInt8U variableNameSize = 0;
		char variableName[64];

		PXInt8U variableTypeNameSize = 0;
		char variableTypeName[64];

		PXInt16U amountOfValues = 0;

		PXInt8U variableInfoFlags;
		CKeyWord type;

		{
			PXInt8U value = 0;

			PXDataStreamReadI8U(inputSteam, &value);

			type = (CKeyWord)value;
		}

	
		//PXCSFetchNext(inputSteam);

		switch (type)
		{
			case CKeyWordTypeDefinition:
			{
				break;
			}
			case CKeyWordEnum:
			{
				break;
			}
			case CKeyWordStruct:
			{
				break;
			}
			case CKeyWordUnion:
			{
				PXInt8U nameSize = 0;
				char name[64];

				PXInt8U nameAliasSize = 0;
				char nameAlias[64];
			
				PXDataStreamReadI8U(inputSteam, &nameSize);
				PXDataStreamReadB(inputSteam, name, nameSize);
			
				PXDataStreamReadI8U(inputSteam, &nameAliasSize);
				PXDataStreamReadB(inputSteam, nameAlias, nameAliasSize);

				PXDataStreamReadI16U(inputSteam, &amountOfValues);

				PXCSSerializeStruct(inputSteam, outputStream, nameAlias, nameAliasSize, amountOfValues);

				break;
			}

			default:
			{

				PXDataStreamWriteB(outputStream, "\t\t[FieldOffset(0)] public ", 26);

				variableInfoFlags = type;

				//PXDataStreamReadI8U(inputSteam, &variableInfoFlags);


				//----------
					// const [ignored] 
// const type [ignored]
// Reguster [ignored]
// restricted [ignored]

				const PXBool isUnsigned = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsSigned);
				const PXBool isAdress = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsAdress);
				const PXBool isVolatile = PXFlagIsSet(variableInfoFlags, MemberFieldFlagVolatile);
				const PXBool isPrimitive = PXFlagIsSet(variableInfoFlags, MemberFieldFlagIsKnownPrimitive);


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

				if (isPrimitive)
				{
					// Get Type
					PXInt8U keyID = 0;

					PXDataStreamReadI8U(inputSteam, &keyID);

					type = (PXInt8U)keyID;
				}
				else
				{
					PXDataStreamReadI8U(inputSteam, &variableTypeNameSize);
					PXDataStreamReadB(inputSteam, variableTypeName, variableTypeNameSize);
				}

				PXDataStreamReadI8U(inputSteam, &variableNameSize);
				PXDataStreamReadB(inputSteam, variableName, variableNameSize);

				if (isVolatile)
				{
					PXDataStreamWriteB(outputStream, volatileText, sizeof(volatileText) - 1);
				}

				PXInt8U sizeofKey = 0;
				const char* keyText = 0;

				if (isPrimitive)
				{
					if (isUnsigned)
					{
						switch (type)
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
						switch (type)
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

					switch (type)
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

					if (isAdress)
					{
						PXDataStreamWriteB(outputStream, "*", 1);
					}

					PXDataStreamWriteB(outputStream, " ", 1);
				}
				else
				{
					// Primitiv

					PXDataStreamWriteI8U(outputStream, variableTypeNameSize);
					PXDataStreamWriteB(outputStream, variableTypeName, variableTypeNameSize);
					PXDataStreamWriteB(outputStream, " ", 1);
				}
				//------------

				// Variable Name
				PXDataStreamWriteI8U(outputStream, variableNameSize);
				PXDataStreamWriteB(outputStream, variableName, variableNameSize);

				PXDataStreamWriteB(outputStream, ";\n", 2u);


				break;
			}

			
		}

	}

	PXDataStreamWriteB(outputStream, "\n\t}\n", 4);
}

void PXCSCreateWrapperFromCSource(PXDataStream* const inputSteam, PXDataStream* const outputStream)
{
	PXDataStreamWriteB(outputStream, "using System;\n", 14);
	PXDataStreamWriteB(outputStream, "using System.Runtime.InteropServices;\n\n", 39);

	PXDataStreamWriteB(outputStream, "namespace PX.Wrapper\n{\n", 23);


	while (!PXDataStreamIsAtEnd(inputSteam))
	{
		CKeyWord keyWord = CKeyWordInvalid;

		{
			PXInt8U keyID = 0;

			PXDataStreamReadI8U(inputSteam, &keyID);

			keyWord = (PXInt8U)keyID;
		}

		switch (keyWord)
		{
			case CKeyWordTypeDefinition:
			{
				PXInt16U amountOfDataValues = 0;

				PXInt8U enumNameBufferLength = 0;
				char enumNameBuffer[64];

				PXInt8U enumAliasBufferLength = 0;
				char enumAliasBuffer[64];


				MemorySet(enumNameBufferLength, 0, sizeof(enumNameBuffer));
				MemorySet(enumAliasBuffer, 0, sizeof(enumNameBuffer));

				{
					PXInt8U keyID = 0;

					PXDataStreamReadI8U(inputSteam, &keyID);

					keyWord = (PXInt8U)keyID;
				}			

				// Fake name
				PXDataStreamReadI8U(inputSteam, &enumNameBufferLength);
				PXDataStreamReadB(inputSteam, enumNameBuffer, enumNameBufferLength);

				// Alias
				PXDataStreamReadI8U(inputSteam, &enumAliasBufferLength);
				PXDataStreamReadB(inputSteam, enumAliasBuffer, enumAliasBufferLength);		

				PXDataStreamReadI16U(inputSteam, &amountOfDataValues);

				switch (keyWord)
				{
					case CKeyWordEnum:
					{
						PXDataStreamWriteB(outputStream, "\tpublic enum ", 13);
						PXDataStreamWriteB(outputStream, enumAliasBuffer, enumAliasBufferLength);
						PXDataStreamWriteB(outputStream, "\n\t{\n", 4);

						for (PXSize i = 0; i < amountOfDataValues; ++i)
						{					
							PXInt8U enumElementBufferLength = 0;
							char enumElementBuffer[64];

							PXDataStreamReadI8U(inputSteam, &enumElementBufferLength);
							PXDataStreamReadB(inputSteam, enumElementBuffer, enumElementBufferLength);

							// if the name contains the same name as the enum, remove it

							PXBool minLength = MathMinimumIU(enumAliasBufferLength, enumElementBufferLength);
							PXBool isSame = PXTextCompareA(enumAliasBuffer, minLength, enumElementBuffer, minLength);
							PXSize offset = 0;

							if (isSame)
							{
								offset += enumAliasBufferLength;
							}

							PXDataStreamWriteB(outputStream, "\t\t", 2);
							PXDataStreamWriteB(outputStream, enumElementBuffer+ offset, enumElementBufferLength- offset);

							if (i < (amountOfDataValues-1))
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
					case CKeyWordStruct:
					case CKeyWordUnion:
					{
						PXCSSerializeStruct(inputSteam, outputStream, enumAliasBuffer, enumAliasBufferLength, amountOfDataValues);
						break;
					}

					default:
						break;
				}
			}

			default:
				break;
		}
	
	}

	PXDataStreamWriteB(outputStream, "}", 2);

	outputStream->DataSize = outputStream->DataCursor;
	outputStream->DataCursor = 0;
}