#include "PXProgram.h"

#include <stdio.h>

#if OSUnix
#include <spawn.h>
#include <wait.h>
#define ExecutePXProgram spawnv
#elif OSWindows
#include <process.h>
#define ExecutePXProgram _spawnv
#endif

#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>

PXThreadResult PXOSAPI PXProgramExecuteThreadFunction(void* data)
{
    PXProgram* program = (PXProgram*)data;

#if OSUnix
    char** envirument = 0;
    int creationResult = posix_spawn(&program->Handle, program->FilePath, NULL, NULL, program->ParameterList, envirument);
    const unsigned char creationSucessful = creationResult == 0;


    if(creationSucessful)
    {
        program->Handle = waitpid(program->Handle, &program->ReturnValue, 0);
        const unsigned char deploySuessful = program->Handle != -1;

        program->ExecutionSuccessfull = deploySuessful;// && programReturnresult == 0;
    }


#elif PXOSWindowsDestop
    program->ReturnValue = ExecutePXProgram(_P_WAIT, program->FilePath, (const char* const*)program->ParameterList);
    program->ExecutionSuccessfull = program->ReturnValue == 0;
#else
    return -1;
#endif

    if(program->PXProgramExecutedCallBack)
    {
        program->PXProgramExecutedCallBack(program->ExecutionSuccessfull, program->ReturnValue, 0);
    }

    // Free?

    return PXThreadSucessful;
}

PXActionResult PXProgramExecute(PXProgram* const program)
{
    if (program->PXProgramExecutedCallBack)
    {
        const PXActionResult actionResult = PXThreadRun(&program->WorkingThread, PXProgramExecuteThreadFunction, program);
        const PXBool sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }
    }
    else
    {
        PXProgramExecuteThreadFunction(program);
    }

    return PXActionSuccessful;
}

PXActionResult PXProgramExecuteAS(PXProgram* program, const char* programPath, const char* parameterString, PXProgramExecutedEvent callback)
{
    PXTextCopyAW(programPath, 260, program->FilePath, 260);

    program->PXProgramExecutedCallBack = callback;

    program->ParameterListSize = 1;
    program->ParameterList = (char**)PXMemoryAllocate(3 * sizeof(char*));

    program->ParameterList[0] = (char*)PXMemoryAllocate(260 * sizeof(char));
    program->ParameterList[1] = (char*)PXMemoryAllocate(1024 * sizeof(char));
    program->ParameterList[2] = 0;

    // Add ""
    {
        program->ParameterList[0] = '\"';
        PXSize copyedBytes = PXTextCopyA(&program->ParameterList[1], 1024, programPath, 1024);
        program->ParameterList[copyedBytes] = '\"';
    }

    PXTextCopyA(parameterString, 1024, program->ParameterList[1], 1024);

    const PXActionResult actionResult = PXProgramExecute(program);

    return actionResult;
}

PXActionResult PXProgramExecuteAL(PXProgram* program, const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent callback)
{
    PXTextCopyA(program->FilePath, 260, programPath, 260);

    program->PXProgramExecutedCallBack = callback;

    program->ParameterListSize = parameterListSize + 1;
    program->ParameterList = (char**)PXMemoryAllocate(sizeof(char*) * program->ParameterListSize + 1);
    //memset(programExecuteInfo->ParameterList, '#', programExecuteInfo->ParameterListSize + 1);

    for(PXSize i = 0; i < parameterListSize; ++i)
    {
        char* newData = (char*)PXMemoryAllocate(sizeof(char) * 1024);

        program->ParameterList[i] = newData;
    }

    // Copy
    {
        PXSize i = 0;

        PXTextCopyA(programPath, 1024, program->ParameterList[i++], 1024);

        for( ; i < parameterListSize; ++i)
        {
            const char* source = parameterList[i];
            char* destination = program->ParameterList[i++];

            PXTextCopyA(source, 1024, destination, 1024);
        }

        program->ParameterList[i++] = 0;
    }

    const PXActionResult actionResult = PXProgramExecute(program);

    return actionResult;
}

PXActionResult PXProgramExecuteWS(PXProgram* program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback)
{
    char programPathA[1024];
    char parameterListA[1024];

    //wcstombs(programPathA, programPath, 1024);
   // wcstombs(parameterListA, parameterList, 1024);

    //  return PXProgram::Execute(programPathA, parameterListA, callback);

    return PXActionRefusedFormatNotSupported;
}

ProcessHandle PXProgramCurrentProcess()
{
#if OSUnix
    return 0;
#elif OSWindows
    return GetCurrentProcess();
#endif
}

ProcessID PXProgramCurrentProcessID()
{
#if OSUnix
    return 0;
#elif OSWindows
    return GetCurrentProcessId();
#endif
}

void PXProgramAttach(PXProgram* program)
{
#if OSUnix
#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL bInheritHandle = 0;
    DWORD dwProcessID = 0;
    HANDLE handleID = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessID);

    program->Handle = handleID;
#endif
}

void PXProgramDetach(PXProgram* program)
{
#if OSUnix
#elif OSWindows
    HANDLE handleID = 0;

    const unsigned char closeResult = CloseHandle(program->Handle);

    if(closeResult)
    {
        program->Handle = 0;
    }

#endif
}

void PXProgramReadMemory(PXProgram* program)
{
#if OSUnix
#elif PXOSWindowsDestop
    HANDLE hProcess = program->Handle;
    LPCVOID lpBaseAddress = 0;
    LPVOID* lpBuffer = 0;
    SIZE_T nSize = 0;
    SIZE_T* lpNumberOfBytesRead = 0;

    unsigned char successful = ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead); // Windows XP, Kernel32.dll, memoryapi.h 
#endif
}
