#include "PXRAR.h"

#include <OS/File/PXFile.h>

const char PXRARSignature[] = { 'R', 'a', 'r', '!', 0x1A ,0x07 };

PXActionResult PXAPI PXRARLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
	PXRAR pxRAR;
	PXClear(PXRAR, &pxRAR);

	// Read Signature
	{
		const PXBool isSignature = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXRARSignature, sizeof(PXRARSignature));

		if (!isSignature)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}

		char checkA = 0;

		PXFileReadB(pxResourceLoadInfo->FileReference, &checkA, 1u);

		switch (checkA)
		{
			case 0: // OK
			{
				pxRAR.Version = PXRARVersion1x5;
				break;
			}			
			case 1:
			{
				const char checkB[2] = { 1,0 };

				const PXBool isSignatureB = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, checkB, 2u);

				if (!isSignatureB)
				{
					return PXActionRefusedInvalidHeaderSignature;
				}

				pxRAR.Version = PXRARVersion5x0;

				break;
			}

			default:
				return PXActionRefusedInvalidHeaderSignature;
		}
	}
	
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXRARSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
	return PXActionRefusedNotImplemented;
}