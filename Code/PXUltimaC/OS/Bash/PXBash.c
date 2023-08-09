#include "PXBash.h"

#if PXBashUSE

PXActionResult PXBashExecute(const PXText* const programPath, const PXBool syncronous)
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

	return PXActionRefusedNotImplemented;
}
#endif