#ifndef ExecutableInclude
#define ExecutableInclude

#include <OS/OSVersion.h>

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <Async/PXThread.h>

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

		PXThread WorkingThread;
		size_t ReturnValue;
		unsigned char ExecutionSuccessfull;
	}
	Program;

	PXPublic PXThreadResult  ProgramExecuteThreadFunction(void* data);

	PXPrivate ActionResult ProgramExecute(Program* const program);

	// Execute function Asyncroinusly in another Thread.
	PXPublic ActionResult ProgramExecuteAS(Program* program, const char* programPath, const char* parameterString, ProgramExecutedEvent* callback);
	PXPublic ActionResult ProgramExecuteAL(Program* program, const char* programPath, const char** parameterList, size_t parameterListSize, ProgramExecutedEvent* callback);

	PXPublic ActionResult ProgramExecuteWS(Program* program, const wchar_t* programPath, const wchar_t* parameterList, ProgramExecutedEvent* callback);


	PXPublic ProcessHandle ProgramCurrentProcess();
	PXPublic ProcessID ProgramCurrentProcessID();

	PXPublic void ProgramAttach(Program* program);
	PXPublic void ProgramDetach(Program* program);
	PXPublic void ProgramReadMemory(Program* program);

#ifdef __cplusplus
}
#endif

#endif
