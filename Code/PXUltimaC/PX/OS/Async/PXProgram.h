#pragma once

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

typedef void (PXAPI* PXProgramExecutedEvent)(const PXBool succesful, const PXSize returnResult, const PXResult errorCode);

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

    PXThread* WorkingThread;
    PXSize ReturnValue;
    unsigned char ExecutionSuccessfull;
}
PXProgram;

PXPublic PXThreadResult PXOSAPI PXProgramExecuteThreadFunction(void* data);

PXPrivate PXResult PXAPI PXProgramExecute(PXProgram PXREF program);

// Execute function Asyncroinusly in another Thread.
PXPublic PXResult PXAPI PXProgramExecuteAS(PXProgram PXREF program, const char* programPath, const char* parameterString, PXProgramExecutedEvent callback);
PXPublic PXResult PXAPI PXProgramExecuteAL(PXProgram PXREF program, const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent callback);

PXPublic PXResult PXAPI PXProgramExecuteWS(PXProgram PXREF program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback);


PXPublic PXResult PXAPI PXProgramWaitForFinish(PXProgram PXREF program, PXI32U PXREF returnCode);

PXPublic ProcessHandle PXAPI PXProgramCurrentProcess();
PXPublic ProcessID PXAPI PXProgramCurrentProcessID();

PXPublic PXResult PXAPI PXProgramAttach(PXProgram PXREF pxProgram);
PXPublic PXResult PXAPI PXProgramDetach(PXProgram PXREF pxProgram);
PXPublic PXResult PXAPI PXProgramReadMemory(PXProgram PXREF pxProgram, const void PXREF adress, void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten);
PXPublic PXResult PXAPI PXProgramWriteMemory(PXProgram PXREF pxProgram, const void PXREF adress, const void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten);

#endif