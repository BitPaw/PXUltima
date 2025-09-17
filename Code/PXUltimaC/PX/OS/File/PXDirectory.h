#ifndef PXDirectoryIncluded
#define PXDirectoryIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Container/List/PXList.h>

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

} PXDirectioySpecialFolder;




//typedef void (PXAPI* PXFileElementDetected)(PXFileElementInfo* pxFileElementInfo);

typedef struct PXDirectoryIterator_
{
    // PXFileElementInfo EntryCurrent;

#if OSUnix
    DIR* ID;
    struct dirent* DirectoryEntryCurrent;
#elif OSWindows
    HANDLE WindowsDirectoryHandleID;
    WIN32_FIND_DATA WindowsDirectoryEntryCurrent;
#endif

    PXI8U EntryDepthCurrent;
    PXI8U Recursive;
    PXBool UseDotFolders;
}
PXDirectoryIterator;

// Container to store the search result from a directorySearch
typedef struct PXDirectorySearchCache_
{
    //PXFileElementDetected Callback; // Notify owner of a detected

    PXI8U SearchDepthCurrent;
    PXI8U SearchDepthMax;

    PXListDynamic FilePathCache; // Store names
    PXList EntryList; // STore "PXFileEntry" objects

#if OSUnix
    DIR* DirectoryHandleCurrent;
#elif OSWindows
    HANDLE DirectoryHandleCurrent;
#endif



    wchar_t* FolderPath;
    PXSize FolderPathSize;

    wchar_t* FileFilter;
    PXSize FileFilterSize;

    unsigned int Flags;
    unsigned char DepthCounter;
    PXBool Recursion;
}
PXDirectorySearchCache;

PXPrivate void PXAPI PXDirectoryEntryStore(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry);
PXPublic  PXResult PXAPI  PXDirectorySearch(PXDirectorySearchCache* const pxDirectorySearchCache, const PXText* const directoryName);

PXPublic PXResult PXAPI PXDirectoryOpenA(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry, const char* const directoryName);
PXPublic PXResult PXAPI PXDirectoryOpen(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* const pxFileEntry, const PXText* const directoryName);
PXPublic PXBool PXAPI PXDirectoryNext(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* pxFileEntry);
PXPublic PXBool PXAPI PXDirectoryClose(PXDirectorySearchCache* const pxDirectorySearchCache);



PXPublic PXResult PXAPI PXDirectoryCreate(const PXText* const directoryName);
PXPublic PXResult PXAPI PXDirectoryCurrentA(char* const directoryCurrent, const PXSize bufferSize, PXSize* const sizeWritten);
PXPublic PXResult PXAPI PXDirectoryCurrentW(wchar_t* const directoryCurrent, const PXSize bufferSize, PXSize* const sizeWritten);
PXPublic PXResult PXAPI PXDirectoryCurrentGet(PXText* const workingDirectory);
PXPublic PXResult PXAPI PXDirectoryCurrentChange(const PXText* const directoryName);


PXPublic PXResult PXAPI PXDirectoryDelete(const PXText* const directoryName);
PXPublic PXResult PXAPI PXDirectoryDeleteA(const char* const directoryName);
PXPublic PXResult PXAPI PXDirectoryDeleteW(const wchar_t* const directoryName);

PXPublic PXResult PXAPI PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize);
PXPublic PXResult PXAPI PXDirectoryFilesInFolderW();

PXPublic PXResult PXAPI PXDirectorySpecialFolderGet(const PXDirectioySpecialFolder pxDirectioySpecialFolder, PXText* const pxTextSpecialFolder, PXText* const pxTextFileName, const PXBool create);

#endif
