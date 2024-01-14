#ifndef PXDialogInclude
#define PXDialogInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXDialogBoxState_
	{
		PXDialogBoxStateInvalid,
		PXDialogBoxStateClosed,
		PXDialogBoxStateOpen
	}
	PXDialogBoxState;

	typedef struct PXDialogContainerMesssage_
	{
		PXTexture2D* CharacterSprite;
		PXText Text;
		PXDialogBoxState State;
	}
	PXDialogContainerMesssage;

	typedef struct PXDialogContainer_
	{
		int __dummy__;
	}
	PXDialogContainer;

	PXPublic PXActionResult PXAPI PXDialogContainerMessageFetch(PXDialogContainer* const pxDialogContainer);

#ifdef __cplusplus
}
#endif

#endif