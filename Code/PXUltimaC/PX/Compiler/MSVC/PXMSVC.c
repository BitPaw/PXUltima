#include "PXMSVC.h"

#include <PX/OS/Bash/PXBash.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Memory/PXMemory.h>

const char inlcudeList[] =
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/include\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/atlmfc/include\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/VS/include\0"
"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt\0"
"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um\0"
"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared\0"
"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt\0"
"C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/cppwinrt\0"
"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/Include/um\0"
"\0";



const char libraryList[] =
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/lib/x64\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/atlmfc/lib/x64\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Auxiliary/VS/lib/x64\0" // Jelper
"C:/Program Files (x86)/Windows Kits/10/lib/10.0.22621.0/ucrt/x64\0" // universal things?
"C:/Program Files (x86)/Windows Kits/10/lib/10.0.22621.0/um/x64\0" // default lib
"C:/Program Files (x86)/Windows Kits/NETFXSDK/4.8/lib/um/x64\0"  // mscoree
"\0";

// Windows 7 SDK C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\winspool.lib

const char libraryFileList[] =
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/kernel32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/user32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/gdi32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/winspool.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/comdlg32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/advapi32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/shell32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/ole32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/oleaut32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/uuid.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/odbc32.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/odbccp32.lib\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/lib/x64/LIBCMT.lib\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/lib/x64/OLDNAMES.lib\0"
"C:/Program Files/Microsoft Visual Studio/2022/Professional/VC/Tools/MSVC/14.40.33807/lib/x64/libvcruntime.lib\0"
"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/ucrt/x64/libucrt.lib\0"
"\0";


const char PXCompilerMSVCWhere[] = "\"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe\" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath";
const PXInt8U PXCompilerMSVCWhereSize = sizeof(PXCompilerMSVCWhere);

PXActionResult PXAPI PXCompilerInitializeMSVC(PXCompilerMSVC* const pxCompilerMSVC)
{
    PXClear(PXCompilerMSVC, pxCompilerMSVC);

    // Command to get the Visual Studio installation path
    // "\"%ProgramFiles(x86)%\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath";


    char* adr = pxCompilerMSVC->CompilerPathRoot;
    PXSize writtenBytes = 0;

    const PXActionResult fetchResult = PXBashExecuteA
    (
        PXCompilerMSVCWhere,
        PXCompilerMSVCWhereSize,
        (char**)&adr,
        260,
        &writtenBytes,
        PXFalse
    );


    // Create path for c-compiler
    writtenBytes += PXTextPrintA(&pxCompilerMSVC->CompilerPathRoot[writtenBytes], 260 - writtenBytes, "/VC/Tools/MSVC");

    // VC\Tools\MSVC
    writtenBytes += PXTextPrintA(&pxCompilerMSVC->CompilerPathRoot[writtenBytes], 260 - writtenBytes, "/14.40.33807/bin/Hostx64/x64");

    // Create library include list
    pxCompilerMSVC->CompilerPathInclude = inlcudeList;
    pxCompilerMSVC->CompilerPathLibrarySearch = libraryList;
    pxCompilerMSVC->CompilerPathLibraryFile = libraryFileList;


    //    memcpy_s(pxCompilerMSVC->CompilerPathInclude, 700, inlcudeList, sizeof(inlcudeList));    
  //memcpy_s(pxCompilerMSVC->CompilerPathLibrarySearch, 700, libraryList, sizeof(libraryList));
    //    memcpy_s(pxCompilerMSVC->CompilerPathLibraryFile, 1024, libraryFileList, sizeof(libraryFileList));

    return 0;
}

PXActionResult PXAPI PXCompilerCompileMSVC(PXCompilerMSVC* const pxCompilerMSVC, const char* fileName)
{
    const size_t size = 1024 * 8;
    char* buffer = PXMemoryHeapCallocT(char, size);
    size_t offset = 0;

    offset += PXTextPrintA(&buffer[offset], size - offset, "\"");

    // Add executable path
    offset += PXTextPrintA(&buffer[offset], size - offset, "\"%s/cl.exe\"", pxCompilerMSVC->CompilerPathRoot);

    for(size_t i = 0; i < offset; ++i)
    {
        if(buffer[i] == '/')
        {
            buffer[i] = '\\';
        }
    }

    // Add library path
    const char* cursor = pxCompilerMSVC->CompilerPathInclude;

    for(;;)
    {
        size_t length = PXTextLengthA(cursor, 260);

        if(length == 0 || length == -1)
        {
            break;
        }

        //printf("[+] Include <%s>\n", cursor);

        offset += PXTextPrintA(&buffer[offset], size - offset, " /I\"%s\"", cursor);

        cursor += length + 1;
    }

    // Add files to compile
    offset += PXTextPrintA(&buffer[offset], size - offset, " /c %s", fileName);
    offset += PXTextPrintA(&buffer[offset], size - offset, "\"");

    PXBashExecuteA(buffer, offset, PXNull, 0, PXNull, PXFalse);

    return 1;
}

PXActionResult PXAPI PXCompilerLinkMSVC(PXCompilerMSVC* const pxCompilerMSVC, int flags, const char* fileName, const char* resultName)
{
    const size_t mmsiize = 1024 * 8;
    char* buffer = PXMemoryHeapCallocT(char, mmsiize);
    size_t offset = 0;

    offset += PXTextPrintA(buffer + offset, mmsiize - offset, "\"");


    switch(flags)
    {
        case PXCompilerTargetExecutable:
        {

            // if exe
            offset += PXTextPrintA(buffer + offset, mmsiize - offset, "\"%s/cl.exe\"", pxCompilerMSVC->CompilerPathRoot);

            const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

            for(;;)
            {
                size_t length = PXTextLengthA(cursor, 260);

                if(length == 0 || length == -1)
                {
                    break;
                }

                // printf("[+] LIB <%s>\n", cursor);

                offset += PXTextPrintA(&buffer[offset], mmsiize - offset, " \"%s\"", cursor);

                cursor += length + 1;
            }

            offset += PXTextPrintA(buffer + offset, mmsiize - offset, " %s /Fe%s.exe", fileName, resultName);


            break;
        }
        case PXCompilerTargetLibraryStatic:
        {
            offset += PXTextPrintA(&buffer[offset], mmsiize, "\"%s/lib.exe\" /out:%s.lib ", pxCompilerMSVC->CompilerPathRoot, resultName, fileName);

            const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

            for(;;)
            {
                size_t length = PXTextLengthA(cursor, 260);

                if(length == 0 || length == -1)
                {
                    break;
                }

                // printf("[+] LIB <%s>\n", cursor);

                offset += PXTextPrintA(&buffer[offset], mmsiize - offset, " \"%s\"", cursor);

                cursor += length + 1;
            }


            break;
        }
        case PXCompilerTargetLibraryDynamic:
        {
            // if DLL
            offset += PXTextPrintA(buffer + offset, mmsiize - offset, "\"%s/cl.exe\" /LD", pxCompilerMSVC->CompilerPathRoot);

            const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

            for(;;)
            {
                size_t length = PXTextLengthA(cursor, 260);

                if(length == 0 || length == -1)
                {
                    break;
                }

                // printf("[+] LIB <%s>\n", cursor);

                offset += PXTextPrintA(&buffer[offset], mmsiize - offset, " \"%s\"", cursor);

                cursor += length + 1;
            }

            offset += PXTextPrintA(buffer + offset, mmsiize - offset, " %s /Fe%s.dll", fileName, resultName);


            break;
        }
        default:
        {
            return 0;
        }
    }

    offset += PXTextPrintA(buffer + offset, mmsiize - offset, "\"");

    PXBashExecuteA(buffer, offset, PXNull, 0, PXNull, PXFalse);

    return 0;
}

#if 0
int PXCompilerTEST()
{
    PXCompilerInitMSVC(&pxCompilerMSVC);
    PXCompilerCompile(&pxCompilerMSVC, "PXTESTCToCSharp.cpp");
    PXCompilerLink(&pxCompilerMSVC, PXCompilerTargetLibraryStatic, "PXTESTCToCSharp.obj", "PXTESTCToCSharp");

    while(1)
    {
        Sleep(1);
    }

    return 0;
}
#endif