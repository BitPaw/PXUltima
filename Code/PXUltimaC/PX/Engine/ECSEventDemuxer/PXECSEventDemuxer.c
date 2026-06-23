#include "PXECSEventDemuxer.h"

typedef struct PXECSEventDemuxer_
{
#if OSUnix
    int Dummy;
#elif OSWindows
    HANDLE IOCompletionPortHandle;
#endif
}
PXECSEventDemuxer;

PXECSEventDemuxer _PXECSEventDemuxer;

PXResult PXAPI PXECSEventDemuxerCreate()
{
#if OSUnix
#elif OSWindows
    _PXECSEventDemuxer.IOCompletionPortHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
#endif

    return PXResultOK;
}

PXResult PXAPI PXECSEventDemuxerRelease()
{
#if OSUnix
#elif OSWindows
    BOOL result = CloseHandle(_PXECSEventDemuxer.IOCompletionPortHandle);
#endif

    return PXResultOK;
}

#if OSUnix
#elif OSWindows
PXResult PXAPI PXECSEventDemuxerHandleAdd(HANDLE winHandle, PXECSInfo PXREF pxECSPayload)
{
    // Associate the handle with the IOCP with userdata pointer.
    // Result is just the same, we can ignore it.
    const HANDLE ioCompletionPortHandle = CreateIoCompletionPort
    (
        winHandle,
        _PXECSEventDemuxer.IOCompletionPortHandle,
        (ULONG_PTR)pxECSPayload,
        0
    );

    return PXResultOK;
}

PXResult PXAPI PXECSEventDemuxerHandleRemove(HANDLE winHandle)
{
    // Removing is implicit: IOCP has no explicit “remove”,
    // but closing the handle stops further events.
    CloseHandle(winHandle);
    return 0;
}
#endif

PXResult PXAPI PXECSEventDemuxerWaitCancel()
{
#if OSUnix
#elif OSWindows
    // Post a dummy completion to wake the wait
    BOOL result = PostQueuedCompletionStatus
    (
        _PXECSEventDemuxer.IOCompletionPortHandle,
        0, 
        (ULONG_PTR)NULL, 
        NULL
    );
#endif

    return 0;
}

PXResult PXAPI PXECSEventDemuxerWait()
{
#if OSUnix
#elif OSWindows
    PXECSEventInfo* pxECSPayload;
    LPOVERLAPPED overLappedData;
    DWORD bytesRead = 0; 
    DWORD waitingTime = INFINITE; // We want to wait forever in the beginning.

    for(;;)
    {
        BOOL ok = GetQueuedCompletionStatus
        (
            _PXECSEventDemuxer.IOCompletionPortHandle,
            &bytesRead,
            &pxECSPayload,
            &overLappedData,
            waitingTime
        );
        PXBool hasPayLoad = pxECSPayload != (ULONG_PTR)NULL;
        PXBool hasBytesRead = bytesRead != 0;
        PXBool wasCanceled = !hasPayLoad && !hasBytesRead && overLappedData == NULL;

        if(wasCanceled)
            break;

        // Trigger event, we got one!
        if(pxECSPayload->EventCallBack)
        {
            pxECSPayload->EventCallBack(pxECSPayload);
        }

        // We prepare for the next loop
        waitingTime = 0; // Next fetch time reduced to zero!
    }
#endif

    return PXResultOK;
}