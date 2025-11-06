#include "PXDirectX9.h"

#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Graphic/PXGraphic.h>
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

#endif


const char PXDirectX9Text[] = "DirectX9";
const char PXDirectX9Library[] = "D3D9.DLL";
const char PXDirectX9Created[] = "Direct3DCreate9";


#if OSWindows
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

#endif

PXResult PXAPI PXDirectX9Initialize(PXDirectX9 PXREF pxDirectX9, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo)
{
#if OSUnix
#elif OSWindows

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
        const PXResult pxActionResult = PXLibraryOpenA(&pxDirectX9->LibraryDirect3D, PXDirectX9Library);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }
    }

    // Get init function and execute
    {
        PXDirect3DCreate9 pxDirect3DCreate;

        const PXResult pxActionResult = PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3D, &pxDirect3DCreate, PXDirectX9Created, PXTrue);

        if(PXActionSuccessful != pxActionResult)
        {
            PXLibraryClose(&pxDirectX9->LibraryDirect3D);

            return pxActionResult;
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

            return PXActionFailedInitialization;
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
        const PXResult libraryCompilerLoad = PXLibraryOpenA(&pxDirectX9->LibraryDirectShaderCompiler, "D3DCOMPILER_43.DLL");

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirectShaderCompiler, &pxDirectX9->ShaderCompile, "D3DCompile", PXTrue);

        const PXResult libraryExtensionLoad = PXLibraryOpenA(&pxDirectX9->LibraryDirect3DExtension, "D3DX9_43.DLL");

        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGet, "D3DXGetShaderConstantTable", PXTrue);
        PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3DExtension, &pxDirectX9->ShaderConstantTableGetEx, "D3DXGetShaderConstantTableEx", PXTrue);
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
        pxGraphicInitializeInfo->WindowReference->Info.Handle.WindowHandle,
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
            PXDirectX9Text,
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
        PXDirectX9Text,
        "Initialize",
        "Device created 0x%p",
        pxDirectX9->Device
    );
#endif

#endif


    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9Release(PXDirectX9 PXREF pxDirectX9)
{
#if OSUnix
#elif OSWindows

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
        pxDirectX9->Device->lpVtbl->Release(pxDirectX9->Device);
    }

    if(pxDirectX9->Context)
    {
        pxDirectX9->Context->lpVtbl->Release(pxDirectX9->Context);
    }

    PXLibraryClose(&pxDirectX9->LibraryDirect3D);

    PXClear(PXDirectX9, pxDirectX9);
#endif

    return PXActionSuccessful;
}

void PXAPI PXDirectX9Select(PXDirectX9 PXREF pxDirectX9)
{

}

void PXAPI PXDirectX9Deselect(PXDirectX9 PXREF pxDirectX9)
{

}

PXResult PXAPI PXDirectX9TextureAction(PXDirectX9 PXREF pxDirectX9, PXTexturInfo PXREF pxTexturInfo)
{
#if OSUnix
#elif OSWindows

    switch(pxTexturInfo->Action)
    {
        case PXResourceActionCreate:
        {
            const UINT levels = 0;
            const DWORD usage = 0; // D3DUSAGE
            const D3DPOOL pool = D3DPOOL_DEFAULT;
            HANDLE sharedHandle = PXNull;

            switch(pxTexturInfo->Type)
            {
                case PXTextureType2D:
                {
                    PXTexture PXREF pxTexture = (PXTexture*)pxTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture->Format);

                    const HRESULT resultID = pxDirectX9->Device->lpVtbl->CreateTexture
                    (
                        pxDirectX9->Device,
                        pxTexture->Width,
                        pxTexture->Height,
                        levels,
                        usage,
                        format,
                        pool,
                        &(IDirect3DTexture9*)pxTexture->Info.Handle.DirectXInterface,
                        &sharedHandle
                    );
                    const PXResult pxResult = PXErrorFromHRESULT(resultID);

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXDirectX9Text,
                        "Texture2D",
                        "Created (%ix%i) 0x%p",
                        pxTexture->Width,
                        pxTexture->Height,
                        pxTexture->Info.Handle.DirectXInterface
                    );
#endif

                    break;
                }

                case PXTextureType3D:
                {
                    PXTexture PXREF pxTexture = (PXTexture*)pxTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture->Format);

                    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateVolumeTexture
                    (
                        pxDirectX9->Device,
                        pxTexture->Width,
                        pxTexture->Height,
                        pxTexture->Depth,
                        levels,
                        usage,
                        format,
                        pool,
                        &(IDirect3DVolumeTexture9*)pxTexture->Info.Handle.DirectXInterface,
                        &sharedHandle
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXDirectX9Text,
                        "Texture3D created (%ix%ix%i) 0x%p",
                        pxTexture->Width,
                        pxTexture->Height,
                        pxTexture->Depth,
                        pxTexture->Info.Handle.DirectXInterface
                    );
#endif

                    break;
                }

                case PXTextureTypeCubeContainer:
                {
                    PXTexture PXREF pxTexture = (PXTexture*)pxTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture->Format);
                    const UINT edgeLength = 0;

                    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateCubeTexture
                    (
                        pxDirectX9->Device,
                        edgeLength,
                        levels,
                        usage,
                        format,
                        pool,
                        &(IDirect3DCubeTexture9*)pxTexture->Info.Handle.DirectXInterface,
                        &sharedHandle
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXDirectX9Text,
                        "TextureCube created 0x%p",
                        pxTexture->Info.Handle.DirectXInterface
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

PXResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXShader PXREF shaderList, const PXSize amount)
{
#if OSUnix
#elif OSWindows

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
                const PXResult vertexShaderCreateResult = PXErrorFromHRESULT(result);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXDirectX9Text,
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
                const PXResult pixelShaderCreateResult = PXErrorFromHRESULT(result);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXDirectX9Text,
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



    return PXActionCompilingError;
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
            return PXActionRefusedFormatNotSupported;
    }
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9 PXREF pxDirectX9, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariable)
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
                case PXShaderVariableTypePXF32Vector4:
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
                case PXShaderVariableTypePXF32Vector4:
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
        const PXResult shaderConstantResult = PXErrorFromHRESULT(setShaderConstantID);

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
                return PXActionRefusedArgumentInvalid;
        }

        // Hanle erturn code

        const PXResult shaderConstantResult = PXWindowsHandleErrorFromID(setShaderConstantID);

        return shaderConstantResult;
    }
#endif

#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9SceneBegin(PXDirectX9 PXREF pxDirectX9)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginScene(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9SceneEnd(PXDirectX9 PXREF pxDirectX9)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->EndScene(pxDirectX9->Device);
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType)
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

PXResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginStateBlock(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
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

PXResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
#if OSUnix
#elif OSWindows
    IDirect3DStateBlock9 PXREF direct3DStateBlock9 = (IDirect3DStateBlock9*)pxDrawScript->Info.Handle.DirectXInterface;

    const ULONG result = direct3DStateBlock9->lpVtbl->Release(direct3DStateBlock9);

#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ViewportSet(PXDirectX9 PXREF pxDirectX9, const PXViewPort PXREF pxViewPort)
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

PXResult PXAPI PXDirectX9ViewportGet(PXDirectX9 PXREF pxDirectX9, PXViewPort PXREF pxViewPort)
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

PXResult PXAPI PXDirectX9MaterialSet(PXDirectX9 PXREF pxDirectX9, const PXMaterial PXREF pxMaterial)
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
    const PXResult pxActionResult = PXErrorFromHRESULT(result);

    return pxActionResult;

#endif
}

PXResult PXAPI PXDirectX9MaterialGet(PXDirectX9 PXREF pxDirectX9, PXMaterial PXREF pxMaterial)
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
    const PXResult pxActionResult = PXErrorFromHRESULT(result);

    PXDirectXMaterialToPXMaterial(pxMaterial, &d3dMaterial);

    return pxActionResult;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
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

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF interval)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF value)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    * value = pxDirectX9->Device->lpVtbl->GetAvailableTextureMem(pxDirectX9->Device);
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF amountOfAdapters)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    * amountOfAdapters = pxDirectX9->Context->lpVtbl->GetAdapterCount(pxDirectX9->Context);
#endif

    return PXActionSuccessful;;
}

PXResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9 PXREF pxDirectX9, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical PXREF pxGraphicDevicePhysicalList)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXI32U amountOfDXDevices = 0;

    PXDirectX9DevicePhysicalListAmountFunction(pxDirectX9, &amountOfDXDevices);

    for(PXI32U i = 0; i < amountOfDXDevices; i++)
    {
        D3DADAPTER_IDENTIFIER9 adapterIdentifier;

        PXClear(D3DADAPTER_IDENTIFIER9, &adapterIdentifier);

        pxDirectX9->Context->lpVtbl->GetAdapterIdentifier(pxDirectX9->Context, i, 0, &adapterIdentifier);

        for(PXI32U deviceIndex = 0; deviceIndex < pxGraphicDevicePhysicalListSize; deviceIndex++)
        {
            PXGPUPhysical* pxGraphicDevicePhysicalCurrent = &pxGraphicDevicePhysicalList[i];

            PXBool found = PXTextCompareA(adapterIdentifier.DeviceName, 32, pxGraphicDevicePhysicalCurrent->DeviceDisplay, PXDeviceDisplaySize, 0);

            if(found)
            {
                PXTextCopyA(adapterIdentifier.Driver, MAX_DEVICE_IDENTIFIER_STRING, pxGraphicDevicePhysicalCurrent->Driver, MAX_DEVICE_IDENTIFIER_STRING);
                break;
            }
        }
    }
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9Clear(PXDirectX9 PXREF pxDirectX9, const PXColorRGBAF PXREF pxColorRGBAF)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    const UINT colorI8 = D3DCOLOR_ARGB
    (
        (PXI8U)(pxColorRGBAF->Alpha * 0xFF),
        (PXI8U)(pxColorRGBAF->Red * 0xFF),
        (PXI8U)(pxColorRGBAF->Green * 0xFF),
        (PXI8U)(pxColorRGBAF->Blue * 0xFF)
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

PXResult PXAPI PXDirectX9SceneDeploy(PXDirectX9 PXREF pxDirectX9)
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

PXResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9 PXREF pxDirectX9, const PXDrawMode PXGraphicDrawMode, const PXI32U startVertex, const PXI32U primitiveCount)
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

PXResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9 PXREF pxDirectX9, const void* pxVertexBufferFormat)
{
#if OSUnix
#elif OSWindows
    const DWORD flagID = 0;// PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetFVF
    (
        pxDirectX9->Device,
        flagID
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9 PXREF pxDirectX9, void* PXREF pxVertexBufferFormat)
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

PXResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXI32U OffsetInBytes, const PXI32U Stride)
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

PXResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXI32U* pOffsetInBytes, PXI32U* pStride)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ModelDraw(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel, const PXCamera PXREF pxCamera)
{
   // const PXSize stride = PXVertexBufferFormatStrideSize(pxModel->Mesh.VertexBuffer.Format);

  //  PXDirectX9StreamSourceSet(pxDirectX9, 0, &pxModel->Mesh.VertexBuffer, 0, stride);
   // PXDirectX9VertexFixedFunctionSet(pxDirectX9, pxModel->Mesh.VertexBuffer.Format);
    PXDirectX9PrimitiveDraw(pxDirectX9, PXDrawModeTriangle, 0, 1);

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9LightSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index)
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

PXResult PXAPI PXDirectX9LightGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index)
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

PXResult PXAPI PXDirectX9LightEnableSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, const PXBool enable)
{
#if OSUnix
#elif OSWindows
    const HRESULT result = pxDirectX9->Device->lpVtbl->LightEnable(pxDirectX9->Device, index, enable);

    pxLight->Enabled = enable;
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9LightEnableGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable)
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

PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderCreate(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderSelect(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ShaderCompile(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader, const PXText PXREF shaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXVertexBuffer PXREF pxVertexBuffer)
{
#if OSUnix
#elif OSWindows && 0
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
        PXDirectX9Text,
        "Vertex buffer created 0x%p",
        pxVertexBuffer->Info.Handle.DirectXInterface
    );
#endif

    IDirect3DVertexBuffer9 PXREF vertexBuffer = (IDirect3DVertexBuffer9*)pxVertexBuffer->Info.Handle.DirectXInterface;
    void* targetAdress = PXNull;

    const HRESULT lockResult = vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, pxVertexBuffer->VertexDataSize, &targetAdress, 0);

    PXMemoryCopy(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize, targetAdress, MemorySizeUnkown);

    const HRESULT unlockResult = vertexBuffer->lpVtbl->Unlock(vertexBuffer);

#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXIndexBuffer PXREF pxIndexBuffer)
{
#if OSUnix
#elif OSWindows
    D3DFORMAT dataFormat = D3DFMT_UNKNOWN;

    // fetch format
    {
        switch(pxIndexBuffer->DataType)
        {
            case PXTypeInt16SLE:
            case PXTypeInt16ULE:
            case PXTypeInt16SBE:
            case PXTypeInt16UBE:
            {
                dataFormat = D3DFMT_INDEX16;
                break;
            }
            case PXTypeInt32SLE:
            case PXTypeInt32ULE:
            case PXTypeInt32SBE:
            case PXTypeInt32UBE:
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
        0,//pxIndexBuffer->DataIndexSizeSegment,
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

PXResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9 PXREF pxDirectX9, PXRenderTarget PXREF pxRenderTarget)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9 PXREF pxDirectX9, PXDepthStencilSurface PXREF pxDepthStencilSurface)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX9ModelRegister(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel)
{
   // PXDirectX9VertexBufferCreate(pxDirectX9, &pxModel->Mesh.VertexBuffer);
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

void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial PXREF pxMaterial, const D3DMATERIAL9 PXREF d3dMaterial)
{
    pxMaterial->Diffuse.Red = d3dMaterial->Diffuse.r;
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
            return 0; // Invalid mode does not exist!
    }
}
#endif
