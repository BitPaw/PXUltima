#pragma once

#ifndef PXTabListIncluded
#define PXTabListIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXTabList_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXTabList;


typedef struct PXTabListCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXTabListCreateInfo;



typedef struct PXEngineUIElementTabPageSingleInfo_
{
    PXWindow** UIElement;
    char* PageName;
    PXIcon* TABIcon;
    PXEngineUIElementType UIElementType;
}
PXEngineUIElementTabPageSingleInfo;


typedef struct PXEngineUIElementTabPageInfo_
{
    PXEngineUIElementTabPageSingleInfo* TabPageSingleInfoList;
    PXSize TabPageSingleInfoAmount;
}
PXEngineUIElementTabPageInfo;



typedef struct PXWindowMenuItem_ PXWindowMenuItem;


typedef struct PXWindowExtendedMenuItem_
{
    PXSize MenuItemAmount;
    PXWindowMenuItem* MenuItemList;
}
PXWindowExtendedMenuItem;

typedef struct PXWindowExtendedBehaviourTab_
{
    PXSize TABPageAmount;
    PXSize TABPageIndexCurrent;
    PXWindow* TABPageList;
}
PXWindowExtendedBehaviourTab;

typedef struct PXResourceViewEntry_
{
    PXWindow* EntryList;
    void* Resource;
}
PXResourceViewEntry;

typedef struct PXWindowExtendedBehaviourResourceView_
{
    PXSize EntryAmount;
    PXSize EntryIndexCurrent;
    PXResourceViewEntry* EntryList;

    //PXResourceManager* ResourceManager;
}
PXWindowExtendedBehaviourResourceView;
//-----------------------------------------------------

PXPublic void PXAPI PXTabListRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXTabListDraw(PXTabList PXREF pxTabList, PXEngineUIDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXTabListCreate(PXTabList** pxTabListREF, PXTabListCreateInfo PXREF pxTabListCreateInfo);

#endif