#include "PXShader.h"
#include <PX/OS/File/PXFile.h>

const char PXShaderText[] = "Shader";
const PXI8U PXShaderTextLength = sizeof(PXShaderText);
const PXECSRegisterInfoStatic PXShaderRegisterInfoStatic =
{
    {sizeof(PXShaderText), sizeof(PXShaderText), PXShaderText, TextFormatASCII},
    sizeof(PXShader),
    __alignof(PXShader),
    PXECSTypeSystem,
    PXShaderCreate,
    PXShaderRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXShaderRegisterInfoDynamic;


void PXAPI PXShaderRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
   
}

PXResult PXAPI PXShaderCreate(PXShader PXREF pxShader, PXShaderCreateInfo PXREF pxShaderCreateInfo)
{
    // is the data already present?
    const PXBool isDataEmbeddedDataPresent = PXTextHasData(&pxShaderCreateInfo->Embedded);

    if(isDataEmbeddedDataPresent)
    {
        PXBufferStore
        (
            &pxShader->Data,
            pxShaderCreateInfo->Embedded.Data,
            pxShaderCreateInfo->Embedded.SizeUsed,
            PXFalse
        );

        return PXResultOK;
    }


    const PXBool hasFilePath = PXTextHasData(&pxShaderCreateInfo->FilePath);

    if(hasFilePath)
    {
        PXFileCreateInfo pxFileCreateInfo;
        PXClear(PXFileCreateInfo, &pxFileCreateInfo);
        pxFileCreateInfo.FilePath = pxShaderCreateInfo->FilePath;
        pxFileCreateInfo.AccessMode = PXAccessModeReadOnly;
        pxFileCreateInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCreateInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        PXFileCreate(&pxShaderCreateInfo->File, &pxFileCreateInfo);
    }

    if(!pxShaderCreateInfo->File)
    {
        return PXResultInvalid;
    }

    pxShader->Data = *PXFileBufferGET(pxShaderCreateInfo->File);

    return PXResultOK;
}

PXResult PXAPI PXShaderRelease(PXShader PXREF pxShader)
{
    return PXResultOK;
}