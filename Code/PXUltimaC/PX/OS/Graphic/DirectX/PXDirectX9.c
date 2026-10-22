#include "PXDirectX9.h"

#if OSWindows

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/File/PXFile.h>

#if PXDX9Enable
#include <d3d9.h>
#include <d3d9types.h>

#define PXDX9IsSDKPresent 0 // change this flag if you have the DirectX SDK installed, if not, code might not be compiled!

#endif

#if PXDX9Enable
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
// #include <d3d9.h> // Already Includedd

#include <d3dcompiler.h>
#include <d3d9caps.h>
#include <d3dcompiler.h>


#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "D3d9.lib")

#if PXDX9IsSDKPresent
#include <D3DX9Shader.h>
#pragma comment(lib, "D3dx9.lib") // Only if SDK is present
#endif
#include <PX/Type/PXWindow.h>

#endif


const char PXDirectX9Text[] = "DirectX9";
const char PXDirectX9Library[] = "D3D9.DLL";
const PXI8U PXDirectX9LibraryLength = sizeof(PXDirectX9Library);

const char PXDirectX9Created[] = "Direct3DCreate9";


const char PXDirectXShaderLibrary[] = "D3DCOMPILER_43.DLL";
const PXI8U PXDirectXShaderLibraryLength = sizeof(PXDirectXShaderLibrary);

const char D3DX9_43[] = "D3DX9_43.DLL";
const PXI8U D3DX9_43Length = sizeof(D3DX9_43);




typedef IDirect3D9* (WINAPI* PXDirect3DCreate9)(UINT SDKVersion);

PXPrivate PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format);
PXPrivate D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat);
PXPrivate void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial PXREF pxMaterial, const D3DMATERIAL9 PXREF d3dMaterial);
PXPrivate void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9 PXREF d3dMaterial, const PXMaterial PXREF pxMaterial);
PXPrivate D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXDrawMode PXGraphicDrawMode);

typedef HRESULT(WINAPI* PXD3DCompile)
(
    __in_bcount(SrcDataSize) LPCVOID pSrcData,
    __in SIZE_T SrcDataSize,
    __in_opt LPCSTR pSourceName,
    __in_xcount_opt(pDefines->Name != NULL) CONST D3D_SHADER_MACRO* pDefines,
    __in_opt void* pIncluded, // ID3DINCLUDE
    __in LPCSTR pEntrypoint,
    __in LPCSTR pTarget,
    __in UINT Flags1,
    __in UINT Flags2,
    __out ID3DBlob** ppCode,
    __out_opt ID3DBlob** ppErrorMsgs
    );

#if PXDX9IsSDKPresent
typedef HRESULT(WINAPI* PXD3DXGetShaderConstantTable)(CONST DWORD* pFunction, LPD3DXCONSTANTTABLE* ppConstantTable);
typedef HRESULT(WINAPI* PXD3DXGetShaderConstantTableEx)(CONST DWORD* pFunction, DWORD Flags, LPD3DXCONSTANTTABLE* ppConstantTable);
#endif

PXResult PXAPI PXDirectX9Initialize(PXDirectX9 PXREF pxDirectX9, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "Initialize",
        "Start..."
    );
#endif

    // Open directX9 lib
    {
        PXText pxText;
        pxResult = PXTextFromAddressA(&pxText, PXDirectX9Library, PXDirectX9LibraryLength, PXDirectX9LibraryLength);

        pxResult = PXLibraryOpen(&pxDirectX9->LibraryDirect3D, &pxText);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }

    // Get init function and execute
    {
        PXDirect3DCreate9 pxDirect3DCreate;

        pxResult = PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3D, (void**)&pxDirect3DCreate, PXDirectX9Created, PXTrue);

        if(PXResultOK != pxResult)
        {
            PXLibraryClose(&pxDirectX9->LibraryDirect3D);

            return pxResult;
        }

        pxDirectX9->Context = pxDirect3DCreate(D3D_SDK_VERSION); // Create DirectX context, alternative Direct3DCreate9Ex()
        const PXBool success = pxDirectX9->Context;

        if(!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXDirectX9Text,
                "Initialize",
                "Instance creation failed",
                pxDirectX9->Context
            );
#endif

            return PXResultFailedInitialization;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectX9Text,
            "Initialize",
            "Instance Created 0x%p",
            pxDirectX9->Context
        );
#endif
    }

    // Get Additional Lib
    {
        PXText pxText;
        PXTextFromAddressA(&pxText, PXDirectXShaderLibrary, PXDirectXShaderLibraryLength, PXDirectXShaderLibraryLength);

        pxResult = PXLibraryOpen(&pxDirectX9->LibraryDirectShaderCompiler, &pxText);

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirectShaderCompiler, &pxDirectX9->ShaderCompile, "D3DCompile", PXTrue);


        PXTextFromAddressA(&pxText, D3DX9_43, D3DX9_43Length, D3DX9_43Length);

        pxResult = PXLibraryOpen(&pxDirectX9->LibraryDirect3DExtension, &pxText);

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGet, "D3DXGetShaderConstantTable", PXTrue);
        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGetEx, "D3DXGetShaderConstantTableEx", PXTrue);
    }
    
    // Merge all functions
    {
        PXEngineRender PXREF pxEngineRender = pxEngineRenderCreateInfo->RenderSystem;
        pxEngineRender->Texture1DRegister = (PXGPUTexture1DRegisterFunction)PXDirectX9Texture1DRegister;
        pxEngineRender->Texture2DRegister = (PXGPUTexture2DRegisterFunction)PXDirectX9Texture2DRegister;
        pxEngineRender->Texture3DRegister = (PXGPUTexture3DRegisterFunction)PXDirectX9Texture3DRegister;
        pxEngineRender->TextureCubeRegister = (PXGPUTextureCubeRegisterFunction)PXDirectX9TextureCubeRegister;
        pxEngineRender->ShaderVariableSet = (PXGPUShaderVariableSetFunction)PXDirectX9ShaderVariableSet;
        pxEngineRender->ScreenBufferRead = PXNull;
        pxEngineRender->DrawModeSet = PXNull;
        pxEngineRender->DrawColorRGBAF = PXNull;
        pxEngineRender->RectangleDraw = PXNull;
        pxEngineRender->RectangleDrawTx = PXNull;
        pxEngineRender->SwapIntervalSet = (PXGPUSwapIntervalSetFunction)PXDirectX9SwapIntervalSet;
        pxEngineRender->SwapIntervalGet = (PXGPUSwapIntervalGetFunction)PXDirectX9SwapIntervalGet;
        pxEngineRender->DevicePhysicalListAmount = PXNull;
        pxEngineRender->DevicePhysicalListFetch = PXNull;
        pxEngineRender->Release = PXNull;
        pxEngineRender->Select = (PXGPUSelectFunction)PXDirectX9Select;
        pxEngineRender->Deselect = (PXGPUDeselectFunction)PXDirectX9Deselect;
        pxEngineRender->Clear = (PXGPUClearFunction)PXDirectX9Clear;
        pxEngineRender->SceneDeploy = (PXGPUSceneDeployFunction)PXDirectX9SceneDeploy;
        pxEngineRender->ViewPortGet = (PXGPUViewPortGetFunction)PXDirectX9ViewportSet;
        pxEngineRender->ViewPortSet = (PXGPUViewPortSetFunction)PXDirectX9ViewportGet;
        pxEngineRender->ShaderProgramCreate = (PXGPUShaderProgramCreateFunction)PXDirectX9ShaderProgramRegister;
        pxEngineRender->ShaderProgramSelect = (PXGPUShaderProgramSelectFunction)PXDirectX9ShaderProgramSelect;
        pxEngineRender->ShaderProgramDelete = (PXGPUShaderProgramDeleteFunction)PXDirectX9ShaderProgramDelete;
        pxEngineRender->SceneBegin = (PXGPUSceneBeginFunction)PXDirectX9SceneBegin;
        pxEngineRender->SceneEnd = (PXGPUSceneEndFunction)PXDirectX9SceneEnd;
        pxEngineRender->DrawScriptCreate = (PXGPUDrawScriptCreate)PXDirectX9DrawScriptCreate;
        pxEngineRender->DrawScriptBegin = (PXGPUDrawScriptBegin)PXDirectX9DrawScriptBegin;
        pxEngineRender->DrawScriptEnd = (PXGPUDrawScriptEnd)PXDirectX9DrawScriptEnd;
        pxEngineRender->DrawScriptDelete = (PXGPUDrawScriptDelete)PXDirectX9DrawScriptDelete;
        pxEngineRender->DrawScriptExecute = (PXGPUDrawScriptExecute)PXDirectX9DrawScriptExecute;
        pxEngineRender->ModelRegister = (PXGPUModelRegisterFunction)PXDirectX9ModelRegister;
        pxEngineRender->ModelDraw = (PXGPUModelDrawFunction)PXDirectX9ModelDraw;
        pxEngineRender->ModelSelect = PXNull;
        pxEngineRender->ModelRelease = PXNull;
        pxEngineRender->LightSet = (PXGPULightSetFunction)PXDirectX9LightSet;
        pxEngineRender->LightGet = (PXGPULightGetFunction)PXDirectX9LightGet;
        pxEngineRender->LightEnableSet = (PXGPULightEnableSetFunction)PXDirectX9LightEnableSet;
        pxEngineRender->LightEnableGet = (PXGPULightEnableGetFunction)PXDirectX9LightEnableGet;
    }

    //PXDirectX9DevicePhysicalListFetchFunction(pxDirectX9, pxGraphicInitializeInfo->Graphic->DevicePhysicalListSize, pxGraphicInitializeInfo->Graphic->DevicePhysicalList);

    D3DPRESENT_PARAMETERS presentParameters;
    PXClear(D3DPRESENT_PARAMETERS, &presentParameters);
    presentParameters.Windowed = TRUE;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.SwapEffect = D3DSWAPEFFECT_COPY; // D3DSWAPEFFECT_DISCARD


    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxEngineRenderCreateInfo->WindowReference);

    UINT adapter = D3DADAPTER_DEFAULT;
    D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
    HWND focusWindow = pxWindowHandle.Data;
    DWORD behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    D3DPRESENT_PARAMETERS* pPresentationParameters = &presentParameters;
    IDirect3DDevice9** ppReturnedDeviceInterface = &pxDirectX9->Device;


#if PXLanguageCPP
    resultID = pxDirectX9->Context->GetDeviceCaps
    (
        adapter,
        deviceType,
        &pxDirectX9->DeviceCapabilitiesCurrent
    );
#else
    resultID = pxDirectX9->Context->lpVtbl->GetDeviceCaps
    (
        pxDirectX9->Context,
        adapter,
        deviceType,
        &pxDirectX9->DeviceCapabilitiesCurrent
    );
#endif


#if PXLanguageCPP
    resultID = pxDirectX9->Context->CreateDevice
    (
        adapter,
        deviceType,
        focusWindow,
        behaviorFlags,
        pPresentationParameters,
        ppReturnedDeviceInterface
    );
#else
    resultID = pxDirectX9->Context->lpVtbl->CreateDevice
    (
        pxDirectX9->Context,
        adapter,
        deviceType,
        focusWindow,
        behaviorFlags,
        pPresentationParameters,
        ppReturnedDeviceInterface
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXDirectX9Text,
            "Initialize",
            "Device creation failed 0x%p",
            pxDirectX9->Device
        );
#endif

        return PXResultFailedInitialization;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "Initialize",
        "Device created 0x%p",
        pxDirectX9->Device
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9Release(PXDirectX9 PXREF pxDirectX9)
{
    if(!pxDirectX9)
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingDeallocation,
        PXDirectX9Text,
        "Release",
        "Destruct session"
    );
#endif

    if(pxDirectX9->Device)
    {
#if PXLanguageCPP
        pxDirectX9->Device->Release();
#else
        pxDirectX9->Device->lpVtbl->Release(pxDirectX9->Device);
#endif
    }

    if(pxDirectX9->Context)
    {
#if PXLanguageCPP
        pxDirectX9->Context->Release();
#else
        pxDirectX9->Context->lpVtbl->Release(pxDirectX9->Context);
#endif     
    }

    PXLibraryClose(&pxDirectX9->LibraryDirect3D);

    PXClear(PXDirectX9, pxDirectX9);

    return PXResultOK;
}

void PXAPI PXDirectX9Select(PXDirectX9 PXREF pxDirectX9)
{

}

void PXAPI PXDirectX9Deselect(PXDirectX9 PXREF pxDirectX9)
{

}

PXResult PXAPI PXDirectX9ShaderVertexRegister(PXDirectX9 PXREF pxDirectX9, PXShaderVertex PXREF pxShaderVertex)
{
#if 0
    if(pxShaderVertex->VersionMajor == 0 && pxShaderVertex->VersionMinor == 0)
    {
        pxShaderVertex->VersionMajor = D3DSHADER_VERSION_MAJOR(pxDirectX9->DeviceCapabilitiesCurrent.VertexShaderVersion);
        pxShaderVertex->VersionMinor = D3DSHADER_VERSION_MINOR(pxDirectX9->DeviceCapabilitiesCurrent.VertexShaderVersion);
    }

    PXTextPrintA(shaderVersion, 8, "vs_%i_%i", pxShader->VersionMajor, pxShader->VersionMinor);
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9ShaderPixelFragmentRegister(PXDirectX9 PXREF pxDirectX9, PXShaderPixelFragment PXREF pxShaderPixelFragment)
{
#if 0
    if(pxShader->VersionMajor == 0 && pxShader->VersionMinor == 0)
    {
        pxShader->VersionMajor = D3DSHADER_VERSION_MAJOR(pxDirectX9->DeviceCapabilitiesCurrent.PixelShaderVersion);
        pxShader->VersionMinor = D3DSHADER_VERSION_MINOR(pxDirectX9->DeviceCapabilitiesCurrent.PixelShaderVersion);
    }

    PXTextPrintA(shaderVersion, 8, "ps_%i_%i", pxShader->VersionMajor, pxShader->VersionMinor);
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9ShaderProgramRegister(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram)
{
#if 0

    for(PXSize i = 0; i < amount; i++)
    {
        PXShader PXREF pxShader = &shaderList[i];
        PXFile PXREF shaderFile = pxShader->ShaderFile;


        PXText pxTextFileName;
        PXTextConstructBufferA(&pxTextFileName, PXPathSizeMax);

        PXFilePath(shaderFile, &pxTextFileName, PXFalse);

        ID3DBlob* shaderByteCode = PXNull;
        ID3DBlob* shaderCompileErrorMessage = PXNull;

        char shaderVersion[8];

        switch(pxShader->Type)
        {
            case PXShaderTypeVertex:
            {
               
                break;
            }
            case PXShaderTypePixel:
            {
               
                break;
            }
            default:
                return PXResultRefusedParameterInvalid;
        }

        PXD3DCompile pxD3DCompile = (PXD3DCompile)pxDirectX9->ShaderCompile;

        PXBuffer* pxBuffer = PXFileBufferGET(shaderFile);

        const HRESULT resultID = pxD3DCompile // d3dcompiler_47.dll, d3dcompiler.h
        (
            pxBuffer->Data,
            pxBuffer->SizeAllowedToUse,
            &pxTextFileName.A, // Name?
            PXNull, // Makro count?
            PXNull, // Included?
            "main", // entry point
            shaderVersion, // target version
            D3DCOMPILE_ENABLE_STRICTNESS, // flag 1
            0, // flag 2
            &shaderByteCode,
            &shaderCompileErrorMessage
        );
        const PXResult vertexShaderCompileResult = PXErrorFromHRESULT(resultID);

        if(shaderCompileErrorMessage) // if we have a message
        {
            char* data = (char*)shaderCompileErrorMessage->lpVtbl->GetBufferPointer(shaderCompileErrorMessage);
            PXSize dataSize = shaderCompileErrorMessage->lpVtbl->GetBufferSize(shaderCompileErrorMessage);

            data[dataSize - 2] = '\0';

            // TODO: Error string contains full path of file EXE. remove it. A new allocate might be needed.

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXDirectX9Text,
                "Shader-Compile",
                "HLSL %s\n"
                "%s",
                shaderVersion,
                data
            );
#endif

            shaderCompileErrorMessage->lpVtbl->Release(shaderCompileErrorMessage);

            return PXResultFailedResourceCompile;
        }


        switch(pxShader->Type)
        {
            case PXShaderTypeVertex:
            {
                const HRESULT result = pxDirectX9->Device->lpVtbl->CreateVertexShader
                (
                    pxDirectX9->Device,
                    shaderByteCode,
                    &pxShader->VertexShader_9
                );
                const PXResult vertexShaderCreateResult = PXErrorFromHRESULT(result);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXDirectX9Text,
                    "Shader-Create",
                    "Vertex 0x%p",
                    pxShader->DirectXHandle
                );
#endif

                break;
            }
            case PXShaderTypePixel:
            {
                const HRESULT result = pxDirectX9->Device->lpVtbl->CreatePixelShader
                (
                    pxDirectX9->Device,
                    shaderByteCode,
                    &pxShader->PixelShader_9
                );
                const PXResult pixelShaderCreateResult = PXErrorFromHRESULT(result);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXDirectX9Text,
                    "Shader-Create",
                    "Pixel 0x%p",
                    pxShader->DirectXHandle
                );
#endif

                break;
            }
            default:
                return PXResultRefusedFormatNotSupported;
        }


        // Extract variable IDs
        {
            typedef struct CTHeader_
            {
                PXI32U Size;
                PXI32U Creator;
                PXI32U Version;
                PXI32U Constants;
                PXI32U ConstantInfo;
                PXI32U Flags;
                PXI32U Target;
            }
            CTHeader;

            typedef struct CTInfo_
            {
                PXI32U Name;
                PXI16U RegisterSet;
                PXI16U RegisterIndex;
                PXI16U RegisterCount;
                PXI16U Reserved;
                PXI32U TypeInfo;
                PXI32U DefaultValue;
            }
            CTInfo;

            typedef struct CTType_
            {
                PXI16U Class;
                PXI16U Type;
                PXI16U Rows;
                PXI16U Columns;
                PXI16U Elements;
                PXI16U StructMembers;
                PXI32U StructMemberInfo;
            }
            CTType;

            // Shader instruction opcodes
            const PXI32U SIO_COMMENT = 0x0000FFFE;
            const PXI32U SIO_END = 0x0000FFFF;
            const PXI32U SI_OPCODE_MASK = 0x0000FFFF;
            const PXI32U SI_COMMENTSIZE_MASK = 0x7FFF0000;
            const PXI32U CTAB_CONSTANT = 0x42415443;

            const PXI32U* shaderByteCodeCursor = (PXI32U*)shaderByteCode;
            PXBool success = PXFalse;

            while((*++shaderByteCodeCursor != SIO_END))
            {
                if((*shaderByteCodeCursor & SI_OPCODE_MASK) == SIO_COMMENT)
                {
                    // Check for CTAB comment
                    PXI32U comment_size = (*shaderByteCodeCursor & SI_COMMENTSIZE_MASK) >> 16;

                    if(*(shaderByteCodeCursor + 1) != CTAB_CONSTANT)
                    {
                        shaderByteCodeCursor += comment_size;
                        continue;
                    }

                    // Read header
                    const char* ctab = shaderByteCodeCursor + 2;
                    size_t ctab_size = (comment_size - 1) * 4;

                    const CTHeader* header = (CTHeader*)ctab;
                    const char* m_creator = ctab + header->Creator;
                    const PXBool isInvalidSize = ctab_size < sizeof(*header) || header->Size != sizeof(*header);

                    //if(isInvalidSize)
                    //    return false;


                    // Read constants
                    pxShader->VariableListAmount = header->Constants;
                    pxShader->VariableListData = PXMemoryHeapCallocT(PXShaderVariable, header->Constants);

                    const CTInfo* info = (CTInfo*)(ctab + header->ConstantInfo);

                    for(PXI32U i = 0; i < header->Constants; ++i)
                    {
                        PXShaderVariable PXREF pxShaderVariable = &pxShader->VariableListData[i];

                        const CTType* type = (CTType*)(ctab + info[i].TypeInfo);

                        const char* name = ctab + info[i].Name;

                        PXTextCopyA(name, PXTextLengthUnkown, pxShaderVariable->Name, 32);

                        pxShaderVariable->RegisterIndex = info[i].RegisterIndex;
                        pxShaderVariable->RegisterCount = info[i].RegisterCount;
                        pxShaderVariable->Rows = type->Rows;
                        pxShaderVariable->Columns = type->Columns;
                        pxShaderVariable->Elements = type->Elements;
                        pxShaderVariable->StructMembers = type->StructMembers;
                        pxShaderVariable->DataTypeSize = 4 * type->Elements * type->Rows * type->Columns;

                        switch(info[i].RegisterSet)
                        {
                            case 0: // BOOL
                                pxShaderVariable->DataType = PXShaderVariableTypeBoolSignle;
                                break;

                            case 1: // RS_INT4
                                pxShaderVariable->DataType = PXShaderVariableTypeInt32SVector4;
                                break;

                            case 2: // RS_PXF324
                                pxShaderVariable->DataType = PXShaderVariableTypePXF32Vector4;
                                break;

                            case 3: // RS_SAMPLER
                                pxShaderVariable->DataType = PXShaderVariableTypeSampler2DF;
                                break;

                            default:
                                pxShaderVariable->DataType = PXShaderVariableTypeInvalid;
                                break;
                        }
                    }

                    success = PXTrue;

                    break;
                }
            }
        }
    }

#endif


    return PXResultOK;


#if 0

    ID3DBlob* ps_blob_ptr = NULL;
    ID3DBlob* error_blob = NULL;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    // COMPILE VERTEX SHADER
    const HRESULT result = D3DCompileFromFile // D3DCompiler_47.dll, d3dcompiler.h
    (
        shaderFilePath->W,
        PXNull,
        D3D_COMPILE_STANDARD_FILE_Included,
        "vs_main",
        "vs_5_0",
        flags,
        0,
        &(ID3DBlob*)pxShader->Content,
        &error_blob
    );
    const PXBool failed = FAILED(result);



    return PXResultCompilingError;
#endif
}

PXResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram)
{
#if 0
    switch(pxShader->Type)
    {
        case PXShaderTypeVertex:
        {
            const HRESULT result = pxDirectX9->Device->lpVtbl->SetVertexShader
            (
                pxDirectX9->Device,
                (IDirect3DVertexShader9*)pxShader->ResourceID.DirectXInterface
            );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDirectX9Text,
                "Shader:Vertex select 0x%p",
                pxShader->ResourceID.DirectXInterface
            );
#endif

            break;
        }
        case PXShaderTypeFragment:
        {
            const HRESULT result = pxDirectX9->Device->lpVtbl->SetPixelShader
            (
                pxDirectX9->Device,
                (IDirect3DPixelShader9*)pxShader->ResourceID.DirectXInterface
            );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXDirectX9Text,
                "Shader:Pixel select 0x%p",
                pxShader->ResourceID.DirectXInterface
            );
#endif

            break;
        }
        default:
            return PXResultRefusedFormatNotSupported;
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9 PXREF pxDirectX9, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariable)
{
    if(!(pxDirectX9 && pxShaderProgram && pxShaderVariable))
    {
        return PXResultRefusedParameterNull;
    }

    HRESULT setShaderConstantID = S_OK;

    // Plan A, use pre-cached data
    switch(0) // pxShaderVariable->ShaderType
    {
        case PXShaderTypeVertex:
        {
            switch(pxShaderVariable->DataType)
            {
                case PXShaderVariableTypeBoolSignle:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetVertexShaderConstantB
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Bool,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantB
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Bool,
                        pxShaderVariable->Amount
                    );
#endif
            
                    break;
                }
                case PXShaderVariableTypeF32Q:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetVertexShaderConstantF
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->F32,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantF
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->F32,
                        pxShaderVariable->Amount
                    );
#endif
                 
                    break;
                }
                case PXShaderVariableTypeInt32SVector4:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetVertexShaderConstantI
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->S32,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantI
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->S32,
                        pxShaderVariable->Amount
                    );
#endif
                
                    break;
                }

                default:
                    return PXResultRefusedParameterInvalid;
            }

            break;
        }
        case PXShaderTypePixel:
        {
            switch(pxShaderVariable->DataType)
            {
                case PXShaderVariableTypeBoolSignle:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetPixelShaderConstantB
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Bool,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantB
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Bool,
                        pxShaderVariable->Amount
                    );
#endif
                
                    break;
                }
                case PXShaderVariableTypeF32Q:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetPixelShaderConstantF
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->F32,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantF
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->F32,
                        pxShaderVariable->Amount
                    );
#endif
                  
                    break;
                }
                case PXShaderVariableTypeInt32SVector4:
                {
#if PXLanguageCPP
                    setShaderConstantID = pxDirectX9->Device->SetPixelShaderConstantI
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->S32,
                        pxShaderVariable->Amount
                    );
#else
                    setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantI
                    (
                        pxDirectX9->Device,
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->S32,
                        pxShaderVariable->Amount
                    );
#endif
                
                    break;
                }

                default:
                    return PXResultRefusedParameterInvalid;
            }

            break;
        }

        default:
            return PXResultRefusedParameterInvalid;
    }

    // Handle result, exit on success
    {
        const PXResult shaderConstantResult = PXErrorFromHRESULT(setShaderConstantID);

        if(PXResultOK == shaderConstantResult)
        {
            return PXResultOK;
        }
    }


#if PXDX9IsSDKPresent
    // Plan B: Fetch the value over the variable table
    {
        PXD3DXGetShaderConstantTable pxD3DXGetShaderConstantTable = (PXD3DXGetShaderConstantTable)pxDirectX9->ShaderConstantTableGet;

        LPD3DXCONSTANTTABLE d3dxConstantTable;

        const HRESULT getTableResult = pxD3DXGetShaderConstantTable(NULL, &d3dxConstantTable);

        const D3DXHANDLE handle = d3dxConstantTable->lpVtbl->GetConstantByName(d3dxConstantTable, PXNull, pxShaderVariable->Name);

        HRESULT setResult = PXNull;

        switch(pxShaderVariable->ShaderType)
        {
            case PXShaderVariableTypeBoolSignle:
            {
                if(pxShaderVariable->Amount > 1)
                {
                    setResult = d3dxConstantTable->lpVtbl->SetBoolArray(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
                }
                else
                {
                    BOOL value = *(BOOL*)pxShaderVariable->Data;

                    setResult = d3dxConstantTable->lpVtbl->SetBool(d3dxConstantTable, pxDirectX9->Device, handle, value);
                }

                break;
            }
            case PXShaderVariableTypeInt32SSingle:
            {
                if(pxShaderVariable->Amount > 1)
                {
                    setResult = d3dxConstantTable->lpVtbl->SetIntArray(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
                }
                else
                {
                    int value = *(int*)pxShaderVariable->Data;

                    setResult = d3dxConstantTable->lpVtbl->SetInt(d3dxConstantTable, pxDirectX9->Device, handle, value);
                }

                break;
            }
            case PXShaderVariableTypePXF32Single:
            {
                if(pxShaderVariable->Amount > 1)
                {
                    setResult = d3dxConstantTable->lpVtbl->SetPXF32Array(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
                }
                else
                {
                    PXF32 value = *(PXF32*)pxShaderVariable->Data;

                    setResult = d3dxConstantTable->lpVtbl->SetPXF32(d3dxConstantTable, pxDirectX9->Device, handle, value);
                }

                break;
            }
            case PXShaderVariableTypeMatrix4x4:
            {
                if(pxShaderVariable->Amount > 1)
                {
                    setResult = d3dxConstantTable->lpVtbl->SetMatrixArray(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
                }
                else
                {
                    D3DXMATRIX* value = (D3DXMATRIX*)pxShaderVariable->Data;

                    setResult = d3dxConstantTable->lpVtbl->SetMatrix(d3dxConstantTable, pxDirectX9->Device, handle, value);
                }

                break;
            }
            case PXShaderVariableTypePXF32Vector4:
            {
                if(pxShaderVariable->Amount > 1)
                {
                    setResult = d3dxConstantTable->lpVtbl->SetVectorArray(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
                }
                else
                {
                    D3DXVECTOR4* value = (D3DXVECTOR4*)pxShaderVariable->Data;

                    setResult = d3dxConstantTable->lpVtbl->SetVector(d3dxConstantTable, pxDirectX9->Device, handle, value);
                }

                break;
            }

            default:
                return PXResultRefusedParameterInvalid;
        }

        // Hanle erturn code

        const PXResult shaderConstantResult = PXWindowsHandleErrorFromID(setShaderConstantID);

        return shaderConstantResult;
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9SceneBegin(PXDirectX9 PXREF pxDirectX9)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->BeginScene();
#else
    resultID = pxDirectX9->Device->lpVtbl->BeginScene(pxDirectX9->Device);
#endif
    pxResult = PXErrorFromHRESULT(resultID);

    
    return pxResult;
}

PXResult PXAPI PXDirectX9SceneEnd(PXDirectX9 PXREF pxDirectX9)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->EndScene();
#else
    resultID = pxDirectX9->Device->lpVtbl->EndScene(pxDirectX9->Device);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType)
{
    PXResult pxResult = PXResultOK;

    HRESULT resultID = S_OK;
    D3DSTATEBLOCKTYPE stateBlcokType = D3DSBT_FORCE_DWORD;

    switch(pxDrawScriptType)
    {
        case PXDrawScriptTypeAll:
            stateBlcokType = D3DSBT_ALL;
            break;
        case PXDrawScriptTypePixelState:
            stateBlcokType = D3DSBT_PIXELSTATE;
            break;
        case PXDrawScriptTypeVertexState:
            stateBlcokType = D3DSBT_VERTEXSTATE;
            break;

        case PXDrawScriptTypeInvalid:
        default:
            return PXResultRefusedFormatNotSupported;
    }

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateStateBlock
    (
        stateBlcokType,
        &pxDrawScript->StateBlock_9
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateStateBlock
    (
        pxDirectX9->Device,
        stateBlcokType,
        &pxDrawScript->StateBlock_9
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return PXResultOK;
}

PXResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->BeginStateBlock();
#else
    resultID = pxDirectX9->Device->lpVtbl->BeginStateBlock(pxDirectX9->Device);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->EndStateBlock(&pxDrawScript->StateBlock_9);
#else
    resultID = pxDirectX9->Device->lpVtbl->EndStateBlock
    (
        pxDirectX9->Device,
        &pxDrawScript->StateBlock_9
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
    IDirect3DStateBlock9 PXREF stateBlock_9 = pxDrawScript->StateBlock_9;

    ULONG result = 0;

#if PXLanguageCPP
    result = stateBlock_9->Release();
#else
    result = stateBlock_9->lpVtbl->Release(stateBlock_9);
#endif  

    return PXResultOK;
}

PXResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ViewportSet(PXDirectX9 PXREF pxDirectX9, const PXViewPort PXREF pxViewPort)
{
    PXResult result = PXResultOK;

    HRESULT resultID;
    D3DVIEWPORT9 viewPort;
    viewPort.X = pxViewPort->X;
    viewPort.Y = pxViewPort->Y;
    viewPort.Width = pxViewPort->Width;
    viewPort.Height = pxViewPort->Height;
    viewPort.MinZ = pxViewPort->ClippingMinimum;
    viewPort.MaxZ = pxViewPort->ClippingMaximum;    

#if PXLanguageCPP
    resultID = pxDirectX9->Device->SetViewport(&viewPort);
#else
    resultID = pxDirectX9->Device->lpVtbl->SetViewport
    (
        pxDirectX9->Device,
        &viewPort
    );
#endif

    result = PXErrorFromHRESULT(result);
 
    return result;
}

PXResult PXAPI PXDirectX9ViewportGet(PXDirectX9 PXREF pxDirectX9, PXViewPort PXREF pxViewPort)
{
    HRESULT result = 0;
    D3DVIEWPORT9 viewPort;

#if PXLanguageCPP
    result = pxDirectX9->Device->GetViewport(&viewPort);
#else
    result = pxDirectX9->Device->lpVtbl->GetViewport
    (
        pxDirectX9->Device,
        &viewPort
    );
#endif
    pxViewPort->X = viewPort.X;
    pxViewPort->Y = viewPort.Y;
    pxViewPort->Width = viewPort.Width;
    pxViewPort->Height = viewPort.Height;
    pxViewPort->ClippingMinimum = viewPort.MinZ;
    pxViewPort->ClippingMaximum = viewPort.MaxZ;

    return PXResultOK;
}

PXResult PXAPI PXDirectX9MaterialSet(PXDirectX9 PXREF pxDirectX9, const PXMaterial PXREF pxMaterial)
{
    PXResult pxResult = PXResultOK;

    HRESULT resultID;
    D3DMATERIAL9 d3dMaterial;
    PXDirectXMaterialFromPXMaterial(&d3dMaterial, pxMaterial);

#if PXLanguageCPP
    resultID = pxDirectX9->Device->SetMaterial(&d3dMaterial);
#else
    resultID = pxDirectX9->Device->lpVtbl->SetMaterial
    (
        pxDirectX9->Device,
        &d3dMaterial
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectX9MaterialGet(PXDirectX9 PXREF pxDirectX9, PXMaterial PXREF pxMaterial)
{
    PXResult pxResult = PXResultOK;

    D3DMATERIAL9 d3dMaterial;
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->GetMaterial(&d3dMaterial);
#else
    resultID = pxDirectX9->Device->lpVtbl->GetMaterial
    (
        pxDirectX9->Device,
        &d3dMaterial
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    PXDirectXMaterialToPXMaterial(pxMaterial, &d3dMaterial);

    return pxResult;
}

PXResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9 PXREF pxDirectX9, const PXI32U interval)
{
#if 0
    IDXGISwapChain x;

    // Get default swap chain
    IDirect3DSwapChain9* direct3DSwapChain = PXNull;

    const HRESULT resultA = pxDirectX->DX9->lpVtbl->GetSwapChain // DXGI.lib, dxgi.h
    (
        pxDirectX->DX9,
        0,
        &direct3DSwapChain
    );

    direct3DSwapChain->lpVtbl->Present
    (
        direct3DSwapChain,
        0,
        0,
        0,
        0,

        );


    if(bVysncEnabled)
    {
        presentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    }
    else
    {
        presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        presentParams.FullScreen_RefreshRateInHz = 0;
    }


    const HRESULT resultB = pxDirectX->DX9->lpVtbl->Present // DXGI.lib, dxgi.h
    (
        pxDirectX->DX9,
        interval,

        );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF interval)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF value)
{
    UINT textureMemory = 0;

#if PXLanguageCPP
    textureMemory = pxDirectX9->Device->GetAvailableTextureMem(); // [WARNING] Maxes out at 4GB!
#else
    textureMemory = pxDirectX9->Device->lpVtbl->GetAvailableTextureMem(pxDirectX9->Device);
#endif

    *value = textureMemory;

    return PXResultOK;
}

PXResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF amountOfAdapters)
{
    UINT amount = 0; 

#if PXLanguageCPP
    amount = pxDirectX9->Context->GetAdapterCount();
#else
    amount = pxDirectX9->Context->lpVtbl->GetAdapterCount(pxDirectX9->Context);
#endif

    *amountOfAdapters = amount;

    return PXResultOK;;
}

PXResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9 PXREF pxDirectX9, const PXI32U pxGraphicDevicePhysicalListSize, PXGPU PXREF pxGraphicDevicePhysicalList)
{
    PXI32U amountOfDXDevices = 0;

    PXDirectX9DevicePhysicalListAmountFunction(pxDirectX9, &amountOfDXDevices);

    for(PXI32U i = 0; i < amountOfDXDevices; i++)
    {
        D3DADAPTER_IDENTIFIER9 adapterIdentifier;

        PXClear(D3DADAPTER_IDENTIFIER9, &adapterIdentifier);

#if PXLanguageCPP
        pxDirectX9->Context->GetAdapterIdentifier(i, 0, &adapterIdentifier);
#else
        pxDirectX9->Context->lpVtbl->GetAdapterIdentifier(pxDirectX9->Context, i, 0, &adapterIdentifier);
#endif       

        for(PXI32U deviceIndex = 0; deviceIndex < pxGraphicDevicePhysicalListSize; deviceIndex++)
        {
            PXGPU* pxGraphicDevicePhysicalCurrent = &pxGraphicDevicePhysicalList[i];

            PXBool found = PXTextCompareA(adapterIdentifier.DeviceName, 32, pxGraphicDevicePhysicalCurrent->DeviceDisplayName, PXDeviceDisplaySize, 0);

            if(found)
            {
                PXTextCopyA(adapterIdentifier.Driver, MAX_DEVICE_IDENTIFIER_STRING, pxGraphicDevicePhysicalCurrent->DriverName, MAX_DEVICE_IDENTIFIER_STRING);
                break;
            }
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXDirectX9Clear(PXDirectX9 PXREF pxDirectX9, const PXColorRGBA_F32 PXREF PXColorRGBA_F32)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

    const UINT colorI8 = D3DCOLOR_ARGB
    (
        (PXI8U)(PXColorRGBA_F32->Alpha * 0xFF),
        (PXI8U)(PXColorRGBA_F32->Red * 0xFF),
        (PXI8U)(PXColorRGBA_F32->Green * 0xFF),
        (PXI8U)(PXColorRGBA_F32->Blue * 0xFF)
    );

    DWORD count = 0;
    D3DRECT* pRects = PXNull;
    DWORD flags = D3DCLEAR_TARGET;
    D3DCOLOR color = colorI8;
    float z = 1.0f;
    DWORD stencil = 0;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->Clear
    (
        count,
        pRects,
        flags,
        color,
        z,
        stencil
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->Clear
    (
        pxDirectX9->Device,
        count,
        pRects,
        flags,
        color,
        z,
        stencil
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9SceneDeploy(PXDirectX9 PXREF pxDirectX9)
{
    HRESULT resultID = S_OK;
    RECT* pSourceRect = PXNull;
    RECT* pDestRect = PXNull;
    HWND hDestWindowOverride = PXNull;
    RGNDATA* pDirtyRegion = PXNull;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->Present
    (
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->Present
    (
        pxDirectX9->Device,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9 PXREF pxDirectX9, const PXDrawMode PXGraphicDrawMode, const PXI32U startVertex, const PXI32U primitiveCount)
{
    const D3DPRIMITIVETYPE primitiveType = PXDirectXDrawTypeFromPX(PXGraphicDrawMode);
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->DrawPrimitive
    (
        primitiveType,
        startVertex,
        primitiveCount
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->DrawPrimitive
    (
        pxDirectX9->Device,
        primitiveType,
        startVertex,
        primitiveCount
    );
#endif
  
    return PXResultOK;
}

PXResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9 PXREF pxDirectX9, const void* pxVertexBufferFormat)
{
    const DWORD flagID = 0;// PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->SetFVF
    (
        flagID
    );
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->SetFVF
    (
        pxDirectX9->Device,
        flagID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9 PXREF pxDirectX9, void* PXREF pxVertexBufferFormat)
{
    DWORD flagID = 0;

#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->GetFVF
    (
        &flagID
    );
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->GetFVF
    (
        pxDirectX9->Device,
        &flagID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXI32U OffsetInBytes, const PXI32U Stride)
{
    HRESULT resultID = S_OK;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->SetStreamSource
    (
        StreamNumber,
        pxVertexBuffer->DX_9,
        OffsetInBytes,
        Stride
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->SetStreamSource
    (
        pxDirectX9->Device,
        StreamNumber,
        pxVertexBuffer->DX_9,
        OffsetInBytes,
        Stride
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXI32U* pOffsetInBytes, PXI32U* pStride)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ModelDraw(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel, const PXCamera PXREF pxCamera)
{
   // const PXSize stride = PXVertexBufferFormatStrideSize(pxModel->Mesh.VertexBuffer.Format);

  //  PXDirectX9StreamSourceSet(pxDirectX9, 0, &pxModel->Mesh.VertexBuffer, 0, stride);
   // PXDirectX9VertexFixedFunctionSet(pxDirectX9, pxModel->Mesh.VertexBuffer.Format);
    PXDirectX9PrimitiveDraw(pxDirectX9, PXDrawModeTriangle, 0, 1);

    return PXResultOK;
}

PXResult PXAPI PXDirectX9LightSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index)
{
    D3DLIGHT9 d3dLight9;
    d3dLight9.Diffuse.r = pxLight->Diffuse[0];
    d3dLight9.Diffuse.g = pxLight->Diffuse[1];
    d3dLight9.Diffuse.b = pxLight->Diffuse[2];
    d3dLight9.Diffuse.a = pxLight->Diffuse[3];
    d3dLight9.Specular.r = pxLight->Specular[0];
    d3dLight9.Specular.g = pxLight->Specular[1];
    d3dLight9.Specular.b = pxLight->Specular[2];
    d3dLight9.Specular.a = pxLight->Specular[3];
    d3dLight9.Ambient.r = pxLight->Ambient[0];
    d3dLight9.Ambient.g = pxLight->Ambient[1];
    d3dLight9.Ambient.b = pxLight->Ambient[2];
    d3dLight9.Ambient.a = pxLight->Ambient[3];
    d3dLight9.Position.x = pxLight->Position[0];
    d3dLight9.Position.y = pxLight->Position[1];
    d3dLight9.Position.z = pxLight->Position[2];
    d3dLight9.Direction.x = pxLight->Direction[0];
    d3dLight9.Direction.y = pxLight->Direction[1];
    d3dLight9.Direction.z = pxLight->Direction[2];
    d3dLight9.Range = pxLight->CutoffRange;
    d3dLight9.Falloff = pxLight->Falloff;
    d3dLight9.Attenuation0 = pxLight->AttenuationConstant;
    d3dLight9.Attenuation1 = pxLight->AttenuationLinear;
    d3dLight9.Attenuation2 = pxLight->AttenuationQuadratic;
    d3dLight9.Theta = pxLight->Theta;
    d3dLight9.Phi = pxLight->Phi;

    switch(pxLight->Type)
    {
        case PXLightTypePoint:
            d3dLight9.Type = D3DLIGHT_POINT;
            break;

        case PXLightTypeSpot:
            d3dLight9.Type = D3DLIGHT_SPOT;
            break;

        case PXLightTypeDirectional:
            d3dLight9.Type = D3DLIGHT_DIRECTIONAL;
            break;

        default:
            return PXResultRefusedParameterInvalid;
    }


#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->SetLight(index, &d3dLight9);
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->SetLight(pxDirectX9->Device, index, &d3dLight9);
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9LightGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index)
{
    BOOL enableBool = 0;

#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->GetLightEnable(index, &enableBool);
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLightEnable(pxDirectX9->Device, index, &enableBool);
#endif

    pxLight->Enabled = enableBool;
    // *enable = enableBool;

    return PXResultOK;
}

PXResult PXAPI PXDirectX9LightEnableSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, const PXBool enable)
{
#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->LightEnable(index, enable);
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->LightEnable(pxDirectX9->Device, index, enable);
#endif

    pxLight->Enabled = enable;

    return PXResultOK;
}

PXResult PXAPI PXDirectX9LightEnableGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable)
{
    D3DLIGHT9 d3dLight9;

#if PXLanguageCPP
    const HRESULT result = pxDirectX9->Device->GetLight(index, &d3dLight9);
#else
    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLight(pxDirectX9->Device, index, &d3dLight9);
#endif


    pxLight->Diffuse[0] = d3dLight9.Diffuse.r;
    pxLight->Diffuse[1] = d3dLight9.Diffuse.g;
    pxLight->Diffuse[2] = d3dLight9.Diffuse.b;
    pxLight->Diffuse[3] = d3dLight9.Diffuse.a;
    pxLight->Specular[0] = d3dLight9.Specular.r;
    pxLight->Specular[1] = d3dLight9.Specular.g;
    pxLight->Specular[2] = d3dLight9.Specular.b;
    pxLight->Specular[3] = d3dLight9.Specular.a;
    pxLight->Ambient[0] = d3dLight9.Ambient.r;
    pxLight->Ambient[1] = d3dLight9.Ambient.g;
    pxLight->Ambient[2] = d3dLight9.Ambient.b;
    pxLight->Ambient[3] = d3dLight9.Ambient.a;
    pxLight->Position[0] = d3dLight9.Position.x;
    pxLight->Position[1] = d3dLight9.Position.y;
    pxLight->Position[2] = d3dLight9.Position.z;
    pxLight->Direction[0] = d3dLight9.Direction.x;
    pxLight->Direction[1] = d3dLight9.Direction.y;
    pxLight->Direction[2] = d3dLight9.Direction.z;
    pxLight->CutoffRange = d3dLight9.Range;
    pxLight->Falloff = d3dLight9.Falloff;
    pxLight->AttenuationConstant = d3dLight9.Attenuation0;
    pxLight->AttenuationLinear = d3dLight9.Attenuation1;
    pxLight->AttenuationQuadratic = d3dLight9.Attenuation2;
    pxLight->Theta = d3dLight9.Theta;
    pxLight->Phi = d3dLight9.Phi;

    switch(d3dLight9.Type)
    {
        case D3DLIGHT_POINT:
            pxLight->Type = PXLightTypePoint;
            break;

        case D3DLIGHT_SPOT:
            pxLight->Type = PXLightTypeSpot;
            break;

        case D3DLIGHT_DIRECTIONAL:
            pxLight->Type = PXLightTypeDirectional;
            break;

        default:
            pxLight->Type = PXLightTypeInvalid;
            break;
    }

    return PXResultOK;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderCreate(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderSelect(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderCompile(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader, const PXText PXREF shaderFilePath)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXVertexBuffer PXREF pxVertexBuffer)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxDirectX9 && pxVertexBuffer))
    {
        return PXResultRefusedParameterNull;
    }
       
   
    HRESULT resultID = 0;

    UINT vertexDataLength = pxVertexBuffer->Data.SizeAllowedToUse;
    DWORD Usage = 0;
    DWORD FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;// PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBuffer->Format);
    D3DPOOL Pool = D3DPOOL_DEFAULT;
    IDirect3DVertexBuffer9** ppVertexBuffer = &pxVertexBuffer->DX_9;
    HANDLE* pSharedHandle = PXNull;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateVertexBuffer
    (
        vertexDataLength,
        Usage,
        FVF,
        Pool,
        ppVertexBuffer,
        pSharedHandle
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateVertexBuffer
    (
        pxDirectX9->Device,
        vertexDataLength,
        Usage,
        FVF,
        Pool,
        ppVertexBuffer,
        pSharedHandle
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "VertexBuffer",
        "PXID:<%i> Vertex buffer created <%p>",
        pxVertexBuffer->Info.ID,
        pxVertexBuffer->DX_9
    );
#endif

    PXDirectX9VertexBufferUpload(pxDirectX9, pxVertexBuffer);

    return PXResultOK;
}

PXResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXIndexBuffer PXREF pxIndexBuffer)
{
    PXResult pxResult = PXResultInvalid;

    HRESULT resultID = 0;

    UINT length = 0;
    DWORD usage = 0;
    D3DFORMAT dataFormat = D3DFMT_UNKNOWN;
    D3DPOOL pool = D3DPOOL_DEFAULT;
    IDirect3DIndexBuffer9** indexBuffer = &pxIndexBuffer->DX_9;
    HANDLE* sharedHandle = PXNull;

    // fetch format
    {
        switch(pxIndexBuffer->BitPerIndex)
        {
            case 16:
            {
                dataFormat = D3DFMT_INDEX16;
                break;
            }
            case 32:
            {
                dataFormat = D3DFMT_INDEX32;
                break;
            }
            default:
            {
                dataFormat = D3DFMT_UNKNOWN;
                break;
            }
        }
    }

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateIndexBuffer
    (
        length,
        usage,
        dataFormat,
        pool,
        indexBuffer,
        sharedHandle
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateIndexBuffer
    (
        pxDirectX9,
        length,
        usage,
        dataFormat,
        pool,
        indexBuffer,
        sharedHandle
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    pxResult = PXDirectX9IndexBufferUpload(pxDirectX9, pxIndexBuffer);

    // Fill data

    return pxResult;
}

PXResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9 PXREF pxDirectX9, PXRenderTarget PXREF pxRenderTarget)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9 PXREF pxDirectX9, PXDepthStencilSurface PXREF pxDepthStencilSurface)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ModelRegister(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel)
{
   // PXDirectX9VertexBufferCreate(pxDirectX9, &pxModel->Mesh.VertexBuffer);
    //PXDirectX9IndexBufferCreate(pxDirectX9, &pxModel->Mesh.IndexBuffer);

    return PXResultOK;
}

PXResult PXAPI PXDirectX9VertexBufferUpload(PXDirectX9 PXREF pxDirectX9, PXVertexBuffer PXREF pxVertexBuffer)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxDirectX9 && pxVertexBuffer))
    {
        return PXResultRefusedParameterNull;
    }

    HRESULT resultID = 0;

    IDirect3DVertexBuffer9 PXREF vertexBuffer = pxVertexBuffer->DX_9;
    UINT OffsetToLock = 0;
    UINT vertexDataLength = pxVertexBuffer->Data.SizeAllowedToUse;
    void* targetAddress = PXNull;
    DWORD flags = 0;

#if PXLanguageCPP
    resultID = vertexBuffer->Lock
    (
        OffsetToLock,
        vertexDataLength,
        &targetAddress,
        flags
    );
#else
    resultID = vertexBuffer->lpVtbl->Lock
    (
        vertexBuffer,
        OffsetToLock,
        vertexDataLength,
        &targetAddress,
        flags
    );
#endif

    PXMemoryCopy(pxVertexBuffer->Data.Address, targetAddress, vertexDataLength);

#if PXLanguageCPP
    resultID = vertexBuffer->Unlock();
#else
    resultID = vertexBuffer->lpVtbl->Unlock(vertexBuffer);
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9IndexBufferUpload(PXDirectX9 PXREF pxDirectX9, PXIndexBuffer PXREF pxIndexBuffer)
{
    HRESULT resultID = 0;
    PXResult pxResult = PXResultInvalid;

    if(!(pxDirectX9 && pxIndexBuffer))
    {
        return PXResultRefusedParameterNull;
    }

    IDirect3DIndexBuffer9 PXREF indexBuffer9 = pxIndexBuffer->DX_9;
    UINT offsetToLock = 0;
    UINT sizeToLock = pxIndexBuffer->Data.SizeAllowedToUse;
    void* insertAddress = PXNull;
    DWORD flags = 0;


#if PXLanguageCPP
    resultID = indexBuffer9->Lock(offsetToLock, sizeToLock, &insertAddress, flags);
#else
    resultID = indexBuffer9->lpVtbl->Lock(indexBuffer9, offsetToLock, sizeToLock, &insertAddress, flags);
#endif
    pxResult = PXErrorFromHRESULT(pxResult);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    PXMemoryCopy(pxIndexBuffer->Data.Address, insertAddress, sizeToLock);

#if PXLanguageCPP
    resultID = indexBuffer9->Unlock();
#else
    resultID = indexBuffer9->lpVtbl->Unlock(indexBuffer9);
#endif
    pxResult = PXErrorFromHRESULT(pxResult);

    return pxResult;
}

PXResult PXAPI PXDirectX9Texture1DRegister(PXDirectX9 PXREF pxDirectX9, PXTexture1D PXREF pxTexture1D)
{
    return PXResultOK;
}

PXResult PXAPI PXDirectX9Texture2DRegister(PXDirectX9 PXREF pxDirectX9, PXTexture2D PXREF pxTexture2D)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = 0;

    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
    const D3DPOOL pool = D3DPOOL_DEFAULT;
    HANDLE sharedHandle = PXNull;

    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture2D->Format);

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateTexture
    (
        pxTexture2D->Width,
        pxTexture2D->Height,
        levels,
        usage,
        format,
        pool,
        &pxTexture2D->DX_9,
        &sharedHandle
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateTexture
    (
        pxDirectX9->Device,
        pxTexture2D->Width,
        pxTexture2D->Height,
        levels,
        usage,
        format,
        pool,
        &pxTexture2D->DX_9,
        &sharedHandle
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "Texture2D",
        "Created (%ix%i) 0x%p",
        pxTexture2D->Width,
        pxTexture2D->Height,
        pxTexture2D->DX_9
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectX9Texture3DRegister(PXDirectX9 PXREF pxDirectX9, PXTexture3D PXREF pxTexture3D)
{
    if(!(pxDirectX9 && pxTexture3D))
    {
        return PXResultRefusedParameterNull;
    }

    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
    const D3DPOOL pool = D3DPOOL_DEFAULT;
    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture3D->Format);
    HANDLE sharedHandle = PXNull;
    HRESULT resultID = S_OK;
    PXResult pxResult = PXResultInvalid;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateVolumeTexture
    (
        pxTexture3D->Width,
        pxTexture3D->Height,
        pxTexture3D->Depth,
        levels,
        usage,
        format,
        pool,
        &pxTexture3D->DX_9,
        &sharedHandle
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateVolumeTexture
    (
        pxDirectX9->Device,
        pxTexture3D->Width,
        pxTexture3D->Height,
        pxTexture3D->Depth,
        levels,
        usage,
        format,
        pool,
        &pxTexture3D->DX_9,
        &sharedHandle
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "Texture3D",
        "created (%ix%ix%i) 0x%p",
        pxTexture3D->Width,
        pxTexture3D->Height,
        pxTexture3D->Depth,
        pxTexture3D->DX_9
    );
#endif

    return pxResult;
}

PXResult PXAPI PXDirectX9TextureCubeRegister(PXDirectX9 PXREF pxDirectX9, PXTextureCube PXREF pxTextureCube)
{
    PXResult pxResult = PXResultOK;
    HRESULT resultID = S_OK;

    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
    const D3DPOOL pool = D3DPOOL_DEFAULT;
    HANDLE sharedHandle = PXNull;

    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTextureCube->Format);
    const UINT edgeLength = 0;

#if PXLanguageCPP
    resultID = pxDirectX9->Device->CreateCubeTexture
    (
        edgeLength,
        levels,
        usage,
        format,
        pool,
        &pxTextureCube->DX9,
        &sharedHandle
    );
#else
    resultID = pxDirectX9->Device->lpVtbl->CreateCubeTexture
    (
        pxDirectX9->Device,
        edgeLength,
        levels,
        usage,
        format,
        pool,
        &pxTextureCube->DX9,
        &sharedHandle
    );
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX9Text,
        "TextureCube",
        "created 0x%p",
        pxTextureCube->DX9
    );
#endif

    return PXResultOK;
}

D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat)
{
    switch(pxColorFormat)
    {
        case PXColorFormatRGBI8:
            return D3DFMT_R8G8B8;
        case PXColorFormatARGBI8:
            return D3DFMT_A8R8G8B8;

        default:
            return D3DFMT_UNKNOWN;
    }
}

void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial PXREF pxMaterial, const D3DMATERIAL9 PXREF d3dMaterial)
{
    /*
    pxMaterial->DiffuseColor.Red = d3dMaterial->Diffuse.r;
    pxMaterial->Diffuse.Green = d3dMaterial->Diffuse.g;
    pxMaterial->Diffuse.Blue = d3dMaterial->Diffuse.b;
    pxMaterial->Diffuse.Alpha = d3dMaterial->Diffuse.a;

    pxMaterial->Ambient.Red = d3dMaterial->Ambient.r;
    pxMaterial->Ambient.Green = d3dMaterial->Ambient.g;
    pxMaterial->Ambient.Blue = d3dMaterial->Ambient.b;
    pxMaterial->Ambient.Alpha = d3dMaterial->Ambient.a;

    pxMaterial->Specular.Red = d3dMaterial->Specular.r;
    pxMaterial->Specular.Green = d3dMaterial->Specular.g;
    pxMaterial->Specular.Blue = d3dMaterial->Specular.b;
    pxMaterial->Specular.Alpha = d3dMaterial->Specular.a;

    pxMaterial->Emission.Red = d3dMaterial->Emissive.r;
    pxMaterial->Emission.Green = d3dMaterial->Emissive.g;
    pxMaterial->Emission.Blue = d3dMaterial->Emissive.b;
    pxMaterial->Emission.Alpha = d3dMaterial->Emissive.a;

    pxMaterial->Power = d3dMaterial->Power;
    */
}

/*
   switch(pxVertexBufferFormat)
    {
#if 0
        case PXVertexBufferFormatP3F32:
            return D3DFVF_XYZ;
        case PXVertexBufferFormatXYZC:
            return D3DFVF_XYZ | D3DFVF_DIFFUSE;
        case PXVertexBufferFormatXYZHWC:
            return D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
#endif

        default:
            return 0;
    }
*/

void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9 PXREF d3dMaterial, const PXMaterial PXREF pxMaterial)
{
    /*
    d3dMaterial->Diffuse.r = pxMaterial->Diffuse.Red;
    d3dMaterial->Diffuse.g = pxMaterial->Diffuse.Green;
    d3dMaterial->Diffuse.b = pxMaterial->Diffuse.Blue;
    d3dMaterial->Diffuse.a = pxMaterial->Diffuse.Alpha;

    d3dMaterial->Ambient.r = pxMaterial->Ambient.Red;
    d3dMaterial->Ambient.g = pxMaterial->Ambient.Green;
    d3dMaterial->Ambient.b = pxMaterial->Ambient.Blue;
    d3dMaterial->Ambient.a = pxMaterial->Ambient.Alpha;

    d3dMaterial->Specular.r = pxMaterial->Specular.Red;
    d3dMaterial->Specular.g = pxMaterial->Specular.Green;
    d3dMaterial->Specular.b = pxMaterial->Specular.Blue;
    d3dMaterial->Specular.a = pxMaterial->Specular.Alpha;

    d3dMaterial->Emissive.r = pxMaterial->Emission.Red;
    d3dMaterial->Emissive.g = pxMaterial->Emission.Green;
    d3dMaterial->Emissive.b = pxMaterial->Emission.Blue;
    d3dMaterial->Emissive.a = pxMaterial->Emission.Alpha;

    d3dMaterial->Power = pxMaterial->Power;
    */
}

PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format)
{
    switch(format)
    {
        case D3DFMT_R8G8B8:
            return PXColorFormatRGBI8;
        case D3DFMT_A8R8G8B8:
            return PXColorFormatARGBI8;
            // case D3DFMT_X8R8G8B8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R5G6B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X1R5G5B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A1R5G5B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A4R4G4B4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R3G3B2: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A8:
            return PXColorFormatA8;
            // case D3DFMT_A8R3G3B2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X4R4G4B4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2B10G10R10: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A8B8G8R8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X8B8G8R8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G16R16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2R10G10B10: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A16B16G16R16: return xxxxxxxxxxxxxxxxxx;
            // case  D3DFMT_A8P8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_P8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_L8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A8L8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A4L4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_L6V5U5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X8L8V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_Q8W8V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_V16U16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2W10V10U10: return xxxxxxxxxxxxxxxxxx;
            //         case D3DFMT_UYVY: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R8G8_B8G8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_YUY2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G8R8_G8B8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT1: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT3: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D16_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D32: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D15S1: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24S8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24X8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24X4S4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D32F_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24FS8: return xxxxxxxxxxxxxxxxxx;
#if !defined(D3D_DISABLE_9EX)
        // case D3DFMT_D32_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
        // case D3DFMT_S8_LOCKABLE: return xxxxxxxxxxxxxxxxxx;

#endif
    // case D3DFMT_L16: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_VERTEXDATA: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_INDEX16: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_INDEX32: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_Q16W16V16U16: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_MULTI2_ARGB8: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_R16F: return xxxxxxxxxxxxxxxxxx;
    // case D3DFMT_G16R16F: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A16B16G16R16F:
            return PXColorFormatARGBF16;
            // case D3DFMT_R32F: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G32R32F: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A32B32G32R32F:
            return PXColorFormatARGBF32;

            // case D3DFMT_CxV8U8: return xxxxxxxxxxxxxxxxxx;
#if !defined(D3D_DISABLE_9EX)
        // case D3DFMT_A1: return xxxxxxxxxxxxxxxxxx;
        // case D3DFMT_A2B10G10R10_XR_BIAS: return xxxxxxxxxxxxxxxxxx;
        // case D3DFMT_BINARYBUFFER: return xxxxxxxxxxxxxxxxxx;
#endif
    // case D3DFMT_FORCE_DWORD: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_UNKNOWN:
        default:
            return PXColorFormatInvalid;
    }
}

D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXDrawMode PXGraphicDrawMode)
{
    switch(PXGraphicDrawMode)
    {
        case PXDrawModePoint:
            return D3DPT_POINTLIST;
        case PXDrawModeLineLoop:
            return D3DPT_LINELIST;
        case PXDrawModeLineStrip:
            return D3DPT_LINESTRIP;
        case PXDrawModeTriangle:
            return D3DPT_TRIANGLELIST;
        case PXDrawModeTriangleStrip:
            return D3DPT_TRIANGLESTRIP;
        case PXDrawModeTriangleFAN:
            return D3DPT_TRIANGLEFAN;
        default:
            return (D3DPRIMITIVETYPE)0; // Invalid mode does not exist!
    }
}
#endif