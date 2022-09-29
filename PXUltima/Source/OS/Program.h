#ifndef ExecutableInclude
#define ExecutableInclude

#include <OS/OSVersion.h>

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <Async/Thread.h>

#if OSUnix
#include <unistd.h>
#define ProcessHandle pid_t
#define ProcessID unsigned int
#elif OSWindows
#include <windows.h>
#define ProcessHandle HANDLE
#define ProcessID DWORD
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*ProgramExecutedEvent)(const unsigned char succesful, size_t returnResult, const ActionResult errorCode);

	typedef struct Program_
	{
		ProcessHandle Handle;

		wchar_t FilePath[260];

		ProgramExecutedEvent ProgramExecutedCallBack;

		size_t ParameterListSize;
		char** ParameterList;

		ThreadID WorkingThread;
		size_t ReturnValue;
		unsigned char ExecutionSuccessfull;
	}
	Program;

	CPublic ThreadResult  ProgramExecuteThreadFunction(void* data);

	// Execute function Asyncroinusly in another Thread.
	CPublic ActionResult ProgramExecuteAS(Program* program, const char* programPath, const char* parameterString, ProgramExecutedEvent* callback);
	CPublic ActionResult ProgramExecuteAL(Program* program, const char* programPath, const char** parameterList, size_t parameterListSize, ProgramExecutedEvent* callback);

	CPublic ActionResult ProgramExecuteWS(Program* program, const wchar_t* programPath, const wchar_t* parameterList, ProgramExecutedEvent* callback);


	CPublic ProcessHandle ProgramCurrentProcess();
	CPublic ProcessID ProgramCurrentProcessID();

	CPublic void ProgramAttach(Program* program);
	CPublic void ProgramDetach(Program* program);
	CPublic void ProgramReadMemory(Program* program);

#ifdef __cplusplus
}
#endif

#endif
