#ifndef PXCPPActionResultInclude
#define PXCPPActionResultInclude

#include <OS/Error/PXActionResult.h>

namespace PX
{
	// Refused => User error
	// Invalid => State error
	// Failed => Fundamental error
	enum class ActionResult
	{
		//---<General>-----------------------
		Invalid, // Default iniitlialisation value, no not use.
		Successful, //  finished successful
		FailedAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
		DeniedNoPermission,
		//-----------------------------------

		NotImplemented,
		NotSupportedByLibrary,
		NotSupportedByOperatingSystem,

		RefuedParameterNull, // Some needed Parameter is null


		RefuedInputBufferNull, // is NULL
		RefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
		RefuedInputBufferTooSmal, // cant 

		RefuedOutputBufferNull, // is NULL 
		RefuedOutputBufferTooSmal, // 


		InvalidStateImpossible, // Is an object is used and has data that should not be possible


		FailedElementNotFound,

		InvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly



		//---<File-General>------------------
		RefusedEmptyFilePath,
		RefusedInvalidFilePath,

		FailedFileNotFound,
		FailedNotAFile,

		FailedFileMapping,// mmap() / CreateViewofFile() failed

		FailedFileCreate,
		FailedFileDelete,
		FailedFileOpen,
		FailedFileClose,
		FailedFileWrite,
		FailedFileRead,

		//---<Socket>---------------------
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
		//-----------------------------------

		//---<File-Parsing>-------------------
		FailedFormatNotDetected,
		RefusedFormatNotSupported,
		FailedFormatInvalid,
		FailedFormatNotAsExpected,
		RefusedInvalidHeaderSignature,
		RefusedInvalidVersion,
		//-----------------------------------

		//---<File-Writing>------------------
		RefusedFormatSettingNotAllowed,
		//-----------------------------------


		//---<Graphics>-----------------------
		FailedResourceRegister,
		RefusedResourceRegister,
		RefusedResourceDelete,
		//------------------------------------



		RequestedServiceNotAvailableForSocket,


		//---<Server>------------------------
		NoPXClientWithThisID,
		//-----------------------------------

		//---<Compiler>----------------------
		CompilingError,
		//-----------------------------------

		//---[ Windows Only (WindowsSocketAgent) ]-------
		SubSystemNotReady,
		VersionNotSupported,
		BlockedByOtherOperation,
		LimitReached,
		InvalidParameter,
		SubSystemNotInitialised,
		SubSystemNetworkFailed,
		SocketIsBlocking,

		WindowsSocketSystemNotInitialized,
		//----------------------------------------

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

	PXDLLExport PX::ActionResult ErrorCurrent();
}

#endif