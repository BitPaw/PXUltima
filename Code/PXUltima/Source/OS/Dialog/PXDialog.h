#ifndef PXDialogINCLUDE
#define PXDialogINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <Media/PXColor.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool PXDialogFileOpen(PXText* const filePath);
	PXPublic PXBool PXDialogFileSave(PXText* const filePath);

	PXPublic PXBool PXDialogColorSelect(PXColorRGBI8* const color);
	PXPublic PXBool PXDialogFontSelect();
	PXPublic PXBool PXDialogPrint();

#ifdef __cplusplus
}
#endif

#endif