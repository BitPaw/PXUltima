#ifndef PXDialogINCLUDE
#define PXDialogINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXDialogFileOpen(PXText* const filePath);
	PXPublic PXActionResult PXAPI PXDialogFileSave(PXText* const filePath);

	PXPublic PXBool PXAPI PXDialogColorSelect(PXColorRGBI8* const color);
	PXPublic PXBool PXAPI PXDialogFontSelect();
	PXPublic PXBool PXAPI PXDialogPrint();

#ifdef __cplusplus
}
#endif

#endif