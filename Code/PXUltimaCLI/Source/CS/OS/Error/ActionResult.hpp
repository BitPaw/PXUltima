#pragma once

namespace PX
{
	public enum class ActionResult
	{
		Invalid, // Default iniitlialisation value, no not use.
		Successful, //  finished successful
		FailedAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
		DeniedNoPermission,
		RefuedInputBufferNull, // is NULL
		RefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
		RefuedInputBufferTooSmal, // cant 
		RefuedOutputBufferNull, // is NULL 
		RefuedOutputBufferTooSmal, // 
		RefusedEmptyFilePath,
		FailedFileNotFound,
		FailedFileMapping,
		FailedFileCreate,
		FailedFileDelete,
		FailedFileOpen,
		FailedFileClose,
		FailedFileWrite,
		FailedFileRead,
		FailedSocketRecieve,
		FailedSocketSend,
		RefusedSocketNotConnected,
		FailedSocketConnect,
		FailedSocketCreation,
		FailedSocketOption,
		FailedSocketBinding,
		FailedSocketListening,
		RefusedSocketInavlidAdressFamily,
		RefusedSocketInvalidFlags,
		RefusedNameServerIsTemporaryOffline,
		RefusedNameServerIsPermanentOffline,
		RefusedSocketTypeNotSupported,
		RefusedRequestedAddressFamilyNotSupported,
		RefusedHostExistsButHasNoData,
		RefusedHostHasNoNetworkAddresses,
		RefusedIPOrPortNotKnown,
		FailedFormatNotDetected,
		RefusedFormatNotSupported,
		FailedFormatInvalid,
		FailedFormatNotAsExpected,
		RefusedInvalidHeaderSignature,
		RefusedInvalidVersion,
		FailedResourceRegister,
		RefusedResourceRegister,
		RefusedResourceDelete,
		RequestedServiceNotAvailableForSocket,
		NoPXClientWithThisID,
		CompilingError,
		SubSystemNotReady,
		VersionNotSupported,
		BlockedByOtherOperation,
		LimitReached,
		InvalidParameter,
		SubSystemNotInitialised,
		SubSystemNetworkFailed,
		SocketIsBlocking,
		WindowsSocketSystemNotInitialized,

		// POSIX Error codes, these are pre-defined
		UnkownError, // OTHER
		PermissionDenied, // ACCES
		AddressInUse, // ADDRINUSE
		AddressNotAvailable, // ADDRNOTAVAIL
		AddressFamilyNotSupported, // AFNOSUPPORT
		ResourceUnavailableTryAgain, // AGAIN
		ConnectionAlreadyInProgress, // ALREADY
		ArgumentListTooLong, // 2BIG
		BadFileDescriptor, // BADF
		BadMessage, // BADMSG
		DeviceOrResourceBusy, // BUSY
		OperationCanceled,// CANCELED
		NoChildProcesses, // CHILD
		ConnectionAborted, // CONNABORTED
		ConnectionRefused, // CONNREFUSED
		ConnectionReset, // CONNRESET
		ResourceDeadlockWouldOccur, // DEADLK
		DestinationAddressRequired, // DESTADDRREQ
		PXMathematicsArgumentOutOfDomainOfFunction, // DOM
		FileExists, // EXIST
		BadAddress, // FAULT
		FileTooLarge, // FBIG
		HostIsUnreachable, // HOSTUNREACH
		IdentifierRemoved, // IDRM
		IllegalByteSequence, // ILSEQ
		OperationInProgress, // INPROGRESS
		InterruptedFunction, // INTR
		InvalidArgument, // INVAL
		IOFailure, // IO
		SocketIsConnected, // ISCONN
		IsADirectory, // ISDIR
		TooManyLevelsOfSymbolicLinks, // LOOP
		FileDescriptorValueTooLarge, // MFILE
		TooManyLinks, // MLINK
		MessageTooLarge, // MSGSIZE
		FilenameTooLong, // NAMETOOLONG
		NetworkIsDown, // NETDOWN
		ConnectionAbortedByNetwork, // NETRESET
		NetworkUnreachable, // NETUNREACH
		TooManyFilesOpenInSystem, // NFILE
		NoBufferSpaceAvailable, // NOBUFS
		NoMessageIsAvailableOnTheStreamHeadReadQueue, // NODATA
		NoSuchDevice, // NODEV
		NoSuchFileOrDirectory, // NOENT
		ExecutableFileFormatError, // NOEXEC
		NoLocksAvailable, // NOLCK
		LinkHasBeenSevered, // NOLINK
		NoMessageOfTheDesiredType, // NOMSG
		ProtocolNotAvailable, // NOPROTOOPT
		NoSpaceLeftOnDevice, // NOSPC
		NoStreamResources, // NOSR
		NotAStream, // NOSTR
		FunctionNotSupported, // NOSYS
		TheSocketIsNotConnected, // NOTCONN
		NotADirectory, // NOTDIR
		DirectoryNotEmpty, // NOTEMPTY
		StateNotRecoverable, // NOTRECOVERABLE
		NotASocket, // NOTSOCK
		NotSupported, // NOTSUP
		InappropriateIOControlOperation, // NOTTY
		NoSuchDeviceOrAddress, // NXIO
		OperationNotSupportedOnSocket, // OPNOTSUPP
		ValueTooLargeToBeStoredInDataType, // OVERFLOW
		PreviousOwnerDied, // OWNERDEAD
		OperationNotPermitted, // PERM
		BrokenPipe, // PIPE
		ProtocolError, // PROTO
		ProtocolNotSupported, // PROTONOSUPPORT
		ProtocolWrongTypeForSocket, // PROTOTYPE
		ResultTooLarge, // RANGE,
		ReadOnlyFileSystem, //ROFS
		InvalidSeek, // SPIPE
		NoSuchProcess, // SRCH
		StreamTimeout, // TIME -> ioctl()
		ConnectionTimedOut, // TIMEDOUT
		TextFileBusy, // TXTBSY
		OperationWouldBlock, // WOULDBLOCK
		CrossDeviceLink // XDEV
	};
}