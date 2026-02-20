#pragma once

#ifndef PXResultIncluded
#define PXResultIncluded

#include <PX/Media/PXType.h>

// Refused => User error
// Invalid => State errorPXActionRefusedArgumentInvalid
// Failed => Fundamental error
// [Note]
// enum by defined API forced to be 32-bit.
// Reducing this type yields no actual smaler code.
typedef enum PXResult_
{
    //---<General>-----------------------
    PXResultInvalid, // Default iniitlialisation value, do not use.
    PXResultOK, // Action finished successful
    PXActionSuccessWithSubstitution,
    PXActionCancelled, // Action was canceled early.
    PXActionDidNothing, // if the function did nothing
    PXActionFailedUnkownError, // [OTHER] Undefined error
    //-----------------------------------

    // Functions
    PXResultRefusedParameterNull, // Some needed Parameter is null
    PXResultRefusedParameterInvalid, // [INVAL] Any given argument is invalid

    // Soft-Exceptions
    PXResultRefusedNoPlaceToStore,
    PXResultRefusedNotEnoughMemory,

    // Hard-Exceptions
    PXResultExceptionAccessViolation, // [SIGSEGV] Read or write from invalid adress, EXCEPTION_ACCESS_VIOLATION
    PXResultExceptionIndexOutOfBounds, // Index of an array is invalid
    PXResultExceptionDataMisalignment,
    PXResultExceptionIllegalInstruction, // [SIGILL] Invalid or unsupported insruction executed by CPU.
    PXResultExceptionPageLost, // STATUS_IN_PAGE_ERROR




    PXResultExceptionFloatDENORMAL_OPERAND,
    PXResultExceptionFloatDIVIDE_BY_ZERO,
    PXResultExceptionFloatINEXACT_RESULT,
    PXResultExceptionFloatINVALID_OPERATION,
    PXResultExceptionFloatOVERFLOW,
    PXResultExceptionFloatSTACK_CHECK,
    PXResultExceptionFloatUNDERFLOW,
    PXResultExceptionINTEGER_DIVIDE_BY_ZERO,
    PXResultExceptionINTEGER_OVERFLOW,
    PXResultExceptionPrivilehedInstruction,

    PXResultExceptionNONCONTINUABLE_EXCEPTION,
    PXResultExceptionSTACK_OVERFLOW,
    PXResultExceptionINVALID_DISPOSITION,
    PXResultExceptionGUARD_PAGE_VIOLATION,
    PXResultExceptionINVALID_HANDLE,
    //STATUS_POSSIBLE_DEADLOCK,
    PXResultExceptionControlCExit, // [SIGABRT]

    // Debugging
    PXResultDebugEventBreakPoint, // [SIGBREAK]
    PXResultDebugEventSingleStep,



    PXActionWaitOnResult, // The result is yet to be recieved

    PXActionRefusedKeyNotFound, // Key-Value, key not found
    PXActionRefusedValueNotFound, // Key-value, value not found

    PXActionFailedLockEnter,

    PXActionFailedWaitTimeout,
    PXActionFailedWaitAbandoned,

    PXActionFailedRead, // Value couln't be read
    PXActionFailedRegister, // Value couln't be registered
    PXActionFailedWrite, // Value couln't be written
    PXActionFailedDelete,
    PXActionFailedUpdate,
    PXActionFailedCreate, // Creation of a resource failed
    PXActionFailedLoad, // Loadin a resource failed
    PXActionFailedCleanup,
    PXActionFailedSave, // Saving a resource failed
    PXActionFailedOpen, // Opening a resource failed
    PXActionFailedClose, // Closing a resource failed
    PXActionFailedSend, // Tryed to send data but failed
    PXActionFailedRecieve, // Expected data to be recieved was not successful

    PXActionFailedStackOverflow,
    PXActionFailedStackUnderflow,

    PXActionFailedNoSystemToHandleData, // Action can not compleated, a vital component is missing or not loaded.

    PXActionRefusedNoDriver, // An object needs a driver to function but no driver is attached
    PXActionRefusedFeatureNotAvailable, // Setting requested is not available on this device

    PXActionCanceledLoopDeteced, // LOOP

    //-------------------------------------------------------
    // Memory
    //-------------------------------------------------------
    PXActionRefusedAdressInvalid, // Attempt to access invalid address.

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
    PXActionRefusedTypeMissmatch, // Fields expected to be same type are different types.

    //-------------------------------------------------------
    // Object / API / Library
    //-------------------------------------------------------
    PXActionRefusedNotInitialized, // Object can't be used before it has been initialized
    PXActionFailedInitialization, // Initialization of this object was tyred but failed while doing so
    PXActionFailedContextBinding, // Initialization was ok, binding the data to the thread failed
    PXActionFailedContextRelease,
    PXActionRefusedContextNotBound, // API is not bound to session
    PXActionRefusedAlreadyInizialized, // Object has already been initialized
    PXActionRefusedAlreadyExists, // [EXIST] Object can't be created, as it alteady exists
    PXActionRefusedTypeInvalid,
    PXActionRefusedNoSuchFunction, // When the OS does not support that function or it does not exist
    PXActionRefusedTypeNotSupported,
    PXActionRefusedAggregationNotSupported,
    PXActionRefusedPropertyNotAvailable,
    PXActionRefusedNotReady, // Object can't process this function, setup is needed or its busy
    PXActionRefusedNotFound, // [404] Invalid ID or HANDLE -> Does not exist
    PXActionRefusedStateInvalid,
    PXActionRefusedStateDoesNotAllowAction, // Action can't be used in this current state
    PXActionRefuedObjectInterfaceNotAvailable,

    PXActionRefusedObjectCreateCallbackMissing, // PX-OBJ-CREATE-FUN-MISSING


    PXActionRefusedObjectSizeZero,
    PXActionRefusedObjectAlreadyExists,
    PXActionRefusedObjectNotCreated,    // PX-OBJ-NOID
    PXActionRefusedObjectIDInvalid,     // Object handle invalid
    PXActionRefusedObjectNameInvalid,   // PX-NAMEINVAL
    PXActionRefusedWindowsClassNameInvalid, 
    PXActionRefusedNameSpaceInvalid,
    PXActionRefusedObjectParentInvalid,
    PXActionRefusedObjectInternalDataMissing,

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

    // Resource is available and has been loaded but containing
    // elements caused a compile error and thus can't be used.
    PXActionFailedResourceCompile,


    PXActionRefusedDirectoryNotEmpty, // [NOTEMPTY]
    PXActionRefusedTargetIsDirectory, // [ISDIR]
    PXActionRefusedNotADirectory, // [NOTDIR]

    PXActionFailedFileNameTooLong, // NAMETOOLONG

    PXActionFailedFileDescriptorValueTooLarge, // MFILE
    PXActionFailedTooManyLinks, // MLINK
    PXActionFailedFileTooLarge, // FBIG
    PXActionRefusedInvalidSeek, // SPIPE
    PXActionFailedTooManyFilesOpenInSystem, // NFILE


    PXActionRefusedDeviceFunctionNotSupported, // Tryed to call a function on a device that is not capable of doing so.

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
    PXActionRefusedConnectionClosed,


  

    //-------------------------------------------------------
    // Library
    //-------------------------------------------------------
    PXActionRefusedNotSupportedByOperatingSystem, // Given action can't be done due the OS not having the ability to do so
    PXActionRefusedNotSupportedByLibrary, // [NOTSUP] Action can't be exectuted, as the function does not exist to be used.
    PXActionRefusedNotImplemented, // Function that exists but does not contain any code

    PXActionFailedNotFoundObject, // Object was searched but not found
    PXActionFailedNotFoundLibrary, // Object was searched but not found
    PXActionFailedNotFoundClass, // Object was searched but not found
    PXActionFailedNotFoundMethod, // Method does not exist in library

    PXActionRefusedVersionTooOld, // Object can't be used, it is requires an update
    PXActionRefusedVersionTooNew, // Object can't be used, the system requires an update
    PXActionRefusedVersionInvalid, // Object version is not defined

    PXActionRefusedMissingCallBack, // Callback is required but missing

    PXActionFailedRelease, // Cleanup of an object failed



    PXActionInterruptedByEvent,
    PXActionInterruptedByFunctionCall,
    PXActionFailedResourcedNotEnough,
    PXActionWouldBlock,
    PXActionNowInProgress,
    PXActionAlreadyInProgress,
    PXActionRefusedInvalidOperationSpecified,
    PXActionRefuedNetworkNotConnected,
    PXActionRefuedNetworkNotReachable,
    PXActionFailedHostNotFound,
    PXActionRefuedServiceNotRunning,
    PXActionRefuedAlreadyInUse,
    PXActionRefuedProtocolTypeInvalid,
    PXActionRefuedProtocolOptionInvalid,
    PXActionRefuedProtocolNotSupported,
    PXActionRefuedProtocolFamilyNotSupported,
    PXActionRefuedAddressFamilyNotSupportedByProtocol,
    PXActionRefuedOperationNotSupported,

    //---<Function input>---
    PXActionRefuedAdressInvalid, // System detected an invalid pointer

    PXActionRefuedTextFormatUnsupported,
    PXActionRefuedInputInvalid,
    PXActionInvalidStateImpossible, // Is an object is used and has data that should not be possible
    PXActionFailedElementNotFound,
    PXActionInvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly
    PXActionRefuedDriverInvalid,
    PXActionRefuedDriverMissing,
    PXActionRefuedFormatIllegal, // Parsed format is not a legal option. The file is wrongly written or corrupted
    PXActionRefuedFormatNotSupported,


        // Images
        PXActionRefuedImageWidthIsZero,
        PXActionRefuedImageHeightIsZero,
        PXActionRefuedImageDepthIsZero,
        PXActionRefuedImageFormatInvalid,


    //---<File-General>------------------
    PXActionRefusedEmptyFilePath,
    PXActionRefusedInvalidFilePath,

    PXActionFailedFileNotFound,
    PXActionFailedNotAFile,

    PXActionFailedFileMapping,// mmap() / CreateViewofFile() failed


    //---<Socket>---------------------
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
    PXActionFailedFormatNotDetected, // Format is not detected and thus unknown
    PXActionRefusedFormatNotSupported, // Fromat was detected but is not supported
    PXActionFailedFormatNotAsExpected, // Excpected format differs from what is detected
    PXActionRefusedInvalidHeaderSignature, // Expected signature was not detected
    //-----------------------------------

    //---<File-Writing>------------------
    PXActionRefusedFormatSettingNotAllowed,
    //-----------------------------------





    //---<Service>-----------------------
    PXActionNoResposeCountNotConnect,
    PXActionFailedConnectionFailure,
    PXActionRefusedNotConnected, // Trying to use a command that requires an already established connection.
    PXActionFailedSettingsInvalid,
    PXActionFailedCommandDeploy, // Failure to awns from source to target
    PXActionFailedCommandExecute, // Faiure to execute on target
    PXActionFailedCommandRetrieve, // Failure to retrieve result from target
    //-----------------------------------

    //---<Compiler>----------------------
    PXActionRefusedParserSymbolNotAsExpected, // While parsing found symbol thats not supported to be read
    //-----------------------------------

    //---[ Windows Only (WindowsSocketAgent) ]-------
    SubSystemNotReady,
    BlockedByOtherOperation,
    LimitReached,
    SocketIsBlocking,

    PXActionFailedDirectoryIsNotEmpty,
    PXActionFailedTooManyProcesses,
    PXActionFailedUserQuotaExceeded,
    PXActionFailedDiskQuotaExceeded,
    PXActionFailedHandleIsStale,
    PXActionFailedResourceNotAvailableLocally,
    //-------------------------------------------------

    //---<Audio>---------------------------------------


    AudioResultDriverNotReachable, // Maybe diabled, or deactivated
    AudioResultDriverNotEnabled, // No driver to use

    AudioResultDeviceAlreadyAllocated,
    AudioResultErrorValueInvalid,
    AudioResultFlagInvalid,
    AudioResultDeviceHandleBusy,
    AudioResultAliasNotFound,
    AudioResultMoreData,
    // Custom
    AudioResultDeviceListNotBigEnough,
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
    PXActionFailedResourceUnavailableTryAgain, // [AGAIN]
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
PXResult;


typedef struct PXExecutionSnapshot_
{
    int x;
}
PXExecutionSnapshot;


PXPublic PXResult PXAPI PXErrorCodeFromID(const int errorCode);
PXPublic PXResult PXAPI PXErrorCurrent(const PXBool wasSuccessful);

#if PXOSWindowsDestop

// Interesting behaviour, you put in if we successfully did the last operation
// If so, we just convert it into a successful error code, if not, fetch the
// correct error
PXPublic PXResult PXAPI PXErrorFromHRESULT(const HRESULT handleResult);
PXPublic PXResult PXAPI PXWindowsMMAudioConvertFromID(const PXI32U mmResultID);
#endif

typedef PXResult(PXAPI* PXCallX1)(void PXREF pxCallX1);

PXPublic PXResult PXAPI PXSafeCall(PXCallX1 pxCallX1, void* p1);


typedef struct PXException_
{
    PXI32U WindowCode;
    void* FaultAddress;
}
PXException;

PXPublic void PXAPI PXExceptionTrigger(PXException PXREF pxException);

#endif
