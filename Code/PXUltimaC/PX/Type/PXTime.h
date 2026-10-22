#pragma once

#ifndef PXTimeIncluded
#define PXTimeIncluded


#include <PX/Type/PXType.h>
#include <time.h>

#define PXYearMask        0b00000000000000000000000000000000
#define PXMonthMask        0b00000000000000000000000000000000 // 5 Bits
#define PXDayMask        0b00000000000000000000000000000000 // 5 Bits


typedef enum PXTimeDayOfWeek_
{
    DayInvalid,
    DayMonday,
    DayTuesday,
    DayWednesday,
    DayThursday,
    DayFriday,
    DaySaturday,
    DaySunday
}
PXTimeDayOfWeek;

typedef enum PXTimeMonth_
{
    MonthInvalid,
    MonthJanuary,
    MonthFebruary,
    MonthMarch,
    MonthApril,
    MonthMay,
    MonthJune,
    MonthJuly,
    MonthAugust,
    MonthSeptember,
    MonthOctober,
    MonthNovember,
    MonthDecember
}
PXTimeMonth;


// 10 Byte
typedef struct PXTime_
{
    PXI16U Year;
    PXI16U Milliseconds; // 0-1000
    PXI8U Second;  // 0-60
    PXI8U Minute; // 0-60
    PXI8U Hour; // 0-24
    PXI8U Day; // 0-31
    PXI8U DayOfWeek; // enum
    PXI8U Month; // 0-12
}
PXTime;

PXPublic PXTimeMonth  PXAPI PXTimeMonthFromID(const PXI8U monthID);
PXPublic PXTimeDayOfWeek PXAPI PXTimeDayFromID(const PXI8U dayID);

PXPublic void PXAPI PXTimeNow(PXTime PXREF time);

PXPublic PXSize PXAPI PXTimeDelta(const PXTime PXREF timeA, const PXTime PXREF timeB, PXTime PXREF timeResult);

PXPublic PXSize PXAPI PXTimeMilliseconds(const PXTime* time);
PXPublic PXSize PXAPI PXTimeMillisecondsDelta(const PXTime* timeA, const PXTime* timeB);

PXPublic PXF32 PXAPI PXTimeCounterStampToSecoundsF(const PXI64U timestamp);

PXPublic void PXAPI PXTimeFromOSCTime(PXTime PXREF pxTime, const time_t timeValue);

#if OSWindows
PXPublic void PXAPI PXTimeFromOSFileTime(PXTime PXREF pxTime, FILETIME PXREF fileTime);
PXPublic void PXAPI PXTimeConvertFromOS(PXTime PXREF time, const SYSTEMTIME PXREF systemTime);
#endif

typedef struct PXTimeStamp_
{
    PXI64U Value;
}
PXTimeStamp;


PXPublic PXF64 PXAPI PXTimeNSToSF(const PXF64 value);
PXPublic PXF64 PXAPI PXTimeNSToMSF(const PXF64 value);
PXPublic PXF64 PXAPI PXTimeNSToUSF(const PXF64 value);


// Minimal interval
PXPublic void PXAPI PXTimeStampInit(void);
PXPublic PXI64U PXAPI PXTimeStampPrecision(void);
PXPublic PXTimeStamp PXAPI PXTimeStampNow(void);


// Insert the started timestamp.
// Get the current time, calculate the delta, convert result into secounds
PXPublic PXF64 PXAPI PXTimeStampDeltaSF(const PXTimeStamp pxTimeStampStart);
PXPublic PXF64 PXAPI PXTimeStampDeltaMSF(const PXTimeStamp pxTimeStampStart);
PXPublic PXF64 PXAPI PXTimeStampDeltaUSF(const PXTimeStamp pxTimeStampStart);
PXPublic PXI64U PXAPI PXTimeStampDeltaNSF(const PXTimeStamp pxTimeStampStart);

PXPublic void PXAPI PXTimeStampAddNS(PXTimeStamp PXREF pxTimeStamp, const PXTimeStamp pxTimeStampAdd);
PXPublic void PXAPI PXTimeStampReduceNS(PXTimeStamp PXREF pxTimeStamp, const PXTimeStamp pxTimeStampReduce);

#endif