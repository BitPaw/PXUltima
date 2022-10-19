#include "Model.h"

#include <Memory/Memory.h>
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

ModelFileFormat ModelGuessFormat(const wchar_t* const filePath)
{
	wchar_t extension[ExtensionMaxSize];

	FilePathExtensionGetW(filePath, PathMaxSize, extension, ExtensionMaxSize);

	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "3ds", 3u))  return ModelFileFormatA3DS;
	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "FBX", 3u))  return ModelFileFormatFBX;
	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "OBJ", 3u))  return ModelFileFormatOBJ;
	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "PLY", 3u))  return ModelFileFormatPLY;
	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "STL", 3u))  return ModelFileFormatSTL;
	if (TextCompareIgnoreCaseWA(extension, ExtensionMaxSize, "WRL", 3u))  return ModelFileFormatWRL;

	return ModelFileFormatUnKown;
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
        const ModelFileFormat modelFileFormat = ModelGuessFormat(filePath);
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
            const ModelFileFormat imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = ModelLoadD(model, &dataStream, imageFileFormat);

            fileFormatID++;
        } while (fileGuessResult == ActionInvalidHeaderSignature);

        return fileGuessResult;
    }

    DataStreamDestruct(&dataStream);
}

ActionResult ModelLoadD(Model* const model, DataStream* const fileStream, const ModelFileFormat modelType)
{
    DataStream modelCompileCache;

    size_t bytesRead = 0;
    ModelCompilerFunction modelCompilerFunction = 0;
    ModelParserFunction modelParserFunction = 0;

    DataStreamMapToMemory(&modelCompileCache, fileStream->DataSize * 10, MemoryReadAndWrite);

    switch (modelType)
    {
        case ModelFileFormatA3DS:
        {
           
            break;
        }
        case ModelFileFormatFBX:
        {
           
            break;
        }
        case ModelFileFormatOBJ:
        {
            modelCompilerFunction = OBJFileCompile;
            modelParserFunction = OBJParseToModel;
            break;
        }
        case ModelFileFormatPLY:
        {
            
            break;
        }
        case ModelFileFormatSTL:
        {
            
            break;
        }
        case ModelFileFormatWRL:
        {
         
            break;
        }
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