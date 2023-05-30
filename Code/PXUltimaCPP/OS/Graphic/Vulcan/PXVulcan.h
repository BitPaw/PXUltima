#ifndef PXVulcanINCLUDE
#define PXVulcanINCLUDE

#include <OS/System/OSVersion.h>

#define PXVulcanUSE OSDeviceToUse == OSDeviceDestop
#if PXVulcanUSE

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXVulcan_
	{
		unsigned int __dummy__;
	}
	PXVulcan;

#ifdef __cplusplus
}
#endif

#endif
#endif