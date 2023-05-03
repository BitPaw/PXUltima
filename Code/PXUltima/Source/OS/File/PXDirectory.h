#ifndef PXDirectoryINCLUDE
#define PXDirectoryINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

#define PXDirectorySearchForDirectorys (1 << 0)
#define PXDirectorySearchForFiles (1 << 1)
#define PXDirectorySearchForDirectoryAndFile PXDirectorySearchForDirectorys | PXDirectorySearchForFiles

#ifdef __cplusplus
extern "C"
{
#endif


	typedef enum PXFileElementInfoType_
	{
		PXFileElementInfoTypeInvalid,
		PXFileElementInfoTypeUnkown, // DT_UNKNOWN

		PXFileElementInfoTypeFile, // DT_REG
		PXFileElementInfoTypeDictionary, // DT_DIR

		PXFileElementInfoTypeNamedPipeFIFO, // DT_FIFO
		PXFileElementInfoTypeLink, // DT_LNK
		PXFileElementInfoTypeSocket, // DT_SOCK

		PXFileElementInfoTypeDeviceCharacter, // DT_CHR
		PXFileElementInfoTypeDeviceBlock, // DT_BLK

		PXFileElementInfoTypeDictionaryRoot, // '.'
		PXFileElementInfoTypeDictionaryParent // '..'
	}
	PXFileElementInfoType;

	typedef struct PXFileElementInfo_
	{
		PXFileElementInfoType Type;

		PXInt8U Depth;

		char* FullPath;
		PXSize FullPathSize;

		char* Name;
		PXSize NameSize;

		PXSize Size;

		PXBool IsSystemDottedFolder;
	}
	PXFileElementInfo;

	typedef void (*PXFileElementDetected)(PXFileElementInfo* pxFileElementInfo);

	typedef struct PXDirectoryIterator_
	{
#if OSUnix
		DIR* ID;
		struct dirent* DirectoryEntryCurrent;
#elif OSWindows
		HANDLE ID;
		WIN32_FIND_DATA DirectoryEntryCurrent;
#endif

		PXInt8U EntryDepthCurrent;

		PXFileElementInfo EntryCurrent;
	}
	PXDirectoryIterator;

	PXPublic void PXDirectoryUpdateEntry(PXDirectoryIterator* const pxDirectoryIterator);

	PXPublic PXActionResult PXDirectoryOpen(PXDirectoryIterator* const pxDirectoryIterator, const PXText* const directoryName);
	PXPublic PXBool PXDirectoryNext(PXDirectoryIterator* const pxDirectoryIterator);
	PXPublic PXBool PXDirectoryClose(PXDirectoryIterator* const pxDirectoryIterator);



	PXPublic PXActionResult PXDirectoryCreate(const PXText* const directoryName);
	PXPublic PXActionResult PXWorkingDirectoryChange(const char* directoryName);
	PXPublic PXActionResult PXWorkingDirectoryGetA(char* workingDirectory, PXSize workingDirectorySize);
	PXPublic PXActionResult PXWorkingDirectoryGetW(wchar_t* workingDirectory, PXSize workingDirectorySize);
	PXPublic PXActionResult PXWorkingDirectoryChangeW(const wchar_t* directoryName);
	PXPublic PXActionResult PXDirectoryDeleteA(const char* directoryName);
	PXPublic PXActionResult PXDirectoryDeleteW(const wchar_t* directoryName);




	typedef struct PXDirectorySearchInfo_
	{
		PXFileElementDetected Callback;

		wchar_t* FolderPath;
		PXSize FolderPathSize;

		wchar_t* FileFilter;
		PXSize FileFilterSize;

		unsigned int Flags;
		unsigned char DepthCounter;
		PXBool Recursion;
	}
	PXDirectorySearchInfo;

	PXPublic PXActionResult PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize);
	PXPublic PXActionResult PXDirectoryFilesInFolderW(const PXDirectorySearchInfo* const pxDirectorySearchInfo);


#ifdef __cplusplus
}
#endif

#endif