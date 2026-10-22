#pragma once

#ifndef PXTreeViewIncluded
#define PXTreeViewIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXTreeView_
{
    PXWindow* WindowBase;
}
PXTreeView;

typedef struct sdfsdfsdfsdf
{
    PXWindowCreateInfo WindowInfo;
}
PXTreeViewCreateInfo;


typedef struct PXEngineUIElementTreeViewItemInfo_
{
    char* TextDataOverride;
    PXSize TextSizeOverride;

    struct PXWindow_* ItemParent;
    struct PXWindow_* TreeView;

    //struct PXEngineUIElement_* ElementSource;
    void* OwningObject;
    PXI32U OwningObjectType;

    PXEngineUIElementTreeViewItemInsertMode InsertMode;

    // Result
    struct _TREEITEM* ItemHandle;
}
PXEngineUIElementTreeViewItemInfo;



PXPublic PXResult PXAPI PXTreeViewDraw(PXTreeView PXREF pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo);
PXPublic PXResult PXAPI PXTreeViewCreate(PXTreeView** pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo);

#endif