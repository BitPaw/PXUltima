#include "PXWindowTest.h"

#include <PX/Engine/PXGUI.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Engine/PXEngine.h>

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
    //PXWindow pxWindow;
    //PXWindowConstruct(&pxWindow);

    //PXWindowCreateA(&pxWindow, -1, -1, -1, -1, "TEST", PXFalse);

    //PXWindowDestruct(&pxWindow);
}

void PXAPI PXTestWindowOpenUIRender()
{
#if 0
    PXWindow pxWindow;
    PXWindowConstruct(&pxWindow);

    PXWindowCreateA(&pxWindow, -1, -1, -1, -1, "TEST-UI", PXFalse);

    

    PXWindow A;
    PXClear(PXWindow, &A);
    A.Type = PXUIElementTypeText;
    A.Position.X = 20;
    A.Position.Y = 20;
    A.Position.Width = 150;
    A.Position.Height = 35;

    PXUIElementCreateOSStyle(&A, &pxWindow);


    PXWindow button;
    PXClear(PXWindow, &button);
    button.Type = PXUIElementTypeButton;
    button.Position.X = 20;
    button.Position.Y = 70;
    button.Position.Width = 150;
    button.Position.Height = 35;
    button.Position.MarginLeft = 0.0;
    button.Position.MarginTop = 0.0;
    button.Position.MarginRight = 0.0;
    button.Position.MarginBottom = 0.00;
    //button.Position.FlagListKeep |= PXUIElementAllignTop;
    button.Position.FlagListKeep |= PXUIElementAllignRight;
    //button.Position.FlagListKeep |= PXUIElementAllignBottom;
    button.Position.FlagListKeep |= PXUIElementKeepWidth;
    //button.Position.FlagListKeep |= PXUIElementKeepHeight;
    button.Button.Text = "Button";
    PXUIElementCreateOSStyle(&button, &pxWindow);


    PXWindow dropDown;
    PXClear(PXWindow, &dropDown);
    dropDown.Type = PXUIElementTypeComboBox;
    dropDown.Position.X = 20;
    dropDown.Position.Y = 120;
    dropDown.Position.Width = 150;
    dropDown.Position.Height = 35;
    PXUIElementCreateOSStyle(&dropDown, &pxWindow);


    PXWindow textEdit;
    PXClear(PXWindow, &textEdit);
    textEdit.Type = PXUIElementTypeTextEdit;
    textEdit.Position.X = 20;
    textEdit.Position.Y = 170;
    textEdit.Position.Width = 150;
    textEdit.Position.Height = 35;
    PXUIElementCreateOSStyle(&textEdit, &pxWindow);


    PXWindow progressBar;
    PXClear(PXWindow, &progressBar);
    progressBar.Type = PXUIElementTypeProgressBar;
    progressBar.Position.X = 20;
    progressBar.Position.Y = 220;
    progressBar.Position.Width = 150;
    progressBar.Position.Height = 35;
    progressBar.ProgressBar.Percentage = 0.7f;
    PXUIElementCreateOSStyle(&progressBar, &pxWindow);


    PXWindow listbox;
    PXClear(PXWindow, &listbox);
    listbox.Type = PXUIElementTypeListBox;
    listbox.Position.X = 20;
    listbox.Position.Y = 270;
    listbox.Position.Width = 150;
    listbox.Position.Height = 35;
    PXUIElementCreateOSStyle(&listbox, &pxWindow);


    PXWindow radioButtonA;
    PXClear(PXWindow, &radioButtonA);
    radioButtonA.Type = PXUIElementTypeRadioButton;
    radioButtonA.Position.X = 20;
    radioButtonA.Position.Y = 320;
    radioButtonA.Position.Width = 60;
    radioButtonA.Position.Height = 35;
    PXUIElementCreateOSStyle(&radioButtonA, &pxWindow);
    
    PXWindow radioButtonB;
    PXClear(PXWindow, &radioButtonB);
    radioButtonB.Type = PXUIElementTypeRadioButton;
    radioButtonB.Position.X = 100;
    radioButtonB.Position.Y = 320;
    radioButtonB.Position.Width = 60;
    radioButtonB.Position.Height = 35;
    PXUIElementCreateOSStyle(&radioButtonB, &pxWindow);







    while(1)
    {
        PXWindowUpdateInfo pxWindowUpdateInfo;
        PXClear(PXWindowUpdateInfo, &pxWindowUpdateInfo);
        pxWindowUpdateInfo.UIElement = &button;
        pxWindowUpdateInfo.WindowReference = &pxWindow;
        pxWindowUpdateInfo.Property = PXUIElementPropertySize;

        PXWindowUpdate(PXNull, &pxWindowUpdateInfo, 1);

        PXWindowUpdate(&pxWindow);
        PXWindowFrameBufferSwap(&pxWindow);
    }


    PXWindowDestruct(&pxWindow);
#endif
}