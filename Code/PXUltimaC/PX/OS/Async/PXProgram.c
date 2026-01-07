#include "PXProgram.h"

#include <stdio.h>

#if OSUnix
#include <spawn.h>
#include <wait.h>
#include <fcntl.h>
#elif OSWindows
#include <process.h>
#endif

#include <PX/Media/PXText.h>
#include <PX/OS/Memory/PXMemory.h>

const char PXExecuteName[] = "PXProgramExecute";
const PXI8U PXExecuteNameLengh = sizeof(PXExecuteName);

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

#if 1 // normal

    PROCESS_INFORMATION processInformation;
    PXClear(PROCESS_INFORMATION, &processInformation);

    STARTUPINFOA startupInfo;
    PXClear(STARTUPINFOA, &startupInfo);
    startupInfo.cb = sizeof(STARTUPINFOA);

    const BOOL createSuccessful = CreateProcessA
    (
        program->FilePath, // lpApplicationName,
        PXNull,            // lpCommandLine,
        PXNull,            // LPSECURITY_ATTRIBUTES lpProcessAttributes,
        PXNull,            // LPSECURITY_ATTRIBUTES lpThreadAttributes,
        PXFalse,           // bInheritHandles,
        0,                  // dwCreationFlags,
        PXNull,            //     LPVOID                lpEnvironment,
        PXNull,            //     LPCSTR                lpCurrentDirectory,
        &startupInfo,
        &processInformation
    );


    //  pxProgram.Handle = processInformation.han

    // PXProgramWaitForFinish();

#else
    program->ReturnValue = _spawnv(_P_WAIT, program->FilePath, (const char PXREF*)program->ParameterList);
    program->ExecutionSuccessfull = program->ReturnValue == 0;
#endif


#else
    return -1;
#endif

    if(program->PXProgramExecutedCallBack)
    {
        program->PXProgramExecutedCallBack(program->ExecutionSuccessfull, program->ReturnValue, PXActionInvalid);
    }

    // Free?

    return PXActionSuccessful;
}

PXResult PXAPI PXProgramExecute(PXProgram PXREF program)
{
    if (program->PXProgramExecutedCallBack)
    {
        PXThreadCreateInfo pxThreadCreateInfo;
        PXClear(PXThreadCreateInfo, &pxThreadCreateInfo);
        PXTextFromAdressA(&pxThreadCreateInfo.Info.Name, PXExecuteName, 0, PXExecuteNameLengh);
        pxThreadCreateInfo.ThreadFunction = PXProgramExecuteThreadFunction;
        pxThreadCreateInfo.Parameter = program;
        pxThreadCreateInfo.Behaviour = PXThreadBehaviourDefault;

        const PXResult actionResult = PXThreadCreate(&program->WorkingThread, &pxThreadCreateInfo);

        if (PXActionSuccessful == actionResult)
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

PXResult PXAPI PXProgramExecuteAS(PXProgram PXREF program, const char* programPath, const char* parameterString, PXProgramExecutedEvent callback)
{
#if 0
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

    const PXResult actionResult = PXProgramExecute(program);

    return actionResult;
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXProgramExecuteAL(PXProgram PXREF program, const char* programPath, const char** parameterList, PXSize parameterListSize, PXProgramExecutedEvent callback)
{
#if 0

    PXTextCopyA(program->FilePath, 260, programPath, 260);

    program->PXProgramExecutedCallBack = callback;

    program->ParameterListSize = parameterListSize + 1;
    program->ParameterList = PXNewList(char*,program->ParameterListSize + 1);
    //memset(programExecuteInfo->ParameterList, '#', programExecuteInfo->ParameterListSize + 1);

    for(PXSize i = 0; i < parameterListSize; ++i)
    {
        char* newData = PXNewList(PXByte, 1024);

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

    const PXResult actionResult = PXProgramExecute(program);

    return actionResult;
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXProgramExecuteWS(PXProgram PXREF program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback)
{
    char programPathA[1024];
    char parameterListA[1024];

    //wcstombs(programPathA, programPath, 1024);
    // wcstombs(parameterListA, parameterList, 1024);

    //  return PXProgram::Execute(programPathA, parameterListA, callback);

    return PXActionRefusedFormatNotSupported;
}

PXResult PXAPI PXProgramWaitForFinish(PXProgram PXREF program, PXI32U PXREF returnCode)
{
#if OSUnix

    const pid_t processID = waitpid(program->Handle, &program->ReturnValue, 0); // Linux, sys/wait.h
    const PXResult waitResult = PXErrorCurrent(-1 != processID);

    return waitResult;

#elif OSWindows

    DWORD result = -1;

    const DWORD waitEventTrigger = WaitForSingleObject(program->Handle, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

    switch (waitEventTrigger)
    {
    case WAIT_TIMEOUT:
    {
        return PXActionFailedConnectionTimedOut;
    }
    case WAIT_FAILED:
    {
        const PXResult pxActionResult = PXErrorCurrent(PXFalse);

        return pxActionResult;
    }
    case WAIT_ABANDONED:
    case WAIT_OBJECT_0:
    default:
        // Success, do nothing.
        break;
    }

    const BOOL exitCodeGetSuccess = GetExitCodeProcess(program->Handle, &result); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxActionResult = PXErrorCurrent(exitCodeGetSuccess);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    *returnCode = result;

#endif

    return PXActionSuccessful;
}

ProcessHandle PXAPI PXProgramCurrentProcess()
{
#if OSUnix
    return getpid(); // get process identification
#elif OSWindows
    return GetCurrentProcess();
#endif
}

ProcessID PXAPI PXProgramCurrentProcessID()
{
#if OSUnix
    return 0;
#elif OSWindows
    return GetCurrentProcessId();
#endif
}

PXResult PXAPI PXProgramAttach(PXProgram PXREF pxProgram)
{
#if OSUnix
    char processFileName[64];

    sprintf(processFileName, "/proc/%d/mem", pxProgram->Handle);

    pxProgram->MemoryFileHandle = open(processFileName, O_RDWR); // Linux, fcntl.h
    const PXBool openSuccess = pxProgram->MemoryFileHandle != -1;

    PXActionReturnOnError(!openSuccess);

    return PXActionSuccessful;

#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL bInheritHandle = 0;
    DWORD dwProcessID = 0;
    pxProgram->Handle = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessID); // Windows XP (+UWP), Kernel32.dll
    const PXResult pxActionResult = PXErrorCurrent(pxProgram->Handle != PXNull);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXProgramDetach(PXProgram PXREF pxProgram)
{
#if OSUnix

    const int result = close(pxProgram->MemoryFileHandle);

    return PXActionSuccessful;

#elif OSWindows
    HANDLE handleID = 0;

    const PXBool closeResult = CloseHandle(pxProgram->Handle);
    const PXResult pxActionResult = PXErrorCurrent(closeResult);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    pxProgram->Handle = PXNull;

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXProgramReadMemory(PXProgram PXREF pxProgram, const void PXREF adress, void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten)
{
#if OSUnix

    lseek(pxProgram->MemoryFileHandle, adress, SEEK_SET);
    *bufferSizeWritten = read(pxProgram->MemoryFileHandle, buffer, bufferSize);

    return PXActionSuccessful;

#elif PXOSWindowsDestop
    SIZE_T readSize = 0;

    const PXBool readResult = ReadProcessMemory(pxProgram->Handle, adress, buffer, bufferSize, &readSize); // Windows XP, Kernel32.dll, memoryapi.h
    const PXResult pxActionResult = PXErrorCurrent(readResult);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    *bufferSizeWritten = readSize;

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXProgramWriteMemory(PXProgram PXREF pxProgram, const void PXREF adress, const void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten)
{
#if OSUnix

    lseek(pxProgram->MemoryFileHandle, adress, SEEK_SET);
    *bufferSizeWritten = write(pxProgram->MemoryFileHandle, buffer, bufferSize);

    return PXActionSuccessful;

#elif PXOSWindowsDestop
    SIZE_T writtenSize = 0;

    const PXBool readResult = WriteProcessMemory(pxProgram->Handle, (LPVOID)adress, buffer, bufferSize, &writtenSize); // Windows XP, Kernel32.dll, memoryapi.h
    const PXResult pxActionResult = PXErrorCurrent(readResult);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    *bufferSizeWritten = writtenSize;

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}
