#pragma once

#ifndef PXResultIncluded
#define PXResultIncluded

#include <PX/Type/PXType.h>

// Refused => User error
// Invalid => State errorPXResultRefusedArgumentInvalid
// Failed => Fundamental error
// [Note]
// enum by defined API forced to be 32-bit.
// Reducing this type yields no actual smaler code.
typedef enum PXResult_
{
    //---<General>-----------------------
    PXResultInvalid, // Default iniitlialisation value, do not use.
    PXResultUnknown, // Error code, undefined. Only use to handle unkown errors from APIs.
    PXResultOK, // Action finished successful
    PXResultSuccessWithSubstitution,
    PXResultCancelled, // Action was canceled early.
    PXResultNoWorkToDo, // if the function did nothing
    PXResultFailedUnkownError, // [OTHER] Undefined error
    //-----------------------------------

    // Exit
    PXResultExitControlC, // [SIGINT][CONTROL_C_EXIT] Console event of control+C

    // Functions
    PXResultRefusedParameterNull, // Some needed Parameter is null
    PXResultRefusedParameterInvalid, // [INVAL] Any given argument is invalid

    // Soft-Exceptions
    PXResultRefusedNoPlaceToStore,
    PXResultRefusedNotEnoughMemory,

    // Hard-Exceptions
    PXResultExceptionAccessViolation, // [SIGSEGV][EXCEPTION_ACCESS_VIOLATION] Read or write from invalid address.
    PXResultExceptionIndexOutOfBounds, // Index of an array is invalid
    PXResultExceptionDataMisalignment,
    PXResultExceptionIllegalInstruction, // [SIGILL][EXCEPTION_ILLEGAL_INSTRUCTION] Invalid or unsupported insruction executed by CPU.
    PXResultExceptionPageLost, // [EXCEPTION_IN_PAGE_ERROR] Running program via network, connection is lost while running.



    // Floats
    PXResultExceptionFloatArithmeticFault, // [SIGFPE]
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
    PXResultExceptionStackOverflow, // [EXCEPTION_STACK_OVERFLOW][GL_STACK_OVERFLOW]
    PXResultExceptionStackUnderflow, // [GL_STACK_UNDERFLOW]
    PXResultExceptionINVALID_DISPOSITION, // [EXCEPTION_INVALID_DISPOSITION]
    PXResultExceptionGUARD_PAGE_VIOLATION, // [EXCEPTION_GUARD_PAGE]
    PXResultRefusedHandleInvalid, // [EXCEPTION_INVALID_HANDLE] 
    //STATUS_POSSIBLE_DEADLOCK,
    PXResultExceptionControlCExit,        // [SIGINT]
    PXResultExceptionTerminateRequest,    // [SIGTERM]
    PXResultExceptionAbort,               // [SIGABRT]

    // Debugging
    PXResultDebugEventBreakPoint, // [SIGBREAK][EXCEPTION_BREAKPOINT]
    PXResultDebugEventSingleStep, // [EXCEPTION_SINGLE_STEP]

    /*
      PXSignalTokenInvalid,
    PXSignalTokenAbort, // Abnormal termination
    PXSignalTokenPXF32MathError, // PXF32ing-point errorc
    PXSignalTokenIllegalInstruction, // Illegal instruction
    PXSignalTokenINT, // CTRL+C signal
    PXSignalTokenMemoryViolation, // Illegal storage access
    PXSignalTokenTerminationRequest // Termination request
    */


    //-----------------------------------------------------
    // Window
    PXResultRefusedWindowDoesNotExist,
    PXResultRefusedWindowHandleNull,
    PXResultRefusedWindowInvalid, // BadWindow, GLXBadCurrentWindow
    PXResultRefusedWindowSurfaceHidden,
    PXResultRefusedWindowSurfaceSizeInvalid,
    //-----------------------------------------------------



    //-----------------------------------------------------
    // Object
    //-----------------------------------------------------
    
    // Resource is already beeing used (busy) and is not allowed to be used by other party (you).
    PXResultRefusedInstanceAlreadyInUse, // ERROR_BUSY:170, 
    //-----------------------------------------------------


    PXResultRefusedRequestInvalid, // BadRequest
    PXResultRefusedValueInvalid, // BadValue

    PXResultRefusedPixmapInvalid, // BadPixmap
    PXResultRefusedAtomInvalid, // BadAtom
    PXResultRefusedCursorInvalid, // BadCursor
    PXResultRefusedFontInvalid, // BadFont
    PXResultRefusedIncompatible, // BadMatch
    PXResultRefusedDrawableInvalid, // BadDrawable, GLXBadDrawable
    PXResultRefusedAccessDenied, // BadAccess
    PXResultRefusedColorInvalid, // BadColor
    PXResultRefusedGraphicsContextInvalid, // BadGC
    PXResultRefusedResourceIDInvalid, // BadIDChoice
    PXResultRefusedNameInvalid, // BadName
    PXResultRefusedLengthInvalid, // BadLength
    PXResultRefusedOperationUnsupported, // BadImplementation

    // Draw, opengl, drectx, vulcan
    PXResultRefusedContextInvalid, // GLXBadContext
    PXResultRefusedContextStateInvalid, // GLXBadContextState


    PXResultRefusedPayloadDataMissing, // When we upload data to somewhere but we didnt select any

    //-------------------------------------------------------   
    // Compile
    //-------------------------------------------------------
    PXResultFailedCompilerCodeBad,
    PXResultFailedLinkerCodeMissmatch,


    PXResultWaitOnResult, // The result is yet to be recieved

    PXResultRefusedKeyNotFound, // Key-Value, key not found
    PXResultRefusedValueNotFound, // Key-value, value not found

    PXResultFailedLockEnter,

    PXResultFailedWaitTimeout,
    PXResultFailedWaitAbandoned,

    PXResultFailedRead, // Value couln't be read
    PXResultFailedRegister, // Value couln't be registered
    PXResultFailedWrite, // Value couln't be written
    PXResultFailedDelete,
    PXResultFailedUpdate,
    PXResultFailedCreate, // Creation of a resource failed
    PXResultFailedLoad, // Loadin a resource failed
    PXResultFailedCleanup,
    PXResultFailedSave, // Saving a resource failed
    PXResultFailedOpen, // Opening a resource failed
    PXResultFailedClose, // Closing a resource failed
    PXResultFailedSend, // Tryed to send data but failed
    PXResultFailedRecieve, // Expected data to be recieved was not successful

    PXResultFailedNoSystemToHandleData, // Action can not compleated, a vital component is missing or not loaded.

    PXResultRefusedNoDriver, // An object needs a driver to function but no driver is attached
    PXResultRefusedFeatureNotAvailable, // Setting requested is not available on this device

    PXResultCanceledLoopDeteced, // LOOP

    //-------------------------------------------------------
    // Memory
    //-------------------------------------------------------
    PXResultRefusedAddressInvalid, // Attempt to access invalid address.

    PXResultFailedMemoryAllocation, // BadAlloc, A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
    PXResultFailedMemoryRelease,

    PXResultRefusedIndexOutOfBounce, // [EXCEPTION_ARRAY_BOUNDS_EXCEEDED] A given index is outside of the given space of the array.

    PXResultFailedBufferRefernceLost, // Windows lost a buffer and it needs to be restord
    PXResultRefuedBufferSizeTooSmal,

PXResultRefuedInputBufferNull, // is NULL
PXResultRefuedInputBufferEmpty, // has no data, address is not NULL but the data is
PXResultRefuedInputBufferTooSmal, // cant
PXResultRefuedInputBufferTooBig,

PXResultRefuedOutputBufferNull, // is NULL
PXResultRefuedOutputBufferTooSmal, //

//-------------------------------------------------------
// Type
//-------------------------------------------------------
PXResultRefusedTypeMissmatch, // Fields expected to be same type are different types.

//-------------------------------------------------------
// Object / API / Library
//-------------------------------------------------------
PXResultRefusedNotInitialized, // Object can't be used before it has been initialized
PXResultFailedInitialization, // Initialization of this object was tyred but failed while doing so
PXResultFailedContextBinding, // Initialization was ok, binding the data to the thread failed
PXResultFailedContextRelease,
PXResultRefusedContextNotBound, // API is not bound to session
PXResultRefusedAlreadyInizialized, // Object has already been initialized
PXResultRefusedAlreadyExists, // [EXIST] Object can't be created, as it alteady exists
PXResultRefusedAlreadyRegisterd,
PXResultRefusedTypeInvalid,
PXResultRefusedNoSuchFunction, // When the OS does not support that function or it does not exist
PXResultRefusedTypeNotSupported,
PXResultRefusedAggregationNotSupported,
PXResultRefusedPropertyNotAvailable,
PXResultRefusedNotReady, // Object can't process this function, setup is needed or its busy
PXResultRefusedNotFound, // [404] Invalid ID or HANDLE -> Does not exist
PXResultRefusedStateInvalid,
PXResultRefusedStateAlreadyLikeRequested, // Requested state change but state is already like requested.
PXResultRefusedStateDoesNotAllowAction, // Action can't be used in this current state
PXResultRefuedObjectInterfaceNotAvailable,

PXResultRefusedObjectCreateCallbackMissing, // PX-OBJ-CREATE-FUN-MISSING


PXResultRefusedObjectSizeZero,
PXResultRefusedObjectAlreadyExists,
PXResultRefusedObjectNotCreated,    // PX-OBJ-NOID
PXResultRefusedHandleNull, // Proviced handle is null
PXResultRefusedTextNull,
PXResultRefusedObjectIDInvalid,     // Object handle invalid
PXResultRefusedObjectNameInvalid,   // PX-NAMEINVAL
PXResultRefusedWindowsClassNameInvalid,
PXResultRefusedNameSpaceInvalid,
PXResultRefusedObjectParentInvalid,
PXResultRefusedObjectInternalDataMissing,

//-------------------------------------------------------
// Permission
//-------------------------------------------------------
PXResultDeniedNoPermission, // [ACCES] You do not have permission to use this command.
PXResultDeniedPriorityTooLow,
PXResultYieldToOtherProcessWithHigherPriority,
//PXResultDenied

//-------------------------------------------------------
// File I/O
//-------------------------------------------------------

// Resource is available and has been loaded but containing
// elements caused a compile error and thus can't be used.
PXResultFailedResourceCompile,


PXResultRefusedDirectoryNotEmpty, // [NOTEMPTY]
PXResultRefusedTargetIsDirectory, // [ISDIR]
PXResultRefusedNotADirectory, // [NOTDIR]

PXResultFailedFileNameTooLong, // NAMETOOLONG

PXResultFailedFileDescriptorValueTooLarge, // MFILE
PXResultFailedTooManyLinks, // MLINK
PXResultFailedFileTooLarge, // FBIG
PXResultRefusedInvalidSeek, // SPIPE
PXResultFailedTooManyFilesOpenInSystem, // NFILE


PXResultRefusedDeviceFunctionNotSupported, // Tryed to call a function on a device that is not capable of doing so.

//-------------------------------------------------------
// Network
//-------------------------------------------------------
PXResultRefusedAlreadyConnected, // [ISCONN]
PXResultRefusedNetworkIsDown, // [NETDOWN]
PXResultFailedConnectionAbortedByNetwork, // [NETRESET]
PXResultFailedNetworkUnreachable, // [NETUNREACH]
PXResultRefusedNotASocket, // [NOTSOCK]
PXResultFailedConnectionTimedOut, // [TIMEDOUT]
PXResultFailedNotConnected, // [NOTCONN]
PXResultRefusedConnectionClosed,


//-------------------------------------------------------
// ASYNC
//-------------------------------------------------------

PXResultRefusedThreadOwnershipViolation,  // Resource is not allowed to be interacted from unowned thread.


    //-------------------------------------------------------
    // Library
    //-------------------------------------------------------
    PXResultRefusedNotSupportedByOperatingSystem, // Given action can't be done due the OS not having the ability to do so
    PXResultRefusedNotSupportedByLibrary, // [NOTSUP] Action can't be exectuted, as the function does not exist to be used.
    PXResultRefusedNotImplemented, // Function that exists but does not contain any code

    PXResultFailedNotFoundObject, // Object was searched but not found
    PXResultFailedNotFoundLibrary, // Object was searched but not found
    PXResultFailedNotFoundClass, // Object was searched but not found
    PXResultFailedNotFoundMethod, // Method does not exist in library

    PXResultRefusedVersionTooOld, // Object can't be used, it is requires an update
    PXResultRefusedVersionTooNew, // Object can't be used, the system requires an update
    PXResultRefusedVersionInvalid, // Object version is not defined

    PXResultRefusedMissingCallBack, // Callback is required but missing

    PXResultFailedRelease, // Cleanup of an object failed



    PXResultInterruptedByEvent,
    PXResultInterruptedByFunctionCall,
    PXResultFailedResourcedNotEnough,
    PXResultWouldBlock,
    PXResultNowInProgress,
    PXResultAlreadyInProgress,
    PXResultRefusedInvalidOperationSpecified,
    PXResultRefuedNetworkNotConnected,
    PXResultRefuedNetworkNotReachable,
    PXResultFailedHostNotFound,
    PXResultRefuedServiceNotRunning,
    PXResultRefuedAlreadyInUse,
    PXResultRefuedProtocolTypeInvalid,
    PXResultRefuedProtocolOptionInvalid,
    PXResultRefuedProtocolNotSupported,
    PXResultRefuedProtocolFamilyNotSupported,
    PXResultRefuedAddressFamilyNotSupportedByProtocol,
    PXResultRefuedOperationNotSupported,

    //---<Function input>---
    PXResultRefuedAddressInvalid, // System detected an invalid pointer

    PXResultRefuedTextFormatUnsupported,
    PXResultRefuedInputInvalid,
    PXResultInvalidStateImpossible, // Is an object is used and has data that should not be possible
    PXResultFailedElementNotFound,
    PXResultInvalidRedundandInteraction, // The function would have no effect and is cancelled imidiatly
    PXResultRefuedDriverInvalid,
    PXResultRefuedDriverMissing,
    PXResultRefuedFormatIllegal, // Parsed format is not a legal option. The file is wrongly written or corrupted
    PXResultRefuedFormatNotSupported,


    // Images
    PXResultRefuedImageWidthIsZero,
    PXResultRefuedImageHeightIsZero,
    PXResultRefuedImageDepthIsZero,
    PXResultRefuedImageFormatInvalid,


    //---<File-General>------------------
    PXResultRefusedEmptyFilePath,
    PXResultRefusedInvalidFilePath,

    PXResultFailedFileNotFound,
    PXResultFailedNotAFile,

    PXResultFailedFileMapping,// mmap() / CreateViewofFile() failed


    //---<Socket>---------------------
    PXResultRefusedSocketNotConnected,

    PXResultFailedSocketConnect,
    PXResultFailedSocketCreation,
    PXResultFailedSocketOption,
    PXResultFailedSocketBinding,
    PXResultFailedSocketListening,

    PXResultRefusedSocketInavlidAddressFamily,
    PXResultRefusedSocketInvalidFlags,
    PXResultRefusedNameServerIsTemporaryOffline,
    PXResultRefusedNameServerIsPermanentOffline,
    PXResultRefusedSocketTypeNotSupported,
    PXResultRefusedRequestedAddressFamilyNotSupported,
    PXResultRefusedHostExistsButHasNoData,
    PXResultRefusedHostHasNoNetworkAddresses,
    PXResultRefusedIPOrPortNotKnown,
    //-----------------------------------

    //---<File-Parsing>-------------------
    PXResultFailedFormatNotDetected, // Format is not detected and thus unknown
    PXResultRefusedFormatNotSupported, // Fromat was detected but is not supported
    PXResultFailedFormatNotAsExpected, // Excpected format differs from what is detected
    PXResultRefusedInvalidHeaderSignature, // Expected signature was not detected
    //-----------------------------------

    //---<File-Writing>------------------
    PXResultRefusedFormatSettingNotAllowed,
    //-----------------------------------





    //---<Service>-----------------------
    PXResultNoResposeCountNotConnect,
    PXResultFailedConnectionFailure,
    PXResultRefusedNotConnected, // Trying to use a command that requires an already established connection.
    PXResultFailedSettingsInvalid,
    PXResultFailedCommandDeploy, // Failure to awns from source to target
    PXResultFailedCommandExecute, // Faiure to execute on target
    PXResultFailedCommandRetrieve, // Failure to retrieve result from target
    //-----------------------------------

    //---<Compiler>----------------------
    PXResultRefusedParserSymbolNotAsExpected, // While parsing found symbol thats not supported to be read
    //-----------------------------------

    //---[ Windows Only (WindowsSocketAgent) ]-------
    SubSystemNotReady,
    BlockedByOtherOperation,
    LimitReached,
    SocketIsBlocking,

    PXResultFailedDirectoryIsNotEmpty,
    PXResultFailedTooManyProcesses,
    PXResultFailedUserQuotaExceeded,
    PXResultFailedDiskQuotaExceeded,
    PXResultFailedHandleIsStale,
    PXResultFailedResourceNotAvailableLocally,
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

    PXResultFailedConnectionTerminatedByNetwork,
    PXResultFailedConnectionTerminatedByOwnSoftware,
    PXResultFailedConnectionTerminatedByPeer,

    PXResultFailedMemoryAllocationInternal,


    // POSIX Error codes, these are pre-defined

    AddressInUse, // ADDRINUSE
    AddressNotAvailable, // ADDRNOTAVAIL
    AddressFamilyNotSupported, // AFNOSUPPORT
    PXResultFailedResourceUnavailableTryAgain, // [AGAIN]
    ConnectionAlreadyInProgress, // ALREADY
    ArgumentListTooLong, // 2BIG
    BadFileDescriptor, // BADF
    BadMessage, // BADMSG
    PXResultRefusedResourceBusy, // BUSY
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

#if OSUnix
PXPublic PXResult PXAPI PXErrorFromX11(const int errorID);
#endif


#if PXOSWindowsDestop

// Interesting behaviour, you put in if we successfully did the last operation
// If so, we just convert it into a successful error code, if not, fetch the
// correct error
PXPublic PXResult PXAPI PXErrorFromHRESULT(const HRESULT handleResult);
PXPublic PXResult PXAPI PXWindowsMMAudioConvertFromID(const PXI32U mmResultID);
#endif

typedef PXResult(PXAPI* PXCallX1)(void PXREF parameterA);
typedef PXResult(PXAPI* PXCallX2)(void PXREF parameterA, void PXREF parameterB);

PXPublic PXResult PXAPI PXSafeCallx1(PXCallX1 pxCallX1, void* p1);
PXPublic PXResult PXAPI PXSafeCallx2(PXCallX2 pxCallX2, void* p1, void* p2);

typedef struct PXException_
{
    PXI32U WindowCode;
    void* FaultAddress;

    PXResult Result;
    PXBool Triggered;
}
PXException;

PXPublic void PXAPI PXExceptionTrigger(PXException PXREF pxException);

#endif