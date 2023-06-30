#include "PXVulcan.h"

#include <OS/Memory/PXMemory.h>

#if PXVulcanUSE



#endif

void PXVulcanConstruct(PXVulcan* const pxVulcan)
{
    PXMemoryClear(pxVulcan, sizeof(PXVulcan));
}

void PXVulcanDestruct(PXVulcan* const pxVulcan)
{
	PXLibraryClose(&pxVulcan->LibraryID);
}

PXActionResult PXVulcanInitialize(PXVulcan* const pxVulcan)
{
	// Load library
	{
		PXText vulcanLibraryName;

#if OSUnix
		PXTextMakeFixedA(&vulcanLibraryName, "libvulkan.so.1");
#elif OSApple
		PXTextMakeFixedA(&vulcanLibraryName, "libvulkan.dylib");
#elif OSWindows
		PXTextMakeFixedA(&vulcanLibraryName, "vulkan-1.dll");
#else
		return PXActionNotSupportedByOperatingSystem;
#endif	

		const PXBool libraryLoadResult = PXLibraryOpen(&pxVulcan->LibraryID, &vulcanLibraryName);

		if (!libraryLoadResult) // If first try failed, try 2nd time
		{
#if OSUnix
			PXTextMakeFixedA(&vulcanLibraryName, "libvulkan.so");
#elif OSApple
			PXTextMakeFixedA(&vulcanLibraryName, "libvulkan.1.dylib");
#elif OSWindows
			return PXActionLibraryNotFound;
#else
			return PXActionNotSupportedByOperatingSystem;
#endif	

			const PXBool libraryReloadResult = PXLibraryOpen(&pxVulcan->LibraryID, &vulcanLibraryName);

			if (!libraryReloadResult)
			{
				return PXActionLibraryNotFound;
			}
		}
	}

	// Fetch all functions
	{
		PXLibraryGetSymbolA(&pxVulcan->LibraryID, &pxVulcan->InstanceProcAddrGetCallBack, "vkGetInstanceProcAddr");
	}
}
