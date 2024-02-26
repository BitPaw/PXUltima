#include "PXVulcan.h"

#include <OS/Memory/PXMemory.h>
#include <OS/Window/PXWindow.h>
#include <OS/Console/PXConsole.h>

#include <stdio.h>

#define PXVulcanDebug 1

PXActionResult PXAPI PXVulcanErrorCodeFromID(const VkResult vkResultID)
{
	switch (vkResultID)
	{
		case VK_SUCCESS: return PXActionSuccessful;
		case VK_NOT_READY: return ResourceUnavailableTryAgain;

#if 0
		case VK_TIMEOUT: return 000000000;
		case VK_EVENT_SET:return 000000000;
		case VK_EVENT_RESET:return 000000000;
		case VK_INCOMPLETE: return PXActionRefuedInputBufferTooSmal;
		case VK_ERROR_OUT_OF_HOST_MEMORY:return 000000000;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:return 000000000;
		case VK_ERROR_INITIALIZATION_FAILED:return 000000000;
		case VK_ERROR_DEVICE_LOST:return 000000000;
		case VK_ERROR_MEMORY_MAP_FAILED:return 000000000;
		case VK_ERROR_LAYER_NOT_PRESENT:return 000000000;
		case VK_ERROR_EXTENSION_NOT_PRESENT:return 000000000;
		case VK_ERROR_FEATURE_NOT_PRESENT:return 000000000;
		case VK_ERROR_INCOMPATIBLE_DRIVER:return 000000000;
		case VK_ERROR_TOO_MANY_OBJECTS:return 000000000;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:return 000000000;
		case VK_ERROR_FRAGMENTED_POOL:return 000000000;
		case VK_ERROR_UNKNOWN: return PXActionFailedUnkownError;
		case VK_ERROR_OUT_OF_POOL_MEMORY:return 000000000;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE: return 000000000;
		case VK_ERROR_FRAGMENTATION: return 000000000;
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return 000000000;
		case VK_PIPELINE_COMPILE_REQUIRED: return 000000000;
		case VK_ERROR_SURFACE_LOST_KHR: return 000000000;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return 000000000;
		case VK_SUBOPTIMAL_KHR: return 000000000;
		case VK_ERROR_OUT_OF_DATE_KHR: return 000000000;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return 000000000;
		case VK_ERROR_VALIDATION_FAILED_EXT: return 000000000;
		case VK_ERROR_INVALID_SHADER_NV: return 000000000;
		case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return 000000000;
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return 000000000;
		case VK_ERROR_NOT_PERMITTED_KHR: return 000000000;
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return 000000000;
		case VK_THREAD_IDLE_KHR: return 000000000;
		case VK_THREAD_DONE_KHR: return 000000000;
		case VK_OPERATION_DEFERRED_KHR: return 000000000;
		case VK_OPERATION_NOT_DEFERRED_KHR: return 000000000;
		case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return 000000000;
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return 000000000;
		case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT: return 000000000;
		// case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return 000000000;
		// case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return 000000000;
		// case VK_ERROR_FRAGMENTATION_EXT: return 000000000;
		// case VK_ERROR_NOT_PERMITTED_EXT: return 000000000;
		// case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: return 000000000;
		// case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR: return 000000000;
		// case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return 000000000;
		// case VK_PIPELINE_COMPILE_REQUIRED_EXT: return 000000000;
		// case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT_: return 000000000;
#endif

		default:
			return PXActionInvalid;
	};
}

PXActionResult PXAPI PXVulcanInitialize(PXVulcan* const pxVulcan, struct PXGraphicInitializeInfo_* const pxGraphicInitializeInfo)
{
	PXClear(PXVulcan, pxVulcan);

#if PXVulcanDebug
	PXLogPrint
	(
		PXLoggingInfo,
		"Vulcan",
		"Initialize",
		"Starting library..."
	);
#endif

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
#if PXVulcanDebug
				PXLogPrint
				(
					PXLoggingError,
					"Vulcan",
					"Initialize",
					"initializing failed: Library not found"
				);
#endif

				return PXActionLibraryNotFound;
			}
		}
	}

#if PXVulcanDebug
	PXLogPrint
	(
		PXLoggingInfo,
		"Vulcan",
		"Initialize",
		"Library detected 0x%p",
		pxVulcan->LibraryID.ID
	);
#endif


	// Link functions
	{
		PXGraphic* pxGraphic = pxGraphicInitializeInfo->Graphic;

		pxGraphic->Release = PXVulcanRelease;
	}


	PXVulcanAllocationCallbacks pxVulcanAllocationCallbacks;
	pxVulcanAllocationCallbacks.pUserData = PXNull;
	pxVulcanAllocationCallbacks.pfnAllocation = PXNull;
	pxVulcanAllocationCallbacks.pfnReallocation = PXNull;
	pxVulcanAllocationCallbacks.pfnFree = PXNull;
	pxVulcanAllocationCallbacks.pfnInternalAllocation = PXNull;
	pxVulcanAllocationCallbacks.pfnInternalFree = PXNull;


	// Fetch all functions
	{
#if PXVulcanDebug
		PXLogPrint
		(
			PXLoggingInfo,
			"Vulcan",
			"Init",
			"Fetching functions..."
		);
#endif

		PXLibraryGetSymbolA(&pxVulcan->LibraryID, &pxVulcan->InstanceProcAddrGetCallBack, "vkGetInstanceProcAddr");

		pxVulcan->InstanceCreate = pxVulcan->InstanceProcAddrGetCallBack(PXNull, "vkCreateInstance");

		// Create instance
		{
			const char* buffer[] = 
			{
				"VK_KHR_surface",  
				"VK_KHR_win32_surface",
				//"VK_KHR_display" // Some say this is only in linux
			};

			VkApplicationInfo applicationInfo;
			applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			applicationInfo.pNext = PXNull;
			applicationInfo.pApplicationName = "PX";
			applicationInfo.applicationVersion = 0;
			applicationInfo.pEngineName = "PXUltima";
			applicationInfo.engineVersion = 0;
			applicationInfo.apiVersion = 0;

			PXVulkanInstanceCreateInfo pxVulkanInstanceCreateInfo;
			pxVulkanInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			pxVulkanInstanceCreateInfo.pNext = PXNull;
			pxVulkanInstanceCreateInfo.flags = PXNull;
			pxVulkanInstanceCreateInfo.pApplicationInfo = &applicationInfo;
			pxVulkanInstanceCreateInfo.enabledLayerCount = PXNull;
			pxVulkanInstanceCreateInfo.ppEnabledLayerNames = PXNull;
			pxVulkanInstanceCreateInfo.enabledExtensionCount = sizeof(buffer)/8;
			pxVulkanInstanceCreateInfo.ppEnabledExtensionNames = buffer;

			const VkResult createResult = pxVulcan->InstanceCreate
			(
				&pxVulkanInstanceCreateInfo,
				PXNull,
				&pxVulcan->Instance
			);


#if PXVulcanDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"Vulcan",
				"Initialize",
				"Created 0x%p",
				pxVulcan->Instance
			);
#endif

		}

		pxVulcan->DeviceCreate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkCreateDevice");
		pxVulcan->DevicePhysicalListEnumerate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkEnumeratePhysicalDevices");
		pxVulcan->DevicePhysicalPropertiesGet = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkGetPhysicalDeviceProperties");
		pxVulcan->DevicePhysicalDisplayPropertiesGet = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
		pxVulcan->ExtensionInstancePropertiesEnumerate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkEnumerateInstanceExtensionProperties");
		pxVulcan->InstanceDestroy = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkDestroyInstance");
		pxVulcan->SwapchainCreate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkDestroyInstance");

#if OSUnix
		pxVulcan->SurfaceCreate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "yxxxxxxxxxxxxxx");
#elif OSWindows
		pxVulcan->SurfaceCreate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "vkCreateWin32SurfaceKHR");
#elif OSAndroid
		pxVulcan->SurfaceCreate = pxVulcan->InstanceProcAddrGetCallBack(pxVulcan->Instance, "xxxxxxxxxxxxxxx");
#endif

	}

	// Fetch extensions
	{
		PXInt32U amountOfExtenions = 0;

		const VkResult fetch = pxVulcan->ExtensionInstancePropertiesEnumerate(PXNull, &amountOfExtenions, PXNull);

		VkExtensionProperties VkExtensionPropertieList[20];

		const VkResult fetchB = pxVulcan->ExtensionInstancePropertiesEnumerate(PXNull, &amountOfExtenions, VkExtensionPropertieList);

#if PXVulcanDebug
		PXLogPrint
		(
			PXLoggingInfo,
			"Vulcan",
			"Initialize",
			"Extensions detected. Amount:%i",
			amountOfExtenions
		);
#endif

		for (PXInt32U i = 0; i < amountOfExtenions; i++)
		{
			VkExtensionProperties* const extensionPropertie = &VkExtensionPropertieList[i];

#if PXVulcanDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"Vulcan",
				"Initialize",
				"- %2i %s ",
				extensionPropertie->specVersion,
				extensionPropertie->extensionName
			);
#endif
		}
	}

	// Setup debugger if wanted
	{

	}

#if OSWindows
	// Create rendering surface
	{
		VkWin32SurfaceCreateInfoKHR vkWin32SurfaceCreateInfoKHR;
		vkWin32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		vkWin32SurfaceCreateInfoKHR.pNext = PXNull;
		vkWin32SurfaceCreateInfoKHR.flags = PXNull;
		vkWin32SurfaceCreateInfoKHR.hinstance = GetModuleHandle(NULL);
		vkWin32SurfaceCreateInfoKHR.hwnd = pxGraphicInitializeInfo->WindowReference->ID;		

		const VkResult createSurface = pxVulcan->SurfaceCreate(pxVulcan->Instance, &vkWin32SurfaceCreateInfoKHR, PXNull, &pxVulcan->SurfaceMainRendering);
	}
#endif

	// List and select physical device
	{

		PXInt32U numberOfDevices = 0;

		pxVulcan->DevicePhysicalListEnumerate(pxVulcan->Instance, &numberOfDevices, PXNull); // Get number of devices

		PXVulkanDevicePhysical pxVulkanDevicePhysicalList[20];

		const VkResult result = pxVulcan->DevicePhysicalListEnumerate(pxVulcan->Instance, &numberOfDevices, pxVulkanDevicePhysicalList);


#if PXVulcanDebug
		PXLogPrint
		(
			PXLoggingInfo,
			"Vulcan",
			"Initialize",
			"Deteced %i physical devices",
			numberOfDevices
		);
#endif

		for (PXInt32U i = 0; i < numberOfDevices; ++i)
		{
			PXVulkanDevicePhysical* const vulkanDevicePhysical = &pxVulkanDevicePhysicalList[i];

			PXVulkanDevicePhysicalProperties pxVulkanDevicePhysicalProperties;

			pxVulcan->DevicePhysicalPropertiesGet(vulkanDevicePhysical, &pxVulkanDevicePhysicalProperties);

#if PXVulcanDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"Vulcan",
				"Init",
				"[Device %i/%i] %s",
				i + 1,
				numberOfDevices, 
				pxVulkanDevicePhysicalProperties.deviceName
			);
#endif
		}
		pxVulcan->DevicePhysical = pxVulkanDevicePhysicalList[1];
	}
		
	// Create virtual device
	{
		PXVulkanDeviceCreateInfo pxVulkanDeviceCreateInfo;
		pxVulkanDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		pxVulkanDeviceCreateInfo.pNext = PXNull;
		pxVulkanDeviceCreateInfo.flags = PXNull;
		pxVulkanDeviceCreateInfo.queueCreateInfoCount = PXNull;
		pxVulkanDeviceCreateInfo.pQueueCreateInfos = PXNull;
		pxVulkanDeviceCreateInfo.enabledLayerCount = PXNull;
		pxVulkanDeviceCreateInfo.ppEnabledLayerNames = PXNull;
		pxVulkanDeviceCreateInfo.enabledExtensionCount = PXNull;
		pxVulkanDeviceCreateInfo.ppEnabledExtensionNames = PXNull;
		pxVulkanDeviceCreateInfo.pEnabledFeatures = PXNull;


		const VkResult createResult = pxVulcan->DeviceCreate
		(
			pxVulcan->DevicePhysical,
			&pxVulkanDeviceCreateInfo,
			PXNull,
			&pxVulcan->DeviceVirtual
		);

#if PXVulcanDebug
		PXLogPrint
		(
			PXLoggingInfo,
			"Vulcan",
			"Init",
			"Creating device..."
		);
#endif
	}

	// Get Display infos
	if(pxVulcan->DevicePhysicalDisplayPropertiesGet)
	{
		PXInt32U amountOfDisplays = 0;

		const VkResult fetchNumberResult = pxVulcan->DevicePhysicalDisplayPropertiesGet(pxVulcan->DevicePhysical, &amountOfDisplays, PXNull);

#if PXVulcanDebug
		PXLogPrint
		(
			PXLoggingInfo,
			"Vulcan",
			"Init",
			"Deteced %i displays",
			amountOfDisplays
		);
#endif

		VkDisplayPropertiesKHR VkDisplayPropertieList[20];

		const VkResult fetchDataResult = pxVulcan->DevicePhysicalDisplayPropertiesGet(pxVulcan->DevicePhysical, &amountOfDisplays, VkDisplayPropertieList);


		for (PXInt32U i = 0; i < amountOfDisplays; i++)
		{
#if PXVulcanDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"Vulcan",
				"Init",
				"[Device %i/%i] (%8ix%-8i) %s\n",
				i + 1,
				amountOfDisplays,
				VkDisplayPropertieList->physicalResolution.width,
				VkDisplayPropertieList->physicalResolution.height,
				VkDisplayPropertieList->displayName
			);
#endif
		}
	}

	// createSwapChain
	{

	}
	// createImageViews
	{

	}
	// createRenderPass
	{

	}
	// createGraphicsPipeline
	{

	}
	// createFramebuffers
	{

	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXVulcanRelease(PXVulcan* const pxVulcan)
{
	pxVulcan->InstanceDestroy(pxVulcan->Instance, PXNull);
	pxVulcan->Instance = PXNull;

	PXLibraryClose(&pxVulcan->LibraryID);

	return PXActionRefusedNotImplemented;
}