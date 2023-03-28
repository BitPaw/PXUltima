#include "XingInfo.h"

#include <File/PXDataStream.h>

#define XingInfoSignatureInfo { 'I', 'n', 'f', 'o' }
#define XingInfoSignatureXing { 'X', 'i', 'n', 'g' }

PXActionResult XingInfoParse(XingInfo* const xingInfo, PXDataStream* const pxDataStream)
{
	// parse signature
	{
		const char infoTag[] = XingInfoSignatureInfo;
		const char xingTag[] = XingInfoSignatureXing;
		char indetifier[4];

		PXDataStreamReadB(pxDataStream, indetifier, 4u);

		const PXBool isInfo =
			indetifier[0] == infoTag[0] &&
			indetifier[1] == infoTag[1] &&
			indetifier[2] == infoTag[2] &&
			indetifier[3] == infoTag[3];

		const PXBool isXing =
			indetifier[0] == xingTag[0] &&
			indetifier[1] == xingTag[1] &&
			indetifier[2] == xingTag[2] &&
			indetifier[3] == xingTag[3];

		const PXBool validHeader = isInfo || isXing;

		if(!validHeader)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}

		if(isInfo)
		{
			xingInfo->Version = XingInfoVersionInfo;
		}

		if(isXing)
		{
			xingInfo->Version = XingInfoVersionXing;
		}
	}

	unsigned int flags = 0;

	PXDataStreamReadI32UE(pxDataStream, &flags, EndianBig);

	const PXBool hasNumberOfFrames = flags & 0b00000001;
	const PXBool hasSizeInBytes = (flags & 0b00000010) >> 1;
	const PXBool hasTOCData = (flags & 0b00000100) >> 2;
	const PXBool hasVBRScale = (flags & 0b00001000) >> 3;

	// (0x0001) if set, then read one 32 bit integer in Big Endian.
	// It represents the total number of frames in the Audio file.
	if(hasNumberOfFrames)
	{
		PXDataStreamReadI32UE(pxDataStream, &xingInfo->NumberOfFrames, EndianBig);
	}

	// (0x0002) is set, then read one 32 bit integer in Big Endian.
	// It represents the total number of bytes of MPEG Audio in the file.
	// This does not include the ID3 tag, however, it includes this very tag.
	if(hasSizeInBytes)
	{
		PXDataStreamReadI32UE(pxDataStream, &xingInfo->SizeInBytes, EndianBig);
	}

	if(hasTOCData)
	{
		PXDataStreamReadB(pxDataStream, &xingInfo->TOCBuffer, XingInfoTOCBufferSize);
	}

	if(hasVBRScale)
	{
		PXDataStreamReadI32UE(pxDataStream, &xingInfo->VBRScale, EndianBig);
	}

	return PXActionSuccessful;
}