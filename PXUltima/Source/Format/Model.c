#include "Model.h"

#include <Memory/PXMemory.h>
#include <File/File.h>
#include <Text/Text.h>
#include <File/DataStream.h>
#include <Format/OBJ/OBJ.h>

void ModelConstruct(PXModel* const model)
{
	MemoryClear(model, sizeof(PXModel));
}

void ModelDestruct(PXModel* const model)
{
	
}

size_t PXModelMaterialAmount(PXModel* const model)
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
        DataStream materialData;
        unsigned int amount = 0;

        DataStreamFromExternal(&materialData, model->MaterialList, -1);
        DataStreamReadIU(&materialData, &amount, EndianLittle);

        return amount;
    }  
}

PXBool PXModelMaterialGet(PXModel* const model, const size_t materialID, PXMaterial* const pxMaterial)
{
    const size_t amount = PXModelMaterialAmount(model);

    {
        const PXBool isInRange = amount >= materialID;

        MemoryClear(pxMaterial, sizeof(PXMaterial));

        if (!isInRange)
        {
            return PXNo;
        }
    }

    DataStream materialData;

    DataStreamFromExternal(&materialData, model->MaterialList, -1);
    DataStreamCursorAdvance(&materialData, sizeof(unsigned int));

    for (size_t i = 0; i < amount; ++i)
    {
        const PXBool isGoal = i == materialID;
        unsigned short materialSize = 0;

        DataStreamReadSU(&materialData, &materialSize, EndianLittle); // Total size    

        if (isGoal)
        {
            unsigned short range = 0;
            DataStreamReadSU(&materialData, &range, EndianLittle);
            pxMaterial->Name = DataStreamCursorPosition(&materialData);
            pxMaterial->NameSize = range;

            DataStreamCursorAdvance(&materialData, range);

            DataStreamReadSU(&materialData, &range, EndianLittle);
            pxMaterial->DiffuseTextureFilePath = DataStreamCursorPosition(&materialData);
            pxMaterial->DiffuseTextureFilePathSize = range;

            DataStreamCursorAdvance(&materialData, range);

            DataStreamReadP(&materialData, &pxMaterial->Ambient, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Diffuse, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Specular, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Emission, sizeof(float) * 3u);

            break;
        }

        DataStreamCursorAdvance(&materialData, materialSize-2u);
    }

    return PXYes;
}

unsigned char ModelSegmentsAmount(const PXModel* const model)
{
    if (!model->Data) // Has data
    {
        return 0;
    }

	return *(PXAdress)model->Data;
}

void* ModelSegmentsAdressGet(const PXModel* const model, const size_t index)
{
    unsigned char* ancer = (unsigned char*)model->Data + 1 + (index * (sizeof(char) + sizeof(int) * 2u));

    return ancer;
}

void ModelSegmentsGet(const PXModel* const model, const size_t index, MeshSegment* const meshSegment)
{
    DataStream dataStream;

    {
        void* const segmentAdress = ModelSegmentsAdressGet(model, index);
        DataStreamFromExternal(&dataStream, segmentAdress, -1);
    }  

    DataStreamReadCU(&dataStream, &meshSegment->DrawStrideSize);
    DataStreamReadIU(&dataStream, &meshSegment->DrawClusterSize, EndianLittle);
    DataStreamReadIU(&dataStream, &meshSegment->TextureID, EndianLittle);

	meshSegment->VertexData = model->DataVertexList;
}

void ModelSegmentsAdd(PXModel* const model, const unsigned int renderMode, const unsigned int renderSize, const unsigned int renderMaterial)
{
    const unsigned char amount = ModelSegmentsAmount(model);
    const unsigned char segmentID = amount + 1u;

    *(PXAdress)model->Data = segmentID;

    DataStream dataStream;

    {
        void* const segmentAdress = ModelSegmentsAdressGet(model, segmentID-1u);
        DataStreamFromExternal(&dataStream, segmentAdress, -1);
    }

    DataStreamWriteCU(&dataStream, renderMode);
    DataStreamWriteIU(&dataStream, renderSize, EndianLittle);
    DataStreamWriteIU(&dataStream, renderMaterial, EndianLittle);

}

size_t ModelVertexDataStride(const PXModel* const model)
{
    return sizeof(float) * (model->DataVertexWidth + model->DataNormalWidth + model->DataTextureWidth + model->DataColorWidth);
}

ActionResult ModelLoadA(PXModel* const model, const char* const filePath)
{
	wchar_t filePathW[PathMaxSize];

	TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

	ActionResult actionResult = ModelLoadW(model, filePathW);

	return actionResult;
}

ActionResult ModelLoadW(PXModel* const model, const wchar_t* const filePath)
{
    DataStream dataStream;    

    DataStreamConstruct(&dataStream);
    ModelConstruct(model);

    {
        const ActionResult fileLoadingResult = DataStreamMapToMemoryW(&dataStream, filePath, 0, MemoryReadOnly);
        const unsigned char sucessful = fileLoadingResult == ActionSuccessful;

        if (!sucessful)
        {
            return fileLoadingResult;
        }
    }

    {
        const FileFormatExtension modelFileFormat = FilePathExtensionDetectTryW(filePath, PathMaxSize);
        const ActionResult fileParsingResult = ModelLoadD(model, &dataStream, modelFileFormat);
        const unsigned char success = fileParsingResult == ActionSuccessful;

        if (success)
        {
            return ActionSuccessful;
        }

        ActionResult fileGuessResult = ActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const FileFormatExtension imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = ModelLoadD(model, &dataStream, imageFileFormat);

            fileFormatID++;
        } while (fileGuessResult == ActionInvalidHeaderSignature);

        return fileGuessResult;
    }

    DataStreamDestruct(&dataStream);
}

ActionResult ModelLoadD(PXModel* const model, DataStream* const fileStream, const FileFormatExtension modelType)
{
    DataStream modelCompileCache;

    size_t bytesRead = 0;
    ModelCompilerFunction modelCompilerFunction = 0;
    ModelParserFunction modelParserFunction = 0;

    DataStreamMapToMemory(&modelCompileCache, fileStream->DataSize * 200, MemoryReadAndWrite);

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
            modelCompilerFunction = OBJFileCompile;
            modelParserFunction = OBJParseToModel;
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
            return ResultFormatNotSupported;
        }
    }

    const ActionResult actionResult = modelCompilerFunction(fileStream, &modelCompileCache);
    const PXBool sucessfull = ActionSuccessful == actionResult;

    if (sucessfull)
    {
        modelParserFunction(&modelCompileCache, model);
    }

    DataStreamDestruct(&modelCompileCache);

    return actionResult;
}