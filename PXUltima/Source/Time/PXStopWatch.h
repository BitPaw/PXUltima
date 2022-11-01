#ifndef PXStopWatchINCLUDE
#define PXStopWatchINCLUDE

#include <Format/Type.h>
#include <Time/PXTime.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXStopWatch_
	{
		PXTime* TimeStampData;
		size_t TimeStampDataSizeUsed;
		size_t TimeStampDataSizeAllocated;
	}
	PXStopWatch;

	PXPublic void PXStopWatchConstruct(PXStopWatch* const stopWatch);

	PXPublic void PXStopWatchSet(PXStopWatch* const stopWatch, void* const data, const size_t dataSize);

	PXPublic void PXStopWatchStart(PXStopWatch* const stopWatch);
	PXPublic PXBool PXStopWatchTrigger(PXStopWatch* const stopWatch, PXTime* const time);
	PXPublic void PXStopWatchReset(PXStopWatch* const stopWatch);

#ifdef __cplusplus
}
#endif

#endif