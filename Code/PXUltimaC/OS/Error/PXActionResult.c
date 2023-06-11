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
		default:
	//	case EOTHER:
	//		return UnkownError;

		case EACCES:
			return PermissionDenied;

	//	case EADDRINUSE:
	//		return AddressInUse;

	//	case EADDRNOTAVAIL:
	//		return AddressNotAvailable;

//		case EAFNOSUPPORT:
	//		return AddressFamilyNotSupported;

	//	case EALREADY:
	//		return ConnectionAlreadyInProgress;

		case E2BIG:
			return ArgumentListTooLong;

		case EBADF:
			return BadFileDescriptor;

	//	case EBADMSG:
	//		return BadMessage;

		case EBUSY:
			return PXActionRefusedResourceBusy;

	//	case ECANCELED:
	//		return OperationCanceled;

		case ECHILD:
			return NoChildProcesses;

	//	case ECONNABORTED:
	//		return ConnectionAborted;

	//	case ECONNREFUSED:
	//		return ConnectionRefused;

	//	case ECONNRESET:
	//		return ConnectionReset;

		case EDEADLK:
			return ResourceDeadlockWouldOccur;

	//	case EDESTADDRREQ:
	//		return DestinationAddressRequired;

		case EDOM:
			return PXMathematicsArgumentOutOfDomainOfFunction;

		case EEXIST:
			return FileExists;

		case EFAULT:
			return BadAddress;

		case EFBIG:
			return FileTooLarge;

	//	case EHOSTUNREACH:
	//		return HostIsUnreachable;

	//	case EIDRM:
	//		return IdentifierRemoved;

		case EILSEQ:
			return IllegalByteSequence;

	//	case EINPROGRESS:
	//		return OperationInProgress;

		case EINTR:
			return InterruptedFunction;

		case EINVAL:
			return InvalidArgument;

		case EIO:
			return IOFailure;

	//	case EISCONN:
	//		return SocketIsConnected;

		case EISDIR:
			return IsADirectory;

	//	case ELOOP:
	//		return TooManyLevelsOfSymbolicLinks;

		case EMFILE:
			return FileDescriptorValueTooLarge;

		case EMLINK:
			return TooManyLinks;

	//	case EMSGSIZE:
	//		return MessageTooLarge;

		case ENAMETOOLONG:
			return FilenameTooLong;

	//	case ENETDOWN:
	//		return NetworkIsDown;

	//	case ENETRESET:
	//		return ConnectionAbortedByNetwork;

	//	case ENETUNREACH:
	//		return NetworkUnreachable;

		case ENFILE:
			return TooManyFilesOpenInSystem;

	//	case ENOBUFS:
	//		return NoBufferSpaceAvailable;

	//	case ENODATA:
	//		return NoMessageIsAvailableOnTheStreamHeadReadQueue;

		case ENODEV:
			return NoSuchDevice;

		case ENOENT:
			return NoSuchFileOrDirectory;

		case ENOEXEC:
			return ExecutableFileFormatError;

		case ENOLCK:
			return NoLocksAvailable;

	//	case ENOLINK:
	//		return LinkHasBeenSevered;

		case ENOMEM:
			return PXActionFailedMemoryAllocation;

	//	case ENOMSG:
	//		return NoMessageOfTheDesiredType;

	//	case ENOPROTOOPT:
	//		return ProtocolNotAvailable;

		case ENOSPC:
			return NoSpaceLeftOnDevice;

	//	case ENOSR:
	//		return NoStreamResources;

	//	case ENOSTR:
	//		return NotAStream;

		case ENOSYS:
			return FunctionNotSupported;

	//	case ENOTCONN:
	//		return TheSocketIsNotConnected;

		case ENOTDIR:
			return NotADirectory;

		case ENOTEMPTY:
			return DirectoryNotEmpty;

	//	case ENOTRECOVERABLE:
	//		return StateNotRecoverable;

	//	case ENOTSOCK:
	//		return NotASocket;

		case ENOTTY:
			return InappropriateIOControlOperation;

		case ENXIO:
			return NoSuchDeviceOrAddress;

#if OSUnix
			//case EAGAIN:
		case EWOULDBLOCK:
			return OperationWouldBlock;

			//case ENOTSUP: // fall throu, same ID unser unix.
		case ENOTSUP:
			return NotSupported;

#elif OSWindows
		case EAGAIN:
			return ResourceUnavailableTryAgain;

	//	case EWOULDBLOCK:
	//		return OperationWouldBlock;

	//	case ENOTSUP:
	//		return NotSupported;

	//	case EOPNOTSUPP:
	//		return OperationNotSupportedOnSocket;
#endif

	//	case EOVERFLOW:
	//		return ValueTooLargeToBeStoredInDataType;

	//	case EOWNERDEAD:
	//		return PreviousOwnerDied;

		case EPERM:
			return OperationNotPermitted;

		case EPIPE:
			return BrokenPipe;

	//	case EPROTO:
	//		return ProtocolError;

	//	case EPROTONOSUPPORT:
	//		return ProtocolNotSupported;

	//	case EPROTOTYPE:
	//		return ProtocolWrongTypeForSocket;

		case ERANGE:
			return ResultTooLarge;

		case EROFS:
			return ReadOnlyFileSystem;

		case ESPIPE:
			return InvalidSeek;

		case ESRCH:
			return NoSuchProcess;

	//	case ETIME:
	//		return StreamTimeout;

	//	case ETIMEDOUT:
	//		return ConnectionTimedOut;

	//	case ETXTBSY:
	//		return TextFileBusy;

		case EXDEV:
			return CrossDeviceLink;
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
