#include "PXProgram.h"

#include <stdio.h>

#if OSUnix
#include <spawn.h>
#include <wait.h>
#elif OSWindows
#include <process.h>
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
    program->ReturnValue = _spawnv(_P_WAIT, program->FilePath, (const char* const*)program->ParameterList);
    program->ExecutionSuccessfull = program->ReturnValue == 0;
#else
    return -1;
#endif

    if(program->PXProgramExecutedCallBack)
    {
        program->PXProgramExecutedCallBack(program->ExecutionSuccessfull, program->ReturnValue, PXActionInvalid);
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
    program->ParameterList = PXNewList(char*, 3);

    program->ParameterList[0] = PXNewList(char, 260);
    program->ParameterList[1] = PXNewList(char, 1024);
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
    return getpid(); // get process identification
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

PXActionResult PXProgramAttach(PXProgram* const pxProgram)
{
#if OSUnix
    char processFileName[64];

    sprintf(processFileName, "/proc/%d/mem", pid); 

    pxProgram->MemoryFileHandle = open(processFileName, O_RDWR); // Linux, fcntl.h
    const PXBool openSuccess = pxProgram->MemoryFileHandle != -1;

    PXActionReturnOnError(!openSuccess);
    
    return PXActionSuccessful;

#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL bInheritHandle = 0;
    DWORD dwProcessID = 0;
    pxProgram->Handle = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessID); // Windows XP (+UWP), Kernel32.dll
    const PXBool openSuccess = pxProgram->Handle != PXNull;

    PXActionReturnOnError(!openSuccess);

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProgramDetach(PXProgram* const pxProgram)
{
#if OSUnix

    const int result = close(pxProgram->MemoryFileHandle);

    return PXActionSuccessful;

#elif OSWindows
    HANDLE handleID = 0;

    const PXBool closeResult = CloseHandle(pxProgram->Handle);

    PXActionReturnOnError(!closeResult);

    pxProgram->Handle = PXNull;

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProgramReadMemory(PXProgram* const pxProgram, const void* const adress, void* const buffer, const PXSize bufferSize, PXSize* const bufferSizeWritten)
{
#if OSUnix

    lseek(pxProgram->MemoryFileHandle, adress, SEEK_SET);
    *bufferSizeWritten = read(pxProgram->MemoryFileHandle, buffer, bufferSize);

    return PXActionSuccessful;

#elif PXOSWindowsDestop
    const PXBool readResult = ReadProcessMemory(pxProgram->Handle, adress, buffer, bufferSize, bufferSizeWritten); // Windows XP, Kernel32.dll, memoryapi.h 

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXProgramWriteMemory(PXProgram* const pxProgram, const void* const adress, const void* const buffer, const PXSize bufferSize, PXSize* const bufferSizeWritten)
{
#if OSUnix

    lseek(pxProgram->MemoryFileHandle, adress, SEEK_SET);
    *bufferSizeWritten = write(pxProgram->MemoryFileHandle, buffer, bufferSize);

    return PXActionSuccessful;

#elif PXOSWindowsDestop
    const PXBool readResult = WriteProcessMemory(pxProgram->Handle, adress, buffer, bufferSize, bufferSizeWritten); // Windows XP, Kernel32.dll, memoryapi.h 

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}
