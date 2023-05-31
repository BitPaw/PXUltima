#include "PXDebug.hpp"

PX::Debug::Debug()
{
	PXDebugConstruct(this);
}

PX::Debug::~Debug()
{
	PXDebugDestruct(this);
}

PX::ActionResult PX::Debug::ProcessBeeingDebugged(bool& isPresent)
{
	PXBool pxBool = 0;

	PX::ActionResult result = (PX::ActionResult)PXDebugProcessBeeingDebugged(this, &pxBool);

	pxBool = pxBool;

	return result;
}

bool PX::Debug::ProcessBeeingDebuggedCurrent()
{
	return PXDebugProcessBeeingDebuggedCurrent();
}

void PX::Debug::DebuggerSendMessage(PX::Text& const message)
{
	PXDebugDebuggerSendMessage(this, &message);
}

bool PX::Debug::DebuggerInitialize()
{
	return DebuggerInitialize();
}

PX::ActionResult PX::Debug::StartProcess(const PX::Text& const applicationName)
{
	return PX::ActionResult();
}

void PX::Debug::Continue()
{
	PXDebugContinue(this);
}

void PX::Debug::Pause()
{
	PXDebugPause(this);
}

bool PX::Debug::PauseOther(const PXProcessID pxProcessID)
{
	return 	PXDebugPauseOther(this, pxProcessID);
}

void PX::Debug::Attach()
{
	PXDebugAttach(this);
}

void PX::Debug::Detach()
{
	PXDebugDetach(this);
}

void PX::Debug::StackTrace()
{
	PXDebugStackTrace(this);
}

PXSize PX::Debug::MemoryRead(const void* const adress, void* const outputData, const PXSize outputDataSize)
{
	return PXDebugMemoryRead(this, adress, outputData, outputDataSize);
}

PXSize PX::Debug::MemoryWrite(const void* const adress, const void* const inputData, const PXSize inputDataSize)
{
	return PXDebugMemoryWrite(this, adress, inputData, inputDataSize);
}

PX::ActionResult PX::Debug::WaitForEvent()
{
	return (PX::ActionResult)PXDebugWaitForEvent(this);
}