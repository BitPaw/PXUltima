#ifndef PXTimeINCLUDE
#define PXTimeINCLUDE

#include <Media/PXType.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif	


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
		PXInt16U Year;
		PXInt16U Milliseconds; // 0-1000
		PXInt8U Second;  // 0-60
		PXInt8U Minute; // 0-60
		PXInt8U Hour; // 0-24
		PXInt8U Day; // 0-31
		PXInt8U DayOfWeek; // enum
		PXInt8U Month; // 0-12	

		union
		{
			clock_t ClockCycle;
			double ClockCycleDelta;
		};
	
	}
	PXTime;

	PXPublic PXTimeMonth  PXAPI PXTimeMonthFromID(const PXInt8U monthID);
	PXPublic PXTimeDayOfWeek PXAPI PXTimeDayFromID(const PXInt8U dayID);

	PXPublic void PXAPI PXTimeNow(PXTime* const time);

	PXPublic PXSize PXAPI PXTimeDelta(const PXTime* const timeA, const PXTime* const timeB, PXTime* const timeResult);

	PXPublic PXSize PXAPI PXTimeMilliseconds(const PXTime* time);
	PXPublic PXSize PXAPI PXTimeMillisecondsDelta(const PXTime* timeA, const PXTime* timeB);

	PXPublic PXInt64U PXAPI PXTimeCounterStampGet();
	PXPublic PXInt64U PXAPI PXTimeCounterFrequencyGet();

#ifdef __cplusplus
}
#endif

#endif