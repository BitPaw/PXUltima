#ifndef OSUIInclude
#define OSUIInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
#include <OS/CWindow.h>
#include <OS/OSVersion.h>

#if defined(OSUnix)
#define UIElementID unsigned int
#elif defined(OSWindows)
#include <Windows.h>
#define UIElementID HWND
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#if defined(OSWindows)
	CPublic void OSUIElementTextGet(const UIElementID uiElementID, const wchar_t* buffer, const size_t bufferSize, size_t* bufferRead);
	CPublic void OSUIElementTextSet(const UIElementID uiElementID, const wchar_t* buffer, const size_t bufferSize, size_t* bufferWritten);
#endif

	CPublic void OSUIElementEnable();

	typedef struct UILayout_
	{
		unsigned int Style;
		unsigned int X;
		unsigned int Y;
		unsigned int Width;
		unsigned int Height;
		
	}UILayout;


	CPublic void UIElementConstruct(const CWindowID window, UIElementID* ID, UILayout* Layout, const wchar_t* className, const wchar_t* name);

	typedef struct OSButton_
	{
		UIElementID ID;
		UILayout Layout;

	}OSButton;

	CPublic void ButtonConstruct
	(
		const CWindowID window,
		OSButton* button,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);

	//-------------------------------------------------------------------------

	typedef struct OSComboBox_
	{
		UIElementID ID;
		UILayout Layout;

	}OSComboBox;

	CPublic void ComboBoxConstruct
	(
		const CWindowID window,
		OSComboBox* comboBox,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);

	//-------------------------------------------------------------------------

	typedef struct OSListBox_
	{
		UIElementID ID;
		UILayout Layout;

	}OSListBox;

	CPublic void ListBoxConstruct
	(
		const CWindowID window,
		OSListBox* listBox,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);
	//-------------------------------------------------------------------------

	typedef struct OSTextEdit_
	{
		UIElementID ID;
		UILayout Layout;

	}OSTextEdit;

	CPublic void TextEditConstruct
	(
		const CWindowID window,
		OSTextEdit* textEdit,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);
	CPublic void TextEditContentGet(OSTextEdit* textEdit, const wchar_t* buffer, const size_t bufferSize, size_t* bufferWritten);

	//-------------------------------------------------------------------------


	// MDICLIENT
	//-------------------------------------------------------------------------


	typedef struct OSRichEdit_
	{
		UIElementID ID;
		UILayout Layout;

	}OSRichEdit;

	CPublic void RichEditConstruct
	(
		const CWindowID window,
		OSRichEdit* richEdit,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);

	//-------------------------------------------------------------------------


	typedef struct OSScrollBar_
	{
		UIElementID ID;
		UILayout Layout;

	}OSScrollBar;

	CPublic void ScrollBarConstruct
	(
		const CWindowID window,
		OSScrollBar* scrollBar,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);

	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------


 // Static objects?


	typedef struct OSUIText_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIText;

	CPublic void OSUITextConstruct
	(
		const CWindowID window,
		OSUIText* uiText,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);


	typedef struct OSUICheckBox_
	{
		UIElementID ID;
		UILayout Layout;
		unsigned char IsChecked;

	}OSUICheckBox;

	CPublic void OSCheckBoxConstruct
	(
		const CWindowID window,
		OSUICheckBox* uiCheckBox,
		const unsigned int x,
		const unsigned int y,
		const unsigned int width,
		const unsigned int height,
		const wchar_t* buttonText
	);

	//-------------------------------------------------------------------------

	typedef struct OSUITrackbar_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUITrackbar;

	CPublic void OSUITrackbarCreate(const CWindowID window, OSUITrackbar* osUITrackbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);

	//-------------------------------------------------------------------------



	//-------------------------------------------------------------------------

	typedef struct OSUIStatusbar_
	{
		UIElementID ID;
		UILayout Layout;
	}
	OSUIStatusbar;

	CPublic void OSUIStatusbarCreate(const CWindowID window, OSUIStatusbar* osUIStatusbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------

	typedef struct OSUIUpDown_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIUpDown;

	CPublic void OSUIUpDownCreate(const CWindowID window, OSUIUpDown* osUIUpDown, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);

	//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------

	typedef struct OSUIProgressbar_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIProgressbar;

	CPublic void OSUIProgressbarCreate(const CWindowID window, OSUIProgressbar* osUIProgressbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);

	//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------

	typedef struct OSUIHotKey_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIHotKey;

	CPublic void OSUIHotKeyCreate(const CWindowID window, OSUIHotKey* osUIHotKey, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);

	//-------------------------------------------------------------------------


	typedef struct OSUICalender_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUICalender;

	CPublic void OSUICalenderCreate(const CWindowID window, OSUICalender* osUICalender, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIToolTip_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIToolTip;

	CPublic void OSUIToolTipCreate(const CWindowID window, OSUIToolTip* osUIToolTip, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIAnimate_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIAnimate;

	CPublic void OSUIAnimateCreate(const CWindowID window, OSUIAnimate* osUIAnimate, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIDateTimePicker_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIDateTimePicker;

	CPublic void OSUIDateTimePickerCreate(const CWindowID window, OSUIDateTimePicker* osUIDateTimePicker, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIGroupBox_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIGroupBox;

	CPublic void OSUIGroupBoxCreate(const CWindowID window, OSUIGroupBox* osUIGroupBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIRadioButton_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIRadioButton;

	CPublic void OSUIRadioButtonCreate(const CWindowID window, OSUIRadioButton* osUIRadioButton, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);




	typedef struct OSUIGroupRadioButton_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIGroupRadioButton;

	CPublic void OSUIGroupRadioButtonCreate(const CWindowID window, OSUIGroupRadioButton* osUIGroupRadioButton, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIListBox_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIListBox;


	CPublic void OSUIListBoxCreate(const CWindowID window, OSUIListBox* osUIListBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUITreeView_
	{
		UIElementID ID;
		UILayout Layout;
	}
	OSUITreeView;

	CPublic void OSUITreeViewCreate(const CWindowID window, OSUITreeView* OSUITreeView, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);



	typedef struct OSUIIPInput_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIIPInput;

	CPublic void OSUIIPInputCreate(const CWindowID window, OSUIIPInput* osUIIPInput, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);



	typedef struct OSUITabControl_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUITabControl;

	CPublic void OSUITabControlCreate(const CWindowID window, OSUITabControl* osUITabControl, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);








	typedef struct OSUIPageScroller_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIPageScroller;

	CPublic void OSUIPageScrollerCreate(const CWindowID window, OSUIPageScroller* osUIPageScroller, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIFontSelector_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIFontSelector;

	CPublic void OSUIFontSelectorCreate(const CWindowID window, OSUIFontSelector* osUIFontSelector, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUIHeader_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUIHeader;

	CPublic void OSUIHeaderCreate(const CWindowID window, OSUIHeader* osUIHeader, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);


	typedef struct OSUILink_
	{
		UIElementID ID;
		UILayout Layout;

	}OSUILink;

	CPublic void OSUILinkCreate(const CWindowID window, OSUILink* osUILink, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText);






	


#ifdef __cplusplus
}
#endif

#endif


