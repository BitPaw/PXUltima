#include "PXSignal.h"

#include <OS/File/PXFile.h>

#include <signal.h> // [POSIX] Exists on Linux and Windows

PXActionResult PXAPI PXSignalCallBackRegister(const PXSignalToken pxSignalToken, PXSignalCallBack pxSignalCallBack)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
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
		case PXSignalTokenINT:
			signalID = SIGBREAK;
			break;
		case PXSignalTokenMemoryViolation:
			signalID = SIGSEGV;
			break;
		case PXSignalTokenTerminationRequest:
			signalID = SIGTERM;
			break;

		default:
			return PXFalse;
	}


	const _crt_signal_t functionPointer = signal(signalID, pxSignalCallBack);
	const PXBool validLinkage = functionPointer != SIG_ERR;

	if (!validLinkage)
	{
		return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}