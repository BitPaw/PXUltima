#include "PXSignal.h"

#include <PX/OS/File/PXFile.h>

#if OSUnix
#define _GNU_SOURCE
#define __USE_GNU 1
#define __x86_64__ 1
#include <signal.h>     // [POSIX] Exists on Linux and Windows
#include <ucontext.h>
#include <sys/ucontext.h>
#define PXSignalUseSigaction 1
#elif OSWindows

#endif

PXSignalCallBackFunction _globalCallBack = 0;

#if OSUnix || OSForcePOSIXForWindows
const PXI8U PXSignalIDList[] =
{
    SIGINT,   // Control-C
    SIGILL,   // Illegal instruction
    SIGFPE,   // Floating point exception
    SIGSEGV,  // Access violation
    SIGTERM,  // Termination request
    SIGABRT,  // Abort() called
#if OSWindows
    SIGBREAK  // Windows break event
#endif
};
const PXResult PXSignalResultList[] =
{
    PXResultExitControlC,                 // SIGINT
    PXResultExceptionIllegalInstruction,  // SIGILL
    PXResultExceptionFloatArithmeticFault,// SIGFPE
    PXResultExceptionAccessViolation,     // SIGSEGV
    PXResultExceptionTerminateRequest,    // SIGTERM
    PXResultExceptionAbort,               // SIGABRT
#if OSWindows
    PXResultDebugEventBreakPoint          // SIGBREAK
#endif
};
const PXI8U PXSignalIDListAmount = sizeof(PXSignalIDList) / sizeof(PXI8U);
#endif

#if OSUnix
PXResult PXAPI PXSignalIDToResult(const int signalID)
{
    const PXI8U index = PXMemoryCompareI8V(PXSignalIDList, PXSignalIDListAmount, signalID);

    if(-1 == index)
    {
        return PXResultUnknown;
    }

    PXResult pxResult = PXSignalResultList[index];

    return pxResult;
}
void PXCDECL PXSignalCallBack_Sigaction(const int signalID, siginfo_t* info, void* ucontext)
{
    ucontext_t* ctx = (ucontext_t*)ucontext;

    PXSignalInfo pxSignalInfo;
    PXClear(PXSignalInfo, &pxSignalInfo);
    pxSignalInfo.ID = signalID;
    pxSignalInfo.Result = PXSignalIDToResult(signalID);
    pxSignalInfo.ProcessID.Data = info->si_pid;
    pxSignalInfo.ThreadID.Data = info->si_uid;
    pxSignalInfo.FaultAddress = info->si_addr;    // Memory address that caused fault

#if OS32B
    pxSignalInfo.Context.X86.EIP = ctx->uc_mcontext.gregs[REG_EIP];
    pxSignalInfo.Context.X86.ESP = ctx->uc_mcontext.gregs[REG_ESP];
    pxSignalInfo.Context.X86.EAX = ctx->uc_mcontext.gregs[REG_EAX];
    pxSignalInfo.Context.X86.EBX = ctx->uc_mcontext.gregs[REG_EBX];
    pxSignalInfo.Context.X86.ECX = ctx->uc_mcontext.gregs[REG_ECX];
    pxSignalInfo.Context.X86.EDX = ctx->uc_mcontext.gregs[REG_EDX];
    pxSignalInfo.Context.X86.ESI = ctx->uc_mcontext.gregs[REG_ESI];
    pxSignalInfo.Context.X86.EDI = ctx->uc_mcontext.gregs[REG_EDI];
    pxSignalInfo.Context.X86.EBP = ctx->uc_mcontext.gregs[REG_EBP];
#elif OS64B
    pxSignalInfo.Context.X64.RIP = ctx->uc_mcontext.gregs[REG_RIP];
    pxSignalInfo.Context.X64.RSP = ctx->uc_mcontext.gregs[REG_RSP];
    pxSignalInfo.Context.X64.RAX = ctx->uc_mcontext.gregs[REG_RAX];
    pxSignalInfo.Context.X64.RBX = ctx->uc_mcontext.gregs[REG_RBX];
    pxSignalInfo.Context.X64.RCX = ctx->uc_mcontext.gregs[REG_RCX];
    pxSignalInfo.Context.X64.RDX = ctx->uc_mcontext.gregs[REG_RDX];
    pxSignalInfo.Context.X64.RSI = ctx->uc_mcontext.gregs[REG_RSI];
    pxSignalInfo.Context.X64.RDI = ctx->uc_mcontext.gregs[REG_RDI];
    pxSignalInfo.Context.X64.RBP = ctx->uc_mcontext.gregs[REG_RBP];
    pxSignalInfo.Context.X64.R8 = ctx->uc_mcontext.gregs[REG_R8];
    pxSignalInfo.Context.X64.R9 = ctx->uc_mcontext.gregs[REG_R9];
    pxSignalInfo.Context.X64.R10 = ctx->uc_mcontext.gregs[REG_R10];
    pxSignalInfo.Context.X64.R11 = ctx->uc_mcontext.gregs[REG_R11];
    pxSignalInfo.Context.X64.R12 = ctx->uc_mcontext.gregs[REG_R12];
    pxSignalInfo.Context.X64.R13 = ctx->uc_mcontext.gregs[REG_R13];
    pxSignalInfo.Context.X64.R14 = ctx->uc_mcontext.gregs[REG_R14];
    pxSignalInfo.Context.X64.R15 = ctx->uc_mcontext.gregs[REG_R15];
#endif

    // Optional: exit or abort
    //exit(EXIT_FAILURE);
}

void PXCDECL PXSignalCallBack_Signal(const int signalID)
{
    PXSignalInfo pxSignalInfo;
    PXClear(PXSignalInfo, &pxSignalInfo);
    pxSignalInfo.ID = signalID;
    pxSignalInfo.Result = PXSignalIDToResult(signalID);


    // We need to recover the point where we need to go?
    PXThreadID threadID = PXThreadCurrentID();

    PXThreadContext pxThreadContext;
    PXResult pxResultContext = PXThreadContextGet(&pxThreadContext, threadID);

    PXAssert(PXResultOK == pxResultContext, "This cant ever fail");

    // Actual recover, secound Parameter will be the element returned by setjmp()
#if PXJumpOldUse
    longjmp(pxThreadContext.POSIXJumpBuffer, pxSignalInfo.Result);
#else
    siglongjmp(pxThreadContext.POSIXJumpBuffer, pxSignalInfo.Result);  // Jump back to recovery point
#endif

}
#elif OSWindows

PXResult PXAPI PXSignalErrorIDToResult(const DWORD errorID)
{
    switch(errorID)
    {
        case STILL_ACTIVE: return PXResultUnknown;
        case EXCEPTION_ACCESS_VIOLATION: return PXResultExceptionAccessViolation;
        case EXCEPTION_DATATYPE_MISALIGNMENT: return PXResultExceptionDataMisalignment;
        case EXCEPTION_BREAKPOINT: return PXResultDebugEventBreakPoint;
        case EXCEPTION_SINGLE_STEP: return PXResultDebugEventSingleStep;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return PXResultRefusedIndexOutOfBounce;
        case EXCEPTION_FLT_DENORMAL_OPERAND: return PXResultExceptionFloatDENORMAL_OPERAND;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO: return PXResultExceptionFloatDIVIDE_BY_ZERO;
        case EXCEPTION_FLT_INEXACT_RESULT: return PXResultExceptionFloatINEXACT_RESULT;
        case EXCEPTION_FLT_INVALID_OPERATION: return PXResultExceptionFloatINVALID_OPERATION;
        case EXCEPTION_FLT_OVERFLOW: return PXResultExceptionFloatOVERFLOW;
        case EXCEPTION_FLT_STACK_CHECK: return PXResultExceptionFloatSTACK_CHECK;
        case EXCEPTION_FLT_UNDERFLOW: return PXResultExceptionFloatUNDERFLOW;
        case EXCEPTION_INT_DIVIDE_BY_ZERO: return PXResultExceptionINTEGER_DIVIDE_BY_ZERO;
        case EXCEPTION_INT_OVERFLOW: return PXResultExceptionINTEGER_OVERFLOW;
        case EXCEPTION_PRIV_INSTRUCTION: return PXResultExceptionPrivilehedInstruction;
        case EXCEPTION_IN_PAGE_ERROR: return PXResultExceptionPageLost;
        case EXCEPTION_ILLEGAL_INSTRUCTION: return PXResultExceptionIllegalInstruction;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return PXResultExceptionNONCONTINUABLE_EXCEPTION;
        case EXCEPTION_STACK_OVERFLOW: return PXResultExceptionStackOverflow;
        case EXCEPTION_INVALID_DISPOSITION: return PXResultExceptionINVALID_DISPOSITION;
        case EXCEPTION_GUARD_PAGE: return PXResultExceptionGUARD_PAGE_VIOLATION;
        case EXCEPTION_INVALID_HANDLE: return PXResultRefusedHandleInvalid;
        //case EXCEPTION_POSSIBLE_DEADLOCK: return PXResultUnknown;
        case CONTROL_C_EXIT: return PXResultExitControlC;
        default:
            return PXResultUnknown;
    }
}

LONG __stdcall PXSignalCallBack_Exception(EXCEPTION_POINTERS* exceptionInfo)
{
    PXSignalInfo pxSignalInfo;
    PXClear(PXSignalInfo, &pxSignalInfo);

    pxSignalInfo.ID = exceptionInfo->ExceptionRecord->ExceptionCode;
    pxSignalInfo.Result = PXSignalErrorIDToResult(pxSignalInfo.ID);
    pxSignalInfo.FaultAddress = exceptionInfo->ExceptionRecord->ExceptionAddress;
    pxSignalInfo.ParameterAmount = exceptionInfo->ExceptionRecord->NumberParameters;

    //DWORD ExceptionFlags;
    //struct _EXCEPTION_RECORD* ExceptionRecord;
    //ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];

    PXThreadContextFromSystem(&pxSignalInfo.Context, exceptionInfo->ContextRecord);

    PXSignalDeploy(&pxSignalInfo);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void PXAPI PXSignalDeploy(PXSignalInfo PXREF pxSignalInfo)
{
    if(!pxSignalInfo)
    {
        // No object
        return;
    }

    if(!_globalCallBack)
    {
        // We dont have any callback!
        return;
    }

    _globalCallBack(pxSignalInfo);
}

void PXAPI PXSignalCallBackSet(PXSignalCallBackFunction pxSignalCallBackFunction)
{
    _globalCallBack = pxSignalCallBackFunction;

#if OSUnix || OSForcePOSIXForWindows

    for(PXI8U i = 0; i < PXSignalIDListAmount; ++i)
    {
        const int signalID = PXSignalResultList[i];

#if PXSignalUseSigaction && !OSForcePOSIXForWindows
        struct sigaction sa;
        sa.sa_sigaction = PXSignalCallBack_Sigaction;
        sa.sa_flags = SA_SIGINFO;

        sigemptyset(&sa.sa_mask);

        sigaction(signalID, &sa, NULL);
#else
        // [Note]  signal() is a very old legacy remainder.
        // It was not dropped but has no logic beeing used at all anymore, ethough it works.
        // sigaction() superceeds this function. It adds vastly more data.
        const PXSignalHandle functionPointer = signal(signalID, PXSignalCallBack_Signal);
#endif
    }

#elif OSWindows
    LPTOP_LEVEL_EXCEPTION_FILTER result = SetUnhandledExceptionFilter(PXSignalCallBack_Exception);
#endif
}

PXResult PXAPI PXRecoveryPointSet(PXRecoveryPoint PXREF pxRecoveryPoint)
{
#if PXDefaultLibraryEnable
    const int resultID = setjmp(pxRecoveryPoint->Enviroment);
    const PXBool success = 0 == resultID;

    return PXResultOK;
#endif
}

PXResult PXAPI PXRecoveryPointRestore(PXRecoveryPoint PXREF pxRecoveryPoint)
{
#if PXDefaultLibraryEnable
    int returnValue = 1;

    longjmp(pxRecoveryPoint->Enviroment, returnValue); // Jump to the saved state

    return PXResultOK;
#else
    return PXResultRefusedNotSupportedByLibrary;
#endif
}
