#ifndef XingInfoInclude
#define XingInfoInclude

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

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

	PXPublic PXActionResult XingInfoParse(XingInfo* const xingInfo, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif