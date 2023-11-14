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

	typedef void (PXAPI*PXLogPrintFunction)(const PXLoggingType loggingType, const char* const source, ...);

	PXPublic void PXAPI PXConsoleClear();
	PXPublic void PXAPI PXConsoleGoToXY(const PXInt32U x, const PXInt32U y);

	//PXPublic void PXAPI PXConsoleTranlateColorsA(char* const bufferInput, char* const bufferOuput);
	PXPublic void PXAPI PXConsoleTranlateColors(PXText* const bufferInput, PXText* const bufferOuput);

	PXPublic void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const format, ...);

	PXPublic void PXAPI PXLogPrintString(const char* const source, PXSize length);
	PXPublic void PXAPI PXLogPrintStringLine(const char* const source, PXSize length);

#ifdef __cplusplus
}
#endif

#endif