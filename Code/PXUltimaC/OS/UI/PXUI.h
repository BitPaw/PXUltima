#ifndef PXUIInclude
#define PXUIInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXUIElementProperty_
	{
		PXUIElementPropertyInvalid,
		PXUIElementPropertyTextContent,
		PXUIElementPropertySize,
		PXUIElementPropertyProgressbarPercentage,
		PXUIElementPropertyProgressbarBarColor,
	}
	PXUIElementProperty;

	typedef struct PXUIElementUpdateInfo_
	{
		PXUIElement* UIElementReference;
		PXWindow* WindowReference;
		PXUIElementProperty Property;
	}
	PXUIElementUpdateInfo;

	PXPublic PXActionResult PXAPI PXUIElementCreateOSStyle(PXUIElement* const pxUIElement, struct PXUIElementCreateData_* const pxUIElementCreateData);

	PXPublic PXActionResult PXAPI PXUIElementUpdateOSStyle(PXUIElementUpdateInfo* const pxUIElementUpdateInfo);
	PXPublic PXActionResult PXAPI PXUIElementUpdateOSStyleV(PXUIElementUpdateInfo* const pxUIElementUpdateInfo, const PXSize amount);

#ifdef __cplusplus
}
#endif

#endif