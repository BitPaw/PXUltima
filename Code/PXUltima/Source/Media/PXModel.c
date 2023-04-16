#include "PXModel.h"

#include <File/PXDataStream.h>
#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Media/Wavefront/PXWavefront.h>

void PXModelConstruct(PXModel* const model)
{
	MemoryClear(model, sizeof(PXModel));
}

void PXModelDestruct(PXModel* const model)
{

}

PXSize PXModelMaterialAmount(PXModel* const model)
{
    // Has materials?
    {
        const PXBool hasMaterial = model->MaterialList;

        if (!hasMaterial)
        {
            return 0; // No material
        }
    }

    // Fetch materials
    {
        PXDataStream materialData;
        unsigned int amount = 0;

        PXDataStreamFromExternal(&materialData, model->MaterialList, -1);
        PXDataStreamReadI32U(&materialData, &amount);

        return amount;
    }
}

PXBool PXModelMaterialGet(PXModel* const model, const PXSize materialID, PXMaterial* const pxMaterial)
{
    const PXSize amount = PXModelMaterialAmount(model);

    {
        const PXBool isInRange = amount >= materialID;

        MemoryClear(pxMaterial, sizeof(PXMaterial));

        if (!isInRange)
        {
            return PXNo;
        }
    }

    PXDataStream materialData;

    PXDataStreamFromExternal(&materialData, model->MaterialList, -1);
    PXDataStreamCursorAdvance(&materialData, sizeof(unsigned int));

    for (PXSize i = 0; i < amount; ++i)
    {
        const PXBool isGoal = i == materialID;
        unsigned short materialSize = 0;

        PXDataStreamReadI16U(&materialData, &materialSize); // Total size

        if (isGoal)
        {
            unsigned short range = 0;
            PXDataStreamReadI16U(&materialData, &range);
            pxMaterial->Name = PXDataStreamCursorPosition(&materialData);
            pxMaterial->NameSize = range;

            PXDataStreamCursorAdvance(&materialData, range);

            PXDataStreamReadI16U(&materialData, &range);
            pxMaterial->DiffuseTextureFilePath = PXDataStreamCursorPosition(&materialData);
            pxMaterial->DiffuseTextureFilePathSize = range;

            PXDataStreamCursorAdvance(&materialData, range);

            PXDataStreamReadB(&materialData, &pxMaterial->Ambient, sizeof(float) * 3u);
            PXDataStreamReadB(&materialData, &pxMaterial->Diffuse, sizeof(float) * 3u);
            PXDataStreamReadB(&materialData, &pxMaterial->Specular, sizeof(float) * 3u);
            PXDataStreamReadB(&materialData, &pxMaterial->Emission, sizeof(float) * 3u);

            break;
        }

        PXDataStreamCursorAdvance(&materialData, materialSize-2u);
    }

    return PXYes;
}

unsigned char PXModelSegmentsAmount(const PXModel* const model)
{
    if (!model->Data) // Has data
    {
        return 0;
    }

	return *(PXAdress)model->Data;
}

void* PXModelSegmentsAdressGet(const PXModel* const model, const PXSize index)
{
    unsigned char* ancer = (unsigned char*)model->Data + 1 + (index * (sizeof(char) + sizeof(int) * 2u));

    return ancer;
}

void PXModelSegmentsGet(const PXModel* const model, const PXSize index, MeshSegment* const meshSegment)
{
    PXDataStream dataStream;

    {
        void* const segmentAdress = PXModelSegmentsAdressGet(model, index);
        PXDataStreamFromExternal(&dataStream, segmentAdress, -1);
    }

    PXDataStreamReadI8U(&dataStream, &meshSegment->DrawStrideSize);
    PXDataStreamReadI32U(&dataStream, &meshSegment->DrawClusterSize);
    PXDataStreamReadI32U(&dataStream, &meshSegment->TextureID);

	meshSegment->VertexData = model->DataVertexList;
}

void PXModelSegmentsAdd(PXModel* const model, const unsigned int renderMode, const unsigned int renderSize, const unsigned int renderMaterial)
{
    const unsigned char amount = PXModelSegmentsAmount(model);
    const unsigned char segmentID = amount + 1u;

    *(PXAdress)model->Data = segmentID;

    PXDataStream dataStream;

    {
        void* const segmentAdress = PXModelSegmentsAdressGet(model, segmentID-1u);
        PXDataStreamFromExternal(&dataStream, segmentAdress, -1);
    }

    PXDataStreamWriteI8U(&dataStream, renderMode);
    PXDataStreamWriteI32U(&dataStream, renderSize);
    PXDataStreamWriteI32U(&dataStream, renderMaterial);

}

PXSize PXModelVertexDataStride(const PXModel* const model)
{
    return sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
}

PXActionResult PXModelLoadA(PXModel* const model, const char* const filePath)
{
	wchar_t filePathW[PathMaxSize];

	PXTextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

	PXActionResult actionResult = PXModelLoadW(model, filePathW);

	return actionResult;
}

PXActionResult PXModelLoadW(PXModel* const model, const wchar_t* const filePath)
{
    PXDataStream dataStream;

    PXDataStreamConstruct(&dataStream);
    PXModelConstruct(model);

    {
        const PXActionResult fileLoadingResult = PXDataStreamMapToMemoryW(&dataStream, filePath, 0, MemoryReadOnly);

        PXActionExitOnError(fileLoadingResult);
    }

    {
        const FileFormatExtension modelFileFormat = FilePathExtensionDetectTryW(filePath, PathMaxSize);
        const PXActionResult fileParsingResult = PXModelLoadD(model, &dataStream, modelFileFormat);

        PXActionExitOnSuccess(fileParsingResult);

        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = PXModelLoadD(model, &dataStream, imageFileFormat);

            fileFormatID++;
        } 
        while (fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXDataStreamDestruct(&dataStream);

        return fileGuessResult;
    }
}

PXActionResult PXModelLoadD(PXModel* const model, PXDataStream* const fileStream, const FileFormatExtension modelType)
{
    PXDataStream modelCompileCache;

    PXSize bytesRead = 0;
    ModelCompilerFunction modelCompilerFunction = 0;
    ModelParserFunction modelParserFunction = 0;

    PXDataStreamMapToMemory(&modelCompileCache, fileStream->DataSize * 200, MemoryReadAndWrite);

    switch (modelType)
    {
 /*       case FileFormatA3DS:
        {

            break;
        }
        case FileFormatFimBox:
        {

            break;
        }*/
        case FileFormatOBJ:
        {
            modelCompilerFunction = PXWavefrontFileCompile;
            modelParserFunction = PXWavefrontParseToModel;
            break;
        }
   /*     case FileFormatPLY:
        {

            break;
        }
        case FileFormatSTL:
        {

            break;
        }
        case FileFormatVRML:
        {

            break;
        }*/
        default:
        {
            return PXActionRefusedFormatNotSupported;
        }
    }

    const PXActionResult actionResult = modelCompilerFunction(fileStream, &modelCompileCache);
    const PXBool sucessfull = PXActionSuccessful == actionResult;

    if (sucessfull)
    {
        modelParserFunction(&modelCompileCache, model);
    }

    PXDataStreamDestruct(&modelCompileCache);

    return actionResult;
}