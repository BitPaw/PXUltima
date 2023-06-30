#ifndef PXVulcanINCLUDE
#define PXVulcanINCLUDE

#include <Media/PXType.h>
#include <OS/Library/PXLibrary.h>

#define PXVulcanUSE OSDeviceToUse == OSDeviceDestop
#if PXVulcanUSE

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*PXVulcanInstanceProcAddrGetFunction)();


	typedef struct PXVulcan_
	{
		unsigned int __dummy__;

		PXLibrary LibraryID;

		PXVulcanInstanceProcAddrGetFunction InstanceProcAddrGetCallBack;
	}
	PXVulcan;

	PXPublic void PXVulcanConstruct(PXVulcan* const pxVulcan);
	PXPublic void PXVulcanDestruct(PXVulcan* const pxVulcan);

	PXPublic PXActionResult PXVulcanInitialize(PXVulcan* const pxVulcan);

#ifdef __cplusplus
}
#endif

#endif
#endif