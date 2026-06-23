#include "PXUI.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/YAML/PXYAML.h>
#include <PX/OS/PXOS.h>

typedef struct PXUI_
{
    PXECSInfo Info;

    int x;
}
PXUI;

const char PXUIText[] = "UI-PX";
const PXI8U PXUITextLength = sizeof(PXUIText);
const PXECSRegisterInfoStatic PXUIInfoStatic =
{
    {sizeof(PXUIText), sizeof(PXUIText), PXUIText, TextFormatASCII},
    sizeof(PXUI),
    __alignof(PXUI),
    PXECSTypeSystem,
    PXUICreate,
    PXUIRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXUIInfoDynamic;






const char* PXBackGroundMatches[] =
{
    "Red",
    "Green",
    "Blue",
};
const PXI8U PXBackGroundMatchesAmount = sizeof(PXBackGroundMatches) / sizeof(char*);

#define PXBackGround_Red   0
#define PXBackGround_Green 1
#define PXBackGround_Blue  2


const char* PXPaddingMatches[] =
{
    "Left",
    "Top",
    "Right",
    "Bottom",
};
const PXI8U PXPaddingMatchesAmount = sizeof(PXPaddingMatches) / sizeof(char*);

#define PXPadding_Left      0
#define PXPadding_Top       1
#define PXPadding_Right     2
#define PXPadding_Bottom    3



const char* PXKeyMatchesLV1[] =
{
    "Window" 
};
const PXI8U PXKeyMatchesLV1Amount = sizeof(PXKeyMatchesLV1) / sizeof(char*);

#define UI_Window          0

const char* PXWindowPropertyMatchesLV2[] =
{
    "Name",
    "Parent",
    "DockSide",
};
const PXI8U PXWindowPropertyLV2Amount = sizeof(PXWindowPropertyMatchesLV2) / sizeof(char*);

#define UI_Window_Name          0
#define UI_Window_Parent        1
#define UI_Window_DockSide      2


const char* PXWindowClassesMatchesLV2[] =
{
    "BackGround",
    "Padding"
};
const PXI8U PXWindowClassesMatchesLV2Amount = sizeof(PXWindowClassesMatchesLV2) / sizeof(char*);

#define UI_Window_BackGround    0
#define UI_Window_Padding       1





PXResult PXAPI PXUILoadFromFile(PXUI PXREF pxUI, PXText PXREF pxFilePath)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "Path:%s",
        pxFilePath->A
    );
#endif

    PXDOM* pxDOM = 0;
    PXDOMCreate(&pxDOM);

    // Load expected UI file
    {
        PXECSCreateInfo pxECSCreateInfo;
        PXClear(PXECSCreateInfo, &pxECSCreateInfo);
        pxECSCreateInfo.FilePath = *pxFilePath;

        PXECSLoad(pxDOM, &pxECSCreateInfo);
    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "DONE"
    );
#endif


    char buffer[128];

    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));



    PXSize amount = PXDOMKeyAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "%-18s : %i", 
        "[DOM-Keys]", amount
    );
#endif

#if 0
    for(size_t i = 0; i < amount; i++)
    {
        PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, i);

        PXTextClear(&pxText);
        PXTextAppendA(&pxText, pxDOMKey->Symbol.Source, pxDOMKey->Symbol.Size);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
            "Load",
            "%-18s : %s",
            "DOM-Key", pxText.A
        );
#endif
    }
#endif


    amount = PXDOMPropertyAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Propertys]",amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMProperty PXREF pxDOMClassValue = PXDOMPropertyGet(pxDOM, i);

        char bufferValue[256];
        PXText pxTextValue;
        PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

        PXTextClear(&pxText);
        PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
            "Load",
            "%-18s : [%2.2i] (P:%i) %-30s ~> <%s>",
            "DOM-Property", i, pxDOMClassValue->ParentObjectIndex, pxText.A,
            pxTextValue.A
        );
#endif
    }


    amount = PXDOMClassAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Classes]", amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMClass PXREF pxDOMClass = PXDOMClassGet(pxDOM, i);

        PXTextClear(&pxText);
        PXDOMClassToString(pxDOM, pxDOMClass, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
            "Load",
            "%-18s : %-30s",
            "DOM-Class", pxText.A,
            i
        );
#endif
    }




    amount = PXDOMObjectAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "%-18s : %i", 
        "[DOM-Object]",amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMObject PXREF pxDOMObject = PXDOMObjectGet(pxDOM, i);

        PXTextClear(&pxText);
        PXDOMObjectToString(pxDOM, pxDOMObject, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
            "Load",
            "%-18s : [%2i][P:%5i][S:%5i][C:%5i] C:[%i] P:[%2.2i]_%-30s",
            "DOM-Object", 
            i,       
            pxDOMObject->ObjectIndexParent,
            pxDOMObject->ObjectIndexSibling,
            pxDOMObject->ObjectIndexChild,
            pxDOMObject->ClassIndex,
            pxDOMObject->PropertyIndexCount,
            pxText.A
        );
#endif

        for(size_t i = 0; i < pxDOMObject->PropertyIndexCount; i++)
        {
            PXDOMIndex propertyStart = pxDOMObject->PropertyIndexStart + i;
            PXDOMProperty PXREF pxDOMClassValue = PXDOMPropertyGet(pxDOM, propertyStart);

            char bufferValue[256];
            PXText pxTextValue;
            PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

            PXTextClear(&pxText);
            PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXUIText,
                "Load",
                "%-18s : [%2.2i] (P:%i) %-30s ~> <%s>",
                "DOM-Property", i, pxDOMClassValue->ParentObjectIndex, pxText.A,
                pxTextValue.A
            );
#endif
        }
    }



    //PXDOMPrintTreeIterative(pxDOM);



    PXDOMCursor pxDOMCursor;
    PXDOMCursorStart(pxDOM, &pxDOMCursor);


    PXDOMCursorUpdate(pxDOM, &pxDOMCursor);
    
    PXDOMCursorNext(pxDOM, &pxDOMCursor);
  
    PXI8U match = 0;
    PXBool yeet = 0;

    while(PXDOMCursorUpdate(pxDOM, &pxDOMCursor))
    {
        match = PXDOMCursorKeyCompare(pxDOM, &pxDOMCursor, PXKeyMatchesLV1, PXKeyMatchesLV1Amount);

       

        switch(match)
        {
            default:
            {
                yeet = 1;
                break;
            }
            case UI_Window:
            {
                PXWindowCreateInfo pxWindowCreateInfo;
                PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
                   

                while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                {
                    match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXWindowPropertyMatchesLV2, PXWindowPropertyLV2Amount);

                    PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                    switch(match)
                    {
                        case UI_Window_Name:
                        {
                            PXTextFromAdressA
                            (
                                &pxWindowCreateInfo.Info.Name,
                                symbol->Source,
                                symbol->Size,
                                symbol->Size
                            );
                            break;
                        }
                        case UI_Window_Parent:
                        {
                            PXTextFromAdressA
                            (
                                &pxWindowCreateInfo.WindowParentName,
                                symbol->Source,
                                symbol->Size,
                                symbol->Size
                            );
                            break;
                        }
                        case UI_Window_DockSide:
                        {
                            break;
                        }
                        default:
                        {
#if PXLogEnable 
                            PXTextClear(&pxText);
                            // PXTextAppendA(&pxText, symbol->Source, symbol->Size);

                            PXLogPrint
                            (
                                PXLoggingWarning,
                                PXUIText,
                                "TR",
                                "unexpected propertydetected : <%s>",
                                "---"
                            );
#endif

                            break;
                        }
                    }

                    PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                }



                PXDOMCursorNext(pxDOM, &pxDOMCursor);

                // Sub classes
                while(PXDOMCursorUpdate(pxDOM, &pxDOMCursor))
                {
                    match = PXDOMCursorKeyCompare(pxDOM, &pxDOMCursor, PXWindowClassesMatchesLV2, PXWindowClassesMatchesLV2Amount);
                    
                    int height = pxDOMCursor.indent;

                    switch(match)
                    {
                        default:
                        {
                            yeet = 1;
                            break;
                        }
                        case UI_Window_BackGround:
                        {
                            PXColorRGBI8 PXREF pxXColorRGBI8 = &pxWindowCreateInfo.BackGroundColor;
                                               
                            while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                            {
                                match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXBackGroundMatches, PXBackGroundMatchesAmount);

                                PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                                switch(match)
                                {
                                    case PXBackGround_Red:
                                    {
                                        pxXColorRGBI8->Red = symbol->I32U;
                                        break;
                                    }
                                    case PXBackGround_Green:
                                    {
                                        pxXColorRGBI8->Green = symbol->I32U; // 255.0f;
                                        break;
                                    }
                                    case PXBackGround_Blue:
                                    {
                                        pxXColorRGBI8->Blue = symbol->I32U;
                                        break;
                                    }
                                }

                                PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                            }                 

                            break;
                        }
                        case UI_Window_Padding:
                        {
                            PXRectangleLTRBI32 PXREF padding = &pxWindowCreateInfo.Layout.Padding;

                            while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                            {
                                match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXPaddingMatches, PXPaddingMatchesAmount);

                                PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                                switch(match)
                                {
                                    case PXPadding_Left:
                                    {
                                        padding->Left = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Top:
                                    {
                                        padding->Top = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Right:
                                    {
                                        padding->Right = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Bottom:
                                    {
                                        padding->Bottom = symbol->I32U;
                                        break;
                                    }
                                }

                                PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                            }

                            break;
                        }
                    }

                    PXDOMCursorNext(pxDOM, &pxDOMCursor);

                    if(height > pxDOMCursor.indent)
                    {
                        break;
                    }

                    if(yeet)
                    {
                        break;
                    }
                } 

                PXWindow* pxWindow = PXNull;

                PXWindowCreate(&pxWindow, &pxWindowCreateInfo);

                break;
            }                
        }

        if(yeet)
        {
            break;
        }

        //PXDOMCursorNext(pxDOM, &pxDOMCursor);
    }


    


#if 0
    for(;;)
    {
        // print
        pxDOMObjectCurrent = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->FirstChild);



        pxDOMObjectChild = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->FirstChild);
        pxDOMObjectSibling = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->NextSibling);

        // Go nested
        for(PXSize i = 0; i < pxDOMObject->; i++)
        {

        }

        // Go properties
        for(PXSize i = 0; i < length; i++)
        {

        }


    }
#endif




#if 0
    PXSlice pxSlice;
    pxSlice.Address = "UI.Window";
    pxSlice.Length = 9;

    PXDOMKey PXREF pxDOMClass = PXDOMKeyFindFirst(pxDOM, &pxSlice);

    if(!pxDOMClass)
    {
        // No UI windows to load?
        return PXResultOK;
    }


    for(PXSize i = 0; i < pxDOMClass->objectCount; i++) 
    {
        ObjectRange r = windows->objects[i];
        for(int k = r.start; k <= r.end; k++) {
            // process KV entries for this object
        }
    }
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingWarning,
        PXUIText,
        "Load",
        "DONE!"
    );
#endif


    return PXResultOK;
}

PXResult PXAPI PXUICreate(PXUI** pxUIREF, PXUIInfo PXREF pxUIInfo)
{
    PXUI* pxUI = PXNull;

    if(!(pxUIREF && pxUIInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxUIInfo->Info.Static = &PXUIInfoStatic;
    pxUIInfo->Info.Dynamic = &PXUIInfoDynamic;
    PXResult pxResult = PXECSCreate(pxUIREF, pxUIInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxUI = *pxUIREF;
    
    PXUILoadFromFile(pxUI, &pxUIInfo->Path);

    return PXResultOK;
}

PXResult PXAPI PXUIRelease(PXUI PXREF pxUI)
{
    return PXResultOK;
}