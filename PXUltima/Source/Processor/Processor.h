#ifndef ProcessorINCLUDE
#define ProcessorINCLUDE

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	CPublic __declspec(dllexport) unsigned int ProcessorFrequencyCurrent();
	CPublic __declspec(dllexport) unsigned int ProcessorTimeReal();

#ifdef __cplusplus
}
#endif

#endif
