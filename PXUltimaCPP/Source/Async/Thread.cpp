#include "Thread.hpp"

ActionResult PX::Thread::Run(const ThreadFunction threadFunction, const void* parameter)
{
	return PXThreadRun(this, threadFunction, parameter);
}

PXSize PX::Thread::CurrentID()
{
	return PXThreadCurrentID();
}

void PX::Thread::CurrentGet()
{
	PXThreadCurrentGet(this);
}

void PX::Thread::WaitForFinish()
{
	PXThreadWaitForFinish(this);
}