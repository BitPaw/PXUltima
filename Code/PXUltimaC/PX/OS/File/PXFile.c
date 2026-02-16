#include "PXFile.h"

#include <PX/Media/PXText.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>


//---------------------------------------------------------
// STD-LIB
//---------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>
//---------------------------------------------------------

//---------------------------------------------------------
// OS-LIB
//---------------------------------------------------------
#if OSUnix
#include <sys/types.h>
//#include <sys/typed.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#elif OSWindows
#include <windows.h>
#include <direct.h>
#endif
//---------------------------------------------------------






//---------------------------------------------------------
// Settings
//---------------------------------------------------------
#define FileLineBufferSize 2048
#define PXFileMappingAllow 1
#define PXFileDebugOutput 0
//---------------------------------------------------------

#define PXFileCursorPositionInvalid (void*)-1;


typedef struct PXFile_
{
    PXBuffer Buffer;

    PXAccessMode AccessMode;
    PXMemoryCachingMode CachingMode;
    PXFileLocationMode LocationMode;

    //-----------------------------------------------------
    // OS-Register
    //-----------------------------------------------------
#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
    int MappingHandle;
#elif OSWindows
    HANDLE MappingHandle;
#endif

#if OSWindows
    HANDLE FileHandle;
#endif

    FILE* FileID;
    int FileDescriptorID;
    //-----------------------------------------------------

    PXBitFormat BitFormatOfData;
    PXEndian EndiannessOfData;

    // The file path can't always be fetched from the OS.
    // for this we store the name here at creation time.
    PXText FilePath;

    PXTime TimeCreation;  // FILETIME
    PXTime TimeAccessLast;
    PXTime TimeWriteLast;

    // Statistic
    PXSize CounterOperationsRead;
    PXSize CounterOperationsWrite;
}
PXFile;

const char PXFileText[] = "File-PX";
const PXI8U PXFileTextLength = sizeof(PXFileText);
const PXECSRegisterInfoStatic PXFileRegisterInfoStatic =
{
    {sizeof(PXFileText), sizeof(PXFileText), PXFileText, TextFormatASCII},
    sizeof(PXFile),
    __alignof(PXFile),
    PXECSTypeResource,
    PXFileCreate,
    PXFileRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXFileRegisterInfoDynamic;






#if OSUnix
#include <sys/mman.h>

// Those makros seem to be inside the
// file "<linux/hugetlb.h>" but not accessable?
#ifndef MAP_HUGE_SHIFT
#define MAP_HUGE_SHIFT 21 // Why is it 21?
#endif

#ifndef MAP_HUGE_2MB
#define MAP_HUGE_2MB (21 << MAP_HUGE_SHIFT)
#endif

#ifndef MAP_HUGE_1GB
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)
#endif

//#define PXPrintfvs vsnprintf
#elif OSWindows
// _O_RDONLY, _O_RDWR, _O_RDWR
//#define PXPrintfvs vsprintf_s

#include <windows.h>
#include <ntstatus.h>
#include <ntsecapi.h>
#include <Sddl.h>

#endif


#define FileReadMode "rb"
#define FileReadModeW L"rb"

#define FileWriteMode "wb"
#define FileWriteModeW L"wb"

#define FileReadWriteMode "rwb"
#define FileReadWriteModeW L"rwb"


#if OSUnix
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

//#include <sys/io.h>


#define OSFileOpenA fopen
#define OSFileOpenW(string, mode) FileOpenA((const char*)string, (const char*)mode)
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
#include <Psapi.h>

#define OSFileOpenA fopen
#define OSFileOpenW _wfopen

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


const char* PXFileLocationModeToString(const PXFileLocationMode pxFileLocationMode)
{
    switch(pxFileLocationMode)
    {
        case PXFileLocationModeInternal:        return "Internal";
        case PXFileLocationModeExternal:        return "External";
        case PXFileLocationModeMappedVirtual:   return "MappedVirtual";
        case PXFileLocationModeMappedFromDisk:  return "MappedFromDisk";
        case PXFileLocationModeDirectCached:    return "DirectCached";
        case PXFileLocationModeDirectUncached:  return "DirectUncached";
        default:                                return "Invalid";
    }
}

PXResult PXAPI PXFilePathSplitt(const PXText PXREF fullPath, PXFilePathStructure PXREF pxFilePathStructure)
{
    PXClear(PXFilePathStructure, pxFilePathStructure);

    switch(fullPath->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            const PXSize indexFirstDot = PXTextFindLastCharacter(fullPath, '.');
            const PXBool hasDot = -1 != indexFirstDot;

            const PXSize indexFirstSlash = PXTextFindFirstCharacter(fullPath, '/');
            const PXSize indexFirstSlashBack = PXTextFindFirstCharacter(fullPath, '\\');
            const PXSize indexLastSlash = PXTextFindLastCharacter(fullPath, '/');
            const PXSize indexLastSlashBack = PXTextFindLastCharacter(fullPath, '\\');

            const PXSize endDrive = PXMathMinimumIU(indexFirstSlash, indexFirstSlashBack);
            const PXSize endPath = PXMathMinimumIU(indexLastSlash, indexLastSlashBack);
            const PXSize fileEnd = hasDot ? indexFirstDot : fullPath->SizeUsed;

            const PXBool hasDirectory = endDrive != -1 || endPath != -1;

            const PXBool hasFileName = hasDot && (indexFirstDot < fullPath->SizeUsed);
            const PXSize indexFileNameStart = hasDirectory ? endPath + 1 : 0;

            const PXSize lengthExtension = fullPath->SizeUsed - indexFirstDot - 1;

            const PXSize lengthFileName = fullPath->SizeUsed - (endPath + 1) - (lengthExtension + 1);

            if(endDrive != -1)
            {
                pxFilePathStructure->Drive.SizeAllocated = endDrive + 1;
                pxFilePathStructure->Drive.SizeUsed = endDrive+1;
                pxFilePathStructure->Drive.A = fullPath->A;
                pxFilePathStructure->Drive.Format = fullPath->Format;
            }

            if(indexFirstDot)
            {
                pxFilePathStructure->Extension.SizeAllocated = lengthExtension;
                pxFilePathStructure->Extension.SizeUsed = lengthExtension;
                pxFilePathStructure->Extension.A = fullPath->A + indexFirstDot + 1;
                pxFilePathStructure->Extension.Format = fullPath->Format;
            }

            // File
            if(hasFileName)
            {
                pxFilePathStructure->FileName.SizeAllocated = lengthFileName;
                pxFilePathStructure->FileName.SizeUsed = lengthFileName;
                pxFilePathStructure->FileName.A = fullPath->A + indexFileNameStart;
                pxFilePathStructure->FileName.Format = fullPath->Format;
            }

            if(hasDirectory)
            {
                pxFilePathStructure->Directory.SizeAllocated = endPath + 1;
                pxFilePathStructure->Directory.SizeUsed = pxFilePathStructure->Directory.SizeAllocated- pxFilePathStructure->Drive.SizeUsed;
                pxFilePathStructure->Directory.A = fullPath->A+ pxFilePathStructure->Drive.SizeUsed;
                pxFilePathStructure->Directory.Format = fullPath->Format;
            }


#if PXLogEnable
            {
                char bufferFullPath[260];

                char bufferDrive[260];
                char bufferFolder[260];
                char bufferFile[260];
                char bufferExtension[260];

                PXClearList(char, bufferDrive, 260);
                PXClearList(char, bufferFolder, 260);
                PXClearList(char, bufferFile, 260);
                PXClearList(char, bufferExtension, 260);

                PXTextCopyA(fullPath->A, fullPath->SizeUsed, bufferFullPath, 260);

                PXTextCopyA(pxFilePathStructure->Drive.A, pxFilePathStructure->Drive.SizeUsed, bufferDrive, 260);
                PXTextCopyA(pxFilePathStructure->Directory.A, pxFilePathStructure->Directory.SizeUsed, bufferFolder, 260);
                PXTextCopyA(pxFilePathStructure->FileName.A, pxFilePathStructure->FileName.SizeUsed, bufferFile, 260);
                PXTextCopyA(pxFilePathStructure->Extension.A, pxFilePathStructure->Extension.SizeUsed, bufferExtension, 260);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "File",
                    "Path-Split",
                    "%15s : x%-2i %s\n"
                    "%15s : x%-2i %s\n"
                    "%15s : x%-2i %s\n"
                    "%15s : x%-2i %s\n"
                    "%15s : x%-2i %s",
                    "Path", fullPath->SizeUsed, bufferFullPath,
                    "Drive", pxFilePathStructure->Drive.SizeUsed, bufferDrive,
                    "Folder", pxFilePathStructure->Directory.SizeUsed, bufferFolder,
                    "File", pxFilePathStructure->FileName.SizeUsed, bufferFile,
                    "Extension", pxFilePathStructure->Extension.SizeUsed, bufferExtension
                );

            }


#endif // 1

            return PXResultOK;










#if OSUnix

#if 0
            char directoryNameCache[PXPathSizeMax];
            char baseNameCache[FileNameMaxSize];

            PXTextCopyA(fullPath, FileNameMaxSize, directoryNameCache, FileNameMaxSize);
            PXTextCopyA(fullPath, FileNameMaxSize, baseNameCache, FileNameMaxSize);

            char* dirNameResult = dirname(directoryNameCache);
            char* baseNameResult = basename(baseNameCache);

            PXSize directoryLength = PXTextCopyA(dirNameResult, DirectoryMaxSize, directory, DirectoryMaxSize);
            PXSize fileNameLength = PXTextCopyA(baseNameResult, FileNameMaxSize, fileName, FileNameMaxSize);

            for(PXSize i = fileNameLength - 1; i > 0; --i)
            {
                const PXBool isDot = fileName[i] == '.';

                if(isDot)
                {
                    PXTextCopyA(fileName + i + 1, ExtensionMaxSize - i, extension, extensionMaxSize);
                    break;
                }
            }

#endif // 0

#elif OSWindows











#if 0
#if 0 // TODO
            _splitpath_s
            (
                fullPath->A,
                drive->A, drive->SizeAllocated,
                directory->A, directory->SizeAllocated,
                fileName->A, fileName->SizeAllocated,
                extension->A, extension->SizeAllocated
            );
#endif

            for(PXSize i = 0; fileName->A[i] != '\0'; i++)
            {
                const PXBool isDot = fileName->A[i] == '.';

                if(isDot)
                {
                    PXTextCopyA(extension->A + i, extension->SizeAllocated, fileNameCache, FileNameMaxSize);
                    PXTextCopyA(fileNameCache, FileNameMaxSize, extension->A, extension->SizeAllocated);
                    break;
                }
            }
#endif
#endif

            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix && 0
            char fullPathA[PXPathSizeMax];
            char driveA[DriveMaxSize];
            char directoryA[DirectoryMaxSize];
            char fileNameA[FileNameMaxSize];
            char extensionA[ExtensionMaxSize];

            PXTextCopyWA(fullPath, PXPathSizeMax, fullPathA, PXPathSizeMax);

            /* ????
            PXFilePathSplitt
            (
                fullPathA, PXPathSizeMax,
                driveA, DriveMaxSize,
                directoryA, DirectoryMaxSize,
                fileNameA, FileNameMaxSize,
                extensionA, ExtensionMaxSize
            );*/

            PXTextCopyAW(driveA, DriveMaxSize, drive, DriveMaxSize);
            PXTextCopyAW(directoryA, DirectoryMaxSize, directory, DirectoryMaxSize);
            PXTextCopyAW(fileNameA, FileNameMaxSize, fileName, FileNameMaxSize);
            PXTextCopyAW(extensionA, ExtensionMaxSize, extension, ExtensionMaxSize);
#elif OSWindows && 0
            wchar_t extensionCache[FileNameMaxSize];

#if 0 // TODO
            _wsplitpath_s
            (
                fullPath,
                drive->W, drive->SizeAllocated,
                directory->W, directory->SizeAllocated,
                fileName->W, fileName->SizeAllocated,
                extension->W, extension->SizeAllocated
            );
#endif

            for(PXSize i = 0; extension->W[i] != '\0'; i++)
            {
                const unsigned char isDot = extension->W[i] == '.';

                if(isDot)
                {
                    PXTextCopyW(extension + i + 1, extension->SizeAllocated, extensionCache, FileNameMaxSize);
                    PXTextCopyW(extensionCache, FileNameMaxSize, extension, extension->SizeAllocated);
                    break;
                }
            }
#endif

            break;
        }
    }
}

PXResult PXAPI PXFilePathCombine(PXText PXREF fullPath, PXFilePathStructure PXREF pxFilePathStructure)
{
    PXAppend(fullPath, &pxFilePathStructure->Drive);
    PXAppend(fullPath, &pxFilePathStructure->Directory);
    PXAppend(fullPath, &pxFilePathStructure->FileName);
    PXAppendA(fullPath, ".", 1);
    PXAppend(fullPath, &pxFilePathStructure->Extension);

    fullPath->A[fullPath->SizeUsed] = '\0';
    return PXResultOK;
}

PXBool PXAPI PXFilePathExtensionCompare(const PXText PXREF filePath, PXText PXREF extension)
{
    switch(filePath->Format)
    {
        case TextFormatASCII:
        {
            return PXTextCompareA(&filePath->A[filePath->SizeUsed - extension->SizeUsed], extension->SizeUsed, extension->A, extension->SizeUsed, 0);
        }
        case TextFormatUNICODE:
        {
            //PXTextCompareA(&pxFileEntry.FilePathData[pxFileEntry.FilePathSize - 4], 4, ".dll", 4, 0);
            break;
        }
        default:
            break;
    }

    return PXFalse;
}

PXSize PXAPI PXFilePathExtensionGet(const PXText PXREF filePath, PXText PXREF extension)
{
    PXText stringResult;
    PXText pxTarget;
    PXTextMakeFixedC(&pxTarget, '.');

    const PXSize index = PXTextFindLast(filePath, &pxTarget, &stringResult);
    const PXBool hasExtension = index != PXTextIndexNotFound;

    if(!hasExtension)
    {
        PXTextClear(extension);
        return 0;
    }

    PXTextMoveByOffset(&stringResult, 1u);

    const PXSize writtenBytes = PXTextCopy(extension, &stringResult);

    return writtenBytes;
}

void PXAPI PXTypeEntryInfo(PXTypeEntry PXREF pxFileDataElementType, PXText PXREF dataType, PXText PXREF dataContent)
{
    PXTextPrint(dataContent, "???");


    const PXSize size = PXTypeSizeMask & pxFileDataElementType->Type;
    PXSize textSize = size;
    char* textTypeExtra = "";
    char* textType = 0;  

    switch(PXTypeTypeMask & pxFileDataElementType->Type)
    {
        case PXTypeBaseNumeric:
        {
            textType = "int";

            textSize *= 8; // Instead of byte size, use bit size

            switch(size)
            {
                case 1:
                {
                    PXI32U number = *(PXI8U*)pxFileDataElementType->Adress;
                    PXTextPrint(dataContent, "%i", number);
                    break;
                }
                case 2:
                {
                    PXI32U number = *(PXI16U*)pxFileDataElementType->Adress;
                    PXTextPrint(dataContent, "%i", number);
                    break;
                }
                case 4:
                {
                    PXI32U number = *(PXI32U*)pxFileDataElementType->Adress;
                    PXTextPrint(dataContent, "%i", number);
                    break;
                }
                case 8:
                {
                    PXI64U number = *(PXI64U*)pxFileDataElementType->Adress;
                    PXTextPrint(dataContent, "%li", number);
                    break;
                }

                default:
                {
                    PXTextPrint(dataContent, "ERR");
                    break;
                }
            }

            if(size > 0)
            {
                switch(PXTypeEndianMask & pxFileDataElementType->Type)
                {
                    case PXTypeEndianBig:
                    {
                        textTypeExtra = "BE";
                        break;
                    }
                    case PXTypeEndianLittle:
                    {
                        textTypeExtra = "LE";
                        break;
                    }
                }
            }

            break;
        }
        case PXTypeBaseDecimal:
        {
            textSize *= 8; // Instead of byte size, use bit size

            switch(size)
            {
                case 4:
                {
                    PXF32 value = *(PXF32*)pxFileDataElementType->Adress;

                    textType = "F-32-";
                    PXTextPrint(dataContent, "%f", value);

                    break;
                }
                case 8:
                {
                    PXF64 value = *(PXF64*)pxFileDataElementType->Adress;

                    textType = "F-64-";
                    PXTextPrint(dataContent, "%lf", value);

                    break;
                }
                default:
                {
                    textType = "???";
                    break;
                }
            }

            break;
        }
        case PXTypeBaseText:
        {
            textType = "text";

            dataContent->SizeUsed = PXTextFromNonTerminated
            (
                dataContent->A,
                dataContent->SizeAllocated,
                (char*)pxFileDataElementType->Adress,
                size+1           
            );

            break;
        }
        case PXTypeBaseBit:
        {
            textType = "bit";

            dataContent->SizeUsed = PXTextFromIntToBinary
            (
                dataContent->A, 
                dataContent->SizeAllocated,
                pxFileDataElementType->Adress,
                size
            );

            break;
        }
        default:
        {
            textType = "???";
            break;
        }
    }

    PXTextPrint(dataType, "%s%i%s", textType, size, textTypeExtra);
}

PXResult PXAPI PXFileRegisterToECS()
{
    PXECSRegister(&PXFileRegisterInfoStatic, &PXFileRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXFileCreate(PXFile** pxFileREF, PXFileCreateInfo PXREF pxFileCreateInfo)
{
    PXFile* pxFile = PXNull;
    PXResult pxResult;

    if(!(pxFileREF && pxFileCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(!*pxFileREF)
    {
        pxFileCreateInfo->Info.Static = &PXFileRegisterInfoStatic;
        pxFileCreateInfo->Info.Dynamic = &PXFileRegisterInfoDynamic;
        pxResult = PXECSCreate(pxFileREF, pxFileCreateInfo);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }   
    }
    pxFile = *pxFileREF;
    pxFile->EndiannessOfData = EndianCurrentSystem;
    pxFile->BitFormatOfData = PXBitFormat64;
    pxFile->AccessMode = pxFileCreateInfo->AccessMode;
    pxFile->CachingMode = pxFileCreateInfo->MemoryCachingMode;

    // Update stuff that might be invalid
    {
        if(PXAccessModeNoAccess == pxFileCreateInfo->AccessMode)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXFileText,
                "Open",
                "File info does not have any permission!? %s\n",
                pxFileCreateInfo->FilePath.A
            );
#endif

            return PXResultRefusedParameterInvalid;
        }
    }

    PXFileFunction pxFileFunction = PXNull;

    switch(PXFileIOInfoFileMask & pxFileCreateInfo->FlagList)
    {
        case PXFileIOInfoFilePhysical:
        {
            pxFileFunction = PXFilePhysical;
            break;
        }
        case PXFileIOInfoFileVirtual:
        {
            pxFileFunction = PXFileVirtual;
            break;
        }
        case PXFileIOInfoFileTemp:
        {
            pxFileFunction = PXFileTemp;
            break;
        }
        case PXFileIOInfoFileMemory:
        {
            pxFileFunction = PXFileMemory;
            break;
        }
        default:
            return PXResultInvalid;
    }

    pxResult = pxFileFunction(pxFile, pxFileCreateInfo);

    return pxResult;
}

PXResult PXAPI PXFilePhysical(PXFile PXREF pxFile, PXFileCreateInfo PXREF pxFileCreateInfo)
{
#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
    const char* readMode = 0u;

    switch(pxFileIOInfo->AccessMode)
    {
        case PXAccessModeReadOnly:
            readMode = "rb";
            break;

        case PXAccessModeWriteOnly:
            readMode = "wb";
            break;
        default:
            return PXFalse;
    }

    // Use this somewhere here
    // int posix_fadvise(int fd, off_t offset, off_t len, int advice);
    // int posix_fadvise64(int fd, off_t offset, off_t len, int advice);

#if CVersionNewerThen2011 && OSWindows
    const errno_t openResultID = fopen_s(&pxFile->FileID, pxText.A, readMode);
    const PXResult openResult = PXErrorCodeFromID(openResultID);
#else
            //open64();
    pxFile->FileID = fopen(pxText.A, readMode);
    const PXResult openResult = PXErrorCurrent(PXNull != pxFile->FileID);
#endif

    if(PXResultOK != openResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "File",
            "Open",
            "Failed to open <%s>",
            pxFileIOInfo->FilePathAdress
        );
#endif

        return openResult;
    }

    pxFile->LocationMode = PXFileLocationModeDirectUncached;

    // Get data from a file
    struct stat fileInfo;

    // obtain the "int" file descriptor from "FILE*"
    pxFile->FileDescriptorID = fileno(pxFile->FileID); // [POSIX]

#if OSWindows
    // If we want to map later, we need a HANDLE, not a FILE*
    pxFile->FileHandle = (HANDLE)_get_osfhandle(pxFile->FileDescriptorID);
#endif

    const int resultID = fstat(pxFile->FileDescriptorID, &fileInfo); // [POSIX]

    // const PXSize fileLength = lseek64(pxFile->FileIDMapping, 0, SEEK_END);

     // Convert
    pxFile->Buffer.SizeAllowedToUse = fileInfo.st_size;

#if OSUnix
    // pxFile->Buffer.DataAllocated = fileInfo.st_blksize * fileInfo.st_blocks; // Might be wrong
    pxFile->Buffer.DataAllocated = pxFile->Buffer.SizeAllowedToUse;
#elif OSWindows
    // Size on harddrive not contained under windows?
    pxFile->Buffer.DataAllocated = fileInfo.st_size;
#endif

    PXTimeFromOSCTime(&pxFile->TimeAccessLast, fileInfo.st_atime);
    PXTimeFromOSCTime(&pxFile->TimeWriteLast, fileInfo.st_mtime);
    PXTimeFromOSCTime(&pxFile->TimeCreation, fileInfo.st_ctime);

#elif OSWindows
    DWORD desiredAccess = 0;
    DWORD shareMode = 0;
    DWORD creationDisposition = 0;
    DWORD flagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE templateFile = PXNull;

    switch(pxFileCreateInfo->AccessMode)
    {
        case PXAccessModeReadOnly:
        {
            shareMode = FILE_SHARE_READ;
            creationDisposition = OPEN_EXISTING;
            desiredAccess = GENERIC_READ;
            break;
        }
        case PXAccessModeWriteOnly:
        {
            creationDisposition = CREATE_ALWAYS;
            desiredAccess = GENERIC_READ | GENERIC_WRITE;
            break;
        }
        case PXAccessModeReadAndWrite:
        {
            creationDisposition = CREATE_ALWAYS;
            desiredAccess = GENERIC_READ | GENERIC_WRITE;
            break;
        }
    }

    //creationDisposition |= PXFileMemoryCachingModeConvertToID(pxFileOpenFromPathInfo->MemoryCachingMode);


    // Create the directory path if needed.
    // Otherwise the file creation will fail because it does not automatically create itself.
    // FilePathExtensionGetW
    if(pxFileCreateInfo->AccessMode == PXAccessModeWriteOnly || pxFileCreateInfo->AccessMode == PXAccessModeReadAndWrite)
    {
        //const PXResult directoryCreateResult = DirectoryCreateA(filePath);

        //PXActionExitOnError(directoryCreateResult);
    }

    // Open file
    {
        HANDLE fileHandle = PXNull;
        SECURITY_ATTRIBUTES* securityAttributes = PXNull;

        switch(pxFileCreateInfo->FilePath.Format)
        {
            case TextFormatASCII:
            case TextFormatUTF8:
            {
                if(PXAccessModeReadOnly == pxFile->AccessMode)
                {
                    const DWORD dwAttrib = GetFileAttributesA(pxFileCreateInfo->FilePath.A); // Windows XP (+UWP), Kernel32.dll, fileapi.h
                    const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
                    const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

                    if(!doesFileExists)
                    {
                        return PXActionFailedFileNotFound;
                    }

                    if(!ifFile)
                    {
                        return PXActionFailedNotAFile;
                    }
                }

                pxFile->FileHandle = CreateFileA // Windows XP, Kernel32.dll, fileapi.h
                (
                    pxFileCreateInfo->FilePath.A,
                    desiredAccess,
                    shareMode,
                    securityAttributes,
                    creationDisposition,
                    flagsAndAttributes,
                    templateFile
                );
                break;
            }
            case TextFormatUNICODE:
            {
                if(PXAccessModeReadOnly == pxFile->AccessMode)
                {
                    const DWORD dwAttrib = GetFileAttributesW(pxFileCreateInfo->FilePath.W); // Windows XP (+UWP), Kernel32.dll, fileapi.h
                    const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
                    const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

                    if(!doesFileExists)
                    {
                        return PXActionFailedFileNotFound;
                    }

                    if(!ifFile)
                    {
                        return PXActionFailedNotAFile;
                    }
                }

                pxFile->FileHandle = CreateFileW // Windows XP, Kernel32.dll, fileapi.h
                (
                    pxFileCreateInfo->FilePath.W,
                    desiredAccess,
                    shareMode,
                    securityAttributes,
                    creationDisposition,
                    flagsAndAttributes,
                    templateFile
                );
                break;
            }
            default:
            {
                return PXActionRefusedInvalidFilePath;
            }
        }

        const PXResult fileOpenResult = PXErrorCurrent(INVALID_HANDLE_VALUE != fileHandle);

        if(PXResultOK != fileOpenResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "File",
                "Open",
                "Failed to open <%s>",
                pxFileCreateInfo->FilePath.A
            );
#endif

            return fileOpenResult;
        }

        pxFile->LocationMode = PXFileLocationModeDirectUncached;

        //int x = _open_osfhandle(pxFile->FileID, _O_APPEND);
        //FILE* fp = _fdopen(x, "rb");
        //pxFile->FileIDPOSIX = fp;
    }


    if(PXAccessModeReadOnly == pxFile->AccessMode)
    {
        // Get file size
        {
            LARGE_INTEGER largeInt;

            const BOOL sizeResult = GetFileSizeEx(pxFile->FileHandle, &largeInt); // Windows XP, Kernel32.dll, fileapi.h

            pxFile->Buffer.SizeAllocated = largeInt.QuadPart;
            pxFile->Buffer.SizeAllowedToUse = largeInt.QuadPart;
        }

        // Get date
        {
            FILETIME creationTime;
            FILETIME lastAccessTime;
            FILETIME lastWriteTime;

            const BOOL fileTimeResult = GetFileTime
            (
                pxFile->FileHandle,
                &creationTime,
                &lastAccessTime,
                &lastWriteTime
            );

            PXTimeFromOSFileTime(&pxFile->TimeCreation, &creationTime);
            PXTimeFromOSFileTime(&pxFile->TimeAccessLast, &lastAccessTime);
            PXTimeFromOSFileTime(&pxFile->TimeWriteLast, &lastWriteTime);
        }

        // TODO: is this better?
        //LPBY_HANDLE_FILE_INFORMATION fileInfo;
        //GetFileInformationByHandle();
    }
    else
    {
        LARGE_INTEGER size;
        size.QuadPart = pxFileCreateInfo->FileSizeRequest;
#if 0

        FILE_ALLOCATION_INFO allocInfo;
        allocInfo.AllocationSize = size;

        const BOOL isOK = SetFileInformationByHandle
        (
            pxFile->FileHandle,
            FileAllocationInfo,
            &allocInfo,
            sizeof(allocInfo)
        );
        PXResult pxResult = PXErrorCurrent(isOK);
#else

        SetFilePointerEx(pxFile->FileHandle, size, NULL, FILE_BEGIN);
        SetEndOfFile(pxFile->FileHandle);

        pxFile->Buffer.SizeAllocated = pxFileCreateInfo->FileSizeRequest;
        pxFile->Buffer.SizeAllowedToUse = pxFileCreateInfo->FileSizeRequest;

#endif


    }


    //PXFilePath(pxFile, &pxText, PXTrue);
#endif




#if PXLogEnable
    PXText pxTextSize;
    PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
    PXTextFormatSize(&pxTextSize, pxFile->Buffer.SizeAllowedToUse);

    PXText pxTextTimeA;
    PXTextConstructNamedBufferA(&pxTextTimeA, pxTextTimeABuffer, 32);
    PXTextFormatDateTime(&pxTextTimeA, &pxFile->TimeCreation);

    PXText pxTextTimeB;
    PXTextConstructNamedBufferA(&pxTextTimeB, pxTextTimeBBuffer, 32);
    PXTextFormatDateTime(&pxTextTimeB, &pxFile->TimeAccessLast);

    PXText pxTextTimeC;
    PXTextConstructNamedBufferA(&pxTextTimeC, pxTextTimeCBuffer, 32);
    PXTextFormatDateTime(&pxTextTimeC, &pxFile->TimeWriteLast);


    PXLogPrint
    (
        PXLoggingInfo,
        PXFileText,
        "Open",
        "OK\n"

        "%20s : %p\n"
        "%20s : %i\n"
#if OSUnix // Mapping
        "%20s : %i\n"
#elif OSWindows
        "%20s : %p\n"
        "%20s : %p\n"
#endif
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s\n"
        "%20s : %s",

        "FILE*", pxFile->FileID,
        "Descriptor", pxFile->FileDescriptorID,
#if OSWindows
        "HANDLE-File", pxFile->FileHandle,
#endif
        "HANDLE-Mapping", pxFile->MappingHandle,
        "Size", pxTextSize.A,
        "Path", pxFileCreateInfo->FilePath.A,
        "Creation", pxTextTimeA.A,
        "AccessLast", pxTextTimeB.A,
        "WriteLast", pxTextTimeC.A
    );
#endif


    PXPerformanceInfo pxPerformanceInfo;
    pxPerformanceInfo.UpdateCounter = 0;
    PXPerformanceInfoGet(&pxPerformanceInfo);

    // Solution A: Load file in memory 1:1
    if(PXAccessModeReadOnly == pxFile->AccessMode) // Do this only when we load
    {
        pxFile->Buffer.Adress = PXMemoryVirtualAllocate
        (
            pxFile->Buffer.SizeAllocated,
            &pxFile->Buffer.SizeAllocated,
            PXAccessModeReadAndWrite
        );

        if(pxFile->Buffer.Adress)
        {
            // SUCCESS
            pxFile->LocationMode = PXFileLocationModeMappedVirtual;

            BOOL ok = ReadFile
            (
                pxFile->FileHandle,
                pxFile->Buffer.Data,
                pxFile->Buffer.SizeAllowedToUse,
                NULL,
                NULL
            );

            // We want to start at the beginning of the file
            pxFile->Buffer.CursorOffsetByte = 0;

            // PXMemoryVirtualPrefetch(pxFile->Buffer.Data, pxFile->Buffer.SizeAllowedToUse);

            PXPerformanceInfoGet(&pxPerformanceInfo);


#if 0
            volatile PXByte sink = 0;

            for(PXSize i = 0; i < pxFile->Buffer.SizeAllowedToUse; i += 4096)
            {
                sink ^= ((PXByte*)pxFile->Buffer.Data)[i]; // Touch one byte per page
            }
#endif

            //  PXPerformanceInfoGet(&pxPerformanceInfo);






              // We NEED to store the reference manually
             // PXFilePathSet(, );

            pxFile->FilePath = pxFileCreateInfo->FilePath;


#if PXLogEnable
            PXI64U bps = pxFile->Buffer.CursorOffsetByte / pxPerformanceInfo.TimeDelta;
            PXText pxTextbps;
            PXTextConstructNamedBufferA(&pxTextbps, pxTextbpsBuffer, 32);
            PXTextFormatSize(&pxTextbps, bps);

            PXLogPrint
            (
                PXLoggingInfo,
                "File",
                "Open",
                "Read with %s/s",
                pxTextbps.A
            );
#endif


            CloseHandle(pxFile->FileHandle);
            pxFile->FileHandle = PXNull;
        }
        else
        {
            // Solution B: Mapping

            // File is now opened.
            // Can we map the whole file into memory?
            const PXBool shallMap = (PXFileIOInfoAllowMapping & pxFileCreateInfo->FlagList) && PXFileMappingAllow;

            if(!shallMap)
            {
                return PXResultOK; // No mapping attempt, we are done
            }

            // Attempt memory mapping...
            PXResult mappingResult = PXFileMapToMemoryEE(pxFile, 0, pxFileCreateInfo->AccessMode, PXTrue);


            PXPerformanceInfoGet(&pxPerformanceInfo);
#if PXLogEnable

            PXI64U bps = pxFile->Buffer.CursorOffsetByte / pxPerformanceInfo.TimeDelta;
            PXText pxTextbps;
            PXTextConstructNamedBufferA(&pxTextbps, pxTextbpsBuffer, 32);
            PXTextFormatSize(&pxTextbps, bps);

            char permissionText[8];

            PXAccessModeToStringA(permissionText, pxFileCreateInfo->AccessMode);

            PXLogPrint
            (
                PXLoggingInfo,
                PXFileText,
                "Mapping",
                "OK, %s -> <%p> with %s/s for <%s>",
                permissionText,
                pxFile->Buffer.Data,
                pxTextbps.A,
                pxFileCreateInfo->FilePath.A
            );
#endif

            // Tell the system we will need this data, so it can prefetch

        }
    }


#if 0
    const int nHandle = _open_osfhandle((intptr_t)pxFile->FileID, osHandleMode);
    FILE* fp = _fdopen(nHandle, fdOpenMode);

#endif
    return PXResultOK;
}

PXResult PXAPI PXFileVirtual(PXFile PXREF pxFile, PXFileCreateInfo PXREF pxFileCreateInfo)
{
    pxFile->Buffer.Adress = PXMemoryVirtualAllocate
    (
        pxFileCreateInfo->FileSizeRequest,
        &pxFile->Buffer.SizeAllocated,
        PXAccessModeReadAndWrite
    );
    pxFile->Buffer.SizeAllowedToUse = pxFile->Buffer.SizeAllocated;
    pxFile->LocationMode = PXFileLocationModeMappedVirtual;

    return PXResultOK;
}

PXResult PXAPI PXFileTemp(PXFile PXREF pxFile, PXFileCreateInfo PXREF pxFileCreateInfo)
{
    // TODO: fix?
#if OSUnix
#elif OSWindows

    PXText tempFileFullPath;
    PXTextConstructNamedBufferW(&tempFileFullPath, tempFileFullPathBuffer, MAX_PATH);

    {
        PXText tempPath;
        PXTextConstructNamedBufferW(&tempPath, tempPathBuffer, MAX_PATH);

        // Gets the temp path env string (no guarantee it's a valid path).
        tempPath.SizeUsed = GetTempPathW(tempPath.SizeAllocated / 2, tempPath.W); // Windows XP (+UWP), Kernel32.dll, fileapi.h

        const PXBool successfulTempPathFetch = tempPath.SizeUsed > 0;


        // Generates a temporary file name.
        tempFileFullPath.SizeUsed = GetTempFileNameW
        (
            tempPath.W, // directory for tmp files
            L"PXUltima",     // temp file name prefix
            0,                // create unique name
            tempFileFullPath.W // buffer for name
        );



        const PXBool successfulTempPathCreate = tempFileFullPath.SizeUsed > 0;
    }

    pxFile->FileHandle = CreateFileW  // Windows XP, Kernel32.dll, fileapi.h
    (
        tempFileFullPath.W, // file name
        GENERIC_ALL,            // open for write
        0,                        // do not share
        NULL,                    // default security
        CREATE_ALWAYS,            // overwrite existing
        FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE,// normal file
        NULL                    // no template
    );


    pxFile->AccessMode = PXAccessModeReadAndWrite;
    pxFile->CachingMode = PXMemoryCachingModeTemporary;
    pxFile->LocationMode = PXFileLocationModeDirectCached;


#else
#endif

    return PXResultOK;
}

PXResult PXAPI PXFileMemory(PXFile PXREF pxFile, PXFileCreateInfo PXREF pxFileCreateInfo)
{
    const PXBool hasSource = pxFileCreateInfo->Data.Data != 0;

    pxFile->AccessMode = pxFileCreateInfo->AccessMode;

    if(hasSource)
    {
        pxFile->Buffer = pxFileCreateInfo->Data;
        pxFile->LocationMode = PXFileLocationModeExternal;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFileText,
            "Open",
            "External <%p> to <%p> %i B",
            pxFile->Buffer.Data,
            pxFile->Buffer.Data + pxFile->Buffer.SizeAllocated - 1,
            pxFile->Buffer.SizeAllocated
        );
#endif
    }
    else
    {
        PXBufferAllocate(&pxFile->Buffer, pxFileCreateInfo->FileSizeRequest);

        pxFile->LocationMode = PXFileLocationModeInternal;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFileText,
            "Open",
            "Created intermal memory <%p> %i B",
            pxFile->Buffer.Data,
            pxFile->Buffer.SizeAllocated
        );
#endif
    }

    pxFile->Buffer.SizeAllowedToUse = pxFile->Buffer.SizeAllocated;

    return PXResultOK;
}

PXResult PXAPI PXFileClose(PXFile PXREF pxFile)
{
    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeMappedFromDisk:
            PXFileUnmapFromMemory(pxFile);
            break;

        case PXFileLocationModeDirectCached:
        case PXFileLocationModeInternal:
            PXBufferRelese(&pxFile->Buffer);
            break;

        case PXFileLocationModeMappedVirtual:
        {
#if 0
            PXMemoryVirtualRelease(pxFile->Buffer.Data, pxFile->Buffer.SizeAllocated);
#endif
            break;
        }
        case PXFileLocationModeExternal:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXFileText,
                "Close",
                "External memory is handled by parrent.."
            );
#endif
            break;
        }
        case PXFileLocationModeDirectUncached:
        {
            CloseHandle(pxFile->FileHandle);
            pxFile->FileHandle = 0;
            break;
        }

    }

#if PXLogEnable
    const char* type = PXFileLocationModeToString(pxFile->LocationMode);

    PXLogPrint
    (
        PXLoggingDeallocation,
        "File",
        "Close",
        "<%s>!",
        type
    );
#endif

    //PXClear(PXFile, pxFile);

    return PXResultOK;
}

PXResult PXAPI PXFileRelease(PXFile PXREF pxFile)
{
   // PXMemoryHeapFree(PXNull, pxFile);

    return PXResultOK;
}

PXBuffer* PXAPI PXFileBufferGET(PXFile PXREF pxFile)
{
    return &pxFile->Buffer;
}

PXBool PXAPI PXFileIsIndexInRegion(const PXFile PXREF pxFile, const PXSize index)
{
    if(!pxFile)
    {
        return 0;
    }

    if(!pxFile->Buffer.Data)
    {
        return 0;
    }

    return pxFile->Buffer.SizeAllowedToUse >= index;
}

PXBool PXAPI PXFileDoesExist(const PXText PXREF pxText)
{
    if(!pxText)
    {
        return PXFalse;
    }

    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix

#elif OSWindows
            const DWORD dwAttrib = GetFileAttributesA(pxText->A); // Windows XP, Kernel32.dll, fileapi.h
            const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
            const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

            if(doesFileExists && ifFile)
            {
                PXLogPrint
                (
                    PXLoggingError,
                    "File",
                    "Exist?",
                    "requested <%s> does not exists\n",
                    pxText->A
                );

                return PXTrue;
            }
#endif

            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix

#elif OSWindows
            const DWORD dwAttrib = GetFileAttributesW(pxText->W); // Windows XP, Kernel32.dll, fileapi.h
            const PXBool doesFileExists = dwAttrib != INVALID_FILE_ATTRIBUTES;
            const PXBool ifFile = !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY);

            if(doesFileExists && ifFile)
            {
                return PXTrue;
            }

#endif

            break;
        }
        default:
        {
            return PXActionRefusedInvalidFilePath;
        }
    }

    return PXFalse;
}

PXBool PXAPI PXFileDoesExistA(const char PXREF filePath)
{
    PXText pxText;
    PXTextFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextLengthUnkown);

    return PXFileDoesExist(&pxText);
}

PXResult PXAPI PXFileRemove(const PXText PXREF filePath)
{
    if(!filePath)
    {
        return PXActionRefusedEmptyFilePath;
    }

    switch(filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix || OSForcePOSIXForWindows
            const int resultID = remove(filePath->A);
            const PXBool success = 0 == resultID;
#elif OSWindows
            const PXBool success = DeleteFileA(filePath->A);
#endif

            const PXResult pxResult = PXErrorCurrent(success);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix

#elif OSWindows

#if OSForcePOSIXForWindows
            const int resultID = _wremove(filePath->W);
            const PXBool success = 0 == resultID;
#else
            const PXBool success = DeleteFileW(filePath->W);
#endif

            const PXResult pxResult = PXErrorCurrent(success);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

#endif

            break;
        }
        default:
        {
            return PXActionRefusedInvalidFilePath;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXFileRename(const PXText PXREF oldName, const PXText PXREF newName)
{
    if(!(oldName && newName))
    {
        return PXActionRefusedEmptyFilePath;
    }

    switch(oldName->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix || OSForcePOSIXForWindows || PXOSWindowsUseUWP
            const int resultID = rename(oldName->A, newName->A); // [POSIX]
            const PXBool success = 0 == resultID;
#elif OSWindows
            const PXBool success = MoveFileA(oldName->A, newName->A); // Windows XP, Kernel32.dll, winbase.h
#endif
            const PXResult pxResult = PXErrorCurrent(success);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            return pxResult;
        }
        case TextFormatUNICODE:
        {
#if OSUnix

#elif OSWindows

#if OSForcePOSIXForWindows || PXOSWindowsUseUWP
            const int resultID = _wrename(oldName->W, newName->W); // [POSIX]
            const PXBool success = 0 == resultID;
#else
            const PXBool success = MoveFileW(oldName->W, newName->W); // Windows XP, Kernel32.dll, winbase.h
#endif

            const PXResult pxResult = PXErrorCurrent(success);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            return pxResult;

#endif
        }
        default:
        {
            return PXActionRefusedInvalidFilePath;
        }
    }
}

PXResult PXAPI PXFileCopy(const PXText PXREF sourceFilePath, const PXText PXREF destinationFilePath, const PXBool overrideIfExists)
{
    if(!sourceFilePath) return PXActionRefuedInputBufferNull;
    if(!destinationFilePath) return PXActionRefuedOutputBufferNull;

    switch(sourceFilePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix || OSForcePOSIXForWindows
            const FILE PXREF fileSource = fopen(sourceFilePath, FileReadMode);
            const FILE PXREF fileDestination = fopen(destinationFilePath, FileWriteMode);
            const PXBool fileOpenSuccesful = fileSource && fileDestination;

            const PXSize swapBufferSize = 1024;
            PXByte swapBuffer[1024];

            if(!fileOpenSuccesful)
            {
                return PXActionFailedOpen;
            }

            while(!feof(fileSource))
            {
                const PXSize readBytes = fread(swapBuffer, sizeof(char), swapBufferSize, fileSource);
                const PXSize writtenBytes = fwrite(swapBuffer, sizeof(char), readBytes, fileDestination);
            }

            const int closeA = fclose(fileSource);
            const int closeB = fclose(fileDestination);
#elif OSWindows
            const PXBool succesfull = CopyFileA(sourceFilePath->A, destinationFilePath->A, overrideIfExists); // Windows XP (+UWP), Kernel32.dll, winbase.h
            const PXResult pxResult = PXErrorCurrent(succesfull);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            return PXResultOK;
#endif

            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix
            // Convert
            return PXActionRefusedNotImplemented;

#elif OSWindows
            const PXBool succesfull = CopyFileW(sourceFilePath->W, destinationFilePath->W, overrideIfExists); // Windows XP (+UWP), Kernel32.dll, winbase.h
            const PXResult pxResult = PXErrorCurrent(succesfull);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            return PXResultOK;
#endif
        }
        default:
        {
            return PXActionRefuedTextFormatUnsupported;
        }
    }
}

/*
void PXFilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
    const PXSize index = PXTextFindLastW(currnetPath, PXPathSizeMax, '/');
    const PXBool found = index != -1;

    if(found)
    {
        const PXSize copyedBytes = PXTextCopyW(currnetPath, index + 1, targetPath, index + 1);
        const PXSize toCopy = PXPathSizeMax - copyedBytes;

        PXTextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
    }
}*/

void PXAPI PXFilePathRelativeFromFileA
(
    const char PXREF pathCurrent, 
    const PXSize patHCurrentSize, 
    const char PXREF pathTarget,
    const PXSize pathTargetSize, 
    char PXREF pathResult,
    PXSize PXREF pathResultSize
)
{



}

PXResult PXAPI PXFilePathRelativeFromFile(const PXFile PXREF pxFile, const PXText PXREF targetPath, PXText PXREF resultPath)
{
    if(targetPath->A[1] == ':')
    {
        // do nothing and keep absolut path

        PXTextCopy(targetPath, resultPath);

        return PXResultOK;
    }

    //---<Get current path>----------------
    PXText currentObjectFilePath;
    PXTextConstructNamedBufferA(&currentObjectFilePath, currentFilePathBuffer, PXPathSizeMax);

    const PXBool success = PXFilePath(pxFile, &currentObjectFilePath, PXFalse); // Work PXWavefront file path
    //-------------------------------

    // When we take the direct folder, we have a shortcut to directs merge paths
    if(targetPath->A[0] == '.')
    {
        PXFilePathStructure pxFilePathStructureBefore;
        PXFilePathStructure pxFilePathStructureAfter;

        // Run a
        {
            const PXResult result = PXFilePathSplitt(&currentObjectFilePath, &pxFilePathStructureBefore);

            if(PXResultOK != result)
            {
                return result;
            }

        }

        // RUn B
        {
            const PXResult result = PXFilePathSplitt(targetPath, &pxFilePathStructureAfter);

            if(PXResultOK != result)
            {
                return result;
            }
        }

        PXAppend(resultPath, &pxFilePathStructureBefore.Drive);
        PXAppend(resultPath, &pxFilePathStructureBefore.Directory);
        PXAppend(resultPath, &pxFilePathStructureAfter.Directory);
        PXAppend(resultPath, &pxFilePathStructureAfter.FileName);
        PXAppendA(resultPath, ".", 1);
        PXAppend(resultPath, &pxFilePathStructureAfter.Extension);
    }
    else
    {
        PXFilePathSwapFileName(&currentObjectFilePath, resultPath, targetPath);
    }

    return PXResultOK;
}

PXResult PXAPI PXFilePathSwapFileName(const PXText PXREF inputPath, PXText PXREF exportPath, const PXText PXREF fileName)
{
    PXFilePathStructure pxFilePathStructureBefore;
    PXFilePathStructure pxFilePathStructureAfter;

    // Run a
    {
        const PXResult result = PXFilePathSplitt(inputPath, &pxFilePathStructureBefore);

        if(PXResultOK != result)
        {
            return result;
        }

    }

    // RUn B
    {
        const PXResult result = PXFilePathSplitt(fileName, &pxFilePathStructureAfter);

        if(PXResultOK != result)
        {
            return result;
        }
    }

    // Override
    {
        PXCopy(PXText, &pxFilePathStructureAfter.FileName, &pxFilePathStructureBefore.FileName);
        PXCopy(PXText, &pxFilePathStructureAfter.Extension, &pxFilePathStructureBefore.Extension);
    }

    PXFilePathCombine(exportPath, &pxFilePathStructureBefore);

    return PXResultOK;
}

PXBool PXAPI PXFileDataAvailable(const PXFile PXREF pxFile)
{
    if(!pxFile)
    {
        return PXFalse;
    }

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeMappedFromDisk:
        {
            return pxFile->Buffer.Data && (pxFile->Buffer.CursorOffsetByte <= pxFile->Buffer.SizeAllocated);
        }
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
            break;

        default:
            return PXFalse;
    }

    return PXTrue;
}

void PXAPI PXFilePathSwapExtension(const PXText PXREF inputPath, PXText PXREF exportPath)
{
    PXText pxResult;
    PXText pxText;

    PXTextMakeFixedC(&pxText, '.');

    const PXSize index = PXTextFindLast(inputPath, &pxText, &pxResult); // Find last dot
    const PXBool found = index != -1;

    if(!found)
    {
        return;
    }

    PXTextMoveByOffset(&pxResult, 1u);

    switch(inputPath->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
            const PXSize written = PXTextCopyA(inputPath->A, index, exportPath->A, PXPathSizeMax); // Copy filePath without extension
            const PXSize writtenFull = PXTextCopyA(pxResult.A, PXPathSizeMax, &exportPath->A[written], PXPathSizeMax); // Copy extension on top

            break;
        }

        case TextFormatUNICODE:
        {
            const PXSize written = PXTextCopyW(inputPath->W, index + 1, exportPath->W, PXPathSizeMax); // Copy filePath without extension
            const PXSize writtenFull = PXTextCopyW(pxResult.W, PXPathSizeMax, &exportPath->W[written], PXPathSizeMax); // Copy extension on top

            break;
        }
    }
}

PXResult PXAPI PXFileName(const PXFile PXREF pxFile, PXText PXREF fileName)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista

    const DWORD flags = FILE_NAME_NORMALIZED | VOLUME_NAME_DOS; // FILE_NAME_NORMALIZED

    const DWORD result = GetFinalPathNameByHandleA(pxFile->FileHandle, fileName->A, fileName->SizeAllocated, flags);

    const char dosDriveTag[] = "\\\\?\\";

    const PXByte isSS = PXTextCompareA(fileName->A, fileName->SizeUsed, dosDriveTag, 4u, 0);

    if(isSS)
    {
        // Move my atleast 4.

        const char wind[] = "C:\\Windows\\System32\\";

        const PXByte isSSEE = PXTextCompareA(fileName->A + 4u, 20u, wind, 20u, 0);


        char* texxxx = fileName->A + 4u;
        PXSize xxxxxy = result - 4u;

        if(isSSEE)
        {
            texxxx = fileName->A + 24u;
            xxxxxy = result - 24u;
        }
        else
        {
            texxxx = fileName->A + 4u;
            xxxxxy = result - 4u;
        }

        const PXSize rerere = PXMemoryMove(texxxx, xxxxxy, fileName, result);


        fileName->SizeUsed = rerere;

        fileName->W[rerere] = '\0';

    }
    else
    {
        fileName->SizeUsed = result;
    }
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

#endif

    return PXResultOK;
}

PXResult PXAPI PXFileNameSet(const PXFile PXREF pxFile, PXText PXREF fileName)
{


    return PXResultOK;
}

PXBool PXAPI PXFileDirectoryPathExtract(const PXText PXREF path, PXText PXREF directoryPath)
{
    PXText stringTarget;
    PXSize size;
    PXBool found;

    {
        PXTextMakeFixedC(&stringTarget, '/');

        size = PXTextFindLast(path, &stringTarget, directoryPath);
        found = size != -1;

        if(found)
        {
            return PXTrue;
        }
    }

    {
        PXTextMakeFixedC(&stringTarget, '\\');

        size = PXTextFindLast(path, &stringTarget, directoryPath);
        found = size != -1;
    }

    return found;
}

PXBool PXAPI PXFileCanDirectAccess(const PXFile PXREF pxFile)
{
    return pxFile->MappingHandle != PXHandleNotSet;
}

PXBool PXAPI PXFileKeyValueFetch(PXFile PXREF pxFile, PXTypeBinding PXREF pxTypeBinding)
{
    return PXResultOK;
}

PXI32U PXAPI PXFileMemoryCachingModeConvertToID(const PXMemoryCachingMode pxMemoryCachingMode)
{
    switch(pxMemoryCachingMode)
    {
        default:
        case PXMemoryCachingModeDefault:
#if OSUnix
            return 0;//POSIX_FADV_NORMAL;
#elif OSWindows
            return 0;
#endif
        case PXMemoryCachingModeRandom:
#if OSUnix
            return 0;//POSIX_FADV_RANDOM;
#elif OSWindows
            return FILE_FLAG_RANDOM_ACCESS;
#endif
        case PXMemoryCachingModeSequential:
#if OSUnix
            return 0;//POSIX_FADV_SEQUENTIAL;
#elif OSWindows
            return FILE_FLAG_SEQUENTIAL_SCAN;
#endif

#if OSWindows

        case PXMemoryCachingModeTemporary:
        case PXMemoryCachingModeUseOnce:
            return FILE_FLAG_DELETE_ON_CLOSE;

            // Windows only
        case PXMemoryCachingModeWriteThrough:
            return FILE_FLAG_WRITE_THROUGH;

        case PXMemoryCachingModeNoBuffering:
            return FILE_FLAG_NO_BUFFERING;
#endif // OSWindows

#if OSUnix
            // UNIX only
        case PXMemoryCachingModeNeedLater:
            return 0;//POSIX_FADV_WILLNEED;

        case PXMemoryCachingModeDontNeedNow:
            return 0;//POSIX_FADV_DONTNEED;
#endif
    }
}


#if OSWindows
//------------------------------
// TODO: remove or change!!
//------------------------------

LSA_HANDLE GetPolicyHandle(WCHAR* SystemName)
{
    LSA_OBJECT_ATTRIBUTES ObjectAttributes;

    LSA_UNICODE_STRING lusSystemName;
    NTSTATUS ntsResult;
    LSA_HANDLE lsahPolicyHandle;

    // Object attributes are reserved, so initialize to zeros.
    PXMemoryClear(&ObjectAttributes, sizeof(ObjectAttributes));


    USHORT SystemNameLength = 0;
    LSA_UNICODE_STRING* systemNameRef = PXNull;


    if(SystemName)
    {
        systemNameRef = &lusSystemName;

        //Initialize an LSA_UNICODE_STRING to the server name.
        SystemNameLength = PXTextLengthW(SystemName, -1);
        lusSystemName.Buffer = SystemName;
        lusSystemName.Length = SystemNameLength * sizeof(WCHAR);
        lusSystemName.MaximumLength = (SystemNameLength + 1) * sizeof(WCHAR);
    }

    // Get a handle to the Policy object.
    ntsResult = LsaOpenPolicy
    (
        systemNameRef,    //Name of the target system.
        &ObjectAttributes, //Object attributes.
        POLICY_ALL_ACCESS, //Desired access permissions.
        &lsahPolicyHandle  //Receives the policy handle.
    );

    if(ntsResult != STATUS_SUCCESS)
    {
        // An error occurred. Display it as a win32 error code.
        auto winError = LsaNtStatusToWinError(ntsResult);
       // wprintf(L"OpenPolicy returned %lu\n", winError);
       // std::cout << "Error message: " << GetErrorAsString(winError) << std::endl;
        return NULL;
    }
    return lsahPolicyHandle;
}

PXBool InitLsaString(PLSA_UNICODE_STRING pLsaString, LPCWSTR pwszString)
{
    DWORD dwLen = 0;

    if(NULL == pLsaString)
        return FALSE;

    if(NULL != pwszString)
    {
        dwLen = PXTextLengthW(pwszString, -1);
        if(dwLen > 0x7ffe)   // String is too large
            return FALSE;
    }

    // Store the string.
    pLsaString->Buffer = (WCHAR*)pwszString;
    pLsaString->Length = (USHORT)dwLen * sizeof(WCHAR);
    pLsaString->MaximumLength = (USHORT)(dwLen + 1) * sizeof(WCHAR);

    return TRUE;
}


void AddPrivileges()
{
    const LSA_HANDLE policyHandle = GetPolicyHandle(PXNull);
    const LPCSTR strSid = "S-1-5-21-2554383953-2022197192-2242278139-1010";
    PSID accountSID = 0;
    // WCHAR SystemName[] = L"DESKTOP-AVV4L7N";

    const BOOL convertResultID = ConvertStringSidToSidA(strSid, &accountSID);




    LSA_UNICODE_STRING lucPrivilege;


    // Create an LSA_UNICODE_STRING for the privilege names.
    const BOOL initSuccess = InitLsaString(&lucPrivilege, L"SeLockMemoryPrivilege");
    const PXResult openTokenResult = PXErrorCurrent(initSuccess);


    if(!initSuccess)
    {
        //wprintf(L"Failed InitLsaString\n");
        return;
    }

    const NTSTATUS ntsResultID = LsaAddAccountRights
    (
        policyHandle,  // An open policy handle.
        accountSID,    // The target SID.
        &lucPrivilege, // The privileges.
        1              // Number of privileges.
    );
    const PXResult ntsResult = PXErrorCurrent(STATUS_SUCCESS == ntsResultID);


    if(ntsResultID == STATUS_SUCCESS)
    {
       // wprintf(L"Privilege added.\n");
    }
    else
    {
       // wprintf(L"Privilege was not added - %lu \n", LsaNtStatusToWinError(ntsResultID));
       // std::cout << "Error message: " << GetErrorAsString(LsaNtStatusToWinError(ntsResult)) << std::endl;
    }
}
#endif

PXResult PXAPI PXFileMapToMemory(PXFile PXREF pxFile, const PXSize size, const PXAccessMode protectionMode)
{
    PXResult pxResult = PXBufferAllocate(&pxFile->Buffer, size);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxFile->AccessMode = protectionMode;
    pxFile->LocationMode = PXFileLocationModeMappedVirtual;

    return PXResultOK;
}

PXResult PXAPI PXFileUnmapFromMemory(PXFile PXREF pxFile)
{
    // Write pending data
    PXBool isWriteMapped = 0;

    switch(pxFile->AccessMode)
    {
        default:
        case PXAccessModeNoAccess:
        case PXAccessModeReadOnly:
            isWriteMapped = PXFalse;
            break;

        case PXAccessModeWriteOnly:
        case PXAccessModeReadAndWrite:
            isWriteMapped = PXTrue;
            break;
    }


#if OSUnix

    // Unmap
    {
        const int unmapResultID = munmap(pxFile->Buffer.Data, pxFile->Buffer.CursorOffsetByte);
        const PXResult unmapResult = PXErrorCurrent(0 == unmapResultID);

        if(PXResultOK != unmapResult)
        {
            return unmapResult;
        }

        pxFile->Buffer.Data = PXNull;
        pxFile->Buffer.CursorOffsetByte = 0;
        pxFile->Buffer.SizeAllowedToUse = 0;
        pxFile->Buffer.DataAllocated = 0;
    }

    // Release
    {
        const int closeResultID = fclose(pxFile->FileID);
        const PXResult closeResult = PXErrorCurrent(0 == closeResultID);


        return closeResult;
    }

#elif OSWindows

    // undo filemapping
    {
#if PXLogEnable

        PXText pxTextFilePath;
        PXTextConstructNamedBufferA(&pxTextFilePath, pxTextFilePathBuffer, PXPathSizeMax);

        PXFilePath(pxFile, &pxTextFilePath, PXFalse);

        PXText pxText;
        PXTextConstructNamedBufferA(&pxText, pxTextBuffer, 260);

        PXTextFormatSize(&pxText, pxFile->Buffer.SizeAllowedToUse);

        PXLogPrint
        (
            PXLoggingDeallocation,
            PXFileText,
            "MMAP-Release",
            "\n"
            "%20s : %i\n"
            "%20s : %s\n"
            "%20s : <%p> %i/%i\n"
            "%20s : <%s>",
            "Mapping-ID", (int)pxFile->MappingHandle,
            "Size", pxText.A,
            "Data", pxFile->Buffer.Data, pxFile->Buffer.CursorOffsetByte, pxFile->Buffer.SizeAllowedToUse,
            "Path", pxTextFilePath.A
        );
#endif

        // Write pending data
        {
            if(isWriteMapped)
            {
                const BOOL flushSuccessful = FlushViewOfFile(pxFile->Buffer.Data, pxFile->Buffer.CursorOffsetByte);
                const PXResult pxResult = PXErrorCurrent(flushSuccessful);

                if(PXResultOK != pxResult)
                {
                    return pxResult;
                }
            }
        }

        {
            const PXBool unmappingSucessful = UnmapViewOfFile(pxFile->Buffer.Data);
            const PXResult pxResult = PXErrorCurrent(unmappingSucessful);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            pxFile->Buffer.Data = PXNull;
        }

        {
            const PXBool closeMappingSucessful = CloseHandle(pxFile->MappingHandle);
            const PXResult pxResult = PXErrorCurrent(closeMappingSucessful);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

            pxFile->MappingHandle = PXHandleNotSet;
        }
    }

    // Mark end of file
    {
        if(isWriteMapped)
        {
            //fseek();

            LARGE_INTEGER largeInteger;

            largeInteger.QuadPart = pxFile->Buffer.CursorOffsetByte;

            const BOOL setSuccessful = SetFilePointerEx(pxFile->FileHandle, largeInteger, 0, FILE_BEGIN);
            const BOOL endSuccessful = SetEndOfFile(pxFile->FileHandle);
        }
    }

    // Close file
    {
        //      _fclose_nolock(pxFile->FileHandleCStyle);

        const PXBool successful = CloseHandle(pxFile->FileHandle); // Windows 2000 (+UWP), Kernel32.dll, handleapi.h
        const PXResult closeResult = PXErrorCurrent(successful);

        if(PXResultOK != closeResult)
        {
            return closeResult;
        }

        pxFile->FileHandle = PXNull;
    }

    // CLEAR ALL
    //PXClear(PXFile, pxFile);

    return PXResultOK;
#endif
}

PXSize PXAPI PXFileAllocatedSize(const PXFile PXREF pxFile)
{
    return pxFile->Buffer.SizeAllocated;
}

PXSize PXAPI PXFileRemainingSize(const PXFile PXREF pxFile)
{
    return pxFile->Buffer.SizeAllowedToUse - pxFile->Buffer.CursorOffsetByte;
}

PXSize PXAPI PXFileRemainingSizeRelativeFromAddress(const PXFile PXREF pxFile, const void PXREF adress)
{
    return ((PXSize)pxFile->Buffer.Data - (PXSize)adress) - pxFile->Buffer.SizeAllowedToUse;
}

PXBool PXAPI PXFileIsAtEnd(const PXFile PXREF pxFile)
{
    return pxFile->Buffer.CursorOffsetByte >= pxFile->Buffer.SizeAllowedToUse;
}

PXBool PXAPI PXFileAssureFreeSize(PXFile PXREF pxFile, const PXSize amount)
{
    const PXSize remainingSize = PXFileRemainingSize(pxFile);
    const PXSize needAlloc = remainingSize < amount;
    PXBool success = PXFalse;

    if(!needAlloc)
    {
        return PXTrue;
    }

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
            break;

        case PXFileLocationModeExternal:
            break;

        case PXFileLocationModeMappedVirtual:
        {
            pxFile->Buffer.SizeAllocated = pxFile->Buffer.SizeAllowedToUse * 2;
            pxFile->Buffer.SizeAllowedToUse = pxFile->Buffer.SizeAllocated;
            pxFile->Buffer.Data = (PXByte*)PXMemoryVirtualReallocate(pxFile->Buffer.Data, pxFile->Buffer.SizeAllowedToUse);

            success = pxFile->Buffer.Data > 0;

            break;
        }

        case PXFileLocationModeMappedFromDisk:
            return PXFalse;

        case PXFileLocationModeDirectUncached:
        {
            success = SetFileValidData
            (
                pxFile->FileHandle,
                amount
            ) != 0;

            pxFile->Buffer.SizeAllocated = amount;

            break;
        }
        case PXFileLocationModeDirectCached:
            break;
    }

    return success;
}

PXSize PXAPI PXFileFindEndOfText(PXFile PXREF pxFile)
{
    const void PXREF startIndex = PXFileDataAtCursor(pxFile);
    const void PXREF targetAdress = PXMemoryLocateFirst(pxFile->Buffer.Data, '\0', pxFile->Buffer.SizeAllowedToUse);
    const PXSize offset = (PXSize)targetAdress - (PXSize)startIndex;

    return offset;
}

void* PXAPI PXFileDataAtCursor(PXFile PXREF pxFile)
{
    if(!pxFile)
    {
        return 0;
    }

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal: // Memory is handled internally.
        case PXFileLocationModeExternal: // Memory is stored outside this object
        case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
        case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
            return (PXAdress)pxFile->Buffer.Data + pxFile->Buffer.CursorOffsetByte;

        case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
        case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
        {
#if OSUnix
            pxFile->Buffer.CursorOffsetByte = ftell(pxFile->FileID);
#elif OSWindows
            // Compile-ERROR: This seems to upset the compiler as its limited to 32-Bit value
            //pxFile->Buffer.CursorOffsetByte = SetFilePointer(pxFile->FileID, 0, 0, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h
            const LARGE_INTEGER distanceToMove = { 0,0 }; // We do not move
            LARGE_INTEGER newFilePointer;

            const PXBool result = SetFilePointerEx(pxFile->FileHandle, distanceToMove, &newFilePointer, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h

            if(result)
            {
                pxFile->Buffer.CursorOffsetByte = newFilePointer.QuadPart;
            }
#endif

        }
        default:
            return PXFileCursorPositionInvalid;
    }
}

void* PXAPI PXFileDataAtCursorWithOffset(PXFile PXREF pxFile, const PXSize offset)
{
    return (PXByte*)PXFileDataAtCursor(pxFile) + offset;
}

PXSize PXAPI PXFileDataPosition(PXFile PXREF pxFile)
{
    return pxFile->Buffer.CursorOffsetByte;
}

PXSize PXAPI PXFileSizeToRead(PXFile PXREF pxFile)
{
    return pxFile->Buffer.SizeAllowedToUse;
}

PXSize PXAPI PXFileOperationsRead(PXFile PXREF pxFile)
{
    return pxFile->CounterOperationsRead;
}

PXSize PXAPI PXFileOperationsWrite(PXFile PXREF pxFile)
{
    return pxFile->CounterOperationsWrite;
}

PXSize PXAPI PXFileCursorOffsetGetBit(PXFile PXREF pxFile)
{
    return pxFile->Buffer.CursorOffsetBit;
}

void PXAPI PXFileCursorOffsetSetBit(PXFile PXREF pxFile, const PXSize offsetBit)
{
    pxFile->Buffer.CursorOffsetBit = offsetBit;
}

void PXAPI PXFileCursorMoveTo(PXFile PXREF pxFile, const PXSize position)
{
    const PXSize minimalInBoundsPosition = PXMathMinimumIU(pxFile->Buffer.SizeAllowedToUse, position); // Prevent cursor from going out of bounce

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal: // Memory is handled internally.
        case PXFileLocationModeExternal: // Memory is stored outside this object
        case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
        case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
        {
            pxFile->Buffer.CursorOffsetBit = 0;
            pxFile->Buffer.CursorOffsetByte = minimalInBoundsPosition;
            break;
        }

        case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
        case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
        {
#if OSUnix
            pxFile->Buffer.CursorOffsetByte = ftell(pxFile->FileID);
#elif OSWindows

#if OS32Bit
            LONG sizeLow = position;
            LONG sizeHigh = 0;
#else OS64Bit

            LONG sizeLow = position & 0x00000000FFFFFFFF;
            LONG sizeHigh = (position & 0xFFFFFFFF00000000) >> 32;
#endif
            pxFile->Buffer.CursorOffsetBit = 0;
            pxFile->Buffer.CursorOffsetByte = SetFilePointer(pxFile->FileHandle, sizeLow, &sizeHigh, FILE_BEGIN); // Windows XP, Kernel32.dll, fileapi.h

#endif
            break;
        }
    }
}

void PXAPI PXFileCursorToBeginning(PXFile PXREF pxFile)
{
#if OSUnix
#elif OSWindows
    const BOOL flushSuccessful = FlushFileBuffers(pxFile->FileHandle); // Windows XP (+UWP), Kernel32.dll, fileapi.h
#endif

    PXFileCursorMoveTo(pxFile, 0);
}

PXSize PXAPI PXFileCursorPeek(PXFile PXREF pxFile, void PXREF data, const PXSize steps)
{
    const PXSize readBytes = PXFileReadB(pxFile, data, steps);

    PXFileCursorRewind(pxFile, readBytes);

    return readBytes;
}

PXSize PXAPI PXFileCursorAdvance(PXFile PXREF pxFile, const PXSize steps)
{
    const PXSize before = pxFile->Buffer.CursorOffsetByte;
    PXFileCursorMoveTo(pxFile, pxFile->Buffer.CursorOffsetByte + steps);
    const PXSize delta = pxFile->Buffer.CursorOffsetByte - before;

    //assert(delta != (PXSize)-1);

    return delta;
}

PXBool PXAPI PXFileCursorIsAt(PXFile PXREF pxFile, const PXSize position)
{
    return pxFile->Buffer.CursorOffsetByte == position;
}

PXSize PXAPI PXFileCursorRewind(PXFile PXREF pxFile, const PXSize steps)
{
    if(pxFile->Buffer.CursorOffsetByte <= steps)
    {
        PXFileCursorMoveTo(pxFile, 0);


        return steps - pxFile->Buffer.CursorOffsetByte;
    }

    const PXSize stepsTaken = pxFile->Buffer.CursorOffsetByte - steps;

    PXFileCursorMoveTo(pxFile, stepsTaken); // Check underflow

    return steps;
}

PXSize PXAPI PXFileCursorOffset(PXFile PXREF pxFile, const PXI32S offset)
{
    if(offset > 0)
    {
        return PXFileCursorAdvance(pxFile, offset);
    }
    else
    {
        PXFileCursorRewind(pxFile, offset * -1);

        return 0; // TODO:
    }
}

void PXAPI PXFileCursorToEnd(PXFile PXREF pxFile)
{
    PXFileCursorMoveTo(pxFile, pxFile->Buffer.SizeAllowedToUse);
}

PXSize PXAPI PXFileSkipEndOfLineCharacters(PXFile PXREF pxFile)
{
    const PXSize oldPosition = pxFile->Buffer.CursorOffsetByte;

    while(!PXFileIsAtEnd(pxFile))
    {
        char character = 0;
        PXFileCursorPeek(pxFile, &character, sizeof(char));
        const PXBool advance = PXTextIsEmpty(character) || PXTextIsTab(character) || PXTextIsEndOfLine(character);

        if(!advance)
        {
            break;
        }

        PXFileCursorAdvance(pxFile, 1u);
    }

    return pxFile->Buffer.CursorOffsetByte - oldPosition;
}

PXSize PXAPI PXFileSkipEmptySpace(PXFile PXREF pxFile)
{
    const PXSize dataPositionBefore = pxFile->Buffer.CursorOffsetByte;

    while(!PXFileIsAtEnd(pxFile))
    {
        char character = 0;
        PXFileCursorPeek(pxFile, &character, sizeof(char));
        const PXBool isEmtpySpace = PXTextIsEmptySpace(character) && !PXTextIsEndOfString(character);

        if(!isEmtpySpace)
        {
            break;
        }

        PXFileCursorAdvance(pxFile, 1u);
    }

    return pxFile->Buffer.CursorOffsetByte - dataPositionBefore;
}

PXSize PXAPI PXFileReadNextLineInto(PXFile PXREF pxFile, void* exportBuffer, const PXSize exportBufferSize)
{
    const PXSize dataPositionBefore = pxFile->Buffer.CursorOffsetByte;

    PXFileSkipEndOfLineCharacters(pxFile);

    while(!PXFileIsAtEnd(pxFile))
    {
        char character = 0;
        PXFileCursorPeek(pxFile, &character, sizeof(char));
        const PXBool advance = !PXTextIsEndOfLine(character) && !PXTextIsEndOfString(character);

        if(!advance)
        {
            break;
        }

        PXFileCursorAdvance(pxFile, 1u);
    }

    const char* dataPoint = (char*)pxFile->Buffer.Data + dataPositionBefore;
    const PXSize dataPositionAfter = pxFile->Buffer.CursorOffsetByte;
    const PXSize length = dataPositionAfter - dataPositionBefore;

    if(length == 0)
    {
        return 0;
    }

    PXTextCopyA(dataPoint, length, (char*)exportBuffer, length);

    PXFileSkipEndOfLineCharacters(pxFile);

    return length;
}

PXSize PXAPI PXFileSkipBlock(PXFile PXREF pxFile)
{
    const PXSize oldPosition = pxFile->Buffer.CursorOffsetByte;

    while(!PXFileIsAtEnd(pxFile))
    {
        char character = 0;
        PXFileCursorPeek(pxFile, &character, sizeof(char));

        const PXBool advance = !PXTextIsEndOfString(character) && !PXTextIsEmptySpace(character) && !PXTextIsEndOfLine(character);

        if(!advance)
        {
            break;
        }

        PXFileCursorAdvance(pxFile, sizeof(char));
    }

    return pxFile->Buffer.CursorOffsetByte - oldPosition;
}

PXSize PXAPI PXFileSkipToNextBlock(PXFile PXREF pxFile)
{
    const PXSize oldPosition = pxFile->Buffer.CursorOffsetByte;

    PXFileSkipBlock(pxFile);

    PXFileSkipEmptySpace(pxFile);

    return pxFile->Buffer.CursorOffsetByte - oldPosition;
}

PXSize PXAPI PXFileSkipLine(PXFile PXREF pxFile)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte;

    while(!PXFileIsAtEnd(pxFile))
    {
        char character = 0;
        PXFileCursorPeek(pxFile, &character, sizeof(char));
        const PXBool advance = !(PXTextIsEndOfLine(character) || PXTextIsEndOfString(character));

        if(!advance)
        {
            break;
        }

        PXFileCursorAdvance(pxFile, 1u);
    }

    PXFileSkipEndOfLineCharacters(pxFile);

    const PXSize skippedBytes = pxFile->Buffer.CursorOffsetByte - positionBefore;

    return skippedBytes;
}

PXSize PXAPI PXFileCursorPositionTerminate(PXFile PXREF pxFile)
{
    pxFile->Buffer.SizeAllowedToUse = pxFile->Buffer.CursorOffsetByte;
    pxFile->Buffer.CursorOffsetByte = 0;

    return pxFile->Buffer.SizeAllowedToUse;
}

PXSize PXAPI PXFileDataCopy(PXFile PXREF pxInputStream, PXFile PXREF pxOutputStream, const PXSize length)
{
    void PXREF insertionPoint = PXFileDataAtCursor(pxOutputStream); // Position of output
    const PXSize copyedBytes = PXFileReadB(pxInputStream, insertionPoint, length); // Copy from input to output
    const PXSize movedBytes = PXFileCursorAdvance(pxOutputStream, copyedBytes); // Move output by copyed amount

    return movedBytes; // return moved amount of bytes
}

PXSize PXAPI PXFileReadTextIU8(PXFile PXREF pxFile, PXI8U PXREF number)
{
    int value = 0;
    const PXSize size = PXFileReadTextI(pxFile, &value);

    *number = value;

    return size;
}

PXSize PXAPI PXFileReadTextI(PXFile PXREF pxFile, int PXREF number)
{
    PXText pxText;
    pxText.SizeAllocated = PXFileRemainingSize(pxFile);
    pxText.SizeUsed = PXFileRemainingSize(pxFile);
    pxText.A = (char*)PXFileDataAtCursor(pxFile);
    pxText.Format = TextFormatASCII;

    pxText.SizeUsed = PXTextToInt(&pxText, number);

    PXFileCursorAdvance(pxFile, pxText.SizeUsed);

    return pxText.SizeUsed;
}

PXSize PXAPI PXFileReadType(PXFile PXREF pxFile, void PXREF valueAdress, const PXI32U type)
{
    char buffer[32];

    const PXSize amount = PXFileReadIXXE
    (
        pxFile, 
        buffer,
        PXTypeSizeGet(type),
        PXTypeEndianGet(type) == PXTypeEndianBig ? PXEndianBig : PXEndianLittle
    );

    PXMemoryCopyX(buffer, PXTypeSizeGet(type), valueAdress, PXTypeReciverSizeGet(type));

    return amount;
}

PXSize PXAPI PXFileReadIXXE(PXFile PXREF pxFile, void PXREF valueAdress, const PXSize valueSize, const PXEndian pxEndian)
{
    const PXSize writtenBytes = PXFileReadB(pxFile, valueAdress, valueSize);

    PXEndianSwap(valueAdress, valueSize, pxEndian, EndianCurrentSystem);

    return writtenBytes;
}

PXSize PXAPI PXFileReadIXXVE(PXFile PXREF pxFile, void* PXREF valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian)
{
    PXSize writtenBytes = 0;

    if(!valueList)
    {
        return 0;
    }

    for(PXSize i = 0; i < valueListSize; i++)
    {
        void PXREF adress = valueList[i];

        writtenBytes = PXFileReadB(pxFile, adress, valueSizeSingle);
    }

    PXEndianSwapV(valueList, valueListSize * valueSizeSingle, valueSizeSingle, pxEndian, EndianCurrentSystem);

    return writtenBytes;
}

PXSize PXAPI PXFileReadI8S(PXFile PXREF pxFile, PXI8S PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI8S));
}

PXSize PXAPI PXFileReadI8SV(PXFile PXREF pxFile, PXI8S PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI8S) * valueListSize);
}

PXSize PXAPI PXFileReadI8U(PXFile PXREF pxFile, PXI8U PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI8U));
}

PXSize PXAPI PXFileReadI8UV(PXFile PXREF pxFile, PXI8U PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI8U) * valueListSize);
}

PXSize PXAPI PXFileReadI16S(PXFile PXREF pxFile, PXI16S PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI16S));
}

PXSize PXAPI PXFileReadI16SV(PXFile PXREF pxFile, PXI16S PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI16S) * valueListSize);
}

PXSize PXAPI PXFileReadI16SE(PXFile PXREF pxFile, PXI16S PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI16S), pxEndian);
}

PXSize PXAPI PXFileReadI16SVE(PXFile PXREF pxFile, PXI16S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI16S), pxEndian);
}

PXSize PXAPI PXFileReadI16U(PXFile PXREF pxFile, PXI16U PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI16U));
}

PXSize PXAPI PXFileReadI16UV(PXFile PXREF pxFile, PXI16U PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI16U) * valueListSize);
}

PXSize PXAPI PXFileReadI16UE(PXFile PXREF pxFile, PXI16U PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI16U), pxEndian);
}

PXSize PXAPI PXFileReadI16UVE(PXFile PXREF pxFile, PXI16U* PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI16U), pxEndian);
}

PXSize PXAPI PXFileReadI32S(PXFile PXREF pxFile, PXI32S PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI32S));
}

PXSize PXAPI PXFileReadI32SV(PXFile PXREF pxFile, PXI32S PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI32S) * valueListSize);
}

PXSize PXAPI PXFileReadI32SE(PXFile PXREF pxFile, PXI32S PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI32S), pxEndian);
}

PXSize PXAPI PXFileReadI32SVE(PXFile PXREF pxFile, PXI32S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI32S), pxEndian);
}

PXSize PXAPI PXFileReadI32U(PXFile PXREF pxFile, PXI32U PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI32U));
}

PXSize PXAPI PXFileReadI32UV(PXFile PXREF pxFile, PXI32U PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI32U) * valueListSize);
}

PXSize PXAPI PXFileReadI32UE(PXFile PXREF pxFile, PXI32U PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI32U), pxEndian);
}

PXSize PXAPI PXFileReadI32UVE(PXFile PXREF pxFile, PXI32U* PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI32U), pxEndian);
}

PXSize PXAPI PXFileReadI64S(PXFile PXREF pxFile, PXI64S PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI64S));
}

PXSize PXAPI PXFileReadI64SV(PXFile PXREF pxFile, PXI64S PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI64S) * valueListSize);
}

PXSize PXAPI PXFileReadI64SE(PXFile PXREF pxFile, PXI64S PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI64S), pxEndian);
}

PXSize PXAPI PXFileReadI64VE(PXFile PXREF pxFile, PXI64S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI64S), pxEndian);
}

PXSize PXAPI PXFileReadI64U(PXFile PXREF pxFile, PXI64U PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI64U));
}

PXSize PXAPI PXFileReadI64UV(PXFile PXREF pxFile, PXI64U PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI64U) * valueListSize);
}

PXSize PXAPI PXFileReadI64UE(PXFile PXREF pxFile, PXI64U PXREF value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI64U), pxEndian);
}

PXSize PXAPI PXFileReadI64UVE(PXFile PXREF pxFile, PXI64U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI64U), pxEndian);
}

PXSize PXAPI PXFileReadF(PXFile PXREF pxFile, PXF32 PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(PXF32));
}

PXSize PXAPI PXFileReadFV(PXFile PXREF pxFile, PXF32 PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXF32) * valueListSize);
}

PXSize PXAPI PXFileReadD(PXFile PXREF pxFile, double PXREF value)
{
    return PXFileReadB(pxFile, value, sizeof(double));
}

PXSize PXAPI PXFileReadDV(PXFile PXREF pxFile, double PXREF valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(double) * valueListSize);
}

PXSize PXAPI PXFileBinding(PXFile PXREF pxFile, void PXREF dataStruct, const PXI32U* listOfTypes, const PXSize listOfTypesAmount, const PXI8U flags)
{
    PXFile* pxFileRedirect = pxFile;
    PXSize totalReadBytes = 0;
    PXSize totalSizeToRead = 0;
    PXFileIOMultibleFunction pxFileIOMultibleFunction = flags & PXFileBindingIOMask ? PXFileWriteB : PXFileReadB;


    PXBool didByteRead = 0;
    PXBool didBitRead = 0;

    // Pre allocate/Cache if needed


    // read/write actual data
    char* insertionPoint = (char*)dataStruct;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "File",
        "I/O-Bind",
        "Data: x%i - %s",
        listOfTypesAmount,
        flags & PXFileBindingIOMask ? "Write" : "Read"
    );
#endif

    for(PXSize i = 0; i < listOfTypesAmount; ++i)
    {
        const PXI32U type = listOfTypes[i];
        PXSize sizeOfType = PXFileDataWidthCalculate(pxFile, type);

        if(PXTypeAvoidTransphere & type)
        {
            insertionPoint += sizeOfType;
            // PXFileCursorAdvance(pxFile, sizeOfType);
            continue;
        }

        switch(PXTypeTypeMask & type)
        {
            case PXTypeBaseText:
            case PXTypeBaseDecimal:
            case PXTypeBaseNumeric:
            {
                const PXBool isInt = (PXTypeTypeMask & PXTypeBaseNumeric) > 0;
                const PXBool endianPreSwap = isInt && PXAccessModeWriteOnly == pxFile->AccessMode;
                const PXBool endianSwapPost = isInt && (PXAccessModeReadOnly == pxFile->AccessMode || PXAccessModeReadAndWrite == pxFile->AccessMode);

                if(didBitRead)
                {
                    PXFileSkipBitsToNextByte(pxFile);
                    didBitRead = PXFalse;
                }

                if(endianPreSwap) // Pre I/O Swap
                {
                    switch(PXTypeEndianMask & type)
                    {
                        case PXTypeEndianBig:
                        {
                            PXEndianSwap(insertionPoint, sizeOfType, PXEndianBig, EndianCurrentSystem);
                            break;
                        }
                        case PXTypeEndianLittle:
                        {
                            PXEndianSwap(insertionPoint, sizeOfType, PXEndianLittle, EndianCurrentSystem);
                            break;
                        }
                    }
                }

                totalReadBytes += pxFileIOMultibleFunction(pxFileRedirect, insertionPoint, sizeOfType); // Get data directly

                if(endianSwapPost) // POSR I/O Swap
                {
                    switch(PXTypeEndianMask & type)
                    {
                        case PXTypeEndianBig:
                        {
                            PXEndianSwap(insertionPoint, sizeOfType, PXEndianBig, EndianCurrentSystem);
                            break;
                        }
                        case PXTypeEndianLittle:
                        {
                            PXEndianSwap(insertionPoint, sizeOfType, PXEndianLittle, EndianCurrentSystem);
                            break;
                        }
                    }
                }

                break;
            }
            case PXTypeBaseBit:
            {
                const PXSize bitFieldValue = PXFileReadBits(pxFile, sizeOfType);

                if(insertionPoint)
                {
                    switch(PXTypeReciverSizeMask & type)
                    {
                        case PXTypeReciverSize08U:
                            *((PXI8U*)insertionPoint) = bitFieldValue;
                            sizeOfType = sizeof(PXI8U);
                            break;
                        case PXTypeReciverSize16U:
                            *((PXI16U*)insertionPoint) = bitFieldValue;
                            sizeOfType = sizeof(PXI16U);
                            break;
                        case PXTypeReciverSize32U:
                            *((PXI32U*)insertionPoint) = bitFieldValue;
                            sizeOfType = sizeof(PXI32U);
                            break;
                        case PXTypeReciverSize64U:
                            *((PXI64U*)insertionPoint) = bitFieldValue;
                            sizeOfType = sizeof(PXI64U);
                            break;
                    }
                }

                didBitRead = PXTrue;

                break;
            }
        }

#if PXLogEnable && 0

        PXText dataType;
        PXTextConstructNamedBufferA(&dataType, dataTypeBuffer, 64);

        PXText dataContent;
        PXTextConstructNamedBufferA(&dataContent, dataContentBuffer, 64);

        PXTypeEntry pxFileDataElementType;
        pxFileDataElementType.Adress = insertionPoint;
        pxFileDataElementType.Type = type;

        PXTypeEntryInfo(&pxFileDataElementType, &dataType, &dataContent);

        PXLogPrint
        (
            PXLoggingInfo,
            "File",
            "I/O-Bind",
            "| %2i | (%i) %3i / %-3i | %2i B | %-7s | %s",
            i + 1,
            pxFileRedirect->DataCursorBitOffset,
            pxFileRedirect->DataCursor - sizeOfType,
            pxFileRedirect->DataAllocated,        
            sizeOfType,
            dataType.A,
            dataContent.A
        );
#endif

        insertionPoint += sizeOfType;
    }

    // If we read bits, we need to adjust or reallign to a byte
    if(didBitRead)
    {
        if(PXFileBindingAllignToByte & flags)
        {
            totalReadBytes += PXFileSkipBitsToNextByte(pxFile);
        }
        else
        {
            totalReadBytes += 1;
        } 
    }

    return totalReadBytes;
}

PXSize PXAPI PXFileReadMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileElementList, const PXSize pxFileElementListFullSize)
{
    return PXFileIOMultible(pxFile, pxFileElementList, pxFileElementListFullSize, PXFileReadB);
}

PXSize PXAPI PXFileDataWidthCalculate(PXFile PXREF pxFile, const PXI32U type)
{
    // If value is ignored
    {
        const PXBool ignoreIn32Bitif32Bit =
            ((type & PXTypeIgnoreIFMask) == PXTypeIgnoreIn32B) &&
            (pxFile->BitFormatOfData != PXBitFormat32);

        const PXBool ignoreIn64Bitif64Bit =
            ((type & PXTypeIgnoreIFMask) == PXTypeIgnoreIn64B) &&
            (pxFile->BitFormatOfData != PXBitFormat64);

        const PXBool ignore = ignoreIn32Bitif32Bit || ignoreIn64Bitif64Bit;

        if(ignore)
        {
            return 0;
        }
    }

    // Adress
    {
        const PXSize isAdress = type & PXTypeAdressMask;

        if(isAdress)
        {
            switch(pxFile->BitFormatOfData)
            {
                case PXBitFormat32:
                    return 4u;

                case PXBitFormat64:
                    return 8u;
            }
        }
    }


    switch(PXTypeTypeMask & type)
    {
        case PXTypeBaseBit:
        {
            return PXTypeSizeMask & type; // bits to byte, round to full byte
        }

        default:
            return PXTypeSizeMask & type;    // If nothing applys, use raw size
    }
}

PXSize PXAPI PXFileIOMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileElementList, const PXSize pxFileElementListFullSize, PXFileIOMultibleFunction pxFileIOMultibleFunction)
{
    const PXSize pxDataStreamElementListSize = pxFileElementListFullSize / sizeof(PXTypeEntry);

    const PXBool needIntermediateCache = !PXFileCanDirectAccess(pxFile);

    PXFile pxStackFile;
    void* stackMemory = PXNull;
    PXFile* pxFileRedirect = pxFile;
    PXSize totalReadBytes = 0;
    PXSize totalSizeToRead = 0;

    if(needIntermediateCache)
    {
        for(PXSize i = 0; i < pxDataStreamElementListSize; ++i)
        {
            const PXTypeEntry PXREF pxFileDataElementType = &pxFileElementList[i];
            const PXSize sizeOfType =
                (!(pxFileDataElementType->Type & PXTypeAdressMask) * (pxFileDataElementType->Type & PXTypeSizeMask)) +
                (((pxFileDataElementType->Type & PXTypeAdressMask) && !(pxFileDataElementType->Type & PXTypeIgnoreIn32B) && (pxFile->BitFormatOfData == PXBitFormat32)) * 4u) +
                (((pxFileDataElementType->Type & PXTypeAdressMask) && !(pxFileDataElementType->Type & PXTypeIgnoreIn64B) && (pxFile->BitFormatOfData == PXBitFormat64)) * 8u);

            PXMemoryClear(pxFileDataElementType->Adress, sizeOfType);

            //assert(sizeOfType > 0);

            totalSizeToRead += sizeOfType;
        }

        void* stackMemory = PXMemoryHeapCallocT(char, totalSizeToRead);

        PXFileCreateInfo pxFileCreateInfo;
        PXClear(PXFileCreateInfo, &pxFileCreateInfo);
        pxFileCreateInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileCreateInfo.Data.Adress = stackMemory;
        pxFileCreateInfo.Data.SizeAllocated = totalSizeToRead;

        const PXResult fileOpenResult = PXFileCreate(&pxStackFile, &pxFileCreateInfo);

        PXFileDataCopy(pxFile, &pxStackFile, totalSizeToRead); // Read actual data all at once        
        PXFileCursorToBeginning(&pxStackFile);



        pxFileRedirect = &pxStackFile;

#if PXFileDebugOutput
        PXLogPrint
        (
            PXLoggingInfo,
            "File",
            "I/O",
            "Cached batch (x%-2i, %2i B)",
            pxDataStreamElementListSize,
            totalSizeToRead
        );
#endif
    }
#if PXFileDebugOutput
    else
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "File",
            "I/O",
            "Direct batch (x%-2i)",
            pxDataStreamElementListSize
        );
    }
#endif

    for(PXSize i = 0; i < pxDataStreamElementListSize; ++i)
    {
        const PXTypeEntry PXREF pxFileDataElementType = &pxFileElementList[i];
        const PXSize sizeOfType = PXFileDataWidthCalculate(pxFile, pxFileDataElementType->Type);

        switch(pxFileDataElementType->Type & PXTypeTypeMask)
        {
            case PXTypeBaseDecimal:
            case PXTypeBaseText:
            case PXTypeBaseNumeric:
            {
                if(PXAccessModeWriteOnly == pxFile->AccessMode) // Pre I/O Swap
                {
                    switch(pxFileDataElementType->Type & PXTypeEndianMask)
                    {
                        case PXTypeEndianBig:
                        {
                            PXEndianSwap(pxFileDataElementType->Adress, sizeOfType, PXEndianBig, EndianCurrentSystem);
                            break;
                        }
                        case PXTypeEndianLittle:
                        {
                            PXEndianSwap(pxFileDataElementType->Adress, sizeOfType, PXEndianLittle, EndianCurrentSystem);
                            break;
                        }
                    }
                }

                totalReadBytes += pxFileIOMultibleFunction(pxFileRedirect, pxFileDataElementType->Adress, sizeOfType); // Get data directly

                if(PXAccessModeReadOnly == pxFile->AccessMode || PXAccessModeReadAndWrite == pxFile->AccessMode) // POSR I/O Swap
                {
                    switch(pxFileDataElementType->Type & PXTypeEndianMask)
                    {
                        case PXTypeEndianBig:
                        {
                            PXEndianSwap(pxFileDataElementType->Adress, sizeOfType, PXEndianBig, EndianCurrentSystem);
                            break;
                        }
                        case PXTypeEndianLittle:
                        {
                            PXEndianSwap(pxFileDataElementType->Adress, sizeOfType, PXEndianLittle, EndianCurrentSystem);
                            break;
                        }
                    }
                }

                break;
            }
            case PXTypeBaseBit:
            {
                const PXSize bitFieldValue = PXFileReadBits(pxFile, sizeOfType);

                if(pxFileDataElementType->Adress)
                {
                    switch(pxFileDataElementType->Type & PXTypeReciverSizeMask)
                    {
                        case PXTypeReciverSize08U:
                            *((PXI8U*)pxFileDataElementType->Adress) = bitFieldValue;
                            break;
                        case PXTypeReciverSize16U:
                            *((PXI16U*)pxFileDataElementType->Adress) = bitFieldValue;
                            break;
                        case PXTypeReciverSize32U:
                            *((PXI32U*)pxFileDataElementType->Adress) = bitFieldValue;
                            break;
                        case PXTypeReciverSize64U:
                            *((PXI64U*)pxFileDataElementType->Adress) = bitFieldValue;
                            break;
                    }
                }

                break;
            }
        }

#if PXFileDebugOutput

        PXText dataType;
        PXTextConstructNamedBufferA(&dataType, dataTypeBuffer, 64);

        PXText dataContent;
        PXTextConstructNamedBufferA(&dataContent, dataContentBuffer, 64);

        PXTypeEntryInfo(pxFileDataElementType, &dataType, &dataContent);

        PXLogPrint
        (
            PXLoggingInfo,
            "File",
            "I/O",
            "| %2i | %3i / %-3i | %2i B | %-7s | %-33s |",
            i + 1,
            pxFileRedirect->DataCursor - sizeOfType,
            pxFileRedirect->DataAllocated,
            sizeOfType,
            dataType.A,
            dataContent.A
        );
#endif
    }

    PXMemoryHeapFree(PXNull, stackMemory);

    return totalReadBytes;
}

PXSize PXAPI PXFileRead(PXFile PXREF pxFile, PXFile PXREF pxFileOUT, const PXSize length)
{
    void* insert = PXFileDataAtCursor(pxFileOUT);
    PXSize readBytes = PXFileReadB(pxFile, insert, length);

    PXFileCursorAdvance(pxFileOUT, readBytes);

    return PXResultOK;
}

PXSize PXAPI PXFileReadB(PXFile PXREF pxFile, void PXREF value, const PXSize length)
{
    ++(pxFile->CounterOperationsRead);

    if(!value)
    {
        return PXFileCursorAdvance(pxFile, length);
    }

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeMappedFromDisk:
        {
            const void* currentPosition = PXFileDataAtCursor(pxFile);
            const PXSize moveSize = PXFileCursorAdvance(pxFile, length);

            PXMemoryCopy(currentPosition, value, moveSize);

            return moveSize;
        }
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
        {
            if(pxFile->Buffer.CursorOffsetByte >= pxFile->Buffer.SizeAllowedToUse)
            {
                return 0; // PXActionRefusedIndexOutOfBounce;
            }

#if OSUnix
            const PXSize writtenBytes = fread(value, sizeof(PXByte), length, pxFile->FileID);
            const PXBool readSuccess = 0 != writtenBytes; // fread does not set errno! 0 equals EOF or read-error.

            if(!readSuccess)
            {
                const PXBool isAtEnd = feof(pxFile->FileID);

                if(isAtEnd)
                {
                    return 0; // PXResultOK;
                }

                // Read error
                const int hasError = ferror(pxFile->FileID);

                return 0; // PXActionFailedRead;
            }

            pxFile->Buffer.CursorOffsetByte += writtenBytes;

            return writtenBytes;

#elif OSWindows
            DWORD writtenBytes = 0;
            const PXBool success = ReadFile(pxFile->FileHandle, value, length, &writtenBytes, PXNull);
            const PXResult pxResult = PXErrorCurrent(success);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

#if PXFileDebugOutput
            PXLogPrint
            (
                PXLoggingInfo,
                "File",
                "Read : %i B",
                writtenBytes
            );
#endif

            pxFile->Buffer.CursorOffsetByte += writtenBytes;

            return writtenBytes;
#endif
        }
    }

    return 0;
}

PXSize PXAPI PXFileReadA(PXFile PXREF pxFile, PXASCII* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

PXSize PXAPI PXFileReadW(PXFile PXREF pxFile, PXUNICODE* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

PXSize PXAPI PXFileTextU(PXFile PXREF pxFile, PXUTF8* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

void PXAPI PXFileReadUntil(PXFile PXREF pxFile, void* value, const PXSize length, const char character)
{
    const char* currentPosition = (char*)PXFileDataAtCursor(pxFile);

    PXSize lengthCopy = 0;

    while(!PXFileIsAtEnd(pxFile))
    {
        const char* data = (char*)PXFileDataAtCursor(pxFile);
        const PXBool advance = *data != character && length <= lengthCopy;

        if(!advance)
        {
            break;
        }

        ++lengthCopy;

        PXFileCursorAdvance(pxFile, 1u);
    }

    const PXSize readableSize = PXFileRemainingSize(pxFile);

    PXMemoryCopyX(currentPosition, readableSize, value, lengthCopy);
}

PXSize PXAPI PXFileByteSwap(PXFile PXREF pxFileTarget, PXFile PXREF pxFileSource)
{
    pxFileTarget->Buffer.CursorOffsetByte = PXFileReadB
    (
        pxFileSource, 
        pxFileTarget->Buffer.Data,
        pxFileSource->Buffer.SizeAllocated
    );

    for(PXSize i = 0; i < pxFileTarget->Buffer.SizeAllowedToUse; i += 2)
    {
        PXByte* cursor = &pxFileTarget->Buffer.Data[i];
        PXByte* cursorA = &cursor[0];
        PXByte* cursorB = &cursor[1];

        PXByte c = *cursorA;
        *cursorA = *cursorB;
        *cursorB = c;
    }

    return 0;
}

PXBool PXAPI PXFileReadAndCompareI64U(PXFile PXREF pxFile, const PXI64U value)
{
    PXI64U valueA = 0;
    const PXI64U valueSize = sizeof(PXI64U);
    const PXSize readBytes = PXFileReadB(pxFile, &valueA, valueSize);
    const PXBool successful = readBytes == valueSize;

    return successful;
}

PXBool PXAPI PXFileReadAndCompare(PXFile PXREF pxFile, const void* value, const PXSize length)
{
    const void* currentPosition = PXFileDataAtCursor(pxFile);
    const PXSize readableSize = PXFileRemainingSize(pxFile);

    const PXBool result = PXMemoryCompare(currentPosition, readableSize, value, length);

    if(result)
    {
        PXFileCursorAdvance(pxFile, length);
    }

    return result;
}

PXBool PXAPI PXFileReadAndCompareV(PXFile PXREF pxFile, const void* PXREF value, const PXSize PXREF valueElementSizeList, const PXSize valueLength)
{
    const void PXREF currentPosition = PXFileDataAtCursor(pxFile);
    const PXSize readableSize = PXFileRemainingSize(pxFile);

    for(size_t i = 0; i < valueLength; ++i)
    {
        const void* text = value[i];
        const PXSize size = valueElementSizeList[i];
        const PXBool result = PXMemoryCompare(currentPosition, readableSize, text, size);

        if(result)
        {
            PXFileCursorAdvance(pxFile, size);
            return PXTrue;
        }
    }

    return PXFalse;
}

PXSize PXAPI PXFileWriteI8S(PXFile PXREF pxFile, const PXI8S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI8S));
}

PXSize PXAPI PXFileWriteI8SV(PXFile PXREF pxFile, const PXI8S PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI8S) * valueListSize);
}

PXSize PXAPI PXFileWriteI8U(PXFile PXREF pxFile, const PXI8U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI8U));
}

PXSize PXAPI PXFileWriteAtI8U(PXFile PXREF pxFile, const PXI8U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte; // save current position

    pxFile->Buffer.CursorOffsetByte = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI8U(pxFile, value); // Length

    pxFile->Buffer.CursorOffsetByte = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI8UV(PXFile PXREF pxFile, const PXI8U PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI8U) * valueListSize);
}

PXSize PXAPI PXFileWriteI16S(PXFile PXREF pxFile, const PXI16S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI16S));
}

PXSize PXAPI PXFileWriteI16SV(PXFile PXREF pxFile, const PXI16S PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI16S) * valueListSize);
}

PXSize PXAPI PXFileWriteI16SE(PXFile PXREF pxFile, const PXI16S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI16S);
    PXI16S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI16SVE(PXFile PXREF pxFile, const PXI16S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI16SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI16U(PXFile PXREF pxFile, const PXI16U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI16U));
}

PXSize PXAPI PXFileWriteAtI16U(PXFile PXREF pxFile, const PXI16U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte; // save current position

    pxFile->Buffer.CursorOffsetByte = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI16U(pxFile, value); // Length

    pxFile->Buffer.CursorOffsetByte = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI16UV(PXFile PXREF pxFile, const PXI16U PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI16U) * valueListSize);
}

PXSize PXAPI PXFileWriteI16UE(PXFile PXREF pxFile, const PXI16U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI16U);
    PXI16U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI16UVE(PXFile PXREF pxFile, const PXI16U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI16UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI32S(PXFile PXREF pxFile, const PXI32S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI32S));
}

PXSize PXAPI PXFileWriteI32SV(PXFile PXREF pxFile, const PXI32S PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI32S) * valueListSize);
}

PXSize PXAPI PXFileWriteI32SE(PXFile PXREF pxFile, const PXI32S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI32S);
    PXI32S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI32SVE(PXFile PXREF pxFile, const PXI32S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI32SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI32U(PXFile PXREF pxFile, const PXI32U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI32U));
}

PXSize PXAPI PXFileWriteAtI32U(PXFile PXREF pxFile, const PXI32U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte; // save current position

    pxFile->Buffer.CursorOffsetByte = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI32U(pxFile, value); // Length

    pxFile->Buffer.CursorOffsetByte = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI32UV(PXFile PXREF pxFile, const PXI32U PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI32U) * valueListSize);
}

PXSize PXAPI PXFileWriteI32UE(PXFile PXREF pxFile, const PXI32U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI32U);
    PXI32U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteAtI32UE(PXFile PXREF pxFile, const PXI32U value, const PXEndian pxEndian, const PXSize index)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte; // save current position

    pxFile->Buffer.CursorOffsetByte = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI32UE(pxFile, value, pxEndian); // Length

    pxFile->Buffer.CursorOffsetByte = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI32UVE(PXFile PXREF pxFile, const PXI32U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI32UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI64S(PXFile PXREF pxFile, const PXI64S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI64S));
}

PXSize PXAPI PXFileWriteI64SV(PXFile PXREF pxFile, const PXI64S PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI64S) * valueListSize);
}

PXSize PXAPI PXFileWriteI64SE(PXFile PXREF pxFile, const PXI64S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI64S);
    PXI64S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI64VE(PXFile PXREF pxFile, const PXI64S PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI64SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI64U(PXFile PXREF pxFile, const PXI64U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI64U));
}

PXSize PXAPI PXFileWriteI64UV(PXFile PXREF pxFile, const PXI64U PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI64U) * valueListSize);
}

PXSize PXAPI PXFileWriteI64UE(PXFile PXREF pxFile, const PXI64U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI64U);
    PXI64U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI64UVE(PXFile PXREF pxFile, const PXI64U PXREF valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI64UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteF(PXFile PXREF pxFile, const PXF32 value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXF32));
}

PXSize PXAPI PXFileWriteFV(PXFile PXREF pxFile, const PXF32 PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXF32) * valueListSize);
}

PXSize PXAPI PXFileWriteD(PXFile PXREF pxFile, const double value)
{
    return PXFileWriteB(pxFile, &value, sizeof(double));
}

PXSize PXAPI PXFileWriteDV(PXFile PXREF pxFile, const double PXREF valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(double) * valueListSize);
}

PXSize PXAPI PXFileWriteInternMove(PXFile PXREF pxFile, const PXI16S offset, const PXSize length)
{
    const PXSize oldPosition = pxFile->Buffer.CursorOffsetByte;
    char PXREF destination = (char*)PXFileDataAtCursor(pxFile);

    // Souce
    const PXSize steptsTaken = PXFileCursorRewind(pxFile, offset); // Safe rewind
    const char PXREF source = (const char*)PXFileDataAtCursor(pxFile);


    // Destination
    pxFile->Buffer.CursorOffsetByte = oldPosition; // restore

    if(steptsTaken != offset)
    {
        // FAILUE
        return 0;
    }

    PXSize amount = PXMemoryMove(source, length, destination, length);

    PXFileCursorAdvance(pxFile, length);

    return amount;
}

PXSize PXAPI PXFileWriteB(PXFile PXREF pxFile, const void PXREF value, const PXSize length)
{
    ++(pxFile->CounterOperationsWrite);

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeMappedFromDisk:
        {
            const PXSize writableSize = PXFileRemainingSize(pxFile);
            void PXREF currentPosition = PXFileDataAtCursor(pxFile);
            const PXSize copyedBytes = PXMemoryCopyX(value, length, currentPosition, writableSize);

            PXFileCursorAdvance(pxFile, copyedBytes);

            return copyedBytes;
        }

        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
        {
#if OSUnix
            // TODO: implement
            return 0;
#elif OSWindows
            DWORD writtenBytes = 0;

            const PXBool result = WriteFile(pxFile->FileHandle, value, length, &writtenBytes, PXNull); // Windows XP (+UWP), Kernel32.dll, fileapi.h
            const PXBool validAction = writtenBytes == length;

            if(!validAction)
            {
                DebugBreak();
                return 0;
            }

            ++(pxFile->CounterOperationsWrite);
            pxFile->Buffer.CursorOffsetByte += writtenBytes;
            pxFile->Buffer.SizeAllowedToUse += writtenBytes;

            return writtenBytes;

#endif
        }

        default:
            return 0;
    }
}

PXSize PXAPI PXFileWriteAtB(PXFile PXREF pxFile, const void PXREF data, const PXSize dataSize, const PXSize index)
{
    const PXSize positionBefore = pxFile->Buffer.CursorOffsetByte; // save current position

    PXFileCursorMoveTo(pxFile, index); // jump to offset

    const PXSize writtenBytes = PXFileWriteB(pxFile, data, dataSize); // Length

    PXFileCursorMoveTo(pxFile, positionBefore); // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteMultible(PXFile PXREF pxFile, const PXTypeEntry PXREF pxFileDataElementTypeList, const PXSize pxFileElementListFullSize)
{
    return PXFileIOMultible(pxFile, pxFileDataElementTypeList, pxFileElementListFullSize, PXFileWriteB);
}

PXSize PXAPI PXFileWriteFill(PXFile PXREF pxFile, const PXByte value, const PXSize length)
{
    if((!pxFile) || (length == 0))
    {
        return 0;
    }

    
    PXByte buffer[64];
    PXByte* stackMemory = buffer;
    PXBool isAllocated = length > 64;

    if(isAllocated)
    {
        stackMemory = PXMemoryHeapCallocT(PXByte, length);
    }

    for(PXSize i = 0; i < length; ++i)
    {
        stackMemory[i] = value;
    }

    const PXSize writtenBytes = PXFileWriteB(pxFile, stackMemory, length);

    if(isAllocated)
    {
        PXMemoryHeapFree(PXNull, stackMemory);
    }

    return writtenBytes;
}

PXSize PXAPI PXFileWriteNewLine(PXFile PXREF pxFile)
{
    return PXFileWriteB(pxFile, "\n", 1);
}

PXSize PXAPI PXFileWriteC(PXFile PXREF pxFile, const char character)
{
    return PXFileWriteB(pxFile, &character, 1);
}

PXSize PXAPI PXFileWriteText(PXFile PXREF pxFile, PXText PXREF pxText)
{
    return PXFileWriteB(pxFile, pxText->Data, pxText->SizeUsed);
}

PXSize PXAPI PXFileWriteA(PXFile PXREF pxFile, const char PXREF text, const PXSize textSize)
{
#if 1
    return PXFileWriteB(pxFile, text, textSize);
#else
    const PXSize writableSize = PXFileRemainingSize(pxFile);
    char PXREF currentPosition = (char PXREF)PXFileCursorPosition(pxFile);

    const PXSize writtenBytes = PXTextCopyA(text, textSize, currentPosition, writableSize);

    PXFileCursorAdvance(pxFile, writtenBytes);

    return writtenBytes;
#endif
}

PXSize PXAPI PXFileWriteW(PXFile PXREF pxFile, const wchar_t PXREF text, const PXSize textSize)
{
    const PXSize writableSize = PXFileRemainingSize(pxFile);
    wchar_t PXREF currentPosition = (wchar_t PXREF)PXFileDataAtCursor(pxFile);

    const PXSize writtenBytes = PXTextCopyW(text, textSize, currentPosition, writableSize);

    PXFileCursorAdvance(pxFile, writtenBytes);

    return writtenBytes;
}

PXSize PXAPI PXFileWriteAF(PXFile PXREF pxFile, const char PXREF format, ...)
{
    PXSize writtenBytes = 0;

    va_list args;
    va_start(args, format);

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal: // Memory is handled internally.
        case PXFileLocationModeExternal: // Memory is stored outside this object
        case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
        case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
        case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
        {
            void PXREF currentPosition = PXFileDataAtCursor(pxFile);

            const PXSize writableSize = PXFileRemainingSize(pxFile);

            PXText pxText;
            PXTextFromAdressA(&pxText, currentPosition, writableSize, writableSize);

            const int writtenBytes = PXTextPrintV(&pxText, format, args);



            const PXBool sucessful = writtenBytes >= 0;

            if(!sucessful)
            {
                return 0;
            }

            PXFileCursorAdvance(pxFile, writtenBytes);

            break;
        }
        case PXFileLocationModeDirectUncached:
        {
            //const int writtenBytesA = vfprintf(pxFile->FileIDPOSIX, format, args);
            char buffer[256];

            writtenBytes = PXTextPrintAV(buffer, 256, format, args);

            pxFile->Buffer.SizeAllocated += writtenBytes;
            pxFile->Buffer.SizeAllowedToUse += writtenBytes;

            PXFileWriteB(pxFile, buffer, writtenBytes);

            break;
        }
        default:
            break;
    }

    va_end(args);

    return writtenBytes;
}

PXSize PXAPI PXFileWriteWF(PXFile PXREF pxFile, const wchar_t PXREF format, ...)
{
    return  0;
}

PXResult PXAPI PXFileStoreOnDiskA(PXFile PXREF pxFile, const char* fileName)
{
    HANDLE fileHandle = CreateFileA
    (
        fileName, 
        GENERIC_WRITE, 
        0,
        NULL, 
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );
    PXResult createResult = PXErrorCurrent(INVALID_HANDLE_VALUE != fileHandle);

    if(PXResultOK != createResult)
    {
        return createResult;
    }

    DWORD writtenFiles = 0;

    WriteFile(fileHandle, pxFile->Buffer.Data, pxFile->Buffer.CursorOffsetByte, &writtenFiles, NULL);

    FlushFileBuffers(fileHandle);

    CloseHandle(fileHandle);

    return PXResultOK;
}

PXResult PXAPI PXFileMapToMemoryEE(PXFile PXREF pxFile, const PXSize requestedSize, const PXAccessMode pxAccessMode, const PXBool prefetch)
{
    PXI64U start = PXTimeCounterStampGet();



#if OSUnix
    const int flags = MAP_SHARED;// MAP_PRIVATE;// | MAP_POPULATE;
    int accessType = 0;

    if(pxFileIOInfo->AccessMode & PXAccessREAD)
    {
        accessType |= PROT_READ;
    }

    if(pxFileIOInfo->AccessMode & PXAccessWRITE)
    {
        accessType |= PROT_WRITE;
    }

    if(pxFileIOInfo->AccessMode & PXAccessEXECUTE)
    {
        accessType |= PROT_EXEC;
    }

    // Map data into virtual memory space
    pxFile->Buffer.Data = mmap
    (
        0, // addressPrefered
        pxFile->Buffer.SizeAllowedToUse,
        accessType,
        flags,
        pxFile->FileDescriptorID, // fileDescriptor
        0 // offset
    );
    const PXResult mappingResult = PXErrorCurrent(PXNull != pxFile->Buffer.Data);

    if(PXResultOK != openResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "File",
            "Mapping",
            "Failed for <%s>\n",
            pxFileIOInfo->FilePathAdress
        );
#endif

        return PXActionFailedFileMapping;
    }

    pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

    // We are allowed to close the file handle and/or descriptor?
    // YES! but better not close the descriptor,
    // Access works in any case but you loose the file reference
    //
    // const int closeResultID = fclose(pxFile->FileID);
    // pxFile->FileID = PXNull;

#elif OSWindows

    // Create mapping
    {
        DWORD flProtect = SEC_COMMIT;

        LARGE_INTEGER maximumSize;
        maximumSize.QuadPart = 0;

        if(PXAccessModeReadAndWrite == pxFile->AccessMode || PXAccessModeWriteOnly == pxFile->AccessMode)
        {
            maximumSize.QuadPart = requestedSize;
        }


        //  DWORD dwMaximumSizeHigh = 0;
        // DWORD dwMaximumSizeLow = 0; // Problem if file is 0 Length

#if OS32Bit
        dwMaximumSizeHigh = 0;
        dwMaximumSizeLow = pxFile->Buffer.DataSize;
#elif OS64Bit
        dwMaximumSizeHigh = (pxFile->Buffer.DataSize & 0xFFFFFFFF00000000) >> 32u;
        dwMaximumSizeLow = pxFile->Buffer.DataSize & 0x00000000FFFFFFFF;
#endif

        switch(pxAccessMode)
        {
            case PXAccessModeNoAccess:
                flProtect |= PAGE_NOACCESS;
                break;

            case PXAccessModeReadOnly:
                flProtect |= PAGE_READONLY;
                break;

            case PXAccessModeWriteOnly:
                flProtect |= PAGE_READWRITE; // PAGE_WRITECOPY
                break;

            case PXAccessModeReadAndWrite:
                flProtect |= PAGE_READWRITE;
                break;
        }

        // [i] I want to add LargePage support but it seems you cant do that with files.

        pxFile->MappingHandle = CreateFileMappingA
        (
            pxFile->FileHandle,
            PXNull, // No security attributes
            flProtect,
            maximumSize.HighPart,   // Is those are both zero, the mapping
            maximumSize.LowPart,    // will be as big as the size.
            PXNull // No Name
        );
        const PXResult createMappingResult = PXErrorCurrent(PXNull != pxFile->MappingHandle);

        // We can get a "ERROR_ALREADY_EXISTS" if the mapping
        // exist already, the HANDLE will still be a valid one

        if(PXResultOK != createMappingResult)
        {
#if PXLogEnable
            char permissionText[8];

            PXAccessModeToStringA(permissionText, pxFile->AccessMode);

            PXLogPrint
            (
                PXLoggingError,
                PXFileText,
                "Mapping-Create",
                "Failed, %s -> <%s>",
                permissionText,
                "--err---"
            );
#endif

            return createMappingResult;
        }

        // TODO: What is this for?
        if(pxFile->Buffer.SizeAllowedToUse == 0)
        {
            pxFile->Buffer.SizeAllowedToUse = maximumSize.QuadPart;
            pxFile->Buffer.SizeAllocated = maximumSize.QuadPart;
        }

        {
            DWORD desiredAccess = 0;
            DWORD fileOffsetHigh = 0;
            DWORD fileOffsetLow = 0;
            PXSize numberOfBytesToMap = 0;
            void* baseAddressTarget = 0;
            //DWORD  numaNodePreferred = -1; // (NUMA_NO_PREFERRED_NODE)

            switch(pxAccessMode)
            {
                case PXAccessModeReadOnly:
                    desiredAccess |= FILE_MAP_READ;
                    break;

                case PXAccessModeWriteOnly:
                    desiredAccess |= FILE_MAP_WRITE;
                    break;

                case PXAccessModeReadAndWrite:
                    desiredAccess |= FILE_MAP_ALL_ACCESS;
                    break;
            }

            // if large pages are supported, anable if
#if WindowsAtleastVista && 0
            if(useLargeMemoryPages)
            {
                desiredAccess |= FILE_MAP_LARGE_PAGES;
            }
#endif

            pxFile->Buffer.Data = (PXByte*)MapViewOfFile // MapViewOfFileExNuma is only useable starting windows vista, this function in XP
            (
                pxFile->MappingHandle,
                desiredAccess,
                fileOffsetHigh,
                fileOffsetLow,
                numberOfBytesToMap
            );
            const PXResult viewMappingResult = PXErrorCurrent(PXNull != pxFile->Buffer.Data);

            if(PXResultOK != viewMappingResult)
            {
#if PXLogEnable
                char permissionText[8];

                PXAccessModeToStringA(permissionText, pxAccessMode);

                PXLogPrint
                (
                    PXLoggingError,
                    PXFileText,
                    "Mapping-View",
                    "Failed, %s -> <%s>",
                    permissionText,
                    "--err---"
                );
#endif

                return viewMappingResult;
            }
        }

        pxFile->LocationMode = PXFileLocationModeMappedFromDisk;

    }

#endif



    if(prefetch)
    {
        PXMemoryVirtualPrefetch(pxFile->Buffer.Data, pxFile->Buffer.SizeAllowedToUse);
    }

    PXI64U stop = PXTimeCounterStampGet();
    PXI64U diff = stop - start;

    PXF32 timeS = PXTimeCounterStampToSecoundsF(diff);

    PXI64U bps = pxFile->Buffer.SizeAllowedToUse / timeS;


#if PXLogEnable
    PXText pxTextbps;
    PXTextConstructNamedBufferA(&pxTextbps, pxTextbpsBuffer, 32);
    PXTextFormatSize(&pxTextbps, bps);

    PXLogPrint
    (
        PXLoggingInfo,
        PXFileText,
        "Open-Mapping",
        "Read with %s/s",
        pxTextbps.A
    );
#endif

    return PXResultOK;
}


PXSize PXAPI PXFileSkipBitsToNextByte(PXFile PXREF pxFile)
{
    const PXBool hasAnyBitConsumed = pxFile->Buffer.CursorOffsetBit > 0;

    if(!hasAnyBitConsumed)
    {
        return 0;
    }

    pxFile->Buffer.CursorOffsetBit = 0; // Reset

    return PXFileCursorAdvance(pxFile, 1); // Go 1 Byte further
}

PXSize PXAPI PXFileCursorMoveBits(PXFile PXREF pxFile, const PXSize amountOfBits)
{
    pxFile->Buffer.CursorOffsetBit += amountOfBits;

    PXFileBitsAllign(pxFile);

    return 0;
}

PXSize PXAPI PXFileBitsAllign(PXFile PXREF pxFile)
{
    PXSize accumulator = 0;

    while(pxFile->Buffer.CursorOffsetBit >= 8u) // Move a Byte at the time forward, 8 Bits = 1 Byte.
    {
        pxFile->Buffer.CursorOffsetByte++;
        pxFile->Buffer.CursorOffsetBit -= 8u;

        ++accumulator;
    }

    return accumulator;
}

PXSize PXAPI PXFilePeekBits(PXFile PXREF pxFile, const PXSize amountOfBits)
{
    unsigned int bitMask = ((1u << amountOfBits) - 1u) << pxFile->Buffer.CursorOffsetBit; // 0000111111
    unsigned int bitBlock;
    unsigned char* a = (unsigned char*)PXFileDataAtCursor(pxFile);
    unsigned char* b = a + 1;
    unsigned char* c = a + 2;
    unsigned char* d = a + 3;
    unsigned char* maxAdress = (unsigned char*)pxFile->Buffer.Data + (pxFile->Buffer.SizeAllowedToUse - 1);

    unsigned int ai = a > maxAdress ? 0 : *a;
    unsigned int bi = b > maxAdress ? 0 : *b;
    unsigned int ci = c > maxAdress ? 0 : *c;
    unsigned int di = d > maxAdress ? 0 : *d;

    // [d][c][b][a] Little pxEndian

    bitBlock = ai | bi << 8 | ci << 16 | di << 24;

    unsigned int result = bitBlock & bitMask;

#if PXFileDebugOutput && 0
    printf("Extract %i Bits. Byte__:%i Offset:%i\n", amountOfBits, pxFile->Buffer.CursorOffsetByte, pxFile->BitOffset);
    printf("BitBlock : ");
    PrintBinary(bitBlock);
    printf("BitMask  : ");
    PrintBinary(bitMask);
    printf("Result   : ");
    //PrintBinary(result);
#endif

    result >>= pxFile->Buffer.CursorOffsetBit; // Shoitft correction

#if PXFileDebugOutput && 0
    printf("Shifted  : ");
    //PrintBinary(result);
#endif

    return result;
}

PXSize PXAPI PXFileReadBits(PXFile PXREF pxFile, const PXSize amountOfBits)
{
    const PXSize result = PXFilePeekBits(pxFile, amountOfBits);

    PXFileCursorMoveBits(pxFile, amountOfBits);

    return result;
}

PXSize PXAPI PXFileWriteBits(PXFile PXREF pxFile, const PXSize bitData, const PXSize amountOfBits)
{
    PXSize PXREF currentPos = (PXSize PXREF)PXFileDataAtCursor(pxFile);
    PXSize bitBlockCache = 0;

    for(PXSize i = 0; i < amountOfBits; i++)
    {
        // Data = current bit << current offset
        bitBlockCache |= (bitData & ((PXSize)1u << i));
    }

    *currentPos |= bitBlockCache << pxFile->Buffer.CursorOffsetBit;
    pxFile->Buffer.CursorOffsetBit += amountOfBits;

    const PXSize movedBytes = PXFileBitsAllign(pxFile);

    return amountOfBits;
}

PXResult PXAPI PXFileTimeGet
(
    const PXFile PXREF pxFile,
    PXTime PXREF pxTimeCreation,
    PXTime PXREF pxTimeAccessLast,
    PXTime PXREF pxTimeWriteLast
)
{
#if OSUnix
    //st_ctime

    //    strftime(date, sizeof date, ... to);

    //stat();

    return PXActionRefusedNotImplemented;
#elif OSWindows
    FILETIME fileTimeList[3];

    // Fetch Data
    {
        const BOOL result = GetFileTime // Windows XP (+UWP), Kernel32.dll, fileapi.h
        (
            pxFile->FileHandle,
            &fileTimeList[0],
            &fileTimeList[1],
            &fileTimeList[2]
        );
        const PXResult pxResult = PXErrorCurrent(result);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }

    // Convert
    {
        PXTime PXREF pxTimeList[3] = { pxTimeCreation, pxTimeAccessLast, pxTimeWriteLast };
        SYSTEMTIME systemTimeList[3];

        for(PXI8U i = 0; i < 3; ++i)
        {
            FileTimeToSystemTime(&fileTimeList[i], &systemTimeList[i]); // Windows XP (+UWP), Kernel32.dll, timezoneapi.h
            PXTimeConvertFromOS(pxTimeList[i], &systemTimeList[i]);
        }
    }

    return PXResultOK;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI PXFilePath(const PXFile PXREF pxFile, PXText PXREF filePath, const PXBool doWrite)
{
    PXResult pxResult = PXResultInvalid;

    /*
    #if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista // On Vista we can always store. No Store needed
    return PXResultOK;
#elif WindowsAtleastXP // On windows XP only if beeing mapped.
    const PXBool isMapped = PXNull != pxFile->MappingHandle;

    if(isMapped)
    {
        return PXResultOK;
    }
#endif
#endif
    // TODO:
    //PXTextCreateCopy(&pxFile->FilePath, filePath);

    return PXResultOK;
    
    */

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
        {
            // As we are not mapped, loaded the file into direct memory, there is no file attached.

            filePath->A = pxFile->FilePath.A;
            filePath->SizeUsed = pxFile->FilePath.SizeUsed;
            filePath->SizeAllocated = pxFile->FilePath.SizeAllocated;
            filePath->Format = TextFormatUTF8;

            pxResult = PXResultOK;

            break;
        }
        case PXFileLocationModeMappedFromDisk:
        {
            // We can translate a handle into a filepath
            pxResult = PXFileNameViaHandle(pxFile, filePath);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return pxResult;
}

PXResult PXAPI PXFilePathLong(PXText PXREF pxTextInput, PXText PXREF pxTextOutput)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    pxTextOutput->SizeUsed = GetFullPathNameA // Windows XP (+UWP), Kernel32.dll, fileapi.h
    (
        pxTextInput->A,
        pxTextInput->SizeUsed,
        pxTextOutput->A,
        PXNull
    );

    return PXResultOK;

#else
    return PXActionRefusedNotSupported;
#endif
}



PXResult PXAPI PXFileNameViaHandle(const PXFile PXREF pxFile, PXText PXREF pxText)
{
    char filePathIntermediate[260];
    char* filePathIntermediateOff = filePathIntermediate;
    PXSize filePathIntermediateSize = 0;


#if OSUnix

    // if the filedescriptor is 0, assume we forgot to set it
    if(0 == pxFile->FileDescriptorID)
    {
        pxFile->FileDescriptorID = fileno(pxFile->FileID);
    }

    const PXBool isValidFileDescriptor = 3 <= pxFile->FileDescriptorID;

    if(!isValidFileDescriptor)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "OS-Kernel",
            "File",
            "Standard input, output and error stream are not files! ID:<%i> FILE*:%p",
            pxFile->FileDescriptorID,
            pxFile->FileID
        );
#endif

        return PXResultRefusedParameterInvalid;
    }

    char namePathBuffer[64];
    const PXSize namePathBufferSIze = sizeof(namePathBuffer);

    PXTextPrintA(namePathBuffer, namePathBufferSIze, "/proc/self/fd/%d", pxFile->FileDescriptorID); // "/prof/self/fd/0123456789"

    const PXSize writtenBytes = readlink(namePathBuffer, filePath->A, filePath->SizeAllocated); // [POSIX.1 - 2008]
    const PXResult readResult = PXErrorCurrent(-1 != writtenBytes);

    if(PXResultOK != readResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "OS-Kernel",
            "File",
            "Translate file descriptor <%i> failed! <%s>, FILE*:%p",
            pxFile->FileDescriptorID,
            namePathBuffer,
            pxFile->FileID
        );
#endif

        return readResult;
    }

    filePath->SizeUsed = writtenBytes;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "OS-Kernel",
        "File",
        "Translate file descriptor <%i> to <%s>",
        pxFile->FileDescriptorID,
        filePath->A
    );
#endif


    // realpath();
    //
    // Only for Apple-OSX
    //const int resultID = fcntl(pxFile->FileID, F_GETPATH, filePath->A); // [POSIX]

    return PXResultOK;

#elif OSWindows

#if WindowsAtleastVista

    // FILE_NAME_OPENED, VOLUME_NAME_DOS

    DWORD flags = 0;
    BOOL s = GetHandleInformation(pxFile->FileHandle, &flags);

    filePathIntermediateSize = GetFinalPathNameByHandleA
    (
        pxFile->FileHandle,
        filePathIntermediate,
        260,
        FILE_NAME_OPENED | VOLUME_NAME_DOS
    ); // Windows Vista, Kernel32.dll, Windows.h
    const PXResult readResult = PXErrorCurrent(0 != filePathIntermediateSize);


    // GetShortPathNameA() makes a path to something like "\\?\C:\Data\WORKSP~1\_GIT_~1\BITFIR~1\GAMECL~1\Shader\SKYBOX~2.GLS"
    // Why would you ever want this?

    // _fullpath(filePath->A, buffer, PXPathSizeMax); also, does not what we need it to do

    if(PXResultOK != readResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXFileText,
            "Name",
            "Translate file handle <%p> failed!",
            pxFile->FileHandle
        );
#endif

        return readResult;
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXFileText,
        "Name",
        "Translate file handle <%p> to <%s>",
        pxFile->FileHandle,
        pxText->A
    );
#endif


    filePathIntermediateSize -= 4u;
    filePathIntermediateOff += 4u;

    char buffer[260];

    const DWORD currentPathSize = GetCurrentDirectoryA(260, buffer); // Windows XP (+UWP), Kernel32.dll, winbase.h

    const PXSize maxSize = PXMathMinimumIU(currentPathSize, filePathIntermediateSize);
    const PXBool isMatching = PXTextCompareA(buffer, currentPathSize, filePathIntermediateOff, maxSize, 0);

    if(isMatching)
    {
        filePathIntermediateOff += (currentPathSize + 1);
        filePathIntermediateSize -= (currentPathSize + 1);
    }

    pxText->SizeUsed = filePathIntermediateSize;

    PXTextReplaceByte(filePathIntermediateOff, filePathIntermediateSize, '\\', '/');

    return PXResultOK;
#elif WindowsAtleastXP && 0

    GetMappedFileName(GetCurrentProcess(), pMem, pszFilename, MAX_PATH)

#endif

        // Last resort not to get the file name per handle but from self-storage
        // TODO:
        // PXTextCopy(&pxFile->FilePath, filePath);

        return PXResultOK;
#endif
}

void PXAPI PXFileEndianessSet(PXFile PXREF pxFile, const PXEndian pxEndian)
{
    pxFile->EndiannessOfData = pxEndian;
}

void PXAPI PXFileBitFormatOfDataSet(PXFile PXREF pxFile, const PXBitFormat pxBitFormat)
{
    pxFile->BitFormatOfData = pxBitFormat;
}

PXSize PXAPI PXFileDataUtilized(const PXFile PXREF pxFile)
{
    PXSize used = pxFile->Buffer.CursorOffsetByte;
    PXSize allocated = pxFile->Buffer.SizeAllocated;

    if(allocated > 0)
    {
        return (int)((used / (PXF32)allocated) * 100);
    }
    else
    {
        return 0;
    }
}

PXBool PXAPI PXDirectoryIsRootFolder(const char* directory)
{
    return ((directory[0] == '.') && (directory[1] == '\0'));
}

PXBool PXAPI PXDirectoryIsCurrentFolder(const char* directory)
{
    return (directory[0] == '.') && (directory[1] == '.') && (directory[2] == '\0');
}

PXBool PXAPI PXDirectoryIsDotFolder(const char* directory)
{
    return PXDirectoryIsRootFolder(directory) || PXDirectoryIsCurrentFolder(directory);
}