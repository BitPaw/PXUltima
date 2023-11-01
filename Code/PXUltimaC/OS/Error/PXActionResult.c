#include "PXActionResult.h"

#include <OS/System/PXOSVersion.h>

#include <errno.h> // POSIX

#if OSUnix

#define EOTHER -1
#elif OSWindows
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
