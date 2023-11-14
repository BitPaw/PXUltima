#include "PXProgram.hpp"

PX::ActionResult PX::Program::ExecuteAS(const char* programPath, const char* parameterString, PXProgramExecutedEvent* callback)
{
    return PX::ActionResult::RefusedNotImplemented;
}

PX::ActionResult PX::Program::ExecuteAL(const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent* callback)
{
    return PX::ActionResult::RefusedNotImplemented;
}

PX::ActionResult PX::Program::ExecuteWS(const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback)
{
    return PX::ActionResult::RefusedNotImplemented;
}

ProcessHandle PX::Program::CurrentProcess()
{
    return PXNull;
}

ProcessID PX::Program::CurrentProcessID()
{
    return PXNull;
}

void PX::Program::Attach()
{
}

void PX::Program::Detach()
{
}

void PX::Program::MemoryRead()
{
}