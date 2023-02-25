#ifndef PXLibraryINCLUDE
#define PXLibraryINCLUDE

#include <Format/Type.h>

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

#ifdef __cplusplus
}
#endif

#endif