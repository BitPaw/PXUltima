#ifndef PXTimeINCLUDE
#define PXTimeINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif	

	typedef enum TimeDayOfWeek_
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
	TimeDayOfWeek;

	typedef enum TimeMonth_
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
	TimeMonth;


	// 10 Byte
	typedef struct PXTime_
	{
		PXInt16U Year;
		PXInt16U Milliseconds; // 0-1000
		PXInt8U Second;  // 0-60
		PXInt8U Minute; // 0-60
		PXInt8U Hour; // 0-24
		PXInt8U Day; // 0-31
		PXInt8U DayOfWeek; // enum
		PXInt8U Month; // 0-12	
	}
	PXTime;

	PXPublic TimeDayOfWeek PXTimeMonthFromID(PXInt8U monthID);
	PXPublic TimeMonth PXTimeDayFromID(PXInt8U dayID);

	PXPublic void PXTimeNow(PXTime* const time);
	PXPublic PXSize PXTimeMillisecondsDelta(const PXTime* timeA, const PXTime* timeB);

	PXPublic PXInt64U PXTimeCounterStampGet();
	PXPublic PXInt64U PXTimeCounterFrequencyGet();

#ifdef __cplusplus
}
#endif

#endif