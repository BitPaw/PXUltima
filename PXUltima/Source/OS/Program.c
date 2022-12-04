#include "Program.h"

#include <stdio.h>

#if OSUnix
#include <spawn.h>
#include <wait.h>
#define ExecuteProgram spawnv
#elif OSWindows
#include <process.h>
#define ExecuteProgram _spawnv
#endif

#include <Text/Text.h>
#include <Memory/PXMemory.h>

PXThreadResult ProgramExecuteThreadFunction(void* data)
{
    Program* program = (Program*)data;

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


#elif OSWindows
    program->ReturnValue = ExecuteProgram(_P_WAIT, program->FilePath, (const char* const*)program->ParameterList);
    program->ExecutionSuccessfull = program->ReturnValue == 0;
#endif

    if(program->ProgramExecutedCallBack)
    {
        program->ProgramExecutedCallBack(program->ExecutionSuccessfull, program->ReturnValue, 0);
    }

    // Free?

    return PXThreadSucessful;
}

ActionResult ProgramExecute(Program* const program)
{
    if (program->ProgramExecutedCallBack)
    {
        const ActionResult actionResult = PXThreadRun(&program->WorkingThread, ProgramExecuteThreadFunction, program);
        const PXBool sucessful = ActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }
    }
    else
    {
        ProgramExecuteThreadFunction(program);
    }

    return ActionSuccessful;
}

ActionResult ProgramExecuteAS(Program* program, const char* programPath, const char* parameterString, ProgramExecutedEvent* callback)
{
    TextCopyAW(programPath, 260, program->FilePath, 260);

    program->ProgramExecutedCallBack = callback;

    program->ParameterListSize = 1;
    program->ParameterList = (char**)MemoryAllocate(3 * sizeof(char*));

    program->ParameterList[0] = (char*)MemoryAllocate(260 * sizeof(char));
    program->ParameterList[1] = (char*)MemoryAllocate(1024 * sizeof(char));
    program->ParameterList[2] = 0;

    // Add ""
    {
        program->ParameterList[0] = '\"';
        PXSize copyedBytes = TextCopyA(&program->ParameterList[1], 1024, programPath, 1024);
        program->ParameterList[copyedBytes] = '\"';
    }

    TextCopyA(parameterString, 1024, program->ParameterList[1], 1024);

    const ActionResult actionResult = ProgramExecute(program);

    return actionResult;
}

ActionResult ProgramExecuteAL(Program* program, const char* programPath, const char** parameterList, PXSize parameterListSize, ProgramExecutedEvent* callback)
{
    TextCopyA(program->FilePath, 260, programPath, 260);

    program->ProgramExecutedCallBack = callback;

    program->ParameterListSize = parameterListSize + 1;
    program->ParameterList = (char**)MemoryAllocate(sizeof(char*) * program->ParameterListSize + 1);
    //memset(programExecuteInfo->ParameterList, '#', programExecuteInfo->ParameterListSize + 1);

    for(PXSize i = 0; i < parameterListSize; ++i)
    {
        char* newData = (char*)MemoryAllocate(sizeof(char) * 1024);

        program->ParameterList[i] = newData;
    }

    // Copy
    {
        PXSize i = 0;

        TextCopyA(programPath, 1024, program->ParameterList[i++], 1024);

        for( ; i < parameterListSize; ++i)
        {
            const char* source = parameterList[i];
            char* destination = program->ParameterList[i++];

            TextCopyA(source, 1024, destination, 1024);
        }

        program->ParameterList[i++] = 0;
    }

    const ActionResult actionResult = ProgramExecute(program);

    return actionResult;
}

ActionResult ProgramExecuteWS(Program* program, const wchar_t* programPath, const wchar_t* parameterList, ProgramExecutedEvent* callback)
{
    char programPathA[1024];
    char parameterListA[1024];

    //wcstombs(programPathA, programPath, 1024);
   // wcstombs(parameterListA, parameterList, 1024);

    //  return Program::Execute(programPathA, parameterListA, callback);

    return ResultFormatNotSupported;
}

ProcessHandle ProgramCurrentProcess()
{
#if OSUnix
    return 0;
#elif OSWindows
    return GetCurrentProcess();
#endif
}

ProcessID ProgramCurrentProcessID()
{
#if OSUnix
    return 0;
#elif OSWindows
    return GetCurrentProcessId();
#endif
}

void ProgramAttach(Program* program)
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

void ProgramDetach(Program* program)
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

void ProgramReadMemory(Program* program)
{
#if OSUnix
#elif OSWindows
    HANDLE hProcess = program->Handle;
    LPCVOID lpBaseAddress = 0;
    LPVOID* lpBuffer = 0;
    SIZE_T nSize = 0;
    SIZE_T* lpNumberOfBytesRead = 0;

    unsigned char successful = ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
#endif
}
