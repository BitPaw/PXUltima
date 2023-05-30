#ifndef PXXingInfoINCLUDE
#define PXXingInfoINCLUDE

#include <OS/File/PXFile.h>

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
		unsigned int NumberOfFrames;
		unsigned int SizeInBytes;
		char TOCBuffer[PXXingInfoTOCBufferSize];
		unsigned int VBRScale; // 0-100
	}
	PXXingInfo;

	PXPublic PXActionResult PXXingInfoParse(PXXingInfo* const xingInfo, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif