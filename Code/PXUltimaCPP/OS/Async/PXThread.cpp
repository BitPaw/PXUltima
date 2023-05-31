#include "PXThread.hpp"

PX::ActionResult PX::Thread::Run(const ThreadFunction threadFunction, const void* parameter)
{
	return (PX::ActionResult)PXThreadRun(this, threadFunction, parameter);
}

void PX::Thread::Suspend()
{
	PXThreadSuspend(this);
}

void PX::Thread::Resume()
{
	PXThreadResume(this);
}

void PX::Thread::Sleep(const PXSize sleepTime)
{
	PXThreadSleep(this, sleepTime);
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