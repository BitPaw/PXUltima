#include "PXActionResult.h"

#include <OS/System/PXOSVersion.h>

#include <errno.h> // POSIX

#if OSUnix
#define EOTHER -1
#elif OSWindows
#include <Windows.h>
#include <mmeapi.h>
#include <dsound.h> // Only used for error codes
#endif

PXActionResult PXErrorCodeFromID(const int errorCode)
{
	switch(errorCode)
	{
		case EOTHER:
			return PXActionFailedUnkownError;

		case EACCES:
			return PXActionDeniedNoPermission;

		case EADDRINUSE:
			return AddressInUse;

		case EADDRNOTAVAIL:
			return AddressNotAvailable;

		case EAFNOSUPPORT:
			return AddressFamilyNotSupported;

		case EALREADY:
			return ConnectionAlreadyInProgress;

		case E2BIG:
			return ArgumentListTooLong;

		case EBADF:
			return BadFileDescriptor;

		case EBADMSG:
			return BadMessage;

		case EBUSY:
			return PXActionRefusedResourceBusy;

		case ECANCELED:
			return OperationCanceled;

		case ECHILD:
			return NoChildProcesses;

		case ECONNABORTED:
			return ConnectionAborted;

		case ECONNREFUSED:
			return ConnectionRefused;

		case ECONNRESET:
			return ConnectionReset;

		case EDEADLK:
			return ResourceDeadlockWouldOccur;

		case EDESTADDRREQ:
			return DestinationAddressRequired;

		case EDOM:
			return PXMathematicsArgumentOutOfDomainOfFunction;

		case EEXIST:
			return PXActionRefusedFileAlreadyExists;

		case EFAULT:
			return BadAddress;

		case EFBIG:
			return PXActionFailedFileTooLarge;

		case EHOSTUNREACH:
			return HostIsUnreachable;

		case EIDRM:
			return IdentifierRemoved;

		case EILSEQ:
			return IllegalByteSequence;

		case EINPROGRESS:
			return OperationInProgress;

		case EINTR:
			return InterruptedFunction;

		case EINVAL:
			return PXActionRefusedArgumentInvalid;

		case EIO:
			return IOFailure;

		case EISCONN:
			return PXActionRefusedAlreadyConnected;

		case EISDIR:
			return PXActionRefusedTargetIsDirectory;

		case ELOOP:
			return PXActionCanceledLoopDeteced;

		case EMFILE:
			return PXActionFailedFileDescriptorValueTooLarge;

		case EMLINK:
			return PXActionFailedTooManyLinks;

		case EMSGSIZE:
			return MessageTooLarge;

		case ENAMETOOLONG:
			return PXActionFailedFileNameTooLong;

		case ENETDOWN:
			return PXActionRefusedNetworkIsDown;

		case ENETRESET:
			return PXActionFailedConnectionAbortedByNetwork;

		case ENETUNREACH:
			return PXActionFailedNetworkUnreachable;

		case ENFILE:
			return PXActionFailedTooManyFilesOpenInSystem;

		case ENOBUFS:
			return NoBufferSpaceAvailable;

		case ENODATA:
			return NoMessageIsAvailableOnTheStreamHeadReadQueue;

		case ENODEV:
			return NoSuchDevice;

		case ENOENT:
			return NoSuchFileOrDirectory;

		case ENOEXEC:
			return ExecutableFileFormatError;

		case ENOLCK:
			return NoLocksAvailable;

		case ENOLINK:
			return LinkHasBeenSevered;

		case ENOMEM:
			return PXActionFailedMemoryAllocation;

		case ENOMSG:
			return NoMessageOfTheDesiredType;

		case ENOPROTOOPT:
			return ProtocolNotAvailable;

		case ENOSPC:
			return NoSpaceLeftOnDevice;

		case ENOSR:
			return NoStreamResources;

		case ENOSTR:
			return NotAStream;

		case ENOSYS:
			return FunctionNotSupported;

		case ENOTCONN:
			return PXActionFailedNotConnected;

		case ENOTDIR:
			return PXActionRefusedNotADirectory;

		case ENOTEMPTY:
			return PXActionRefusedDirectoryNotEmpty;

		case ENOTRECOVERABLE:
			return StateNotRecoverable;

		case ENOTSOCK:
			return PXActionRefusedNotASocket;

		case ENOTTY:
			return InappropriateIOControlOperation;

		case ENXIO:
			return NoSuchDeviceOrAddress;

		case EWOULDBLOCK:
			return OperationWouldBlock;

		case ENOTSUP:
			return PXActionNotSupportedByOperatingSystem;

		case EAGAIN:
			return ResourceUnavailableTryAgain;

		case EOPNOTSUPP:
			return OperationNotSupportedOnSocket;

		case EOVERFLOW:
			return ValueTooLargeToBeStoredInDataType;

		case EOWNERDEAD:
			return PreviousOwnerDied;

		case EPERM:
			return OperationNotPermitted;

		case EPIPE:
			return BrokenPipe;

		case EPROTO:
			return ProtocolError;

		case EPROTONOSUPPORT:
			return ProtocolNotSupported;

		case EPROTOTYPE:
			return ProtocolWrongTypeForSocket;

		case ERANGE:
			return ResultTooLarge;

		case EROFS:
			return ReadOnlyFileSystem;

		case ESPIPE:
			return PXActionRefusedInvalidSeek;

		case ESRCH:
			return NoSuchProcess;

		case ETIME:
			return StreamTimeout;

		case ETIMEDOUT:
			return PXActionFailedConnectionTimedOut;

		case ETXTBSY:
			return TextFileBusy;

		case EXDEV:
			return CrossDeviceLink;

		default:
			return PXActionInvalid;
	}
}

PXActionResult PXErrorCurrent()
{
	const int errorID = errno;
	const PXActionResult actionResult = PXErrorCodeFromID(errorID);

	/*
			DWORD error = GetLastError();
            wchar_t errorBuffer[1024];
            wsprintf(errorBuffer, L"Error creating window. Error code, decimal %d, hexadecimal %X.", error, error);
            MessageBox(NULL, errorBuffer, L"Error", MB_ICONHAND);
	*/

	return actionResult;
}


#if OSWindows
PXActionResult PXAPI PXWindowsHandleErrorFromID(const HRESULT handleResult)
{
	switch (handleResult)
	{
		case S_OK:
			return PXActionSuccessful;

		case S_FALSE:
			return PXActionSuccessful;

		case RPC_E_CHANGED_MODE:
			return PXActionRedundantAlreadyInitialized;

			// The call succeeded, but we had to substitute the 3D algorithm
		case DS_NO_VIRTUALIZATION:
			return PXActionSuccessWithSubstitution;

			// The call failed because resources (such as a priority level) were already being used by another caller
		case DSERR_ALLOCATED:
			return PXActionRefuedObjectAlreadyExists;

			// The control (vol, pan, etc.) requested by the caller is not available
		case DSERR_CONTROLUNAVAIL:
			return PXActionRefuedObjectPropertyNotAvailable;

			// An invalid parameter was passed to the returning function
		case DSERR_INVALIDPARAM:
			return PXActionRefusedArgumentInvalid;

			// This call is not valid for the current state of this object
		case DSERR_INVALIDCALL:
			return PXActionRefuedObjectStateDoesNotAllowAction;

			// An undetermined error occurred inside the DirectSound subsystem
		case DSERR_GENERIC:
			return PXActionFailedUnkownError;

			// The caller does not have the priority level required for the function to succeed
		case DSERR_PRIOLEVELNEEDED:
			return PXActionDeniedPriorityTooLow;

			// Not enough free memory is available to complete the operation
		case DSERR_OUTOFMEMORY:
			return PXActionFailedMemoryAllocation;

			// The specified WAVE format is not supported
		case DSERR_BADFORMAT:
			return PXActionRefuedFormatIllegal;

			// The function called is not supported at this time
		case DSERR_UNSUPPORTED:
			return PXActionRefuedFormatNotSupported;

			// No sound driver is available for use
		case DSERR_NODRIVER:
			return PXActionRefuedDriverMissing;

			// This object is already initialized
		case DSERR_ALREADYINITIALIZED:
			return PXActionRefuedObjectAlreadyInizialized;

			// This object does not support aggregation
		case DSERR_NOAGGREGATION:
			return PXActionRefuedObjectAggregationNotSupported;

			// The buffer memory has been lost, and must be restored
		case DSERR_BUFFERLOST:
			return PXActionFailedBufferRefernceLost;

			// Another app has a higher priority level, preventing this call from succeeding
		case DSERR_OTHERAPPHASPRIO:
			return PXActionYieldToOtherProcessWithHigherPriority;

			// This object has not been initialized
		case DSERR_UNINITIALIZED:
			return PXActionRefuedObjectNotInizialized;

			// The requested COM interface is not available
		case DSERR_NOINTERFACE:
			return PXActionRefuedObjectInterfaceNotAvailable;

			// Access is denied
		case DSERR_ACCESSDENIED:
			return PXActionDeniedNoPermission;

			// Tried to create a DSBCAPS_CTRLFX buffer shorter than DSBSIZE_FX_MIN milliseconds
		case DSERR_BUFFERTOOSMALL:
			return PXActionRefuedBufferSizeTooSmal;

			// Attempt to use DirectSound 8 functionality on an older DirectSound object
		case DSERR_DS8_REQUIRED:
			return PXActionRefusedLibraryRequiredUpgrade;

			// A circular loop of send effects was detected
		case DSERR_SENDLOOP:
			return PXActionCanceledLoopDeteced;

			// The GUID specified in an audiopath file does not match a valid MIXIN buffer
		case DSERR_BADSENDBUFFERGUID:
			return PXActionRefuedObjectIDInvalid;

			// The object requested was not found (numerically equal to DMUS_E_NOT_FOUND)
		case DSERR_OBJECTNOTFOUND:
			return PXActionRefuedObjectIDInvalid;

			// The effects requested could not be found on the system, or they were found
			// but in the wrong order, or in the wrong hardware/software locations.
		case DSERR_FXUNAVAILABLE:
			return PXActionRefusedEffectNotAvailable;

		default:
			return PXActionInvalid;
	}
}
#endif