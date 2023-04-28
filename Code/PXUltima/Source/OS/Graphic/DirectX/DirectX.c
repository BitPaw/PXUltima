#include "DirectX.h"

#if OSUnix
#elif OSWindows
#include <windows.h>
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
//#include <d3d12.h>

#if WindowsAtleastVista
#include <d3d12.h>
#else
#include <d3d9.h>
#endif

#pragma comment(lib, "D3d9.lib")
#endif

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#include <stdio.h>

void DirectXContextConstruct(DirectXContext* const directXContext)
{
	MemoryClear(directXContext, sizeof(DirectXContext));
}

void DirectXContextDestruct(DirectXContext* const directXContext)
{
#if OSUnix
#elif OSWindows
#endif
}

void DirectXContextCreate(DirectXContext* const directXContext)
{
	DirectXContextConstruct(directXContext);

#if OSUnix
#elif OSWindows

	IDirect3D9* const context = Direct3DCreate9(D3D_SDK_VERSION);
	const PXBool success = context;

	if (!success)
		return E_FAIL;

	directXContext->ContextD9 = context;

	const unsigned int amountOfAdapters = context->lpVtbl->GetAdapterCount(context);

	for (PXSize i = 0; i < amountOfAdapters; ++i)
	{
		D3DADAPTER_IDENTIFIER9 aDAPTER_IDENTIFIER9;

		MemoryClear(&aDAPTER_IDENTIFIER9, sizeof(D3DADAPTER_IDENTIFIER9));

		context->lpVtbl->GetAdapterIdentifier(context, i, 0, &aDAPTER_IDENTIFIER9);

		PXTextCopyA(aDAPTER_IDENTIFIER9.Driver, MAX_DEVICE_IDENTIFIER_STRING, directXContext->Driver, MAX_DEVICE_IDENTIFIER_STRING);
		PXTextCopyA(aDAPTER_IDENTIFIER9.Description, MAX_DEVICE_IDENTIFIER_STRING, directXContext->Description, MAX_DEVICE_IDENTIFIER_STRING);
		PXTextCopyA(aDAPTER_IDENTIFIER9.DeviceName, 32, directXContext->DeviceName, 32);
	}

	/*
		LPDIRECT3DDEVICE9 pDevice = NULL;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;*/




	//D3DCAPS9 xxD3DCAPS9;
	//g_pD3D->lpVtbl->GetDeviceCaps(g_pD3D, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &xxD3DCAPS9);


	/*


	Direct3DCreate9();

	Direct3DCreate9Ex();


	if (FAILED(g_pD3D.CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,	D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &d3dDevice)))
		return E_FAIL;* /
	*/
#endif
}