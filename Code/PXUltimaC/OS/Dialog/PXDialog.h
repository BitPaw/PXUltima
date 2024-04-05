#ifndef PXDialogINCLUDE
#define PXDialogINCLUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

typedef struct PXDialogSystem_
{
	PXLibrary CommonDialogLibrary;

	void* DialogColorChooseA;
	void* DialogColorChooseW;
	void* DialogFontChooseA;
	void* DialogFontChooseW;
	void* DialogFileNameOpenGetA;
	void* DialogFileNameOpenGetW;
	void* DialogFileNameSaveGetA;
	void* DialogFileNameSaveGetW;
	void* DialogPrintA;
	void* DialogPrintW;
}
PXDialogSystem;

PXPublic PXActionResult PXAPI PXDialogSystemInitialize(PXDialogSystem* const pxDialogSystem);
PXPublic PXActionResult PXAPI PXDialogSystemRelease(PXDialogSystem* const pxDialogSystem);

PXPublic PXActionResult PXAPI PXDialogFileOpen(PXDialogSystem* const pxDialogSystem, PXText* const filePath);
PXPublic PXActionResult PXAPI PXDialogFileSave(PXDialogSystem* const pxDialogSystem, PXText* const filePath);

PXPublic PXBool PXAPI PXDialogColorSelect(PXDialogSystem* const pxDialogSystem, PXColorRGBI8* const color);
PXPublic PXBool PXAPI PXDialogFontSelect(PXDialogSystem* const pxDialogSystem);
PXPublic PXBool PXAPI PXDialogPrint(PXDialogSystem* const pxDialogSystem);

#endif