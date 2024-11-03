#include "PXTime.h"

#include <OS/System/PXOSVersion.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix
#include <time.h>
#elif OSWindows
#include <Windows.h>
#endif

PXTimeMonth PXAPI PXTimeMonthFromID(const PXInt8U monthID)
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

PXTimeDayOfWeek PXAPI PXTimeDayFromID(const PXInt8U dayID)
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

void PXAPI PXTimeNow(PXTime* const pxTime)
{
#if OSUnix

    // POSIX.1-2008 marks gettimeofday() as obsolete,

    // clock_gettime
    long             days;
    struct timespec  ts;

    const int getTimeResultID = clock_gettime(CLOCK_REALTIME, &ts);
    const PXActionResult getTimeResult = PXErrorCurrent(0 == getTimeResultID);

    if(PXActionSuccessful != getTimeResult)
    {
        return getTimeResult;
    }

    struct tm *tm_info = localtime(&ts.tv_sec);
     //strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info);

    pxTime->Year         = tm_info->tm_year;
    pxTime->Month        = tm_info->tm_mon;
    pxTime->DayOfWeek    = 0;
    pxTime->Day          = tm_info->tm_mday;
    pxTime->Hour         = tm_info->tm_hour;
    pxTime->Minute       = tm_info->tm_min;
    pxTime->Second       = tm_info->tm_sec;
    pxTime->Milliseconds = 0;



    // ctime


#elif OSWindows
    SYSTEMTIME systemTime;

    //GetSystemTime(&st);
    GetLocalTime(&systemTime);

    PXTimeConvertFromOS(pxTime, &systemTime);
#endif
}

PXSize PXAPI PXTimeDelta(const PXTime* const timeA, const PXTime* const timeB, PXTime* const timeResult)
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

float PXAPI PXTimeCounterStampToSecoundsF(const PXInt64U timestamp)
{
    float result = 0;

#if OSUnix

#elif OSWindows
    LARGE_INTEGER frequency;

    QueryPerformanceFrequency(&frequency);

    result = timestamp / (float)frequency.QuadPart;

    //result /= 1000000.0f;
#else
    // Not supported
#endif

    return result;
}


// Generates a timestamp in nanosecound space
PXInt64U PXAPI PXTimeCounterStampGet()
{
#if OSUnix
    struct timespec  ts;

    const int resultID = clock_gettime(CLOCK_MONOTONIC, &ts); // CLOCK_THREAD_CPUTIME_ID
    const PXBool success = -1 == resultID;
    //const PXActionResult result = POSIXError(success);

    if(success)
    {
        return ts.tv_sec;
    }
    else
    {
        return 0;
    }

#elif OSWindows


    LARGE_INTEGER largeInteger;
    const BOOL success = QueryPerformanceCounter(&largeInteger); // Windows 2000, Kernel32.dll

    if(success)
    {
        return largeInteger.QuadPart;
    }
    else
    {
        return 0;
    }
#else
    return 0;
#endif
}

PXInt64U PXAPI PXTimeCounterFrequencyGet()
{
#if OSUnix
    /*
    if (clock_getres(clock, &ts) == -1) {
               perror("clock_getres");
               exit(EXIT_FAILURE);
           }

           if (showRes)
               printf("     resolution: %10jd.%09ld\n",
                      (intmax_t) ts.tv_sec, ts.tv_nsec);
    */


    return 0;
#elif OSWindows
    LARGE_INTEGER largeInteger;
    const BOOL success = QueryPerformanceFrequency(&largeInteger); // Windows 2000, Kernel32.dll

    return largeInteger.QuadPart;
#endif
}

#if OSWindows
void PXAPI PXTimeConvertFromOS(PXTime* const time, const SYSTEMTIME* const systemTime)
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
