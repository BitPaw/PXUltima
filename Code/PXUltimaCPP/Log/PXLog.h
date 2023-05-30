#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum LoggingType_
	{
		LoggingTypeInvalid,
		LoggingInfo,
		LoggingWarning,
		LoggingQuestion,
		LoggingError,
		LoggingFailure,
		LoggingAllocation,
		LoggingReallocation,
		LoggingDeallocation
	}
	LoggingType;

	PXPublic void PXLogPrintf(const LoggingType loggingType, const char* const source, const char* const input, ...);

	PXPublic void PXLogPrintString(const char* const source, PXSize length);
	PXPublic void PXLogPrintStringLine(const char* const source, PXSize length);

#ifdef __cplusplus
}
#endif

#endif