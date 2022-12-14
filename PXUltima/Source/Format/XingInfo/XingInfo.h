#ifndef XingInfoInclude
#define XingInfoInclude

#include <Format/Type.h>

#include <Error/PXActionResult.h>

#define XingInfoTOCBufferSize 100u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum XingInfoVersion_
	{
		XingInfoVersionInvalid,
		XingInfoVersionInfo,
		XingInfoVersionXing
	}
	XingInfoVersion;

	typedef struct XingInfo_
	{
		XingInfoVersion Version;
		unsigned int NumberOfFrames;
		unsigned int SizeInBytes;
		char TOCBuffer[XingInfoTOCBufferSize];
		unsigned int VBRScale; // 0-100
	}
	XingInfo;

	PXPublic PXActionResult XingInfoParse(XingInfo* xingInfo, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif