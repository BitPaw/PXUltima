#include "PXXingInfo.h"

static const char PXXingInfoSignatureInfo[4] = { 'I', 'n', 'f', 'o' };
static const char PXXingInfoSignatureXing[4] = { 'X', 'i', 'n', 'g' };

PXActionResult PXXingInfoParse(PXXingInfo* const xingInfo, PXFile* const PXFile)
{
	// parse signature
	{
		char indetifier[4];

		PXFileReadB(PXFile, indetifier, 4u);

		const PXBool isInfo = PXMemoryCompare(PXXingInfoSignatureInfo, 4, indetifier, 4);
		const PXBool isXing = PXMemoryCompare(PXXingInfoSignatureXing, 4, indetifier, 4);

		const PXBool validHeader = isInfo || isXing;

		if(!validHeader)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}

		if(isInfo)
		{
			xingInfo->Version = PXXingInfoVersionInfo;
		}

		if(isXing)
		{
			xingInfo->Version = PXXingInfoVersionXing;
		}
	}

	unsigned int flags = 0;

	PXFileReadI32UE(PXFile, &flags, PXEndianBig);

	const PXBool hasNumberOfFrames = flags & 0b00000001;
	const PXBool hasSizeInBytes = (flags & 0b00000010) >> 1;
	const PXBool hasTOCData = (flags & 0b00000100) >> 2;
	const PXBool hasVBRScale = (flags & 0b00001000) >> 3;

	// (0x0001) if set, then read one 32 bit integer in Big PXEndian.
	// It represents the total number of frames in the Audio file.
	if(hasNumberOfFrames)
	{
		PXFileReadI32UE(PXFile, &xingInfo->NumberOfFrames, PXEndianBig);
	}

	// (0x0002) is set, then read one 32 bit integer in Big PXEndian.
	// It represents the total number of bytes of MPEG Audio in the file.
	// This does not include the PXID3 tag, however, it includes this very tag.
	if(hasSizeInBytes)
	{
		PXFileReadI32UE(PXFile, &xingInfo->SizeInBytes, PXEndianBig);
	}

	if(hasTOCData)
	{
		PXFileReadB(PXFile, &xingInfo->TOCBuffer, PXXingInfoTOCBufferSize);
	}

	if(hasVBRScale)
	{
		PXFileReadI32UE(PXFile, &xingInfo->VBRScale, PXEndianBig);
	}

	return PXActionSuccessful;
}