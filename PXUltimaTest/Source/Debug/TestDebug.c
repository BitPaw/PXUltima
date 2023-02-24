#include "TestDebug.h"

#include <OS/Debug/PXDebug.h>

void PXTestDebugAll()
{
	PXTestDebugProcess();
}

void PXTestDebugProcess()
{
	PXDebug pxDebug;

	PXDebugStartProcessA(&pxDebug, "A:/F/F.exe");

	while (1)
	{

	}
}