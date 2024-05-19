#include "PXJava.h"

#include <Media/PXDocument.h>

PXActionResult PXAPI PXJavaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXJavaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
	if(!pxResourceSaveInfo)
	{
		return PXActionRefusedArgumentNull;
	}

	if(PXResourceTypeCodeDocument != pxResourceSaveInfo->Type)
	{
		return PXActionRefusedArgumentInvalid;
	}

	PXCodeDocument* const pxCodeDocument = (PXCodeDocument*)pxResourceSaveInfo->Target;

	PXCodeDocumentElement* const pxCodeDocumentElement = pxCodeDocument->ElementRoot;
	
	while(pxCodeDocumentElement)
	{
		switch(pxCodeDocumentElement->Type)
		{
			case PXDocumentElementTypeClass:
			{
				

				break;
			}

			default:
				break;
		}		
	}



	for(PXSize entryID = 0; entryID < pxCodeDocument->ElementListAmount; ++entryID)
	{
		PXCodeDocumentElement* const pxCodeDocumentElement = &pxCodeDocument->ElementList[entryID];

		

	}
	


    return PXActionRefusedNotImplemented;
}
