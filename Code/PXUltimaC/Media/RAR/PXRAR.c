#include "PXRAR.h"

const char PXRARSignature[] = { 'R', 'a', 'r', '!', 0x1A ,0x07 };

PXActionResult PXAPI PXRARLoadFromFile(PXFile* const pxUncompressedFile, PXFile* const pxCompressedFile)
{
	PXRAR pxRAR;
	PXClear(PXRAR, &pxRAR);

	// Read Signature
	{
		const PXBool isSignature = PXFileReadAndCompare(pxCompressedFile, PXRARSignature, sizeof(PXRARSignature));

		if (!isSignature)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}

		char checkA = 0;

		PXFileReadB(pxCompressedFile, &checkA, 1u);

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

				const PXBool isSignatureB = PXFileReadAndCompare(pxCompressedFile, checkB, 2u);

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

PXActionResult PXAPI PXRARSaveToFile(const PXFile* const pxUncompressedFile, PXFile* const pxCompressedFile)
{
	return PXActionRefusedNotImplemented;
}