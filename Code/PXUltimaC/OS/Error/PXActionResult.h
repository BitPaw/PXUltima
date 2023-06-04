#ifndef PXActionResultInclude
#define PXActionResultInclude

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Refused => User error
	// Invalid => State error
	// Failed => Fundamental error
	typedef enum PXActionResult_
	{
		//---<General>-----------------------
		PXActionInvalid, // Default iniitlialisation value, do not use.
		PXActionSuccessful, // PXAction finished successful
		PXActionDidNothing, // if the function did nothing
		PXActionFailedMemoryAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
		PXActionFailedMemoryRelease,
		PXActionDeniedNoPermission,
		//-----------------------------------

		PXActionNotImplemented,
		PXActionNotSupportedByLibrary,
		PXActionNotSupportedByOperatingSystem,

		//---<Function input>---
		PXActionRefuedParameterNull, // Some needed Parameter is null

		PXActionRefuedTextFormatUnsupported,

		PXActionRefuedObjectNotReady, // Object can't process this function, setup is needed or its busy

		
		PXActionRefuedInputBufferNull, // is NULL
		PXActionRefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
		PXActionRefuedInputBufferTooSmal, // cant 
	
		PXActionRefuedOutputBufferNull, // is NULL 
		PXActionRefuedOutputBufferTooSmal, // 


		PXActionInvalidStateImpossible, // Is an object is used and has data that should not be possible


		PXActionFailedElementNotFound,

		PXActionInvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly

	


		//---<File-General>------------------
		PXActionRefusedEmptyFilePath,
		PXActionRefusedInvalidFilePath,

		PXActionFailedFileNotFound,
		PXActionFailedNotAFile,

		PXActionFailedFileMapping,// mmap() / CreateViewofFile() failed

		PXActionFailedFileCreate,
		PXActionFailedFileDelete,
		PXActionFailedFileOpen,
		PXActionFailedFileClose,
		PXActionFailedFileWrite,
		PXActionFailedFileRead,

		//---<Socket>---------------------
		PXActionFailedSocketRecieve,
		PXActionFailedSocketSend,
		PXActionRefusedSocketNotConnected,

		PXActionFailedSocketConnect,
		PXActionFailedSocketCreation,
		PXActionFailedSocketOption,
		PXActionFailedSocketBinding,
		PXActionFailedSocketListening,

		PXActionRefusedSocketInavlidAdressFamily,
		PXActionRefusedSocketInvalidFlags,
		PXActionRefusedNameServerIsTemporaryOffline,
		PXActionRefusedNameServerIsPermanentOffline,
		PXActionRefusedSocketTypeNotSupported,
		PXActionRefusedRequestedAddressFamilyNotSupported,
		PXActionRefusedHostExistsButHasNoData,
		PXActionRefusedHostHasNoNetworkAddresses,
		PXActionRefusedIPOrPortNotKnown,
		//-----------------------------------

		//---<File-Parsing>-------------------
		PXActionFailedFormatNotDetected,
		PXActionRefusedFormatNotSupported,
		PXActionFailedFormatInvalid,
		PXActionFailedFormatNotAsExpected,
		PXActionRefusedInvalidHeaderSignature,
		PXActionRefusedInvalidVersion,
		//-----------------------------------

		//---<File-Writing>------------------
		PXActionRefusedFormatSettingNotAllowed,
		//-----------------------------------


		//---<Graphics>-----------------------
		PXActionFailedResourceRegister,
		PXActionRefusedResourceRegister,
		PXActionRefusedResourceDelete,
		//------------------------------------



		RequestedServiceNotAvailableForSocket,


		//---<Service>-----------------------
		PXActionNoResposeCountNotConnect,
		PXActionFailedConnectionFailure,
		PXActionRefusedNotConnected, // Trying to use a command that requires an already established connection.
		PXActionFailedSettingsInvalid,
		PXActionFailedCommandDeploy, // Failure to awns from source to target
		PXActionFailedCommandExecute, // Faiure to execute on target
		PXActionFailedCommandRetrieve, // Failure to retrieve result from target
		//-----------------------------------

		//---<Server>------------------------
		NoPXClientWithThisID,
		//-----------------------------------

		//---<Compiler>----------------------
		PXActionCompilingError,
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
		//-------------------------------------------------

		//---<Audio>---------------------------------------
			AudioResultDeviceIDInvalid,
			AudioResultDeviceHandleInvalid,
			AudioResultDeviceNoDriver,

			AudioResultDriverNotReachable, // Maybe diabled, or deactivated
			AudioResultDriverNotEnabled, // No driver to use


			AudioResultDeviceAlreadyAllocated,
			AudioResultOutOfMemory,

			AudioResultNotSupported,

			AudioResultErrorValueInvalid,


			AudioResultFlagInvalid,
			AudioResultParameterInvalid,

			AudioResultDeviceHandleBusy,

			AudioResultAliasNotFound,



			AudioResultPXRegistryValueNotFound,

			AudioResultDriverNoCallback,

			AudioResultMoreData,


			// Custom
			AudioResultDeviceListNotBigEnough,


			// Windows spesific
			AudioResultPXRegistryDatabaseInvalid,
			AudioResultPXRegistryKeyNotFound,
			AudioResultPXRegistryWriteError,
			AudioResultPXRegistryReadError,
			AudioResultPXRegistryDeleteError,

			AudioResultErrorUnspecified, 		// Bad, but windows, am i right?



			// only
			AudioResultWaveFormatUnsupported,
			AudioResultDeviceIsStillPlaying,
			AudioResultReaderIsNotPrepared,
			AudioResultDeviceIsSynchronous,
		//-------------------------------------------------


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
	}
	PXActionResult;

#define PXActionReturnOnSuccess(actionResult) if (PXActionSuccessful == actionResult) return PXActionSuccessful;
#define PXActionReturnOnError(actionResult) if (PXActionSuccessful != actionResult) return actionResult;
#define PXActionOnErrorFetchAndReturn(b) if(b) { return PXErrorCurrent(); }

	PXPublic PXActionResult PXErrorCodeFromID(const int errorCode);
	PXPublic PXActionResult PXErrorCurrent();

#ifdef __cplusplus
}
#endif

#endif