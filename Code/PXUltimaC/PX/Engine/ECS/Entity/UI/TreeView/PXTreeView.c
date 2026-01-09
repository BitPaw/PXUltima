#include "PXTreeView.h"
#include <CommCtrl.h>

PXResult PXAPI PXTreeViewDraw(PXTreeView PXREF pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXTreeViewCreate(PXTreeView** pxTreeView, PXTreeViewCreateInfo PXREF pxTreeViewCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxTreeViewCreateInfo->WindowInfo;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_TREEVIEW;
    pxWindowCreateInfo->StyleFlags |=
        TVIF_TEXT |
        TVIF_IMAGE |
        TVIF_SELECTEDIMAGE |
        TVIF_PARAM | // Required to get the a selected item, otherwise its just NULL.
        TVS_HASBUTTONS |
        TVS_HASLINES |
        TVS_LINESATROOT;
#endif

    return PXResultOK;
}