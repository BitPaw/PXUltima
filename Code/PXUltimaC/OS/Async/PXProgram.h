#ifndef PXProgramINCLUDE
#define PXProgramINCLUDE

#include <OS/Async/PXThread.h>

#if OSUnix
#include <unistd.h>
typedef pid_t ProcessHandle;
typedef unsigned int ProcessID;
#elif OSWindows
#include <windows.h>
typedef HANDLE ProcessHandle;
typedef DWORD ProcessID;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef void (*PXProgramExecutedEvent)(const unsigned char succesful, PXSize returnResult, const PXActionResult errorCode);

	typedef struct PXProgram_
	{
		ProcessHandle Handle;

		wchar_t FilePath[260];

		PXProgramExecutedEvent PXProgramExecutedCallBack;

		PXSize ParameterListSize;
		char** ParameterList;

		PXThread WorkingThread;
		PXSize ReturnValue;
		unsigned char ExecutionSuccessfull;
	}
	PXProgram;

	PXPublic PXThreadResult PXOSAPI PXProgramExecuteThreadFunction(void* data);

	PXPrivate PXActionResult PXProgramExecute(PXProgram* const program);

	// Execute function Asyncroinusly in another Thread.
	PXPublic PXActionResult PXProgramExecuteAS(PXProgram* program, const char* programPath, const char* parameterString, PXProgramExecutedEvent callback);
	PXPublic PXActionResult PXProgramExecuteAL(PXProgram* program, const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent callback);

	PXPublic PXActionResult PXProgramExecuteWS(PXProgram* program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback);


	PXPublic ProcessHandle PXProgramCurrentProcess();
	PXPublic ProcessID PXProgramCurrentProcessID();

	PXPublic void PXProgramAttach(PXProgram* program);
	PXPublic void PXProgramDetach(PXProgram* program);
	PXPublic void PXProgramReadMemory(PXProgram* program);

#ifdef __cplusplus
}
#endif

#endif
