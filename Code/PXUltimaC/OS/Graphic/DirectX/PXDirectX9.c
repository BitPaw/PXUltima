#include "PXDirectX9.h"

#include <OS/GUI/PXGUI.h>
#include <OS/Console/PXConsole.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Memory/PXMemory.h>

#if PXDX9Enable
#include <d3d9.h>
#include <d3d9types.h>

#define PXDX9IsSDKPresent 0 // change this flag if you have the DirectX SDK installed, if not, code might not be compiled!

#endif

#if PXDX9Enable
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
// #include <d3d9.h> // Already included

#include <d3dcompiler.h>
#include <d3d9caps.h>
#include <d3dcompiler.h>


#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "D3d9.lib")

#if PXDX9IsSDKPresent
#include <D3DX9Shader.h>
#pragma comment(lib, "D3dx9.lib") // Only if SDK is present
#endif

#endif


#if OSWindows
typedef IDirect3D9* (WINAPI* PXDirect3DCreate9)(UINT SDKVersion);

PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format);
D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat);
void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial);
void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial);
D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode);
PXInt32U PXAPI PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat);

typedef HRESULT(WINAPI* PXD3DCompile)
(
    __in_bcount(SrcDataSize) LPCVOID pSrcData,
    __in SIZE_T SrcDataSize,
    __in_opt LPCSTR pSourceName,
    __in_xcount_opt(pDefines->Name != NULL) CONST D3D_SHADER_MACRO* pDefines,
    __in_opt ID3DInclude* pInclude,
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

#endif

PXActionResult PXAPI PXDirectX9Initialize(PXDirectX9* const pxDirectX9, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
#if OSUnix
#elif OSWindows

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX9",
        "Initialize",
        "Start..."
    );
#endif

    // Open directX9 lib
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDirectX9->LibraryDirect3D, "D3D9.DLL");

        if (PXActionSuccessful != pxActionResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    // Get init function and execute
    {
        PXDirect3DCreate9 pxDirect3DCreate;

        const PXActionResult pxActionResult = PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3D, &pxDirect3DCreate, "Direct3DCreate9");

        if (PXActionSuccessful != pxActionResult)
        {
            PXLibraryClose(&pxDirectX9->LibraryDirect3D);
            return PXActionRefusedNotSupportedByOperatingSystem;
        }

        pxDirectX9->Context = pxDirect3DCreate(D3D_SDK_VERSION); // Create DirectX context, alternative Direct3DCreate9Ex()
        const PXBool success = pxDirectX9->Context;

        if (!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "DirectX9",
                "Initialize",
                "Instance creation failed",
                pxDirectX9->Context
            );
#endif

            return PXActionFailedInitialization;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX9",
            "Initialize",
            "Instance Created 0x%p",
            pxDirectX9->Context
        );
#endif
    }

    // Get Additional Lib
    {
        const PXActionResult libraryCompilerLoad = PXLibraryOpenA(&pxDirectX9->LibraryDirectShaderCompiler, "D3DCOMPILER_43.DLL");

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirectShaderCompiler, &pxDirectX9->ShaderCompile, "D3DCompile");

        const PXActionResult libraryExtensionLoad = PXLibraryOpenA(&pxDirectX9->LibraryDirect3DExtension, "D3DX9_43.DLL");

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGet, "D3DXGetShaderConstantTable");
        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGetEx, "D3DXGetShaderConstantTableEx");
    }

    // Merge all functions
    {
        PXGraphic* pxGraphic = pxGraphicInitializeInfo->Graphic;
        pxGraphic->TextureAction = PXDirectX9TextureAction;
        pxGraphic->ShaderVariableSet = PXDirectX9ShaderVariableSet;
        pxGraphic->ScreenBufferRead = PXNull;
        pxGraphic->DrawModeSet = PXNull;
        pxGraphic->DrawColorRGBAF = PXNull;
        pxGraphic->RectangleDraw = PXNull;
        pxGraphic->RectangleDrawTx = PXNull;
        pxGraphic->SwapIntervalSet = PXDirectX9SwapIntervalSet;
        pxGraphic->SwapIntervalGet = PXDirectX9SwapIntervalGet;
        pxGraphic->DevicePhysicalListAmount = PXNull;
        pxGraphic->DevicePhysicalListFetch = PXNull;
        pxGraphic->Release = PXNull;
        pxGraphic->Select = PXDirectX9Select;
        pxGraphic->Deselect = PXDirectX9Deselect;
        pxGraphic->Clear = PXDirectX9Clear;
        pxGraphic->SceneDeploy = PXDirectX9SceneDeploy;
        pxGraphic->ViewPortGet = PXDirectX9ViewportSet;
        pxGraphic->ViewPortSet = PXDirectX9ViewportGet;
        pxGraphic->ShaderProgramCreate = PXDirectX9ShaderProgramCreate;
        pxGraphic->ShaderProgramSelect = PXDirectX9ShaderProgramSelect;
        pxGraphic->ShaderProgramDelete = PXDirectX9ShaderProgramDelete;
        pxGraphic->SceneBegin = PXDirectX9SceneBegin;
        pxGraphic->SceneEnd = PXDirectX9SceneEnd;
        pxGraphic->DrawScriptCreate = PXDirectX9DrawScriptCreate;
        pxGraphic->DrawScriptBegin = PXDirectX9DrawScriptBegin;
        pxGraphic->DrawScriptEnd = PXDirectX9DrawScriptEnd;
        pxGraphic->DrawScriptDelete = PXDirectX9DrawScriptDelete;
        pxGraphic->DrawScriptExecute = PXDirectX9DrawScriptExecute;
        pxGraphic->ModelRegister = PXDirectX9ModelRegister;
        pxGraphic->ModelDraw = PXDirectX9ModelDraw;
        pxGraphic->ModelSelect = PXNull;
        pxGraphic->ModelRelease = PXNull;
        pxGraphic->LightSet = PXDirectX9LightSet;
        pxGraphic->LightGet = PXDirectX9LightGet;
        pxGraphic->LightEnableSet = PXDirectX9LightEnableSet;
        pxGraphic->LightEnableGet = PXDirectX9LightEnableGet;
    }

    //PXDirectX9DevicePhysicalListFetchFunction(pxDirectX9, pxGraphicInitializeInfo->Graphic->DevicePhysicalListSize, pxGraphicInitializeInfo->Graphic->DevicePhysicalList);

    D3DPRESENT_PARAMETERS presentParameters;
    PXClear(D3DPRESENT_PARAMETERS, &presentParameters);
    presentParameters.Windowed = TRUE;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.SwapEffect = D3DSWAPEFFECT_COPY; // D3DSWAPEFFECT_DISCARD

    const HRESULT deviceCapaResult = pxDirectX9->Context->lpVtbl->GetDeviceCaps(pxDirectX9->Context, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &pxDirectX9->DeviceCapabilitiesCurrent);

    const HRESULT createResult = pxDirectX9->Context->lpVtbl->CreateDevice
                                 (
                                     pxDirectX9->Context,
                                     D3DADAPTER_DEFAULT,
                                     D3DDEVTYPE_HAL,
                                     pxGraphicInitializeInfo->WindowReference->Info.Handle.WindowID,
                                     D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                     &presentParameters,
                                     &pxDirectX9->Device
                                 );
    const PXBool success = createResult == 0;

    if(!success)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "DirectX9",
            "Initialize",
            "Device creation failed 0x%p",
            pxDirectX9->Device
        );
#endif

        return PXActionFailedInitialization;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX9",
        "Initialize",
        "Device created 0x%p",
        pxDirectX9->Device
    );
#endif

#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9Release(PXDirectX9* const pxDirectX9)
{
#if OSUnix
#elif OSWindows

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingDeallocation,
        "DirectX9",
        "Release",
        "Destruct session"
    );
#endif

    if (pxDirectX9->Device)
    {
        pxDirectX9->Device->lpVtbl->Release(pxDirectX9->Device);
    }

    if (pxDirectX9->Context)
    {
        pxDirectX9->Context->lpVtbl->Release(pxDirectX9->Context);
    }

    PXLibraryClose(&pxDirectX9->LibraryDirect3D);

    PXClear(PXDirectX9, pxDirectX9);
#endif

}

void PXAPI PXDirectX9Select(PXDirectX9* const pxDirectX9)
{

}

void PXAPI PXDirectX9Deselect(PXDirectX9* const pxDirectX9)
{

}

PXActionResult PXAPI PXDirectX9TextureAction(PXDirectX9* const pxDirectX9, PXGraphicTexturInfo* const pxGraphicTexturInfo)
{
#if OSUnix
#elif OSWindows

    switch(pxGraphicTexturInfo->Action)
    {
    case PXResourceActionCreate:
    {
        const UINT levels = 0;
        const DWORD usage = 0; // D3DUSAGE
        const D3DPOOL pool = D3DPOOL_DEFAULT;
        HANDLE sharedHandle = PXNull;

        switch(pxGraphicTexturInfo->Type)
        {
        case PXGraphicTextureType2D:
        {
            PXTexture2D* const pxTexture2D = (PXTexture2D*)pxGraphicTexturInfo->TextureReference;
            const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture2D->Image->Format);

            const HRESULT result = pxDirectX9->Device->lpVtbl->CreateTexture
                                   (
                                       pxDirectX9->Device,
                                       pxTexture2D->Image->Width,
                                       pxTexture2D->Image->Height,
                                       levels,
                                       usage,
                                       format,
                                       pool,
                                       &(IDirect3DTexture9*)pxTexture2D->Info.Handle.DirectXInterface,
                                       &sharedHandle
                                   );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX9",
                "Texture2D",
                "Created (%ix%i) 0x%p",
                pxTexture2D->Image->Width,
                pxTexture2D->Image->Height,
                pxTexture2D->Info.Handle.DirectXInterface
            );
#endif

            break;
        }

        case PXGraphicTextureType3D:
        {
            PXTexture3D* const pxTexture3D = (PXTexture3D*)pxGraphicTexturInfo->TextureReference;
            const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture3D->Image->Format);

            const HRESULT result = pxDirectX9->Device->lpVtbl->CreateVolumeTexture
                                   (
                                       pxDirectX9->Device,
                                       pxTexture3D->Image->Width,
                                       pxTexture3D->Image->Height,
                                       pxTexture3D->Image->Depth,
                                       levels,
                                       usage,
                                       format,
                                       pool,
                                       &(IDirect3DVolumeTexture9*)pxTexture3D->Info.Handle.DirectXInterface,
                                       &sharedHandle
                                   );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX9",
                "Texture3D created (%ix%ix%i) 0x%p",
                pxTexture3D->Image->Width,
                pxTexture3D->Image->Height,
                pxTexture3D->Image->Depth,
                pxTexture3D->Info.Handle.DirectXInterface
            );
#endif

            break;
        }

        case PXGraphicTextureTypeCubeContainer:
        {
            PXTextureCube* const pxTextureCube = (PXTextureCube*)pxGraphicTexturInfo->TextureReference;
            const D3DFORMAT format = PXDirectXColorFormatFromID(pxTextureCube->Format);
            const UINT edgeLength = 0;

            const HRESULT result = pxDirectX9->Device->lpVtbl->CreateCubeTexture
                                   (
                                       pxDirectX9->Device,
                                       edgeLength,
                                       levels,
                                       usage,
                                       format,
                                       pool,
                                       &(IDirect3DCubeTexture9*)pxTextureCube->Info.Handle.DirectXInterface,
                                       &sharedHandle
                                   );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX9",
                "TextureCube created 0x%p",
                pxTextureCube->Info.Handle.DirectXInterface
            );
#endif

            break;

        }

        default:
            return PXActionRefusedNotSupportedByLibrary;
        }

        break;
    }
    case PXResourceActionDelete:
    {
        break;
    }
    case PXResourceActionUpdate:
    {
        break;
    }
    default:
        return PXActionRefusedArgumentInvalid;
    }

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount)
{
#if OSUnix
#elif OSWindows

    for(PXSize i = 0; i < amount; i++)
    {
        PXShader* const pxShader = &shaderList[i];
        PXFile* const shaderFile = pxShader->ShaderFile;


        PXText pxTextFileName;
        PXTextConstructBufferA(&pxTextFileName, PXPathSizeMax);

        PXFilePathGet(shaderFile, &pxTextFileName);

        ID3DBlob* shaderByteCode = PXNull;
        ID3DBlob* shaderCompileErrorMessage = PXNull;

        char shaderVersion[8];

        switch(pxShader->Type)
        {
        case PXShaderTypeVertex:
        {
            if(pxShader->VersionMajor == 0 && pxShader->VersionMinor == 0)
            {
                pxShader->VersionMajor = D3DSHADER_VERSION_MAJOR(pxDirectX9->DeviceCapabilitiesCurrent.VertexShaderVersion);
                pxShader->VersionMinor = D3DSHADER_VERSION_MINOR(pxDirectX9->DeviceCapabilitiesCurrent.VertexShaderVersion);
            }

            PXTextPrintA(shaderVersion, 8, "vs_%i_%i", pxShader->VersionMajor, pxShader->VersionMinor);
            break;
        }
        case PXShaderTypePixel:
        {
            if(pxShader->VersionMajor == 0 && pxShader->VersionMinor == 0)
            {
                pxShader->VersionMajor = D3DSHADER_VERSION_MAJOR(pxDirectX9->DeviceCapabilitiesCurrent.PixelShaderVersion);
                pxShader->VersionMinor = D3DSHADER_VERSION_MINOR(pxDirectX9->DeviceCapabilitiesCurrent.PixelShaderVersion);
            }

            PXTextPrintA(shaderVersion, 8, "ps_%i_%i", pxShader->VersionMajor, pxShader->VersionMinor);
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
        }

        PXD3DCompile pxD3DCompile = (PXD3DCompile)pxDirectX9->ShaderCompile;

        const HRESULT resultID = pxD3DCompile // d3dcompiler_47.dll, d3dcompiler.h
                                 (
                                     shaderFile->Data,
                                     shaderFile->DataUsed,
                                     &pxTextFileName.TextA, // Name?
                                     PXNull, // Makro count?
                                     PXNull, // include?
                                     "main", // entry point
                                     shaderVersion, // target version
                                     D3DCOMPILE_ENABLE_STRICTNESS, // flag 1
                                     0, // flag 2
                                     &shaderByteCode,
                                     &shaderCompileErrorMessage
                                 );
        const PXActionResult vertexShaderCompileResult = PXWindowsHandleErrorFromID(resultID);

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
                "DirectX9",
                "Shader-Compile",
                "HLSL %s\n"
                "%s",
                shaderVersion,
                data
            );
#endif

            shaderCompileErrorMessage->lpVtbl->Release(shaderCompileErrorMessage);

            return PXActionFailedResourceCompile;
        }


        switch(pxShader->Type)
        {
        case PXShaderTypeVertex:
        {
            const HRESULT result = pxDirectX9->Device->lpVtbl->CreateVertexShader
                                   (
                                       pxDirectX9->Device,
                                       shaderByteCode,
                                       &(IDirect3DVertexShader9*)pxShader->Info.Handle.DirectXInterface
                                   );
            const PXActionResult vertexShaderCreateResult = PXWindowsHandleErrorFromID(result);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX9",
                "Shader-Create",
                "Vertex 0x%p",
                pxShader->Info.Handle.DirectXInterface
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
                                       &(IDirect3DPixelShader9*)pxShader->Info.Handle.DirectXInterface
                                   );
            const PXActionResult pixelShaderCreateResult = PXWindowsHandleErrorFromID(result);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "DirectX9",
                "Shader-Create",
                "Pixel 0x%p",
                pxShader->Info.Handle.DirectXInterface
            );
#endif

            break;
        }
        default:
            return PXActionRefusedFormatNotSupported;
        }


        // Extract variable IDs
        {
            typedef struct CTHeader_
            {
                PXInt32U Size;
                PXInt32U Creator;
                PXInt32U Version;
                PXInt32U Constants;
                PXInt32U ConstantInfo;
                PXInt32U Flags;
                PXInt32U Target;
            }
            CTHeader;

            typedef struct CTInfo_
            {
                PXInt32U Name;
                PXInt16U RegisterSet;
                PXInt16U RegisterIndex;
                PXInt16U RegisterCount;
                PXInt16U Reserved;
                PXInt32U TypeInfo;
                PXInt32U DefaultValue;
            }
            CTInfo;

            typedef struct CTType_
            {
                PXInt16U Class;
                PXInt16U Type;
                PXInt16U Rows;
                PXInt16U Columns;
                PXInt16U Elements;
                PXInt16U StructMembers;
                PXInt32U StructMemberInfo;
            }
            CTType;

            // Shader instruction opcodes
            const PXInt32U SIO_COMMENT = 0x0000FFFE;
            const PXInt32U SIO_END = 0x0000FFFF;
            const PXInt32U SI_OPCODE_MASK = 0x0000FFFF;
            const PXInt32U SI_COMMENTSIZE_MASK = 0x7FFF0000;
            const PXInt32U CTAB_CONSTANT = 0x42415443;

            const PXInt32U* shaderByteCodeCursor = (PXInt32U*)shaderByteCode;
            PXBool success = PXFalse;

            while((*++shaderByteCodeCursor != SIO_END))
            {
                if((*shaderByteCodeCursor & SI_OPCODE_MASK) == SIO_COMMENT)
                {
                    // Check for CTAB comment
                    PXInt32U comment_size = (*shaderByteCodeCursor & SI_COMMENTSIZE_MASK) >> 16;

                    if(*(shaderByteCodeCursor + 1) != CTAB_CONSTANT)
                    {
                        shaderByteCodeCursor += comment_size;
                        continue;
                    }

                    // Read header
                    const char* ctab = shaderByteCodeCursor + 2;
                    size_t ctab_size = (comment_size - 1) * 4;

                    const CTHeader* header = (CTHeader*)ctab;
                    char* m_creator = ctab + header->Creator;
                    const PXBool isInvalidSize = ctab_size < sizeof(*header) || header->Size != sizeof(*header);

                    //if(isInvalidSize)
                    //    return false;

                    // Read constants
                    pxShader->VariableListAmount = header->Constants;
                    PXNewList(PXShaderVariable, header->Constants, &pxShader->VariableListData, PXNull);

                    const CTInfo* info = (CTInfo*)(ctab + header->ConstantInfo);

                    for(PXInt32U i = 0; i < header->Constants; ++i)
                    {
                        PXShaderVariable* const pxShaderVariable = &pxShader->VariableListData[i];

                        const CTType* type = (CTType*)(ctab + info[i].TypeInfo);

                        char* name = ctab + info[i].Name;

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

                        case 2: // RS_FLOAT4
                            pxShaderVariable->DataType = PXShaderVariableTypeFloatVector4;
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


    return PXActionSuccessful;


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
                               shaderFilePath->TextW,
                               PXNull,
                               D3D_COMPILE_STANDARD_FILE_INCLUDE,
                               "vs_main",
                               "vs_5_0",
                               flags,
                               0,
                               &(ID3DBlob*)pxShader->Content,
                               &error_blob
                           );
    const PXBool failed = FAILED(result);



    return PXActionCompilingError;
#endif
}

PXActionResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram)
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
            "DirectX9",
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
            "DirectX9",
            "Shader:Pixel select 0x%p",
            pxShader->ResourceID.DirectXInterface
        );
#endif

        break;
    }
    default:
        return PXActionRefusedFormatNotSupported;
    }
#endif
}

PXActionResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariable)
{
    if(!(pxDirectX9 && pxShaderProgram && pxShaderVariable))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows


    HRESULT setShaderConstantID = PXNull;

    // Plan A, use pre-cached data
    switch(pxShaderVariable->ShaderType)
    {
    case PXShaderTypeVertex:
    {
        switch(pxShaderVariable->DataType)
        {
        case PXShaderVariableTypeBoolSignle:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantB
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }
        case PXShaderVariableTypeFloatVector4:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantF
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }
        case PXShaderVariableTypeInt32SVector4:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetVertexShaderConstantI
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
        }

        break;
    }
    case PXShaderTypePixel:
    {
        switch(pxShaderVariable->DataType)
        {
        case PXShaderVariableTypeBoolSignle:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantB
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }
        case PXShaderVariableTypeFloatVector4:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantF
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }
        case PXShaderVariableTypeInt32SVector4:
        {
            setShaderConstantID = pxDirectX9->Device->lpVtbl->SetPixelShaderConstantI
                                  (
                                      pxDirectX9->Device,
                                      pxShaderVariable->RegisterIndex,
                                      pxShaderVariable->Data,
                                      pxShaderVariable->Amount
                                  );
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
        }

        break;
    }

    default:
        return PXActionRefusedArgumentInvalid;
    }

    // Handle result, exit on success
    {
        const PXActionResult shaderConstantResult = PXWindowsHandleErrorFromID(setShaderConstantID);

        if(PXActionSuccessful == shaderConstantResult)
        {
            return PXActionSuccessful;
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
        case PXShaderVariableTypeFloatSingle:
        {
            if(pxShaderVariable->Amount > 1)
            {
                setResult = d3dxConstantTable->lpVtbl->SetFloatArray(d3dxConstantTable, pxDirectX9->Device, handle, pxShaderVariable->Data, pxShaderVariable->Amount);
            }
            else
            {
                float value = *(float*)pxShaderVariable->Data;

                setResult = d3dxConstantTable->lpVtbl->SetFloat(d3dxConstantTable, pxDirectX9->Device, handle, value);
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
        case PXShaderVariableTypeFloatVector4:
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
            return PXActionRefusedArgumentInvalid;
        }

        // Hanle erturn code

        const PXActionResult shaderConstantResult = PXWindowsHandleErrorFromID(setShaderConstantID);

        return shaderConstantResult;
    }
#endif

#endif

}

PXActionResult PXAPI PXDirectX9SceneBegin(PXDirectX9* const pxDirectX9)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginScene(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9SceneEnd(PXDirectX9* const pxDirectX9)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->EndScene(pxDirectX9->Device);
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType)
{
#if OSUnix
#elif OSWindows
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
        return PXActionRefusedFormatNotSupported;
    }

    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateStateBlock
                           (
                               pxDirectX9->Device,
                               stateBlcokType,
                               &(IDirect3DStateBlock9*)pxDrawScript->Info.Handle.DirectXInterface
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginStateBlock(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->EndStateBlock
                           (
                               pxDirectX9->Device,
                               &(IDirect3DStateBlock9*)pxDrawScript->Info.Handle.DirectXInterface
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
#if OSUnix
#elif OSWindows
    IDirect3DStateBlock9* const direct3DStateBlock9 = (IDirect3DStateBlock9*)pxDrawScript->Info.Handle.DirectXInterface;

    const ULONG result = direct3DStateBlock9->lpVtbl->Release(direct3DStateBlock9);

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ViewportSet(PXDirectX9* const pxDirectX9, const PXViewPort* const pxViewPort)
{
#if OSUnix
#elif OSWindows
    D3DVIEWPORT9 viewPort;

    // Convert
    {
        viewPort.X = pxViewPort->X;
        viewPort.Y = pxViewPort->Y;
        viewPort.Width = pxViewPort->Width;
        viewPort.Height = pxViewPort->Height;
        viewPort.MinZ = pxViewPort->ClippingMinimum;
        viewPort.MaxZ = pxViewPort->ClippingMaximum;
    }

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetViewport
                           (
                               pxDirectX9->Device,
                               &viewPort
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9ViewportGet(PXDirectX9* const pxDirectX9, PXViewPort* const pxViewPort)
{
#if OSUnix
#elif OSWindows
    D3DVIEWPORT9 viewPort;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetViewport
                           (
                               pxDirectX9->Device,
                               &viewPort
                           );

    // Convert
    {
        pxViewPort->X = viewPort.X;
        pxViewPort->Y = viewPort.Y;
        pxViewPort->Width = viewPort.Width;
        pxViewPort->Height = viewPort.Height;
        pxViewPort->ClippingMinimum = viewPort.MinZ;
        pxViewPort->ClippingMaximum = viewPort.MaxZ;
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9MaterialSet(PXDirectX9* const pxDirectX9, const PXMaterial* const pxMaterial)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    D3DMATERIAL9 d3dMaterial;
    PXDirectXMaterialFromPXMaterial(&d3dMaterial, pxMaterial);

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetMaterial
                           (
                               pxDirectX9->Device,
                               &d3dMaterial
                           );
    const PXActionResult pxActionResult = PXWindowsHandleErrorFromID(result);

    return pxActionResult;

#endif
}

PXActionResult PXAPI PXDirectX9MaterialGet(PXDirectX9* const pxDirectX9, PXMaterial* const pxMaterial)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    D3DMATERIAL9 d3dMaterial;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetMaterial
                           (
                               pxDirectX9->Device,
                               &d3dMaterial
                           );
    const PXActionResult pxActionResult = PXWindowsHandleErrorFromID(result);

    PXDirectXMaterialToPXMaterial(pxMaterial, &d3dMaterial);

    return pxActionResult;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9* const pxDirectX9, const PXInt32U interval)
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


    if (bVysncEnabled)
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
}

PXActionResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9* const pxDirectX9, PXInt32U* const interval)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9* const pxDirectX9, PXInt32U* const value)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    *value = pxDirectX9->Device->lpVtbl->GetAvailableTextureMem(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9* const pxDirectX9, PXInt32U* const amountOfAdapters)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    *amountOfAdapters = pxDirectX9->Context->lpVtbl->GetAdapterCount(pxDirectX9->Context);
#endif

    return PXActionSuccessful;;
}

PXActionResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9* const pxDirectX9, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXInt32U amountOfDXDevices = 0;

    PXDirectX9DevicePhysicalListAmountFunction(pxDirectX9, &amountOfDXDevices);

    for (PXInt32U i = 0; i < amountOfDXDevices; i++)
    {
        D3DADAPTER_IDENTIFIER9 adapterIdentifier;

        PXClear(D3DADAPTER_IDENTIFIER9, &adapterIdentifier);

        pxDirectX9->Context->lpVtbl->GetAdapterIdentifier(pxDirectX9->Context, i, 0, &adapterIdentifier);

        for (PXInt32U deviceIndex = 0; deviceIndex < pxGraphicDevicePhysicalListSize; deviceIndex++)
        {
            PXGraphicDevicePhysical* pxGraphicDevicePhysicalCurrent = &pxGraphicDevicePhysicalList[i];

            PXBool found = PXTextCompareA(adapterIdentifier.DeviceName, 32, pxGraphicDevicePhysicalCurrent->DeviceDisplay, PXDeviceDisplaySize);

            if (found)
            {
                PXTextCopyA(adapterIdentifier.Driver, MAX_DEVICE_IDENTIFIER_STRING, pxGraphicDevicePhysicalCurrent->Driver, MAX_DEVICE_IDENTIFIER_STRING);
                break;
            }
        }
    }
#endif
}

PXActionResult PXAPI PXDirectX9Clear(PXDirectX9* const pxDirectX9, const PXColorRGBAF* const pxColorRGBAF)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    const UINT colorI8 = D3DCOLOR_ARGB
                         (
                             (PXInt8U)(pxColorRGBAF->Alpha * 0xFF),
                             (PXInt8U)(pxColorRGBAF->Red * 0xFF),
                             (PXInt8U)(pxColorRGBAF->Green * 0xFF),
                             (PXInt8U)(pxColorRGBAF->Blue * 0xFF)
                         );

    const HRESULT result = pxDirectX9->Device->lpVtbl->Clear
                           (
                               pxDirectX9->Device,
                               0,
                               0,
                               D3DCLEAR_TARGET,
                               colorI8,
                               1.0f,
                               0
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9SceneDeploy(PXDirectX9* const pxDirectX9)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->Present
                           (
                               pxDirectX9->Device,
                               0,
                               0,
                               0,
                               0
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9* const pxDirectX9, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount)
{
#if OSUnix
#elif OSWindows
    const D3DPRIMITIVETYPE primitiveType = PXDirectXDrawTypeFromPX(PXGraphicDrawMode);

    const HRESULT result = pxDirectX9->Device->lpVtbl->DrawPrimitive
                           (
                               pxDirectX9->Device,
                               primitiveType,
                               startVertex,
                               primitiveCount
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9* const pxDirectX9, const PXVertexBufferFormat pxVertexBufferFormat)
{
#if OSUnix
#elif OSWindows
    const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetFVF
                           (
                               pxDirectX9->Device,
                               flagID
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9* const pxDirectX9, PXVertexBufferFormat* const pxVertexBufferFormat)
{
#if OSUnix
#elif OSWindows
    DWORD flagID = 0;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetFVF
                           (
                               pxDirectX9->Device,
                               &flagID
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->SetStreamSource
                           (
                               pxDirectX9->Device,
                               StreamNumber,
                               pxVertexBuffer->Info.Handle.DirectXInterface,
                               OffsetInBytes,
                               Stride
                           );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ModelDraw(PXDirectX9* const pxDirectX9, PXModel* const pxModel, const PXCamera* const pxCamera)
{
    const PXSize stride = PXVertexBufferFormatStrideSize(pxModel->Mesh.VertexBuffer.Format);

    PXDirectX9StreamSourceSet(pxDirectX9, 0, &pxModel->Mesh.VertexBuffer, 0, stride);
    PXDirectX9VertexFixedFunctionSet(pxDirectX9, pxModel->Mesh.VertexBuffer.Format);
    PXDirectX9PrimitiveDraw(pxDirectX9, PXGraphicDrawModeTriangle, 0, 1);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index)
{
#if OSUnix
#elif OSWindows
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
        return PXActionRefusedArgumentInvalid;
    }

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetLight(pxDirectX9->Device, index, &d3dLight9);

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index)
{
#if OSUnix
#elif OSWindows
    BOOL enableBool = 0;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLightEnable(pxDirectX9->Device, index, &enableBool);

    pxLight->Enabled = enableBool;
    // *enable = enableBool;
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightEnableSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, const PXBool enable)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->LightEnable(pxDirectX9->Device, index, enable);

    pxLight->Enabled = enable;
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightEnableGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, PXBool* const enable)
{
#if OSUnix
#elif OSWindows
    D3DLIGHT9 d3dLight9;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLight(pxDirectX9->Device, index, &d3dLight9);

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

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderCreate(PXDirectX9* const pxDirectX9, PXShader* const pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderSelect(PXDirectX9* const pxDirectX9, PXShader* const pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderCompile(PXDirectX9* const pxDirectX9, PXShader* const pxShader, const PXText* const shaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9* const pxDirectX9, PXVertexBuffer* const pxVertexBuffer)
{
#if OSUnix
#elif OSWindows
    const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBuffer->Format);

    const HRESULT bufferCreateResult = pxDirectX9->Device->lpVtbl->CreateVertexBuffer
                                       (
                                           pxDirectX9->Device,
                                           pxVertexBuffer->VertexDataSize,
                                           0,
                                           flagID,
                                           D3DPOOL_DEFAULT,
                                           &(IDirect3DVertexBuffer9*)pxVertexBuffer->Info.Handle.DirectXInterface,
                                           PXNull
                                       );
    const PXBool bufferCreateSuccess = bufferCreateResult > 0;


    const PXBool hasData = pxVertexBuffer->VertexData && pxVertexBuffer->VertexDataSize;

    if(!hasData)
    {
        return PXActionInvalid;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX9",
        "Vertex buffer created 0x%p",
        pxVertexBuffer->Info.Handle.DirectXInterface
    );
#endif

    IDirect3DVertexBuffer9* const vertexBuffer = (IDirect3DVertexBuffer9*)pxVertexBuffer->Info.Handle.DirectXInterface;
    void* targetAdress = PXNull;

    const HRESULT lockResult = vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, pxVertexBuffer->VertexDataSize, &targetAdress, 0);

    PXMemoryCopy(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize, targetAdress, MemorySizeUnkown);

    const HRESULT unlockResult = vertexBuffer->lpVtbl->Unlock(vertexBuffer);

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9* const pxDirectX9, PXIndexBuffer* const pxIndexBuffer)
{
#if OSUnix
#elif OSWindows
    D3DFORMAT dataFormat = D3DFMT_UNKNOWN;

    // fetch format
    {
        switch(pxIndexBuffer->IndexDataType)
        {
        case PXDataTypeInt16SLE:
        case PXDataTypeInt16ULE:
        case PXDataTypeInt16SBE:
        case PXDataTypeInt16UBE:
        {
            dataFormat = D3DFMT_INDEX16;
            break;
        }
        case PXDataTypeInt32SLE:
        case PXDataTypeInt32ULE:
        case PXDataTypeInt32SBE:
        case PXDataTypeInt32UBE:
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

    const HRESULT bufferCreateResult = pxDirectX9->Device->lpVtbl->CreateIndexBuffer
                                       (
                                           pxDirectX9->Device,
                                           pxIndexBuffer->IndexDataSize,
                                           0,
                                           dataFormat,
                                           0,
                                           &(IDirect3DIndexBuffer9*)pxIndexBuffer->Info.Handle.DirectXInterface,
                                           PXNull
                                       );
#endif

    // Fill data

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9* const pxDirectX9, PXRenderTarget* const pxRenderTarget)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9* const pxDirectX9, PXDepthStencilSurface* const pxDepthStencilSurface)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ModelRegister(PXDirectX9* const pxDirectX9, PXModel* const pxModel)
{
    PXDirectX9VertexBufferCreate(pxDirectX9, &pxModel->Mesh.VertexBuffer);
    PXDirectX9IndexBufferCreate(pxDirectX9, &pxModel->Mesh.IndexBuffer);

    return PXActionSuccessful;
}

















#if OSUnix
#elif OSWindows
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

void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial)
{
    pxMaterial->Diffuse[0] = d3dMaterial->Diffuse.r;
    pxMaterial->Diffuse[1] = d3dMaterial->Diffuse.g;
    pxMaterial->Diffuse[2] = d3dMaterial->Diffuse.b;
    pxMaterial->Diffuse[3] = d3dMaterial->Diffuse.a;

    pxMaterial->Ambient[0] = d3dMaterial->Ambient.r;
    pxMaterial->Ambient[1] = d3dMaterial->Ambient.g;
    pxMaterial->Ambient[2] = d3dMaterial->Ambient.b;
    pxMaterial->Ambient[3] = d3dMaterial->Ambient.a;

    pxMaterial->Specular[0] = d3dMaterial->Specular.r;
    pxMaterial->Specular[1] = d3dMaterial->Specular.g;
    pxMaterial->Specular[2] = d3dMaterial->Specular.b;
    pxMaterial->Specular[3] = d3dMaterial->Specular.a;

    pxMaterial->Emission[0] = d3dMaterial->Emissive.r;
    pxMaterial->Emission[1] = d3dMaterial->Emissive.g;
    pxMaterial->Emission[2] = d3dMaterial->Emissive.b;
    pxMaterial->Emission[3] = d3dMaterial->Emissive.a;

    pxMaterial->Power = d3dMaterial->Power;
}

PXInt32U PXAPI PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
    case PXVertexBufferFormatXYZFloat:
        return D3DFVF_XYZ;
    case PXVertexBufferFormatXYZC:
        return D3DFVF_XYZ | D3DFVF_DIFFUSE;
    case PXVertexBufferFormatXYZHWC:
        return D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

    default:
        return 0;
    }
}

void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial)
{
    d3dMaterial->Diffuse.r = pxMaterial->Diffuse[0];
    d3dMaterial->Diffuse.g = pxMaterial->Diffuse[1];
    d3dMaterial->Diffuse.b = pxMaterial->Diffuse[2];
    d3dMaterial->Diffuse.a = pxMaterial->Diffuse[3];

    d3dMaterial->Ambient.r = pxMaterial->Ambient[0];
    d3dMaterial->Ambient.g = pxMaterial->Ambient[1];
    d3dMaterial->Ambient.b = pxMaterial->Ambient[2];
    d3dMaterial->Ambient.a = pxMaterial->Ambient[3];

    d3dMaterial->Specular.r = pxMaterial->Specular[0];
    d3dMaterial->Specular.g = pxMaterial->Specular[1];
    d3dMaterial->Specular.b = pxMaterial->Specular[2];
    d3dMaterial->Specular.a = pxMaterial->Specular[3];

    d3dMaterial->Emissive.r = pxMaterial->Emission[0];
    d3dMaterial->Emissive.g = pxMaterial->Emission[1];
    d3dMaterial->Emissive.b = pxMaterial->Emission[2];
    d3dMaterial->Emissive.a = pxMaterial->Emission[3];

    d3dMaterial->Power = pxMaterial->Power;
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

D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode)
{
    switch(PXGraphicDrawMode)
    {
    case PXGraphicDrawModePoint:
        return D3DPT_POINTLIST;
    case PXGraphicDrawModeLineLoop:
        return D3DPT_LINELIST;
    case PXGraphicDrawModeLineStrip:
        return D3DPT_LINESTRIP;
    case PXGraphicDrawModeTriangle:
        return D3DPT_TRIANGLELIST;
    case PXGraphicDrawModeTriangleStrip:
        return D3DPT_TRIANGLESTRIP;
    case PXGraphicDrawModeTriangleFAN:
        return D3DPT_TRIANGLEFAN;

    default:
        return 0; // Invalid mode does not exist!
    }
}
#endif
