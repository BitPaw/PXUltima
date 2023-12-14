#ifndef PXXingInfoINCLUDE
#define PXXingInfoINCLUDE

#include "../PXResource.h"

#define PXXingInfoTOCBufferSize 100u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXXingInfoVersion_
	{
		PXXingInfoVersionInvalid,
		PXXingInfoVersionInfo,
		PXXingInfoVersionXing
	}
	PXXingInfoVersion;

	typedef struct PXXingInfo_
	{
		PXXingInfoVersion Version;
		PXInt32U NumberOfFrames;
		PXInt32U SizeInBytes;
		char TOCBuffer[PXXingInfoTOCBufferSize];
		PXInt32U VBRScale; // 0-100
	}
	PXXingInfo;

	PXPublic PXActionResult PXXingInfoParse(PXXingInfo* const xingInfo, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif