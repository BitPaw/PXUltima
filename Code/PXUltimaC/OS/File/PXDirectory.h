#ifndef PXDirectoryINCLUDE
#define PXDirectoryINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#define PXDirectorySearchForDirectorys (1 << 0)
#define PXDirectorySearchForFiles (1 << 1)
#define PXDirectorySearchForDirectoryAndFile PXDirectorySearchForDirectorys | PXDirectorySearchForFiles

#if OSUnix
#include <sys/types.h>
#include <dirent.h>
#endif


typedef enum PXDirectioySpecialFolder_
{
	PXDirectioySpecialFolder_ADMINTOOLS,
	PXDirectioySpecialFolderAPPDATA,
	PXDirectioySpecialFolderCOMMON_ADMINTOOLS,
	PXDirectioySpecialFolderCOMMON_APPDATA,
	PXDirectioySpecialFolderCOMMON_DOCUMENTS,
	PXDirectioySpecialFolderCOOKIES,
	PXDirectioySpecialFolderHISTORY,
	PXDirectioySpecialFolderINTERNET_CACHE,
	PXDirectioySpecialFolderLOCAL_APPDATA,
	PXDirectioySpecialFolderPictures,
	PXDirectioySpecialFolderPERSONAL,
	PXDirectioySpecialFolderPROGRAM_FILES,
	PXDirectioySpecialFolderPROGRAM_FILES_COMMON,
	PXDirectioySpecialFolderSystem,
	PXDirectioySpecialFolderWinfows

}PXDirectioySpecialFolder;


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

	char FullPath[PXPathSizeMax];
	PXSize FullPathSize;
	PXSize FullPathOffset;

	char* Name;
	PXSize NameSize;

	PXSize Size;

	PXBool IsSystemDottedFolder;
}
PXFileElementInfo;

typedef void (PXAPI* PXFileElementDetected)(PXFileElementInfo* pxFileElementInfo);

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
	PXInt8U Recursive;


	PXFileElementInfo EntryCurrent;
}
PXDirectoryIterator;

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

PXPublic PXActionResult PXAPI PXDirectoryOpen(PXDirectoryIterator* const pxDirectoryIterator, const PXText* const directoryName);
PXPublic PXBool PXAPI PXDirectoryNext(PXDirectoryIterator* const pxDirectoryIterator);
PXPublic PXBool PXAPI PXDirectoryClose(PXDirectoryIterator* const pxDirectoryIterator);

PXPublic PXActionResult PXAPI PXDirectoryCreate(const PXText* const directoryName);
PXPublic PXActionResult PXAPI PXWorkingDirectoryGet(PXText* const workingDirectory);
PXPublic PXActionResult PXAPI PXWorkingDirectoryChange(const PXText* const directoryName);

PXPublic PXActionResult PXAPI PXDirectoryDelete(const PXText* const directoryName);

PXPublic PXActionResult PXAPI PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize);
PXPublic PXActionResult PXAPI PXDirectoryFilesInFolderW(const PXDirectorySearchInfo* const pxDirectorySearchInfo);

PXPublic PXActionResult PXAPI PXDirectorySpecialFolderGet(const PXDirectioySpecialFolder pxDirectioySpecialFolder, PXText* const pxTextSpecialFolder, PXText* const pxTextFileName, const PXBool create);

#endif