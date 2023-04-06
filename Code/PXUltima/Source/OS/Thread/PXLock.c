#include "PXLock.h"

#include <OS/Memory/PXMemory.h>

void PXLockClear(PXLock* const lock)
{
	MemoryClear(lock, sizeof(PXLock));
}

PXActionResult PXLockCreate(PXLock* const lock, const PXLockType type)
{
#if OSUnix
	int sharedPointer = 0;
	unsigned int value = 1;

	const int resultID = sem_init(&lock->ID, sharedPointer, value);
	const unsigned char sucessful = resultID == 0; // 0=sucessful, -1=Error

	lock->Type = PXLockTypeGlobal;

#elif OSWindows
	
	switch (type)
	{
		case PXLockTypeGlobal:
		{
			LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = 0;
			LONG lInitialCount = 10;
			LONG lMaximumCount = 10;
			LPCWSTR lpName = L"BFE_ASYNC_LOCK";

			const HANDLE handle = CreateSemaphoreW(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
			const PXBool sucessful = handle != 0;

			if (!sucessful)
			{
				const PXActionResult actionResult = GetCurrentError();

				PXLockClear(lock);

				return actionResult;
			}

			lock->ID = handle;
			lock->Type = PXLockTypeGlobal;

			break;
		}
		case PXLockTypeProcessOnly:
		{
#if OSUnix

#elif OSWindows
			InitializeCriticalSection(&lock->LockCriticalSection);

			lock->Type = PXLockTypeProcessOnly;
#endif
		}
		default:
			return InvalidParameter;
	}

#endif

	return PXActionSuccessful;
}

PXActionResult PXLockDelete(PXLock* const lock)
{
	switch (lock->Type)
	{
		case PXLockTypeGlobal:
		{
			int closingResult = -1;

#if OSUnix
			closingResult = sem_destroy(&lock->ID);
#elif OSWindows
			closingResult = CloseHandle(lock->ID);
#endif

			break;
		}
		case PXLockTypeProcessOnly:
		{
#if OSUnix
			
#elif OSWindows
			DeleteCriticalSection(&lock->LockCriticalSection);
#endif	
		}
		default:
			return PXActionInvalidStateImpossible;
	}

    PXLockClear(lock);

	return PXActionSuccessful;
}

PXActionResult PXLockEngage(PXLock* const lock)
{
	switch (lock->Type)
	{
		case PXLockTypeGlobal:
		{
			int lockResult = -1;

#if OSUnix
			lockResult = sem_wait(&lock->ID);
#elif OSWindows
			lockResult = WaitForSingleObject(lock->ID, INFINITE);
#endif
			break;
		}
		case PXLockTypeProcessOnly:
		{
#if OSUnix

#elif OSWindows
			EnterCriticalSection(&lock->LockCriticalSection);
#endif	
		}
		default:
			return PXActionInvalidStateImpossible;
	}

	++(lock->LockCounter);

	return PXActionSuccessful;
}

PXActionResult PXLockRelease(PXLock* const lock)
{
	switch (lock->Type)
	{
		case PXLockTypeGlobal:
		{
			int releaseResult = -1;

#if OSUnix
			releaseResult = sem_post(&lock->ID);
#elif OSWindows
			releaseResult = ReleaseSemaphore(lock->ID, 1, 0);
#endif

			break;
		}
		case PXLockTypeProcessOnly:
		{
#if OSUnix
			
#elif OSWindows
			LeaveCriticalSection(&lock->LockCriticalSection);
#endif	
		}
		default:
			return PXActionInvalidStateImpossible;
	}

	--(lock->LockCounter);

	return PXActionSuccessful;
}
