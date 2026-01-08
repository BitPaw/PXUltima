#pragma once

#ifndef PXButtonIncluded
#define PXButtonIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Engine/ECS/Entity/UI/Label/PXLabel.h>

typedef struct PXMenuStrip_
{
    PXWindow* WindowBase;
}
PXMenuStrip;

typedef struct PXMenuStripCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
    PXLabelCreateInfo TextInfo;
}
PXMenuStripCreateInfo;




//---------------------------------------------------------
// Menu Item - Behaviour
//---------------------------------------------------------
#define PXWindowMenuItemText
#define PXWindowMenuItemIcon
#define PXGUIMenuItemTypeImage      (1<<0)
#define PXGUIMenuItemTypeCheckmark  (1<<1)
#define PXGUIMenuItemTypeDATA       (1<<2)
#define PXGUIMenuItemTypeFTYPE      (1<<3)
#define PXGUIMenuItemTypeID         (1<<4)
#define PXGUIMenuItemTypeSTATE      (1<<5)
#define PXGUIMenuItemTypeSTRING     (1<<6)
#define PXGUIMenuItemTypeSUBMENU    (1<<7)
#define PXGUIMenuItemTypeTYPE       (1<<8)
#define PXGUIMenuItemTypeMFT_MENUBARBREAK  (1<<10)
#define PXGUIMenuItemTypeMFT_MENUBREAK          (1<<11)
#define PXGUIMenuItemTypeMFT_OWNERDRAW         (1<<12)
#define PXGUIMenuItemTypeMFT_RADIOCHECK         (1<<13)
#define PXGUIMenuItemTypeMFT_RIGHTJUSTIFY         (1<<14)
#define PXGUIMenuItemTypeMFT_RIGHTORDER         (1<<15)
#define PXGUIMenuItemTypeMFT_SEPARATOR         (1<<16)
#define PXGUIMenuItemTypeMFT_STRING         (1<<17)

#define PXGUIMenuItemStateMFS_CHECKED  (1<<0)
#define PXGUIMenuItemStateMFS_DEFAULT  (1<<1)
#define PXGUIMenuItemStateMFS_DISABLED  (1<<2)
#define PXGUIMenuItemStateMFS_ENABLED  (1<<3)
#define PXGUIMenuItemStateMFS_GRAYED  (1<<4)
#define PXGUIMenuItemStateMFS_HILITE  (1<<5)
#define PXGUIMenuItemStateMFS_UNCHECKED  (1<<6)
#define PXGUIMenuItemStateMFS_UNHILITE  (1<<7)


typedef struct PXWindowMenuItem_
{
    PXECSInfo Info;

    PXIcon* Icon;

    char* TextData;
    PXSize TextSize;
}
PXWindowMenuItem;


typedef struct PXWindowMenuItemInfo_
{
    PXI32U Flags;
    PXI32U State;
    PXWindow* Parent;
    struct PXWindowMenuItemList_* ChildList;

    char* TextData;
    PXSize TextSize;
}
PXWindowMenuItemInfo;

typedef struct PXWindowMenuItemList_
{
    PXWindowMenuItemInfo* MenuItemInfoListData;
    PXSize MenuItemInfoListAmount;
}
PXWindowMenuItemList;

PXPublic PXResult PXAPI PXMenuStripDraw(PXMenuStrip PXREF pxMenuStrip, PXMenuStripCreateInfo PXREF pxMenuStripCreateInfo);
PXPublic PXResult PXAPI PXMenuStripCreate(PXMenuStrip** pxMenuStrip, PXMenuStripCreateInfo PXREF pxMenuStripCreateInfo);

#endif