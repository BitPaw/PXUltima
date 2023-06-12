#include "PXProcess.hpp"

PX::Process::Process()
{
	PXProcessConstruct(this);
}

void PX::Process::Current()
{
	PXProcessCurrent(this);
}

void PX::Process::Parent()
{
	PXProcessParent(this);
}

PX::ActionResult PX::Process::Create(PX::Text& programmPath, const PXProcessCreationMode mode)
{
	return (PX::ActionResult)PXProcessCreate(this, &programmPath, mode);
}

PX::ActionResult PX::Process::Open()
{
	return PX::ActionResult();
}

PX::ActionResult PX::Process::Close()
{
	return (PX::ActionResult)PXProcessClose(this);
}

PXSize PX::Process::MemoryWrite(const void* const targetAdress, const void* const buffer, const PXSize bufferSize)
{
	return PXProcessMemoryWrite(this, targetAdress, buffer, bufferSize);
}

PXSize PX::Process::MemoryRead(void* const targetAdress, const void* const buffer, const PXSize bufferSize)
{
	return PXProcessMemoryRead(this, targetAdress, buffer, bufferSize);;
}

PX::ActionResult PX::Process::MemoryInfoFetch(PXProcessMemoryInfo& pxProcessMemoryInfo)
{
	return (PX::ActionResult)PXProcessMemoryInfoFetch(&pxProcessMemoryInfo);
}