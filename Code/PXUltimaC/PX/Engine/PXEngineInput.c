#include "PXEngineInput.h"

#if OSUnix
#elif OSWindows
#include <Xinput.h> // XINPUT_STATE
#endif

const char PXEngineInputText[] = "Renderer-PX";
const PXI8U PXEngineInputLength = sizeof(PXEngineInputText);
const PXECSRegisterInfoStatic PXEngineInputInfoStatic =
{
    {sizeof(PXEngineInputText), sizeof(PXEngineInputText), PXEngineInputText, TextFormatASCII},
    sizeof(PXEngineInput),
    __alignof(PXEngineInput),
    PXECSTypeSystem,
    PXEngineInputCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineInputInfoDynamic;


PXResult PXAPI PXEngineInputCreate(PXEngineInput** pxEngineInputREF, PXEngineInputCreateInfo PXREF pxEngineInputCreateInfo)
{
    PXEngineInput* pxEngineInput = PXNull;

    if(!(pxEngineInputREF && pxEngineInputCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineInputCreateInfo->Info.Static = &PXEngineInputInfoStatic;
    pxEngineInputCreateInfo->Info.Dynamic = &PXEngineInputInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineInputREF, pxEngineInputCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineInput = *pxEngineInputREF;

    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineInput->ModuleInfo, PXEngineInputUpdate);

    return pxResult;
}

PXResult PXAPI PXEngineInputUpdate(PXEngineInput PXREF pxEngineInput, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
#if OSUnix
#elif OSWindows
    XINPUT_STATE inputState;
    ZeroMemory(&inputState, sizeof(inputState));

    DWORD errorCode = XInputGetState(0, &inputState); // Windows XP?, Xinput.lib, xinput.h
    PXBool isOK = ERROR_SUCCESS == errorCode;

    if(isOK)
    {
        SHORT lx = inputState.Gamepad.sThumbLX;
        SHORT ly = inputState.Gamepad.sThumbLY;
        BYTE a = (inputState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;

        printf("[XINPUT] LX=%d LY=%d A=%d\n", lx, ly, a);
    }
#endif
}