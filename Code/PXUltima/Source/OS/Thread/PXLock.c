#include "PXLock.h"

#include <OS/Memory/PXMemory.h>

void PXLockClear(PXLock* const lock)
{
#if OSUnix
    MemoryClear(&lock->PXLockID, sizeof(PXLock));
#elif OSWindows
	lock->PXLockID = 0;
#endif
}

PXActionResult PXLockCreate(PXLock* const lock)
{
#if OSUnix
	int sharedPointer = 0;
	unsigned int value = 1;

	const int resultID = sem_init(&lock->PXLockID, sharedPointer, value);
	const unsigned char sucessful = resultID == 0; // 0=sucessful, -1=Error

#elif OSWindows
	LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = 0;
	LONG lInitialCount = 1;
	LONG lMaximumCount = 1;
	LPCWSTR lpName = L"BFE_ASYNC_LOCK";

	const HANDLE handle = CreateSemaphoreW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
	const unsigned char sucessful = handle != 0;

	lock->PXLockID = handle;
#endif

	if (!sucessful)
	{
        const PXActionResult actionResult = GetCurrentError();

		PXLockClear(lock);

        return actionResult;
	}

	return PXActionSuccessful;
}

PXActionResult PXLockDelete(PXLock* const lock)
{
	int closingResult = -1;

#if OSUnix
	closingResult = sem_destroy(&lock->PXLockID);
#elif OSWindows
	closingResult = CloseHandle(lock->PXLockID);
#endif

    PXLockClear(lock);

	return closingResult;
}

PXActionResult PXLockEngage(PXLock* const lock)
{
	int lockResult = -1;

#if OSUnix
	lockResult = sem_wait(&lock->PXLockID);
#elif OSWindows
	lockResult = WaitForSingleObject(lock->PXLockID, INFINITE);
#endif

	return lockResult;
}

PXActionResult PXLockRelease(PXLock* const lock)
{
	int releaseResult = -1;

#if OSUnix
	releaseResult = sem_post(&lock->PXLockID);
#elif OSWindows
	releaseResult = ReleaseSemaphore(lock->PXLockID, 1, 0);
#endif

	return releaseResult;
}
