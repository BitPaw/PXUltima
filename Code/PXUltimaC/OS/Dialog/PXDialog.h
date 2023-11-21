#ifndef PXDialogINCLUDE
#define PXDialogINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXDialogFileOpen(PXText* const filePath);
	PXPublic PXActionResult PXDialogFileSave(PXText* const filePath);

	PXPublic PXBool PXDialogColorSelect(PXColorRGBI8* const color);
	PXPublic PXBool PXDialogFontSelect();
	PXPublic PXBool PXDialogPrint();

#ifdef __cplusplus
}
#endif

#endif