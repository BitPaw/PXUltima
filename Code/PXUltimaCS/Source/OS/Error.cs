namespace PX
{
    public enum ActionResult
    {
        //---<General>-----------------------
        Invalid, // Default iniitlialisation value, do not use.
        Successful, //  finished successful
        DidNothing, // if the function did nothing
        FailedMemoryAllocation, // A call to malloc failed due to not enough memory or the buffer is too big to fit in a free memory block. [NOMEM]
        FailedMemoryRelease,
        DeniedNoPermission,
        //-----------------------------------

        NotImplemented,
        NotSupportedByLibrary,
        NotSupportedByOperatingSystem,

        RefusedMissingCallBack,

        FailedInitialization,
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



        RefuedObjectTypeInvalid,
        RefuedObjectTypeNotSupported,

        RefuedOperationNotSupported,

        Cancelled,

        //---<Object>------------
        RefuedObjectNotReady, // Object can't process this function, setup is needed or its busy
        RefuedObjectIDInvalid, // Use if you have an invalid ID or HANDLE
                                       //-----------------------


        //---<Function input>---
        RefuedAdressInvalid, // System detected an invalid pointer


        RefuedParameterNull, // Some needed Parameter is null

        RefuedTextFormatUnsupported,


        RefuedInputInvalid,

        RefuedInputBufferNull, // is NULL
        RefuedInputBufferEmpty, // has no data, adress is not NULL but the data is
        RefuedInputBufferTooSmal, // cant 
        RefuedInputBufferTooBig,

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
        InvalidParameter,
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

        AudioResultErrorUnspecified,        // Bad, but windows, am i right?



        // only
        AudioResultWaveFormatUnsupported,
        AudioResultDeviceIsStillPlaying,
        AudioResultReaderIsNotPrepared,
        AudioResultDeviceIsSynchronous,
        //-------------------------------------------------

        FailedConnectionTerminatedByNetwork,
        FailedConnectionTerminatedByOwnSoftware,
        FailedConnectionTerminatedByPeer,

        FailedMemoryAllocationInternal,


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



        RefusedResourceBusy, // BUSY
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
        RefusedAlreadyConnected, // ISCONN
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
        FailedNotConnected, // NOTCONN
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
}