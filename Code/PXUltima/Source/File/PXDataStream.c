#include "PXDataStream.h"

#include <OS/System/OSVersion.h>
#include <OS/Memory/PXMemory.h>
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

void PXDataStreamConstruct(PXDataStream* const dataStream)
{
	MemoryClear(dataStream, sizeof(PXDataStream));
}

void PXDataStreamDestruct(PXDataStream* const dataStream)
{
	switch (dataStream->DataLocation)
	{
	case FileLocationMappedFromDisk:
		PXDataStreamUnmapFromMemory(dataStream);
		break;

	case FileLocationMappedVirtual:
		MemoryVirtualRelease(dataStream->Data, dataStream->DataSize);
		break;

	case  FileLocationCachedFromDisk:
		MemoryRelease(dataStream->Data, dataStream->DataSize);
		break;

	case FileLocationLinked:
		PXDataStreamClose(dataStream);
		break;
	}
}


PXDataStreamCachingMode ConvertToFileCachingMode(const unsigned int value)
{
	return FileCachingDefault;
}

unsigned int ConvertFromFileCachingMode(const PXDataStreamCachingMode fileCachingMode)
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


void PXDataStreamFromExternal(PXDataStream* const dataStream, void* const data, const PXSize dataSize)
{
	PXDataStreamConstruct(dataStream);

	dataStream->Data = data;
	dataStream->DataSize = dataSize;
	dataStream->MemoryMode = MemoryReadAndWrite;
	dataStream->DataLocation = FileLocationExternal;
}

PXActionResult PXDataStreamOpenFromPathA(PXDataStream* const dataStream, const PXTextASCII filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXByte filePathU[PathMaxSize];

	TextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

	PXActionResult actionResult = PXDataStreamOpenFromPathU(dataStream, filePathU, fileOpenMode, dataStreamCachingMode);

	return actionResult;
}

PXActionResult PXDataStreamOpenFromPathW(PXDataStream* const dataStream, const PXTextUNICODE filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
{
	PXCharUTF8 filePathU[PathMaxSize];

	TextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

	PXActionResult actionResult = PXDataStreamOpenFromPathU(dataStream, filePathU, fileOpenMode, dataStreamCachingMode);

	return actionResult;
}

PXActionResult PXDataStreamOpenFromPathU(PXDataStream* const dataStream, const PXTextUTF8 filePath, const MemoryProtectionMode fileOpenMode, const PXDataStreamCachingMode dataStreamCachingMode)
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


	(readMode != 0);

	// Use this somewhere here
	// int posix_fadvise(int fd, off_t offset, off_t len, int advice);
	// int posix_fadvise64(int fd, off_t offset, off_t len, int advice);

	dataStream->FileHandle = fopen(filePath, readMode);

	return dataStream->FileHandle ? PXActionSuccessful : PXActionFailedFileOpen;


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
	if(fileOpenMode == MemoryWriteOnly || fileOpenMode == MemoryReadAndWrite)
	{
		//const PXActionResult directoryCreateResult = DirectoryCreateA(filePath);

		//PXActionExitOnError(directoryCreateResult);
	}

	// UTF
	{
		HANDLE const fileHandle = CreateFileA
		(
			filePath,
			dwDesiredAccess,
			dwShareMode,
			0,
			dwCreationDisposition,
			dwFlagsAndAttributes,
			hTemplateFile
		);
		const PXBool successful = fileHandle != INVALID_HANDLE_VALUE;

		PXActionOnErrorFetchAndExit(!successful);

		dataStream->FileHandle = fileHandle;
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

		const int nHandle = _open_osfhandle((intptr_t)dataStream->FileHandle, osHandleMode);
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

	dataStream->DataLocation = FileLocationLinked;

	return PXActionSuccessful;
#endif
}

PXActionResult PXDataStreamClose(PXDataStream* const dataStream)
{
#if OSUnix
	const int closeResult = fclose(dataStream->FileHandle);

	switch (closeResult)
	{
        case 0:
            return PXActionSuccessful;

        default:
            return PXActionFailedFileClose;
	}
#elif OSWindows
	if (dataStream->FileHandleCStyle)
	{
		_fclose_nolock(dataStream->FileHandleCStyle);

		dataStream->FileHandleCStyle = PXNull;
		dataStream->FileHandle = PXNull;
	}

	if (dataStream->FileHandle)
	{
		const PXBool successful = CloseHandle(dataStream->FileHandle);

		if (!successful)
		{
			return PXActionFailedFileClose;
		}

		dataStream->FileHandle = PXNull;
	}

	return PXActionSuccessful;
#endif
}

PXActionResult PXDataStreamMapToMemoryA(PXDataStream* const dataStream, const PXTextASCII filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXByte filePathU[PathMaxSize];

	TextCopyAU(filePath, PathMaxSize, filePathU, PathMaxSize);

	const PXActionResult actionResult = PXDataStreamMapToMemoryU(dataStream, filePathU, fileSize, protectionMode);

	return actionResult;
}

PXActionResult PXDataStreamMapToMemoryW(PXDataStream* const dataStream, const PXTextUNICODE filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXCharUTF8 filePathU[PathMaxSize];

	TextCopyWU(filePath, PathMaxSize, filePathU, PathMaxSize);

	const PXActionResult actionResult = PXDataStreamMapToMemoryU(dataStream, filePathU, fileSize, protectionMode);

	return actionResult;
}

PXActionResult PXDataStreamMapToMemoryU(PXDataStream* const dataStream, const PXTextUTF8 filePath, const PXSize fileSize, const MemoryProtectionMode protectionMode)
{
	PXBool useLargeMemoryPages = PXNo;

	PXDataStreamConstruct(dataStream);

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
		const PXBool sucessfulOpen = fileDescriptor != -1;

		PXActionOnErrorFetchAndExit(!sucessfulOpen)

		dataStream->IDMapping = fileDescriptor;
	}

	// Get file length
	{
		const PXSize fileLength = lseek64(dataStream->IDMapping, 0, SEEK_END);
		const PXBool sucessful = fileLength > 0;

		if (!sucessful)
		{
			return PXActionFailedFileRead;
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
		const PXBool successfulMapping = mappedData != 0;

		if (!successfulMapping)
		{
			return PXActionFailedFileMapping;
		}

		dataStream->Data = mappedData;
	}

	dataStream->DataLocation = FileLocationMappedFromDisk;

	close(dataStream->IDMapping);

	dataStream->IDMapping = 0;


#elif OSWindows

	// Open file
	{
		const PXActionResult openResult = PXDataStreamOpenFromPathU(dataStream, filePath, protectionMode, FileCachingSequential);

		PXActionExitOnError(openResult);
	}

	dataStream->MemoryMode = protectionMode;
	dataStream->DataSize = fileSize;
	dataStream->DataAllocated = fileSize;

	// Create mapping
	{
		SECURITY_ATTRIBUTES	fileMappingAttributes;
		DWORD flProtect = SEC_COMMIT;
		DWORD dwMaximumSizeHigh = 0;
		DWORD dwMaximumSizeLow = 0; // Problem if file is 0 Length
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
				dataStream->DataAllocated = largeInt.QuadPart;
				break;
			}
			case MemoryReadAndWrite:
			case MemoryWriteOnly:
			{
#if OS64Bit
				dwMaximumSizeHigh = (fileSize & (0xFFFFFFFF00000000)) >> 32u;
				dwMaximumSizeLow = fileSize & 0x00000000FFFFFFFF;
#elif OS32Bit
				dwMaximumSizeHigh = 0;
				dwMaximumSizeLow = fileSize & 0xFFFFFFFF;
#endif
				break;
			}
			default:
				break;
		}

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

		// [i] I want to add LargePage support but it seems you cant do that with files.

		const HANDLE fileMappingHandleResult = CreateFileMappingA
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

			PXActionOnErrorFetchAndExit(!successful); // File memory-mapping failed

			dataStream->IDMapping = fileMappingHandleResult; // Mapping [OK]
		}
	}

	{
		DWORD desiredAccess = 0;
		DWORD fileOffsetHigh = 0;
		DWORD fileOffsetLow = 0;
		PXSize numberOfBytesToMap = 0;
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

		void* const fileMapped = MapViewOfFile // MapViewOfFileExNuma is only useable starting windows vista, this function in XP
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

	return PXActionSuccessful;
}

PXActionResult PXDataStreamMapToMemory(PXDataStream* const dataStream, const PXSize size, const MemoryProtectionMode protectionMode)
{
	const void* data = MemoryVirtualAllocate(size, protectionMode);
	const PXBool successful = data != 0;

	if (!successful)
	{
		return PXActionFailedAllocation;
	}

	PXDataStreamConstruct(dataStream);

	dataStream->MemoryMode = protectionMode;
	dataStream->DataLocation = FileLocationMappedVirtual;
	dataStream->Data = data;
	dataStream->DataSize = size;

	return PXActionSuccessful;
}

PXActionResult PXDataStreamUnmapFromMemory(PXDataStream* const dataStream)
{
	// Write pending data
	PXBool isWriteMapped = 0;

	switch (dataStream->MemoryMode)
	{
		default:
		case MemoryInvalid:
		case MemoryNoReadWrite:
		case MemoryReadOnly:
			isWriteMapped = PXFalse;
			break;

		case MemoryWriteOnly:
		case MemoryReadAndWrite:
			isWriteMapped = PXTrue;
			break;
	}


#if MemoryDebugOutput
	printf("[#][Memory] 0x%p (%10zi B) MMAP-Release\n", dataStream->Data, dataStream->DataSize);
#endif

#if OSUnix
	const int result = munmap(dataStream->Data, dataStream->DataSize);
	const PXBool sucessful = result != -1;

    PXActionOnErrorFetchAndExit(!sucessful)

	dataStream->Data = PXNull;
	dataStream->DataSize = 0;

	return PXActionSuccessful;

#elif OSWindows

	// Write pending data
	{
		if (isWriteMapped)
		{
			const BOOL flushSuccessful = FlushViewOfFile(dataStream->Data, dataStream->DataCursor);

			PXActionOnErrorFetchAndExit(flushSuccessful);
		}
	}

	{
		const PXBool unmappingSucessful = UnmapViewOfFile(dataStream->Data);

		PXActionOnErrorFetchAndExit(unmappingSucessful);

		dataStream->Data = PXNull;
	}

	{
		const PXBool closeMappingSucessful = CloseHandle(dataStream->IDMapping);

		PXActionOnErrorFetchAndExit(closeMappingSucessful);

		dataStream->IDMapping = PXNull;
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

		const PXActionResult closeFile = PXDataStreamClose(dataStream);

		PXActionExitOnError(closeFile);

		dataStream->FileHandle = PXNull;
	}

	return PXActionSuccessful;
#endif
}

PXSize PXDataStreamRemainingSize(const PXDataStream* __restrict const dataStream)
{
	return dataStream->DataSize - dataStream->DataCursor;
}

PXBool PXDataStreamIsAtEnd(const PXDataStream* __restrict const dataStream)
{
	return dataStream->DataCursor >= dataStream->DataSize;
}

void* PXDataStreamCursorPosition(PXDataStream* const dataStream)
{
	return (PXAdress)dataStream->Data + dataStream->DataCursor;
}

void PXDataStreamCursorMoveTo(PXDataStream* const dataStream, const PXSize position)
{
	dataStream->DataCursor = MathMinimumIU(position, dataStream->DataSize);
}

void PXDataStreamCursorToBeginning(PXDataStream* const dataStream)
{
	dataStream->DataCursor = 0;
}

PXSize PXDataStreamCursorAdvance(PXDataStream* const dataStream, const PXSize steps)
{
	const PXSize readableSize = PXDataStreamRemainingSize(dataStream);
	const PXSize before = dataStream->DataCursor;
	const PXSize maximalMove = MathMinimumIU(readableSize, steps); // Prevent cursor from going out of bounce

	dataStream->DataCursor += maximalMove;

	return maximalMove;
}

void PXDataStreamCursorRewind(PXDataStream* const dataStream, const PXSize steps)
{
	dataStream->DataCursor -= steps; // Check underflow
}

void PXDataStreamCursorToEnd(PXDataStream* const dataStream)
{
	dataStream->DataCursor = dataStream->DataSize;
}

PXSize PXDataStreamReadNextLineInto(PXDataStream* const dataStream, void* exportBuffer, const PXSize exportBufferSize)
{
	const PXSize dataPositionBefore = dataStream->DataCursor;

	PXDataStreamSkipEndOfLineCharacters(dataStream);

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = PXDataStreamCursorPosition(dataStream);
		const PXBool advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	const char* dataPoint = (char*)dataStream->Data + dataPositionBefore;
	const PXSize dataPositionAfter = dataStream->DataCursor;
	const PXSize length = dataPositionAfter - dataPositionBefore;

	if (length == 0)
	{
		return 0;
	}

	TextCopyA(dataPoint, length, exportBuffer, length);

	PXDataStreamSkipEndOfLineCharacters(dataStream);

	return length;
}

PXSize PXDataStreamSkipEndOfLineCharacters(PXDataStream* const dataStream)
{
	const PXSize dataPositionBefore = dataStream->DataCursor;

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = PXDataStreamCursorPosition(dataStream);
		const PXBool advance = IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - dataPositionBefore;
}

PXSize PXDataStreamSkipEmptySpace(PXDataStream* const dataStream)
{
	const PXSize oldPosition = dataStream->DataCursor;

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const char* data = PXDataStreamCursorPosition(dataStream);
		const PXBool advance = IsEmptySpace(*data) || IsTab(*data);

		if (!advance)
		{
			break;
		}

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - oldPosition;
}

PXSize PXDataStreamSkipBlock(PXDataStream* const dataStream)
{
	const PXSize oldPosition = dataStream->DataCursor;

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const char* data = PXDataStreamCursorPosition(dataStream);
		const char character = *data;
		const PXBool advance = !IsEndOfString(character) and !IsEmptySpace(character) and !IsEndOfLineCharacter(character);

		if (!advance)
		{
			break;
		}

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	return dataStream->DataCursor - oldPosition;
}

PXSize PXDataStreamSkipToNextBlock(PXDataStream* const dataStream)
{
	const PXSize oldPosition = dataStream->DataCursor;

	PXDataStreamSkipBlock(dataStream);

	PXDataStreamSkipEmptySpace(dataStream);

	return dataStream->DataCursor - oldPosition;
}

PXSize PXDataStreamSkipLine(PXDataStream* const dataStream)
{
	const PXSize positionBefore = dataStream->DataCursor;

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = PXDataStreamCursorPosition(dataStream);
		const PXBool advance = !IsEndOfLineCharacter(*data) and !IsEndOfString(*data);

		if (!advance)
		{
			break;
		}

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	PXDataStreamSkipEndOfLineCharacters(dataStream);

	const PXSize skippedBytes = dataStream->DataCursor - positionBefore;

	return skippedBytes;
}

PXSize PXDataStreamDataCopy(PXDataStream* const pxInputStream, PXDataStream* const pxOutputStream, const PXSize length)
{
	void* const insertionPoint = PXDataStreamCursorPosition(pxOutputStream); // Position of output
	const PXSize copyedBytes = PXDataStreamReadB(pxInputStream, insertionPoint, length); // Copy from input to output
	const PXSize movedBytes = PXDataStreamCursorAdvance(pxOutputStream, copyedBytes); // Move output by copyed amount

	return movedBytes; // return moved amount of bytes
}

PXSize PXDataStreamReadTextIU8(PXDataStream* const dataStream, PXInt8U* const number)
{
	unsigned int value = 0;
	const PXSize size = PXDataStreamReadTextI(dataStream, &value);

	*number = value;

	return size;
}

PXSize PXDataStreamReadTextI(PXDataStream* const dataStream, int* const number)
{
	const void* const adress = PXDataStreamCursorPosition(dataStream);
	const PXSize size = PXDataStreamRemainingSize(dataStream);
	const PXSize sizeRead = TextToIntA(adress, size, number);

	PXDataStreamCursorAdvance(dataStream, sizeRead);

	return sizeRead;
}

PXSize PXDataStreamReadI8S(PXDataStream* const dataStream, PXInt8S* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt8S));
}

PXSize PXDataStreamReadI8SV(PXDataStream* const dataStream, PXInt8S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt8S) * valueListSize);
}

PXSize PXDataStreamReadI8U(PXDataStream* const dataStream, PXInt8U* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt8U));
}

PXSize PXDataStreamReadI8UV(PXDataStream* const dataStream, PXInt8U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt8U) * valueListSize);
}

PXSize PXDataStreamReadI16S(PXDataStream* const dataStream, PXInt16S* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt16S));
}

PXSize PXDataStreamReadI16SV(PXDataStream* const dataStream, PXInt16S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt16S) * valueListSize);
}

PXSize PXDataStreamReadI16SE(PXDataStream* const dataStream, PXInt16S* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt16S);
	const PXInt16S* const data = (const PXInt16S* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI16SVE(PXDataStream* const dataStream, PXInt16S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI16SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamReadI16U(PXDataStream* const dataStream, PXInt16U* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt16U));
}

PXSize PXDataStreamReadI16UV(PXDataStream* const dataStream, PXInt16U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt16U) * valueListSize);
}

PXSize PXDataStreamReadI16UE(PXDataStream* const dataStream, PXInt16U* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt16U);
	const PXInt16U* const data = (const PXInt16U* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI16UVE(PXDataStream* const dataStream, PXInt16U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI16UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamReadI32S(PXDataStream* const dataStream, PXInt32S* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt32S));
}

PXSize PXDataStreamReadI32SV(PXDataStream* const dataStream, PXInt32S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt32S) * valueListSize);
}

PXSize PXDataStreamReadI32SE(PXDataStream* const dataStream, PXInt32S* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt32S);
	const PXInt32S* const data = (const PXInt32S* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI32SVE(PXDataStream* const dataStream, PXInt32S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI32SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamReadI32U(PXDataStream* const dataStream, PXInt32U* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt32U));
}

PXSize PXDataStreamReadI32UV(PXDataStream* const dataStream, PXInt32U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt32U) * valueListSize);
}

PXSize PXDataStreamReadI32UE(PXDataStream* const dataStream, PXInt32U* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt32U);
	const PXInt32U* const data = (const PXInt32U* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI32UVE(PXDataStream* const dataStream, PXInt32U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI32UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamReadI64S(PXDataStream* const dataStream, PXInt64S* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt64S));
}

PXSize PXDataStreamReadI64SV(PXDataStream* const dataStream, PXInt64S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt64S) * valueListSize);
}

PXSize PXDataStreamReadI64SE(PXDataStream* const dataStream, PXInt64S* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt64S);
	const PXInt64S* const data = (const PXInt64S* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI64VE(PXDataStream* const dataStream, PXInt64S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI64SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamReadI64U(PXDataStream* const dataStream, PXInt64U* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(PXInt64U));
}

PXSize PXDataStreamReadI64UV(PXDataStream* const dataStream, PXInt64U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(PXInt64U) * valueListSize);
}

PXSize PXDataStreamReadI64UE(PXDataStream* const dataStream, PXInt64U* const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt64U);
	const PXInt64U* const data = (const PXInt64U* const)PXDataStreamCursorPosition(dataStream);
	const PXSize movedSteps = PXDataStreamCursorAdvance(dataStream, dataSize);

	*value = *data;

	EndianSwap(value, movedSteps, endian, EndianCurrentSystem);

	return movedSteps;
}

PXSize PXDataStreamReadI64UVE(PXDataStream* const dataStream, PXInt64U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamReadI64UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}


PXSize PXDataStreamReadF(PXDataStream* const dataStream, float* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(float));
}

PXSize PXDataStreamReadFV(PXDataStream* const dataStream, float* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(float) * valueListSize);
}

PXSize PXDataStreamReadD(PXDataStream* const dataStream, double* const value)
{
	return PXDataStreamReadB(dataStream, value, sizeof(double));
}

PXSize PXDataStreamReadDV(PXDataStream* const dataStream, double* const valueList, const PXSize valueListSize)
{
	return PXDataStreamReadB(dataStream, valueList, sizeof(double) * valueListSize);
}

PXSize PXDataStreamReadMultible(PXDataStream* const dataStream, PXDataStreamElementType* const pxDataStreamElementList, const PXSize pxDataStreamElementListSize)
{
	PXSize totalReadBytes = 0;

	for (size_t i = 0; i < pxDataStreamElementListSize; ++i)
	{
		PXDataStreamElementType* const dataStreamElement = &pxDataStreamElementList[i];

		switch (dataStreamElement->Type)
		{
			case PXDataTypeInt8S:
				totalReadBytes += PXDataStreamReadI8S(dataStream, (PXInt8S*)dataStreamElement->Adress);
				break;

			case PXDataTypeInt8U:
				totalReadBytes += PXDataStreamReadI8U(dataStream, (PXInt8S*)dataStreamElement->Adress);
				break;

			case PXDataTypeLEInt16S:
				totalReadBytes += PXDataStreamReadI16SE(dataStream, (PXInt16S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt16U:
				totalReadBytes += PXDataStreamReadI16UE(dataStream, (PXInt16U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt32S:
				totalReadBytes += PXDataStreamReadI32SE(dataStream, (PXInt32S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt32U:
				totalReadBytes += PXDataStreamReadI32UE(dataStream, (PXInt32U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt64S:
				totalReadBytes += PXDataStreamReadI64SE(dataStream, (PXInt64S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt64U:
				totalReadBytes += PXDataStreamReadI64UE(dataStream, (PXInt64U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeBEInt16S:
				totalReadBytes += PXDataStreamReadI16SE(dataStream, (PXInt16S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt16U:
				totalReadBytes += PXDataStreamReadI16UE(dataStream, (PXInt16U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt32S:
				totalReadBytes += PXDataStreamReadI32SE(dataStream, (PXInt32S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt32U:
				totalReadBytes += PXDataStreamReadI32UE(dataStream, (PXInt32U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt64S:
				totalReadBytes += PXDataStreamReadI64SE(dataStream, (PXInt64S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt64U:
				totalReadBytes += PXDataStreamReadI64UE(dataStream, (PXInt64U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataStreamFloat:
				totalReadBytes += PXDataStreamReadF(dataStream, (float*)dataStreamElement->Adress);
				break;

			case PXDataStreamDouble:
				totalReadBytes += PXDataStreamReadD(dataStream, (double*)dataStreamElement->Adress);
				break;

			case PXDataTypeTypeInvalid:
			default:
				break;
		}
	}

	return totalReadBytes;
}


PXSize PXDataStreamReadB(PXDataStream* const dataStream, void* value, const PXSize length)
{
	const void* currentPosition = PXDataStreamCursorPosition(dataStream);
	const PXSize moveSize = PXDataStreamCursorAdvance(dataStream, length);

	MemoryCopy(currentPosition, moveSize, value, moveSize);

	return moveSize;
}

PXSize PXDataStreamReadTextA(PXDataStream* const dataStream, PXTextASCII value, const PXSize length)
{
	const PXSize size = PXDataStreamReadB(dataStream, value, length);

	value[size] = 0;

	return size;
}

PXSize PXDataStreamReadTextW(PXDataStream* const dataStream, PXTextUNICODE value, const PXSize length)
{
	const PXSize size = PXDataStreamReadB(dataStream, value, length);

	value[size] = 0;

	return size;
}

PXSize PXDataStreamReadTextU(PXDataStream* const dataStream, PXTextUNICODE value, const PXSize length)
{
	const PXSize size = PXDataStreamReadB(dataStream, value, length);

	value[size] = 0;

	return size;
}

void PXDataStreamReadUntil(PXDataStream* const dataStream, void* value, const PXSize length, const char character)
{
	const unsigned char* currentPosition = PXDataStreamCursorPosition(dataStream);

	PXSize lengthCopy = 0;

	while (!PXDataStreamIsAtEnd(dataStream))
	{
		const unsigned char* data = PXDataStreamCursorPosition(dataStream);
		const unsigned char advance = *data != character && length <= lengthCopy;

		if (!advance)
		{
			break;
		}

		++lengthCopy;

		PXDataStreamCursorAdvance(dataStream, 1u);
	}

	const PXSize readableSize = PXDataStreamRemainingSize(dataStream);

	MemoryCopy(currentPosition, readableSize, value, lengthCopy);
}

PXBool PXDataStreamReadAndCompare(PXDataStream* const dataStream, const void* value, const PXSize length)
{
	const void* currentPosition = PXDataStreamCursorPosition(dataStream);
	const PXSize readableSize = PXDataStreamRemainingSize(dataStream);

	const PXBool result = MemoryCompare(currentPosition, readableSize, value, length);

	if (result)
	{
		PXDataStreamCursorAdvance(dataStream, length);
	}

	return result;
}

PXBool PXDataStreamReadAndCompareV(PXDataStream* const dataStream, const void** value, const PXSize* valueElementSizeList, const PXSize valueLength)
{
	const void* currentPosition = PXDataStreamCursorPosition(dataStream);
	const PXSize readableSize = PXDataStreamRemainingSize(dataStream);

	for (size_t i = 0; i < valueLength; ++i)
	{
		void* text = value[i];
		PXSize size = valueElementSizeList[i];
		const PXBool result = MemoryCompare(currentPosition, readableSize, text, size);

		if (result)
		{
			PXDataStreamCursorAdvance(dataStream, size);
			return PXTrue;
		}
	}

	return PXFalse;
}

PXSize PXDataStreamWriteA(PXDataStream* const dataStream, const char* const text, PXSize textSize)
{
#if 1
	return PXDataStreamWriteB(dataStream, text, textSize);
#else
	const PXSize writableSize = PXDataStreamRemainingSize(dataStream);
	char* const currentPosition = (char* const)PXDataStreamCursorPosition(dataStream);

	const PXSize writtenBytes = TextCopyA(text, textSize, currentPosition, writableSize);

	PXDataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
#endif
}

PXSize PXDataStreamWriteW(PXDataStream* const dataStream, const wchar_t* const text, PXSize textSize)
{
	const PXSize writableSize = PXDataStreamRemainingSize(dataStream);
	wchar_t* const currentPosition = (wchar_t* const)PXDataStreamCursorPosition(dataStream);

	const PXSize writtenBytes = TextCopyW(text, textSize, currentPosition, writableSize);

	PXDataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
}

PXSize PXDataStreamWriteFill(PXDataStream* const dataStream, const unsigned char value, const PXSize length)
{
	void* const beforePosition = PXDataStreamCursorPosition(dataStream);
	const PXSize writableSize = PXDataStreamRemainingSize(dataStream);
	const PXSize write = MathMinimumIU(writableSize, length);

	MemorySet(beforePosition, value, write);

	PXDataStreamCursorAdvance(dataStream, write);

	return write;
}

PXSize PXDataStreamWrite(PXDataStream* const dataStream, const char* format, ...)
{
	const unsigned char* currentPosition = PXDataStreamCursorPosition(dataStream);

	va_list args;
	va_start(args, format);

	const PXSize writableSize = PXDataStreamRemainingSize(dataStream);
	const int writtenBytes = PrintSVN(currentPosition, writableSize, format, args);

	va_end(args);

	{
		const unsigned char sucessful = writtenBytes >= 0;

		if (!sucessful)
		{
			return 0;
		}
	}

	PXDataStreamCursorAdvance(dataStream, writtenBytes);

	return writtenBytes;
}

PXSize PXDataStreamSkipBitsToNextByte(PXDataStream* const dataStream)
{
	const unsigned char hasAnyBitConsumed = dataStream->DataCursorBitOffset > 0;

	if (hasAnyBitConsumed)
	{
		dataStream->DataCursorBitOffset = 0; // Reset
		++dataStream->DataCursor; // Go 1 Byte further
	}

	return 0;
}

PXSize PXDataStreamCursorMoveBits(PXDataStream* const dataStream, const PXSize amountOfBits)
{
	dataStream->DataCursorBitOffset += amountOfBits;

	PXDataStreamBitsAllign(dataStream);

	return 0;
}

PXSize PXDataStreamBitsAllign(PXDataStream* const dataStream)
{
	PXSize accumulator = 0;

	while (dataStream->DataCursorBitOffset >= 8u) // Move a Byte__ at the time forward, 8 Bits = 1 Byte__.
	{
		dataStream->DataCursor++;
		dataStream->DataCursorBitOffset -= 8u;

		++accumulator;
	}

	return accumulator;
}

PXSize PXDataStreamPeekBits(PXDataStream* const dataStream, const PXSize amountOfBits)
{
	unsigned int bitMask = ((1u << amountOfBits) - 1u) << dataStream->DataCursorBitOffset; // 0000111111
	unsigned int bitBlock;
	unsigned char* a =  PXDataStreamCursorPosition(dataStream);
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

PXSize PXDataStreamReadBits(PXDataStream* const dataStream, const PXSize amountOfBits)
{
	const PXSize result = PXDataStreamPeekBits(dataStream, amountOfBits);

	PXDataStreamCursorMoveBits(dataStream, amountOfBits);

	return result;
}

PXSize PXDataStreamWriteBits(PXDataStream* const dataStream, const PXSize bitData, const PXSize amountOfBits)
{
	PXSize* const currentPos = (PXSize* const)PXDataStreamCursorPosition(dataStream);
	PXSize bitBlockCache = 0;

	for (PXSize i = 0; i < amountOfBits; i++)
	{
		// Data = current bit << current offset
		bitBlockCache |= (bitData & ((PXSize)1u << i));
	}

	*currentPos |= bitBlockCache << dataStream->DataCursorBitOffset;
	dataStream->DataCursorBitOffset += amountOfBits;

	const PXSize movedBytes = PXDataStreamBitsAllign(dataStream);

	return amountOfBits;
}

PXSize PXDataStreamFilePathGetA(PXDataStream* const dataStream, char* const filePath, const PXSize filePathMaxSize)
{
#if OSUnix
    return 0; // TODO

#elif OSWindows
	const PXSize length = GetFinalPathNameByHandleA(dataStream->FileHandle, filePath, filePathMaxSize, VOLUME_NAME_DOS); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u == length;

	if (!sucessful)
	{
		const PXActionResult result = GetCurrentError();

		return result;
	}

	return PXActionSuccessful;
#endif
}

PXSize PXDataStreamFilePathGetW(PXDataStream* const dataStream, wchar_t* const filePath, const PXSize filePathMaxSize)
{
#if OSUnix
    return 0; // TODO

#elif OSWindows
	const DWORD flags = VOLUME_NAME_DOS | FILE_NAME_NORMALIZED;
	const DWORD length = GetFinalPathNameByHandleW(dataStream->FileHandle, filePath, filePathMaxSize, flags); // Minimum support: Windows Vista, Windows.h, Kernel32.dll
	const PXBool sucessful = 0u != length;

	if (!sucessful)
	{
		const PXActionResult result = GetCurrentError();

		return result;
	}

	wchar_t buffer[PathMaxSize];

	if (filePath[0] == '\\')
	{
		TextCopyW(filePath+4, length-4, buffer, PathMaxSize);
		TextCopyW(buffer, PathMaxSize, filePath, length-4);
	}

	return PXActionSuccessful;
#endif
}




PXSize PXDataStreamWriteI8S(PXDataStream* const dataStream, const PXInt8S value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt8S));
}

PXSize PXDataStreamWriteI8SV(PXDataStream* const dataStream, const PXInt8S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt8S) * valueListSize);
}

PXSize PXDataStreamWriteI8U(PXDataStream* const dataStream, const PXInt8U value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt8U));
}

PXSize PXDataStreamWriteAtI8U(PXDataStream* const dataStream, const PXInt8U value, const PXSize index)
{
	const PXSize positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const PXSize writtenBytes = PXDataStreamWriteI8U(dataStream, value); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

PXSize PXDataStreamWriteI8UV(PXDataStream* const dataStream, const PXInt8U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt8U) * valueListSize);
}

PXSize PXDataStreamWriteI16S(PXDataStream* const dataStream, const PXInt16S const value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt16S));
}

PXSize PXDataStreamWriteI16SV(PXDataStream* const dataStream, const PXInt16S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt16S) * valueListSize);
}

PXSize PXDataStreamWriteI16SE(PXDataStream* const dataStream, const PXInt16S const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt16S);
	PXInt16S dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteI16SVE(PXDataStream* const dataStream, const PXInt16S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI16SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteI16U(PXDataStream* const dataStream, const PXInt16U const value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt16U));
}

PXSize PXDataStreamWriteAtI16U(PXDataStream* const dataStream, const PXInt16U const value, const PXSize index)
{
	const PXSize positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const PXSize writtenBytes = PXDataStreamWriteI16U(dataStream, value); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

PXSize PXDataStreamWriteI16UV(PXDataStream* const dataStream, const PXInt16U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt16U) * valueListSize);
}

PXSize PXDataStreamWriteI16UE(PXDataStream* const dataStream, const PXInt16U const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt16U);
	PXInt16U dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteI16UVE(PXDataStream* const dataStream, const PXInt16U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI16UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteI32S(PXDataStream* const dataStream, const PXInt32S value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt32S));
}

PXSize PXDataStreamWriteI32SV(PXDataStream* const dataStream, const PXInt32S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt32S) * valueListSize);
}

PXSize PXDataStreamWriteI32SE(PXDataStream* const dataStream, const PXInt32S value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt32S);
	PXInt32S dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteI32SVE(PXDataStream* const dataStream, const PXInt32S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI32SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteI32U(PXDataStream* const dataStream, const PXInt32U value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt32U));
}

PXSize PXDataStreamWriteAtI32U(PXDataStream* const dataStream, const PXInt32U value, const PXSize index)
{
	const PXSize positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const PXSize writtenBytes = PXDataStreamWriteI32U(dataStream, value); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

PXSize PXDataStreamWriteI32UV(PXDataStream* const dataStream, const PXInt32U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt32U) * valueListSize);
}

PXSize PXDataStreamWriteI32UE(PXDataStream* const dataStream, const PXInt32U value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt32U);
	PXInt32U dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteAtI32UE(PXDataStream* const dataStream, const PXInt32U value, const Endian endian, const PXSize index)
{
	const PXSize positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const PXSize writtenBytes = PXDataStreamWriteI32UE(dataStream, value, endian); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

PXSize PXDataStreamWriteI32UVE(PXDataStream* const dataStream, const PXInt32U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI32UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteI64S(PXDataStream* const dataStream, const PXInt64S const value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt64S));
}

PXSize PXDataStreamWriteI64SV(PXDataStream* const dataStream, const PXInt64S* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt64S) * valueListSize);
}

PXSize PXDataStreamWriteI64SE(PXDataStream* const dataStream, const PXInt64S const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt64S);
	PXInt64S dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteI64VE(PXDataStream* const dataStream, const PXInt64S* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI64SE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteI64U(PXDataStream* const dataStream, const PXInt64U const value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(PXInt64U));
}

PXSize PXDataStreamWriteI64UV(PXDataStream* const dataStream, const PXInt64U* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(PXInt64U) * valueListSize);
}

PXSize PXDataStreamWriteI64UE(PXDataStream* const dataStream, const PXInt64U const value, const Endian endian)
{
	const PXSize dataSize = sizeof(PXInt64U);
	PXInt64U dataValue = value;

	EndianSwap(&dataValue, dataSize, EndianCurrentSystem, endian);

	PXDataStreamWriteB(dataStream, &dataValue, dataSize);

	return dataSize;
}

PXSize PXDataStreamWriteI64UVE(PXDataStream* const dataStream, const PXInt64U* const valueList, const PXSize valueListSize, const Endian endian)
{
	PXSize accumulator = 0;

	for (PXSize i = 0; i < valueListSize; ++i)
	{
		accumulator += PXDataStreamWriteI64UE(dataStream, &valueList[i], endian);
	}

	return accumulator;
}

PXSize PXDataStreamWriteF(PXDataStream* const dataStream, const float value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(float));
}

PXSize PXDataStreamWriteFV(PXDataStream* const dataStream, const float* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(float) * valueListSize);
}

PXSize PXDataStreamWriteD(PXDataStream* const dataStream, const double value)
{
	return PXDataStreamWriteB(dataStream, &value, sizeof(double));
}

PXSize PXDataStreamWriteDV(PXDataStream* const dataStream, const double* const valueList, const PXSize valueListSize)
{
	return PXDataStreamWriteB(dataStream, valueList, sizeof(double) * valueListSize);
}

PXSize PXDataStreamWriteB(PXDataStream* const dataStream, const void* const value, const PXSize length)
{
	const PXSize writableSize = PXDataStreamRemainingSize(dataStream);
	void* const currentPosition = PXDataStreamCursorPosition(dataStream);
	const PXSize copyedBytes = MemoryCopy(value, length, currentPosition, writableSize);

	PXDataStreamCursorAdvance(dataStream, copyedBytes);

	return copyedBytes;
}

PXSize PXDataStreamWriteAtB(PXDataStream* const dataStream, const void* const data, const PXSize dataSize, const PXSize index)
{
	const PXSize positionBefore = dataStream->DataCursor; // save current position

	dataStream->DataCursor = index; // jump to offset

	const PXSize writtenBytes = PXDataStreamWriteB(dataStream, data, dataSize); // Length

	dataStream->DataCursor = positionBefore; // Reset old position

	return writtenBytes;
}

PXSize PXDataStreamWriteMultible(PXDataStream* const dataStream, const PXDataStreamElementType* const pxDataStreamElementList, const PXSize pxDataStreamElementListSize)
{
	PXSize totalReadBytes = 0;

	for (PXSize i = 0; i < pxDataStreamElementListSize; ++i)
	{
		PXDataStreamElementType* const dataStreamElement = &pxDataStreamElementList[i];

		switch (dataStreamElement->Type)
		{
			case PXDataTypeInt8S:
				totalReadBytes += PXDataStreamWriteI8S(dataStream, *(PXInt8S*)dataStreamElement->Adress);
				break;

			case PXDataTypeInt8U:
				totalReadBytes += PXDataStreamWriteI8U(dataStream, *(PXInt8S*)dataStreamElement->Adress);
				break;

			case PXDataTypeLEInt16S:
				totalReadBytes += PXDataStreamWriteI16SE(dataStream, *(PXInt16S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt16U:
				totalReadBytes += PXDataStreamWriteI16UE(dataStream, *(PXInt16U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt32S:
				totalReadBytes += PXDataStreamWriteI32SE(dataStream, *(PXInt32S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt32U:
				totalReadBytes += PXDataStreamWriteI32UE(dataStream, *(PXInt32U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt64S:
				totalReadBytes += PXDataStreamWriteI64SE(dataStream, *(PXInt64S*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeLEInt64U:
				totalReadBytes += PXDataStreamWriteI64UE(dataStream, *(PXInt64U*)dataStreamElement->Adress, EndianLittle);
				break;

			case PXDataTypeBEInt16S:
				totalReadBytes += PXDataStreamWriteI16SE(dataStream, *(PXInt16S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt16U:
				totalReadBytes += PXDataStreamWriteI16UE(dataStream, *(PXInt16U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt32S:
				totalReadBytes += PXDataStreamWriteI32SE(dataStream, *(PXInt32S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt32U:
				totalReadBytes += PXDataStreamWriteI32UE(dataStream, *(PXInt32U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt64S:
				totalReadBytes += PXDataStreamWriteI64SE(dataStream, *(PXInt64S*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataTypeBEInt64U:
				totalReadBytes += PXDataStreamWriteI64UE(dataStream, *(PXInt64U*)dataStreamElement->Adress, EndianBig);
				break;

			case PXDataStreamFloat:
				totalReadBytes += PXDataStreamWriteF(dataStream, *(float*)dataStreamElement->Adress);
				break;

			case PXDataStreamDouble:
				totalReadBytes += PXDataStreamWriteD(dataStream, *(double*)dataStreamElement->Adress);
				break;

			case PXDataTypeTypeInvalid:
			default:
				break;
		}
	}

	return totalReadBytes;
}