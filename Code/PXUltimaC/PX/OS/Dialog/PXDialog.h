#pragma once

#ifndef PXDialogIncluded
#define PXDialogIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

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

PXPublic PXResult PXAPI PXDialogSystemInitialize(PXDialogSystem PXREF pxDialogSystem);
PXPublic PXResult PXAPI PXDialogSystemRelease(PXDialogSystem PXREF pxDialogSystem);

PXPublic PXResult PXAPI PXDialogFileOpen(PXDialogSystem PXREF pxDialogSystem, PXText PXREF filePath);
PXPublic PXResult PXAPI PXDialogFileSave(PXDialogSystem PXREF pxDialogSystem, PXText PXREF filePath);

PXPublic PXBool PXAPI PXDialogColorSelect(PXDialogSystem PXREF pxDialogSystem, PXColorRGBI8 PXREF color);
PXPublic PXBool PXAPI PXDialogFontSelect(PXDialogSystem PXREF pxDialogSystem);
PXPublic PXBool PXAPI PXDialogPrint(PXDialogSystem PXREF pxDialogSystem);

#endif
