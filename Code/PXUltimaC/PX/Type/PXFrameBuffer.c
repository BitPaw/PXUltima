#include "PXFrameBuffer.h"

const char PXFrameBufferText[] = "FrameBuffer";
const PXI8U PXFrameBufferTextLength = sizeof(PXFrameBufferText);
const PXECSRegisterInfoStatic PXFrameBufferRegisterInfoStatic =
{
    {sizeof(PXFrameBufferTextLength), sizeof(PXFrameBufferTextLength), PXFrameBufferText, TextFormatASCII},
    sizeof(PXFrameBuffer),
    __alignof(PXFrameBuffer),
    PXECSTypeResource,
    PXFrameBufferCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXFrameBufferRegisterInfoDynamic;

void PXAPI PXFrameBufferRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXFrameBufferRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXFrameBufferRegisterInfoDynamic;
}

PXResult PXAPI PXFrameBufferCreate(PXFrameBuffer** pxFrameBufferREF, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo)
{
    PXFrameBuffer* pxFrameBuffer = 0;

    switch(0) // pxFrameBufferCreateInfo->System
    {
        case PXGraphicAPINative:
        {
#if OSUnix



#elif OSWindows
            PXWindow* pxWindow = pxFrameBufferCreateInfo->WindowTarget;
            HDC windowDC = PXWindowDCGet(pxWindow);
            HDC memoryDeviceContext = CreateCompatibleDC(windowDC);
            HBITMAP framebuffer = CreateCompatibleBitmap
            (
                memoryDeviceContext,
                pxFrameBufferCreateInfo->Width,
                pxFrameBufferCreateInfo->Height
            );

            pxFrameBuffer->GDI.MemoryDeviceContext = memoryDeviceContext;
            pxFrameBuffer->GDI.FrameBufferTexture = framebuffer;
            pxFrameBuffer->Width = pxFrameBufferCreateInfo->Width;
            pxFrameBuffer->Height = pxFrameBufferCreateInfo->Height;

            HGDIOBJ gdiObject = SelectObject(memoryDeviceContext, framebuffer);
#endif       

            break;
        }
        default:
        {
            break;
        }
    }

    return PXResultOK;
}