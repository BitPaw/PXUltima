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


	PXUIElement textA;
	PXClear(PXUIElement, &textA);
	textA.Type = PXUIElementTypeText;
	textA.Poisition2D.X = 20;
	textA.Poisition2D.Y = 20;
	textA.Poisition2D.Width = 150;
	textA.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&textA, &pxWindow);


	PXUIElement button;
	PXClear(PXUIElement, &button);
	button.Type = PXUIElementTypeButton;
	button.Poisition2D.X = 20;
	button.Poisition2D.Y = 70;
	button.Poisition2D.Width = 150;
	button.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&button, &pxWindow);


	PXUIElement dropDown;
	PXClear(PXUIElement, &dropDown);
	dropDown.Type = PXUIElementTypeComboBox;
	dropDown.Poisition2D.X = 20;
	dropDown.Poisition2D.Y = 120;
	dropDown.Poisition2D.Width = 150;
	dropDown.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&dropDown, &pxWindow);


	PXUIElement textEdit;
	PXClear(PXUIElement, &textEdit);
	textEdit.Type = PXUIElementTypeTextEdit;
	textEdit.Poisition2D.X = 20;
	textEdit.Poisition2D.Y = 170;
	textEdit.Poisition2D.Width = 150;
	textEdit.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&textEdit, &pxWindow);


	PXUIElement progressBar;
	PXClear(PXUIElement, &progressBar);
	progressBar.Type = PXUIElementTypeProgressBar;
	progressBar.Poisition2D.X = 20;
	progressBar.Poisition2D.Y = 220;
	progressBar.Poisition2D.Width = 150;
	progressBar.Poisition2D.Height = 35;
	progressBar.ProgressBar.Percentage = 0.7f;
	PXUIElementCreateOSStyle(&progressBar, &pxWindow);


	PXUIElement listbox;
	PXClear(PXUIElement, &listbox);
	listbox.Type = PXUIElementTypeListBox;
	listbox.Poisition2D.X = 20;
	listbox.Poisition2D.Y = 270;
	listbox.Poisition2D.Width = 150;
	listbox.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&listbox, &pxWindow);


	PXUIElement radioButtonA;
	PXClear(PXUIElement, &radioButtonA);
	radioButtonA.Type = PXUIElementTypeRadioButton;
	radioButtonA.Poisition2D.X = 20;
	radioButtonA.Poisition2D.Y = 320;
	radioButtonA.Poisition2D.Width = 60;
	radioButtonA.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&radioButtonA, &pxWindow);
	
	PXUIElement radioButtonB;
	PXClear(PXUIElement, &radioButtonB);
	radioButtonB.Type = PXUIElementTypeRadioButton;
	radioButtonB.Poisition2D.X = 100;
	radioButtonB.Poisition2D.Y = 320;
	radioButtonB.Poisition2D.Width = 60;
	radioButtonB.Poisition2D.Height = 35;
	PXUIElementCreateOSStyle(&radioButtonB, &pxWindow);







	while(1)
	{
		PXWindowUpdate(&pxWindow);
		PXWindowFrameBufferSwap(&pxWindow);
	}


	PXWindowDestruct(&pxWindow);
}