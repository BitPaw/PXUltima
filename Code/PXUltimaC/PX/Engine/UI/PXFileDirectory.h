#pragma once

#ifndef PXFileDirectoryIncluded
#define PXFileDirectoryIncluded

#include <PX/Type/PXWindow.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/Engine/UI/PXButton.h>

typedef struct PXFileDirectory_
{
    PXECSInfo Info;

    PXBool IsDirty;

    PXWindow* WindowBase;
    PXButton* ButtonFileBrowser;
    PXButton* ButtonFolderUp;
    PXButton* ButtonScrollUp;
    PXButton* ButtonScrollDown;
    PXButton* ButtonListRefresh;

    PXDirectorySearchCache DirectorySearchCache;
}
PXFileDirectory;

typedef struct PXFileDirectoryCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXFileDirectoryCreateInfo;

PXPublic void PXAPI PXFileDirectoryRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);


PXPublic PXResult PXAPI PXFileDirectoryCreate(PXFileDirectory** pxFileDirectory, PXFileDirectoryCreateInfo PXREF pxFileDirectoryCreateInfo);
PXPublic PXResult PXAPI PXFileDirectoryDestroy(PXFileDirectory PXREF pxFileDirectory);

PXPublic PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXDrawInfo PXREF pxDrawInfo);
#endif