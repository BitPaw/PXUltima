#include "PXActionResult.h"

#include <OS/System/PXOSVersion.h>
#include <OS/Console/PXConsole.h>

#include <errno.h> // POSIX

#if OSUnix
#define EOTHER -1
#elif OSWindows
#include <Windows.h>
#if WindowsAtleastVista
#include <mmeapi.h>
#else
#include <MMSystem.h>
#endif
#include <dsound.h> // Windows XP, Only used for error codes
#include <WbemCli.h> // Windows Vista
#endif

PXActionResult PXAPI PXErrorCodeFromID(const int errorCode)
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
			return PXActionInvalid;//PXActionRefusedObjectAlreadyExists;

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

		case ENOTSUP: // Same as 'EOPNOTSUPP' in linux
			return PXActionInvalid;//PXActionNotSupportedByOperatingSystem;		
#if OSWindows
		case EAGAIN: // Same as 'EWOULDBLOCK' in linux
			return PXActionFailedResourceUnavailableTryAgain;

		case EOPNOTSUPP:
			return OperationNotSupportedOnSocket;
#endif

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

PXActionResult PXAPI PXErrorCurrent()
{
#if OSUnix || OSForcePOSIXForWindows

	const int errorID = errno;
	const PXActionResult actionResult = PXErrorCodeFromID(errorID);

	return actionResult;

#elif OSWindows

	const DWORD lastErrorID = GetLastError();
	const PXActionResult actionResult = PXErrorCodeFromID(lastErrorID);

	return actionResult;

#else
	return PXActionInvalid;
#endif

	return actionResult;
}


#if OSWindows
PXActionResult PXAPI PXWindowsErrorCurrent(const PXBool wasSuccessful)
{
	// if we did fail, we dont even aknowlege if an error is set.
	if(wasSuccessful)
	{
		return PXActionSuccessful;
	}

	// We will definitly have some error code now.	
	const DWORD lastErrorID = GetLastError(); // Will fetch the global current errorID
	const PXActionResult actionResult = PXErrorCodeFromID(lastErrorID); // Translate windows errorID to our own errorID

	char* errorMessageData = 0;

	// Generate an error message string with our current errorID
	const PXSize errorMessageLength = FormatMessageA
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastErrorID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&errorMessageData,
		0,
		NULL
	);

#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Windows",
		"Error",
		"%s",
		errorMessageData
	);
#endif

	// Free the Win32's string's buffer.
	LocalFree(errorMessageData);

	
	return actionResult;
}


PXActionResult PXAPI PXWindowsHandleErrorFromID(const HRESULT handleResult)
{
	switch (handleResult)
	{



		//-------------------------------------------------
		// System Error Codes(0 - 499) (0x0 - 0x1f3)
		//-------------------------------------------------
		
		// case S_OK:
		case ERROR_SUCCESS: return PXActionSuccessful; // The operation completed successfully.	0 (0x0)
		
		// case S_FALSE:
		case ERROR_INVALID_FUNCTION: return PXActionSuccessful; // Incorrect function.	1 (0x1)



		case	ERROR_FILE_NOT_FOUND: return PXActionInvalid; //	The system cannot find the file specified.	2 (0x2)
		case	ERROR_PATH_NOT_FOUND: return PXActionInvalid; //	The system cannot find the path specified.	3 (0x3)
		case	ERROR_TOO_MANY_OPEN_FILES: return PXActionInvalid; //	The system cannot open the file.	4 (0x4)
		case	ERROR_ACCESS_DENIED: return PXActionDeniedNoPermission; //	Access is denied.	5 (0x5)
		case	ERROR_INVALID_HANDLE: return PXActionInvalid; //	The handle is invalid.	6 (0x6)
		case	ERROR_ARENA_TRASHED: return PXActionInvalid; //	The storage control blocks were destroyed.	7 (0x7)
		case	ERROR_NOT_ENOUGH_MEMORY: return PXActionInvalid; //	Not enough memory resources are available to process this command.	8 (0x8)
		case	ERROR_INVALID_BLOCK: return PXActionInvalid; //	The storage control block address is invalid.	9 (0x9)
		case	ERROR_BAD_ENVIRONMENT: return PXActionInvalid; //	The environment is incorrect.	10 (0xA)
		case	ERROR_BAD_FORMAT: return PXActionInvalid; //	An attempt was made to load a program with an incorrect format.	11 (0xB)
		case	ERROR_INVALID_ACCESS: return PXActionInvalid; //	The access code is invalid.	12 (0xC)
		case	ERROR_INVALID_DATA: return PXActionInvalid; //	The data is invalid.	13 (0xD)
		case	ERROR_OUTOFMEMORY: return PXActionInvalid; //	Not enough storage is available to complete this operation.	14 (0xE)
		case	ERROR_INVALID_DRIVE: return PXActionInvalid; //	The system cannot find the drive specified.	15 (0xF)
		case	ERROR_CURRENT_DIRECTORY: return PXActionInvalid; //	The directory cannot be removed.	16 (0x10)
		case	ERROR_NOT_SAME_DEVICE: return PXActionInvalid; //	The system cannot move the file to a different disk drive.	17 (0x11)
		case	ERROR_NO_MORE_FILES: return PXActionInvalid; //	There are no more files.	18 (0x12)
		case	ERROR_WRITE_PROTECT: return PXActionInvalid; //	The media is write protected.	19 (0x13)
		case	ERROR_BAD_UNIT: return PXActionInvalid; //	The system cannot find the device specified.	20 (0x14)
		case	ERROR_NOT_READY: return PXActionInvalid; //	The device is not ready.	21 (0x15)
		case	ERROR_BAD_COMMAND: return PXActionInvalid; //	The device does not recognize the command.	22 (0x16)
		case	ERROR_CRC: return PXActionInvalid; //	Data error (cyclic redundancy check).	23 (0x17)
		case	ERROR_BAD_LENGTH: return PXActionInvalid; //	The program issued a command but the command length is incorrect.	24 (0x18)
		case	ERROR_SEEK: return PXActionInvalid; //	The drive cannot locate a specific area or track on the disk.	25 (0x19)
		case	ERROR_NOT_DOS_DISK: return PXActionInvalid; //	The specified disk or diskette cannot be accessed.	26 (0x1A)
		case	ERROR_SECTOR_NOT_FOUND: return PXActionInvalid; //	The drive cannot find the sector requested.	27 (0x1B)
		case	ERROR_OUT_OF_PAPER: return PXActionInvalid; //	The printer is out of paper.	28 (0x1C)
		case	ERROR_WRITE_FAULT: return PXActionInvalid; //	The system cannot write to the specified device.	29 (0x1D)
		case	ERROR_READ_FAULT: return PXActionInvalid; //	The system cannot read from the specified device.	30 (0x1E)
		case	ERROR_GEN_FAILURE: return PXActionInvalid; //	A device attached to the system is not functioning.	31 (0x1F)
		case	ERROR_SHARING_VIOLATION: return PXActionInvalid; //	The process cannot access the file because it is being used by another process.	32 (0x20)
		case	ERROR_LOCK_VIOLATION: return PXActionInvalid; //	The process cannot access the file because another process has locked a portion of the file.	33 (0x21)
		case	ERROR_WRONG_DISK: return PXActionInvalid; //	The wrong diskette is in the drive. Insert %2 (Volume Serial Number: %3) into drive %1.	34 (0x22)
		case	ERROR_SHARING_BUFFER_EXCEEDED: return PXActionInvalid; //	Too many files opened for sharing.	36 (0x24)
		case	ERROR_HANDLE_EOF: return PXActionInvalid; //	Reached the end of the file.	38 (0x26)
		case	ERROR_HANDLE_DISK_FULL: return PXActionInvalid; //	The disk is full.	39 (0x27)
		case	ERROR_NOT_SUPPORTED: return PXActionInvalid; //	The request is not supported.	50 (0x32)
		case	ERROR_REM_NOT_LIST: return PXActionInvalid; //	Windows cannot find the network path. Verify that the network path is correct and the destination computer is not busy or turned off. If Windows still cannot find the network path, contact your network administrator.	51 (0x33)
		case	ERROR_DUP_NAME: return PXActionInvalid; //	You were not connected because a duplicate name exists on the network. If joining a domain, go to System in Control Panel to change the computer name and try again. If joining a workgroup, choose another workgroup name.	52 (0x34)
		case	ERROR_BAD_NETPATH: return PXActionInvalid; //	The network path was not found.	53 (0x35)
		case	ERROR_NETWORK_BUSY: return PXActionInvalid; //	The network is busy.	54 (0x36)
		case	ERROR_DEV_NOT_EXIST: return PXActionInvalid; //	The specified network resource or device is no longer available.	55 (0x37)
		case	ERROR_TOO_MANY_CMDS: return PXActionInvalid; //	The network BIOS command limit has been reached.	56 (0x38)
		case	ERROR_ADAP_HDW_ERR: return PXActionInvalid; //	A network adapter hardware error occurred.	57 (0x39)
		case	ERROR_BAD_NET_RESP: return PXActionInvalid; //	The specified server cannot perform the requested operation.	58 (0x3A)
		case	ERROR_UNEXP_NET_ERR: return PXActionInvalid; //	An unexpected network error occurred.	59 (0x3B)
		case	ERROR_BAD_REM_ADAP: return PXActionInvalid; //	The remote adapter is not compatible.	60 (0x3C)
		case	ERROR_PRINTQ_FULL: return PXActionInvalid; //	The printer queue is full.	61 (0x3D)
		case	ERROR_NO_SPOOL_SPACE: return PXActionInvalid; //	Space to store the file waiting to be printed is not available on the server.	62 (0x3E)
		case	ERROR_PRINT_CANCELLED: return PXActionInvalid; //	Your file waiting to be printed was deleted.	63 (0x3F)
		case	ERROR_NETNAME_DELETED: return PXActionInvalid; //	The specified network name is no longer available.	64 (0x40)
		case	ERROR_NETWORK_ACCESS_DENIED: return PXActionInvalid; //	Network access is denied.	65 (0x41)
		case	ERROR_BAD_DEV_TYPE: return PXActionInvalid; //	The network resource type is not correct.	66 (0x42)
		case	ERROR_BAD_NET_NAME: return PXActionInvalid; //	The network name cannot be found.	67 (0x43)
		case	ERROR_TOO_MANY_NAMES: return PXActionInvalid; //	The name limit for the local computer network adapter card was exceeded.	68 (0x44)
		case	ERROR_TOO_MANY_SESS: return PXActionInvalid; //	The network BIOS session limit was exceeded.	69 (0x45)
		case	ERROR_SHARING_PAUSED: return PXActionInvalid; //	The remote server has been paused or is in the process of being started.	70 (0x46)
		case	ERROR_REQ_NOT_ACCEP: return PXActionInvalid; //	No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept.	71 (0x47)
		case	ERROR_REDIR_PAUSED: return PXActionInvalid; //	The specified printer or disk device has been paused.	72 (0x48)
		case	ERROR_FILE_EXISTS: return PXActionInvalid; //	The file exists.	80 (0x50)
		case	ERROR_CANNOT_MAKE: return PXActionInvalid; //	The directory or file cannot be created.	82 (0x52)
		case	ERROR_FAIL_I24: return PXActionInvalid; //	Fail on INT 24.	83 (0x53)
		case	ERROR_OUT_OF_STRUCTURES: return PXActionInvalid; //	Storage to process this request is not available.	84 (0x54)
		case	ERROR_ALREADY_ASSIGNED: return PXActionInvalid; //	The local device name is already in use.	85 (0x55)
		case	ERROR_INVALID_PASSWORD: return PXActionInvalid; //	The specified network password is not correct.	86 (0x56)
		case	ERROR_INVALID_PARAMETER: return PXActionRefusedArgumentInvalid; //	The parameter is incorrect.	87 (0x57)
		case	ERROR_NET_WRITE_FAULT: return PXActionInvalid; //	A write fault occurred on the network.	88 (0x58)
		case	ERROR_NO_PROC_SLOTS: return PXActionInvalid; //	The system cannot start another process at this time.	89 (0x59)
		case	ERROR_TOO_MANY_SEMAPHORES: return PXActionInvalid; //	Cannot create another system semaphore.	100 (0x64)
		case	ERROR_EXCL_SEM_ALREADY_OWNED: return PXActionInvalid; //	The exclusive semaphore is owned by another process.	101 (0x65)
		case	ERROR_SEM_IS_SET: return PXActionInvalid; //	The semaphore is set and cannot be closed.	102 (0x66)
		case	ERROR_TOO_MANY_SEM_REQUESTS: return PXActionInvalid; //	The semaphore cannot be set again.	103 (0x67)
		case	ERROR_INVALID_AT_INTERRUPT_TIME: return PXActionInvalid; //	Cannot request exclusive semaphores at interrupt time.	104 (0x68)
		case	ERROR_SEM_OWNER_DIED: return PXActionInvalid; //	The previous ownership of this semaphore has ended.	105 (0x69)
		case	ERROR_SEM_USER_LIMIT: return PXActionInvalid; //	Insert the diskette for drive %1.	106 (0x6A)
		case	ERROR_DISK_CHANGE: return PXActionInvalid; //	The program stopped because an alternate diskette was not inserted.	107 (0x6B)
		case	ERROR_DRIVE_LOCKED: return PXActionInvalid; //	The disk is in use or locked by another process.	108 (0x6C)
		case	ERROR_BROKEN_PIPE: return PXActionInvalid; //	The pipe has been ended.	109 (0x6D)
		case	ERROR_OPEN_FAILED: return PXActionInvalid; //	The system cannot open the device or file specified.	110 (0x6E)
		case	ERROR_BUFFER_OVERFLOW: return PXActionInvalid; //	The file name is too long.	111 (0x6F)
		case	ERROR_DISK_FULL: return PXActionInvalid; //	There is not enough space on the disk.	112 (0x70)
		case	ERROR_NO_MORE_SEARCH_HANDLES: return PXActionInvalid; //	No more internal file identifiers available.	113 (0x71)
		case	ERROR_INVALID_TARGET_HANDLE: return PXActionInvalid; //	The target internal file identifier is incorrect.	114 (0x72)
		case	ERROR_INVALID_CATEGORY: return PXActionInvalid; //	The IOCTL call made by the application program is not correct.	117 (0x75)
		case	ERROR_INVALID_VERIFY_SWITCH: return PXActionInvalid; //	The verify-on-write switch parameter value is not correct.	118 (0x76)
		case	ERROR_BAD_DRIVER_LEVEL: return PXActionInvalid; //	The system does not support the command requested.	119 (0x77)
		case	ERROR_CALL_NOT_IMPLEMENTED: return PXActionInvalid; //	This function is not supported on this system.	120 (0x78)
		case	ERROR_SEM_TIMEOUT: return PXActionInvalid; //	The semaphore timeout period has expired.	121 (0x79)
		case	ERROR_INSUFFICIENT_BUFFER: return PXActionInvalid; //	The data area passed to a system call is too small.	122 (0x7A)
		case	ERROR_INVALID_NAME: return PXActionInvalid; //	The filename, directory name, or volume label syntax is incorrect.	123 (0x7B)
		case	ERROR_INVALID_LEVEL: return PXActionInvalid; //	The system call level is not correct.	124 (0x7C)
		case	ERROR_NO_VOLUME_LABEL: return PXActionInvalid; //	The disk has no volume label.	125 (0x7D)
		case	ERROR_MOD_NOT_FOUND: return PXActionInvalid; //	The specified module could not be found.	126 (0x7E)
		case	ERROR_PROC_NOT_FOUND: return PXActionInvalid; //	The specified procedure could not be found.	127 (0x7F)
		case	ERROR_WAIT_NO_CHILDREN: return PXActionInvalid; //	There are no child processes to wait for.	128 (0x80)
		case	ERROR_CHILD_NOT_COMPLETE: return PXActionInvalid; //	The %1 application cannot be run in Win32 mode.	129 (0x81)
		case	ERROR_DIRECT_ACCESS_HANDLE: return PXActionInvalid; //	Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O.	130 (0x82)
		case	ERROR_NEGATIVE_SEEK: return PXActionInvalid; //	An attempt was made to move the file pointer before the beginning of the file.	131 (0x83)
		case	ERROR_SEEK_ON_DEVICE: return PXActionInvalid; //	The file pointer cannot be set on the specified device or file.	132 (0x84)
		case	ERROR_IS_JOIN_TARGET: return PXActionInvalid; //	A JOIN or SUBST command cannot be used for a drive that contains previously joined drives.	133 (0x85)
		case	ERROR_IS_JOINED: return PXActionInvalid; //	An attempt was made to use a JOIN or SUBST command on a drive that has already been joined.	134 (0x86)
		case	ERROR_IS_SUBSTED: return PXActionInvalid; //	An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted.	135 (0x87)
		case	ERROR_NOT_JOINED: return PXActionInvalid; //	The system tried to delete the JOIN of a drive that is not joined.	136 (0x88)
		case	ERROR_NOT_SUBSTED: return PXActionInvalid; //	The system tried to delete the substitution of a drive that is not substituted.	137 (0x89)
		case	ERROR_JOIN_TO_JOIN: return PXActionInvalid; //	The system tried to join a drive to a directory on a joined drive.	138 (0x8A)
		case	ERROR_SUBST_TO_SUBST: return PXActionInvalid; //	The system tried to substitute a drive to a directory on a substituted drive.	139 (0x8B)
		case	ERROR_JOIN_TO_SUBST: return PXActionInvalid; //	The system tried to join a drive to a directory on a substituted drive.	140 (0x8C)
		case	ERROR_SUBST_TO_JOIN: return PXActionInvalid; //	The system tried to SUBST a drive to a directory on a joined drive.	141 (0x8D)
		case	ERROR_BUSY_DRIVE: return PXActionInvalid; //	The system cannot perform a JOIN or SUBST at this time.	142 (0x8E)
		case	ERROR_SAME_DRIVE: return PXActionInvalid; //	The system cannot join or substitute a drive to or for a directory on the same drive.	143 (0x8F)
		case	ERROR_DIR_NOT_ROOT: return PXActionInvalid; //	The directory is not a subdirectory of the root directory.	144 (0x90)
		case	ERROR_DIR_NOT_EMPTY: return PXActionInvalid; //	The directory is not empty.	145 (0x91)
		case	ERROR_IS_SUBST_PATH: return PXActionInvalid; //	The path specified is being used in a substitute.	146 (0x92)
		case	ERROR_IS_JOIN_PATH: return PXActionInvalid; //	Not enough resources are available to process this command.	147 (0x93)
		case	ERROR_PATH_BUSY: return PXActionInvalid; //	The path specified cannot be used at this time.	148 (0x94)
		case	ERROR_IS_SUBST_TARGET: return PXActionInvalid; //	An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute.	149 (0x95)
		case	ERROR_SYSTEM_TRACE: return PXActionInvalid; //	System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed.	150 (0x96)
		case	ERROR_INVALID_EVENT_COUNT: return PXActionInvalid; //	The number of specified semaphore events for DosMuxSemWait is not correct.	151 (0x97)
		case	ERROR_TOO_MANY_MUXWAITERS: return PXActionInvalid; //	DosMuxSemWait did not execute; too many semaphores are already set.	152 (0x98)
		case	ERROR_INVALID_LIST_FORMAT: return PXActionInvalid; //	The DosMuxSemWait list is not correct.	153 (0x99)
		case	ERROR_LABEL_TOO_LONG: return PXActionInvalid; //	The volume label you entered exceeds the label character limit of the target file system.	154 (0x9A)
		case	ERROR_TOO_MANY_TCBS: return PXActionInvalid; //	Cannot create another thread.	155 (0x9B)
		case	ERROR_SIGNAL_REFUSED: return PXActionInvalid; //	The recipient process has refused the signal.	156 (0x9C)
		case	ERROR_DISCARDED: return PXActionInvalid; //	The segment is already discarded and cannot be locked.	157 (0x9D)
		case	ERROR_NOT_LOCKED: return PXActionInvalid; //	The segment is already unlocked.	158 (0x9E)
		case	ERROR_BAD_THREADID_ADDR: return PXActionInvalid; //	The address for the thread ID is not correct.	159 (0x9F)
		case	ERROR_BAD_ARGUMENTS: return PXActionInvalid; //	One or more arguments are not correct.	160 (0xA0)
		case	ERROR_BAD_PATHNAME: return PXActionInvalid; //	The specified path is invalid.	161 (0xA1)
		case	ERROR_SIGNAL_PENDING: return PXActionInvalid; //	A signal is already pending.	162 (0xA2)
		case	ERROR_MAX_THRDS_REACHED: return PXActionInvalid; //	No more threads can be created in the system.	164 (0xA4)
		case	ERROR_LOCK_FAILED: return PXActionInvalid; //	Unable to lock a region of a file.	167 (0xA7)
		case	ERROR_BUSY: return PXActionInvalid; //	The requested resource is in use.	170 (0xAA)
		
#if WindowsAtleastVista
		case	ERROR_DEVICE_SUPPORT_IN_PROGRESS: return PXActionInvalid; //	Device's command support detection is in progress.	171 (0xAB)
#endif

		case	ERROR_CANCEL_VIOLATION: return PXActionInvalid; //	A lock request was not outstanding for the supplied cancel region.	173 (0xAD)
		case	ERROR_ATOMIC_LOCKS_NOT_SUPPORTED: return PXActionInvalid; //	The file system does not support atomic changes to the lock type.	174 (0xAE)
		case	ERROR_INVALID_SEGMENT_NUMBER: return PXActionInvalid; //	The system detected a segment number that was not correct.	180 (0xB4)
		case	ERROR_INVALID_ORDINAL: return PXActionInvalid; //	The operating system cannot run %1.	182 (0xB6)
		case	ERROR_ALREADY_EXISTS: return PXActionInvalid; //	Cannot create a file when that file already exists.	183 (0xB7)
		case	ERROR_INVALID_FLAG_NUMBER: return PXActionInvalid; //	The flag passed is not correct.	186 (0xBA)
		case	ERROR_SEM_NOT_FOUND: return PXActionInvalid; //	The specified system semaphore name was not found.	187 (0xBB)
		case	ERROR_INVALID_STARTING_CODESEG: return PXActionInvalid; //	The operating system cannot run %1.	188 (0xBC)
		case	ERROR_INVALID_STACKSEG: return PXActionInvalid; //	The operating system cannot run %1.	189 (0xBD)
		case	ERROR_INVALID_MODULETYPE: return PXActionInvalid; //	The operating system cannot run %1.	190 (0xBE)
		case	ERROR_INVALID_EXE_SIGNATURE: return PXActionInvalid; //	Cannot run %1 in Win32 mode.	191 (0xBF)
		case	ERROR_EXE_MARKED_INVALID: return PXActionInvalid; //	The operating system cannot run %1.	192 (0xC0)
		case	ERROR_BAD_EXE_FORMAT: return PXActionInvalid; //	%1 is not a valid Win32 application.	193 (0xC1)
		case	ERROR_ITERATED_DATA_EXCEEDS_64k: return PXActionInvalid; //	The operating system cannot run %1.	194 (0xC2)
		case	ERROR_INVALID_MINALLOCSIZE: return PXActionInvalid; //	The operating system cannot run %1.	195 (0xC3)
		case	ERROR_DYNLINK_FROM_INVALID_RING: return PXActionInvalid; //	The operating system cannot run this application program.	196 (0xC4)
		case	ERROR_IOPL_NOT_ENABLED: return PXActionInvalid; //	The operating system is not presently configured to run this application.	197 (0xC5)
		case	ERROR_INVALID_SEGDPL: return PXActionInvalid; //	The operating system cannot run %1.	198 (0xC6)
		case	ERROR_AUTODATASEG_EXCEEDS_64k: return PXActionInvalid; //	The operating system cannot run this application program.	199 (0xC7)
		case	ERROR_RING2SEG_MUST_BE_MOVABLE: return PXActionInvalid; //	The code segment cannot be greater than or equal to 64K.	200 (0xC8)
		case	ERROR_RELOC_CHAIN_XEEDS_SEGLIM: return PXActionInvalid; //	The operating system cannot run %1.	201 (0xC9)
		case	ERROR_INFLOOP_IN_RELOC_CHAIN: return PXActionInvalid; //	The operating system cannot run %1.	202 (0xCA)
		case	ERROR_ENVVAR_NOT_FOUND: return PXActionInvalid; //	The system could not find the environment option that was entered.	203 (0xCB)
		case	ERROR_NO_SIGNAL_SENT: return PXActionInvalid; //	No process in the command subtree has a signal handler.	205 (0xCD)
		case	ERROR_FILENAME_EXCED_RANGE: return PXActionInvalid; //	The filename or extension is too long.	206 (0xCE)
		case	ERROR_RING2_STACK_IN_USE: return PXActionInvalid; //	The ring 2 stack is in use.	207 (0xCF)
		case	ERROR_META_EXPANSION_TOO_LONG: return PXActionInvalid; //	The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified.	208 (0xD0)
		case	ERROR_INVALID_SIGNAL_NUMBER: return PXActionInvalid; //	The signal being posted is not correct.	209 (0xD1)
		case	ERROR_THREAD_1_INACTIVE: return PXActionInvalid; //	The signal handler cannot be set.	210 (0xD2)
		case	ERROR_LOCKED: return PXActionInvalid; //	The segment is locked and cannot be reallocated.	212 (0xD4)
		case	ERROR_TOO_MANY_MODULES: return PXActionInvalid; //	Too many dynamic-link modules are attached to this program or dynamic-link module.	214 (0xD6)
		case	ERROR_NESTING_NOT_ALLOWED: return PXActionInvalid; //	Cannot nest calls to LoadModule.	215 (0xD7)
		case	ERROR_EXE_MACHINE_TYPE_MISMATCH: return PXActionInvalid; //	This version of %1 is not compatible with the version of Windows you're running. Check your computer's system information and then contact the software publisher.	216 (0xD8)
		case	ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY: return PXActionInvalid; //	The image file %1 is signed, unable to modify.	217 (0xD9)
		case	ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY: return PXActionInvalid; //	The image file %1 is strong signed, unable to modify.	218 (0xDA)
		case	ERROR_FILE_CHECKED_OUT: return PXActionInvalid; //	This file is checked out or locked for editing by another user.	220 (0xDC)
		case	ERROR_CHECKOUT_REQUIRED: return PXActionInvalid; //	The file must be checked out before saving changes.	221 (0xDD)
		case	ERROR_BAD_FILE_TYPE: return PXActionInvalid; //	The file type being saved or retrieved has been blocked.	222 (0xDE)
		case	ERROR_FILE_TOO_LARGE: return PXActionInvalid; //	The file size exceeds the limit allowed and cannot be saved.	223 (0xDF)
		case	ERROR_FORMS_AUTH_REQUIRED: return PXActionInvalid; //	Access Denied. Before opening files in this location, you must first add the web site to your trusted sites list, browse to the web site, and select the option to login automatically.	224 (0xE0)
		case	ERROR_VIRUS_INFECTED: return PXActionInvalid; //	Operation did not complete successfully because the file contains a virus or potentially unwanted software.	225 (0xE1)
		case	ERROR_VIRUS_DELETED: return PXActionInvalid; //	This file contains a virus or potentially unwanted software and cannot be opened. Due to the nature of this virus or potentially unwanted software, the file has been removed from this location.	226 (0xE2)
		case	ERROR_PIPE_LOCAL: return PXActionInvalid; //	The pipe is local.	229 (0xE5)
		case	ERROR_BAD_PIPE: return PXActionInvalid; //	The pipe state is invalid.	230 (0xE6)
		case	ERROR_PIPE_BUSY: return PXActionInvalid; //	All pipe instances are busy.	231 (0xE7)
		case	ERROR_NO_DATA: return PXActionInvalid; //	The pipe is being closed.	232 (0xE8)
		case	ERROR_PIPE_NOT_CONNECTED: return PXActionInvalid; //	No process is on the other end of the pipe.	233 (0xE9)
		case	ERROR_MORE_DATA: return PXActionInvalid; //	More data is available.	234 (0xEA)
		case	ERROR_VC_DISCONNECTED: return PXActionInvalid; //	The session was canceled.	240 (0xF0)
		case	ERROR_INVALID_EA_NAME: return PXActionInvalid; //	The specified extended attribute name was invalid.	254 (0xFE)
		case	ERROR_EA_LIST_INCONSISTENT: return PXActionInvalid; //	The extended attributes are inconsistent.	255 (0xFF)
		case	WAIT_TIMEOUT: return PXActionInvalid; //	The wait operation timed out.	258 (0x102)
		case	ERROR_NO_MORE_ITEMS: return PXActionInvalid; //	No more data is available.	259 (0x103)
		case	ERROR_CANNOT_COPY: return PXActionInvalid; //	The copy functions cannot be used.	266 (0x10A)
		case	ERROR_DIRECTORY: return PXActionInvalid; //	The directory name is invalid.	267 (0x10B)
		case	ERROR_EAS_DIDNT_FIT: return PXActionInvalid; //	The extended attributes did not fit in the buffer.	275 (0x113)
		case	ERROR_EA_FILE_CORRUPT: return PXActionInvalid; //	The extended attribute file on the mounted file system is corrupt.	276 (0x114)
		case	ERROR_EA_TABLE_FULL: return PXActionInvalid; //	The extended attribute table file is full.	277 (0x115)
		case	ERROR_INVALID_EA_HANDLE: return PXActionInvalid; //	The specified extended attribute handle is invalid.	278 (0x116)
		case	ERROR_EAS_NOT_SUPPORTED: return PXActionInvalid; //	The mounted file system does not support extended attributes.	282 (0x11A)
		case	ERROR_NOT_OWNER: return PXActionInvalid; //	Attempt to release mutex not owned by caller.	288 (0x120)
		case	ERROR_TOO_MANY_POSTS: return PXActionInvalid; //	Too many posts were made to a semaphore.	298 (0x12A)
		case	ERROR_PARTIAL_COPY: return PXActionInvalid; //	Only part of a ReadProcessMemory or WriteProcessMemory request was completed.	299 (0x12B)
		case	ERROR_OPLOCK_NOT_GRANTED: return PXActionInvalid; //	The oplock request is denied.	300 (0x12C)
		case	ERROR_INVALID_OPLOCK_PROTOCOL: return PXActionInvalid; //	An invalid oplock acknowledgment was received by the system.	301 (0x12D)
		case	ERROR_DISK_TOO_FRAGMENTED: return PXActionInvalid; //	The volume is too fragmented to complete this operation.	302 (0x12E)
		case	ERROR_DELETE_PENDING: return PXActionInvalid; //	The file cannot be opened because it is in the process of being deleted.	303 (0x12F)
		case	ERROR_INCOMPATIBLE_WITH_GLOBAL_SHORT_NAME_REGISTRY_SETTING: return PXActionInvalid; //	Short name settings may not be changed on this volume due to the global registry setting.	304 (0x130)
		case	ERROR_SHORT_NAMES_NOT_ENABLED_ON_VOLUME: return PXActionInvalid; //	Short names are not enabled on this volume.	305 (0x131)
		case	ERROR_SECURITY_STREAM_IS_INCONSISTENT: return PXActionInvalid; //	The security stream for the given volume is in an inconsistent state. Please run CHKDSK on the volume.	306 (0x132)
		case	ERROR_INVALID_LOCK_RANGE: return PXActionInvalid; //	A requested file lock operation cannot be processed due to an invalid byte range.	307 (0x133)
		case	ERROR_IMAGE_SUBSYSTEM_NOT_PRESENT: return PXActionInvalid; //	The subsystem needed to support the image type is not present.	308 (0x134)
		case	ERROR_NOTIFICATION_GUID_ALREADY_DEFINED: return PXActionInvalid; //	The specified file already has a notification GUID associated with it.	309 (0x135)
		
#if WindowsAtleastVista
		case	ERROR_INVALID_EXCEPTION_HANDLER: return PXActionInvalid; //	An invalid exception handler routine has been detected.	310 (0x136)
		case	ERROR_DUPLICATE_PRIVILEGES: return PXActionInvalid; //	Duplicate privileges were specified for the token.	311 (0x137)
		case	ERROR_NO_RANGES_PROCESSED: return PXActionInvalid; //	No ranges for the specified operation were able to be processed.	312 (0x138)
		case	ERROR_NOT_ALLOWED_ON_SYSTEM_FILE: return PXActionInvalid; //	Operation is not allowed on a file system internal file.	313 (0x139)
		case	ERROR_DISK_RESOURCES_EXHAUSTED: return PXActionInvalid; //	The physical resources of this disk have been exhausted.	314 (0x13A)
		case	ERROR_INVALID_TOKEN: return PXActionInvalid; //	The token representing the data is invalid.	315 (0x13B)
		case	ERROR_DEVICE_FEATURE_NOT_SUPPORTED: return PXActionInvalid; //	The device does not support the command feature.	316 (0x13C)
#endif
		
		case	ERROR_MR_MID_NOT_FOUND: return PXActionInvalid; //	The system cannot find message text for message number 0x%1 in the message file for %2.	317 (0x13D)
		case	ERROR_SCOPE_NOT_FOUND: return PXActionInvalid; //	The scope specified was not found.	318 (0x13E)
		
#if WindowsAtleastVista
		case	ERROR_UNDEFINED_SCOPE: return PXActionInvalid; //	The Central Access Policy specified is not defined on the target machine.	319 (0x13F)
		case	ERROR_INVALID_CAP: return PXActionInvalid; //	The Central Access Policy obtained from Active Directory is invalid.	320 (0x140)
		case	ERROR_DEVICE_UNREACHABLE: return PXActionInvalid; //	The device is unreachable.	321 (0x141)
		case	ERROR_DEVICE_NO_RESOURCES: return PXActionInvalid; //	The target device has insufficient resources to complete the operation.	322 (0x142)
		case	ERROR_DATA_CHECKSUM_ERROR: return PXActionInvalid; //	A data integrity checksum error occurred. Data in the file stream is corrupt.	323 (0x143)
		case	ERROR_INTERMIXED_KERNEL_EA_OPERATION: return PXActionInvalid; //	An attempt was made to modify both a KERNEL and normal Extended Attribute (EA) in the same operation.	324 (0x144)
		case	ERROR_FILE_LEVEL_TRIM_NOT_SUPPORTED: return PXActionInvalid; //	Device does not support file-level TRIM.	326 (0x146)
		case	ERROR_OFFSET_ALIGNMENT_VIOLATION: return PXActionInvalid; //	The command specified a data offset that does not align to the device's granularity/alignment.	327 (0x147)
		case	ERROR_INVALID_FIELD_IN_PARAMETER_LIST: return PXActionInvalid; //	The command specified an invalid field in its parameter list.	328 (0x148)
		case	ERROR_OPERATION_IN_PROGRESS: return PXActionInvalid; //	An operation is currently in progress with the device.	329 (0x149)
		case	ERROR_BAD_DEVICE_PATH: return PXActionInvalid; //	An attempt was made to send down the command via an invalid path to the target device.	330 (0x14A)
		case	ERROR_TOO_MANY_DESCRIPTORS: return PXActionInvalid; //	The command specified a number of descriptors that exceeded the maximum supported by the device.	331 (0x14B)
		case	ERROR_SCRUB_DATA_DISABLED: return PXActionInvalid; //	Scrub is disabled on the specified file.	332 (0x14C)
		case	ERROR_NOT_REDUNDANT_STORAGE: return PXActionInvalid; //	The storage device does not provide redundancy.	333 (0x14D)
		case	ERROR_RESIDENT_FILE_NOT_SUPPORTED: return PXActionInvalid; //	An operation is not supported on a resident file.	334 (0x14E)
		case	ERROR_COMPRESSED_FILE_NOT_SUPPORTED: return PXActionInvalid; //	An operation is not supported on a compressed file.	335 (0x14F)
		case	ERROR_DIRECTORY_NOT_SUPPORTED: return PXActionInvalid; //	An operation is not supported on a directory.	336 (0x150)
		case	ERROR_NOT_READ_FROM_COPY: return PXActionInvalid; //	The specified copy of the requested data could not be read.	337 (0x151)
#endif
		
		case	ERROR_FAIL_NOACTION_REBOOT: return PXActionInvalid; //	No action was taken as a system reboot is required.	350 (0x15E)
		case	ERROR_FAIL_SHUTDOWN: return PXActionInvalid; //	The shutdown operation failed.	351 (0x15F)
		case	ERROR_FAIL_RESTART: return PXActionInvalid; //	The restart operation failed.	352 (0x160)
		case	ERROR_MAX_SESSIONS_REACHED: return PXActionInvalid; //	The maximum number of sessions has been reached.	353 (0x161)
		case	ERROR_THREAD_MODE_ALREADY_BACKGROUND: return PXActionInvalid; //	The thread is already in background processing mode.	400 (0x190)
		case	ERROR_THREAD_MODE_NOT_BACKGROUND: return PXActionInvalid; //	The thread is not in background processing mode.	401 (0x191)
		case	ERROR_PROCESS_MODE_ALREADY_BACKGROUND: return PXActionInvalid; //	The process is already in background processing mode.	402 (0x192)
		case	ERROR_PROCESS_MODE_NOT_BACKGROUND: return PXActionInvalid; //	The process is not in background processing mode.	403 (0x193)
		case	ERROR_INVALID_ADDRESS: return PXActionInvalid; //	Attempt to access invalid address.	487 (0x1E7)

		//-------------------------------------------------
		// System Error Codes(500 - 999) (0x1f4 - 0x3e7)
		//-------------------------------------------------
		
		//-------------------------------------------------
		// System Error Codes(1000 - 1299) (0x3e8 - 0x513)
		//-------------------------------------------------
		
		//-------------------------------------------------
		// System Error Codes(1300 - 1699) (0x514 - 0x6a3)
		//-------------------------------------------------
		 
		//-------------------------------------------------
		// System Error Codes(1700 - 3999) (0x6a4 - 0xf9f)
		//-------------------------------------------------

		//-------------------------------------------------
		// System Error Codes(4000 - 5999) (0xfa0 - 0x176f)
		//-------------------------------------------------
		case ERROR_WINS_INTERNAL: return  PXActionInvalid; // WINS encountered an error while processing the command. 4000 (0xFA0)
		case ERROR_CAN_NOT_DEL_LOCAL_WINS: return  PXActionInvalid; // The local WINS cannot be deleted. 4001 (0xFA1)
		case ERROR_STATIC_INIT: return  PXActionInvalid; // The importation from the file failed. 4002 (0xFA2)
		case ERROR_INC_BACKUP: return  PXActionInvalid; // The backup failed. Was a full backup done before? 4003 (0xFA3)
		case ERROR_FULL_BACKUP: return  PXActionInvalid; // The backup failed. Check the directory to which you are backing the database. 4004 (0xFA4)
		case ERROR_REC_NON_EXISTENT: return  PXActionInvalid; // The name does not exist in the WINS database. 4005 (0xFA5)
		case ERROR_RPL_NOT_ALLOWED: return  PXActionInvalid; // Replication with a nonconfigured partner is not allowed. 4006 (0xFA6)
		case PEERDIST_ERROR_CONTENTINFO_VERSION_UNSUPPORTED: return  PXActionInvalid; // The version of the supplied content information is not supported. 4050 (0xFD2)
		case PEERDIST_ERROR_CANNOT_PARSE_CONTENTINFO: return  PXActionInvalid; // The supplied content information is malformed. 4051 (0xFD3)
		case PEERDIST_ERROR_MISSING_DATA: return  PXActionInvalid; // The requested data cannot be found in local or peer caches. 4052 (0xFD4)
		case PEERDIST_ERROR_NO_MORE: return  PXActionInvalid; // No more data is available or required. 4053 (0xFD5)
		case PEERDIST_ERROR_NOT_INITIALIZED: return  PXActionInvalid; // The supplied object has not been initialized. 4054 (0xFD6)
		case PEERDIST_ERROR_ALREADY_INITIALIZED: return  PXActionInvalid; // The supplied object has already been initialized. 4055 (0xFD7)
		case PEERDIST_ERROR_SHUTDOWN_IN_PROGRESS: return  PXActionInvalid; // A shutdown operation is already in progress. 4056 (0xFD8)
		case PEERDIST_ERROR_INVALIDATED: return  PXActionInvalid; // The supplied object has already been invalidated. 4057 (0xFD9)
		case PEERDIST_ERROR_ALREADY_EXISTS: return  PXActionInvalid; // An element already exists and was not replaced. 4058 (0xFDA)
		case PEERDIST_ERROR_OPERATION_NOTFOUND: return  PXActionInvalid; // Can not cancel the requested operation as it has already been completed. 4059 (0xFDB)
		case PEERDIST_ERROR_ALREADY_COMPLETED: return  PXActionInvalid; // Can not perform the reqested operation because it has already been carried out. 4060 (0xFDC)
		case PEERDIST_ERROR_OUT_OF_BOUNDS: return  PXActionInvalid; // An operation accessed data beyond the bounds of valid data. 4061 (0xFDD)
		case PEERDIST_ERROR_VERSION_UNSUPPORTED: return  PXActionInvalid; // The requested version is not supported. 4062 (0xFDE)
		case PEERDIST_ERROR_INVALID_CONFIGURATION: return  PXActionInvalid; // A configuration value is invalid. 4063 (0xFDF)
		case PEERDIST_ERROR_NOT_LICENSED: return  PXActionInvalid; // The SKU is not licensed. 4064 (0xFE0)
		case PEERDIST_ERROR_SERVICE_UNAVAILABLE: return  PXActionInvalid; // PeerDist Service is still initializing and will be available shortly. 4065 (0xFE1)
		
#if WindowsAtleastVista
		case PEERDIST_ERROR_TRUST_FAILURE: return  PXActionInvalid; // Communication with one or more computers will be temporarily blocked due to recent errors. 4066 (0xFE2)
#endif
		
		case ERROR_DHCP_ADDRESS_CONFLICT: return  PXActionInvalid; // The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address. 4100 (0x1004)
		case ERROR_WMI_GUID_NOT_FOUND: return  PXActionInvalid; // The GUID passed was not recognized as valid by a WMI data provider. 4200 (0x1068)
		case ERROR_WMI_INSTANCE_NOT_FOUND: return  PXActionInvalid; // The instance name passed was not recognized as valid by a WMI data provider. 4201 (0x1069)
		case ERROR_WMI_ITEMID_NOT_FOUND: return  PXActionInvalid; // The data item ID passed was not recognized as valid by a WMI data provider. 4202 (0x106A)
		case ERROR_WMI_TRY_AGAIN: return  PXActionInvalid; // The WMI request could not be completed and should be retried. 4203 (0x106B)
		case ERROR_WMI_DP_NOT_FOUND: return  PXActionInvalid; // The WMI data provider could not be located. 4204 (0x106C)
		case ERROR_WMI_UNRESOLVED_INSTANCE_REF: return  PXActionInvalid; // The WMI data provider references an instance set that has not been registered. 4205 (0x106D)
		case ERROR_WMI_ALREADY_ENABLED: return  PXActionInvalid; // The WMI data block or event notification has already been enabled. 4206 (0x106E)
		case ERROR_WMI_GUID_DISCONNECTED: return  PXActionInvalid; // The WMI data block is no longer available. 4207 (0x106F)
		case ERROR_WMI_SERVER_UNAVAILABLE: return  PXActionInvalid; // The WMI data service is not available. 4208 (0x1070)
		case ERROR_WMI_DP_FAILED: return  PXActionInvalid; // The WMI data provider failed to carry out the request. 4209 (0x1071)
		case ERROR_WMI_INVALID_MOF: return  PXActionInvalid; // The WMI MOF information is not valid. 4210 (0x1072)
		case ERROR_WMI_INVALID_REGINFO: return  PXActionInvalid; // The WMI registration information is not valid. 4211 (0x1073)
		case ERROR_WMI_ALREADY_DISABLED: return  PXActionInvalid; // The WMI data block or event notification has already been disabled. 4212 (0x1074)
		case ERROR_WMI_READ_ONLY: return  PXActionInvalid; // The WMI data item or data block is read only. 4213 (0x1075)
		case ERROR_WMI_SET_FAILURE: return  PXActionInvalid; // The WMI data item or data block could not be changed. 4214 (0x1076)
		
#if WindowsAtleastVista
		case ERROR_NOT_APPCONTAINER: return  PXActionInvalid; // This operation is only valid in the context of an app container. 4250 (0x109A)
		case ERROR_APPCONTAINER_REQUIRED: return  PXActionInvalid; // This application can only run in the context of an app container. 4251 (0x109B)
		case ERROR_NOT_SUPPORTED_IN_APPCONTAINER: return  PXActionInvalid; // This functionality is not supported in the context of an app container. 4252 (0x109C)
		case ERROR_INVALID_PACKAGE_SID_LENGTH: return  PXActionInvalid; // The length of the SID supplied is not a valid length for app container SIDs. 4253 (0x109D)
#endif
		
		case ERROR_INVALID_MEDIA: return  PXActionInvalid; // The media identifier does not represent a valid medium. 4300 (0x10CC)
		case ERROR_INVALID_LIBRARY: return  PXActionInvalid; // The library identifier does not represent a valid library. 4301 (0x10CD)
		case ERROR_INVALID_MEDIA_POOL: return  PXActionInvalid; // The media pool identifier does not represent a valid media pool. 4302 (0x10CE)
		case ERROR_DRIVE_MEDIA_MISMATCH: return  PXActionInvalid; // The drive and medium are not compatible or exist in different libraries. 4303 (0x10CF)
		case ERROR_MEDIA_OFFLINE: return  PXActionInvalid; // The medium currently exists in an offline library and must be online to perform this operation. 4304 (0x10D0)
		case ERROR_LIBRARY_OFFLINE: return  PXActionInvalid; // The operation cannot be performed on an offline library. 4305 (0x10D1)
		case ERROR_EMPTY: return  PXActionInvalid; // The library, drive, or media pool is empty. 4306 (0x10D2)
		case ERROR_NOT_EMPTY: return  PXActionInvalid; // The library, drive, or media pool must be empty to perform this operation. 4307 (0x10D3)
		case ERROR_MEDIA_UNAVAILABLE: return  PXActionInvalid; // No media is currently available in this media pool or library. 4308 (0x10D4)
		case ERROR_RESOURCE_DISABLED: return  PXActionInvalid; // A resource required for this operation is disabled. 4309 (0x10D5)
		case ERROR_INVALID_CLEANER: return  PXActionInvalid; // The media identifier does not represent a valid cleaner. 4310 (0x10D6)
		case ERROR_UNABLE_TO_CLEAN: return  PXActionInvalid; // The drive cannot be cleaned or does not support cleaning. 4311 (0x10D7)
		case ERROR_OBJECT_NOT_FOUND: return  PXActionInvalid; // The object identifier does not represent a valid object. 4312 (0x10D8)
		case ERROR_DATABASE_FAILURE: return  PXActionInvalid; // Unable to read from or write to the database. 4313 (0x10D9)
		case ERROR_DATABASE_FULL: return  PXActionInvalid; // The database is full. 4314 (0x10DA)
		case ERROR_MEDIA_INCOMPATIBLE: return  PXActionInvalid; // The medium is not compatible with the device or media pool. 4315 (0x10DB)
		case ERROR_RESOURCE_NOT_PRESENT: return  PXActionInvalid; // The resource required for this operation does not exist. 4316 (0x10DC)
		case ERROR_INVALID_OPERATION: return  PXActionInvalid; // The operation identifier is not valid. 4317 (0x10DD)
		case ERROR_MEDIA_NOT_AVAILABLE: return  PXActionInvalid; // The media is not mounted or ready for use. 4318 (0x10DE)
		case ERROR_DEVICE_NOT_AVAILABLE: return  PXActionInvalid; // The device is not ready for use. 4319 (0x10DF)
		case ERROR_REQUEST_REFUSED: return  PXActionInvalid; // The operator or administrator has refused the request. 4320 (0x10E0)
		case ERROR_INVALID_DRIVE_OBJECT: return  PXActionInvalid; // The drive identifier does not represent a valid drive. 4321 (0x10E1)
		case ERROR_LIBRARY_FULL: return  PXActionInvalid; // Library is full. No slot is available for use. 4322 (0x10E2)
		case ERROR_MEDIUM_NOT_ACCESSIBLE: return  PXActionInvalid; // The transport cannot access the medium. 4323 (0x10E3)
		case ERROR_UNABLE_TO_LOAD_MEDIUM: return  PXActionInvalid; // Unable to load the medium into the drive. 4324 (0x10E4)
		case ERROR_UNABLE_TO_INVENTORY_DRIVE: return  PXActionInvalid; // Unable to retrieve the drive status. 4325 (0x10E5)
		case ERROR_UNABLE_TO_INVENTORY_SLOT: return  PXActionInvalid; // Unable to retrieve the slot status. 4326 (0x10E6)
		case ERROR_UNABLE_TO_INVENTORY_TRANSPORT: return  PXActionInvalid; // Unable to retrieve status about the transport. 4327 (0x10E7)
		case ERROR_TRANSPORT_FULL: return  PXActionInvalid; // Cannot use the transport because it is already in use. 4328 (0x10E8)
		case ERROR_CONTROLLING_IEPORT: return  PXActionInvalid; // Unable to open or close the inject/eject port. 4329 (0x10E9)
		case ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA: return  PXActionInvalid; // Unable to eject the medium because it is in a drive. 4330 (0x10EA)
		case ERROR_CLEANER_SLOT_SET: return  PXActionInvalid; // A cleaner slot is already reserved. 4331 (0x10EB)
		case ERROR_CLEANER_SLOT_NOT_SET: return  PXActionInvalid; // A cleaner slot is not reserved. 4332 (0x10EC)
		case ERROR_CLEANER_CARTRIDGE_SPENT: return  PXActionInvalid; // The cleaner cartridge has performed the maximum number of drive cleanings. 4333 (0x10ED)
		case ERROR_UNEXPECTED_OMID: return  PXActionInvalid; // Unexpected on-medium identifier. 4334 (0x10EE)
		case ERROR_CANT_DELETE_LAST_ITEM: return  PXActionInvalid; // The last remaining item in this group or resource cannot be deleted. 4335 (0x10EF)
		case ERROR_MESSAGE_EXCEEDS_MAX_SIZE: return  PXActionInvalid; // The message provided exceeds the maximum size allowed for this parameter. 4336 (0x10F0)
		case ERROR_VOLUME_CONTAINS_SYS_FILES: return  PXActionInvalid; // The volume contains system or paging files. 4337 (0x10F1)
		case ERROR_INDIGENOUS_TYPE: return  PXActionInvalid; // The media type cannot be removed from this library since at least one drive in the library reports it can support this media type. 4338 (0x10F2)
		case ERROR_NO_SUPPORTING_DRIVES: return  PXActionInvalid; // This offline media cannot be mounted on this system since no enabled drives are present which can be used. 4339 (0x10F3)
		case ERROR_CLEANER_CARTRIDGE_INSTALLED: return  PXActionInvalid; // A cleaner cartridge is present in the tape library. 4340 (0x10F4)
		case ERROR_IEPORT_FULL: return  PXActionInvalid; // Cannot use the inject/eject port because it is not empty. 4341 (0x10F5)
		case ERROR_FILE_OFFLINE: return  PXActionInvalid; // This file is currently not available for use on this computer. 4350 (0x10FE)
		case ERROR_REMOTE_STORAGE_NOT_ACTIVE: return  PXActionInvalid; // The remote storage service is not operational at this time. 4351 (0x10FF)
		case ERROR_REMOTE_STORAGE_MEDIA_ERROR: return  PXActionInvalid; // The remote storage service encountered a media error. 4352 (0x1100)
		case ERROR_NOT_A_REPARSE_POINT: return  PXActionInvalid; // The file or directory is not a reparse point. 4390 (0x1126)
		case ERROR_REPARSE_ATTRIBUTE_CONFLICT: return  PXActionInvalid; // The reparse point attribute cannot be set because it conflicts with an existing attribute. 4391 (0x1127)
		case ERROR_INVALID_REPARSE_DATA: return  PXActionInvalid; // The data present in the reparse point buffer is invalid. 4392 (0x1128)
		case ERROR_REPARSE_TAG_INVALID: return  PXActionInvalid; // The tag present in the reparse point buffer is invalid. 4393 (0x1129)
		case ERROR_REPARSE_TAG_MISMATCH: return  PXActionInvalid; // There is a mismatch between the tag specified in the request and the tag present in the reparse point. 4394 (0x112A)
		
#if WindowsAtleastVista
		case ERROR_APP_DATA_NOT_FOUND: return  PXActionInvalid; // Fast Cache data not found. 4400 (0x1130)
		case ERROR_APP_DATA_EXPIRED: return  PXActionInvalid; // Fast Cache data expired. 4401 (0x1131)
		case ERROR_APP_DATA_CORRUPT: return  PXActionInvalid; // Fast Cache data corrupt. 4402 (0x1132)
		case ERROR_APP_DATA_LIMIT_EXCEEDED: return  PXActionInvalid; // Fast Cache data has exceeded its max size and cannot be updated. 4403 (0x1133)
		case ERROR_APP_DATA_REBOOT_REQUIRED: return  PXActionInvalid; // Fast Cache has been ReArmed and requires a reboot until it can be updated. 4404 (0x1134)
		case ERROR_SECUREBOOT_ROLLBACK_DETECTED: return  PXActionInvalid; // Secure Boot detected that rollback of protected data has been attempted. 4420 (0x1144)
		case ERROR_SECUREBOOT_POLICY_VIOLATION: return  PXActionInvalid; // The value is protected by Secure Boot policy and cannot be modified or deleted. 4421 (0x1145)
		case ERROR_SECUREBOOT_INVALID_POLICY: return  PXActionInvalid; // The Secure Boot policy is invalid. 4422 (0x1146)
		case ERROR_SECUREBOOT_POLICY_PUBLISHER_NOT_FOUND: return  PXActionInvalid; // A new Secure Boot policy did not contain the current publisher on its update list. 4423 (0x1147)
		case ERROR_SECUREBOOT_POLICY_NOT_SIGNED: return  PXActionInvalid; // The Secure Boot policy is either not signed or is signed by a non-trusted signer. 4424 (0x1148)
		case ERROR_SECUREBOOT_NOT_ENABLED: return  PXActionInvalid; // Secure Boot is not enabled on this machine. 4425 (0x1149)
		case ERROR_SECUREBOOT_FILE_REPLACED: return  PXActionInvalid; // Secure Boot requires that certain files and drivers are not replaced by other files or drivers. 4426 (0x114A)
		case ERROR_OFFLOAD_READ_FLT_NOT_SUPPORTED: return  PXActionInvalid; // The copy offload read operation is not supported by a filter. 4440 (0x1158)
		case ERROR_OFFLOAD_WRITE_FLT_NOT_SUPPORTED: return  PXActionInvalid; // The copy offload write operation is not supported by a filter. 4441 (0x1159)
		case ERROR_OFFLOAD_READ_FILE_NOT_SUPPORTED: return  PXActionInvalid; // The copy offload read operation is not supported for the file. 4442 (0x115A)
		case ERROR_OFFLOAD_WRITE_FILE_NOT_SUPPORTED: return  PXActionInvalid; // The copy offload write operation is not supported for the file. 4443 (0x115B)
#endif
		
		case ERROR_VOLUME_NOT_SIS_ENABLED: return  PXActionInvalid; // Single Instance Storage is not available on this volume. 4500 (0x1194)
		case ERROR_DEPENDENT_RESOURCE_EXISTS: return  PXActionInvalid; // The operation cannot be completed because other resources are dependent on this resource. 5001 (0x1389)
		case ERROR_DEPENDENCY_NOT_FOUND: return  PXActionInvalid; // The cluster resource dependency cannot be found. 5002 (0x138A)
		case ERROR_DEPENDENCY_ALREADY_EXISTS: return  PXActionInvalid; // The cluster resource cannot be made dependent on the specified resource because it is already dependent. 5003 (0x138B)
		case ERROR_RESOURCE_NOT_ONLINE: return  PXActionInvalid; // The cluster resource is not online. 5004 (0x138C)
		case ERROR_HOST_NODE_NOT_AVAILABLE: return  PXActionInvalid; // A cluster node is not available for this operation. 5005 (0x138D)
		case ERROR_RESOURCE_NOT_AVAILABLE: return  PXActionInvalid; // The cluster resource is not available. 5006 (0x138E)
		case ERROR_RESOURCE_NOT_FOUND: return  PXActionInvalid; // The cluster resource could not be found. 5007 (0x138F)
		case ERROR_SHUTDOWN_CLUSTER: return  PXActionInvalid; // The cluster is being shut down. 5008 (0x1390)
		case ERROR_CANT_EVICT_ACTIVE_NODE: return  PXActionInvalid; // A cluster node cannot be evicted from the cluster unless the node is down or it is the last node. 5009 (0x1391)
		case ERROR_OBJECT_ALREADY_EXISTS: return  PXActionInvalid; // The object already exists. 5010 (0x1392)
		case ERROR_OBJECT_IN_LIST: return  PXActionInvalid; // The object is already in the list. 5011 (0x1393)
		case ERROR_GROUP_NOT_AVAILABLE: return  PXActionInvalid; // The cluster group is not available for any new requests. 5012 (0x1394)
		case ERROR_GROUP_NOT_FOUND: return  PXActionInvalid; // The cluster group could not be found. 5013 (0x1395)
		case ERROR_GROUP_NOT_ONLINE: return  PXActionInvalid; // The operation could not be completed because the cluster group is not online. 5014 (0x1396)
		case ERROR_HOST_NODE_NOT_RESOURCE_OWNER: return  PXActionInvalid; // The operation failed because either the specified cluster node is not the owner of the resource, or the node is not a possible owner of the resource. 5015 (0x1397)
		case ERROR_HOST_NODE_NOT_GROUP_OWNER: return  PXActionInvalid; // The operation failed because either the specified cluster node is not the owner of the group, or the node is not a possible owner of the group. 5016 (0x1398)
		case ERROR_RESMON_CREATE_FAILED: return  PXActionInvalid; // The cluster resource could not be created in the specified resource monitor. 5017 (0x1399)
		case ERROR_RESMON_ONLINE_FAILED: return  PXActionInvalid; // The cluster resource could not be brought online by the resource monitor. 5018 (0x139A)
		case ERROR_RESOURCE_ONLINE: return  PXActionInvalid; // The operation could not be completed because the cluster resource is online. 5019 (0x139B)
		case ERROR_QUORUM_RESOURCE: return  PXActionInvalid; // The cluster resource could not be deleted or brought offline because it is the quorum resource. 5020 (0x139C)
		case ERROR_NOT_QUORUM_CAPABLE: return  PXActionInvalid; // The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource. 5021 (0x139D)
		case ERROR_CLUSTER_SHUTTING_DOWN: return  PXActionInvalid; // The cluster software is shutting down. 5022 (0x139E)
		case ERROR_INVALID_STATE: return  PXActionInvalid; // The group or resource is not in the correct state to perform the requested operation. 5023 (0x139F)
		case ERROR_RESOURCE_PROPERTIES_STORED: return  PXActionInvalid; // The properties were stored but not all changes will take effect until the next time the resource is brought online. 5024 (0x13A0)
		case ERROR_NOT_QUORUM_CLASS: return  PXActionInvalid; // The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class. 5025 (0x13A1)
		case ERROR_CORE_RESOURCE: return  PXActionInvalid; // The cluster resource could not be deleted since it is a core resource. 5026 (0x13A2)
		case ERROR_QUORUM_RESOURCE_ONLINE_FAILED: return  PXActionInvalid; // The quorum resource failed to come online. 5027 (0x13A3)
		case ERROR_QUORUMLOG_OPEN_FAILED: return  PXActionInvalid; // The quorum log could not be created or mounted successfully. 5028 (0x13A4)
		case ERROR_CLUSTERLOG_CORRUPT: return  PXActionInvalid; // The cluster log is corrupt. 5029 (0x13A5)
		case ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE: return  PXActionInvalid; // The record could not be written to the cluster log since it exceeds the maximum size. 5030 (0x13A6)
		case ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE: return  PXActionInvalid; // The cluster log exceeds its maximum size. 5031 (0x13A7)
		case ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND: return  PXActionInvalid; // No checkpoint record was found in the cluster log. 5032 (0x13A8)
		case ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE: return  PXActionInvalid; // The minimum required disk space needed for logging is not available. 5033 (0x13A9)
		case ERROR_QUORUM_OWNER_ALIVE: return  PXActionInvalid; // The cluster node failed to take control of the quorum resource because the resource is owned by another active node. 5034 (0x13AA)
		case ERROR_NETWORK_NOT_AVAILABLE: return  PXActionInvalid; // A cluster network is not available for this operation. 5035 (0x13AB)
		case ERROR_NODE_NOT_AVAILABLE: return  PXActionInvalid; // A cluster node is not available for this operation. 5036 (0x13AC)
		case ERROR_ALL_NODES_NOT_AVAILABLE: return  PXActionInvalid; // All cluster nodes must be running to perform this operation. 5037 (0x13AD)
		case ERROR_RESOURCE_FAILED: return  PXActionInvalid; // A cluster resource failed. 5038 (0x13AE)
		case ERROR_CLUSTER_INVALID_NODE: return  PXActionInvalid; // The cluster node is not valid. 5039 (0x13AF)
		case ERROR_CLUSTER_NODE_EXISTS: return  PXActionInvalid; // The cluster node already exists. 5040 (0x13B0)
		case ERROR_CLUSTER_JOIN_IN_PROGRESS: return  PXActionInvalid; // A node is in the process of joining the cluster. 5041 (0x13B1)
		case ERROR_CLUSTER_NODE_NOT_FOUND: return  PXActionInvalid; // The cluster node was not found. 5042 (0x13B2)
		case ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND: return  PXActionInvalid; // The cluster local node information was not found. 5043 (0x13B3)
		case ERROR_CLUSTER_NETWORK_EXISTS: return  PXActionInvalid; // The cluster network already exists. 5044 (0x13B4)
		case ERROR_CLUSTER_NETWORK_NOT_FOUND: return  PXActionInvalid; // The cluster network was not found. 5045 (0x13B5)
		case ERROR_CLUSTER_NETINTERFACE_EXISTS: return  PXActionInvalid; // The cluster network interface already exists. 5046 (0x13B6)
		case ERROR_CLUSTER_NETINTERFACE_NOT_FOUND: return  PXActionInvalid; // The cluster network interface was not found. 5047 (0x13B7)
		case ERROR_CLUSTER_INVALID_REQUEST: return  PXActionInvalid; // The cluster request is not valid for this object. 5048 (0x13B8)
		case ERROR_CLUSTER_INVALID_NETWORK_PROVIDER: return  PXActionInvalid; // The cluster network provider is not valid. 5049 (0x13B9)
		case ERROR_CLUSTER_NODE_DOWN: return  PXActionInvalid; // The cluster node is down. 5050 (0x13BA)
		case ERROR_CLUSTER_NODE_UNREACHABLE: return  PXActionInvalid; // The cluster node is not reachable. 5051 (0x13BB)
		case ERROR_CLUSTER_NODE_NOT_MEMBER: return  PXActionInvalid; // The cluster node is not a member of the cluster. 5052 (0x13BC)
		case ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS: return  PXActionInvalid; // A cluster join operation is not in progress. 5053 (0x13BD)
		case ERROR_CLUSTER_INVALID_NETWORK: return  PXActionInvalid; // The cluster network is not valid. 5054 (0x13BE)
		case ERROR_CLUSTER_NODE_UP: return  PXActionInvalid; // The cluster node is up. 5056 (0x13C0)
		case ERROR_CLUSTER_IPADDR_IN_USE: return  PXActionInvalid; // The cluster IP address is already in use. 5057 (0x13C1)
		case ERROR_CLUSTER_NODE_NOT_PAUSED: return  PXActionInvalid; // The cluster node is not paused. 5058 (0x13C2)
		case ERROR_CLUSTER_NO_SECURITY_CONTEXT: return  PXActionInvalid; // No cluster security context is available. 5059 (0x13C3)
		case ERROR_CLUSTER_NETWORK_NOT_INTERNAL: return  PXActionInvalid; // The cluster network is not configured for internal cluster communication. 5060 (0x13C4)
		case ERROR_CLUSTER_NODE_ALREADY_UP: return  PXActionInvalid; // The cluster node is already up. 5061 (0x13C5)
		case ERROR_CLUSTER_NODE_ALREADY_DOWN: return  PXActionInvalid; // The cluster node is already down. 5062 (0x13C6)
		case ERROR_CLUSTER_NETWORK_ALREADY_ONLINE: return  PXActionInvalid; // The cluster network is already online. 5063 (0x13C7)
		case ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE: return  PXActionInvalid; // The cluster network is already offline. 5064 (0x13C8)
		case ERROR_CLUSTER_NODE_ALREADY_MEMBER: return  PXActionInvalid; // The cluster node is already a member of the cluster. 5065 (0x13C9)
		case ERROR_CLUSTER_LAST_INTERNAL_NETWORK: return  PXActionInvalid; // The cluster network is the only one configured for internal cluster communication between two or more active cluster nodes. The internal communication capability cannot be removed from the network. 5066 (0x13CA)
		case ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS: return  PXActionInvalid; // One or more cluster resources depend on the network to provide service to clients. The client access capability cannot be removed from the network. 5067 (0x13CB)
		case ERROR_INVALID_OPERATION_ON_QUORUM: return  PXActionInvalid; // This operation cannot be performed on the cluster resource as it the quorum resource. You may not bring the quorum resource offline or modify its possible owners list. 5068 (0x13CC)
		case ERROR_DEPENDENCY_NOT_ALLOWED: return  PXActionInvalid; // The cluster quorum resource is not allowed to have any dependencies. 5069 (0x13CD)
		case ERROR_CLUSTER_NODE_PAUSED: return  PXActionInvalid; // The cluster node is paused. 5070 (0x13CE)
		case ERROR_NODE_CANT_HOST_RESOURCE: return  PXActionInvalid; // The cluster resource cannot be brought online. The owner node cannot run this resource. 5071 (0x13CF)
		case ERROR_CLUSTER_NODE_NOT_READY: return  PXActionInvalid; // The cluster node is not ready to perform the requested operation. 5072 (0x13D0)
		case ERROR_CLUSTER_NODE_SHUTTING_DOWN: return  PXActionInvalid; // The cluster node is shutting down. 5073 (0x13D1)
		case ERROR_CLUSTER_JOIN_ABORTED: return  PXActionInvalid; // The cluster join operation was aborted. 5074 (0x13D2)
		case ERROR_CLUSTER_INCOMPATIBLE_VERSIONS: return  PXActionInvalid; // The cluster join operation failed due to incompatible software versions between the joining node and its sponsor. 5075 (0x13D3)
		case ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED: return  PXActionInvalid; // This resource cannot be created because the cluster has reached the limit on the number of resources it can monitor. 5076 (0x13D4)
		case ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED: return  PXActionInvalid; // The system configuration changed during the cluster join or form operation. The join or form operation was aborted. 5077 (0x13D5)
		case ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND: return  PXActionInvalid; // The specified resource type was not found. 5078 (0x13D6)
		case ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED: return  PXActionInvalid; // The specified node does not support a resource of this type. This may be due to version inconsistencies or due to the absence of the resource DLL on this node. 5079 (0x13D7)
		case ERROR_CLUSTER_RESNAME_NOT_FOUND: return  PXActionInvalid; // The specified resource name is not supported by this resource DLL. This may be due to a bad (or changed) name supplied to the resource DLL. 5080 (0x13D8)
		case ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED: return  PXActionInvalid; // No authentication package could be registered with the RPC server. 5081 (0x13D9)
		case ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST: return  PXActionInvalid; // You cannot bring the group online because the owner of the group is not in the preferred list for the group. To change the owner node for the group, move the group. 5082 (0x13DA)
		case ERROR_CLUSTER_DATABASE_SEQMISMATCH: return  PXActionInvalid; // The join operation failed because the cluster database sequence number has changed or is incompatible with the locker node. This may happen during a join operation if the cluster database was changing during the join. 5083 (0x13DB)
		case ERROR_RESMON_INVALID_STATE: return  PXActionInvalid; // The resource monitor will not allow the fail operation to be performed while the resource is in its current state. This may happen if the resource is in a pending state. 5084 (0x13DC)
		case ERROR_CLUSTER_GUM_NOT_LOCKER: return  PXActionInvalid; // A non locker code got a request to reserve the lock for making global updates. 5085 (0x13DD)
		case ERROR_QUORUM_DISK_NOT_FOUND: return  PXActionInvalid; // The quorum disk could not be located by the cluster service. 5086 (0x13DE)
		case ERROR_DATABASE_BACKUP_CORRUPT: return  PXActionInvalid; // The backed up cluster database is possibly corrupt. 5087 (0x13DF)
		case ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT: return  PXActionInvalid; // A DFS root already exists in this cluster node. 5088 (0x13E0)
		case ERROR_RESOURCE_PROPERTY_UNCHANGEABLE: return  PXActionInvalid; // An attempt to modify a resource property failed because it conflicts with another existing property. 5089 (0x13E1)
		case ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE: return  PXActionInvalid; // An operation was attempted that is incompatible with the current membership state of the node. 5890 (0x1702)
		case ERROR_CLUSTER_QUORUMLOG_NOT_FOUND: return  PXActionInvalid; // The quorum resource does not contain the quorum log. 5891 (0x1703)
		case ERROR_CLUSTER_MEMBERSHIP_HALT: return  PXActionInvalid; // The membership engine requested shutdown of the cluster service on this node. 5892 (0x1704)
		case ERROR_CLUSTER_INSTANCE_ID_MISMATCH: return  PXActionInvalid; // The join operation failed because the cluster instance ID of the joining node does not match the cluster instance ID of the sponsor node. 5893 (0x1705)
		case ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP: return  PXActionInvalid; // A matching cluster network for the specified IP address could not be found. 5894 (0x1706)
		case ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH: return  PXActionInvalid; // The actual data type of the property did not match the expected data type of the property. 5895 (0x1707)
		case ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP: return  PXActionInvalid; // The cluster node was evicted from the cluster successfully, but the node was not cleaned up. To determine what cleanup steps failed and how to recover, see the Failover Clustering application event log using Event Viewer. 5896 (0x1708)
		case ERROR_CLUSTER_PARAMETER_MISMATCH: return  PXActionInvalid; // Two or more parameter values specified for a resource's properties are in conflict. 5897 (0x1709)
		case ERROR_NODE_CANNOT_BE_CLUSTERED: return  PXActionInvalid; // This computer cannot be made a member of a cluster. 5898 (0x170A)
		case ERROR_CLUSTER_WRONG_OS_VERSION: return  PXActionInvalid; // This computer cannot be made a member of a cluster because it does not have the correct version of Windows installed. 5899 (0x170B)
		case ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME: return  PXActionInvalid; // A cluster cannot be created with the specified cluster name because that cluster name is already in use. Specify a different name for the cluster. 5900 (0x170C)
		case ERROR_CLUSCFG_ALREADY_COMMITTED: return  PXActionInvalid; // The cluster configuration action has already been committed. 5901 (0x170D)
		case ERROR_CLUSCFG_ROLLBACK_FAILED: return  PXActionInvalid; // The cluster configuration action could not be rolled back. 5902 (0x170E)
		case ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT: return  PXActionInvalid; // The drive letter assigned to a system disk on one node conflicted with the drive letter assigned to a disk on another node. 5903 (0x170F)
		case ERROR_CLUSTER_OLD_VERSION: return  PXActionInvalid; // One or more nodes in the cluster are running a version of Windows that does not support this operation. 5904 (0x1710)
		case ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME: return  PXActionInvalid; // The name of the corresponding computer account doesn't match the Network Name for this resource. 5905 (0x1711)
		case ERROR_CLUSTER_NO_NET_ADAPTERS: return  PXActionInvalid; // No network adapters are available. 5906 (0x1712)
		case ERROR_CLUSTER_POISONED: return  PXActionInvalid; // The cluster node has been poisoned. 5907 (0x1713)
		case ERROR_CLUSTER_GROUP_MOVING: return  PXActionInvalid; // The group is unable to accept the request since it is moving to another node. 5908 (0x1714)
		case ERROR_CLUSTER_RESOURCE_TYPE_BUSY: return  PXActionInvalid; // The resource type cannot accept the request since is too busy performing another operation. 5909 (0x1715)
		case ERROR_RESOURCE_CALL_TIMED_OUT: return  PXActionInvalid; // The call to the cluster resource DLL timed out. 5910 (0x1716)
		case ERROR_INVALID_CLUSTER_IPV6_ADDRESS: return  PXActionInvalid; // The address is not valid for an IPv6 Address resource. A global IPv6 address is required, and it must match a cluster network. Compatibility addresses are not permitted. 5911 (0x1717)
		case ERROR_CLUSTER_INTERNAL_INVALID_FUNCTION: return  PXActionInvalid; // An internal cluster error occurred. A call to an invalid function was attempted. 5912 (0x1718)
		case ERROR_CLUSTER_PARAMETER_OUT_OF_BOUNDS: return  PXActionInvalid; // A parameter value is out of acceptable range. 5913 (0x1719)
		case ERROR_CLUSTER_PARTIAL_SEND: return  PXActionInvalid; // A network error occurred while sending data to another node in the cluster. The number of bytes transmitted was less than required. 5914 (0x171A)
		case ERROR_CLUSTER_REGISTRY_INVALID_FUNCTION: return  PXActionInvalid; // An invalid cluster registry operation was attempted. 5915 (0x171B)
		case ERROR_CLUSTER_INVALID_STRING_TERMINATION: return  PXActionInvalid; // An input string of characters is not properly terminated. 5916 (0x171C)
		case ERROR_CLUSTER_INVALID_STRING_FORMAT: return  PXActionInvalid; // An input string of characters is not in a valid format for the data it represents. 5917 (0x171D)
		case ERROR_CLUSTER_DATABASE_TRANSACTION_IN_PROGRESS: return  PXActionInvalid; // An internal cluster error occurred. A cluster database transaction was attempted while a transaction was already in progress. 5918 (0x171E)
		case ERROR_CLUSTER_DATABASE_TRANSACTION_NOT_IN_PROGRESS: return  PXActionInvalid; // An internal cluster error occurred. There was an attempt to commit a cluster database transaction while no transaction was in progress. 5919 (0x171F)
		case ERROR_CLUSTER_NULL_DATA: return  PXActionInvalid; // An internal cluster error occurred. Data was not properly initialized. 5920 (0x1720)
		case ERROR_CLUSTER_PARTIAL_READ: return  PXActionInvalid; // An error occurred while reading from a stream of data. An unexpected number of bytes was returned. 5921 (0x1721)
		case ERROR_CLUSTER_PARTIAL_WRITE: return  PXActionInvalid; // An error occurred while writing to a stream of data. The required number of bytes could not be written. 5922 (0x1722)
		case ERROR_CLUSTER_CANT_DESERIALIZE_DATA: return  PXActionInvalid; // An error occurred while deserializing a stream of cluster data. 5923 (0x1723)
		case ERROR_DEPENDENT_RESOURCE_PROPERTY_CONFLICT: return  PXActionInvalid; // One or more property values for this resource are in conflict with one or more property values associated with its dependent resource(s). 5924 (0x1724)
		case ERROR_CLUSTER_NO_QUORUM: return  PXActionInvalid; // A quorum of cluster nodes was not present to form a cluster. 5925 (0x1725)
		case ERROR_CLUSTER_INVALID_IPV6_NETWORK: return  PXActionInvalid; // The cluster network is not valid for an IPv6 Address resource, or it does not match the configured address. 5926 (0x1726)
		case ERROR_CLUSTER_INVALID_IPV6_TUNNEL_NETWORK: return  PXActionInvalid; // The cluster network is not valid for an IPv6 Tunnel resource. Check the configuration of the IP Address resource on which the IPv6 Tunnel resource depends. 5927 (0x1727)
		case ERROR_QUORUM_NOT_ALLOWED_IN_THIS_GROUP: return  PXActionInvalid; // Quorum resource cannot reside in the Available Storage group. 5928 (0x1728)
		case ERROR_DEPENDENCY_TREE_TOO_COMPLEX: return  PXActionInvalid; // The dependencies for this resource are nested too deeply. 5929 (0x1729)
		case ERROR_EXCEPTION_IN_RESOURCE_CALL: return  PXActionInvalid; // The call into the resource DLL raised an unhandled exception. 5930 (0x172A)
		case ERROR_CLUSTER_RHS_FAILED_INITIALIZATION: return  PXActionInvalid; // The RHS process failed to initialize. 5931 (0x172B)
		case ERROR_CLUSTER_NOT_INSTALLED: return  PXActionInvalid; // The Failover Clustering feature is not installed on this node. 5932 (0x172C)
		case ERROR_CLUSTER_RESOURCES_MUST_BE_ONLINE_ON_THE_SAME_NODE: return  PXActionInvalid; // The resources must be online on the same node for this operation. 5933 (0x172D)
		case ERROR_CLUSTER_MAX_NODES_IN_CLUSTER: return  PXActionInvalid; // A new node can not be added since this cluster is already at its maximum number of nodes. 5934 (0x172E)
		case ERROR_CLUSTER_TOO_MANY_NODES: return  PXActionInvalid; // This cluster can not be created since the specified number of nodes exceeds the maximum allowed limit. 5935 (0x172F)
		case ERROR_CLUSTER_OBJECT_ALREADY_USED: return  PXActionInvalid; // An attempt to use the specified cluster name failed because an enabled computer object with the given name already exists in the domain. 5936 (0x1730)
		case ERROR_NONCORE_GROUPS_FOUND: return  PXActionInvalid; // This cluster cannot be destroyed. It has non-core application groups which must be deleted before the cluster can be destroyed. 5937 (0x1731)
		case ERROR_FILE_SHARE_RESOURCE_CONFLICT: return  PXActionInvalid; // File share associated with file share witness resource cannot be hosted by this cluster or any of its nodes. 5938 (0x1732)
		case ERROR_CLUSTER_EVICT_INVALID_REQUEST: return  PXActionInvalid; // Eviction of this node is invalid at this time. Due to quorum requirements node eviction will result in cluster shutdown. If it is the last node in the cluster, destroy cluster command should be used. 5939 (0x1733)
		case ERROR_CLUSTER_SINGLETON_RESOURCE: return  PXActionInvalid; // Only one instance of this resource type is allowed in the cluster. 5940 (0x1734)
		case ERROR_CLUSTER_GROUP_SINGLETON_RESOURCE: return  PXActionInvalid; // Only one instance of this resource type is allowed per resource group. 5941 (0x1735)
		case ERROR_CLUSTER_RESOURCE_PROVIDER_FAILED: return  PXActionInvalid; // The resource failed to come online due to the failure of one or more provider resources. 5942 (0x1736)
		case ERROR_CLUSTER_RESOURCE_CONFIGURATION_ERROR: return  PXActionInvalid; // The resource has indicated that it cannot come online on any node. 5943 (0x1737)
		case ERROR_CLUSTER_GROUP_BUSY: return  PXActionInvalid; // The current operation cannot be performed on this group at this time. 5944 (0x1738)
		case ERROR_CLUSTER_NOT_SHARED_VOLUME: return  PXActionInvalid; // The directory or file is not located on a cluster shared volume. 5945 (0x1739)
		case ERROR_CLUSTER_INVALID_SECURITY_DESCRIPTOR: return  PXActionInvalid; // The Security Descriptor does not meet the requirements for a cluster. 5946 (0x173A)
		case ERROR_CLUSTER_SHARED_VOLUMES_IN_USE: return  PXActionInvalid; // There is one or more shared volumes resources configured in the cluster. Those resources must be moved to available storage in order for operation to succeed. 5947 (0x173B)
		case ERROR_CLUSTER_USE_SHARED_VOLUMES_API: return  PXActionInvalid; // This group or resource cannot be directly manipulated. Use shared volume APIs to perform desired operation. 5948 (0x173C)
		case ERROR_CLUSTER_BACKUP_IN_PROGRESS: return  PXActionInvalid; // Back up is in progress. Please wait for backup completion before trying this operation again. 5949 (0x173D)
		case ERROR_NON_CSV_PATH: return  PXActionInvalid; // The path does not belong to a cluster shared volume. 5950 (0x173E)
		case ERROR_CSV_VOLUME_NOT_LOCAL: return  PXActionInvalid; // The cluster shared volume is not locally mounted on this node. 5951 (0x173F)
		case ERROR_CLUSTER_WATCHDOG_TERMINATING: return  PXActionInvalid; // The cluster watchdog is terminating. 5952 (0x1740)
	
#if WindowsAtleastVista
		case ERROR_CLUSTER_RESOURCE_VETOED_MOVE_INCOMPATIBLE_NODES: return  PXActionInvalid; // A resource vetoed a move between two nodes because they are incompatible. 5953 (0x1741)
		case ERROR_CLUSTER_INVALID_NODE_WEIGHT: return  PXActionInvalid; // The request is invalid either because node weight cannot be changed while the cluster is in disk-only quorum mode, or because changing the node weight would violate the minimum cluster quorum requirements. 5954 (0x1742)
		case ERROR_CLUSTER_RESOURCE_VETOED_CALL: return  PXActionInvalid; // The resource vetoed the call. 5955 (0x1743)
		case ERROR_RESMON_SYSTEM_RESOURCES_LACKING: return  PXActionInvalid; // Resource could not start or run because it could not reserve sufficient system resources. 5956 (0x1744)
		case ERROR_CLUSTER_RESOURCE_VETOED_MOVE_NOT_ENOUGH_RESOURCES_ON_DESTINATION: return  PXActionInvalid; // A resource vetoed a move between two nodes because the destination currently does not have enough resources to complete the operation. 5957 (0x1745)
		case ERROR_CLUSTER_RESOURCE_VETOED_MOVE_NOT_ENOUGH_RESOURCES_ON_SOURCE: return  PXActionInvalid; // A resource vetoed a move between two nodes because the source currently does not have enough resources to complete the operation. 5958 (0x1746)
		case ERROR_CLUSTER_GROUP_QUEUED: return  PXActionInvalid; // The requested operation can not be completed because the group is queued for an operation. 5959 (0x1747)
		case ERROR_CLUSTER_RESOURCE_LOCKED_STATUS: return  PXActionInvalid; // The requested operation can not be completed because a resource has locked status. 5960 (0x1748)
		case ERROR_CLUSTER_SHARED_VOLUME_FAILOVER_NOT_ALLOWED: return  PXActionInvalid; // The resource cannot move to another node because a cluster shared volume vetoed the operation. 5961 (0x1749)
		case ERROR_CLUSTER_NODE_DRAIN_IN_PROGRESS: return  PXActionInvalid; // A node drain is already in progress. 5962 (0x174A)
		case ERROR_CLUSTER_DISK_NOT_CONNECTED: return  PXActionInvalid; // Clustered storage is not connected to the node. 5963 (0x174B)
		case ERROR_DISK_NOT_CSV_CAPABLE: return  PXActionInvalid; // The disk is not configured in a way to be used with CSV. CSV disks must have at least one partition that is formatted with NTFS.  5964 (0x174C) 
		case ERROR_RESOURCE_NOT_IN_AVAILABLE_STORAGE: return  PXActionInvalid; // The resource must be part of the Available Storage group to complete this action. 5965 (0x174D)
		case ERROR_CLUSTER_SHARED_VOLUME_REDIRECTED: return  PXActionInvalid; // CSVFS failed operation as volume is in redirected mode. 5966 (0x174E)
		case ERROR_CLUSTER_SHARED_VOLUME_NOT_REDIRECTED: return  PXActionInvalid; // CSVFS failed operation as volume is not in redirected mode. 5967 (0x174F)
		case ERROR_CLUSTER_CANNOT_RETURN_PROPERTIES: return  PXActionInvalid; // Cluster properties cannot be returned at this time. 5968 (0x1750)
		case ERROR_CLUSTER_RESOURCE_CONTAINS_UNSUPPORTED_DIFF_AREA_FOR_SHARED_VOLUMES: return  PXActionInvalid; // The clustered disk resource contains software snapshot diff area that are not supported for Cluster Shared Volumes. 5969 (0x1751)
		case ERROR_CLUSTER_RESOURCE_IS_IN_MAINTENANCE_MODE: return  PXActionInvalid; // The operation cannot be completed because the resource is in maintenance mode. 5970 (0x1752)
		case ERROR_CLUSTER_AFFINITY_CONFLICT: return  PXActionInvalid; // The operation cannot be completed because of cluster affinity conflicts. 5971 (0x1753)
		case ERROR_CLUSTER_RESOURCE_IS_REPLICA_VIRTUAL_MACHINE: return  PXActionInvalid; // The operation cannot be completed because the resource is a replica virtual machine. 5972 (0x1754)
#endif

		// 
		// 
		// System Error Codes(6000 - 8199) (0x1770 - 0x2007)
		// System Error Codes(8200 - 8999) (0x2008 - 0x2327)
		// System Error Codes(9000 - 11999) (0x2328 - 0x2edf)
		// System Error Codes(12000 - 15999) (0x2ee0 - 0x3e7f)




		//-------------------------------------------------
		// 0x80041068 - 0x80041099	Errors that originate in WMI itself.
		// 0x8007xxxx	Errors originating in the core operating system.WMI may return this type of error because of an external failure, for example, DCOM security failure.
		// 0x80040xxx	Errors originating in DCOM.For example, the DCOM configuration for operations to a remote computer may be incorrect.
		// 0x8005xxxx	Error originating from ADSI(Active Directory Service Interfaces) or LDAP(Lightweight Directory Access Protocol), for example, an Active Directory access failure when using the WMI Active Directory providers.
		// Some methods in WMI classes can return system and network error codes(64 for example).You can check the definition of these types of error codes by using the net helpmsg command in the command prompt window.For example, the command net helpmsg 64 returns the message : The specified network name is no longer available.In C++, you can call FormatMessage and specify C : \Windows\System32\wbem\wmiutils.dll as the message module.
		//-------------------------------------------------
		case 	WBEM_E_FAILED: return PXActionFailedUnkownError; //	Call failed.	2147749889 (0x80041001)
		case 	WBEM_E_NOT_FOUND: return PXActionRefusedNotFound; //	Object cannot be found.	2147749890 (0x80041002)
		case 	WBEM_E_ACCESS_DENIED: return PXActionDeniedNoPermission; //	Current user does not have permission to perform the action.	2147749891 (0x80041003)
		case 	WBEM_E_PROVIDER_FAILURE: return PXActionInvalid; //	Provider has failed at some time other than during initialization.	2147749892 (0x80041004)
		case 	WBEM_E_TYPE_MISMATCH: return PXActionRefusedTypeMissmatch; //	Type mismatch occurred.	2147749893 (0x80041005)
		case 	WBEM_E_OUT_OF_MEMORY: return PXActionFailedMemoryAllocation; //	Not enough memory for the operation.	2147749894 (0x80041006)
		case 	WBEM_E_INVALID_CONTEXT: return PXActionInvalid; //	The IWbemContext object is not valid.	2147749895 (0x80041007)
		case 	WBEM_E_INVALID_PARAMETER: return PXActionInvalid; //	One of the parameters to the call is not correct.	2147749896 (0x80041008)
		case 	WBEM_E_NOT_AVAILABLE: return PXActionInvalid; //	Resource, typically a remote server, is not currently available.	2147749897 (0x80041009)
		case 	WBEM_E_CRITICAL_ERROR: return PXActionInvalid; //	Internal, critical, and unexpected error occurred. Report the error to Microsoft Technical Support.	2147749898 (0x8004100A)
		case 	WBEM_E_INVALID_STREAM: return PXActionInvalid; //	One or more network packets were corrupted during a remote session.	2147749899 (0x8004100B)
		case 	WBEM_E_NOT_SUPPORTED: PXActionInvalid;//return PXActionRefusedNotSupported; //	Feature or operation is not supported.	2147749900 (0x8004100C)
		case 	WBEM_E_INVALID_SUPERCLASS: return PXActionRefusedObjectParentInvalid; //	Parent class specified is not valid.	2147749901 (0x8004100D)
		case 	WBEM_E_INVALID_NAMESPACE: return PXActionRefusedNameSpaceInvalid; //	Namespace specified cannot be found.	2147749902 (0x8004100E)
		case 	WBEM_E_INVALID_OBJECT: return PXActionRefusedObjectNameInvalid; //	Specified instance is not valid.	2147749903 (0x8004100F)
		case 	WBEM_E_INVALID_CLASS: return PXActionRefusedWindowsClassNameInvalid; //	Specified class is not valid.	2147749904 (0x80041010)
		case 	WBEM_E_PROVIDER_NOT_FOUND: return PXActionInvalid; //	Provider referenced in the schema does not have a corresponding registration.	2147749905 (0x80041011)
		case 	WBEM_E_INVALID_PROVIDER_REGISTRATION: return PXActionInvalid; //	Provider referenced in the schema has an incorrect or incomplete registration.	2147749906
		case 	WBEM_E_PROVIDER_LOAD_FAILURE: return PXActionInvalid; //	COM cannot locate a provider referenced in the schema.	2147749907 (0x80041013)
		case 	WBEM_E_INITIALIZATION_FAILURE: return PXActionInvalid; //	Component, such as a provider, failed to initialize for internal reasons.	2147749908 (0x80041014)
		case 	WBEM_E_TRANSPORT_FAILURE: return PXActionInvalid; //	Networking error that prevents normal operation has occurred.	2147749909 (0x80041015)
		case 	WBEM_E_INVALID_OPERATION: return PXActionInvalid; //	Requested operation is not valid. This error usually applies to invalid attempts to delete classes or properties.	2147749910 (0x80041016)
		case 	WBEM_E_INVALID_QUERY: return PXActionInvalid; //	Query was not syntactically valid.	2147749911 (0x80041017)
		case 	WBEM_E_INVALID_QUERY_TYPE: return PXActionInvalid; //	Requested query language is not supported.	2147749912 (0x80041018)
		case 	WBEM_E_ALREADY_EXISTS: return PXActionInvalid; //	In a put operation, the wbemChangeFlagCreateOnly flag was specified, but the instance already exists.	2147749913 (0x80041019)
		case 	WBEM_E_OVERRIDE_NOT_ALLOWED: return PXActionInvalid; //	Not possible to perform the add operation on this qualifier because the owning object does not permit overrides.	2147749914 (0x8004101A)
		case 	WBEM_E_PROPAGATED_QUALIFIER: return PXActionInvalid; //	User attempted to delete a qualifier that was not owned. The qualifier was inherited from a parent class.	2147749915 (0x8004101B)
		case 	WBEM_E_PROPAGATED_PROPERTY: return PXActionInvalid; //	User attempted to delete a property that was not owned. The property was inherited from a parent class.	2147749916 (0x8004101C)
		case 	WBEM_E_UNEXPECTED: return PXActionInvalid; //	Client made an unexpected and illegal sequence of calls, such as calling EndEnumeration before calling BeginEnumeration.	2147749917 (0x8004101D)
		case 	WBEM_E_ILLEGAL_OPERATION: return PXActionInvalid; //	User requested an illegal operation, such as spawning a class from an instance.	2147749918 (0x8004101E)
		case 	WBEM_E_CANNOT_BE_KEY: return PXActionInvalid; //	Illegal attempt to specify a key qualifier on a property that cannot be a key. The keys are specified in the class definition for an object and cannot be altered on a per-instance basis.	2147749919 (0x8004101F)
		case 	WBEM_E_INCOMPLETE_CLASS: return PXActionInvalid; //	Current object is not a valid class definition. Either it is incomplete or it has not been registered with WMI using SWbemObject.Put_.	2147749920 (0x80041020)
		case 	WBEM_E_INVALID_SYNTAX: return PXActionInvalid; //	Query is syntactically not valid.	2147749921 (0x80041021)
		case 	WBEM_E_NONDECORATED_OBJECT: return PXActionInvalid; //	Reserved for future use.	2147749922 (0x80041022)
		case 	WBEM_E_READ_ONLY: return PXActionInvalid; //	An attempt was made to modify a read-only property.	2147749923 (0x80041023)
		case 	WBEM_E_PROVIDER_NOT_CAPABLE: return PXActionInvalid; //	Provider cannot perform the requested operation. This can include a query that is too complex, retrieving an instance, creating or updating a class, deleting a class, or enumerating a class.	2147749924 (0x80041024)
		case 	WBEM_E_CLASS_HAS_CHILDREN: return PXActionInvalid; //	Attempt was made to make a change that invalidates a subclass.	2147749925 (0x80041025)
		case 	WBEM_E_CLASS_HAS_INSTANCES: return PXActionInvalid; //	Attempt was made to delete or modify a class that has instances.	2147749926 (0x80041026)
		case 	WBEM_E_QUERY_NOT_IMPLEMENTED: return PXActionInvalid; //	Reserved for future use.	2147749927 (0x80041027)
		case 	WBEM_E_ILLEGAL_NULL: return PXActionInvalid; //	Value of Nothing/NULL was specified for a property that must have a value, such as one that is marked by a Key, Indexed, or Not_Null qualifier.	2147749928 (0x80041028)
		case 	WBEM_E_INVALID_QUALIFIER_TYPE: return PXActionInvalid; //	Variant value for a qualifier was provided that is not a legal qualifier type.	2147749929 (0x80041029)
		case 	WBEM_E_INVALID_PROPERTY_TYPE: return PXActionInvalid; //	CIM type specified for a property is not valid.	2147749930 (0x8004102A)
		case 	WBEM_E_VALUE_OUT_OF_RANGE: return PXActionInvalid; //	Request was made with an out-of-range value or it is incompatible with the type.	2147749931 (0x8004102B)
		case 	WBEM_E_CANNOT_BE_SINGLETON: return PXActionInvalid; //	Illegal attempt was made to make a class singleton, such as when the class is derived from a non-singleton class.	2147749932 (0x8004102C)
		case 	WBEM_E_INVALID_CIM_TYPE: return PXActionInvalid; //	CIM type specified is not valid.	2147749933 (0x8004102D)
		case 	WBEM_E_INVALID_METHOD: return PXActionInvalid; //	Requested method is not available.	2147749934 (0x8004102E)
		case 	WBEM_E_INVALID_METHOD_PARAMETERS: return PXActionInvalid; //	Parameters provided for the method are not valid.	2147749935 (0x8004102F)
		case 	WBEM_E_SYSTEM_PROPERTY: return PXActionInvalid; //	There was an attempt to get qualifiers on a system property.	2147749936 (0x80041030)
		case 	WBEM_E_INVALID_PROPERTY: return PXActionInvalid; //	Property type is not recognized.	2147749937 (0x80041031)
		case 	WBEM_E_CALL_CANCELLED: return PXActionInvalid; //	Asynchronous process has been canceled internally or by the user. Note that due to the timing and nature of the asynchronous operation, the operation may not have been truly canceled.	2147749938 (0x80041032)
		case 	WBEM_E_SHUTTING_DOWN: return PXActionInvalid; //	User has requested an operation while WMI is in the process of shutting down.	2147749939 (0x80041033)
		case 	WBEM_E_PROPAGATED_METHOD: return PXActionInvalid; //	Attempt was made to reuse an existing method name from a parent class and the signatures do not match.	2147749940 (0x80041034)
		case 	WBEM_E_UNSUPPORTED_PARAMETER: return PXActionInvalid; //	One or more parameter values, such as a query text, is too complex or unsupported. WMI is therefore requested to retry the operation with simpler parameters.	2147749941 (0x80041035)
		case 	WBEM_E_MISSING_PARAMETER_ID: return PXActionInvalid; //	Parameter was missing from the method call.	2147749942 (0x80041036)
		case 	WBEM_E_INVALID_PARAMETER_ID: return PXActionInvalid; //	Method parameter has an ID qualifier that is not valid.	2147749943 (0x80041037)
		case 	WBEM_E_NONCONSECUTIVE_PARAMETER_IDS: return PXActionInvalid; //	One or more of the method parameters have ID qualifiers that are out of sequence.	2147749944 (0x80041038)
		case 	WBEM_E_PARAMETER_ID_ON_RETVAL: return PXActionInvalid; //	Return value for a method has an ID qualifier.	2147749945 (0x80041039)
		case 	WBEM_E_INVALID_OBJECT_PATH: return PXActionInvalid; //	Specified object path was not valid.	2147749946 (0x8004103A)
		case 	WBEM_E_OUT_OF_DISK_SPACE: return PXActionInvalid; //	Disk is out of space or the 4 GB limit on WMI repository (CIM repository) size is reached.	2147749947 (0x8004103B)
		case 	WBEM_E_BUFFER_TOO_SMALL: return PXActionInvalid; //	Supplied buffer was too small to hold all of the objects in the enumerator or to read a string property.	2147749948 (0x8004103C)
		case 	WBEM_E_UNSUPPORTED_PUT_EXTENSION: return PXActionInvalid; //	Provider does not support the requested put operation.	2147749949 (0x8004103D)
		case 	WBEM_E_UNKNOWN_OBJECT_TYPE: return PXActionInvalid; //	Object with an incorrect type or version was encountered during marshaling.	2147749950 (0x8004103E)
		case 	WBEM_E_UNKNOWN_PACKET_TYPE: return PXActionInvalid; //	Packet with an incorrect type or version was encountered during marshaling.	2147749951 (0x8004103F)
		case 	WBEM_E_MARSHAL_VERSION_MISMATCH: return PXActionInvalid; //	Packet has an unsupported version.	2147749952 (0x80041040)
		case 	WBEM_E_MARSHAL_INVALID_SIGNATURE: return PXActionInvalid; //	Packet appears to be corrupt.	2147749953 (0x80041041)
		case 	WBEM_E_INVALID_QUALIFIER: return PXActionInvalid; //	Attempt was made to mismatch qualifiers, such as putting [key] on an object instead of a property.	2147749954 (0x80041042)
		case 	WBEM_E_INVALID_DUPLICATE_PARAMETER: return PXActionInvalid; //	Duplicate parameter was declared in a CIM method.	2147749955 (0x80041043)
		case 	WBEM_E_TOO_MUCH_DATA: return PXActionInvalid; //	Reserved for future use.	2147749956 (0x80041044)
		case 	WBEM_E_SERVER_TOO_BUSY: return PXActionInvalid; //	Call to IWbemObjectSink::Indicate has failed. The provider can refire the event.	2147749957 (0x80041045)
		case 	WBEM_E_INVALID_FLAVOR: return PXActionInvalid; //	Specified qualifier flavor was not valid.	2147749958 (0x80041046)
		case 	WBEM_E_CIRCULAR_REFERENCE: return PXActionInvalid; //	Attempt was made to create a reference that is circular (for example, deriving a class from itself).	2147749959 (0x80041047)
		case 	WBEM_E_UNSUPPORTED_CLASS_UPDATE: return PXActionInvalid; //	Specified class is not supported.	2147749960 (0x80041048)
		case 	WBEM_E_CANNOT_CHANGE_KEY_INHERITANCE: return PXActionInvalid; //	Attempt was made to change a key when instances or subclasses are already using the key.	2147749961 (0x80041049)
		case 	WBEM_E_CANNOT_CHANGE_INDEX_INHERITANCE: return PXActionInvalid; //	An attempt was made to change an index when instances or subclasses are already using the index.	2147749968 (0x80041050)
		case 	WBEM_E_TOO_MANY_PROPERTIES: return PXActionInvalid; //	Attempt was made to create more properties than the current version of the class supports.	2147749969 (0x80041051)
		case 	WBEM_E_UPDATE_TYPE_MISMATCH: return PXActionInvalid; //	Property was redefined with a conflicting type in a derived class.	2147749970 (0x80041052)
		case 	WBEM_E_UPDATE_OVERRIDE_NOT_ALLOWED: return PXActionInvalid; //	Attempt was made in a derived class to override a qualifier that cannot be overridden.	2147749971 (0x80041053)
		case 	WBEM_E_UPDATE_PROPAGATED_METHOD: return PXActionInvalid; //	Method was re-declared with a conflicting signature in a derived class.	2147749972 (0x80041054)
		case 	WBEM_E_METHOD_NOT_IMPLEMENTED: return PXActionInvalid; //	Attempt was made to execute a method not marked with [implemented] in any relevant class.	2147749973 (0x80041055)
		case 	WBEM_E_METHOD_DISABLED: return PXActionInvalid; //		Attempt was made to execute a method marked with [disabled].
		case 	WBEM_E_REFRESHER_BUSY: return PXActionInvalid; //	Refresher is busy with another operation.	2147749975 (0x80041057)
		case 	WBEM_E_UNPARSABLE_QUERY: return PXActionInvalid; //	Filtering query is syntactically not valid.	2147749976 (0x80041058)
		case 	WBEM_E_NOT_EVENT_CLASS: return PXActionInvalid; //	The FROM clause of a filtering query references a class that is not an event class (not derived from __Event).	2147749977 (0x80041059)
		case 	WBEM_E_MISSING_GROUP_WITHIN: return PXActionInvalid; //	A GROUP BY clause was used without the corresponding GROUP WITHIN clause.	2147749978 (0x8004105A)
		case 	WBEM_E_MISSING_AGGREGATION_LIST: return PXActionInvalid; //	A GROUP BY clause was used. Aggregation on all properties is not supported.	2147749979 (0x8004105B)
		case 	WBEM_E_PROPERTY_NOT_AN_OBJECT: return PXActionInvalid; //	Dot notation was used on a property that is not an embedded object.	2147749980 (0x8004105C)
		case 	WBEM_E_AGGREGATING_BY_OBJECT: return PXActionInvalid; //	A GROUP BY clause references a property that is an embedded object without using dot notation.	2147749981 (0x8004105D)
		case 	WBEM_E_UNINTERPRETABLE_PROVIDER_QUERY: return PXActionInvalid; //	Event provider registration query (__EventProviderRegistration) did not specify the classes for which events were provided.	2147749983 (0x8004105F)
		case 	WBEM_E_BACKUP_RESTORE_WINMGMT_RUNNING: return PXActionInvalid; //	Request was made to back up or restore the repository while it was in use by WinMgmt.exe, or by the SVCHOST process that contains the WMI service.	2147749984 (0x80041060)
		case 	WBEM_E_QUEUE_OVERFLOW: return PXActionInvalid; //	Asynchronous delivery queue overflowed from the event consumer being too slow.	2147749985 (0x80041061)
		case 	WBEM_E_PRIVILEGE_NOT_HELD: return PXActionInvalid; //	Operation failed because the client did not have the necessary security privilege.	2147749986 (0x80041062)
		case 	WBEM_E_INVALID_OPERATOR: return PXActionInvalid; //	Operator is not valid for this property type.	2147749987 (0x80041063)
		case 	WBEM_E_LOCAL_CREDENTIALS: return PXActionInvalid; //	User specified a username/password/authority on a local connection. The user must use a blank username/password and rely on default security.	2147749988 (0x80041064)
		case 	WBEM_E_CANNOT_BE_ABSTRACT: return PXActionInvalid; //	Class was made abstract when its parent class is not abstract.	2147749989 (0x80041065)
		case 	WBEM_E_AMENDED_OBJECT: return PXActionInvalid; //	Amended object was written without the WBEM_FLAG_USE_AMENDED_QUALIFIERS flag being specified.	2147749990 (0x80041066)
		case 	WBEM_E_CLIENT_TOO_SLOW: return PXActionInvalid; //	Client did not retrieve objects quickly enough from an enumeration. This constant is returned when a client creates an enumeration object, but does not retrieve objects from the enumerator in a timely fashion, causing the enumerator's object caches to back up.	2147749991 (0x80041067)
		case 	WBEM_E_NULL_SECURITY_DESCRIPTOR: return PXActionInvalid; //	Null security descriptor was used.	2147749992 (0x80041068)
		case 	WBEM_E_TIMED_OUT: return PXActionInvalid; //	Operation timed out.	2147749993 (0x80041069)
		case 	WBEM_E_INVALID_ASSOCIATION: return PXActionInvalid; //	Association is not valid.	2147749994
		case 	WBEM_E_AMBIGUOUS_OPERATION: return PXActionInvalid; //	Operation was ambiguous.	2147749995 (0x8004106B)
		case 	WBEM_E_QUOTA_VIOLATION: return PXActionInvalid; //	WMI is taking up too much memory. This can be caused by low memory availability or excessive memory consumption by WMI.	2147749996 (0x8004106C)
		//case 	WBEM_E_TRANSACTION_CONFLICT: return PXActionInvalid; //	Operation resulted in a transaction conflict.	2147749997 (0x8004106D)
		//case 	WBEM_E_FORCED_ROLLBACK: return PXActionInvalid; //	Transaction forced a rollback.	2147749998 (0x8004106E)
		case 	WBEM_E_UNSUPPORTED_LOCALE: return PXActionInvalid; //	Locale used in the call is not supported.	2147749999 (0x8004106F)
		case 	WBEM_E_HANDLE_OUT_OF_DATE: return PXActionInvalid; //	Object handle is out-of-date.	2147750000 (0x80041070)
		case 	WBEM_E_CONNECTION_FAILED: return PXActionInvalid; //	Connection to the SQL database failed.	2147750001 (0x80041071)
		case 	WBEM_E_INVALID_HANDLE_REQUEST: return PXActionInvalid; //	Handle request was not valid.	2147750002 (0x80041072)
		case 	WBEM_E_PROPERTY_NAME_TOO_WIDE: return PXActionInvalid; //	Property name contains more than 255 characters.	2147750003 (0x80041073)
		case 	WBEM_E_CLASS_NAME_TOO_WIDE: return PXActionInvalid; //	Class name contains more than 255 characters.	2147750004 (0x80041074)
		case 	WBEM_E_METHOD_NAME_TOO_WIDE: return PXActionInvalid; //	Method name contains more than 255 characters.	2147750005 (0x80041075)
		case 	WBEM_E_QUALIFIER_NAME_TOO_WIDE: return PXActionInvalid; //	Qualifier name contains more than 255 characters.	2147750006 (0x80041076)
		case 	WBEM_E_RERUN_COMMAND: return PXActionInvalid; //	The SQL command must be rerun because there is a deadlock in SQL. This can be returned only when data is being stored in an SQL database.	2147750007 (0x80041077)
		case 	WBEM_E_DATABASE_VER_MISMATCH: return PXActionInvalid; //	The database version does not match the version that the repository driver processes.	2147750008 (0x80041078)
		case 	WBEM_E_VETO_DELETE: return PXActionInvalid; //	WMI cannot execute the delete operation because the provider does not allow it.	2147750009 (0x80041079)
		case 	WBEM_E_VETO_PUT: return PXActionInvalid; //	WMI cannot execute the put operation because the provider does not allow it.	2147750010 (0x8004107A)
		case 	WBEM_E_INVALID_LOCALE: return PXActionInvalid; //	Specified locale identifier was not valid for the operation.	2147750016 (0x80041080)
		case 	WBEM_E_PROVIDER_SUSPENDED: return PXActionInvalid; //	Provider is suspended.	2147750017 (0x80041081)
		case 	WBEM_E_SYNCHRONIZATION_REQUIRED: return PXActionInvalid; //	Object must be written to the WMI repository and retrieved again before the requested operation can succeed. This constant is returned when an object must be committed and retrieved to see the property value.	2147750018 (0x80041082)
		case 	WBEM_E_NO_SCHEMA: return PXActionInvalid; //	Operation cannot be completed; no schema is available.	2147750019 (0x80041083)
		case 	WBEM_E_PROVIDER_ALREADY_REGISTERED: return PXActionInvalid; //	Provider cannot be registered because it is already registered.	02147750020 (0x119FD010)
		case 	WBEM_E_PROVIDER_NOT_REGISTERED: return PXActionInvalid; //	Provider was not registered.	2147750021 (0x80041085)
		case 	WBEM_E_FATAL_TRANSPORT_ERROR: return PXActionInvalid; //	A fatal transport error occurred.	2147750022 (0x80041086)
		case 	WBEM_E_ENCRYPTED_CONNECTION_REQUIRED: return PXActionInvalid; //	User attempted to set a computer name or domain without an encrypted connection.	2147750023 (0x80041087)
		case 	WBEM_E_PROVIDER_TIMED_OUT: return PXActionInvalid; //	A provider failed to report results within the specified timeout.	2147750024 (0x80041088)
		case 	WBEM_E_NO_KEY: return PXActionInvalid; //	User attempted to put an instance with no defined key.	2147750025 (0x80041089)
		case 	WBEM_E_PROVIDER_DISABLED: return PXActionInvalid; //	User attempted to register a provider instance but the COM server for the provider instance was unloaded.	2147750026 (0x8004108A)
		case 	WBEMESS_E_REGISTRATION_TOO_BROAD: return PXActionInvalid; //	Provider registration overlaps with the system event domain.	2147753985 (0x80042001)
		case 	WBEMESS_E_REGISTRATION_TOO_PRECISE: return PXActionInvalid; //	A WITHIN clause was not used in this query.	2147753986 (0x80042002)
		case 	WBEMESS_E_AUTHZ_NOT_PRIVILEGED: return PXActionInvalid; //	This computer does not have the necessary domain permissions to support the security functions that relate to the created subscription instance. Contact the Domain Administrator to get this computer added to the Windows Authorization Access Group.	2147753987 (0x80042003)
		//case 	WBEM_E_RETRY_LATER: return PXActionInvalid; //	Reserved for future use.	2147758081 (0x80043001)
		//case 	WBEM_E_RESOURCE_CONTENTION: return PXActionInvalid; //	Reserved for future use.	2147758082 (0x80043002)
		case 	WBEMMOF_E_EXPECTED_QUALIFIER_NAME: return PXActionInvalid; //	Expected a qualifier name.	2147762177 (0x80044001)
		case 	WBEMMOF_E_EXPECTED_SEMI: return PXActionInvalid; //	Expected semicolon or '='.	2147762178 (0x80044002)
		case 	WBEMMOF_E_EXPECTED_OPEN_BRACE: return PXActionInvalid; //	Expected an opening brace.	2147762179 (0x80044003)
		case 	WBEMMOF_E_EXPECTED_CLOSE_BRACE: return PXActionInvalid; //	Missing closing brace or an illegal array element.	2147762180 (0x80044004)
		case 	WBEMMOF_E_EXPECTED_CLOSE_BRACKET: return PXActionInvalid; //	Expected a closing bracket.	2147762181 (0x80044005)
		case 	WBEMMOF_E_EXPECTED_CLOSE_PAREN: return PXActionInvalid; //	Expected closing parenthesis.	2147762182 (0x80044006)
		case 	WBEMMOF_E_ILLEGAL_CONSTANT_VALUE: return PXActionInvalid; //	Numeric value out of range or strings without quotes.	2147762183 (0x80044007)
		case 	WBEMMOF_E_EXPECTED_TYPE_IDENTIFIER: return PXActionInvalid; //	Expected a type identifier.	2147762184 (0x80044008)
		case 	WBEMMOF_E_EXPECTED_OPEN_PAREN: return PXActionInvalid; //	Expected an open parenthesis.	2147762185 (0x80044009)
		case 	WBEMMOF_E_UNRECOGNIZED_TOKEN: return PXActionInvalid; //	Unexpected token in the file.	2147762186 (0x8004400A)
		case 	WBEMMOF_E_UNRECOGNIZED_TYPE: return PXActionInvalid; //	Unrecognized or unsupported type identifier.	2147762187 (0x8004400B)
		case 	WBEMMOF_E_EXPECTED_PROPERTY_NAME: return PXActionInvalid; //	Expected property or method name.	2147762187 (0x8004400B)
		case 	WBEMMOF_E_TYPEDEF_NOT_SUPPORTED: return PXActionInvalid; //	Typedefs and enumerated types are not supported.	2147762189 (0x8004400D)
		case 	WBEMMOF_E_UNEXPECTED_ALIAS: return PXActionInvalid; //	Only a reference to a class object can have an alias value.	2147762190 (0x8004400E)
		case 	WBEMMOF_E_UNEXPECTED_ARRAY_INIT: return PXActionInvalid; //	Unexpected array initialization. Arrays must be declared with [].	2147762191 (0x8004400F)
		case 	WBEMMOF_E_INVALID_AMENDMENT_SYNTAX: return PXActionInvalid; //	Namespace path syntax is not valid.	2147762192 (0x80044010)
		case 	WBEMMOF_E_INVALID_DUPLICATE_AMENDMENT: return PXActionInvalid; //	Duplicate amendment specifiers.	2147762193 (0x80044011)
		case 	WBEMMOF_E_INVALID_PRAGMA: return PXActionInvalid; //	#pragma must be followed by a valid keyword.	2147762194 (0x80044012)
		case 	WBEMMOF_E_INVALID_NAMESPACE_SYNTAX: return PXActionInvalid; //	Namespace path syntax is not valid.	2147762195 (0x80044013)
		case 	WBEMMOF_E_EXPECTED_CLASS_NAME: return PXActionInvalid; //	Unexpected character in class name must be an identifier.	2147762196 (0x80044014)
		case 	WBEMMOF_E_TYPE_MISMATCH: return PXActionInvalid; //	The value specified cannot be made into the appropriate type.	2147762197 (0x80044015)
		case 	WBEMMOF_E_EXPECTED_ALIAS_NAME: return PXActionInvalid; //	Dollar sign must be followed by an alias name as an identifier.	2147762198 (0x80044016)
		case 	WBEMMOF_E_INVALID_CLASS_DECLARATION: return PXActionInvalid; //	Class declaration is not valid.	2147762199 (0x80044017)
		case 	WBEMMOF_E_INVALID_INSTANCE_DECLARATION: return PXActionInvalid; //	The instance declaration is not valid. It must start with "instance of"	2147762200 (0x80044018)
		case 	WBEMMOF_E_EXPECTED_DOLLAR: return PXActionInvalid; //	Expected dollar sign. An alias in the form "$name" must follow the "as" keyword.	2147762201 (0x80044019)
		case 	WBEMMOF_E_CIMTYPE_QUALIFIER: return PXActionInvalid; //	CIMTYPE qualifier cannot be specified directly in a MOF file. Use standard type notation.	2147762202 (0x8004401A)
		case 	WBEMMOF_E_DUPLICATE_PROPERTY: return PXActionInvalid; //	Duplicate property name was found in the MOF.	2147762203 (0x8004401B)
		case 	WBEMMOF_E_INVALID_NAMESPACE_SPECIFICATION: return PXActionInvalid; //	Namespace syntax is not valid. References to other servers are not allowed.	2147762204 (0x8004401C)
		case 	WBEMMOF_E_OUT_OF_RANGE: return PXActionInvalid; //	Value out of range.	2147762205 (0x8004401D)
		case 	WBEMMOF_E_INVALID_FILE: return PXActionInvalid; //	The file is not a valid text MOF file or binary MOF file.	2147762206 (0x8004401E)
		case 	WBEMMOF_E_ALIASES_IN_EMBEDDED: return PXActionInvalid; //	Embedded objects cannot be aliases.	2147762207 (0x8004401F)
		case 	WBEMMOF_E_NULL_ARRAY_ELEM: return PXActionInvalid; //	NULL elements in an array are not supported.	2147762208 (0x80044020)
		case 	WBEMMOF_E_DUPLICATE_QUALIFIER: return PXActionInvalid; //	Qualifier was used more than once on the object.	2147762209 (0x80044021)
		case 	WBEMMOF_E_EXPECTED_FLAVOR_TYPE: return PXActionInvalid; //	Expected a flavor type such as ToInstance, ToSubClass, EnableOverride, or DisableOverride.	2147762210 (0x80044022)
		case 	WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES: return PXActionInvalid; //	Combining EnableOverride and DisableOverride on same qualifier is not legal.	2147762211 (0x80044023)
		case 	WBEMMOF_E_MULTIPLE_ALIASES: return PXActionInvalid; //	An alias cannot be used twice.	2147762212 (0x80044024)
		case 	WBEMMOF_E_INCOMPATIBLE_FLAVOR_TYPES2: return PXActionInvalid; //	Combining Restricted, and ToInstance or ToSubClass is not legal.	2147762213 (0x80044025)
		case 	WBEMMOF_E_NO_ARRAYS_RETURNED: return PXActionInvalid; //	Methods cannot return array values.	2147762214 (0x80044026)
		case 	WBEMMOF_E_MUST_BE_IN_OR_OUT: return PXActionInvalid; //	Arguments must have an In or Out qualifier.	2147762215 (0x80044027)
		case 	WBEMMOF_E_INVALID_FLAGS_SYNTAX: return PXActionInvalid; //	Flags syntax is not valid.	2147762216 (0x80044028)
		case 	WBEMMOF_E_EXPECTED_BRACE_OR_BAD_TYPE: return PXActionInvalid; //	The final brace and semi-colon for a class are missing.	2147762217 (0x80044029)
		case 	WBEMMOF_E_UNSUPPORTED_CIMV22_QUAL_VALUE: return PXActionInvalid; //	A CIM version 2.2 feature is not supported for a qualifier value.	2147762218 (0x8004402A)
		case 	WBEMMOF_E_UNSUPPORTED_CIMV22_DATA_TYPE: return PXActionInvalid; //	The CIM version 2.2 data type is not supported.	2147762219 (0x8004402B)
		case 	WBEMMOF_E_INVALID_DELETEINSTANCE_SYNTAX: return PXActionInvalid; //	The delete instance syntax is not valid. It should be #pragma DeleteInstance("instancepath", FAIL|NOFAIL)	2147762220 (0x8004402C)
		case 	WBEMMOF_E_INVALID_QUALIFIER_SYNTAX: return PXActionInvalid; //	The qualifier syntax is not valid. It should be qualifiername:type=value,scope(class|instance), flavorname.	2147762221 (0x8004402D)
		case 	WBEMMOF_E_QUALIFIER_USED_OUTSIDE_SCOPE: return PXActionInvalid; //	The qualifier is used outside of its scope.	2147762222 (0x8004402E)
		case 	WBEMMOF_E_ERROR_CREATING_TEMP_FILE: return PXActionInvalid; //	Error creating temporary file. The temporary file is an intermediate stage in the MOF compilation.	2147762223 (0x8004402F)
		case 	WBEMMOF_E_ERROR_INVALID_INCLUDE_FILE: return PXActionInvalid; //	A file included in the MOF by the preprocessor command #include is not valid.	2147762224 (0x80044030)
		case 	WBEMMOF_E_INVALID_DELETECLASS_SYNTAX: return PXActionInvalid; //	The syntax for the preprocessor commands #pragma deleteinstance or #pragma deleteclass is not valid.	2147762225 (0x80044031)




		// A specified class is not registered in the registration database.
		// Also can indicate that the type of server you requested in the CLSCTX
		// enumeration is not registered or the values for the server types in the registry are corrupt.
		case REGDB_E_CLASSNOTREG: 
			return PXActionInvalid;//PXActionRefuedObjectNotInizialized;

		// The ppv parameter is NULL.
		case E_POINTER:  
			return PXActionRefusedArgumentNull;

		case RPC_E_CHANGED_MODE:
			return PXActionInvalid;//PXActionRedundantAlreadyInitialized;

			// The call succeeded, but we had to substitute the 3D algorithm
		case DS_NO_VIRTUALIZATION:
			return PXActionSuccessWithSubstitution;

			// The call failed because resources (such as a priority level) were already being used by another caller
		case DSERR_ALLOCATED:
			return PXActionInvalid;//PXActionRefuedObjectAlreadyExists;

			// The control (vol, pan, etc.) requested by the caller is not available
		case DSERR_CONTROLUNAVAIL:
			return PXActionInvalid;//PXActionRefuedObjectPropertyNotAvailable;

			// An invalid parameter was passed to the returning function
		case DSERR_INVALIDPARAM:
			return PXActionRefusedArgumentInvalid;

			// This call is not valid for the current state of this object
		case DSERR_INVALIDCALL:
			return PXActionInvalid;// PXActionRefuedObjectStateDoesNotAllowAction;

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
			return PXActionInvalid;//PXActionRefuedObjectAlreadyInizialized;
			
		case CLASS_E_NOAGGREGATION: // This class cannot be created as part of an aggregate.			
		//case DSERR_NOAGGREGATION: // This object does not support aggregation
			return PXActionInvalid;//PXActionRefuedObjectAggregationNotSupported;

			// The buffer memory has been lost, and must be restored
		case DSERR_BUFFERLOST:
			return PXActionFailedBufferRefernceLost;

			// Another app has a higher priority level, preventing this call from succeeding
		case DSERR_OTHERAPPHASPRIO:
			return PXActionYieldToOtherProcessWithHigherPriority;

			// This object has not been initialized
		case DSERR_UNINITIALIZED:
			return PXActionInvalid;//PXActionRefuedObjectNotInizialized;

		// The specified class does not implement the requested interface,
		// or the controlling IUnknown does not expose the requested interface.
		//case E_NOINTERFACE:	
		case DSERR_NOINTERFACE: // The requested COM interface is not available
			return PXActionRefuedObjectInterfaceNotAvailable;	

			// Access is denied
		case DSERR_ACCESSDENIED:
			return PXActionDeniedNoPermission;

			// Tried to create a DSBCAPS_CTRLFX buffer shorter than DSBSIZE_FX_MIN milliseconds
		case DSERR_BUFFERTOOSMALL:
			return PXActionRefuedBufferSizeTooSmal;

			// Attempt to use DirectSound 8 functionality on an older DirectSound object
		case DSERR_DS8_REQUIRED:
			return PXActionInvalid;//PXActionRefusedLibraryRequiredUpgrade;

			// A circular loop of send effects was detected
		case DSERR_SENDLOOP:
			return PXActionCanceledLoopDeteced;

			// The GUID specified in an audiopath file does not match a valid MIXIN buffer
		case DSERR_BADSENDBUFFERGUID:
			return PXActionRefusedNotFound;

			// The object requested was not found (numerically equal to DMUS_E_NOT_FOUND)
		case DSERR_OBJECTNOTFOUND:
			return PXActionRefusedNotFound;

			// The effects requested could not be found on the system, or they were found
			// but in the wrong order, or in the wrong hardware/software locations.
		case DSERR_FXUNAVAILABLE:
			return PXActionInvalid;//PXActionRefusedEffectNotAvailable;

		default:
			return PXActionInvalid;
	}
}

PXActionResult PXAPI PXWindowsMMAudioConvertFromID(const PXInt32U mmResultID)
{
	switch (mmResultID)
	{
		case MMSYSERR_NOERROR:  return PXActionSuccessful;
			//case MMSYSERR_ERROR:return AudioResultErrorUnspecified;
			//case MMSYSERR_BADDEVICEID:  return AudioResultDeviceIDInvalid;
		case MMSYSERR_NOTENABLED:   return AudioResultDriverNotEnabled;
		case MMSYSERR_ALLOCATED: return AudioResultDeviceAlreadyAllocated;
			//case MMSYSERR_INVALHANDLE:  return AudioResultDeviceHandleInvalid;
			//case MMSYSERR_NODRIVER:   return AudioResultDeviceNoDriver;
		case MMSYSERR_NOMEM:return PXActionFailedMemoryAllocation;
			//case MMSYSERR_NOTSUPPORTED: return PXActionRefusedNotSupported;
		case MMSYSERR_BADERRNUM:  return AudioResultErrorValueInvalid;
		case MMSYSERR_INVALFLAG: return AudioResultFlagInvalid;
		case MMSYSERR_INVALPARAM: return PXActionRefusedArgumentInvalid;
		case MMSYSERR_HANDLEBUSY:return AudioResultDeviceHandleBusy;
		case MMSYSERR_INVALIDALIAS:return AudioResultAliasNotFound;
			//case MMSYSERR_BADDB: return AudioResultPXRegistryDatabaseInvalid;
			//case MMSYSERR_KEYNOTFOUND: return AudioResultPXRegistryKeyNotFound;
			//case MMSYSERR_READERROR:  return AudioResultPXRegistryReadError;
			//case MMSYSERR_WRITEERROR: return AudioResultPXRegistryWriteError;
			//case MMSYSERR_DELETEERROR:   return AudioResultPXRegistryDeleteError;
			//case MMSYSERR_VALNOTFOUND: return AudioResultPXRegistryValueNotFound;
		//case MMSYSERR_NODRIVERCB:  return AudioResultDriverNoCallback;
			//case MMSYSERR_MOREDATA:   return AudioResultMoreData;

		case WAVERR_BADFORMAT: return PXActionRefusedFormatNotSupported;
		case WAVERR_STILLPLAYING: return AudioResultDeviceIsStillPlaying;
		case WAVERR_UNPREPARED: return AudioResultReaderIsNotPrepared;
		case WAVERR_SYNC: return AudioResultDeviceIsSynchronous;

		default:
			return PXActionInvalid;
	}
}
#endif
