#pragma once

#ifndef PXTabListIncluded
#define PXTabListIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

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



typedef struct PXUIElementTabPageSingleInfo_
{
    PXWindow** UIElement;
    char* PageName;
    PXIcon* TABIcon;
    PXUIElementType UIElementType;
}
PXUIElementTabPageSingleInfo;


typedef struct PXUIElementTabPageInfo_
{
    PXUIElementTabPageSingleInfo* TabPageSingleInfoList;
    PXSize TabPageSingleInfoAmount;
}
PXUIElementTabPageInfo;



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

PXPublic PXResult PXAPI PXTabListRegisterToECS();

PXPublic PXResult PXAPI PXTabListDraw(PXTabList PXREF pxTabList, PXDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXTabListCreate(PXTabList** pxTabListREF, PXTabListCreateInfo PXREF pxTabListCreateInfo);

#endif