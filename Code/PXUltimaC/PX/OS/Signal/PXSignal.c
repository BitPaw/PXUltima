#include "PXSignal.h"

#include <PX/OS/File/PXFile.h>

#include <signal.h> // [POSIX] Exists on Linux and Windows


#if OSUnix
typedef __sighandler_t PXSignalHandle;
#elif OSWindows
typedef _crt_signal_t PXSignalHandle;

LONG WINAPI PXSignalFilter(EXCEPTION_POINTERS* exceptionInfo) 
{
    printf("Unhandled exception occurred!\n");
    return EXCEPTION_EXECUTE_HANDLER;
}

#endif

PXResult PXAPI PXSignalCallBackRegister(const PXSignalToken pxSignalToken, PXSignalCallBack pxSignalCallBack)
{
#if OSUnix || OSForcePOSIXForWindows
    int signalID;

    switch (pxSignalToken)
    {
    case PXSignalTokenAbort:
        signalID = SIGABRT;
        break;
    case PXSignalTokenPXF32MathError:
        signalID = SIGFPE;
        break;
    case PXSignalTokenIllegalInstruction:
        signalID = SIGILL;
        break;
#if OSWindows
    case PXSignalTokenINT:
        signalID = SIGBREAK;
        break;
#endif
    case PXSignalTokenMemoryViolation:
        signalID = SIGSEGV;
        break;
    case PXSignalTokenTerminationRequest:
        signalID = SIGTERM;
        break;

    default:
        return PXResultRefusedParameterInvalid;
    }




    const PXSignalHandle functionPointer = signal(signalID, pxSignalCallBack);
    const PXBool validLinkage = functionPointer != SIG_ERR;

    if (!validLinkage)
    {
        return PXResultRefusedParameterInvalid;
    }    

    return PXResultOK;
#elif OSWindows

    SetUnhandledExceptionFilter(PXSignalFilter);

    return PXActionRefusedNotImplemented;
#else
    return PXActionNotSupportedByOperatingSystem;
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
    return PXActionRefusedNotSupportedByLibrary;
#endif
}
