#ifndef ProcessorINCLUDE
#define ProcessorINCLUDE

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct Processor_
	{
		char NumberOfProcessors;
		char BrandName[64];  // string includes manufacturer, model and clockspeed
	}
	Processor;

	PXPublic void ProcessorFetchInfo(Processor* const processor);

	PXPublic unsigned int ProcessorFrequencyCurrent();
	PXPublic unsigned int ProcessorTimeReal();



#ifdef __cplusplus
}
#endif

#endif
