#include "PXDirectX12.h"

#if PXDX12Enable
#include <d3d12.h>
#include <d3d12sdklayers.h>
//#include <DXGI.h>
//#include <dxgi1_3.h>
#include <dxgi1_4.h>

#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "DXGUID.LIB")
#pragma comment(lib, "uuid.lib")

#endif

#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>

PXResult PXAPI PXDirectX12Initialize(PXDirectX12 PXREF pxDirectX12, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX12",
        "Initialize",
        "Start..."
    );
#endif
#if PXDX12Enable

    // Enable the debug layer.
    {
        ID3D12Debug* debugController = PXNull;
        // IID_PPV_ARGS()
        const HRESULT xx = D3D12GetDebugInterface // D3D12.dll, d3d12.h
                           (
                               &CLSID_D3D12Debug,
                               &debugController
                           );

        debugController->lpVtbl->EnableDebugLayer(debugController);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX12",
            "Initialize",
            "Debug"
        );
#endif
    }

    // Factory
    {
        const HRESULT resultID = CreateDXGIFactory(&IID_IDXGIFactory, &pxDirectX12->GIFactory);
    }

    // IDXGIFactory1::EnumAdapters.

    // Create the device.
    {
        const HRESULT createDeviceResultID = D3D12CreateDevice // D3D12.dll, d3d12.h
                                             (
                                                     PXNull, // VideoAdapter
                                                     D3D_FEATURE_LEVEL_12_0,
                                                     &IID_ID3D12Device,
                                                     &pxDirectX12->Device
                                             );

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX12",
            "Initialize",
            "Device"
        );
#endif
    }

    // Create the command queue.
    {
        D3D12_COMMAND_QUEUE_DESC D3D12CommandQueueDesc;
        PXClear(D3D12_COMMAND_QUEUE_DESC, &D3D12CommandQueueDesc);
        D3D12CommandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        D3D12CommandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        const HRESULT resultID = pxDirectX12->Device->lpVtbl->CreateCommandQueue // D3D12.dll, d3d12.h
                                 (
                                     pxDirectX12->Device,
                                     &D3D12CommandQueueDesc,
                                     &IID_ID3D12CommandQueue,
                                     &pxDirectX12->CommandQueue
                                 );

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX12",
            "Initialize",
            "Command Queue"
        );
#endif
    }

    // Create the swap chain.
    {
        DXGI_SWAP_CHAIN_DESC dxGISwapChainDescription;
        dxGISwapChainDescription.BufferCount = pxGraphicInitializeInfo->TargetFrameRate;
        dxGISwapChainDescription.BufferDesc.Width = pxGraphicInitializeInfo->Width;
        dxGISwapChainDescription.BufferDesc.Height = pxGraphicInitializeInfo->Height;
        dxGISwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxGISwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxGISwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        dxGISwapChainDescription.OutputWindow = pxGraphicInitializeInfo->WindowReference->ID;
        dxGISwapChainDescription.SampleDesc.Count = 1;
        dxGISwapChainDescription.Windowed = TRUE;

        const HRESULT resultID = pxDirectX12->GIFactory->lpVtbl->CreateSwapChain // Windows 10, Dxgi.lib, dxgi1_4.h
                                 (
                                     pxDirectX12->GIFactory,
                                     pxDirectX12->Device,
                                     &dxGISwapChainDescription,
                                     &pxDirectX12->SwapChain
                                 );

        //const UINT index = GetCurrentBackBufferIndex();

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX12",
            "Initialize",
            "SwapChain"
        );
#endif
    }

    // Create a render target view(RTV) descriptor heap.
    // Note:    A descriptor heap can be thought of as an array of descriptors.
    //          Where each descriptor fully describes an object to the GPU.
    {
        D3D12_DESCRIPTOR_HEAP_DESC descriptiorHeapDescription;
        descriptiorHeapDescription.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        descriptiorHeapDescription.NumDescriptors = pxGraphicInitializeInfo->TargetFrameRate;
        descriptiorHeapDescription.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        descriptiorHeapDescription.NodeMask = 0;

        const HRESULT resultID = pxDirectX12->Device->lpVtbl->CreateDescriptorHeap
                                 (
                                     pxDirectX12->Device,
                                     &descriptiorHeapDescription,
                                     &IID_ID3D12DescriptorHeap,
                                     &pxDirectX12->HeapDescriptor
                                 );

        // const UINT number = GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX12",
            "Initialize",
            "HeapDescriptor"
        );
#endif
    }

    // Create frame resources(a render target view for each frame).
    {
        D3D12_CPU_DESCRIPTOR_HANDLE* d3d12CPUDescriptorHandle;

        pxDirectX12->HeapDescriptor->lpVtbl->GetGPUDescriptorHandleForHeapStart(pxDirectX12->HeapDescriptor, &d3d12CPUDescriptorHandle);

        // Create a RTV for each frame.
        for(UINT n = 0; n < pxGraphicInitializeInfo->TargetFrameRate; n++)
        {
            //ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
            //m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
            //rtvHandle.Offset(1, m_rtvDescriptorSize);
        }

        // Create a command allocator.
        {
            const HRESULT resultID = pxDirectX12->Device->lpVtbl->CreateCommandAllocator
                                     (
                                         pxDirectX12->Device,
                                         D3D12_COMMAND_LIST_TYPE_DIRECT,
                                         &IID_ID3D12CommandAllocator,
                                         &pxDirectX12->CommandAllocator
                                     );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX12",
                "Initialize",
                "Command allocator"
            );
#endif
        }
    }





#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX12",
        "Initialize",
        "Done...\n"
    );
#endif

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXDirectX12Release(PXDirectX12 PXREF pxDirectX12)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX12DevicePhysicalListAmountFunction(PXDirectX12 PXREF pxDirectX12, PXI32U PXREF amountOfAdapters)
{
#if 0
    D3D_FEATURE_LEVEL featureLevel;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion12Emulate1x0Core:
        featureLevel = D3D_FEATURE_LEVEL_1_0_CORE;
        break;
    case PXDirectXVersion12Emulate9x1:
        featureLevel = D3D_FEATURE_LEVEL_9_1;
        break;
    case PXDirectXVersion12Emulate9x2:
        featureLevel = D3D_FEATURE_LEVEL_9_2;
        break;
    case PXDirectXVersion12Emulate9x3:
        featureLevel = D3D_FEATURE_LEVEL_9_3;
        break;
    case PXDirectXVersion12Emulate10x0:
        featureLevel = D3D_FEATURE_LEVEL_10_0;
        break;
    case PXDirectXVersion12Emulate10x1:
        featureLevel = D3D_FEATURE_LEVEL_10_1;
        break;
    case PXDirectXVersion12Emulate11x0:
        featureLevel = D3D_FEATURE_LEVEL_11_0;
        break;
    case PXDirectXVersion12Emulate11x1:
        featureLevel = D3D_FEATURE_LEVEL_11_1;
        break;
    case PXDirectXVersion12Emulate12x0:
        featureLevel = D3D_FEATURE_LEVEL_12_0;
        break;
    case PXDirectXVersion12Emulate12x1:
        featureLevel = D3D_FEATURE_LEVEL_12_1;
        break;
    case PXDirectXVersion12Emulate12x2:
        featureLevel = D3D_FEATURE_LEVEL_12_2;
        break;
    }

    void* adpater = 0;

    const HRESULT result = D3D12CreateDevice
                           (
                               adpater,
                               featureLevel,
                               0,
                               0
                           );
#endif

    return PXActionSuccessful;
}
