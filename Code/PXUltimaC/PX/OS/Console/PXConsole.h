#pragma once

#ifndef PXLogIncluded
#define PXLogIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>

#include <stdarg.h>

#define PXLogEnable         1
#define PXLogEnableColor    0

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

typedef enum PXConsoleTextColor_
{
    PXConsoleTextColorInvalid,
    PXConsoleTextColorBLACK,
    PXConsoleTextColorDARKBLUE,
    PXConsoleTextColorDARKGREEN,
    PXConsoleTextColorDARKCYAN,
    PXConsoleTextColorDARKRED,
    PXConsoleTextColorDARKMAGENTA,
    PXConsoleTextColorDARKYELLOW,
    PXConsoleTextColorDARKGRAY,
    PXConsoleTextColorGRAY,
    PXConsoleTextColorBLUE,
    PXConsoleTextColorGREEN,
    PXConsoleTextColorCYAN,
    PXConsoleTextColorRED,
    PXConsoleTextColorMAGENTA,
    PXConsoleTextColorYELLOW,
    PXConsoleTextColorWHITE,
}
PXConsoleTextColor;

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
        struct PXFile* FileReference;
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


typedef void (PXAPI* PXLogPrintFunction)(const PXLoggingType loggingType, const char PXREF source, ...);

PXPublic PXResult PXAPI PXConsoleTextColorSet(const PXConsoleTextColor pxConsoleTextColor);
PXPublic PXResult PXAPI PXConsoleTextColorSetFromID(const PXI16U coliorID);

PXPublic void PXAPI PXConsoleClear();
PXPublic void PXAPI PXConsoleGoToXY(const PXI32U x, const PXI32U y);
PXPublic void PXAPI PXConsoleWriteF(const PXSize length, const char PXREF source, ...);
PXPublic void PXAPI PXConsoleWrite(const PXSize length, const char PXREF source);
PXPublic void PXAPI PXConsoleWriteFV(const PXSize length, const char PXREF source, va_list va_list);


PXPublic void PXAPI PXConsoleWriteTablePXF32(const PXF32 PXREF data, const PXSize amount, const PXSize width);
PXPublic void PXAPI PXConsoleWriteTableInt(const PXI8U PXREF data, const PXSize amount, const PXSize width);

//PXPublic void PXAPI PXConsoleTranlateColorsA(char PXREF bufferInput, char PXREF bufferOuput);
PXPublic void PXAPI PXConsoleWriteWithColorCodes(struct PXText_ PXREF bufferInput);

PXPublic void PXAPI PXLogPrintInvoke(PXLoggingEventData PXREF pxLoggingEventData, ...);

PXPublic void PXAPI PXLogEnableASYNC();
PXPublic void PXAPI PXLogPrint(const PXLoggingType loggingType, const char PXREF source, const char PXREF action, const char PXREF format, ...);

#endif
