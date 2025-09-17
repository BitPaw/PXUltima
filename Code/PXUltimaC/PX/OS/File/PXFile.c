#include "PXFile.h"

#include <PX/Media/PXText.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <assert.h>


const char PXFileText[] = "File-OS";

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

PXResult PXAPI  PXFilePathSplitt(const PXText* const fullPath, PXFilePathStructure* const pxFilePathStructure)
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
                pxFilePathStructure->Drive.NumberOfCharacters;
                pxFilePathStructure->Drive.A = fullPath->A;
                pxFilePathStructure->Drive.Format = fullPath->Format;
            }

            if(indexFirstDot)
            {
                pxFilePathStructure->Extension.SizeAllocated = lengthExtension;
                pxFilePathStructure->Extension.SizeUsed = lengthExtension;
                pxFilePathStructure->Extension.NumberOfCharacters;
                pxFilePathStructure->Extension.A = fullPath->A + indexFirstDot + 1;
                pxFilePathStructure->Extension.Format = fullPath->Format;
            }

            // File
            if(hasFileName)
            {
                pxFilePathStructure->FileName.SizeAllocated = lengthFileName;
                pxFilePathStructure->FileName.SizeUsed = lengthFileName;
                pxFilePathStructure->FileName.NumberOfCharacters;
                pxFilePathStructure->FileName.A = fullPath->A + indexFileNameStart;
                pxFilePathStructure->FileName.Format = fullPath->Format;
            }

            if(hasDirectory)
            {
                pxFilePathStructure->Directory.SizeAllocated = endPath + 1;
                pxFilePathStructure->Directory.SizeUsed = pxFilePathStructure->Directory.SizeAllocated- pxFilePathStructure->Drive.SizeUsed;
                pxFilePathStructure->Directory.NumberOfCharacters;
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

            return PXActionSuccessful;










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

PXResult PXAPI  PXFilePathCombine(PXText* const fullPath, PXFilePathStructure* const pxFilePathStructure)
{
    PXAppend(fullPath, &pxFilePathStructure->Drive);
    PXAppend(fullPath, &pxFilePathStructure->Directory);
    PXAppend(fullPath, &pxFilePathStructure->FileName);
    PXAppendA(fullPath, ".", 1);
    PXAppend(fullPath, &pxFilePathStructure->Extension);

    fullPath->A[fullPath->SizeUsed] = '\0';
    return PXActionSuccessful;
}

PXSize PXAPI PXFilePathExtensionGet(const PXText* const filePath, PXText* const extension)
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

    const PXSize writtenBytes = PXTextCopy(&stringResult, extension);

    return writtenBytes;
}


PXResult PXAPI  PXFileFormatInfoViaPath(PXFileFormatInfo* const pxFileFormatInfo, const PXText* const filePath)
{
    PXText pxTextExtension;
    PXTextConstructNamedBufferA(&pxTextExtension, extensionBuffer, ExtensionMaxSize);

    const PXSize writtenBytes = PXFilePathExtensionGet(filePath, &pxTextExtension);

    PXTextToUpperCase(&pxTextExtension, &pxTextExtension);

#if PXLogEnable
    char path[PXPathSizeMax];
    char extension[PXPathSizeMax];

    PXTextCopyA(filePath->A, filePath->SizeUsed, path, PXPathSizeMax);
    PXTextCopyA(pxTextExtension.A, pxTextExtension.SizeUsed, extension, PXPathSizeMax);

    PXLogPrint
    (
        PXLoggingInfo,
        PXFileText,
        "Extension",
        "Detect:\n"
        "%20s : %s\n"
        "%20s : %s",
        "Path", path,
        "Extension", extension
    );
#endif

    pxFileFormatInfo->Flags = PXFileFormatUnkown;

    switch(writtenBytes)
    {
        case 0:
            pxFileFormatInfo->Flags =  PXFileFormatInvalid;
            break;

        case 1u:
        {
            switch(*pxTextExtension.A)
            {
                case 'H':
                    pxFileFormatInfo->Flags = PXFileFormatC;
                    break;
                case 'C':
                    pxFileFormatInfo->Flags =  PXFileFormatC;
                    break;
                case 'O':
                    pxFileFormatInfo->Flags =  PXFileFormatBinaryLinux;
                    break;
            }

            break;
        }
        case 2u:
        {
            const PXI32U list = PXI16FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI16Make('K', 'O'):
                case PXI16Make('S', 'O'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinaryLinux;
                    break;
                case PXI16Make('F', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatFastFile;
                    break;
                case PXI16Make('C', 'S'):
                    pxFileFormatInfo->Flags =  PXFileFormatCSharp;
                    break;
                case PXI16Make('J', 'S'):
                    pxFileFormatInfo->Flags =  PXFileFormatJavaScript;
                    break;
            }

            break;
        }
        case 3u:
        {
            const PXI32U list = PXI24FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI24Make('V', 'O', 'B'):
                case PXI24Make('I', 'F', 'O'):
                case PXI24Make('B', 'U', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatVideoObject;
                    break;

                case PXI24Make('V', '6', '4'):
                case PXI24Make('Z', '6', '4'):
                case PXI24Make('N', '6', '4'):
                    pxFileFormatInfo->Flags =  PXFileFormatN64;
                    break;

                case PXI24Make('C', 'P', 'P'):
                case PXI24Make('H', 'P', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatCPP;
                    break;

                case PXI24Make('C', 'R', '3'):
                    pxFileFormatInfo->Flags =  PXFileFormatCanonRaw3;
                    break;

                case PXI24Make('B', 'K', '2'):
                case PXI24Make('B', 'I', 'K'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinkVideo;
                    break;

                case PXI24Make('D', 'D', 'S'):
                    pxFileFormatInfo->Flags =  PXFileFormatDirectDrawSurfaceTexture;
                    break;

                case PXI24Make('B', 'I', 'N'):
                case PXI24Make('P', 'R', 'X'):
                case PXI24Make('M', 'O', 'D'):
                case PXI24Make('E', 'L', 'F'):
                case PXI24Make('O', 'U', 'T'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinaryLinux;
                    break;

                case PXI24Make('R', 'A', 'R'):
                    pxFileFormatInfo->Flags =  PXFileFormatEugeneRoshalArchive;
                    break;

                case PXI24Make('F', 'N', 'T'):
                    pxFileFormatInfo->Flags =  PXFileFormatSpriteFont;
                    break;

                case PXI24Make('G', 'I', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatGIF;
                    break;
                case PXI24Make('H', 'T', 'M'):
                    pxFileFormatInfo->Flags =  PXFileFormatHTML;
                    break;
                case PXI24Make('I', 'N', 'I'):
                    pxFileFormatInfo->Flags =  PXFileFormatINI;
                    break;
                case PXI24Make('M', '4', 'A'):
                    pxFileFormatInfo->Flags =  PXFileFormatM4A;
                    break;
                case PXI24Make('3', 'D', 'S'):
                    pxFileFormatInfo->Flags =  PXFileFormatA3DS;
                    break;
                case PXI24Make('A', 'C', 'C'):
                    pxFileFormatInfo->Flags =  PXFileFormatAAC;
                    break;
                case PXI24Make('A', 'V', 'I'):
                    pxFileFormatInfo->Flags =  PXFileFormatAVI;
                    break;
                case PXI24Make('B', 'M', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatBitMap;
                    break;
                case PXI24Make('C', 'S', 'S'):
                    pxFileFormatInfo->Flags =  PXFileFormatCSS;
                    break;
                case PXI24Make('E', 'M', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatEML;
                    break;
                case PXI24Make('S', 'Y', 'S'):
                case PXI24Make('C', 'O', 'M'):
                case PXI24Make('D', 'L', 'L'):
                case PXI24Make('E', 'X', 'E'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinaryWindows;
                    break;
                case PXI24Make('F', 'B', 'X'):
                    pxFileFormatInfo->Flags =  PXFileFormatFilmBox;
                    break;
                case PXI24Make('M', 'P', '3'):
                    pxFileFormatInfo->Flags =  PXFileFormatMP3;
                    break;
                case PXI24Make('M', 'P', '4'):
                    pxFileFormatInfo->Flags =  PXFileFormatMP4;
                    break;
                case PXI24Make('M', 'S', 'I'):
                    pxFileFormatInfo->Flags =  PXFileFormatMSI;
                    break;
                case PXI24Make('M', 'T', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatMTL;
                    break;
                case PXI24Make('O', 'B', 'J'):
                    pxFileFormatInfo->Flags =  PXFileFormatWavefront;
                    break;
                case PXI24Make('M', 'K', 'V'):
                    pxFileFormatInfo->Flags =  PXFileFormatMatroska;
                    break;
                case PXI24Make('O', 'G', 'G'):
                    pxFileFormatInfo->Flags =  PXFileFormatOGG;
                    break;
                case PXI24Make('P', 'D', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatPDF;
                    break;
                case PXI24Make('P', 'H', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatPHP;
                    break;
                case PXI24Make('P', 'L', 'Y'):
                    pxFileFormatInfo->Flags =  PXFileFormatPLY;
                    break;
                case PXI24Make('P', 'N', 'G'):
                    pxFileFormatInfo->Flags =  PXFileFormatPNG;
                    break;
                case PXI24Make('Q', 'U', 'I'):
                    pxFileFormatInfo->Flags =  PXFileFormatQOI;
                    break;
                case PXI24Make('S', 'T', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatSTL;
                    break;
                case PXI24Make('S', 'V', 'G'):
                    pxFileFormatInfo->Flags =  PXFileFormatSVG;
                    break;
                case PXI24Make('T', 'I', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatTagImage;
                    break;
                case PXI24Make('U', 'S', 'D'):
                    pxFileFormatInfo->Flags =  PXFileFormatUniversalSceneDescription;
                    break;
                case PXI24Make('T', 'G', 'A'):
                    pxFileFormatInfo->Flags =  PXFileFormatTGA;
                    break;
                case PXI24Make('T', 'T', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatTrueTypeFont;
                    break;
                case PXI24Make('W', 'A', 'D'):
                    pxFileFormatInfo->Flags =  PXFileFormatWAD;
                    break;
                case PXI24Make('W', 'A', 'V'):
                    pxFileFormatInfo->Flags =  PXFileFormatWave;
                    break;
                case PXI24Make('W', 'M', 'A'):
                    pxFileFormatInfo->Flags =  PXFileFormatWMA;
                    break;
                case PXI24Make('X', 'M', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatXML;
                    break;
                case PXI24Make('Y', 'M', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatYAML;
                    break;
                case PXI24Make('Z', 'I', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatZIP;
                    break;
            }

            break;
        }
        case 4u:
        {
            const PXI32U list = PXI32FromAdress(pxTextExtension.A);

            switch(list)
            {
                case PXI32Make('B', 'I', 'K', '2'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinkVideo;
                    break;
                case PXI32Make('J', 'A', 'V', 'A'):
                    pxFileFormatInfo->Flags =  PXFileFormatJava;
                    break;
                case PXI32Make('H', 'E', 'I', 'C'): // Fall though
                case PXI32Make('H', 'E', 'I', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatHighEfficiencyImageFile;
                    break;
                case PXI32Make('P', 'U', 'F', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatBinaryLinux;
                    break;
                case PXI32Make('F', 'L', 'A', 'C'):
                    pxFileFormatInfo->Flags =  PXFileFormatFLAC;
                    break;
                case PXI32Make('M', 'I', 'D', 'I'):
                    pxFileFormatInfo->Flags =  PXFileFormatMIDI; 
                    break;
                case PXI32Make('S', 'T', 'E', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatSTEP;
                    break;
                case PXI32Make('T', 'I', 'F', 'F'):
                    pxFileFormatInfo->Flags =  PXFileFormatTagImage;
                    break;
                case PXI32Make('J', 'P', 'E', 'G'):
                    pxFileFormatInfo->Flags =  PXFileFormatJPEG;
                    break;
                case PXI32Make('J', 'S', 'O', 'N'):
                    pxFileFormatInfo->Flags =  PXFileFormatJSON;
                    break;
                case PXI32Make('V', 'R', 'M', 'L'):
                    pxFileFormatInfo->Flags =  PXFileFormatVRML;
                    break;
                case PXI32Make('W', 'E', 'B', 'M'):
                    pxFileFormatInfo->Flags =  PXFileFormatWEBM;
                    break;
                case PXI32Make('W', 'E', 'B', 'P'):
                    pxFileFormatInfo->Flags =  PXFileFormatWEBP;
                    break;
            }

            break;
        }
    }

    if(pxFileFormatInfo->Flags != PXFileFormatUnkown)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFileText,
            "Extension",
            "Sucessfully detected!"
        );
#endif

        return PXActionSuccessful;
    }


    // When we did not detect any format, it could be, that some nice guy added
    // an additional "fake" extension. 
    // Example: Instead of "myfile.ext" we get "myfile.ext"
    
    // Lets find the secound dot.
    PXSize firstDot = PXTextFindLastCharacter(filePath, '.');
    
    if(firstDot != -1)
    {
        PXText pxText;
        PXTextConstructFromAdressA(&pxText, filePath->A, firstDot, firstDot);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXFileText,
            "Extension",
            "Unkown extension! Two dots are detected, we might have a fake extension"
        );
#endif

        return PXFileFormatInfoViaPath(pxFileFormatInfo, &pxText);
    }

    pxFileFormatInfo->Flags = PXFileFormatUnkown;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingError,
        PXFileText,
        "Extension",
        "Could not resolve extension"
    );
#endif

    return PXActionSuccessful;
}

const PXByte PXFileFormatRSkinedMeshSignature[] = { 0xC3, 0x4F, 0xFD, 0x22};
const PXByte PXFileFormatRSkinedMeshSignatureLength = sizeof(PXFileFormatRSkinedMeshSignature) / sizeof(PXByte);

const PXByte PXSignature6D[] = { 0x6D, 0x8D, 0xBF, 0x30};
const PXByte PXSignature6DLength = sizeof(PXSignature6D) / sizeof(PXByte);


const PXByte PXFileFormatRSkinSimpleSignature[4] = { 0x33, 0x22, 0x11, 0x00 };
const PXByte PXFileFormatRSkinSimpleSignatureLength = sizeof(PXFileFormatRSkinSimpleSignature) / sizeof(PXByte);

const PXByte PXFileFormatRLightGridSignature[] = {0x03, 0x00, 0x00, 0x00};;
const PXByte PXFileFormatRLightGridSignatureLength = sizeof(PXFileFormatRLightGridSignature) / sizeof(PXByte);

const char PXExtensionUnkown[] = "ooo";

PXResult PXAPI  PXFileFormatInfoViaContent(PXFileFormatInfo* const pxFileFormatInfo, PXFile* const pxFile)
{
    PXClear(PXFileFormatInfo, pxFileFormatInfo);

    const PXFileFormatInfo pxFileFormatInfoList[] =
    {
        {"scb", 6, "r3d2Mesh", 8, 0, PXFileFormatRedshiftMesh | PXFileFormatVarriantBinary},
        {"anm", 6, "r3d2anmd", 8, 0, PXFileFormatRedshiftAnimation | PXFileFormatVarriantBinary},
        {"anm", 6, "r3d2canm", 8, 0, PXFileFormatRedshiftAnimation | PXFileFormatVarriantBinary},
        {"skl", 6, "r3d2sklt", 8, 0, PXFileFormatRedshiftSkeleton | PXFileFormatVarriantBinary},
        {"dds", 3, "DDS", 3, 0, PXFileFormatDirectDrawSurfaceTexture | PXFileFormatVarriantBinary},
        {"skn", 4, PXFileFormatRSkinSimpleSignature, PXFileFormatRSkinSimpleSignatureLength, 0, PXFileFormatRSkinSimple},
        {"bin", 4, "PROP", 4, 0, PXFileFormatRProperty },
        {"bnk", 3, "BKHD", 4, 0, PXFileFormatWAudioBank},
        {"wgeo", 3, "WGEO", 4, 0, PXFileFormatRGeometryWorld},
        {"mapgeo", 3, "OEGM", 4, 0, PXFileFormatRGeometryMap},
        {"sco", 3, "[ObjectBegin]", 13, 0, PXFileFormatRedshiftMesh | PXFileFormatVarriantText},
        {"luaobj", 3, "LuaQ", 4, 0, PXFileFormatLua},
        {PXExtensionUnkown, 3, "PreLoad", 7, 0, PXFileFormatRPreLoad },
        {PXExtensionUnkown, 3, PXFileFormatRLightGridSignature, PXFileFormatRLightGridSignatureLength, 0, PXFileFormatRLightGrid },
        {PXExtensionUnkown, 3, "RST", 3, 0, PXFileFormatRStringTable },
        {PXExtensionUnkown, 3, "PTCH", 4, 0, PXFileFormatRPropertyOverride},
        {PXExtensionUnkown, 3, PXFileFormatRSkinedMeshSignature, PXFileFormatRSkinedMeshSignatureLength, 0, PXFileFormatRSkinedMesh },
        {"tex", 3, "TEX", 3, 0, PXFileFormatRTexture},      
        {PXExtensionUnkown, 8, PXSignature6D, PXSignature6DLength, 4, PXFileFormat6D},
        {"oegm", 8, "OEGM", 4, 0, PXFileFormatOEGM},
        {"opam", 8, "OPAM", 4, 0, PXFileFormatOEGM},        
    
    };
    const PXI8U amount = sizeof(pxFileFormatInfoList) / sizeof(PXFileFormatInfo);


    if(!pxFile->Data)
    {

    }


    for(size_t i = 0; i < amount; ++i)
    {
        const PXFileFormatInfo* const pxFileFormatInfoComp = &pxFileFormatInfoList[i];

        PXBool isMatch = PXTextCompareA
        (
            pxFileFormatInfoComp->SigantureText,
            pxFileFormatInfoComp->SigantureLength, 
            (PXByte*)pxFile->Data + pxFileFormatInfoComp->SigantureOffset,
            pxFile->DataUsed, 
            0
        );

        if(isMatch)
        {
            PXCopy(PXFileFormatInfo, pxFileFormatInfoComp, pxFileFormatInfo);
            return PXActionSuccessful;
        }
    } 

    PXClear(PXFileFormatInfo, pxFileFormatInfo);
    pxFileFormatInfo->Flags = PXFileFormatUnkown; 

    return PXActionSuccessful;
}

void PXAPI PXTypeEntryInfo(PXTypeEntry* const pxFileDataElementType, PXText* const dataType, PXText* const dataContent)
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

PXBool PXAPI PXFileDoesExist(const PXText* const filePath)
{
    if(!filePath)
    {
        return PXFalse;
    }

    switch(filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix

#elif OSWindows
            const DWORD dwAttrib = GetFileAttributesA(filePath->A); // Windows XP, Kernel32.dll, fileapi.h
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
                    filePath->A
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
            const DWORD dwAttrib = GetFileAttributesW(filePath->W); // Windows XP, Kernel32.dll, fileapi.h
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

PXBool PXAPI PXFileDoesExistA(const char* const filePath)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextLengthUnkown);

    return PXFileDoesExist(&pxText);
}

PXResult PXAPI  PXFileRemove(const PXText* const filePath)
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

            const PXActionResult pxActionResult = PXErrorCurrent(success);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
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

            const PXActionResult pxActionResult = PXErrorCurrent(success);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

#endif

            break;
        }
        default:
        {
            return PXActionRefusedInvalidFilePath;
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXFileRename(const PXText* const oldName, const PXText* const newName)
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
            const PXActionResult pxActionResult = PXErrorCurrent(success);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            return pxActionResult;
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

            const PXActionResult pxActionResult = PXErrorCurrent(success);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            return pxActionResult;

#endif
        }
        default:
        {
            return PXActionRefusedInvalidFilePath;
        }
    }
}

PXResult PXAPI  PXFileCopy(const PXText* const sourceFilePath, const PXText* const destinationFilePath, const PXBool overrideIfExists)
{
    if(!sourceFilePath) return PXActionRefuedInputBufferNull;
    if(!destinationFilePath) return PXActionRefuedOutputBufferNull;

    switch(sourceFilePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix || OSForcePOSIXForWindows
            const FILE* const fileSource = fopen(sourceFilePath, FileReadMode);
            const FILE* const fileDestination = fopen(destinationFilePath, FileWriteMode);
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
            const PXActionResult pxActionResult = PXErrorCurrent(succesfull);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            return PXActionSuccessful;
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
            const PXActionResult pxActionResult = PXErrorCurrent(succesfull);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            return PXActionSuccessful;
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
    const char* const pathCurrent, 
    const PXSize patHCurrentSize, 
    const char* const pathTarget,
    const PXSize pathTargetSize, 
    char* const pathResult,
    PXSize* const pathResultSize
)
{



}

PXResult PXAPI  PXFilePathRelativeFromFile(const PXFile* const pxFile, const PXText* const targetPath, PXText* const resultPath)
{
    if(targetPath->A[1] == ':')
    {
        // do nothing and keep absolut path

        PXTextCopy(targetPath, resultPath);

        return PXActionSuccessful;
    }

    //---<Get current path>----------------
    PXText currentObjectFilePath;
    PXTextConstructNamedBufferA(&currentObjectFilePath, currentFilePathBuffer, PXPathSizeMax);

    const PXBool success = PXFilePathGet(pxFile, &currentObjectFilePath); // Work PXWavefront file path
    //-------------------------------

    // When we take the direct folder, we have a shortcut to directs merge paths
    if(targetPath->A[0] == '.')
    {
        PXFilePathStructure pxFilePathStructureBefore;
        PXFilePathStructure pxFilePathStructureAfter;

        // Run a
        {
            const PXActionResult result = PXFilePathSplitt(&currentObjectFilePath, &pxFilePathStructureBefore);

            if(PXActionSuccessful != result)
            {
                return result;
            }

        }

        // RUn B
        {
            const PXActionResult result = PXFilePathSplitt(targetPath, &pxFilePathStructureAfter);

            if(PXActionSuccessful != result)
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
}

PXResult PXAPI  PXFilePathSwapFileName(const PXText* const inputPath, PXText* const exportPath, const PXText* const fileName)
{
    PXFilePathStructure pxFilePathStructureBefore;
    PXFilePathStructure pxFilePathStructureAfter;

    // Run a
    {
        const PXActionResult result = PXFilePathSplitt(inputPath, &pxFilePathStructureBefore);

        if(PXActionSuccessful != result)
        {
            return result;
        }

    }

    // RUn B
    {
        const PXActionResult result = PXFilePathSplitt(fileName, &pxFilePathStructureAfter);

        if(PXActionSuccessful != result)
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

    return PXActionSuccessful;
}

void PXAPI PXFilePathSwapExtension(const PXText* const inputPath, PXText* const exportPath)
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

PXResult PXAPI  PXFileName(const PXFile* const pxFile, PXText* const fileName)
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

    return PXActionSuccessful;
}

PXBool PXAPI PXFileDirectoryPathExtract(const PXText* const path, PXText* const directoryPath)
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

PXBool PXAPI PXFileCanDirectAccess(const PXFile* const pxFile)
{
    return pxFile->MappingHandle != PXHandleNotSet;
}

PXBool PXAPI PXFileKeyValueFetch(PXFile* const pxFile, PXTypeBinding* const pxTypeBinding)
{
    return PXActionSuccessful;
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
    const PXActionResult openTokenResult = PXErrorCurrent(initSuccess);


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
    const PXActionResult ntsResult = PXErrorCurrent(STATUS_SUCCESS == ntsResultID);


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

PXResult PXAPI  PXFileOpen(PXFile* const pxFile, PXFileOpenInfo* const pxFileIOInfo)
{
    if(!(pxFile && pxFileIOInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    PXClear(PXFile, pxFile);

    pxFile->EndiannessOfData = EndianCurrentSystem;
    pxFile->BitFormatOfData = PXBitFormat64;
    pxFile->AccessMode = pxFileIOInfo->AccessMode;
    pxFile->CachingMode = pxFileIOInfo->MemoryCachingMode;

    // Update stuff that might be invalid
    {
        if(PXAccessModeNoAccess == pxFileIOInfo->AccessMode)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "File",
                "Open",
                "File info does not have any permission!? %s\n",
                pxFileIOInfo->FilePathAdress
            );
#endif

            return PXActionRefusedArgumentInvalid;
        }
    }


    PXText pxText;
    PXTextConstructFromAdressA(&pxText, pxFileIOInfo->FilePathAdress, pxFileIOInfo->FilePathSize, pxFileIOInfo->FilePathSize);

    switch(PXFileIOInfoFileMask & pxFileIOInfo->FlagList)
    {
        case PXFileIOInfoFilePhysical:
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
            const PXActionResult openResult = PXErrorCodeFromID(openResultID);
#else
            //open64();
            pxFile->FileID = fopen(pxText.A, readMode);
            const PXActionResult openResult = PXErrorCurrent(PXNull != pxFile->FileID);
#endif

            if(PXActionSuccessful != openResult)
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
            pxFile->DataUsed = fileInfo.st_size;

#if OSUnix
           // pxFile->DataAllocated = fileInfo.st_blksize * fileInfo.st_blocks; // Might be wrong
            pxFile->DataAllocated = pxFile->DataUsed;
#elif OSWindows
            // Size on harddrive not contained under windows?
            pxFile->DataAllocated = fileInfo.st_size;
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

            switch(pxFileIOInfo->AccessMode)
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
            if(pxFileIOInfo->AccessMode == PXAccessModeWriteOnly || pxFileIOInfo->AccessMode == PXAccessModeReadAndWrite)
            {
                //const PXActionResult directoryCreateResult = DirectoryCreateA(filePath);

                //PXActionExitOnError(directoryCreateResult);
            }

            // Open file
            {
                HANDLE fileHandle = PXNull;
                SECURITY_ATTRIBUTES* securityAttributes = PXNull;

                switch(pxText.Format)
                {
                    case TextFormatASCII:
                    case TextFormatUTF8:
                    {
                        if(PXAccessModeReadOnly == pxFile->AccessMode)
                        {
                            const DWORD dwAttrib = GetFileAttributesA(pxText.A); // Windows XP (+UWP), Kernel32.dll, fileapi.h
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
                            pxText.A,
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
                            const DWORD dwAttrib = GetFileAttributesW(pxText.W); // Windows XP (+UWP), Kernel32.dll, fileapi.h
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
                            pxText.W,
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

                const PXActionResult fileOpenResult = PXErrorCurrent(INVALID_HANDLE_VALUE != fileHandle);

                if(PXActionSuccessful != fileOpenResult)
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

                    pxFile->DataUsed = largeInt.QuadPart;
                    pxFile->DataAllocated = largeInt.QuadPart;
                }

                // Get date
                {
                    FILETIME creationTime;
                    FILETIME lastAccessTime;
                    FILETIME lastWriteTime;

                    const BOOL fileTimeResult = GetFileTime(pxFile->FileHandle, &creationTime, &lastAccessTime, &lastWriteTime);

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
                // Get current time or set it
            }


            PXFilePathSet(pxFile, &pxText);
#endif




#if PXLogEnable
            PXText pxTextSize;
            PXTextConstructNamedBufferA(&pxTextSize, pxTextBuffer, 32);
            PXTextFormatSize(&pxTextSize, pxFile->DataUsed);

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
                "File",
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
                "Path", pxFileIOInfo->FilePathAdress,
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
                pxFile->Data = PXMemoryVirtualAllocate(pxFile->DataUsed, &pxFile->DataAllocated, PXAccessModeReadAndWrite);

                if(pxFile->Data)
                {
                    // SUCCESS

                    pxFile->DataUsed = pxFile->DataAllocated;
                    pxFile->LocationMode = PXFileLocationModeMappedVirtual;

                    BOOL ok = ReadFile(pxFile->FileHandle, pxFile->Data, pxFile->DataUsed, NULL, NULL);


                   // PXMemoryVirtualPrefetch(pxFile->Data, pxFile->DataUsed);

                    PXPerformanceInfoGet(&pxPerformanceInfo);


#if 0
                    volatile PXByte sink = 0;

                    for(PXSize i = 0; i < pxFile->DataUsed; i += 4096)
                    {
                        sink ^= ((PXByte*)pxFile->Data)[i]; // Touch one byte per page
                    }
#endif

                  //  PXPerformanceInfoGet(&pxPerformanceInfo);



               


                    // We NEED to store the reference manually
                   // PXFilePathSet(, );

                    pxFile->FilePathData = pxFileIOInfo->FilePathAdress;
                    pxFile->FilePathSize = pxFileIOInfo->FilePathSize;




#if PXLogEnable
                    PXI64U bps = pxFile->DataUsed / pxPerformanceInfo.TimeDelta;
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
                    const PXBool shallMap = (PXFileIOInfoAllowMapping & pxFileIOInfo->FlagList) && PXFileMappingAllow;

                    if(!shallMap)
                    {
                        return PXActionSuccessful; // No mapping attempt, we are done
                    }

                    // Attempt memory mapping...
                    PXActionResult mappingResult = PXFileMapToMemoryEE(pxFile, 0, pxFileIOInfo->AccessMode, PXTrue);


                    PXPerformanceInfoGet(&pxPerformanceInfo);
#if PXLogEnable

                    PXI64U bps = pxFile->DataUsed / pxPerformanceInfo.TimeDelta;
                    PXText pxTextbps;
                    PXTextConstructNamedBufferA(&pxTextbps, pxTextbpsBuffer, 32);
                    PXTextFormatSize(&pxTextbps, bps);

                    char permissionText[8];

                    PXAccessModeToStringA(permissionText, pxFileIOInfo->AccessMode);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "File",
                        "Mapping",
                        "OK, %s -> <%p> with %s/s for <%s>",
                        permissionText,
                        pxFile->Data,
                        pxTextbps.A,
                        pxFileIOInfo->FilePathAdress
                    );
#endif

                    // Tell the system we will need this data, so it can prefetch
             
                }
            }


#if 0
            const int nHandle = _open_osfhandle((intptr_t)pxFile->FileID, osHandleMode);
                FILE* fp = _fdopen(nHandle, fdOpenMode);

#endif

            break;
        }
        case PXFileIOInfoFileVirtual:
        {
            pxFile->Data = PXMemoryVirtualAllocate(pxFileIOInfo->FileSizeRequest, &pxFile->DataAllocated, PXAccessModeReadAndWrite);
            pxFile->DataUsed = pxFile->DataAllocated;
            pxFile->LocationMode = PXFileLocationModeMappedVirtual;

            break;
        }
        case PXFileIOInfoFileTemp:
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


            break;
        }
        case PXFileIOInfoFileMemory:
        {
            const PXBool hasSource = pxFileIOInfo->BufferData > 0;

            pxFile->AccessMode = pxFileIOInfo->AccessMode;

            if(hasSource)
            {
                pxFile->Data = pxFileIOInfo->BufferData;
                pxFile->DataAllocated = pxFileIOInfo->BufferSize;
                pxFile->LocationMode = PXFileLocationModeExternal;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "File",
                    "Open",
                    "Binding external memory <%p> %i B",
                    pxFile->Data,
                    pxFile->DataAllocated
                );
#endif
            }
            else
            {
                pxFile->Data = PXMemoryHeapCallocT(PXByte, pxFileIOInfo->FileSizeRequest);
                pxFile->DataAllocated = pxFileIOInfo->FileSizeRequest;
                pxFile->LocationMode = PXFileLocationModeInternal;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "File",
                    "Open",
                    "Created intermal memory <%p> %i B",
                    pxFile->Data,
                    pxFile->DataAllocated
                );
#endif
            }

            pxFile->DataUsed = pxFile->DataAllocated;

            break;
        }

        default:
            return PXActionInvalid;
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXFileClose(PXFile* const pxFile)
{
    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeMappedFromDisk:
            PXFileUnmapFromMemory(pxFile);
            break;

        case PXFileLocationModeDirectCached:
        case PXFileLocationModeInternal:
            PXMemoryHeapFree(PXNull, pxFile->Data);
            break;

        case PXFileLocationModeMappedVirtual:
            PXMemoryVirtualRelease(pxFile->Data, pxFile->DataAllocated);
            break;

        case PXFileLocationModeExternal:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "File",
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

    return PXActionSuccessful;
}

PXResult PXAPI  PXFileMapToMemory(PXFile* const pxFile, const PXSize size, const PXAccessMode protectionMode)
{
    void* const data = PXMemoryHeapMalloc(PXNull, size);
    const PXBool successful = data != 0;

    if(!successful)
    {
        return PXActionFailedMemoryAllocation;
    }

    pxFile->AccessMode = protectionMode;
    pxFile->LocationMode = PXFileLocationModeMappedVirtual;
    pxFile->Data = data;
    pxFile->DataUsed = size;

    return PXActionSuccessful;
}

PXResult PXAPI  PXFileUnmapFromMemory(PXFile* const pxFile)
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
        const int unmapResultID = munmap(pxFile->Data, pxFile->DataCursor);
        const PXActionResult unmapResult = PXErrorCurrent(0 == unmapResultID);

        if(PXActionSuccessful != unmapResult)
        {
            return unmapResult;
        }

        pxFile->Data = PXNull;
        pxFile->DataCursor = 0;
        pxFile->DataUsed = 0;
        pxFile->DataAllocated = 0;
    }

    // Release
    {
        const int closeResultID = fclose(pxFile->FileID);
        const PXActionResult closeResult = PXErrorCurrent(0 == closeResultID);


        return closeResult;
    }

#elif OSWindows

    // undo filemapping
    {
#if PXLogEnable

        PXText pxTextFilePath;
        PXTextConstructNamedBufferA(&pxTextFilePath, pxTextFilePathBuffer, PXPathSizeMax);

        PXFilePathGet(pxFile, &pxTextFilePath);

        PXText pxText;
        PXTextConstructNamedBufferA(&pxText, pxTextBuffer, 260);

        PXTextFormatSize(&pxText, pxFile->DataAllocated);

        PXLogPrint
        (
            PXLoggingDeallocation,
            "File",
            "MMAP-Release",
            "\n"
            "%20s : %i\n"
            "%20s : %s\n"
            "%20s : <%p> %i/%i\n"
            "%20s : <%s>",
            "Mapping-ID", (int)pxFile->MappingHandle,
            "Size", pxText.A,
            "Data", pxFile->Data, pxFile->DataCursor, pxFile->DataUsed,
            "Path", pxTextFilePath.A
        );
#endif

        // Write pending data
        {
            if(isWriteMapped)
            {
                const BOOL flushSuccessful = FlushViewOfFile(pxFile->Data, pxFile->DataCursor);
                const PXActionResult pxActionResult = PXErrorCurrent(flushSuccessful);

                if(PXActionSuccessful != pxActionResult)
                {
                    return pxActionResult;
                }
            }
        }

        {
            const PXBool unmappingSucessful = UnmapViewOfFile(pxFile->Data);
            const PXActionResult pxActionResult = PXErrorCurrent(unmappingSucessful);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            pxFile->Data = PXNull;
        }

        {
            const PXBool closeMappingSucessful = CloseHandle(pxFile->MappingHandle);
            const PXActionResult pxActionResult = PXErrorCurrent(closeMappingSucessful);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
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

            largeInteger.QuadPart = pxFile->DataCursor;

            const BOOL setSuccessful = SetFilePointerEx(pxFile->FileHandle, largeInteger, 0, FILE_BEGIN);
            const BOOL endSuccessful = SetEndOfFile(pxFile->FileHandle);
        }
    }

    // Close file
    {
        //      _fclose_nolock(pxFile->FileHandleCStyle);

        const PXBool successful = CloseHandle(pxFile->FileHandle); // Windows 2000 (+UWP), Kernel32.dll, handleapi.h
        const PXActionResult closeResult = PXErrorCurrent(successful);

        if(PXActionSuccessful != closeResult)
        {
            return closeResult;
        }

        pxFile->FileHandle = PXNull;
    }

    // CLEAR ALL
    //PXClear(PXFile, pxFile);

    return PXActionSuccessful;
#endif
}

PXSize PXAPI PXFileRemainingSize(const PXFile* const pxFile)
{
    return pxFile->DataUsed - pxFile->DataCursor;
}

PXSize PXAPI PXFileRemainingSizeRelativeFromAddress(const PXFile* const pxFile, const void* const adress)
{
    return ((PXSize)pxFile->Data - (PXSize)adress) - pxFile->DataUsed;
}

PXBool PXAPI PXFileIsAtEnd(const PXFile* const pxFile)
{
    return pxFile->DataCursor >= pxFile->DataUsed;
}

PXBool PXAPI PXFileAssureFreeSize(PXFile* const pxFile, const PXSize amount)
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
            pxFile->DataAllocated = pxFile->DataUsed * 2;
            pxFile->DataUsed = pxFile->DataAllocated;
            pxFile->Data = PXMemoryVirtualReallocate(pxFile->Data, pxFile->DataUsed);

            success = pxFile->Data > 0;

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
            );

            pxFile->DataAllocated = amount;

            break;
        }
        case PXFileLocationModeDirectCached:
            break;
    }

    return success;
}

PXSize PXAPI PXFileFindEndOfText(PXFile* const pxFile)
{
    const void* const startIndex = PXFileCursorPosition(pxFile);
    const void* const targetAdress = PXMemoryLocateFirst(pxFile->Data, '\0', pxFile->DataUsed);
    const PXSize offset = (PXSize)targetAdress - (PXSize)startIndex;

    return offset;
}

void* PXAPI PXFileCursorPosition(PXFile* const pxFile)
{
    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal: // Memory is handled internally.
        case PXFileLocationModeExternal: // Memory is stored outside this object
        case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
        case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
            return (PXAdress)pxFile->Data + pxFile->DataCursor;

        case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
        case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
        {
#if OSUnix
            pxFile->DataCursor = ftell(pxFile->FileID);
#elif OSWindows
            // Compile-ERROR: This seems to upset the compiler as its limited to 32-Bit value
            //pxFile->DataCursor = SetFilePointer(pxFile->FileID, 0, 0, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h
            const LARGE_INTEGER distanceToMove = { 0,0 }; // We do not move
            LARGE_INTEGER newFilePointer;

            const PXBool result = SetFilePointerEx(pxFile->FileHandle, distanceToMove, &newFilePointer, FILE_CURRENT); // Windows XP (+UWP), Kernel32.dll, fileapi.h

            if(result)
            {
                pxFile->DataCursor = newFilePointer.QuadPart;
            }
#endif

        }
        default:
            return PXFileCursorPositionInvalid;
    }
}

void PXAPI PXFileCursorMoveTo(PXFile* const pxFile, const PXSize position)
{
    const PXSize minimalInBoundsPosition = PXMathMinimumIU(pxFile->DataUsed, position); // Prevent cursor from going out of bounce

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal: // Memory is handled internally.
        case PXFileLocationModeExternal: // Memory is stored outside this object
        case PXFileLocationModeMappedVirtual: // Used 'VirtalAlloc()' / 'mmap()'
        case PXFileLocationModeMappedFromDisk: // Used 'FileView()' / 'fmap()'
        {
            pxFile->DataCursor = minimalInBoundsPosition;
            break;
        }

        case PXFileLocationModeDirectCached: // Read & Write operations are cached into a buffer first.
        case PXFileLocationModeDirectUncached: // Read & Write operations are directly put into
        {
#if OSUnix
            pxFile->DataCursor = ftell(pxFile->FileID);
#elif OSWindows

#if OS32Bit
            LONG sizeLow = position;
            LONG sizeHigh = 0;
#else OS64Bit

            LONG sizeLow = position & 0x00000000FFFFFFFF;
            LONG sizeHigh = (position & 0xFFFFFFFF00000000) >> 32;
#endif

            pxFile->DataCursor = SetFilePointer(pxFile->FileHandle, sizeLow, &sizeHigh, FILE_BEGIN); // Windows XP, Kernel32.dll, fileapi.h

#endif
            break;
        }
    }
}

void PXAPI PXFileCursorToBeginning(PXFile* const pxFile)
{
#if OSUnix
#elif OSWindows
    const BOOL flushSuccessful = FlushFileBuffers(pxFile->FileHandle); // Windows XP (+UWP), Kernel32.dll, fileapi.h
#endif

    PXFileCursorMoveTo(pxFile, 0);
}

PXSize PXAPI PXFileCursorPeek(PXFile* const pxFile, void* const data, const PXSize steps)
{
    const PXSize readBytes = PXFileReadB(pxFile, data, steps);

    PXFileCursorRewind(pxFile, readBytes);

    return readBytes;
}

PXSize PXAPI PXFileCursorAdvance(PXFile* const pxFile, const PXSize steps)
{
    const PXSize before = pxFile->DataCursor;
    PXFileCursorMoveTo(pxFile, pxFile->DataCursor + steps);
    const PXSize delta = pxFile->DataCursor - before;

    //assert(delta != (PXSize)-1);

    return delta;
}

PXSize PXAPI PXFileCursorRewind(PXFile* const pxFile, const PXSize steps)
{
    if(pxFile->DataCursor <= steps)
    {
        PXFileCursorMoveTo(pxFile, 0);


        return steps - pxFile->DataCursor;
    }

    const PXSize stepsTaken = pxFile->DataCursor - steps;

    PXFileCursorMoveTo(pxFile, stepsTaken); // Check underflow

    return steps;
}

PXSize PXAPI PXFileCursorOffset(PXFile* const pxFile, const PXI32S offset)
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

void PXAPI PXFileCursorToEnd(PXFile* const pxFile)
{
    PXFileCursorMoveTo(pxFile, pxFile->DataCursor);
}

PXSize PXAPI PXFileSkipEndOfLineCharacters(PXFile* const pxFile)
{
    const PXSize oldPosition = pxFile->DataCursor;

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

    return pxFile->DataCursor - oldPosition;
}

PXSize PXAPI PXFileSkipEmptySpace(PXFile* const pxFile)
{
    const PXSize dataPositionBefore = pxFile->DataCursor;

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

    return pxFile->DataCursor - dataPositionBefore;
}

PXSize PXAPI PXFileReadNextLineInto(PXFile* const pxFile, void* exportBuffer, const PXSize exportBufferSize)
{
    const PXSize dataPositionBefore = pxFile->DataCursor;

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

    const char* dataPoint = (char*)pxFile->Data + dataPositionBefore;
    const PXSize dataPositionAfter = pxFile->DataCursor;
    const PXSize length = dataPositionAfter - dataPositionBefore;

    if(length == 0)
    {
        return 0;
    }

    PXTextCopyA(dataPoint, length, (char*)exportBuffer, length);

    PXFileSkipEndOfLineCharacters(pxFile);

    return length;
}

PXSize PXAPI PXFileSkipBlock(PXFile* const pxFile)
{
    const PXSize oldPosition = pxFile->DataCursor;

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

    return pxFile->DataCursor - oldPosition;
}

PXSize PXAPI PXFileSkipToNextBlock(PXFile* const pxFile)
{
    const PXSize oldPosition = pxFile->DataCursor;

    PXFileSkipBlock(pxFile);

    PXFileSkipEmptySpace(pxFile);

    return pxFile->DataCursor - oldPosition;
}

PXSize PXAPI PXFileSkipLine(PXFile* const pxFile)
{
    const PXSize positionBefore = pxFile->DataCursor;

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

    const PXSize skippedBytes = pxFile->DataCursor - positionBefore;

    return skippedBytes;
}

PXSize PXAPI PXFileCursorPositionTerminate(PXFile* const pxFile)
{
    pxFile->DataUsed = pxFile->DataCursor;
    pxFile->DataCursor = 0;

    return pxFile->DataUsed;
}

PXSize PXAPI PXFileDataCopy(PXFile* const pxInputStream, PXFile* const pxOutputStream, const PXSize length)
{
    void* const insertionPoint = PXFileCursorPosition(pxOutputStream); // Position of output
    const PXSize copyedBytes = PXFileReadB(pxInputStream, insertionPoint, length); // Copy from input to output
    const PXSize movedBytes = PXFileCursorAdvance(pxOutputStream, copyedBytes); // Move output by copyed amount

    return movedBytes; // return moved amount of bytes
}

PXSize PXAPI PXFileReadTextIU8(PXFile* const pxFile, PXI8U* const number)
{
    int value = 0;
    const PXSize size = PXFileReadTextI(pxFile, &value);

    *number = value;

    return size;
}

PXSize PXAPI PXFileReadTextI(PXFile* const pxFile, int* const number)
{
    PXText pxText;
    pxText.SizeAllocated = PXFileRemainingSize(pxFile);
    pxText.SizeUsed = PXFileRemainingSize(pxFile);
    pxText.NumberOfCharacters = PXFileRemainingSize(pxFile);
    pxText.A = (char*)PXFileCursorPosition(pxFile);
    pxText.Format = TextFormatASCII;

    pxText.SizeUsed = PXTextToInt(&pxText, number);

    PXFileCursorAdvance(pxFile, pxText.SizeUsed);

    return pxText.SizeUsed;
}

PXSize PXAPI PXFileReadType(PXFile* const pxFile, void* const valueAdress, const PXI32U type)
{
    char buffer[32];

    const PXSize amount = PXFileReadIXXE
    (
        pxFile, 
        buffer,
        PXTypeSizeGet(type),
        PXTypeEndianGet(type)
    );

    PXMemoryCopyX(buffer, PXTypeSizeGet(type), valueAdress, PXTypeReciverSizeGet(type));
}

PXSize PXAPI PXFileReadIXXE(PXFile* const pxFile, void* const valueAdress, const PXSize valueSize, const PXEndian pxEndian)
{
    const PXSize writtenBytes = PXFileReadB(pxFile, valueAdress, valueSize);

    PXEndianSwap(valueAdress, valueSize, pxEndian, EndianCurrentSystem);

    return writtenBytes;
}

PXSize PXAPI PXFileReadIXXVE(PXFile* const pxFile, void** const valueList, const PXSize valueListSize, const PXSize valueSizeSingle, const PXEndian pxEndian)
{
    PXSize writtenBytes = 0;

    if(!valueList)
    {
        return 0;
    }

    for(PXSize i = 0; i < valueListSize; i++)
    {
        void* const adress = valueList[i];

        writtenBytes = PXFileReadB(pxFile, adress, valueSizeSingle);
    }

    PXEndianSwapV(valueList, valueListSize * valueSizeSingle, valueSizeSingle, pxEndian, EndianCurrentSystem);

    return writtenBytes;
}

PXSize PXAPI PXFileReadI8S(PXFile* const pxFile, PXI8S* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI8S));
}

PXSize PXAPI PXFileReadI8SV(PXFile* const pxFile, PXI8S* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI8S) * valueListSize);
}

PXSize PXAPI PXFileReadI8U(PXFile* const pxFile, PXI8U* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI8U));
}

PXSize PXAPI PXFileReadI8UV(PXFile* const pxFile, PXI8U* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI8U) * valueListSize);
}

PXSize PXAPI PXFileReadI16S(PXFile* const pxFile, PXI16S* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI16S));
}

PXSize PXAPI PXFileReadI16SV(PXFile* const pxFile, PXI16S* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI16S) * valueListSize);
}

PXSize PXAPI PXFileReadI16SE(PXFile* const pxFile, PXI16S* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI16S), pxEndian);
}

PXSize PXAPI PXFileReadI16SVE(PXFile* const pxFile, PXI16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI16S), pxEndian);
}

PXSize PXAPI PXFileReadI16U(PXFile* const pxFile, PXI16U* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI16U));
}

PXSize PXAPI PXFileReadI16UV(PXFile* const pxFile, PXI16U* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI16U) * valueListSize);
}

PXSize PXAPI PXFileReadI16UE(PXFile* const pxFile, PXI16U* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI16U), pxEndian);
}

PXSize PXAPI PXFileReadI16UVE(PXFile* const pxFile, PXI16U** const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI16U), pxEndian);
}

PXSize PXAPI PXFileReadI32S(PXFile* const pxFile, PXI32S* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI32S));
}

PXSize PXAPI PXFileReadI32SV(PXFile* const pxFile, PXI32S* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI32S) * valueListSize);
}

PXSize PXAPI PXFileReadI32SE(PXFile* const pxFile, PXI32S* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI32S), pxEndian);
}

PXSize PXAPI PXFileReadI32SVE(PXFile* const pxFile, PXI32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI32S), pxEndian);
}

PXSize PXAPI PXFileReadI32U(PXFile* const pxFile, PXI32U* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI32U));
}

PXSize PXAPI PXFileReadI32UV(PXFile* const pxFile, PXI32U* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI32U) * valueListSize);
}

PXSize PXAPI PXFileReadI32UE(PXFile* const pxFile, PXI32U* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI32U), pxEndian);
}

PXSize PXAPI PXFileReadI32UVE(PXFile* const pxFile, PXI32U** const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI32U), pxEndian);
}

PXSize PXAPI PXFileReadI64S(PXFile* const pxFile, PXI64S* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI64S));
}

PXSize PXAPI PXFileReadI64SV(PXFile* const pxFile, PXI64S* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI64S) * valueListSize);
}

PXSize PXAPI PXFileReadI64SE(PXFile* const pxFile, PXI64S* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI64S), pxEndian);
}

PXSize PXAPI PXFileReadI64VE(PXFile* const pxFile, PXI64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI64S), pxEndian);
}

PXSize PXAPI PXFileReadI64U(PXFile* const pxFile, PXI64U* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXI64U));
}

PXSize PXAPI PXFileReadI64UV(PXFile* const pxFile, PXI64U* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXI64U) * valueListSize);
}

PXSize PXAPI PXFileReadI64UE(PXFile* const pxFile, PXI64U* const value, const PXEndian pxEndian)
{
    return PXFileReadIXXE(pxFile, value, sizeof(PXI64U), pxEndian);
}

PXSize PXAPI PXFileReadI64UVE(PXFile* const pxFile, PXI64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    return PXFileReadIXXVE(pxFile, valueList, valueListSize, sizeof(PXI64U), pxEndian);
}

PXSize PXAPI PXFileReadF(PXFile* const pxFile, PXF32* const value)
{
    return PXFileReadB(pxFile, value, sizeof(PXF32));
}

PXSize PXAPI PXFileReadFV(PXFile* const pxFile, PXF32* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(PXF32) * valueListSize);
}

PXSize PXAPI PXFileReadD(PXFile* const pxFile, double* const value)
{
    return PXFileReadB(pxFile, value, sizeof(double));
}

PXSize PXAPI PXFileReadDV(PXFile* const pxFile, double* const valueList, const PXSize valueListSize)
{
    return PXFileReadB(pxFile, valueList, sizeof(double) * valueListSize);
}

PXSize PXAPI PXFileBinding(PXFile* const pxFile, void* const dataStruct, const PXI32U* listOfTypes, const PXSize listOfTypesAmount, const PXI8U flags)
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

PXSize PXAPI PXFileReadMultible(PXFile* const pxFile, const PXTypeEntry* const pxFileElementList, const PXSize pxFileElementListFullSize)
{
    return PXFileIOMultible(pxFile, pxFileElementList, pxFileElementListFullSize, PXFileReadB);
}

PXSize PXAPI PXFileDataWidthCalculate(PXFile* const pxFile, const PXI32U type)
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

PXSize PXAPI PXFileIOMultible(PXFile* const pxFile, const PXTypeEntry* const pxFileElementList, const PXSize pxFileElementListFullSize, PXFileIOMultibleFunction pxFileIOMultibleFunction)
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
            const PXTypeEntry* const pxFileDataElementType = &pxFileElementList[i];
            const PXSize sizeOfType =
                (!(pxFileDataElementType->Type & PXTypeAdressMask) * (pxFileDataElementType->Type & PXTypeSizeMask)) +
                (((pxFileDataElementType->Type & PXTypeAdressMask) && !(pxFileDataElementType->Type & PXTypeIgnoreIn32B) && (pxFile->BitFormatOfData == PXBitFormat32)) * 4u) +
                (((pxFileDataElementType->Type & PXTypeAdressMask) && !(pxFileDataElementType->Type & PXTypeIgnoreIn64B) && (pxFile->BitFormatOfData == PXBitFormat64)) * 8u);

            PXMemoryClear(pxFileDataElementType->Adress, sizeOfType);

            //assert(sizeOfType > 0);

            totalSizeToRead += sizeOfType;
        }

        void* stackMemory = PXMemoryHeapCallocT(char, totalSizeToRead);

        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileOpenInfo.BufferData = stackMemory;
        pxFileOpenInfo.BufferSize = totalSizeToRead;

        const PXActionResult fileOpenResult = PXFileOpen(&pxStackFile, &pxFileOpenInfo);

        PXFileDataCopy(pxFile, &pxStackFile, totalSizeToRead); // Read actual data all at once
        pxStackFile.DataCursor = 0;

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
        const PXTypeEntry* const pxFileDataElementType = &pxFileElementList[i];
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

PXSize PXAPI PXFileReadB(PXFile* const pxFile, void* const value, const PXSize length)
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
            const void* currentPosition = PXFileCursorPosition(pxFile);
            const PXSize moveSize = PXFileCursorAdvance(pxFile, length);

            PXMemoryCopy(currentPosition, value, moveSize);

            return moveSize;
        }
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
        {
            if(pxFile->DataCursor >= pxFile->DataUsed)
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
                    return 0; // PXActionSuccessful;
                }

                // Read error
                const int hasError = ferror(pxFile->FileID);

                return 0; // PXActionFailedRead;
            }

            pxFile->DataCursor += writtenBytes;

            return writtenBytes;

#elif OSWindows
            DWORD writtenBytes = 0;
            const PXBool success = ReadFile(pxFile->FileHandle, value, length, &writtenBytes, PXNull);
            const PXActionResult pxActionResult = PXErrorCurrent(success);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
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

            pxFile->DataCursor += writtenBytes;

            return writtenBytes;
#endif
        }
    }

    return 0;
}

PXSize PXAPI PXFileReadA(PXFile* const pxFile, PXASCII* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

PXSize PXAPI PXFileReadW(PXFile* const pxFile, PXUNICODE* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

PXSize PXAPI PXFileTextU(PXFile* const pxFile, PXUTF8* value, const PXSize length)
{
    const PXSize size = PXFileReadB(pxFile, value, length);

    value[size] = 0;

    return size;
}

void PXAPI PXFileReadUntil(PXFile* const pxFile, void* value, const PXSize length, const char character)
{
    const char* currentPosition = (char*)PXFileCursorPosition(pxFile);

    PXSize lengthCopy = 0;

    while(!PXFileIsAtEnd(pxFile))
    {
        const char* data = (char*)PXFileCursorPosition(pxFile);
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

PXSize PXAPI PXFileByteSwap(PXFile* const pxFileTarget, PXFile* const pxFileSource)
{
    pxFileTarget->DataUsed = PXFileReadB(pxFileSource, pxFileTarget->Data, pxFileSource->DataAllocated);

    for(PXSize i = 0; i < pxFileTarget->DataUsed; i += 2)
    {
        PXI8U* cursor = &((PXI8U*)pxFileTarget->Data)[i];
        PXI8U* cursorA = &cursor[0];
        PXI8U* cursorB = &cursor[1];

        PXI8U c = *cursorA;
        *cursorA = *cursorB;
        *cursorB = c;
    }

    return 0;
}

PXBool PXAPI PXFileReadAndCompareI64U(PXFile* const pxFile, const PXI64U value)
{
    PXI64U valueA = 0;
    const PXI64U valueSize = sizeof(PXI64U);
    const PXSize readBytes = PXFileReadB(pxFile, &valueA, valueSize);
    const PXBool successful = readBytes == valueSize;

    return successful;
}

PXBool PXAPI PXFileReadAndCompare(PXFile* const pxFile, const void* value, const PXSize length)
{
    const void* currentPosition = PXFileCursorPosition(pxFile);
    const PXSize readableSize = PXFileRemainingSize(pxFile);

    const PXBool result = PXMemoryCompare(currentPosition, readableSize, value, length);

    if(result)
    {
        PXFileCursorAdvance(pxFile, length);
    }

    return result;
}

PXBool PXAPI PXFileReadAndCompareV(PXFile* const pxFile, const void** const value, const PXSize* const valueElementSizeList, const PXSize valueLength)
{
    const void* const currentPosition = PXFileCursorPosition(pxFile);
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

PXSize PXAPI PXFileWriteI8S(PXFile* const pxFile, const PXI8S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI8S));
}

PXSize PXAPI PXFileWriteI8SV(PXFile* const pxFile, const PXI8S* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI8S) * valueListSize);
}

PXSize PXAPI PXFileWriteI8U(PXFile* const pxFile, const PXI8U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI8U));
}

PXSize PXAPI PXFileWriteAtI8U(PXFile* const pxFile, const PXI8U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->DataCursor; // save current position

    pxFile->DataCursor = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI8U(pxFile, value); // Length

    pxFile->DataCursor = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI8UV(PXFile* const pxFile, const PXI8U* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI8U) * valueListSize);
}

PXSize PXAPI PXFileWriteI16S(PXFile* const pxFile, const PXI16S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI16S));
}

PXSize PXAPI PXFileWriteI16SV(PXFile* const pxFile, const PXI16S* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI16S) * valueListSize);
}

PXSize PXAPI PXFileWriteI16SE(PXFile* const pxFile, const PXI16S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI16S);
    PXI16S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI16SVE(PXFile* const pxFile, const PXI16S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI16SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI16U(PXFile* const pxFile, const PXI16U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI16U));
}

PXSize PXAPI PXFileWriteAtI16U(PXFile* const pxFile, const PXI16U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->DataCursor; // save current position

    pxFile->DataCursor = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI16U(pxFile, value); // Length

    pxFile->DataCursor = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI16UV(PXFile* const pxFile, const PXI16U* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI16U) * valueListSize);
}

PXSize PXAPI PXFileWriteI16UE(PXFile* const pxFile, const PXI16U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI16U);
    PXI16U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI16UVE(PXFile* const pxFile, const PXI16U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI16UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI32S(PXFile* const pxFile, const PXI32S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI32S));
}

PXSize PXAPI PXFileWriteI32SV(PXFile* const pxFile, const PXI32S* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI32S) * valueListSize);
}

PXSize PXAPI PXFileWriteI32SE(PXFile* const pxFile, const PXI32S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI32S);
    PXI32S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI32SVE(PXFile* const pxFile, const PXI32S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI32SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI32U(PXFile* const pxFile, const PXI32U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI32U));
}

PXSize PXAPI PXFileWriteAtI32U(PXFile* const pxFile, const PXI32U value, const PXSize index)
{
    const PXSize positionBefore = pxFile->DataCursor; // save current position

    pxFile->DataCursor = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI32U(pxFile, value); // Length

    pxFile->DataCursor = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI32UV(PXFile* const pxFile, const PXI32U* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI32U) * valueListSize);
}

PXSize PXAPI PXFileWriteI32UE(PXFile* const pxFile, const PXI32U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI32U);
    PXI32U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteAtI32UE(PXFile* const pxFile, const PXI32U value, const PXEndian pxEndian, const PXSize index)
{
    const PXSize positionBefore = pxFile->DataCursor; // save current position

    pxFile->DataCursor = index; // jump to offset

    const PXSize writtenBytes = PXFileWriteI32UE(pxFile, value, pxEndian); // Length

    pxFile->DataCursor = positionBefore; // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteI32UVE(PXFile* const pxFile, const PXI32U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI32UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI64S(PXFile* const pxFile, const PXI64S value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI64S));
}

PXSize PXAPI PXFileWriteI64SV(PXFile* const pxFile, const PXI64S* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI64S) * valueListSize);
}

PXSize PXAPI PXFileWriteI64SE(PXFile* const pxFile, const PXI64S value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI64S);
    PXI64S dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI64VE(PXFile* const pxFile, const PXI64S* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI64SE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteI64U(PXFile* const pxFile, const PXI64U value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXI64U));
}

PXSize PXAPI PXFileWriteI64UV(PXFile* const pxFile, const PXI64U* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXI64U) * valueListSize);
}

PXSize PXAPI PXFileWriteI64UE(PXFile* const pxFile, const PXI64U value, const PXEndian pxEndian)
{
    const PXSize dataSize = sizeof(PXI64U);
    PXI64U dataValue = value;

    PXEndianSwap(&dataValue, dataSize, EndianCurrentSystem, pxEndian);

    PXFileWriteB(pxFile, &dataValue, dataSize);

    return dataSize;
}

PXSize PXAPI PXFileWriteI64UVE(PXFile* const pxFile, const PXI64U* const valueList, const PXSize valueListSize, const PXEndian pxEndian)
{
    PXSize accumulator = 0;

    for(PXSize i = 0; i < valueListSize; ++i)
    {
        accumulator += PXFileWriteI64UE(pxFile, valueList[i], pxEndian);
    }

    return accumulator;
}

PXSize PXAPI PXFileWriteF(PXFile* const pxFile, const PXF32 value)
{
    return PXFileWriteB(pxFile, &value, sizeof(PXF32));
}

PXSize PXAPI PXFileWriteFV(PXFile* const pxFile, const PXF32* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(PXF32) * valueListSize);
}

PXSize PXAPI PXFileWriteD(PXFile* const pxFile, const double value)
{
    return PXFileWriteB(pxFile, &value, sizeof(double));
}

PXSize PXAPI PXFileWriteDV(PXFile* const pxFile, const double* const valueList, const PXSize valueListSize)
{
    return PXFileWriteB(pxFile, valueList, sizeof(double) * valueListSize);
}

PXSize PXAPI PXFileWriteInternMove(PXFile* const pxFile, const PXI16S offset, const PXSize length)
{
    const PXSize oldPosition = pxFile->DataCursor;
    char* const destination = (char*)PXFileCursorPosition(pxFile);

    // Souce
    const PXSize steptsTaken = PXFileCursorRewind(pxFile, offset); // Safe rewind
    const char* const source = (const char*)PXFileCursorPosition(pxFile);


    // Destination
    pxFile->DataCursor = oldPosition; // restore

    if(steptsTaken != offset)
    {
        // FAILUE
        return 0;
    }

    PXSize amount = PXMemoryMove(source, length, destination, length);

    PXFileCursorAdvance(pxFile, length);

    return amount;
}

PXSize PXAPI PXFileWriteB(PXFile* const pxFile, const void* const value, const PXSize length)
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
            void* const currentPosition = PXFileCursorPosition(pxFile);
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
            pxFile->DataCursor += writtenBytes;
            pxFile->DataUsed += writtenBytes;

            return writtenBytes;

#endif
        }

        default:
            return 0;
    }
}

PXSize PXAPI PXFileWriteAtB(PXFile* const pxFile, const void* const data, const PXSize dataSize, const PXSize index)
{
    const PXSize positionBefore = pxFile->DataCursor; // save current position

    PXFileCursorMoveTo(pxFile, index); // jump to offset

    const PXSize writtenBytes = PXFileWriteB(pxFile, data, dataSize); // Length

    PXFileCursorMoveTo(pxFile, positionBefore); // Reset old position

    return writtenBytes;
}

PXSize PXAPI PXFileWriteMultible(PXFile* const pxFile, const PXTypeEntry* const pxFileDataElementTypeList, const PXSize pxFileElementListFullSize)
{
    return PXFileIOMultible(pxFile, pxFileDataElementTypeList, pxFileElementListFullSize, PXFileWriteB);
}

PXSize PXAPI PXFileWriteFill(PXFile* const pxFile, const PXByte value, const PXSize length)
{
    if((!pxFile) || (length == 0))
    {
        return 0;
    }

    
    char buffer[64];
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

PXSize PXAPI PXFileWriteNewLine(PXFile* const pxFile)
{
    return PXFileWriteB(pxFile, "\n", 1);
}

PXSize PXAPI PXFileWriteC(PXFile* const pxFile, const char character)
{
    return PXFileWriteB(pxFile, &character, 1);
}

PXSize PXAPI PXFileWriteA(PXFile* const pxFile, const char* const text, const PXSize textSize)
{
#if 1
    return PXFileWriteB(pxFile, text, textSize);
#else
    const PXSize writableSize = PXFileRemainingSize(pxFile);
    char* const currentPosition = (char* const)PXFileCursorPosition(pxFile);

    const PXSize writtenBytes = PXTextCopyA(text, textSize, currentPosition, writableSize);

    PXFileCursorAdvance(pxFile, writtenBytes);

    return writtenBytes;
#endif
}

PXSize PXAPI PXFileWriteW(PXFile* const pxFile, const wchar_t* const text, const PXSize textSize)
{
    const PXSize writableSize = PXFileRemainingSize(pxFile);
    wchar_t* const currentPosition = (wchar_t* const)PXFileCursorPosition(pxFile);

    const PXSize writtenBytes = PXTextCopyW(text, textSize, currentPosition, writableSize);

    PXFileCursorAdvance(pxFile, writtenBytes);

    return writtenBytes;
}

PXSize PXAPI PXFileWriteAF(PXFile* const pxFile, const char* const format, ...)
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
            void* const currentPosition = PXFileCursorPosition(pxFile);

            const PXSize writableSize = PXFileRemainingSize(pxFile);

            PXText pxText;
            PXTextConstructFromAdressA(&pxText, currentPosition, writableSize, writableSize);

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

            pxFile->DataAllocated += writtenBytes;
            pxFile->DataUsed += writtenBytes;

            PXFileWriteB(pxFile, buffer, writtenBytes);

            break;
        }
        default:
            break;
    }

    va_end(args);

    return writtenBytes;
}

PXSize PXAPI PXFileWriteWF(PXFile* const pxFile, const wchar_t* const format, ...)
{
    return  0;
}

PXResult PXAPI  PXFileStoreOnDiskA(PXFile* const pxFile, const char* fileName)
{
    HANDLE fileHandle = CreateFileA
    (
        fileName, 
        GENERIC_WRITE, 
        0,
        NULL, 
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL, 
        NULL
    );
    PXActionResult createResult = PXErrorCurrent(INVALID_HANDLE_VALUE != fileHandle);

    if(PXActionSuccessful != createResult)
    {
        return createResult;
    }

    DWORD writtenFiles = 0;

    WriteFile(fileHandle, pxFile->Data, pxFile->DataUsed, &writtenFiles, NULL);

    FlushFileBuffers(fileHandle);

    CloseHandle(fileHandle);

    return PXActionSuccessful;
}

PXSize PXAPI PXFileSkipBitsToNextByte(PXFile* const pxFile)
{
    const PXBool hasAnyBitConsumed = pxFile->DataCursorBitOffset > 0;

    if(!hasAnyBitConsumed)
    {
        return 0;
    }

    pxFile->DataCursorBitOffset = 0; // Reset

    return PXFileCursorAdvance(pxFile, 1); // Go 1 Byte further
}

PXSize PXAPI PXFileCursorMoveBits(PXFile* const pxFile, const PXSize amountOfBits)
{
    pxFile->DataCursorBitOffset += amountOfBits;

    PXFileBitsAllign(pxFile);

    return 0;
}

PXSize PXAPI PXFileBitsAllign(PXFile* const pxFile)
{
    PXSize accumulator = 0;

    while(pxFile->DataCursorBitOffset >= 8u) // Move a Byte__ at the time forward, 8 Bits = 1 Byte__.
    {
        pxFile->DataCursor++;
        pxFile->DataCursorBitOffset -= 8u;

        ++accumulator;
    }

    return accumulator;
}

PXSize PXAPI PXFilePeekBits(PXFile* const pxFile, const PXSize amountOfBits)
{
    unsigned int bitMask = ((1u << amountOfBits) - 1u) << pxFile->DataCursorBitOffset; // 0000111111
    unsigned int bitBlock;
    unsigned char* a = (unsigned char*)PXFileCursorPosition(pxFile);
    unsigned char* b = a + 1;
    unsigned char* c = a + 2;
    unsigned char* d = a + 3;
    unsigned char* maxAdress = (unsigned char*)pxFile->Data + (pxFile->DataUsed - 1);

    unsigned int ai = a > maxAdress ? 0 : *a;
    unsigned int bi = b > maxAdress ? 0 : *b;
    unsigned int ci = c > maxAdress ? 0 : *c;
    unsigned int di = d > maxAdress ? 0 : *d;

    // [d][c][b][a] Little pxEndian

    bitBlock = ai | bi << 8 | ci << 16 | di << 24;

    unsigned int result = bitBlock & bitMask;

#if PXFileDebugOutput && 0
    printf("Extract %i Bits. Byte__:%i Offset:%i\n", amountOfBits, pxFile->DataCursor, pxFile->BitOffset);
    printf("BitBlock : ");
    PrintBinary(bitBlock);
    printf("BitMask  : ");
    PrintBinary(bitMask);
    printf("Result   : ");
    //PrintBinary(result);
#endif

    result >>= pxFile->DataCursorBitOffset; // Shoitft correction

#if PXFileDebugOutput && 0
    printf("Shifted  : ");
    //PrintBinary(result);
#endif

    return result;
}

PXSize PXAPI PXFileReadBits(PXFile* const pxFile, const PXSize amountOfBits)
{
    const PXSize result = PXFilePeekBits(pxFile, amountOfBits);

    PXFileCursorMoveBits(pxFile, amountOfBits);

    return result;
}

PXSize PXAPI PXFileWriteBits(PXFile* const pxFile, const PXSize bitData, const PXSize amountOfBits)
{
    PXSize* const currentPos = (PXSize* const)PXFileCursorPosition(pxFile);
    PXSize bitBlockCache = 0;

    for(PXSize i = 0; i < amountOfBits; i++)
    {
        // Data = current bit << current offset
        bitBlockCache |= (bitData & ((PXSize)1u << i));
    }

    *currentPos |= bitBlockCache << pxFile->DataCursorBitOffset;
    pxFile->DataCursorBitOffset += amountOfBits;

    const PXSize movedBytes = PXFileBitsAllign(pxFile);

    return amountOfBits;
}

PXResult PXAPI  PXFileTimeGet
(
    const PXFile* const pxFile,
    PXTime* const pxTimeCreation,
    PXTime* const pxTimeAccessLast,
    PXTime* const pxTimeWriteLast
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
        const PXActionResult pxActionResult = PXErrorCurrent(result);

        if(PXActionSuccessful != pxActionResult)
        {
            return pxActionResult;
        }
    }

    // Convert
    {
        PXTime* const pxTimeList[3] = { pxTimeCreation, pxTimeAccessLast, pxTimeWriteLast };
        SYSTEMTIME systemTimeList[3];

        for(PXI8U i = 0; i < 3; ++i)
        {
            FileTimeToSystemTime(&fileTimeList[i], &systemTimeList[i]); // Windows XP (+UWP), Kernel32.dll, timezoneapi.h
            PXTimeConvertFromOS(pxTimeList[i], &systemTimeList[i]);
        }
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI  PXFilePathSet(PXFile* const pxFile, const PXText* const filePath)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista // On Vista we can always store. No Store needed
    return PXActionSuccessful;
#elif WindowsAtleastXP // On windows XP only if beeing mapped.
    const PXBool isMapped = PXNull != pxFile->MappingHandle;

    if(isMapped)
    {
        return PXActionSuccessful;
    }
#endif
#endif
    // TODO:
    //PXTextCreateCopy(&pxFile->FilePath, filePath);

    return PXActionSuccessful;
}

PXResult PXAPI  PXFilePathGet(const PXFile* const pxFile, PXText* const filePath)
{
    PXActionResult pxActionResult = PXActionInvalid;

    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
        {
            // As we are not mapped, loaded the file into direct memory, there is no file attached.

            filePath->A = pxFile->FilePathData;
            filePath->SizeUsed = pxFile->FilePathSize;
            filePath->SizeAllocated = pxFile->FilePathSize;
            filePath->Format = TextFormatUTF8;

            pxActionResult = PXActionSuccessful;

            break;
        }
        case PXFileLocationModeMappedFromDisk:
        {
            // We can translate a handle into a filepath
            pxActionResult = PXFileNameViaHandleA(pxFile, filePath->A, filePath->SizeAllocated, &filePath->SizeUsed);
            break;
        }
        default:
            return PXActionInvalidStateImpossible;
    }

    return pxActionResult;
}

PXResult PXAPI  PXFilePathGetA(PXFile* const pxFile, char* const filePath, const PXSize filePathSize, PXSize* const sizeWritten)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, filePath, 0, filePathSize);

    PXActionResult res = PXFilePathGet(pxFile, &pxText);

    if(sizeWritten)
    {
        *sizeWritten = pxText.SizeUsed;
    }

    return res;
}

PXResult PXAPI  PXFIlePathGetLong(PXText* const pxTextInput, PXText* const pxTextOutput)
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

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}














/*

PXActionResult FileWorkingDirectoryChangeA(const char* directoryName)
{
    int creationResult = WorkingDirectoryChangeA(directoryName);
    bool wasSuccesful = creationResult == 0;

    if (!wasSuccesful)
    {
        return GetCurrentError();
    }

    return ErrorCode::Successful;
}

PXActionResult FileWorkingDirectoryChangeW(const wchar_t* directoryName)
{
    int creationResult = WorkingDirectoryChangeW(directoryName);
    bool wasSuccesful = creationResult == 0;

    if (!wasSuccesful)
    {
        return GetCurrentError();
    }

    return ErrorCode::Successful;
}

PXActionResult FileDirectoryDeleteA(const char* directoryName)
{
    int creationResult = FileRemoveA(directoryName);
    bool wasSuccesful = creationResult == 0;

    if (!wasSuccesful)
    {
        return GetCurrentError();
    }

    return ErrorCode::Successful;
}

PXActionResult FileDirectoryDeleteW(const wchar_t* directoryName)
{
    int creationResult = FileRemoveW(directoryName);
    bool wasSuccesful = creationResult == 0;

    if (!wasSuccesful)
    {
        return GetCurrentError();
    }

    return ErrorCode::Successful;
}

PXActionResult FileMapToVirtualMemoryA(const char* filePath, const PXAccessMode protectionMode)
{
}

PXActionResult FileMapToVirtualMemoryW(const wchar_t* filePath, const PXAccessMode protectionMode)
{

}

PXActionResult FileMapToVirtualMemory(const PXSize size, const PXAccessMode protectionMode)
{

}

PXActionResult FileUnmapFromVirtualMemory()
{

}

PXActionResult FileReadFromDisk(const char* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
    File file;

    // Open file
    {
        const PXActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);
        const bool sucessful = result == PXActionSuccessful;

        if(!sucessful)
        {
            return result;
        }
    }

    // Read
    {
        const PXActionResult result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);
        const bool sucessful = result == PXActionSuccessful;

        if(!sucessful)
        {
            file.Close();
            return result;
        }
    }

    // Close
    {
        const PXActionResult result = file.Close();
        const bool sucessful = result == PXActionSuccessful;

        if(!sucessful)
        {
            return result;
        }
    }

    _fileLocation = FileLocation::CachedFromDisk;

    return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(const wchar_t* filePath, bool addNullTerminator, FilePersistence filePersistence)
{
    File file;
    PXActionResult result = file.Open(filePath, FileOpenMode::Read, FileCachingMode::Sequential);

    if(result != PXActionSuccessful)
    {
        return result;
    }

    result = file.ReadFromDisk(&Data, DataSize, addNullTerminator);

    if(result != PXActionSuccessful)
    {
        file.Close();
        return result;
    }

    result = file.Close();

    if(result != PXActionSuccessful)
    {
        return result;
    }

    _fileLocation = FileLocation::CachedFromDisk;

    return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(FILE* file, Byte__** targetBuffer, PXSize& bufferSize, bool addNullTerminator)
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

    PXSize readBytes = fread(dataBuffer, 1u, bufferSize, file);
    PXSize overAllocatedBytes = bufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

    assert(bufferSize == readBytes);

    return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(const wchar_t* filePath, Byte__** targetBuffer, PXSize& bufferSize, bool addNullTerminator, FilePersistence filePersistence)
{
    File file;
    PXActionResult result = file.Open(filePath, FileOpenMode::Read);

    if(result != PXActionSuccessful)
    {
        return result;
    }

    //result = ReadFromDisk(file.FileMarker, targetBuffer, bufferSize, addNullTerminator);

    file.ReadFromDisk(targetBuffer, bufferSize, addNullTerminator);

    result = file.Close();

    return PXActionSuccessful;
}

PXActionResult FileWriteToDisk(const bool value)
{
    return WriteToDisk(&value, sizeof(bool));
}

PXActionResult FileWriteToDisk(const char value)
{
    return WriteToDisk(&value, sizeof(char));
}

PXActionResult FileWriteToDisk(const unsigned char value)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const short value, const pxEndian pxEndian)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned short value, const pxEndian pxEndian)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const int value, const pxEndian pxEndian)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const unsigned int value, const pxEndian pxEndian)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const char* string, const PXSize length)
{
    return WriteToDisk(string, length);
}

PXActionResult FileWriteToDisk(const unsigned char* string, const PXSize length)
{
    return WriteToDisk(string, length);
}

PXActionResult FileWriteToDisk(const unsigned long long& value, const pxEndian pxEndian)
{
    return WriteToDisk(&value, sizeof(unsigned char));
}

PXActionResult FileWriteToDisk(const void* value, const PXSize length)
{
#if OSUnix
FILE* fileHandle = FileHandle;
#elif OSWindows
FILE* fileHandle = FileHandleCStyle;
#endif

    const PXSize writtenSize = fwrite(value, sizeof(Byte__), length, fileHandle);

    if(writtenSize > 0)
    {
        return PXActionSuccessful;
    }
    else
    {
        return ResultWriteFailure;
    }
}

PXActionResult FileWriteToDisk(const char* format, ...)
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

    return PXActionSuccessful;
}

PXActionResult FileWriteIntoFile(const void* data, const PXSize dataSize)
{
#if OSUnix
    PXSize writtenBytes = fwrite(data, sizeof(char), dataSize, FileHandle);
#elif OSWindows
    DWORD writtenBytes = 0;
    const bool successful = WriteFile(FileHandle, data, dataSize, &writtenBytes, nullptr);
#endif

    return PXActionResult();
}

PXActionResult FileWriteToDisk(const char* filePath, FilePersistence filePersistence)
{
    File file;
    PXActionResult filePXActionResult = file.Open(filePath, FileOpenMode::Write);

    if(filePXActionResult != PXActionSuccessful)
    {
        return filePXActionResult;
    }

    WriteIntoFile(Data, DataSize);

    filePXActionResult = file.Close();

    return PXActionSuccessful;
}

PXActionResult FileWriteToDisk(const wchar_t* filePath, FilePersistence filePersistence)
{
    File file;

    {
        const PXActionResult filePXActionResult = file.Open(filePath, FileOpenMode::Write);
        const bool sucessful = filePXActionResult == PXActionSuccessful;

        if(!sucessful)
        {
            return filePXActionResult;
        }
    }

#if OSUnix
    PXSize writtenBytes = fwrite(Data, sizeof(char), DataCursorPosition, file.FileHandle);
#elif OSWindows
    DWORD writtenBytes = 0;
    const bool successful = WriteFile(file.FileHandle, Data, DataCursor, &writtenBytes, nullptr);
#endif

    {
        const PXActionResult closeResult = file.Close();
        const bool sucessful = closeResult == PXActionSuccessful;

        if(!sucessful)
        {
            return closeResult;
        }
    }

    return PXActionSuccessful;
}

PXActionResult FileReadFromDisk(unsigned char** outPutBuffer, PXSize& outPutBufferSize, const bool addTerminatorByte)
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

    PXSize readBytes = fread(dataBuffer, 1u, outPutBufferSize, FileHandle);
    PXSize overAllocatedBytes = outPutBufferSize - readBytes; // if overAllocatedBytes > 0 there was a reading error.

    assert(outPutBufferSize == readBytes);

    return PXActionSuccessful;
#elif OSWindows
    const DWORD fileSize = GetFileSize(FileHandle, nullptr);
    DWORD numberOfBytesRead = 0;
    OVERLAPPED* overlapped = nullptr;
    //LARGE_INTEGER size;
    //bool succesSize = GetFileSizeEx(FileHandle, &size);
    PXSize allocationSize = fileSize;

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

    return PXActionSuccessful;
#endif
}

void FilePathSwapFile(const wchar_t* currnetPath, wchar_t* targetPath, const wchar_t* newFileName)
{
    const PXSize index = PXTextFindLastW(currnetPath, PXPathSizeMax, '/');
    const bool found = index != -1;

    if (found)
    {
        const PXSize copyedBytes = PXTextCopyW(currnetPath, index + 1, targetPath, index + 1);
        const PXSize toCopy = PXPathSizeMax - copyedBytes;

        PXTextCopyW(newFileName, toCopy, targetPath + copyedBytes, toCopy);
    }
}

void FileFilesInFolder(const char* folderPath, wchar_t*** list, PXSize& listSize)
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

        for (PXSize index = 0; directoryInfo = readdir(directory); index++)
        {
            const bool isFile = directoryInfo->d_type == DT_REG || true;

            if (isFile)
            {
                const char* fileName = directoryInfo->d_name;
                const PXSize length = Text::Length(fileName);
                wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);
                wchar_t** target = &(*list)[index];

                if (!newString)
                {
                    return; // Error: OutOfMemory
                }

                (*target) = newString;
                PXSize writtenBytes = Text::Copy(fileName, length, *target, length);
            }
        }

        closedir(directory);
    }
#elif OSWindows
    wchar_t folderPathW[PXPathSizeMax];
    PXSize writtenBytes = PXTextCopyAW(folderPath, PXPathSizeMax, folderPathW, PXPathSizeMax);

    WIN32_FIND_DATA dataCursour{0};
    HANDLE hFind = 0;

    //Memory::Set(&dataCursour, 0, sizeof(WIN32_FIND_DATA));

    hFind = FindFirstFile(folderPathW, &dataCursour);     // "/*.*";

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
    PXSize fileIndex = 0;

    do
    {
        const PXSize length = TextLengthW(dataCursour.cFileName);
        const wchar_t* filePathSource = dataCursour.cFileName;
        wchar_t* newString = Memory::Allocate<wchar_t>(length + 1);

        if (!newString)
        {
            return; // Error: OutOfMemory
        }

        PXTextCopyW(filePathSource, length, newString, length);

        (*list)[fileIndex] = newString;

        fileIndex++;
    }
    while (FindNextFile(hFind, &dataCursour));

    FindClose(hFind);
#endif
}

void FileFilesInFolder(const wchar_t* folderPath, wchar_t*** list, PXSize& listSize)
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
    char filePathA[PXPathSizeMax];

    Text::Copy(filePath, PXPathSizeMax, filePathA, PXPathSizeMax);

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
