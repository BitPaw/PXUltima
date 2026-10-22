#include "PXTime.h"


#include <PX/OS/System/Version/PXOSVersion.h>
#include <PX/OS/Error/PXResult.h>

#if OSUnix
#include <time.h>
#elif OSWindows
#include <Windows.h>
#include <intsafe.h>

PXBool _is10MHz;

PXI64U _NsPerTickMultiplier = 0;
const PXI32U _ShiftAmount = 32; // Used to shift numbers back and forth for int calculations.

// Modern windows uses a 10MHz interval.
// This makes the lowest precision around 100ns.
const PXI64U _PerformanceFrequencyExpectation = 10000000ULL;
PXI64U _PerformanceFrequency = 0;

#endif

void PXAPI PXTimeStampInit(void)
{
#if OSUnix
#elif OSWindows
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    _PerformanceFrequency = freq.QuadPart;

    _is10MHz = _PerformanceFrequency == _PerformanceFrequencyExpectation;

    // init time
    if(!_is10MHz)
    {
        // Fallback for rare/older hardware configurations (e.g., HPET at 14.3 MHz)
        // Scaled Multiplier = (1,000,000,000 << 32) / Frequency
        _NsPerTickMultiplier = (1000000000ULL << _ShiftAmount) / freq.QuadPart;
    }
#endif
}

PXI64U PXAPI PXTimeStampPrecision(void)
{
    return 1000000000 / _PerformanceFrequency;
}

PXTimeStamp PXAPI PXTimeStampNow(void)
{
    PXTimeStamp pxTimeStamp;

#if OSUnix
#elif OSWindows
    // Get actual current time
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    if(_is10MHz)
    {
        // fast integer multiply by 100
        pxTimeStamp.Value = counter.QuadPart * 100u;
    }
    else
    {
        // [Note] This can trigger an overflow! Use bigger register size 
        pxTimeStamp.Value = (counter.QuadPart * _NsPerTickMultiplier) >> _ShiftAmount;
    }
  
    return pxTimeStamp;
#endif
}

PXF64 PXAPI PXTimeNSToSF(const PXF64 value)
{
    return value / 1000000000.0;
}

PXF64 PXAPI PXTimeNSToMSF(const PXF64 value)
{
    return value / 1000000.0;
}

PXF64 PXAPI PXTimeNSToUSF(const PXF64 value)
{
    return value / 1000.0;
}

PXF64 PXAPI PXTimeStampDeltaSF(const PXTimeStamp pxTimeStampStart)
{
    PXTimeStamp timeCounterNow = PXTimeStampNow();
    PXF64 timeDelta = (PXF64)timeCounterNow.Value - (PXF64)pxTimeStampStart.Value;

    timeDelta = PXTimeNSToSF(timeDelta);

    return timeDelta;
}

PXF64 PXAPI PXTimeStampDeltaMSF(const PXTimeStamp pxTimeStampStart)
{
    PXTimeStamp timeCounterNow = PXTimeStampNow();
    PXF64 timeDelta = (PXF64)timeCounterNow.Value - (PXF64)pxTimeStampStart.Value;

    timeDelta = PXTimeNSToMSF(timeDelta);

    return timeDelta;
}

PXF64 PXAPI PXTimeStampDeltaUSF(const PXTimeStamp pxTimeStampStart)
{
    PXTimeStamp timeCounterNow = PXTimeStampNow();
    PXF64 timeDelta = (PXF64)timeCounterNow.Value - (PXF64)pxTimeStampStart.Value;

    timeDelta = PXTimeNSToUSF(timeDelta);

    return timeDelta;
}

PXI64U PXAPI PXTimeStampDeltaNSF(const PXTimeStamp pxTimeStampStart)
{
    PXTimeStamp timeCounterNow = PXTimeStampNow();
    PXI64U timeDelta = timeCounterNow.Value - pxTimeStampStart.Value;

    return timeDelta;
}

void PXAPI PXTimeStampAddNS(PXTimeStamp PXREF pxTimeStamp, const PXTimeStamp pxTimeStampAdd)
{
    pxTimeStamp->Value += pxTimeStampAdd.Value;
}

void PXAPI PXTimeStampReduceNS(PXTimeStamp PXREF pxTimeStamp, const PXTimeStamp pxTimeStampReduce)
{
    pxTimeStamp->Value -= pxTimeStampReduce.Value;
}

PXTimeMonth PXAPI PXTimeMonthFromID(const PXI8U monthID)
{
    switch(monthID)
    {
        case 1u:
            return MonthJanuary;
        case 2u:
            return MonthFebruary;
        case 3u:
            return MonthMarch;
        case 4u:
            return MonthApril;
        case 5u:
            return MonthMarch;
        case 6u:
            return MonthJune;
        case 7u:
            return MonthJuly;
        case 8u:
            return MonthAugust;
        case 9u:
            return MonthSeptember;
        case 10u:
            return MonthOctober;
        case 11u:
            return MonthNovember;
        case 12u:
            return MonthDecember;
        default:
            return MonthInvalid;
    }
}

PXTimeDayOfWeek PXAPI PXTimeDayFromID(const PXI8U dayID)
{
    switch(dayID)
    {
        case 1u:
            return DayMonday;
        case 2u:
            return DayTuesday;
        case 3u:
            return DayWednesday;
        case 4u:
            return DayThursday;
        case 5u:
            return DayFriday;
        case 6u:
            return DaySaturday;
        case 0u:
            return DaySunday;
        default:
            return DayInvalid;
    }
}

void PXAPI PXTimeNow(PXTime PXREF pxTime)
{
#if OSUnix

    // POSIX.1-2008 marks gettimeofday() as obsolete,

    // clock_gettime
    long             days;
    struct timespec  ts;

    const int getTimeResultID = clock_gettime(CLOCK_REALTIME, &ts);
    const PXResult getTimeResult = PXErrorCurrent(0 == getTimeResultID);

    if(PXResultOK != getTimeResult)
    {
        return getTimeResult;
    }

    struct tm* tm_info = localtime(&ts.tv_sec);
    //strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);

    pxTime->Year = tm_info->tm_year;
    pxTime->Month = tm_info->tm_mon;
    pxTime->DayOfWeek = 0;
    pxTime->Day = tm_info->tm_mday;
    pxTime->Hour = tm_info->tm_hour;
    pxTime->Minute = tm_info->tm_min;
    pxTime->Second = tm_info->tm_sec;
    pxTime->Milliseconds = 0;



    // ctime


#elif OSWindows
    SYSTEMTIME systemTime;

    //GetSystemTime(&st);
    GetLocalTime(&systemTime);

    PXTimeConvertFromOS(pxTime, &systemTime);
#endif
}

PXSize PXAPI PXTimeDelta(const PXTime PXREF timeA, const PXTime PXREF timeB, PXTime PXREF timeResult)
{
    timeResult->Day = (timeB->Day - timeA->Day);
    timeResult->Hour = (timeB->Hour - timeA->Hour);
    timeResult->Minute = (timeB->Minute - timeA->Minute);
    timeResult->Second = (timeB->Second - timeA->Second);
    timeResult->Milliseconds = (timeB->Milliseconds - timeA->Milliseconds);

    return 0;
}

PXSize PXAPI PXTimeMilliseconds(const PXTime* time)
{
    const PXSize dayDelta = (time->Day);
    const PXSize hourDelta = (time->Hour) + (dayDelta * 24);
    const PXSize minuteDelta = (time->Minute) + (hourDelta * 60);
    const PXSize secondsDelta = (time->Second) + (minuteDelta * 60);
    const PXSize millisecondsDelta = (time->Milliseconds) + (secondsDelta * 1000);

    return millisecondsDelta;
}

PXSize PXAPI PXTimeMillisecondsDelta(const PXTime* timeA, const PXTime* timeB)
{
    // Year = -1;
    // Month = Month::Invalid;
    // DayOfWeek = DayOfWeek::Invalid;;

    const PXSize dayDelta = (timeB->Day - timeA->Day);
    const PXSize hourDelta = (timeB->Hour - timeA->Hour) + (dayDelta * 24);
    const PXSize minuteDelta = (timeB->Minute - timeA->Minute) + (hourDelta * 60);
    const PXSize secondsDelta = (timeB->Second - timeA->Second) + (minuteDelta * 60);
    const PXSize millisecondsDelta = (timeB->Milliseconds - timeA->Milliseconds) + (secondsDelta * 1000);

    return millisecondsDelta;
}

PXF32 PXAPI PXTimeCounterStampToSecoundsF(const PXI64U timestamp)
{
    PXF32 result = 0;

#if OSUnix

#elif OSWindows
    LARGE_INTEGER frequency;

    QueryPerformanceFrequency(&frequency);

    result = timestamp / (PXF32)frequency.QuadPart;

    //result /= 1000000.0f;
#else
    // Not supported
#endif

    return result;
}

void PXAPI PXTimeFromOSCTime(PXTime PXREF pxTime, const time_t timeValue)
{
#if OSUnix || OSForcePOSIXForWindows

    struct tm* timeinfo = localtime(&timeValue);

    pxTime->Year = 1900 + timeinfo->tm_year; // years since 1900
    pxTime->Milliseconds = 0;
    pxTime->Second = timeinfo->tm_sec; // seconds after the minute - [0, 60] including leap second
    pxTime->Minute = timeinfo->tm_min;   // minutes after the hour - [0, 59]
    pxTime->Hour = timeinfo->tm_hour;  // hours since midnight - [0, 23]
    pxTime->Day = timeinfo->tm_mday;  // day of the month - [1, 31]
    pxTime->DayOfWeek = 1 + timeinfo->tm_wday; // days since Sunday - [0, 6]
    pxTime->Month = 1 + timeinfo->tm_mon;  // months since January - [0, 11]

#elif OSWindows

    FILETIME fileTime;

#if 1
    ULARGE_INTEGER time_value;
    time_value.QuadPart = (timeValue * 10000000LL) + 116444736000000000LL;
    fileTime.dwLowDateTime = time_value.LowPart;
    fileTime.dwHighDateTime = time_value.HighPart;
#elif
    //  LEGACY
    LONGLONG time_value = Int32x32To64(t, 10000000) + 116444736000000000;
    pft->dwLowDateTime = (DWORD)time_value;
    pft->dwHighDateTime = time_value >> 32;
#endif

    PXTimeFromOSFileTime(pxTime, &fileTime);

#endif

}

#if OSWindows
void PXAPI PXTimeFromOSFileTime(PXTime PXREF pxTime, FILETIME PXREF fileTime)
{
#if OSUnix
#elif OSWindows
    SYSTEMTIME systemTime;

    const BOOL result = FileTimeToSystemTime(fileTime, &systemTime);

    PXTimeConvertFromOS(pxTime, &systemTime);
#endif
}

void PXAPI PXTimeConvertFromOS(PXTime PXREF time, const SYSTEMTIME PXREF systemTime)
{
    time->Year = systemTime->wYear;
    time->Month = systemTime->wMonth;
    time->DayOfWeek = systemTime->wDayOfWeek;
    time->Day = systemTime->wDay;
    time->Hour = systemTime->wHour;
    time->Minute = systemTime->wMinute;
    time->Second = systemTime->wSecond;
    time->Milliseconds = systemTime->wMilliseconds;
}
#endif