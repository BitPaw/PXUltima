#ifndef PXLogINCLUDE
#define PXLogINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXLoggingType_
	{
		PXLoggingTypeInvalid,
		PXLoggingInfo,
		PXLoggingWarning,
		PXLoggingQuestion,
		PXLoggingError,
		PXLoggingFailure,
		PXLoggingAllocation,
		PXLoggingReallocation,
		PXLoggingDeallocation
	}
	PXLoggingType;

	PXPublic void (PXAPI*PXLogPrintFunction)(const PXLoggingType loggingType, const char* const source, ...);

	PXPublic void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const format, ...);

	PXPublic void PXAPI PXLogPrintString(const char* const source, PXSize length);
	PXPublic void PXAPI PXLogPrintStringLine(const char* const source, PXSize length);

#ifdef __cplusplus
}
#endif

#endif