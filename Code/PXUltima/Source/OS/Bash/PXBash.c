#include "PXBash.h"

PXActionResult BashExecuteA(const char* programPath, const unsigned char syncronous)
{
	/*
	Pipe pipe;
	PXActionResult result = pipe.Open(programPath, FileOpenMode::Read);
	PXSize dataLength = 0;
	unsigned char* data = 0;

	if(result != PXActionSuccessful)
	{
		return result;
	}

	result = File::ReadFromDisk(pipe.FileMarker, &data, dataLength, true);

	result = pipe.Close();

	printf("[PEx] %s\n", data);

	free(data);*/

	return PXActionSuccessful;
}

PXActionResult BashExecuteW(const wchar_t* programPath, const unsigned char syncronous)
{
	return PXActionSuccessful;
}
