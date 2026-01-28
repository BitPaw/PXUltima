#pragma once

#ifndef PXGraphicsCardIncluded
#define PXGraphicsCardIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Monitor/PXMonitor.h>
#include <PX/Engine/ECS/PXECS.h>

#define PXDeviceDisplaySize 32
#define PXDeviceNameSize 32
#define PXDeviceIDSize 64
#define PXDeviceKeySize 128

#define PXDeviceOpenGLDriverSize 32
#define PXDeviceOpenGLVendorSize 32
#define PXDeviceOpenGLRendererSize 32
#define PXDeviceOpenGLShaderSize 32

typedef struct PXGraphicsCard_
{
    char DeviceDisplay[PXDeviceDisplaySize]; // \\.\DISPLAY6
    char DeviceName[PXDeviceNameSize]; // NVIDIA GeForce GTX 1080
    char DeviceID[PXDeviceIDSize]; // Windows DeviceID, "PCI\VEN_10DE&DEV_1B80&SUBSYS_336..."
    char DeviceKey[PXDeviceKeySize]; // Windows Regestry "\Registry\Machine\System\Current..."

    char Driver[PXDeviceOpenGLDriverSize]; // xxxxx.DLL

    char Vendor[PXDeviceOpenGLVendorSize];
    char Renderer[PXDeviceOpenGLRendererSize];
    char Shader[PXDeviceOpenGLShaderSize];

    PXI64U VideoMemoryDedicated; // dedicated video memory, total size (in kb) of the GPU memory
    PXI64U VideoMemoryCurrent; // total available memory, total size (in Kb) of the memory available for allocations
    PXI64U VideoMemoryTotal; // current available dedicated video memory (in kb), currently unused GPU memory

    PXI64U VideoMemoryEvictionCount; // How many times memory got displaced to Main-RAM
    PXI64U VideoMemoryEvictionSize; // size of total video memory evicted (in kb)

    PXBool IsConnectedToMonitor;

    PXMonitor* AttachedMonitor;
}
PXGraphicsCard;


PXPublic PXResult PXAPI PXGraphicsCardRegisterToECS();
PXPublic PXResult PXAPI PXGraphicsCardFetchList();

#endif