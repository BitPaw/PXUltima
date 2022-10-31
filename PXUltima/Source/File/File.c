#include "File.h"

#include <Text/Text.h>
#include <Memory/PXMemory.h>

#include <stdarg.h>
#include <fcntl.h>


#define FileReadMode "rb"
#define FileReadModeW L"rb"

#define FileWriteMode "wb"
#define FileWriteModeW L"wb"

#define FileReadWriteMode "rwb"
#define FileReadWriteModeW L"rwb"


#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>

#define OSFileOpenA fopen
#define OSFileOpenW(string, mode) FileOpenA((const char*)string, (const char*)mode)
#define OSFileRemoveA remove
#define OSFileRemoveW(string) remove((const char*)string)
#define OSFileRenameA rename
#define OSFileRenameW(oldName, newName) rename((const char*)oldName, (const char*)newName)
#define OSFileDirectoryCreateA(string) mkdir(string, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define OSFileDirectoryCreateW(string) OSFileDirectoryCreateA((const char*)string)
#define OSWorkingDirectoryCurrentA getcwd
#define OSWorkingDirectoryCurrentW(string, size) (wchar_t*)OSWorkingDirectoryCurrentA((char*)string, size)
#define OSWorkingDirectoryChangeA chdir
#define OSWorkingDirectoryChangeW(string) OSWorkingDirectoryChangeA((const char*)string)

#define PipeOpen popen
#define PipeOpenW(wchar, mode) popen((char*)wchar, (const char*) mode) // TODO: instable
#define PipeClose pclose

#elif OSWindows
#include <io.h>

#define OSFileOpenA fopen
#define OSFileOpenW _wfopen
#define OSFileRemoveA remove
#define OSFileRemoveW _wremove
#define OSFileRenameA rename
#define OSFileRenameW _wrename
#define OSFileDirectoryCreateA _mkdir
#define OSFileDirectoryCreateW _wmkdir
#define OSWorkingDirectoryCurrentA _getcwd
#define OSWorkingDirectoryCurrentW _wgetcwd
#define OSWorkingDirectoryChangeA _chdir
#define OSWorkingDirectoryChangeW _wchdir


#define PipeOpen _popen
#define PipeOpenW _wpopen
#define PipeClose _pclose

// WinNT.h
// FILE_ATTRIBUTE_TEMPORARY

#endif



void FilePathSplittA(const char* fullPath, size_t fullPathMaxSize, char* drive, size_t driveMaxSize, char* directory, size_t directoryMaxSize, char* fileName, size_t fileNameMaxSize, char* extension, size_t extensionMaxSize)
{
#if OSUnix
	char directoryNameCache[PathMaxSize];
	char baseNameCache[FileNameMaxSize];

	TextCopyA(fullPath, FileNameMaxSize, directoryNameCache, FileNameMaxSize);
	TextCopyA(fullPath, FileNameMaxSize, baseNameCache, FileNameMaxSize);

	char* dirNameResult = dirname(directoryNameCache);
	char* baseNameResult = basename(baseNameCache);

	size_t directoryLength = TextCopyA(dirNameResult, DirectoryMaxSize, directory, DirectoryMaxSize);
	size_t fileNameLength = TextCopyA(baseNameResult, FileNameMaxSize, fileName, FileNameMaxSize);

	for(size_t i = fileNameLength - 1; i > 0; --i)
	{
		const unsigned char isDot = fileName[i] == '.';

		if(isDot)
		{
			TextCopyA(fileName + i + 1, ExtensionMaxSize - i, extension, extensionMaxSize);
			break;
		}
	}
#elif OSWindows
	char fileNameCache[FileNameMaxSize];

	_splitpath_s
	(
		fullPath,
		drive, driveMaxSize,
		directory, directoryMaxSize,
		fileName, fileNameMaxSize,
		extension, extensionMaxSize
	);

	for(size_t i = 0; fileName[i] != '\0'; i++)
	{
		const unsigned char isDot = fileName[i] == '.';

		if(isDot)
		{
			TextCopyA(extension + i, extensionMaxSize, fileNameCache, FileNameMaxSize);
			TextCopyA(fileNameCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void FilePathSplittW(const wchar_t* fullPath, size_t fullPathMaxSize, wchar_t* drive, size_t driveMaxSize, wchar_t* directory, size_t directoryMaxSize, wchar_t* fileName, size_t fileNameMaxSize, wchar_t* extension, size_t extensionMaxSize)
{
#if OSUnix
	char fullPathA[PathMaxSize];
	char driveA[DriveMaxSize];
	char directoryA[DirectoryMaxSize];
	char fileNameA[FileNameMaxSize];
	char extensionA[ExtensionMaxSize];

	TextCopyWA(fullPath, PathMaxSize, fullPathA, PathMaxSize);

	FilePathSplittA
	(
		fullPathA,PathMaxSize,
		driveA,DriveMaxSize,
		directoryA,DirectoryMaxSize,
		fileNameA, FileNameMaxSize,
		extensionA, ExtensionMaxSize
	);

	TextCopyAW(driveA, DriveMaxSize, drive, DriveMaxSize);
	TextCopyAW(directoryA, DirectoryMaxSize, directory, DirectoryMaxSize);
	TextCopyAW(fileNameA, FileNameMaxSize, fileName, FileNameMaxSize);
	TextCopyAW(extensionA, ExtensionMaxSize, extension, ExtensionMaxSize);
#elif OSWindows
	wchar_t extensionCache[FileNameMaxSize];

	_wsplitpath_s
	(
		fullPath,
		drive, driveMaxSize,
		directory, directoryMaxSize,
		fileName, fileNameMaxSize,
		extension, extensionMaxSize
	);

	for(size_t i = 0; extension[i] != '\0'; i++)
	{
		const unsigned char isDot = extension[i] == '.';

		if(isDot)
		{
			TextCopyW(extension + i + 1, extensionMaxSize, extensionCache, FileNameMaxSize);
			TextCopyW(extensionCache, FileNameMaxSize, extension, extensionMaxSize);
			break;
		}
	}
#endif
}

void FilePathSplittPositionW(const wchar_t* fullPath, size_t fullPathMaxSize, size_t* drivePos, size_t driveSize, size_t* directory, size_t directorySize, size_t* fileName, size_t fileNameSize, size_t* extension, size_t extensionSize)
{
}

void FilePathExtensionGetA(const char* filePath, const size_t filePathSize, char* extension, const size_t extensionSizeMax)
{
	const size_t index = TextFindLastA(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != TextIndexNotFound;

	if(!hasExtension)
	{
		TextClearA(extension, extensionSizeMax);
		return;
	}

	TextCopyA(filePath + index, filePathSize - index, extension, extensionSizeMax);
}

void FilePathExtensionGetW(const wchar_t* filePath, const size_t filePathSize, wchar_t* extension, const size_t extensionSizeMax)
{
	const size_t index = TextFindLastW(filePath, filePathSize, '.');
	const unsigned char hasExtension = index != TextIndexNotFound;

	if(!hasExtension)
	{
		TextClearW(extension, extensionSizeMax);
		return;
	}

	TextCopyW(filePath + index + 1, filePathSize - index, extension, extensionSizeMax);
}

unsigned char FileDoesExistA(const char* filePath)
{
	return 0;
}

unsigned char FileDoesExistW(const wchar_t* filePath)
{
	return 0;
}

ActionResult FileRemoveA(const char* filePath)
{
	int removeResult = OSFileRemoveA(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return ActionInvalid;
}

ActionResult FileRemoveW(const wchar_t* filePath)
{
	int removeResult = OSFileRemoveW(filePath);
	//ErrorCode errorCode = ConvertErrorCode(removeResult);

	return ActionInvalid;
}

ActionResult FileRenameA(const char* oldName, const char* newName)
{
	int renameResult = OSFileRenameA(oldName, newName);
	//ErrorCode errorCode = ConvertErrorCode(renameResult);

	return ActionInvalid;
}

ActionResult FileRenameW(const wchar_t* oldName, const wchar_t* newName)
{
	int renameResult = OSFileRenameW(oldName, newName);
	const unsigned char wasSuccesful = renameResult == 0;

	if(!wasSuccesful)
	{
		return ActionInvalid;// GetCurrentError();
	}

	return ActionSuccessful;
}

ActionResult FileCopyA(const char* sourceFilePath, const char* destinationFilePath)
{
	if(!sourceFilePath || !destinationFilePath)
	{
		return ResultEmptyPath;
	}
#if OSUnix
	FILE* fileSource = fopen(sourceFilePath, FileReadMode);
	FILE* fileDestination = fopen(destinationFilePath, FileWriteMode);
	const unsigned char fileOpenSuccesful = fileSource && fileDestination;

	const size_t swapBufferSize = 2048;
	unsigned char swapBuffer[swapBufferSize];

	if(!fileOpenSuccesful)
	{
		return ResultFileOpenFailure;
	}

	while(!feof(fileSource))
	{
		size_t readBytes = fread(swapBuffer, sizeof(char), swapBufferSize, fileSource);
		size_t writtenBytes = fwrite(swapBuffer, sizeof(char), readBytes, fileDestination);
	}

	fclose(fileSource);
	fclose(fileDestination);
#elif OSWindows
	unsigned char succesfull = CopyFileA(sourceFilePath, destinationFilePath, 0);

	if(!succesfull)
	{
		return ResultFileCopyFailure;
	}
#endif

	return ActionSuccessful;
}

ActionResult FileCopyW(const wchar_t* sourceFilePath, const wchar_t* destinationFilePath)
{
	if(!sourceFilePath || !destinationFilePath)
	{
		return ResultEmptyPath;
	}

#if OSUnix
	char sourceFilePathA[PathMaxSize];
	char destinationFilePathA[PathMaxSize];

	TextCopyWA(sourceFilePath, PathMaxSize, sourceFilePathA, PathMaxSize);
	TextCopyWA(destinationFilePath, PathMaxSize, destinationFilePathA, PathMaxSize);

	return FileCopyA(sourceFilePathA, destinationFilePathA);
#elif OSWindows
	const unsigned char succesfull = CopyFileW(sourceFilePath, destinationFilePath, 0);

	if(!succesfull)
	{
		return ResultFileCopyFailure;
	}

	return ActionSuccessful;
#endif
}

void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
	const size_t index = TextFindLastW(currnetPath, PathMaxSize, '/');
	const unsigned char found = index != -1;

	if(found)
	{
		const size_t copyedBytes = TextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const size_t toCopy = PathMaxSize - copyedBytes;

		TextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
	}
}

void FilePathSwapFileNameW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileName)
{
	wchar_t extension[ExtensionMaxSize];

	size_t indexFileName = TextFindLastW(inputPath, PathMaxSize, '/');
	const unsigned char found = indexFileName != -1;

	if (!found)
	{
		indexFileName = 0;
	}

	// Fetch extension
	{
		const size_t indexDot = TextFindLastW(inputPath, PathMaxSize, '.'); // Find last dot
		const unsigned char found = indexDot != -1;

		if (!found)
		{
			return;
		}

		TextCopyW(&inputPath[indexDot], PathMaxSize, extension, ExtensionMaxSize);
	}

	// Copy old filename
	TextCopyW(inputPath, PathMaxSize, exportPath, PathMaxSize);
	TextCopyW(fileName, PathMaxSize, &exportPath[indexFileName], PathMaxSize);

	// Add old extension
	{
		size_t length = TextLengthW(exportPath, PathMaxSize);

		TextCopyW(extension , PathMaxSize, &exportPath[length], ExtensionMaxSize);
	}
}

void FilePathSwapExtensionW(const wchar_t* const inputPath, wchar_t* const exportPath, const wchar_t* const fileExtension)
{
	const size_t index = TextFindLastW(inputPath, PathMaxSize, '.'); // Find last dot
	const unsigned char found = index != -1;

	if (!found)
	{
		return;
	}

	const size_t written = TextCopyW(inputPath, index+1, exportPath, PathMaxSize); // Copy filePath without extension
	const size_t writtenFull = TextCopyW(fileExtension , PathMaxSize, &exportPath[written], PathMaxSize); // Copy extension on top
}

ActionResult DirectoryCreateA(const char* directoryName)
{
	const int creationResult = OSFileDirectoryCreateA(directoryName);
	const unsigned char wasSuccesful = creationResult == 0;

	if(!wasSuccesful)
	{
		return ActionInvalid; //GetCurrentError();
	}

	return ActionSuccessful;
}

ActionResult DirectoryCreateW(const wchar_t* directoryName)
{
	wchar_t directoryNameSegment[PathMaxSize];
	size_t starPos = 0;
	size_t successful = 0;

	do
	{
		size_t offset = TextFindFirstW(directoryName + starPos, PathMaxSize - starPos, '/');

		if (offset == -1)
		{
			break;
		}

		TextCopyW(directoryName + starPos, offset-1, directoryNameSegment, PathMaxSize);

		const int creationResult = OSFileDirectoryCreateW(directoryNameSegment);
		const unsigned char wasSuccesful = creationResult == 0;

		if (!wasSuccesful)
		{
			break;
			//return ActionInvalid; //GetCurrentError();
		}

		starPos += offset;
		++successful;
	} 
	while (1);
	   	
	return ActionSuccessful;
}

ActionResult WorkingDirectoryChange(const char* directoryName)
{
	return ActionInvalid;
}

ActionResult WorkingDirectoryGetA(char* workingDirectory, size_t workingDirectorySize)
{
	char* workingDirectoryResult = OSWorkingDirectoryCurrentA(workingDirectory, workingDirectorySize);
	const unsigned char  wasSuccesful = workingDirectoryResult;

	if(!wasSuccesful)
	{
		return ActionInvalid; //GetCurrentError();
	}

	return ActionSuccessful;
}

ActionResult WorkingDirectoryGetW(wchar_t* workingDirectory, size_t workingDirectorySize)
{
	wchar_t* workingDirectoryResult = OSWorkingDirectoryCurrentW(workingDirectory, workingDirectorySize);
	const unsigned char  wasSuccesful = workingDirectoryResult;

	if(!wasSuccesful)
	{
		return ActionInvalid; //GetCurrentError();
	}

	return ActionSuccessful;
}

ActionResult WorkingDirectoryChangeW(const wchar_t* directoryName)
{
	return ActionInvalid;
}

ActionResult DirectoryDeleteA(const char* directoryName)
{
	return ActionInvalid;
}

ActionResult DirectoryDeleteW(const wchar_t* directoryName)
{
	return ActionInvalid;
}

ActionResult DirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, size_t* listSize)
{
	return ActionInvalid;
}

ActionResult DirectoryFilesInFolderW(const wchar_t* folderPath, wchar_t*** list, size_t* listSize)
{
	return ActionInvalid;
}


/*

ActionResult FileWorkingDirectoryChangeA(const char* directoryName)
{
	int creationResult = WorkingDirectoryChangeA(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

ActionResult FileWorkingDirectoryChangeW(const wchar_t* directoryName)
{
	int creationResult = WorkingDirectoryChangeW(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

ActionResult FileDirectoryDeleteA(const char* directoryName)
{
	int creationResult = FileRemoveA(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

ActionResult FileDirectoryDeleteW(const wchar_t* directoryName)
{
	int creationResult = FileRemoveW(directoryName);
	bool wasSuccesful = creationResult == 0;

	if (!wasSuccesful)
	{
		return GetCurrentError();
	}

	return ErrorCode::Successful;
}

ActionResult FileMapToVirtualMemoryA(const char* filePath, const MemoryProtectionMode protectionMode)
{
}

ActionResult FileMapToVirtualMemoryW(const wchar_t* filePath, const MemoryProtectionMode protectionMode)
{

}

ActionResult FileMapToVirtualMemory(const size_t size, const MemoryProtectionMode protectionMode)
{

}

ActionResult FileUnmapFromVirtualMemory()
{

}

ActionResult FileReadFromDisk(const char* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;

	// Open file
	{
		const ActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);
		const bool sucessful = result == ActionSuccessful;

		if(!sucessful)
		{
			return result;
		}
	}

	// Read
	{
		const ActionResult result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);
		const bool sucessful = result == ActionSuccessful;

		if(!sucessful)
		{
			file.Close();
			return result;
		}
	}

	// Close
	{
		const ActionResult result = file.Close();
		const bool sucessful = result == ActionSuccessful;

		if(!sucessful)
		{
			return result;
		}
	}

	_fileLocation = FileLocation::CachedFromDisk;

	return ActionSuccessful;
}

ActionResult FileReadFromDisk(const wchar_t* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;
	ActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);

	if(result != ActionSuccessful)
	{
		return result;
	}

	result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);

	if(result != ActionSuccessful)
	{
		file.Close();
		return result;
	}

	result = file.Close();

	if(result != ActionSuccessful)
	{
		return result;
	}

	_fileLocation = FileLocation::CachedFromDisk;

	return ActionSuccessful;
}

ActionResult FileReadFromDisk(FILE* file, Byte__** targetBuffer, size_t& bufferSize, bool addNullTerminator)
{
	fseek(file, 0, SEEK_END); // Jump to end of file
	bufferSize = ftell(file); // Get current 'data-cursor' position

	if(!bufferSize) // If no bytes in file, exit.
	{
		return ResultFileEmpty;
	}

	rewind(file); // Jump to the begining of the file

	if(addNullTerminator)
	{
		++bufferSize;
	}

	Byte__* dataBuffer = Memory::Allocate<Byte__>(bufferSize);

	if(!dataBuffer) // If malloc failed
	{
		return ResultOutOfMemory;
	}

	*targetBuffer = dataBuffer;

	if(addNullTerminator)
	{
		dataBuffer[bufferSize - 1] = '\0';
		--bufferSize;
	}

	size_t readBytes = fread(dataBuffer, 1u, bufferSize, file);
	size_t overAllocatedBytes = bufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

	assert(bufferSize == readBytes);

	return ActionSuccessful;
}

ActionResult FileReadFromDisk(const wchar_t* filePath, Byte__** targetBuffer, size_t& bufferSize, bool addNullTerminator, FilePersistence filePersistence)
{
	File file;
	ActionResult result = file.Open(filePath, FileOpenMode::Read);

	if(result != ActionSuccessful)
	{
		return result;
	}

	//result = ReadFromDisk(file.FileMarker, targetBuffer, bufferSize, addNullTerminator);

	file.ReadFromDisk(targetBuffer, bufferSize, addNullTerminator);

	result = file.Close();

	return ActionSuccessful;
}

ActionResult FileWriteToDisk(const bool value)
{
	return WriteToDisk(&value, sizeof(bool));
}

ActionResult FileWriteToDisk(const char value)
{
	return WriteToDisk(&value, sizeof(char));
}

ActionResult FileWriteToDisk(const unsigned char value)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const short value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const unsigned short value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const int value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const unsigned int value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const char* string, const size_t length)
{
	return WriteToDisk(string, length);
}

ActionResult FileWriteToDisk(const unsigned char* string, const size_t length)
{
	return WriteToDisk(string, length);
}

ActionResult FileWriteToDisk(const unsigned long long& value, const Endian endian)
{
	return WriteToDisk(&value, sizeof(unsigned char));
}

ActionResult FileWriteToDisk(const void* value, const size_t length)
{
#if OSUnix
FILE* fileHandle = FileHandle;
#elif OSWindows
FILE* fileHandle = FileHandleCStyle;
#endif

	const size_t writtenSize = fwrite(value, sizeof(Byte__), length, fileHandle);

	if(writtenSize > 0)
	{
		return ActionSuccessful;
	}
	else
	{
		return ResultWriteFailure;
	}
}

ActionResult FileWriteToDisk(const char* format, ...)
{
#if OSUnix
    FILE* fileHandle = FileHandle;
#elif OSWindows
    FILE* fileHandle = FileHandleCStyle;
#endif

	va_list args;
	va_start(args, format);

	const int writtenBytes = vfprintf(fileHandle, format, args);
	const bool sucessful = writtenBytes >= 0;

	va_end(args);

	if(!sucessful)
	{
		return ResultWriteFailure;
	}

	return ActionSuccessful;
}

ActionResult FileWriteIntoFile(const void* data, const size_t dataSize)
{
#if OSUnix
	size_t writtenBytes = fwrite(data, sizeof(char), dataSize, FileHandle);
#elif OSWindows
	DWORD writtenBytes = 0;
	const bool successful = WriteFile(FileHandle, data, dataSize, &writtenBytes, nullptr);
#endif

	return ActionResult();
}

ActionResult FileWriteToDisk(const char* filePath, FilePersistence filePersistence)
{
	File file;
	ActionResult fileActionResult = file.Open(filePath, FileOpenMode::Write);

	if(fileActionResult != ActionSuccessful)
	{
		return fileActionResult;
	}

	WriteIntoFile(Data, DataSize);

	fileActionResult = file.Close();

	return ActionSuccessful;
}

ActionResult FileWriteToDisk(const wchar_t* filePath, FilePersistence filePersistence)
{
	File file;

	{
		const ActionResult fileActionResult = file.Open(filePath, FileOpenMode::Write);
		const bool sucessful = fileActionResult == ActionSuccessful;

		if(!sucessful)
		{
			return fileActionResult;
		}
	}

#if OSUnix
	size_t writtenBytes = fwrite(Data, sizeof(char), DataCursorPosition, file.FileHandle);
#elif OSWindows
	DWORD writtenBytes = 0;
	const bool successful = WriteFile(file.FileHandle, Data, DataCursor, &writtenBytes, nullptr);
#endif

	{
		const ActionResult closeResult = file.Close();
		const bool sucessful = closeResult == ActionSuccessful;

		if(!sucessful)
		{
			return closeResult;
		}
	}

	return ActionSuccessful;
}

ActionResult FileReadFromDisk(unsigned char** outPutBuffer, size_t& outPutBufferSize, const bool addTerminatorByte)
{
#if OSUnix
	fseek(FileHandle, 0, SEEK_END); // Jump to end of file
	outPutBufferSize = ftell(FileHandle); // Get current 'data-cursor' position

	if (!outPutBufferSize) // If no bytes in file, exit.
	{
		return ResultFileEmpty;
	}

	rewind(FileHandle); // Jump to the begining of the file

	if (addTerminatorByte)
	{
		++outPutBufferSize;
	}

	unsigned char* dataBuffer = Memory::Allocate<unsigned char>(outPutBufferSize);

	if (!dataBuffer) // If malloc failed
	{
		return ResultOutOfMemory;
	}

	*outPutBuffer = dataBuffer;

	if (addTerminatorByte)
	{
		dataBuffer[outPutBufferSize - 1] = '\0';
		--outPutBufferSize;
	}

	size_t readBytes = fread(dataBuffer, 1u, outPutBufferSize, FileHandle);
	size_t overAllocatedBytes = outPutBufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

	assert(outPutBufferSize == readBytes);

	return ActionSuccessful;
#elif OSWindows
	const DWORD fileSize = GetFileSize(FileHandle, nullptr);
	DWORD numberOfBytesRead = 0;
	OVERLAPPED* overlapped = nullptr;
	//LARGE_INTEGER size;
	//bool succesSize = GetFileSizeEx(FileHandle, &size);
	size_t allocationSize = fileSize;

	if (addTerminatorByte)
	{
		++allocationSize;
	}

	unsigned char* buffer = Memory::Allocate<unsigned char>(allocationSize);

	if (!buffer)
	{
		return ResultOutOfMemory;
	}

	bool sucessful = ReadFile
	(
		FileHandle, // file
		buffer, // input
		fileSize, // Shall read
		&numberOfBytesRead, // Has read
		overlapped
	);

	if (!sucessful)
	{
		return ResultFileReadFailure;
	}

	buffer[allocationSize - 1] = '\0';

	(*outPutBuffer) = buffer;
	outPutBufferSize = numberOfBytesRead;

	return ActionSuccessful;
#endif
}

void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
	const size_t index = TextFindLastW(currnetPath, PathMaxSize, '/');
	const bool found = index != -1;

	if (found)
	{
		const size_t copyedBytes = TextCopyW(currnetPath, index + 1, targetPath, index + 1);
		const size_t toCopy = PathMaxSize - copyedBytes;

		TextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
	}
}

void FileFilesInFolder(const char* folderPath, wchar_t*** list, size_t& listSize)
{
#if OSUnix
	DIR* directory = opendir(folderPath);

	if (directory)
	{
		struct dirent* directoryInfo = nullptr;

		while (directoryInfo = readdir(directory))
		{
			++listSize;
		}

		rewinddir(directory);

		(*list) = Memory::Allocate<wchar_t*>(listSize);

		for (size_t index = 0; directoryInfo = readdir(directory); index++)
		{
			const bool isFile = directoryInfo->d_type == DT_REG || true;

			if (isFile)
			{
				const char* fileName = directoryInfo->d_name;
				const size_t length = Text::Length(fileName);
				wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);
				wchar_t** target = &(*list)[index];

				if (!newString)
				{
					return; // Error: OutOfMemory
				}

				(*target) = newString;
				size_t writtenBytes = Text::Copy(fileName, length, *target, length);
			}
		}

		closedir(directory);
	}
#elif OSWindows
	wchar_t folderPathW[PathMaxSize];
	size_t writtenBytes = TextCopyAW(folderPath, PathMaxSize, folderPathW, PathMaxSize);

	WIN32_FIND_DATA dataCursour{0};
	HANDLE hFind = 0;

	//Memory::Set(&dataCursour, 0, sizeof(WIN32_FIND_DATA));

	hFind = FindFirstFile(folderPathW, &dataCursour); 	// "/*.*";

	bool foundData = hFind != INVALID_HANDLE_VALUE;

	if (!foundData)
	{
		return;
	}

	++listSize;

	for (; FindNextFile(hFind, &dataCursour); listSize++);

	MemorySet(&dataCursour, 0, sizeof(WIN32_FIND_DATA));

	(*list) = Memory::Allocate<wchar_t*>(listSize);

	hFind = FindFirstFile(folderPathW, &dataCursour); // Expected "." Folder
	size_t fileIndex = 0;

	do
	{
		const size_t length = TextLengthW(dataCursour.cFileName);
		const wchar_t* filePathSource = dataCursour.cFileName;
		wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);

		if (!newString)
		{
			return; // Error: OutOfMemory
		}

		TextCopyW(filePathSource, length, newString, length);

		(*list)[fileIndex] = newString;

		fileIndex++;
	}
	while (FindNextFile(hFind, &dataCursour));

	FindClose(hFind);
#endif
}

void FileFilesInFolder(const wchar_t* folderPath, wchar_t*** list, size_t& listSize)
{

}

unsigned char FileDoesExistA(const char* filePath)
{
	FILE* file = OSFileOpenA(filePath, "rb");

	if (file)
	{
		fclose(file);

		return 1;
	}

	return 0;
}

unsigned char FileDoesExistW(const wchar_t* filePath)
{
#if OSUnix
    char filePathA[PathMaxSize];

    Text::Copy(filePath, PathMaxSize, filePathA, PathMaxSize);

    return DoesFileExist(filePathA);

#elif OSWindows
	FILE* file = OSFileOpenW(filePath, L"rb");

	if(file)
	{
		fclose(file);

		return 1;
	}
#endif // defined

	return 0;
}*/

