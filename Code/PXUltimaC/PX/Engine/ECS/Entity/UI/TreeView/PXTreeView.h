#pragma once

#ifndef PXTreeViewIncluded
#define PXTreeViewIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

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


typedef struct PXUIElementTreeViewItemInfo_
{
    char* TextDataOverride;
    PXSize TextSizeOverride;

    struct PXWindow_* ItemParent;
    struct PXWindow_* TreeView;

    //struct PXUIElement_* ElementSource;
    void* OwningObject;
    PXI32U OwningObjectType;

    PXUIElementTreeViewItemInsertMode InsertMode;

    // Result
    struct _TREEITEM* ItemHandle;
}
PXUIElementTreeViewItemInfo;



PXPublic PXResult PXAPI PXTreeViewDraw(PXTreeView PXREF pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo);
PXPublic PXResult PXAPI PXTreeViewCreate(PXTreeView** pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo);

#endif