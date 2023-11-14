#pragma once

#include <Media/PXType.h>

namespace PX
{
	// Refused => User error
	// Invalid => State error
	// Failed => Fundamental error
	enum class ActionResult
	{
		//---<General>-----------------------
		Invalid, // Default iniitlialisation value, do not use.
		Successful, // Action finished successful
		SuccessWithSubstitution,
		DidNothing, // if the function did nothing
		FailedUnkownError, // [OTHER] Undefined error
		//-----------------------------------

		CanceledLoopDeteced, // LOOP

		//-------------------------------------------------------
		// Memory 
		//-------------------------------------------------------
		FailedMemoryAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
		FailedMemoryRelease,

		FailedBufferRefernceLost, // Windows lost a buffer and it needs to be restord 
		RefuedBufferSizeTooSmal,

		RefuedInputBufferNull, // is NULL
		RefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
		RefuedInputBufferTooSmal, // cant 
		RefuedInputBufferTooBig,

		RefuedOutputBufferNull, // is NULL 
		RefuedOutputBufferTooSmal, // 

		//-------------------------------------------------------
		// Object
		//-------------------------------------------------------
		RefuedObjectNotReady, // Object can't process this function, setup is needed or its busy
		RefuedObjectIDInvalid, // Use if you have an invalid ID or HANDLE
		RefuedObjectStateDoesNotAllowAction,
		RefuedObjectInterfaceNotAvailable,
		RefuedObjectNotInizialized,
		RefuedObjectAlreadyInizialized,
		RefuedObjectAlreadyExists,
		RefuedObjectTypeInvalid,
		RefuedObjectTypeNotSupported,
		RefuedObjectAggregationNotSupported,
		RefuedObjectPropertyNotAvailable,

		//-------------------------------------------------------
		// Permission
		//-------------------------------------------------------
		DeniedNoPermission, // [ACCES] You do not have permission to use this command.
		DeniedPriorityTooLow,
		YieldToOtherProcessWithHigherPriority,
		//Denied

		//-------------------------------------------------------
		// File I/O 
		//-------------------------------------------------------
		FailedFileNameTooLong, // NAMETOOLONG
		RefusedTargetIsDirectory, // ISDIR
		FailedFileDescriptorValueTooLarge, // MFILE
		FailedTooManyLinks, // MLINK
		RefusedFileAlreadyExists, // EXIST
		FailedFileTooLarge, // FBIG
		RefusedInvalidSeek, // SPIPE
		FailedTooManyFilesOpenInSystem, // NFILE
		RefusedNotADirectory, // NOTDIR
		RefusedDirectoryNotEmpty, // NOTEMPTY

		//-------------------------------------------------------
		// Network 
		//-------------------------------------------------------
		RefusedAlreadyConnected, // [ISCONN]
		RefusedNetworkIsDown, // [NETDOWN]
		FailedConnectionAbortedByNetwork, // [NETRESET]
		FailedNetworkUnreachable, // [NETUNREACH]
		RefusedNotASocket, // [NOTSOCK]
		FailedConnectionTimedOut, // [TIMEDOUT]
		FailedNotConnected, // [NOTCONN]

		//-------------------------------------------------------
		// Audio/Video
		//-------------------------------------------------------
		RefusedEffectNotAvailable,


		//-------------------------------------------------------
		// Functions
		//-------------------------------------------------------
		RefusedArgumentNull, // Some needed Parameter is null
		RefusedArgumentInvalid, // [INVAL]	

		//-------------------------------------------------------
		// Library
		//-------------------------------------------------------
		RefusedNotSupported, // [NOTSUP] Action can't be exectuted, as the function does not exist to be used.
		RefusedNotImplemented, // Function that exists but does not contain any code
		NotSupportedByLibrary, // 
		LibraryNotFound,
		NotSupportedByOperatingSystem,

		RefusedLibraryRequiredUpgrade,


		RefusedMissingCallBack, // Callback is required but missing

		FailedInitialization, // Re
		RedundantAlreadyInitialized,
		FailedCleanup,
		FailedModuleLoad,
		FailedDataFetch,

		InterruptedByEvent,
		InterruptedByFunctionCall,



		RefuedPermissionDenied,
		FailedResourcedNotEnough,

		WouldBlock,
		NowInProgress,
		AlreadyInProgress,


		RefusedInvalidOperationSpecified,
		FailedStackOverflow,
		FailedStackUnderflow,


		RefuedNetworkNotConnected,
		RefuedNetworkNotReachable,
		FailedHostNotFound,

		RefuedServiceNotRunning,

		xxxxxxxxxxxx,

		RefuedAlreadyInUse,

		RefuedProtocolTypeInvalid,
		RefuedProtocolOptionInvalid,
		RefuedProtocolNotSupported,
		RefuedProtocolFamilyNotSupported,

		RefuedAddressFamilyNotSupportedByProtocol,




		RefuedOperationNotSupported,

		Cancelled,

		//---<Function input>---
		RefuedAdressInvalid, // System detected an invalid pointer





		RefuedTextFormatUnsupported,


		RefuedInputInvalid,




		InvalidStateImpossible, // Is an object is used and has data that should not be possible


		FailedElementNotFound,

		InvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly





		RefuedDriverInvalid,
		RefuedDriverMissing,


		RefuedFormatIllegal,
		RefuedFormatNotSupported,











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


		//---<Service>-----------------------
		NoResposeCountNotConnect,
		FailedConnectionFailure,
		RefusedNotConnected, // Trying to use a command that requires an already established connection.
		FailedSettingsInvalid,
		FailedCommandDeploy, // Failure to awns from source to target
		FailedCommandExecute, // Faiure to execute on target
		FailedCommandRetrieve, // Failure to retrieve result from target
		//-----------------------------------

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
		SubSystemNotInitialised,
		SubSystemNetworkFailed,
		SocketIsBlocking,


		FailedDirectoryIsNotEmpty,

		FailedTooManyProcesses,

		FailedUserQuotaExceeded,
		FailedDiskQuotaExceeded,

		FailedHandleIsStale,

		FailedResourceNotAvailableLocally,

		FailedNetworkSubsystemNotReady,

		RefusedResourceIsShuttingdown,

		WindowsSocketVersionNotSupported,
		WindowsSocketSystemNotInitialized,
		WindowsSocketSystemWasShutdown,
		//-------------------------------------------------

		//---<Audio>---------------------------------------
		AudioResultDeviceIDInvalid,
		AudioResultDeviceHandleInvalid,
		AudioResultDeviceNoDriver,

		AudioResultDriverNotReachable, // Maybe diabled, or deactivated
		AudioResultDriverNotEnabled, // No driver to use


		AudioResultDeviceAlreadyAllocated,


		AudioResultErrorValueInvalid,


		AudioResultFlagInvalid,

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

		AudioResultDeviceIsStillPlaying,
		AudioResultReaderIsNotPrepared,
		AudioResultDeviceIsSynchronous,
		//-------------------------------------------------

		FailedConnectionTerminatedByNetwork,
		FailedConnectionTerminatedByOwnSoftware,
		FailedConnectionTerminatedByPeer,

		FailedMemoryAllocationInternal,


		// POSIX Error codes, these are pre-defined



		AddressInUse, // ADDRINUSE
		AddressNotAvailable, // ADDRNOTAVAIL
		AddressFamilyNotSupported, // AFNOSUPPORT
		ResourceUnavailableTryAgain, // AGAIN
		ConnectionAlreadyInProgress, // ALREADY
		ArgumentListTooLong, // 2BIG
		BadFileDescriptor, // BADF
		BadMessage, // BADMSG
		RefusedResourceBusy, // BUSY
		OperationCanceled,// CANCELED
		NoChildProcesses, // CHILD
		ConnectionAborted, // CONNABORTED
		ConnectionRefused, // CONNREFUSED
		ConnectionReset, // CONNRESET
		ResourceDeadlockWouldOccur, // DEADLK
		DestinationAddressRequired, // DESTADDRREQ
		PXMathematicsArgumentOutOfDomainOfFunction, // DOM


		BadAddress, // FAULT

		HostIsUnreachable, // HOSTUNREACH
		IdentifierRemoved, // IDRM
		IllegalByteSequence, // ILSEQ
		OperationInProgress, // INPROGRESS
		InterruptedFunction, // INTR

		IOFailure, // IO








		MessageTooLarge, // MSGSIZE



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


		StateNotRecoverable, // NOTRECOVERABLE




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

		NoSuchProcess, // SRCH
		StreamTimeout, // TIME -> ioctl()

		TextFileBusy, // TXTBSY
		OperationWouldBlock, // WOULDBLOCK
		CrossDeviceLink // XDEV
	};

	PXDLLExport PX::ActionResult ErrorCurrent();
}