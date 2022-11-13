#include "Model.h"

#include <Memory/PXMemory.h>
#include <File/File.h>
#include <Text/Text.h>
#include <File/DataStream.h>

#include <Format/OBJ/OBJ.h>

void ModelConstruct(Model* const model)
{
	MemorySet(model, sizeof(Model), 0);
}

void ModelDestruct(Model* const model)
{
	
}

size_t PXModelMaterialAmount(Model* const model)
{
    DataStream materialData;

    DataStreamFromExternal(&materialData, model->MaterialList, -1);

    unsigned int amount = 0;

    DataStreamReadIU(&materialData, &amount, EndianLittle);

    return amount;    
}

PXBool PXModelMaterialGet(Model* const model, const size_t materialID, PXMaterial* const pxMaterial)
{
    const size_t amount = PXModelMaterialAmount(model);
    const PXBool isInRange = amount <= materialID;

    MemorySet(pxMaterial, sizeof(PXMaterial), 0);

    if (!isInRange)
    {
        return PXNo;
    }

    DataStream materialData;

    DataStreamFromExternal(&materialData, model->MaterialList, -1);
    DataStreamCursorAdvance(&materialData, sizeof(unsigned int));

    for (size_t i = 0; i < amount; ++i)
    {
        unsigned short materialSize = 0;

        DataStreamReadSU(&materialData, &materialSize, EndianLittle); // Total size

        PXBool isGoal = i == materialID;

        if (isGoal)
        {
            DataStreamReadSU(&materialData, &pxMaterial->NameSize, EndianLittle);
            pxMaterial->Name = DataStreamCursorPosition(&materialData);

            DataStreamReadP(&materialData, &pxMaterial->Ambient, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Diffuse, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Specular, sizeof(float) * 3u);
            DataStreamReadP(&materialData, &pxMaterial->Emission, sizeof(float) * 3u);

            break;
        }

        DataStreamCursorAdvance(&materialData, materialSize);
    }

    return PXYes;
}

unsigned char ModelSegmentsAmount(const Model* const model)
{
	return *(unsigned char*)model->Data;
}

void ModelSegmentsGet(const Model* const model, const size_t index, MeshSegment* const meshSegment)
{
	unsigned char* ancer= (unsigned char*)model->Data +1 + (index * (sizeof(char) + sizeof(int)));

	meshSegment->DrawStrideSize = ancer;
	meshSegment->DrawClusterSize = ancer + sizeof(char);

	meshSegment->VertexData = model->DataVertex;
}

ActionResult ModelLoadA(Model* const model, const char* const filePath)
{
	wchar_t filePathW[PathMaxSize];

	TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

	ActionResult actionResult = ModelLoadW(model, filePathW);

	return actionResult;
}

ActionResult ModelLoadW(Model* const model, const wchar_t* const filePath)
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

ActionResult ModelLoadD(Model* const model, DataStream* const fileStream, const FileFormatExtension modelType)
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