#include "PXWindowTest.h"

#include <OS/Window/PXWindow.h>
#include <OS/Memory/PXMemory.h>
#include <OS/UI/PXUI.h>

void PXAPI PXTestWindowAll()
{
#if 0
	PXTestWindowOpenClose();
#endif

#if 1
	PXTestWindowOpenUIRender();
#endif

}

void PXAPI PXTestWindowOpenClose()
{
	PXWindow pxWindow;
	PXWindowConstruct(&pxWindow);


	PXWindowCreateA(&pxWindow, -1, -1, -1, -1, "TEST", PXFalse);




	PXWindowDestruct(&pxWindow);
}

void PXAPI PXTestWindowOpenUIRender()
{
	PXWindow pxWindow;
	PXWindowConstruct(&pxWindow);

	PXWindowCreateA(&pxWindow, -1, -1, -1, -1, "TEST-UI", PXFalse);

	

	//pxWindow.HandleDeviceContext = GetDC(pxWindow.ID);

	PXUIElement button;
	PXClear(PXUIElement, &button);
	button.Type = PXUIElementTypeButton;
	button.Poisition2D.X = 30;
	button.Poisition2D.Y = 30;
	button.Poisition2D.Width = 300;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);


	PXUIElement dropDown;
	PXClear(PXUIElement, &dropDown);
	button.Type = PXUIElementTypeComboBox;
	button.Poisition2D.X = 30;
	button.Poisition2D.Y = 80;
	button.Poisition2D.Width = 300;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);


	PXUIElement textEdit;
	PXClear(PXUIElement, &textEdit);
	button.Type = PXUIElementTypeTextEdit;
	button.Poisition2D.X = 30;
	button.Poisition2D.Y = 150;
	button.Poisition2D.Width = 300;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);


	PXUIElement dataPick;
	PXClear(PXUIElement, &dataPick);
	button.Type = PXUIElementTypeDatePicker;
	button.Poisition2D.X = 350;
	button.Poisition2D.Y = 80;
	button.Poisition2D.Width = 300;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);


	PXUIElement listbox;
	PXClear(PXUIElement, &listbox);
	button.Type = PXUIElementTypeListBox;
	button.Poisition2D.X = 350;
	button.Poisition2D.Y = 150;
	button.Poisition2D.Width = 300;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);

	while(1)
	{
		PXWindowUpdate(&pxWindow);
		PXWindowFrameBufferSwap(&pxWindow);
	}


	PXWindowDestruct(&pxWindow);
}