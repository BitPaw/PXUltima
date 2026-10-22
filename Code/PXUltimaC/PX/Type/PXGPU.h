#pragma once

#ifndef PXGPUIncluded
#define PXGPUIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXMonitor.h>
#include <PX/Engine/ECS/PXECS.h>

#define PXDeviceDisplaySize 32
#define PXDeviceNameSize 32
#define PXDeviceIDSize 64
#define PXDeviceKeySize 128

#define PXDeviceOpenGLDriverSize 32
#define PXDeviceOpenGLVendorSize 32
#define PXDeviceOpenGLRendererSize 32
#define PXDeviceOpenGLShaderSize 32

typedef struct PXGPU_
{
    PXECSInfo Info;

#if OSUnix
#elif OSWindows
    LUID AdapterLUID;
    HMONITOR MonitorHandle;
#endif

    char Name[PXDeviceNameSize]; // NVIDIA GeForce GTX 1080


    char DeviceDisplayName[PXDeviceDisplaySize]; // \\.\DISPLAY6

    char DeviceIDName[PXDeviceIDSize]; // Windows DeviceID, "PCI\VEN_10DE&DEV_1B80&SUBSYS_336..."
    char DeviceKeyName[PXDeviceKeySize]; // Windows Regestry "\Registry\Machine\System\Current..."

    char DriverName[PXDeviceOpenGLDriverSize]; // xxxxx.DLL

    char VendorName[PXDeviceOpenGLVendorSize];
    char RendererName[PXDeviceOpenGLRendererSize];
    char ShaderName[PXDeviceOpenGLShaderSize];

    PXI32U VendorID; // DirectX10
    PXI32U DeviceID; // DirectX10
    PXI32U SubSysID; // DirectX10
    PXI32U Revision; // DirectX10


    PXI64U VideoMemoryUsage;
    PXF64 VideoMemoryUsageInPercent;
    PXI64U VideoMemoryDedicated; // Physical RAM on GPU.
    PXI64U VideoMemoryAllocatedOnCPUExlusive; // Memory forproessing on CPU side.
    PXI64U VideoMemoryAllocatedOnCPUShared;

    //PXI64U VideoMemoryCurrent; // total available memory, total size (in Kb) of the memory available for allocations
    //PXI64U VideoMemoryTotal; // current available dedicated video memory (in kb), currently unused GPU memory

    PXI64U VideoMemoryEvictionCount; // How many times memory got displaced to Main-RAM
    PXI64U VideoMemoryEvictionSize; // size of total video memory evicted (in kb)

    PXBool IsActive;
    PXBool IsConnectedToMonitor;

    PXMonitor* AttachedMonitor;
}
PXGPU;


PXPublic void PXAPI PXGPURegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXGPUFetchList();

PXPublic PXResult PXAPI PXGPUUsageUpdate(PXGPU PXREF pxGPU);

#endif