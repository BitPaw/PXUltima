#ifndef PXLogINCLUDE
#define PXLogINCLUDE

#include <Media/PXResource.h>

#define PXLogEnable 1
#define PXLogEnableColor 0

//PXBool PXLogEnablePrint = 1;
//PXBool PXLogEnableColor = 1;

typedef enum PXLoggingTypeTarget_
{
	PXLoggingTypeTargetInvalid,
	PXLoggingTypeTargetMemory,
	PXLoggingTypeTargetFile,
}
PXLoggingTypeTarget;

typedef enum PXLoggingType_
{
	PXLoggingTypeInvalid,
	PXLoggingInfo,
	PXLoggingEvent,
	PXLoggingWarning,
	PXLoggingQuestion,
	PXLoggingError,
	PXLoggingFailure,
	PXLoggingAllocation,
	PXLoggingReallocation,
	PXLoggingDeallocation
}
PXLoggingType;

typedef struct PXLoggingMemoryData_
{
	PXSize TypeSize;
	PXSize Amount;

	const char* NameFile;
	const char* NameFunction;
	PXSize NumberLine;
}
PXLoggingMemoryData;

typedef struct PXLoggingEventData_
{
	union
	{
		PXSize Time;
		PXFile* FileReference;
		PXLoggingMemoryData MemoryData;
	};

	const char* ModuleSource;
	const char* ModuleAction;
	const char* PrintFormat;

	char Symbol;
	PXLoggingType Type;
	PXLoggingTypeTarget Target;
}
PXLoggingEventData;

#define PXLoggingEventDataConstructSize(obj, type, size, moduleSource, moduleAction, printFormat, ...) \
		obj->Type = type; \
		obj->Size = size; \
		obj->ModuleSource = moduleSource; \
		obj->ModuleAction = moduleAction; \
		obj->PrintFormat = printFormat; \
		obj->Type = type; \


typedef void (PXAPI* PXLogPrintFunction)(const PXLoggingType loggingType, const char* const source, ...);


PXPublic void PXAPI PXConsoleClear();
PXPublic void PXAPI PXConsoleGoToXY(const PXInt32U x, const PXInt32U y);
PXPublic void PXAPI PXConsoleWriteF(const PXSize length, const char* const source, ...);
PXPublic void PXAPI PXConsoleWrite(const PXSize length, const char* const source);
PXPublic void PXAPI PXConsoleWriteFV(const PXSize length, const char* const source, va_list* const va_list);


//PXPublic void PXAPI PXConsoleTranlateColorsA(char* const bufferInput, char* const bufferOuput);
PXPublic void PXAPI PXConsoleTranlateColors(PXText* const bufferInput, PXText* const bufferOuput);


PXPublic void PXAPI PXLogPrintInvoke
(
	PXLoggingEventData* const pxLoggingEventData,
	...
);

PXPublic void PXAPI PXLogPrint(const PXLoggingType loggingType, const char* const source, const char* const action, const char* const format, ...);

#endif