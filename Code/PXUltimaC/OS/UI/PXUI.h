#ifndef PXUIInclude
#define PXUIInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXUIElementProperty_
	{
		PXUIElementPropertyInvalid,
		PXUIElementPropertyTextContent,
		PXUIElementPropertyTextAllign,
		PXUIElementPropertyTextColor,
		PXUIElementPropertySize,
		PXUIElementPropertyProgressbarPercentage,
		PXUIElementPropertyProgressbarBarColor,


        PXUIElementPropertyItemAdd,
        PXUIElementPropertyItemDelete,
        PXUIElementPropertyItemUpdate,

		// Tree view

        PXUIElementPropertyTreeView_CreateDragImage,
        PXUIElementPropertyTreeView_DeleteAllItems,
        PXUIElementPropertyTreeView_DeleteItem,
        PXUIElementPropertyTreeView_EditLabel,
        PXUIElementPropertyTreeView_EndEditLabelNow,
        PXUIElementPropertyTreeView_EnsureVisible,
        PXUIElementPropertyTreeView_Expand,
        PXUIElementPropertyTreeView_GetBkColor,
        PXUIElementPropertyTreeView_GetCheckState,
        PXUIElementPropertyTreeView_GetChild,
        PXUIElementPropertyTreeView_GetCount,
        PXUIElementPropertyTreeView_GetDropHilight,
        PXUIElementPropertyTreeView_GetEditControl,
        PXUIElementPropertyTreeView_GetExtendedStyle,
        PXUIElementPropertyTreeView_GetFirstVisible,
        PXUIElementPropertyTreeView_GetImageList,
        PXUIElementPropertyTreeView_GetIndent,
        PXUIElementPropertyTreeView_GetInsertMarkColor,
        PXUIElementPropertyTreeView_GetISearchString,
        PXUIElementPropertyTreeView_GetItem,
        PXUIElementPropertyTreeView_GetItemHeight,
        PXUIElementPropertyTreeView_GetItemPartRect,
        PXUIElementPropertyTreeView_GetItemRect,
        PXUIElementPropertyTreeView_GetItemState,
        PXUIElementPropertyTreeView_GetLastVisible,
        PXUIElementPropertyTreeView_GetLineColor,
        PXUIElementPropertyTreeView_GetNextItem,
        PXUIElementPropertyTreeView_GetNextSelected,
        PXUIElementPropertyTreeView_GetNextSibling,
        PXUIElementPropertyTreeView_GetNextVisible,
        PXUIElementPropertyTreeView_GetParent,
        PXUIElementPropertyTreeView_GetPrevSibling,
        PXUIElementPropertyTreeView_GetPrevVisible,
        PXUIElementPropertyTreeView_GetRoot,
        PXUIElementPropertyTreeView_GetScrollTime,
        PXUIElementPropertyTreeView_GetSelectedCount,
        PXUIElementPropertyTreeView_GetSelection,
        PXUIElementPropertyTreeView_GetTextColor,
        PXUIElementPropertyTreeView_GetToolTips,
        PXUIElementPropertyTreeView_GetUnicodeFormat,
        PXUIElementPropertyTreeView_GetVisibleCount,
        PXUIElementPropertyTreeView_HitTest,
        PXUIElementPropertyTreeView_InsertItem,
        PXUIElementPropertyTreeView_MapAccIDToHTREEITEM,
        PXUIElementPropertyTreeView_MapHTREEITEMtoAccID,
        PXUIElementPropertyTreeView_Select,
        PXUIElementPropertyTreeView_SelectDropTarget,
        PXUIElementPropertyTreeView_SelectItem,
        PXUIElementPropertyTreeView_SelectSetFirstVisible,
        PXUIElementPropertyTreeView_SetAutoScrollInfo,
        PXUIElementPropertyTreeView_SetBkColor,
        PXUIElementPropertyTreeView_SetBorder,
        PXUIElementPropertyTreeView_SetCheckState,
        PXUIElementPropertyTreeView_SetExtendedStyle,
        PXUIElementPropertyTreeView_SetHot,
        PXUIElementPropertyTreeView_SetImageList,
        PXUIElementPropertyTreeView_SetIndent,
        PXUIElementPropertyTreeView_SetInsertMark,
        PXUIElementPropertyTreeView_SetInsertMarkColor,
        PXUIElementPropertyTreeView_SetItem,
        PXUIElementPropertyTreeView_SetItemHeight,
        PXUIElementPropertyTreeView_SetItemState,
        PXUIElementPropertyTreeView_SetLineColor,
        PXUIElementPropertyTreeView_SetScrollTime,
        PXUIElementPropertyTreeView_SetTextColor,
        PXUIElementPropertyTreeView_SetToolTips,
        PXUIElementPropertyTreeView_SetUnicodeFormat,
        PXUIElementPropertyTreeView_ShowInfoTip,
        PXUIElementPropertyTreeView_SortChildren,
        PXUIElementPropertyTreeView_SortChildrenCB,

	}
	PXUIElementProperty;

	typedef struct PXUIElementUpdateInfo_
	{
		PXUIElement* UIElementReference;
		PXWindow* WindowReference;
		PXUIElementProperty Property;
	}
	PXUIElementUpdateInfo;

	PXPublic PXActionResult PXAPI PXUIElementCreateOSStyle(PXUIElement* const pxUIElement, struct PXUIElementCreateData_* const pxUIElementCreateData);

	PXPublic PXActionResult PXAPI PXUIElementUpdateOSStyle(PXUIElementUpdateInfo* const pxUIElementUpdateInfo);
	PXPublic PXActionResult PXAPI PXUIElementUpdateOSStyleV(PXUIElementUpdateInfo* const pxUIElementUpdateInfo, const PXSize amount);

#ifdef __cplusplus
}
#endif

#endif