#include "PXFrameBuffer.h"

const char PXFrameBufferText[] = "FrameBuffer";
const PXI8U PXFrameBufferTextLength = sizeof(PXFrameBufferText);
const PXECSRegisterInfoStatic PXFrameBufferRegisterInfoStatic =
{
    {sizeof(PXFrameBufferTextLength), sizeof(PXFrameBufferTextLength), PXFrameBufferText, TextFormatASCII},
    sizeof(PXFrameBuffer),
    __alignof(PXFrameBuffer),
    PXECSTypeResource,
    PXFrameBufferCreate
};
PXECSRegisterInfoDynamic PXFrameBufferRegisterInfoDynamic;

PXResult PXAPI PXFrameBufferRegisterToECS()
{
    PXECSRegister(&PXFrameBufferRegisterInfoStatic, &PXFrameBufferRegisterInfoDynamic);

    return PXActionSuccessful;
}

PXResult PXAPI PXFrameBufferCreate(PXFrameBuffer** pxFrameBufferREF, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo)
{
    PXFrameBuffer* pxFrameBuffer = 0;

    switch(pxFrameBufferCreateInfo->System)
    {
        case PXGraphicSystemNative:
        {
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

            break;
        }
        default:
        {
            break;
        }
    }

    return PXActionSuccessful;
}