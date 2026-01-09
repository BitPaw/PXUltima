#include "PXShader.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>

const char PXShaderProgramText[] = "ShaderProgram";
const char PXShaderText[] = "Shader";

const char* PXAPI PXShaderTypeToString(const PXShaderType pxShaderType)
{
    switch(pxShaderType)
    {
        case PXShaderTypeVertex:     // .vert - a vertex shader
            return "VertexShader";

        case PXShaderTypePixel:   // .frag - a fragment shader
            return "PixelShader";

        case PXShaderTypeTessellationControl:    // .tesc - a tessellation control shader
            return "TessellationControlShader";

        case PXShaderTypeTessellationEvaluation:     // .tese - a tessellation evaluation shader
            return "TessellationEvaluationShader";

        case PXShaderTypeGeometry:      // .geom - a geometry shader
            return "GeometryShader";

        case PXShaderTypeCompute:   // .comp - a compute shader
            return "ComputeShader";

        case PXShaderTypeInvalid:
        default:
            return PXNull;
    }
}

PXResult PXAPI PXShaderProgramCreate(PXShaderProgram** pxShaderProgramREF, PXShaderProgramCreateInfo PXREF pxShaderProgramCreateInfo)
{
    PXShaderProgram PXREF pxShaderProgram = 0;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXShaderProgramText,
        "Create",
        "ShaderProgram [PXID:%i]",
        pxShaderProgram->Info.ID
    );
#endif

    // is the data already present?
    const PXBool isDataPrensent = pxShaderProgramCreateInfo->ShaderVertex.A && pxShaderProgramCreateInfo->ShaderPixel.A;

    if(isDataPrensent)
    {
        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;

        PXBufferSet
        (
            &pxFileOpenInfo.Data,
            pxShaderProgramCreateInfo->ShaderVertex.A,
            pxShaderProgramCreateInfo->ShaderVertex.SizeUsed
        );

        PXFileOpen(pxShaderProgramCreateInfo->ShaderVertexFile, &pxFileOpenInfo);

        PXBufferSet
        (
            &pxFileOpenInfo.Data,
            pxShaderProgramCreateInfo->ShaderPixel.A,
            pxShaderProgramCreateInfo->ShaderPixel.SizeUsed
        );

        PXFileOpen(pxShaderProgramCreateInfo->ShaderPixelFile, &pxFileOpenInfo);

        pxShaderProgram->Info.Behaviour |= PXECSInfoStorageMemory;

        //PXResourceStorePath(&pxShaderProgram->Info, "<Internal>", -1);
    }
    else // load file
    {
        PXFileOpenInfo pxFileOpenFromPathInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenFromPathInfo);
        pxFileOpenFromPathInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        // TODO: we can make it a loop?

        pxFileOpenFromPathInfo.FilePath = pxShaderProgramCreateInfo->ShaderVertexFilePath;

        const PXResult vertexLoadResult = PXFileOpen(pxShaderProgramCreateInfo->ShaderVertexFile, &pxFileOpenFromPathInfo);

        if(PXResultOK != vertexLoadResult)
            return vertexLoadResult;

        pxFileOpenFromPathInfo.FilePath = pxShaderProgramCreateInfo->ShaderPixelFilePath;

        const PXResult fragmentLoadResult = PXFileOpen(pxShaderProgramCreateInfo->ShaderPixelFile, &pxFileOpenFromPathInfo);

        if(PXResultOK != fragmentLoadResult)
            return fragmentLoadResult;;

        pxShaderProgram->Info.Behaviour |= PXECSInfoStorageMemory;

        // PXResourceStorePath(&pxShaderProgram->Info, pxShaderProgramCreateData->ShaderVertexFilePath.A, -1);
    }

#if 0
    // Create job to register shader, register might need to be sync. 
    {
        PXResourceCreateInfo* pxResourceCreateInfoASYNC = PXMemoryHeapCallocT(PXResourceCreateInfo, 1);
        PXCopy(PXResourceCreateInfo, pxResourceCreateInfo, pxResourceCreateInfoASYNC);
        pxResourceCreateInfoASYNC->Flags |= PXResourceCreateBehaviourIsASYNCCall;

        PXThreadPoolQueueWork
        (
            PXNull,
            //PXResourceManagerAdd,
            pxResourceCreateInfoASYNC,
            PXNull,
            PXTaskParameterRelease | PXTaskExecuteASYNC
        );
    }
#endif

    return PXResultOK;
}