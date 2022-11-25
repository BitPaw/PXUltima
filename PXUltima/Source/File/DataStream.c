#include "DataStream.h"

#include <OS/OSVersion.h>
#include <Memory/PXMemory.h>
#include <Math/PXMath.h>
#include <Text/Text.h>

#include <iso646.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>

#define MemoryPageLargeEnable 0

//-----------------------------------------------------------------------------
#define FileCachingModeDefault 0 //POSIX_FADV_NORMAL

#if OSUnix

#define FileCachingModeSequentialScan 2 // POSIX_FADV_SEQUENTIAL
#define FileCachingModeRandomAccess  1 // POSIX_FADV_RANDOM
#define FileCachingModeWillNeed 3 // POSIX_FADV_WILLNEED
#define FileCachingModeDontNeed 4 // POSIX_FADV_DONTNEED
#define FileCachingModeNoReuse 5 // POSIX_FADV_NOREUSE

#elif OSWindows
// From WinBase.h

#define FileCachingModeWriteThrough      0x80000000 // FILE_FLAG_WRITE_THROUGH
#define FileCachingModeOverLapped        0x40000000
#define FileCachingModeNoBuffering       0x20000000 // FILE_FLAG_NO_BUFFERING
#define FileCachingModeRandomAccess      0x10000000 // FILE_FLAG_RANDOM_ACCESS
#define FileCachingModeSequentialScan    0x08000000 // FILE_FLAG_SEQUENTIAL_SCAN
#define FileCachingModeDeleteOnClose     0x04000000
#define FileCachingModeBackUpSemantics   0x02000000
#define FileCachingModePOSIXSemantics    0x01000000
#define FileCachingModeSessionAware      0x00800000
#define FileCachingModeOpenReparsePoint  0x00200000
#define FileCachingModeOpenNoRecall      0x00100000
#define FileCachingModeFirstPipeInstance 0x00080000
#endif
//-----------------------------------------------------------------------------

#if OSUnix
#define PrintSVN vsnprintf
#elif OSWindows
 // _O_RDONLY, _O_RDWR, _O_RDWR
#define PrintSVN vsprintf_s
#endif


#define BitStreamDebug 0

#if BitStreamDebug
#include <stdio.h>
#endif

void DataStreamConstruct(DataStream* const dataStream)
{
	MemorySet(dataStream, sizeof(DataStream), 0);
}

void DataStreamDestruct(DataStream* const dataStream)
{
	switch (dataStream->DataLocation)
	{
	case FileLocationMappedFromDisk:
		DataStreamUnmapFromMemory(dataStream);
		break;

	case FileLocationMappedVirtual:
		MemoryVirtualRelease(dataStream->Data, dataStream->DataSize);
		break;

	case  FileLocationCachedFromDisk:
		MemoryRelease(dataStream->Data, dataStream->DataSize);
		break;

	case FileLocationLinked:
		DataStreamClose(dataStream);
		break;
	}
}


DataStreamCachingMode ConvertToFileCachingMode(const unsigned int value)
{
	return FileCachingDefault;
}

unsigned int ConvertFromFileCachingMode(const DataStreamCachingMode fileCachingMode)
{
	switch (fileCachingMode)
	{
	default:
	case FileCachingDefault:
		return FileCachingModeDefault;

	case FileCachingRandom:
		return FileCachingModeRandomAccess;

	case FileCachingSequential:
		return FileCachingModeSequentialScan;

		// case BF::FileCachingMode::Temporary:
		//     break;

		// case BF::FileCachingMode::UseOnce:
		//     break;

		// case BF::FileCachingMode::NoBuffering:
		//     return FileCachingModeNoBuffering;

		// case BF::FileCachingMode::NeedLater:
		//     return FileCachingModeDontNeed;

		// case BF::FileCachingMode::DontNeedNow:
		//     return FileCachingModeNoReuse;
	}
}


void DataStreamFromExternal(DataStream* const dataStream, void* const data, const size_t dataSize)
{
	DataStreamConstruct(dataStream);

	dataStream->Data = data;
	dataStream->DataSize = dataSize;
	dataStream->MemoryMode = MemoryReadAndWrite;
	dataStream->DataLocation = FileLocationExternal;
}

ActionResult DataStreamOpenFromPathA(DataStream* const dataStream, const char* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode)
{
#if OSUnix
	const char* readMode = 0u;

	switch (fileOpenMode)
	{
	case MemoryReadOnly:
		readMode = "rb";
		break;

	case MemoryWriteOnly:
		readMode = "wb";
		break;
	}

	assert(readMode != 0);

	// Use this somewhere here
	// int posix_fadvise(int fd, off_t offset, off_t len, int advice);
	// int posix_fadvise64(int fd, off_t offset, off_t len, int advice);

	dataStream->FileHandle = fopen(filePath, readMode);

	return dataStream->FileHandle ? ActionSuccessful : ResultFileOpenFailure;

#elif OSWindows
	wchar_t filePathW[PathMaxSize];

	TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

	return DataStreamOpenFromPathW(dataStream, filePathW, fileOpenMode, dataStreamCachingMode);
#endif
}

ActionResult DataStreamOpenFromPathW(DataStream* const dataStream, const wchar_t* filePath, const MemoryProtectionMode fileOpenMode, const DataStreamCachingMode dataStreamCachingMode)
{
#if OSUnix
	char filePathA[PathMaxSize];

	TextCopyWA(filePath, PathMaxSize, filePathA, PathMaxSize);

	const ActionResult openResult = DataStreamOpenFromPathA(dataStream, filePathA, fileOpenMode, dataStreamCachingMode);
	const unsigned char successful = openResult == ActionSuccessful;

	if (!successful)
	{
		return openResult;
	}

	return ActionSuccessful;
#elif OSWindows
	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	//SECURITY_ATTRIBUTES securityAttributes = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile = 0;

	switch (fileOpenMode)
	{
	case MemoryReadOnly:
	{
		dwShareMode = FILE_SHARE_READ;
		dwCreationDisposition = OPEN_EXISTING;
		dwDesiredAccess = GENERIC_READ;
		break;
	}
	case MemoryWriteOnly:
	{
		dwShareMode = FILE_SHARE_WRITE;
		dwCreationDisposition = CREATE_ALWAYS;
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	}
	case MemoryReadAndWrite:
	{
		dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
		dwCreationDisposition = CREATE_ALWAYS;
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	}
	}

	dwFlagsAndAttributes |= ConvertFromFileCachingMode(dataStreamCachingMode);


	// Make directory if needed
	// FilePathExtensionGetW
	{
		const ActionResult directoryCreateResult = DirectoryCreateW(filePath);
		const PXBool successful = ActionSuccessful == directoryCreateResult;

		if (!successful)
		{
			return directoryCreateResult;
		}
	}

	HANDLE fileHandle = CreateFileW
	(
		filePath,
		dwDesiredAccess,
		dwShareMode,
		0,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile
	);

	{
		const PXBool successful = fileHandle != INVALID_HANDLE_VALUE;

		if (!successful)
		{
			switch (fileOpenMode)
			{
			case MemoryReadOnly:
				return ResultFileNotFound;

			case MemoryWriteOnly:
				return ResultFileCreateFailure;
			}
			/*
			const ErrorCode error = GetCurrentError();

			switch(error)
			{
				case ErrorCode::NoSuchFileOrDirectory:
					return ResultFileNotFound;

				default:
					return ResultFileOpenFailure;
			}*/
		}
	}

	// Get a FILE* from file HANDLE
	{
		int osHandleMode = 0;
		const char* fdOpenMode = 0;

		switch (fileOpenMode)
		{
		case MemoryReadOnly:
		{
			osHandleMode = _O_RDONLY;
			fdOpenMode = "rb";
			break;
		}
		case  MemoryWriteOnly:
		{
			osHandleMode = _O_RDWR;//_O_WRONLY;
			fdOpenMode = "wb";
			break;
		}
		case  MemoryReadAndWrite:
		{
			osHandleMode = _O_RDWR;
			fdOpenMode = "wb";
			break;
		}
		}

		const int nHandle = _open_osfhandle((intptr_t)fileHandle, osHandleMode);
		const PXBool sucessful = nHandle != -1;

		if (sucessful)
		{
			FILE* fp = _fdopen(nHandle, fdOpenMode);
			const PXBool result = fp;

			if (result)
			{
				dataStream->FileHandleCStyle = fp;
			}
			else
			{
				// handle?
			}
		}
		else
		{
			// handle?
		}
	}

	dataStream->FileHandle = fileHandle;
	dataStream->DataLocation = FileLocationLinked;

	return ActionSuccessful;
#endif
}

ActionResult DataStreamClose(DataStream* const dataStream)
{
#if OSUnix
	const int closeResult = fclose(dataStream->FileHandle);

	switch (closeResult)
	{
	case 0:
		return ActionSuccessful;

	default:
		return ResultFileCloseFailure;
	}
#elif OSWindows
	if (dataStream->FileHandleCStyle)
	{
		_fclose_nolock(dataStream->FileHandleCStyle);

		dataStream->FileHandleCStyle = 0;
		dataStream->FileHandle = 0;
	}

	if (dataStream->FileHandle)
	{
		const PXBool successful = CloseHandle(dataStream->FileHandle);

		dataStream->FileHandle = 0;

		return successful ? ActionSuccessful : ResultFileCloseFailure;
	}

	return ActionSuccessful;
#endif
}

ActionResult DataStreamMapToMemoryA(DataStream* const dataStream, const char* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode)
{
	DataStreamConstruct(dataStream);

#if OSUnix
	int accessType = PROT_READ;
	int flags = MAP_PRIVATE;// | MAP_POPULATE;
	int fileDescriptor = 0;
	off_t length = 0;

	// Open file
	{
		int openFlag = 0;

		switch (protectionMode)
		{
		case MemoryNoReadWrite:
			openFlag = 0;
			break;

		case MemoryReadOnly:
			openFlag = O_RDONLY;
			break;

		case MemoryWriteOnly:
			openFlag = O_WRONLY;
			break;

		case MemoryReadAndWrite:
			openFlag = O_RDWR;
			break;
		}

		const int fileDescriptor = open64(filePath, openFlag);
		const unsigned char sucessfulOpen = fileDescriptor != -1;

		if (!sucessfulOpen)
		{
			const ActionResult actionResult = GetCurrentError(); // ResultFileOpenFailure

			return actionResult;
		}

		dataStream->IDMapping = fileDescriptor;
	}

	// Get file length
	{
		const size_t fileLength = lseek64(dataStream->IDMapping, 0, SEEK_END);
		const unsigned char sucessful = fileLength > 0;

		if (!sucessful)
		{
			return ResultFileReadFailure;
		}

		dataStream->DataSize = fileLength;
	}

	// Map data
	{
		const MemoryProtectionModeType protectionModeID = ConvertFromMemoryProtectionMode(protectionMode);
		const int flags = MAP_PRIVATE;// | MAP_POPULATE;
		const off_t offset = 0;

		const void* mappedData = mmap
		(
			0, // addressPrefered
			dataStream->DataSize,
			protectionModeID,
			flags,
			dataStream->IDMapping, // fileDescriptor
			offset
		);
		const unsigned char successfulMapping = mappedData != 0;

		if (!successfulMapping)
		{
			return ResultFileMemoryMappingFailed;
		}

		dataStream->Data = mappedData;
	}

	dataStream->DataLocation = FileLocationMappedFromDisk;

	close(dataStream->IDMapping);

	dataStream->IDMapping = 0;

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) MMAP %ls\n", Data, DataSize, filePath);
#endif

	return ActionSuccessful;

#elif OSWindows
	wchar_t filePathW[PathMaxSize];

	TextCopyAW(filePath, PathMaxSize, filePathW, PathMaxSize);

	const ActionResult fileActionResult = DataStreamMapToMemoryW(dataStream, filePathW, fileSize, protectionMode);

	return fileActionResult;
#endif
}


#if OSWindows
ActionResult WindowsProcessPrivilege(const wchar_t* pszPrivilege, BOOL bEnable)
{
	HANDLE           hToken;
	TOKEN_PRIVILEGES tp;
	BOOL             status;
	DWORD            error;

	// open process token
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return GetCurrentError(); // OpenProcessToken

	// get the luid
	if (!LookupPrivilegeValue(NULL, pszPrivilege, &tp.Privileges[0].Luid))
		return GetCurrentError();  // LookupPrivilegeValue

	tp.PrivilegeCount = 1;

	// enable or disable privilege
	if (bEnable)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// enable or disable privilege
	status = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	// It is possible for AdjustTokenPrivileges to return TRUE and still not succeed.
	// So always check for the last error value.
	error = GetLastError();
	if (!status || (error != ERROR_SUCCESS))
		return GetCurrentError();  // AdjustTokenPrivileges

	// close the handle
	if (!CloseHandle(hToken))
		return GetCurrentError(); // CloseHandle

	return ActionSuccessful;
}
#endif // OSWindows

ActionResult DataStreamMapToMemoryW(DataStream* const dataStream, const wchar_t* filePath, const size_t fileSize, const MemoryProtectionMode protectionMode)
{
	PXBool useLargeMemoryPages = PXNo;

	DataStreamConstruct(dataStream);

#if OSUnix
	char filePathA[PathMaxSize];

	TextCopyWA(filePath, PathMaxSize, filePathA, PathMaxSize);

	return DataStreamMapToMemoryA(dataStream, filePathA, fileSize, protectionMode);

#elif OSWindows

	// Open file
	{
		const ActionResult openResult = DataStreamOpenFromPathW(dataStream, filePath, protectionMode, FileCachingSequential);
		const PXBool openSuccess = ActionSuccessful == openResult;

		if (!openSuccess)
		{
			return openResult; //openResult;
		}
	}

	dataStream->MemoryMode = protectionMode;
	dataStream->DataSize = fileSize;


	// Create mapping
	{
		SECURITY_ATTRIBUTES	fileMappingAttributes;
		DWORD				flProtect = 0;
		DWORD				dwMaximumSizeHigh = 0;
		DWORD				dwMaximumSizeLow = 0; // Problem if file is 0 Length
		wchar_t* name = filePath;

		switch (protectionMode)
		{
		case MemoryReadOnly:
		{
			LARGE_INTEGER largeInt;

			const BOOL sizeResult = GetFileSizeEx(dataStream->FileHandle, &largeInt);

			dwMaximumSizeHigh = 0;
			dwMaximumSizeLow = 0;

			dataStream->DataSize = largeInt.QuadPart;
			break;
		}
		case MemoryWriteOnly:
		{
			dwMaximumSizeHigh = 0;
			dwMaximumSizeLow = fileSize;
			break;
		}
		case MemoryReadAndWrite:
		{
			break;
		}

		default:
			break;
		}


#if MemoryPageLargeEnable
		// Check if large pages can be used
		{
			const size_t largePageMinimumSize = GetLargePageMinimum(); // [Kernel32.dll] OS minimum: Windows Vista
			const PXBool hasLargePageSupport = largePageMinimumSize != 0u;

			useLargeMemoryPages = hasLargePageSupport && (dataStream->DataSize > (largePageMinimumSize * 0.5f)); // if the allocation is atleah half of a big page, use that.

			if (useLargeMemoryPages)
			{
				dwMaximumSizeLow = largePageMinimumSize * ((dataStream->DataSize / largePageMinimumSize) + 1);

				ActionResult actionResult = WindowsProcessPrivilege(L"SeLockMemoryPrivilege", TRUE);

				flProtect |= SEC_COMMIT | SEC_LARGE_PAGES;
			}
		}
#endif

		switch (protectionMode)
		{
			case MemoryNoReadWrite:
				flProtect |= PAGE_NOACCESS;
				break;

			case MemoryReadOnly:
				flProtect |= PAGE_READONLY;
				break;

			case MemoryWriteOnly:
				flProtect |= PAGE_READWRITE; // PAGE_WRITECOPY
				break;

			case MemoryReadAndWrite:
				flProtect |= PAGE_READWRITE;
				break;
		}

		const HANDLE fileMappingHandleResult = CreateFileMappingW
		(
			dataStream->FileHandle,
			0, // No security attributes
			flProtect,
			dwMaximumSizeHigh,
			dwMaximumSizeLow,
			0 // No Name
		);

		// check if successful
		{
			const PXBool successful = fileMappingHandleResult;

			if (!successful)
			{
				const ActionResult mappingError = GetCurrentError(); // File memory-mapping failed

				return mappingError;
			}

			dataStream->IDMapping = fileMappingHandleResult; // Mapping [OK]
		}
	}

	{
		DWORD desiredAccess = 0;
		DWORD fileOffsetHigh = 0;
		DWORD fileOffsetLow = 0;
		size_t numberOfBytesToMap = 0;
		void* baseAddressTarget = 0;
		//DWORD  numaNodePreferred = -1; // (NUMA_NO_PREFERRED_NODE)

		switch (protectionMode)
		{
		case MemoryReadOnly:
			desiredAccess |= FILE_MAP_READ;
			break;

		case MemoryWriteOnly:
			desiredAccess |= FILE_MAP_WRITE;
			break;

		case MemoryReadAndWrite:
			desiredAccess |= FILE_MAP_ALL_ACCESS;
			break;
		}

		// if large pages are supported, anable if
		if (useLargeMemoryPages)
		{
			desiredAccess |= FILE_MAP_LARGE_PAGES;
		}

		void* const fileMapped = MapViewOfFile // MapViewOfFileExNuma is only useable starting windows vista
		(
			dataStream->IDMapping,
			desiredAccess,
			fileOffsetHigh,
			fileOffsetLow,
			numberOfBytesToMap
		);

		dataStream->Data = fileMapped;

		MemoryVirtualPrefetch(fileMapped, dataStream->DataSize);
	}

#endif

	dataStream->DataLocation = FileLocationMappedFromDisk;

#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) MMAP %ls\n", dataStream->Data, dataStream->DataSize, filePath);
#endif

	return ActionSuccessful;
}

ActionResult DataStreamMapToMemory(DataStream* const dataStream, const size_t size, const MemoryProtectionMode protectionMode)
{
	const void* data = MemoryVirtualAllocate(size, protectionMode);
	const unsigned char successful = data != 0;

	if (!successful)
	{
		return ActionSystemOutOfMemory;
	}

	DataStreamConstruct(dataStream);

	dataStream->MemoryMode = protectionMode;
	dataStream->DataLocation = FileLocationMappedVirtual;
	dataStream->Data = data;
	dataStream->DataSize = size;

	return ActionSuccessful;
}

ActionResult DataStreamUnmapFromMemory(DataStream* const dataStream)
{
	// Write pending data
	unsigned char isWriteMapped = 0;

	switch (dataStream->MemoryMode)
	{
	default:
	case MemoryInvalid:
	case MemoryNoReadWrite:
	case MemoryReadOnly:
		isWriteMapped = 0;
		break;

	case MemoryWriteOnly:
	case MemoryReadAndWrite:
		isWriteMapped = 1;
		break;
	}


#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) MMAP-Release\n", dataStream->Data, dataStream->DataSize);
#endif

#if OSUnix
	const int result = munmap(dataStream->Data, dataStream->DataSize);
	const unsigned char sucessful = result != -1;

	if (!sucessful)
	{
		const ActionResult errorCode = GetCurrentError(); // Not quite well

		return ResultFileMemoryMappingFailed;
	}

	dataStream->Data = 0;
	dataStream->DataSize = 0;

	return ActionSuccessful;

#elif OSWindows

	// Write pending data
	{
		if (isWriteMapped)
		{
			const BOOL flushSuccessful = FlushViewOfFile(dataStream->Data, dataStream->DataCursor);

			printf("");
		}
	}

	{
		const unsigned char unmappingSucessful = UnmapViewOfFile(dataStream->Data);

		if (!unmappingSucessful)
		{
			//const ErrorCode error = GetCurrentError();

			return ActionInvalid; // TODO: fix this
		}

		dataStream->Data = 0;
	}

	{
		const unsigned char closeMappingSucessful = CloseHandle(dataStream->IDMapping);

		if (!closeMappingSucessful)
		{
			return ActionInvalid; // TODO: fix this
		}

		dataStream->IDMapping = 0;
	}

	// Close
	{

		if (isWriteMapped)
		{
			//fseek();

			LARGE_INTEGER largeInteger;

			largeInteger.QuadPart = dataStream->DataCursor;

			const BOOL setSuccessful = SetFilePointerEx(dataStream->FileHandle, largeInteger, 0, FILE_BEGIN);

			const BOOL endSuccessful = SetEndOfFile(dataStream->FileHandle);
		}

		const ActionResult closeFile = DataStreamClose(dataStream);
		const unsigned char sucessful = ActionSuccessful == closeFile;

		if (!sucessful)
		{

		}

		dataStream->FileHandle = 0;
	}

	return ActionSuccessful;
#endif
}

size_t DataStreamRemainingSize(DataStream* const dataStream)
{
	return dataStream->DataSize - dataStream->DataCursor;
}

unsigned char DataStreamIsAtEnd(DataStream* const dataStream)
{
	return dataStream->DataCursor >= dataStream->DataSize;
}

void* DataStreamCursorPosition(DataStream* const dataStream)
{
	return  (unsigned char*)dataStream->Data + dataStream->DataCursor;
}

void DataStreamCursorToBeginning(DataStream* const dataStream)
{
	dataStream->DataCursor = 0;
}

void DataStreamCursorAdvance(DataStream* const dataStream, const size_t steps)
{
	dataStream->DataCursor += steps; // Check overflow
}

void DataStreamCursorRewind(DataStream* const dataStream, const size_t steps)
{
	dataStream->DataCursor -= steps; // Check underflow
}

void DataStreamCursorToEnd(DataStream* const dataStream)
{
	dataStream->DataCursor = dataStream->DataSize;
}

size_t DataStreamReadNextLineInto(DataStream* const dataStream, void* exportBuffer, const size_t exportBufferSize)
{
	const size_t dataPositionBefore = dataStream->DataCursor;

	DataStreamSkipEndOfLineCharacters(dataStream);

	while (!DataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = DataStreamCursorPosition(dataStream);
		const unsigned char advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		DataStreamCursorAdvance(dataStream, 1u);
	}

	const char* dataPoint = (char*)dataStream->Data + dataPositionBefore;
	const size_t dataPositionAfter = dataStream->DataCursor;
	const size_t length = dataPositionAfter - dataPositionBefore;

	if (length == 0)
	{
		return 0;
	}

	TextCopyA(dataPoint, length, exportBuffer, length);

	DataStreamSkipEndOfLineCharacters(dataStream);

	return length;
}

size_t DataStreamSkipEndOfLineCharacters(DataStream* const dataStream)
{
	const size_t dataPositionBefore = dataStream->DataCursor;

	while (!DataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = DataStreamCursorPosition(dataStream);
		const unsigned char advance = IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		DataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - dataPositionBefore;
}

size_t DataStreamSkipEmptySpace(DataStream* const dataStream)
{
	const size_t oldPosition = dataStream->DataCursor;

	while (!DataStreamIsAtEnd(dataStream))
	{
		const char* data = DataStreamCursorPosition(dataStream);
		const PXBool advance = IsEmptySpace(*data) || IsTab(*data);

		if (!advance)
		{
			break;
		}

		DataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - oldPosition;
}

size_t DataStreamSkipBlock(DataStream* const dataStream)
{
	const size_t oldPosition = dataStream->DataCursor;

	while (!DataStreamIsAtEnd(dataStream))
	{
		const char* data = DataStreamCursorPosition(dataStream);
		const char character = *data;
		const PXBool advance = !IsEndOfString(character) and !IsEmptySpace(character) and !IsEndOfLineCharacter(character);

		if (!advance)
		{
			break;
		}

		DataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - oldPosition;
}

size_t DataStreamSkipToNextBlock(DataStream* const dataStream)
{
	const size_t oldPosition = dataStream->DataCursor;

	DataStreamSkipBlock(dataStream);

	DataStreamSkipEmptySpace(dataStream);

	return dataStream->DataCursor - oldPosition;
}

size_t DataStreamSkipLine(DataStream* const dataStream)
{
	const size_t positionBefore = dataStream->DataCursor;

	while (!DataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = DataStreamCursorPosition(dataStream);
		const unsigned char advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		DataStreamCursorAdvance(dataStream, 1u);
	}

	DataStreamSkipEndOfLineCharacters(dataStream);

	const size_t skippedBytes = dataStream->DataCursor - positionBefore;

	return skippedBytes;
}

size_t DataStreamReadC(DataStream* const dataStream, char* const value)
{
	return DataStreamReadP(dataStream, value, sizeof(char));
}

size_t DataStreamReadCU(DataStream* const dataStream, unsigned char* const value)
{
	return DataStreamReadP(dataStream, value, sizeof(unsigned char));
}

size_t DataStreamReadS(DataStream* const dataStream, short* const value, const Endian endian)
{
	return DataStreamReadSU(dataStream, (unsigned short*)value, endian);
}

size_t DataStreamReadSU(DataStream* const dataStream, unsigned short* const value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned short);
	const unsigned char* data = DataStreamCursorPosition(dataStream);
	const unsigned short* dataValue = (unsigned short*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	DataStreamCursorAdvance(dataStream, dataSize);

	return dataSize;
}

size_t DataStreamReadI(DataStream* const dataStream, int* const value, const Endian endian)
{
	return DataStreamReadIU(dataStream, (unsigned int*)value, endian);
}

size_t DataStreamReadIU(DataStream* const dataStream, unsigned int* const value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned int);
	const unsigned char* data = DataStreamCursorPosition(dataStream);
	const unsigned int* dataValue = (unsigned int*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	DataStreamCursorAdvance(dataStream, dataSize);

	return dataSize;
}

size_t DataStreamReadLL(DataStream* const dataStream, long long* const value, const Endian endian)
{
	return DataStreamReadLLU(dataStream, (unsigned long long*)value, endian);
}

size_t DataStreamReadLLU(DataStream* const dataStream, unsigned long long* const value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned long long);
	const unsigned char* data = DataStreamCursorPosition(dataStream);
	const unsigned long long* dataValue = (unsigned long long*)data;

	*value = *dataValue;

	EndianSwap(value, dataSize, endian, EndianCurrentSystem);

	DataStreamCursorAdvance(dataStream, dataSize);

	return dataSize;
}

size_t DataStreamReadF(DataStream* const dataStream, float* const value)
{
	const size_t dataSize = sizeof(float);
	const size_t writtenBytes = DataStreamReadP(dataStream, value, dataSize);

	return writtenBytes;
}

size_t DataStreamReadD(DataStream* const dataStream, double* const value)
{
	const size_t dataSize = sizeof(double);
	const size_t writtenBytes = DataStreamReadP(dataStream, value, dataSize);

	return writtenBytes;
}

size_t DataStreamReadP(DataStream* const dataStream, void* value, const size_t length)
{
	const unsigned char* currentPosition = DataStreamCursorPosition(dataStream);
	const size_t readableSize = DataStreamRemainingSize(dataStream);
	const size_t copyedBytes = MemoryCopy(currentPosition, readableSize, value, length);

	DataStreamCursorAdvance(dataStream, copyedBytes);

	return copyedBytes;
}

size_t DataStreamRead(DataStream* const dataStream, const void* format, const size_t length, ...)
{
	return 0;
}

size_t DataStreamReadA(DataStream* const dataStream, char* const value, const size_t length)
{
	const size_t size = DataStreamReadP(dataStream, value, length);

	value[size] = 0;

	return size;
}

size_t DataStreamReadW(DataStream* const dataStream, wchar_t* const value, const size_t length)
{
	const size_t size = DataStreamReadP(dataStream, value, length);

	value[size] = 0;

	return size;
}

void DataStreamReadUntil(DataStream* const dataStream, void* value, const size_t length, const char character)
{
	const unsigned char* currentPosition = DataStreamCursorPosition(dataStream);

	size_t lengthCopy = 0;

	while (!DataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = DataStreamCursorPosition(dataStream);
		const unsigned char advance = *data != character && length <= lengthCopy;

		if (!advance)
		{
			break;
		}

		++lengthCopy;

		DataStreamCursorAdvance(dataStream, 1u);
	}

	const size_t readableSize = DataStreamRemainingSize(dataStream);

	MemoryCopy(currentPosition, readableSize, value, lengthCopy);
}

unsigned char DataStreamReadAndCompare(DataStream* const dataStream, const void* value, const size_t length)
{
	const unsigned char* currentPosition = DataStreamCursorPosition(dataStream);
	const size_t readableSize = DataStreamRemainingSize(dataStream);

	const unsigned char result = MemoryCompare(currentPosition, readableSize, value, length);

	if (result)
	{
		DataStreamCursorAdvance(dataStream, length);
	}

	return result;
}

size_t DataStreamWriteC(DataStream* const dataStream, const char value)
{
	return DataStreamWriteCU(dataStream, value);
}

size_t DataStreamWriteCU(DataStream* const dataStream, const unsigned char value)
{
	const size_t dataSize = sizeof(unsigned char);

	DataStreamWriteP(dataStream, &value, dataSize);

	return dataSize;
}

size_t DataStreamWriteS(DataStream* const dataStream, const short value, const Endian endian)
{
	return DataStreamWriteSU(dataStream, value, endian);
}

size_t DataStreamWriteSU(DataStream* const dataStream, const unsigned short value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned short);
	unsigned short dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	DataStreamWriteP(dataStream, &dataValue, dataSize);

	return dataSize;
}

size_t DataStreamWriteI(DataStream* const dataStream, const int value, const Endian endian)
{
	return DataStreamWriteIU(dataStream, value, endian);
}

size_t DataStreamWriteIU(DataStream* const dataStream, const unsigned int value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned int);
	unsigned int dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	DataStreamWriteP(dataStream, &dataValue, dataSize);

	return dataSize;
}

size_t DataStreamWriteLL(DataStream* const dataStream, const long long value, const Endian endian)
{
	return DataStreamWriteLLU(dataStream, value, endian);
}

size_t DataStreamWriteLLU(DataStream* const dataStream, const unsigned long long value, const Endian endian)
{
	const size_t dataSize = sizeof(unsigned long long);
	unsigned long long dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	DataStreamWriteP(dataStream, &dataValue, dataSize);

	return dataSize;
}

size_t DataStreamWriteF(DataStream* const dataStream, const float value)
{
	return DataStreamWriteP(dataStream, &value, sizeof(float));
}

size_t DataStreamWriteD(DataStream* const dataStream, const double value)
{
	return DataStreamWriteP(dataStream, &value, sizeof(double));
}

size_t DataStreamWriteA(DataStream* const dataStream, const char* const text, size_t textSize)
{
#if 1
	return DataStreamWriteP(dataStream, text, textSize);
#else
	const size_t writableSize = DataStreamRemainingSize(dataStream);
	char* const currentPosition = (char* const)DataStreamCursorPosition(dataStream);

	const size_t writtenBytes = TextCopyA(text, textSize, currentPosition, writableSize);

	DataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
#endif
}

size_t DataStreamWriteW(DataStream* const dataStream, const wchar_t* const text, size_t textSize)
{
	const size_t writableSize = DataStreamRemainingSize(dataStream);
	wchar_t* const currentPosition = (wchar_t* const)DataStreamCursorPosition(dataStream);

	const size_t writtenBytes = TextCopyW(text, textSize, currentPosition, writableSize);

	DataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
}

size_t DataStreamWriteP(DataStream* const dataStream, const void* value, const size_t length)
{
	const size_t writableSize = DataStreamRemainingSize(dataStream);
	unsigned char* currentPosition = DataStreamCursorPosition(dataStream);

	const size_t copyedBytes = MemoryCopy(value, length, currentPosition, writableSize);

	DataStreamCursorAdvance(dataStream, copyedBytes);

	return copyedBytes;
}

size_t DataStreamWriteFill(DataStream* const dataStream, const unsigned char value, const size_t length)
{
	const size_t writableSize = DataStreamRemainingSize(dataStream);
	unsigned char* beforePosition = DataStreamCursorPosition(dataStream);
	const size_t write = MathMinimumIU(writableSize, length);

	for (size_t i = 0; i < write; ++i)
	{
		beforePosition[i] = value;
	}

	DataStreamCursorAdvance(dataStream, write);

	return write;
}

size_t DataStreamWrite(DataStream* const dataStream, const char* format, ...)
{
	const unsigned char* currentPosition = DataStreamCursorPosition(dataStream);

	va_list args;
	va_start(args, format);

	const size_t writableSize = DataStreamRemainingSize(dataStream);
	const int writtenBytes = PrintSVN(currentPosition, writableSize, format, args);

	va_end(args);

	{
		const unsigned char sucessful = writtenBytes >= 0;

		if (!sucessful)
		{
			return 0;
		}
	}

	DataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
}

size_t DataStreamWriteAtP(DataStream* const dataStream, const void* const data, const size_t dataSize, const size_t index)
{
	const size_t positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const size_t writtenBytes = DataStreamWriteP(dataStream, data, dataSize); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

size_t DataStreamWriteAtCU(DataStream* const dataStream, const unsigned char value, const size_t index)
{
	return DataStreamWriteAtP(dataStream, &value, sizeof(unsigned char), index);
}

size_t DataStreamWriteAtSU(DataStream* const dataStream, const unsigned short value, const Endian endian, const size_t index)
{
	const size_t positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	DataStreamWriteSU(dataStream, value, endian); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return 2u;
}

size_t DataStreamWriteAtIU(DataStream* const dataStream, const unsigned int value, const Endian endian, const size_t index)
{
	const size_t positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	DataStreamWriteIU(dataStream, value, endian); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return 4u;
}

size_t DataStreamSkipBitsToNextByte(DataStream* const dataStream)
{
	const unsigned char hasAnyBitConsumed = dataStream->DataCursorBitOffset > 0;

	if (hasAnyBitConsumed)
	{
		dataStream->DataCursorBitOffset = 0; // Reset
		++dataStream->DataCursor; // Go 1 Byte further
	}

	return 0;
}

size_t DataStreamCursorMoveBits(DataStream* const dataStream, const size_t amountOfBits)
{
	dataStream->DataCursorBitOffset += amountOfBits;

	DataStreamBitsAllign(dataStream);

	return 0;
}

size_t DataStreamBitsAllign(DataStream* const dataStream)
{
	while (dataStream->DataCursorBitOffset >= 8u) // Move a Byte__ at the time forward, 8 Bits = 1 Byte__.
	{
		dataStream->DataCursor++;
		dataStream->DataCursorBitOffset -= 8u;
	}
}

size_t DataStreamPeekBits(DataStream* const dataStream, const size_t amountOfBits)
{
	unsigned int bitMask = ((1u << amountOfBits) - 1u) << dataStream->DataCursorBitOffset; // 0000111111
	unsigned int bitBlock;
	unsigned char* a =  DataStreamCursorPosition(dataStream);
	unsigned char* b = a + 1;
	unsigned char* c = a + 2;
	unsigned char* d = a + 3;
	unsigned char* maxAdress = (unsigned char*)dataStream->Data + (dataStream->DataSize - 1);

	unsigned int ai = a > maxAdress ? 0 : *a;
	unsigned int bi = b > maxAdress ? 0 : *b;
	unsigned int ci = c > maxAdress ? 0 : *c;
	unsigned int di = d > maxAdress ? 0 : *d;

	// [d][c][b][a] Little Endian

	bitBlock = ai | bi << 8 | ci << 16 | di << 24;

	unsigned int result = bitBlock & bitMask;

#if BitStreamDebug
	printf("Extract %i Bits. Byte__:%i Offset:%i\n", amountOfBits, dataStream->DataCursor, dataStream->BitOffset);
	printf("BitBlock : ");
	PrintBinary(bitBlock);
	printf("BitMask  : ");
	PrintBinary(bitMask);
	printf("Result   : ");
	//PrintBinary(result);
#endif

	result >>= dataStream->DataCursorBitOffset; // Shoitft correction

#if BitStreamDebug
	printf("Shifted  : ");
	//PrintBinary(result);
#endif

	return result;
}

size_t DataStreamReadBits(DataStream* const dataStream, const size_t amountOfBits)
{
	const size_t result = DataStreamPeekBits(dataStream, amountOfBits);

	DataStreamCursorMoveBits(dataStream, amountOfBits);

	return result;
}

size_t DataStreamWriteBits(DataStream* const dataStream, const size_t bitData, const size_t amountOfBits)
{
	unsigned char* currentPos = DataStreamCursorPosition(dataStream);

	unsigned char* a = currentPos;
	unsigned char* b = currentPos + 1;
	unsigned char* c = currentPos + 2;
	unsigned char* d = currentPos + 3;

	size_t moveCounter = 0;

	for (size_t i = 1; i <= amountOfBits; i++)
	{
		unsigned char bit = bitData << i;

		*currentPos += bit;
		*currentPos << 1;

		dataStream->DataCursorBitOffset++;
		moveCounter++;
	}

	DataStreamBitsAllign(dataStream);

	return moveCounter;
}

size_t DataStreamFilePathGetA(DataStream* const dataStream, char* const filePath, const size_t filePathMaxSize)
{
#if OSUnix
    return 0; // TODO

#elif OSWindows
	const size_t length = GetFinalPathNameByHandleA(dataStream->FileHandle, filePath, filePathMaxSize, VOLUME_NAME_DOS); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u == length;

	if (!sucessful)
	{
		const ActionResult result = GetCurrentError();

		return result;
	}

	return ActionSuccessful;
#endif
}

size_t DataStreamFilePathGetW(DataStream* const dataStream, wchar_t* const filePath, const size_t filePathMaxSize)
{
#if OSUnix
    return 0; // TODO

#elif OSWindows
	const DWORD flags = VOLUME_NAME_DOS | FILE_NAME_NORMALIZED;
	const DWORD length = GetFinalPathNameByHandleW(dataStream->FileHandle, filePath, filePathMaxSize, flags); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u != length;

	if (!sucessful)
	{
		const ActionResult result = GetCurrentError();

		return result;
	}

	wchar_t buffer[PathMaxSize];

	if (filePath[0] == '\\')
	{
		TextCopyW(filePath+4, length-4, buffer, PathMaxSize);
		TextCopyW(buffer, PathMaxSize, filePath, length-4);
	}

	return ActionSuccessful;
#endif
}
