#include "PXProgram.h"

#include <stdio.h>

#if OSUnix
#include <spawn.h>
#include <wait.h>
#include <fcntl.h>
#elif OSWindows
#include <process.h>
#endif

#include <PX/Type/PXText.h>
#include <PX/OS/Memory/PXMemory.h>

const char PXExecuteName[] = "PXProgramExecute";
const PXI8U PXExecuteNameLengh = sizeof(PXExecuteName);

PXThreadResult PXOSAPI PXProgramExecuteThreadFunction(void* data)
{
    PXThreadResult pxThreadResult;
    PXProgram* program = (PXProgram*)data;

#if OSUnix
    char** envirument = 0;
    int creationResult = posix_spawn(&program->Handle.Data, program->FilePath, NULL, NULL, program->ParameterList, envirument);
    const unsigned char creationSucessful = creationResult == 0;

    if(creationSucessful)
    {
        program->Handle.Data = waitpid(program->Handle.Data, &program->ReturnValue, 0);
        const unsigned char deploySuessful = program->Handle.Data != -1;

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
        program->PXProgramExecutedCallBack(program->ExecutionSuccessfull, program->ReturnValue, PXResultInvalid);
    }

    // Free?
    pxThreadResult.Data = PXResultOK;

    return pxThreadResult;
}

PXResult PXAPI PXProgramExecute(PXProgram PXREF program)
{
    if (program->PXProgramExecutedCallBack)
    {
        PXThreadCreateInfo pxThreadCreateInfo;
        PXClear(PXThreadCreateInfo, &pxThreadCreateInfo);
        PXTextFromAddressA(&pxThreadCreateInfo.Info.Name, PXExecuteName, 0, PXExecuteNameLengh);
        pxThreadCreateInfo.ThreadFunction = PXProgramExecuteThreadFunction;
        pxThreadCreateInfo.Parameter = program;

        const PXResult actionResult = PXThreadCreate(&program->WorkingThread, &pxThreadCreateInfo);

        if (PXResultOK == actionResult)
        {
            return actionResult;
        }
    }
    else
    {
        PXProgramExecuteThreadFunction(program);
    }

    return PXResultOK;
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

    return PXResultRefusedNotImplemented;
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

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXProgramExecuteWS(PXProgram PXREF program, const wchar_t* programPath, const wchar_t* parameterList, PXProgramExecutedEvent* callback)
{
    char programPathA[1024];
    char parameterListA[1024];

    //wcstombs(programPathA, programPath, 1024);
    // wcstombs(parameterListA, parameterList, 1024);

    //  return PXProgram::Execute(programPathA, parameterListA, callback);

    return PXResultRefusedFormatNotSupported;
}

PXResult PXAPI PXProgramWaitForFinish(PXProgram PXREF program, PXI32U PXREF returnCode)
{
    if(!program)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    const pid_t processID = waitpid(program->Handle.Data, &program->ReturnValue, 0); // Linux, sys/wait.h
    const PXResult waitResult = PXErrorCurrent(-1 != processID);

    return waitResult;

#elif OSWindows

    DWORD result = -1;
    HANDLE handle = program->Handle.Data;

    const DWORD waitEventTrigger = WaitForSingleObject(handle, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

    switch(waitEventTrigger)
    {
        case WAIT_TIMEOUT:
        {
            return PXResultFailedConnectionTimedOut;
        }
        case WAIT_FAILED:
        {
            const PXResult pxResult = PXErrorCurrent(PXFalse);

            return pxResult;
        }
        case WAIT_ABANDONED:
        case WAIT_OBJECT_0:
        default:
        {  // Success, do nothing.
            break;
        }
    }

    const BOOL exitCodeGetSuccess = GetExitCodeProcess(handle, &result); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxResult = PXErrorCurrent(exitCodeGetSuccess);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *returnCode = result;

#endif

    return PXResultOK;
}

PXResult PXAPI PXProgramAttach(PXProgram PXREF pxProgram)
{
    PXResult pxResult = PXEndianInvalid;

    if(!pxProgram)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    char processFileName[64];

    sprintf(processFileName, "/proc/%d/mem", pxProgram->ID.Data);

    pxProgram->Handle.Data = open(processFileName, O_RDWR); // Linux, fcntl.h
    pxResult = PXErrorCurrent(pxProgram->Handle.Data != -1);

#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL bInheritHandle = 0;
    DWORD dwProcessID = 0;
    pxProgram->Handle.Data = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessID); // Windows XP (+UWP), Kernel32.dll
    pxResult = PXErrorCurrent(pxProgram->Handle.Data != PXNull);

#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

PXResult PXAPI PXProgramDetach(PXProgram PXREF pxProgram)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxProgram)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    const int resultID = close(pxProgram->Handle.Data);
    pxResult = PXErrorCurrent(0 == resultID);

#elif OSWindows
    const PXBool resultID = CloseHandle(pxProgram->Handle.Data);
    pxResult = PXErrorCurrent(resultID);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxProgram->Handle.Data = PXNull;

#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

PXResult PXAPI PXProgramReadMemory(PXProgram PXREF pxProgram, const void PXREF address, void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxProgram)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    lseek(pxProgram->Handle.Data, address, SEEK_SET);
    *bufferSizeWritten = read(pxProgram->Handle.Data, buffer, bufferSize);

    pxResult = PXResultOK;

#elif PXOSWindowsDestop
    SIZE_T readSize = 0;

    const PXBool readResult = ReadProcessMemory(pxProgram->Handle.Data, address, buffer, bufferSize, &readSize); // Windows XP, Kernel32.dll, memoryapi.h
    pxResult = PXErrorCurrent(readResult);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *bufferSizeWritten = readSize;
#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

PXResult PXAPI PXProgramWriteMemory(PXProgram PXREF pxProgram, const void PXREF address, const void PXREF buffer, const PXSize bufferSize, PXSize PXREF bufferSizeWritten)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxProgram)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    lseek(pxProgram->Handle.Data, address, SEEK_SET);
    *bufferSizeWritten = write(pxProgram->Handle.Data, buffer, bufferSize);

    pxResult = PXResultOK;

#elif PXOSWindowsDestop
    SIZE_T writtenSize = 0;

    const PXBool readResult = WriteProcessMemory(pxProgram->Handle.Data, (LPVOID)address, buffer, bufferSize, &writtenSize); // Windows XP, Kernel32.dll, memoryapi.h
    pxResult = PXErrorCurrent(readResult);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    *bufferSizeWritten = writtenSize;

#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}
