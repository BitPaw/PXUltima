#ifndef PXStopWatchINCLUDE
#define PXStopWatchINCLUDE

#include <Media/PXType.h>
#include <OS/Time/PXTime.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXStopWatch_
	{
		PXTime TimeStartreference;

		PXTime* TimeStampData;
		PXSize TimeStampDataSizeUsed;
		PXSize TimeStampDataSizeAllocated;
	}
	PXStopWatch;

	PXPublic void PXAPI PXStopWatchConstruct(PXStopWatch* const stopWatch);

	PXPublic void PXAPI PXStopWatchSet(PXStopWatch* const stopWatch, void* const data, const PXSize dataSize);

	PXPublic void PXAPI PXStopWatchStart(PXStopWatch* const stopWatch);
	PXPublic PXBool PXAPI PXStopWatchTrigger(PXStopWatch* const stopWatch, PXTime* const time);
	PXPublic void PXAPI PXStopWatchReset(PXStopWatch* const stopWatch);

#ifdef __cplusplus
}
#endif

#endif