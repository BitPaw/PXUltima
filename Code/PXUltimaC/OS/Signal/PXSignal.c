#include "PXSignal.h"

#include <OS/File/PXFile.h>

#include <signal.h> // [POSIX] Exists on Linux and Windows

#if OSUnix
typedef __sighandler_t PXSignalHandle;
#elif OSWindows
typedef _crt_signal_t PXSignalHandle;
#endif

PXActionResult PXAPI PXSignalCallBackRegister(const PXSignalToken pxSignalToken, PXSignalCallBack pxSignalCallBack)
{
#if OSUnix || OSForcePOSIXForWindows
    int signalID;

    switch (pxSignalToken)
    {
        case PXSignalTokenAbort:
            signalID = SIGABRT;
            break;
        case PXSignalTokenFloatMathError:
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
            return PXActionRefusedArgumentInvalid;
    }




    const PXSignalHandle functionPointer = signal(signalID, pxSignalCallBack);
    const PXBool validLinkage = functionPointer != SIG_ERR;

    if (!validLinkage)
    {
        return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}