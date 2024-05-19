#include "PXDocument.h"

#include <OS/Console/PXConsole.h>

const char* PXAPI PXDocumentElementTypeToString(const PXDocumentElementType pxDocumentElementType)
{
	switch(pxDocumentElementType)
	{
		case PXDocumentElementTypeInclude: return "include";
		case PXDocumentElementTypeNamespace: return "namespace";
		case PXDocumentElementTypeStruct: return "struct";
		case PXDocumentElementTypeUnion: return "union";
		case PXDocumentElementTypeEnum: return "enum";
		case PXDocumentElementTypeClass: return "class";
		case PXDocumentElementTypeFunction: return "function";
		case PXDocumentElementTypeClassMember: return "class-member";
		case PXDocumentElementTypeClassAttribute: return "attribute";
		case PXDocumentElementTypeEnumMember: return "enum-member";
		case PXDocumentElementTypeFunctionParameter: return "fuction-parameter";

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

PXActionResult PXAPI PXDocumentElementAttributeAdd(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{

#if PXLogEnable
	const char* typeName = PXDocumentElementTypeToString(pxDocumentElement->Type);
	
	char nameBuffer[64];
	PXMemoryClear(nameBuffer, 64);
	PXTextCopyA(pxDocumentElement->NameAdress, pxDocumentElement->NameSize, nameBuffer, 64);

	switch(pxDocumentElement->Type)
	{
		case PXDocumentElementTypeInclude:
		{
			PXLogPrint
			(
				PXLoggingInfo,
				"CodeDocument",
				"Entry-Add",
				"\n"
				"%10s : %s\n"
				"%10s : %s\n"
				"%10s : %s\n",
				"Type", typeName,
				"Name", nameBuffer,
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
				"Entry-Add",
				"\n"
				"%10s : %s\n"
				"%10s : %s\n"
				"%10s : %s\n"
				"%10s : %i",
				"Type", typeName,
				"Name", nameBuffer,
				"Alias", aliasBuffer,
				"Members", pxDocumentElement->MemberAmount
			);

			break;
		}
		case PXDocumentElementTypeClassAttribute:
		case PXDocumentElementTypeClassMember:
		case PXDocumentElementTypeFunctionParameter:
		case PXDocumentElementTypeEnumMember:
		{
			PXLogPrint
			(
				PXLoggingInfo,
				"CodeDocument",
				"Entry-Add",
				"\n"
				"%10s : %s\n"
				"%10s : %s\n",
				"Type", typeName,
				"Name", nameBuffer
			);

			break;
		}
		case PXDocumentElementTypeFunction:
		{
			PXLogPrint
			(
				PXLoggingInfo,
				"CodeDocument",
				"Entry-Add",
				"\n"
				"%10s : %s\n"
				"%10s : %s\n"
				"%10s : %s\n",
				"Type", typeName,
				"Name", nameBuffer,
				"Call", "stst"
			);

			break;
		}
		default:
			break;
	}


#endif



	return 0;









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

	return PXActionSuccessful;
}

PXSize PXAPI PXDocumentElementIO(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction)
{
	const PXInt8U symbolID = pxDocumentElement->Type;
	PXInt32U amount = 0;

	PXFileDataElementType pxFileDataElementTypeList[] =
	{
		//&pxDocumentElement->ID, PXDataTypeSize,
		&pxDocumentElement->Depth, PXDataTypeSize,
		&pxDocumentElement->NameSize, PXDataTypeSize,
		&pxDocumentElement->NameAdress, PXDataTypeAdress,
		&pxDocumentElement->AttribtesAmount, PXDataTypeSize,
		&pxDocumentElement->MemberAmount, PXDataTypeSize,
		&pxDocumentElement->ElementChildrenAmount, PXDataTypeSize,
		&pxDocumentElement->ElementSiblingsAmount, PXDataTypeSize,
		&pxDocumentElement->ParaentDataPosition, PXDataTypeSize,
		&symbolID, PXDataTypeInt08U,
		&amount, PXDataTypeInt32U
	};

	const PXSize bytesTransphered = pxFileIOMultibleFunction(&pxDocument->Data, pxFileDataElementTypeList, sizeof(pxFileDataElementTypeList));
	
	return bytesTransphered;
}

PXActionResult PXAPI PXDocumentElementWrite(PXCodeDocument* const pxDocument, PXCodeDocumentElement* const pxDocumentElement)
{
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

	++(pxDocument->ElementListAmount);
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
			PXDocumentElementAttributeAdd(pxDocument, pxDocumentElement);
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
		case PXDataTypeBool:
		{
			PXBool value;
			PXCopy(PXBool, &pxDocumentElement->ElementMember.ValueAdress, &value);

			PXTextPrintA(typeBuffer, 64, "bool");
			PXTextPrintA(valueBuffer, 64, "%s", value ? "true" : "false");
			break;
		}
		case PXDataTypeFloat:
		{
			float value;
			PXCopy(float, &pxDocumentElement->ElementMember.ValueAdress, &value);

			PXTextPrintA(typeBuffer, 64, "float");
			PXTextPrintA(valueBuffer, 64, "%-5.2f", value);
			break;
		}
		case PXDataTypeIntS:
		{
			int value;
			PXCopy(int, &pxDocumentElement->ElementMember.ValueAdress, &value);

			PXTextPrintA(typeBuffer, 64, "int");
			PXTextPrintA(valueBuffer, 64, "%i", value);
			break;
		}
		case PXDataTypeString:
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
	PXCodeDocumentElement pxDocumentElement;

	PXDocumentElementIO(pxDocument, &pxDocumentElement, PXFileReadMultible);// Get root

	for (PXSize i = 0; i < pxDocumentElement.ElementChildrenAmount; ++i) // loop over all roots children
	{
		PXDocumentPrintNode(&pxDocumentElement);

		// Get all
		//printf("");
	}

	return PXActionSuccessful;
}