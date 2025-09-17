#ifndef PXProgramIncluded
#define PXProgramIncluded

#include <PX/OS/Async/PXThread.h>

#if OSUnix
#include <unistd.h>
typedef pid_t ProcessHandle;
typedef unsigned int ProcessID;
#elif OSWindows
#include <windows.h>
typedef HANDLE ProcessHandle;
typedef DWORD ProcessID;
#endif

typedef void (PXAPI* PXProgramExecutedEvent)(const PXBool succesful, const PXSize returnResult, const PXActionResult errorCode);

typedef struct PXProgram_
{
    ProcessHandle Handle;

#if OSUnix
    int MemoryFileHandle;
#endif

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

PXPrivate PXResult PXAPI  PXProgramExecute(PXProgram* const program);

// Execute function Asyncroinusly in another Thread.
PXPublic PXResult PXAPI PXProgramExecuteAS(PXProgram* const program, const char* programPath, const char* parameterString, PXProgramExecutedEvent callback);
PXPublic PXResult PXAPI PXProgramExecuteAL(PXProgram* const program, const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent callback);

PXPublic PXResult PXAPI PXProgramExecuteWS(PXProgram* const program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback);


PXPublic PXResult PXAPI PXProgramWaitForFinish(PXProgram* const program, PXI32U* const returnCode);

PXPublic ProcessHandle PXAPI PXProgramCurrentProcess();
PXPublic ProcessID PXAPI PXProgramCurrentProcessID();

PXPublic PXResult PXAPI PXProgramAttach(PXProgram* const pxProgram);
PXPublic PXResult PXAPI PXProgramDetach(PXProgram* const pxProgram);
PXPublic PXResult PXAPI PXProgramReadMemory(PXProgram* const pxProgram, const void* const adress, void* const buffer, const PXSize bufferSize, PXSize* const bufferSizeWritten);
PXPublic PXResult PXAPI PXProgramWriteMemory(PXProgram* const pxProgram, const void* const adress, const void* const buffer, const PXSize bufferSize, PXSize* const bufferSizeWritten);

#endif
