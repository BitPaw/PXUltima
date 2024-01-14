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
		PXActionSuccessful, // Action finished successful
		PXActionSuccessWithSubstitution,
		PXActionDidNothing, // if the function did nothing
		PXActionFailedUnkownError, // [OTHER] Undefined error
		//-----------------------------------

		PXActionCanceledLoopDeteced, // LOOP

		//-------------------------------------------------------
		// Memory 
		//-------------------------------------------------------
		PXActionFailedMemoryAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
		PXActionFailedMemoryRelease,

		PXActionRefusedIndexOutOfBounce, // A given index is outside of the given space of the array.

		PXActionFailedBufferRefernceLost, // Windows lost a buffer and it needs to be restord 
		PXActionRefuedBufferSizeTooSmal,

		PXActionRefuedInputBufferNull, // is NULL
		PXActionRefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
		PXActionRefuedInputBufferTooSmal, // cant 
		PXActionRefuedInputBufferTooBig,

		PXActionRefuedOutputBufferNull, // is NULL 
		PXActionRefuedOutputBufferTooSmal, // 

		//-------------------------------------------------------
		// Type
		//-------------------------------------------------------
		PXActionRefusedTypeMissmatch, // When two types are not copartible

		//-------------------------------------------------------
		// Object
		//-------------------------------------------------------
		PXActionRefuedObjectNotReady, // Object can't process this function, setup is needed or its busy
		PXActionRefusedObjectNotFound, // [404] Invalid ID or HANDLE -> Does not exist
		PXActionRefusedObjectAlreadyExists, // [EXIST]
		PXActionRefuedObjectStateDoesNotAllowAction,
		PXActionRefuedObjectInterfaceNotAvailable,
		PXActionRefuedObjectNotInizialized,
		PXActionRefuedObjectAlreadyInizialized,
		PXActionRefuedObjectAlreadyExists,
		PXActionRefuedObjectTypeInvalid,
		PXActionRefuedObjectTypeNotSupported,
		PXActionRefuedObjectAggregationNotSupported,
		PXActionRefuedObjectPropertyNotAvailable,

		PXActionRefusedObjectNameInvalid,
		PXActionRefusedClassNameInvalid,
		PXActionRefusedNameSpaceInvalid,
		PXActionRefusedObjectParentInvalid,

		//-------------------------------------------------------
		// Permission
		//-------------------------------------------------------
		PXActionDeniedNoPermission, // [ACCES] You do not have permission to use this command.
		PXActionDeniedPriorityTooLow,
		PXActionYieldToOtherProcessWithHigherPriority,
		//PXActionDenied

		//-------------------------------------------------------
		// File I/O 
		//-------------------------------------------------------
		PXActionFailedFileNameTooLong, // NAMETOOLONG
		PXActionRefusedTargetIsDirectory, // ISDIR
		PXActionFailedFileDescriptorValueTooLarge, // MFILE
		PXActionFailedTooManyLinks, // MLINK
		PXActionFailedFileTooLarge, // FBIG
		PXActionRefusedInvalidSeek, // SPIPE
		PXActionFailedTooManyFilesOpenInSystem, // NFILE
		PXActionRefusedNotADirectory, // NOTDIR
		PXActionRefusedDirectoryNotEmpty, // NOTEMPTY


		//-------------------------------------------------------
		// Compiler 
		//-------------------------------------------------------
		PXActionRefusedSymbolInvalid,

		//-------------------------------------------------------
		// Network 
		//-------------------------------------------------------
		PXActionRefusedAlreadyConnected, // [ISCONN]
		PXActionRefusedNetworkIsDown, // [NETDOWN]
		PXActionFailedConnectionAbortedByNetwork, // [NETRESET]
		PXActionFailedNetworkUnreachable, // [NETUNREACH]
		PXActionRefusedNotASocket, // [NOTSOCK]
		PXActionFailedConnectionTimedOut, // [TIMEDOUT]
		PXActionFailedNotConnected, // [NOTCONN]

		//-------------------------------------------------------
		// Audio/Video
		//-------------------------------------------------------
		PXActionRefusedEffectNotAvailable,


		//-------------------------------------------------------
		// Functions
		//-------------------------------------------------------
		PXActionRefusedArgumentNull, // Some needed Parameter is null
		PXActionRefusedArgumentInvalid, // [INVAL]	

		//-------------------------------------------------------
		// Library
		//-------------------------------------------------------
		PXActionRefusedNotSupported, // [NOTSUP] Action can't be exectuted, as the function does not exist to be used.
		PXActionRefusedNotImplemented, // Function that exists but does not contain any code
		PXActionNotSupportedByLibrary, // 
		PXActionLibraryNotFound,
		PXActionNotSupportedByOperatingSystem,

		PXActionRefusedLibraryRequiredUpgrade,


		PXActionRefusedMissingCallBack, // Callback is required but missing

		PXActionFailedInitialization, // Re
		PXActionRedundantAlreadyInitialized,
		PXActionFailedCleanup,
		PXActionFailedModuleLoad,
		PXActionFailedDataFetch,

		PXActionInterruptedByEvent,
		PXActionInterruptedByFunctionCall,



		PXActionRefuedPermissionDenied,
		PXActionFailedResourcedNotEnough,

		PXActionWouldBlock,
		PXActionNowInProgress,
		PXActionAlreadyInProgress,


		PXActionRefusedInvalidOperationSpecified,
		PXActionFailedStackOverflow,
		PXActionFailedStackUnderflow,


		PXActionRefuedNetworkNotConnected,
		PXActionRefuedNetworkNotReachable,
		PXActionFailedHostNotFound,

		PXActionRefuedServiceNotRunning,

		xxxxxxxxxxxx,

		PXActionRefuedAlreadyInUse,

		PXActionRefuedProtocolTypeInvalid,
		PXActionRefuedProtocolOptionInvalid,
		PXActionRefuedProtocolNotSupported,
		PXActionRefuedProtocolFamilyNotSupported,

		PXActionRefuedAddressFamilyNotSupportedByProtocol,




		PXActionRefuedOperationNotSupported,

		PXActionCancelled,

		//---<Function input>---
		PXActionRefuedAdressInvalid, // System detected an invalid pointer


	


		PXActionRefuedTextFormatUnsupported,


		PXActionRefuedInputInvalid,




		PXActionInvalidStateImpossible, // Is an object is used and has data that should not be possible


		PXActionFailedElementNotFound,

		PXActionInvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly





		PXActionRefuedDriverInvalid,
		PXActionRefuedDriverMissing,


			PXActionRefuedFormatIllegal,
		PXActionRefuedFormatNotSupported,











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
		PXActionRefusedInvalidSymbol, // While parsing found symbol thats not supported to be read
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
		SubSystemNotInitialised,
		SubSystemNetworkFailed,
		SocketIsBlocking,


		PXActionFailedDirectoryIsNotEmpty,

		PXActionFailedTooManyProcesses,

		PXActionFailedUserQuotaExceeded,
		PXActionFailedDiskQuotaExceeded,

		PXActionFailedHandleIsStale,

		PXActionFailedResourceNotAvailableLocally,

		PXActionFailedNetworkSubsystemNotReady,

		PXActionRefusedResourceIsShuttingdown,

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

		PXActionFailedConnectionTerminatedByNetwork,
		PXActionFailedConnectionTerminatedByOwnSoftware,
		PXActionFailedConnectionTerminatedByPeer,

		PXActionFailedMemoryAllocationInternal,


		// POSIX Error codes, these are pre-defined


	
		AddressInUse, // ADDRINUSE
		AddressNotAvailable, // ADDRNOTAVAIL
		AddressFamilyNotSupported, // AFNOSUPPORT
		ResourceUnavailableTryAgain, // AGAIN
		ConnectionAlreadyInProgress, // ALREADY
		ArgumentListTooLong, // 2BIG
		BadFileDescriptor, // BADF
		BadMessage, // BADMSG
		PXActionRefusedResourceBusy, // BUSY
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
	}
	PXActionResult;

#define PXActionReturnOnSuccess(actionResult) if (PXActionSuccessful == actionResult) return PXActionSuccessful;
#define PXActionReturnOnError(actionResult) if (PXActionSuccessful != actionResult) return actionResult;
#define PXActionContinueOnError(actionResult) if (PXActionSuccessful != actionResult) continue;
#define PXActionOnErrorFetchAndReturn(b) if(b) { return PXErrorCurrent(); }

	PXPublic PXActionResult PXAPI PXErrorCodeFromID(const int errorCode);
	PXPublic PXActionResult PXAPI PXErrorCurrent();

#if PXOSWindowsDestop
	PXPublic PXActionResult PXAPI PXWindowsHandleErrorFromID(const HRESULT handleResult);
	PXPublic PXActionResult PXAPI PXWindowsMMAudioConvertFromID(const PXInt32U mmResultID);
#endif


#ifdef __cplusplus
}
#endif

#endif