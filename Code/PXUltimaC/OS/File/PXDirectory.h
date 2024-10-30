#ifndef PXDirectoryINCLUDE
#define PXDirectoryINCLUDE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>
#include <Container/List/PXList.h>

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

    PXInt8U EntryDepthCurrent;
    PXInt8U Recursive;
    PXBool UseDotFolders;
}
PXDirectoryIterator;

// Container to store the search result from a directorySearch
typedef struct PXDirectorySearchCache_
{
    //PXFileElementDetected Callback; // Notify owner of a detected 

    PXInt8U SearchDepthCurrent;
    PXInt8U SearchDepthMax;

    PXListDynamic FilePathCache;

    PXList EntryList; // PXFileEntry0

#if OSUnix
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
PXPublic  PXActionResult PXAPI PXDirectorySearch(PXDirectorySearchCache* const pxDirectorySearchCache, const PXText* const directoryName);

PXPublic PXActionResult PXAPI PXDirectoryOpen(PXDirectorySearchCache* const pxDirectorySearchCache, const PXText* const directoryName);
PXPublic PXBool PXAPI PXDirectoryNext(PXDirectorySearchCache* const pxDirectorySearchCache, PXFileEntry* pxFileEntry);
PXPublic PXBool PXAPI PXDirectoryClose(PXDirectorySearchCache* const pxDirectorySearchCache);



PXPublic PXActionResult PXAPI PXDirectoryCreate(const PXText* const directoryName);
PXPublic PXActionResult PXAPI PXWorkingDirectoryGet(PXText* const workingDirectory);
PXPublic PXActionResult PXAPI PXWorkingDirectoryChange(const PXText* const directoryName);

PXPublic PXActionResult PXAPI PXDirectoryDelete(const PXText* const directoryName);

PXPublic PXActionResult PXAPI PXDirectoryFilesInFolderA(const char* folderPath, wchar_t*** list, PXSize* listSize);
PXPublic PXActionResult PXAPI PXDirectoryFilesInFolderW();

PXPublic PXActionResult PXAPI PXDirectorySpecialFolderGet(const PXDirectioySpecialFolder pxDirectioySpecialFolder, PXText* const pxTextSpecialFolder, PXText* const pxTextFileName, const PXBool create);

#endif