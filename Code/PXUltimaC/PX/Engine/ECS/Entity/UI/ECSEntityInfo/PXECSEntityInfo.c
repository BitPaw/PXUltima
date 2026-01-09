#include "PXECSEntityInfo.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXGUI.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>

const char PXECSEntityInfoText[] = "ECS-EntityInfo";
const PXI8U PXECSEntityInfoTextLength = sizeof(PXECSEntityInfoText);
const PXECSRegisterInfoStatic PXECSEntityInfoInfoStatic =
{
    {sizeof(PXECSEntityInfoText), sizeof(PXECSEntityInfoText), PXECSEntityInfoText, TextFormatASCII},
    sizeof(PXECSEntityInfo),
    __alignof(PXECSEntityInfo),
    PXECSTypeEntity,
    PXECSEntityInfoCreate,
    PXNull,
    PXECSEntityInfoDraw
};
PXECSRegisterInfoDynamic PXECSEntityInfoInfoDynamic;

PXResult PXAPI PXECSEntityInfoRegisterToECS()
{
    PXECSRegister(&PXECSEntityInfoInfoStatic, &PXECSEntityInfoInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXECSEntityInfoCreate(PXECSEntityInfo** pxECSEntityInfo, PXECSEntityInfoCreateInfo PXREF pxECSEntityInfoCreateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXECSEntityInfoDraw(PXECSEntityInfo PXREF pxECSEntityInfo, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
       PXGUITheme* pxGUITheme = PXGUIThemeGet();
       PXWindow PXREF pxWindow = pxECSEntityInfo->WindowBase;
       PXECSInfo* pxECSInfo = (PXECSInfo*)pxWindow;

   #if PXLogEnable
       PXLogPrint
       (
           PXLoggingInfo,
           PXECSEntityInfoText,
           "Draw",
           "--- PXID:%i, ResourceInfo --- START",
           pxECSInfo->ID
       );
   #endif

       PXWindowDrawRectangle2D
       (
           pxWindow,
           pxWindowDrawInfo
       );


       char buffer[260];

       PXText pxText;
       PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

       PXTextPrint(&pxText, "PXID:%i", pxECSInfo->ID);

       PXWindowDrawText(pxWindow, pxWindowDrawInfo, &pxText);
       

       /*

       PXDictionaryEntry pxDictionaryEntry;

       PXDictionary* pxDictionary = &PXResourceManagerGet()->GUIElementLookup;

       PXDictionaryIndex(pxDictionary, 0, &pxDictionaryEntry);

       PXECSInfo = &(*(PXWindow**)pxDictionaryEntry.Value)->Info;

       PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);
       PXNativDrawColorSetBrush
       (
           &_pxGUIManager.NativDraw,
           pxWindow,
           pxGUITheme->TextWhite,
           PXGUIDrawModeFront
       );
       PXNativDrawColorSetBrush
       (
           &_pxGUIManager.NativDraw,
           pxWindow,
           pxGUITheme->MainPrimary,
           PXGUIDrawModeBack
       );


       char nameID[32];

       PXSize textSize = PXTextPrintA(nameID, 32, "ID: %i", PXECSInfo->ID);

       PXNativDrawA
       (
           &_pxGUIManager.NativDraw,
           pxWindow,
           pxWindow->Position.Form.X,
           pxWindow->Position.Form.Y,
           pxWindow->Position.Form.Width,
           pxWindow->Position.Form.Height,
           nameID,
           textSize,
           PXWindowAllignLeft
       );

       PXIcon pxIcon;

       PXGUIIconLoad(&pxIcon);



       int left = pxWindow->Position.Form.X;
       int y = 16 + 10;
       int height = 16;


       char* table[16] =
       {
           "Exist",
           "Active",
           "Render",

           "Drive",
           "Cached",
           "Memory",
           "Device",

           "Name",
           "Source",

           "OS",
           "User",
           "Engine",
           "Undefined",

           "READ",
           "WRITE",
           "EXECUTE",
       };


       for(size_t i = 0; i < 16; i++)
       {
           PXUIElementPosition pxUIElementPositionPrev = pxWindow->Position;

           pxWindow->Position.Form.X = left;
           pxWindow->Position.Form.Y = y;
           // pxWindow->Position.Right = pxWindow->Position.Right;
           pxWindow->Position.Form.Height = y + height;
           pxWindow->Info.Setting &= ~PXWindowAllignFlags;
           pxWindow->Info.Setting |= PXWindowAllignLeft;


           PXNativDrawIconDraw
           (
               &_pxGUIManager.NativDraw,
               pxWindow,
               &pxIcon,
               pxWindow->Position.Form.X + 2,
               pxWindow->Position.Form.Y,
               16,
               16
           );

           pxWindow->Position.Form.X += 16 + 4;


           PXSize len = PXTextLengthA(table[i], 30);

           PXNativDrawA
           (
               &_pxGUIManager.NativDraw,
               pxWindow,
               pxWindow->Position.Form.X,
               pxWindow->Position.Form.Y,
               pxWindow->Position.Form.Width,
               pxWindow->Position.Form.Height,
               table[i],
               len,
               PXWindowAllignLeft
           );



           pxWindow->Position.Form.X += 120;

           char* textxx = (PXECSInfo->Behaviour & (1 << i)) != 0 ? "Yes" : "No";
           len = PXTextLengthA(textxx, 4);

           PXNativDrawA
           (
               &_pxGUIManager.NativDraw,
               pxWindow,
               pxWindow->Position.Form.X,
               pxWindow->Position.Form.Y,
               pxWindow->Position.Form.Width,
               pxWindow->Position.Form.Height,
               textxx,
               len,
               PXWindowAllignLeft
           );


           pxWindow->Position = pxUIElementPositionPrev;


           y += height + 3;
       }
       */


#if PXLogEnable
PXLogPrint
(
    PXLoggingInfo,
    PXECSEntityInfoText,
    "Draw",
    "--- PXID:%i, ResourceInfo --- END",
    pxECSInfo->ID
);
#endif

    return PXResultOK;
}
