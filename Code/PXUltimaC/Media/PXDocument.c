#include "PXDocument.h"

#include <OS/Console/PXConsole.h>

PXActionResult PXAPI PXDocumentElementRoot(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementChildGet(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementSiblingGet(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	return PXActionSuccessful;
}

PXActionResult PXAPI PXDocumentElementAttributeAdd(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{

		// This is not an attribute but a member.
		// We need to get the parent object and use that name to merge it into a data entry


		PXFileCursorMoveTo(&pxDocument->Data, pxDocument->LastEntryOffset); // Move to prev entry

		PXDocumentElement pxDocumentElementClass;
		PXDocumentElement pxDocumentElementMemberName;

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
			}
#endif
		}
	

	PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileWriteMultible); // Write change

	PXDocumentPrintNode(pxDocumentElement);

	return PXActionSuccessful;
}

PXSize PXAPI PXDocumentElementIO(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement, PXFileIOMultibleFunction pxFileIOMultibleFunction)
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

PXActionResult PXAPI PXDocumentElementWrite(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	const PXBool isNotRoot = pxDocumentElement->Depth == (pxDocument->LastEntryDepth + 1);
		
	PXSize parrentPosition = 0;

	if (isNotRoot)
	{
		// Update current Entry

		PXFileCursorMoveTo(&pxDocument->Data, pxDocument->LastEntryOffset); // Move to prev entry

		PXDocumentElement pxDocumentElementTemp;

		const PXSize readBytes = PXDocumentElementIO(pxDocument, &pxDocumentElementTemp, PXFileReadMultible); // Read parent entry

		PXFileCursorRewind(&pxDocument->Data, readBytes);

		++(pxDocumentElementTemp.ElementChildrenAmount); // Update amount of sub-objects

		parrentPosition = pxDocument->Data.DataCursor;

		PXDocumentElementIO(pxDocument, &pxDocumentElementTemp, PXFileWriteMultible); // Write change		

#if 1
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

	++(pxDocument->AmountOfEntrys);
	pxDocumentElement->ParaentDataPosition = parrentPosition;
	pxDocument->LastEntryOffset = pxDocument->Data.DataCursor;
	pxDocument->LastEntryDepth = pxDocumentElement->Depth;

	PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileWriteMultible); // Write node

#if 1
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

PXActionResult PXAPI PXDocumentElementRead(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	return PXDocumentElementIO(pxDocument, pxDocumentElement, PXFileReadMultible);;
}

PXActionResult PXAPI PXDocumentElementAdd(PXDocument* const pxDocument, PXDocumentElement* const pxDocumentElement)
{
	switch (pxDocumentElement->Type)
	{
		case PXDocumentElementTypeIncludeGlobal:
		{
			break;
		}
		case PXDocumentElementTypeIncludePath:
		{
			break;
		}
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

PXActionResult PXAPI PXDocumentPrintNode(PXDocumentElement* const pxDocumentElement)
{
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

			sprintf_s(typeBuffer, 64, "bool");
			sprintf_s(valueBuffer, 64, "%s", value ? "true" : "false");
			break;
		}
		case PXDataTypeFloat:
		{
			float value;
			PXCopy(float, &pxDocumentElement->ElementMember.ValueAdress, &value);

			sprintf_s(typeBuffer, 64, "float");
			sprintf_s(valueBuffer, 64, "%-5.2f", value);
			break;
		}
		case PXDataTypeIntS:
		{
			int value;
			PXCopy(int, &pxDocumentElement->ElementMember.ValueAdress, &value);

			sprintf_s(typeBuffer, 64, "int");
			sprintf_s(valueBuffer, 64, "%i", value);
			break;
		}
		case PXDataTypeString:
		{
			sprintf_s(typeBuffer, 64, "string");
			PXTextCopyA(pxDocumentElement->ElementMember.ValueAdress, pxDocumentElement->ElementMember.ValueSize, valueBuffer, 63);
			break;
		}
		default:
			break;
	}

	switch (pxDocumentElement->Type)
	{
		case PXDocumentElementTypeIncludeGlobal:
		{
			break;
		}
		case PXDocumentElementTypeIncludePath:
		{
			break;
		}
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
			sprintf_s(elementType, 64, "class");
			break;
		}
		case PXDocumentElementTypeFunction:
		{
			sprintf_s(elementType, 64, "function");
			break;
		}
		case PXDocumentElementTypeClassMember:
		{
			sprintf_s(elementType, 64, "member");
			break;
		}
		case PXDocumentElementTypeClassAttribute:
		{
			sprintf_s(elementType, 64, "attribute");
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
}

PXActionResult PXAPI PXDocumentPrint(PXDocument* const pxDocument)
{
	PXDocumentElement pxDocumentElement;

	PXDocumentElementIO(pxDocument, &pxDocumentElement, PXFileReadMultible);// Get root

	for (PXSize i = 0; i < pxDocumentElement.ElementChildrenAmount; ++i) // loop over all roots children
	{
		PXDocumentPrintNode(&pxDocumentElement);

		// Get all
		printf("");
	}

	return PXActionSuccessful;
}