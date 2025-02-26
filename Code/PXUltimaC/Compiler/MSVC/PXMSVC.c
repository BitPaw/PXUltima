#define PXCompilerTargetExecutable      (1<<0)
#define PXCompilerTargetLibraryStatic   (1<<1)
#define PXCompilerTargetLibraryDynamic  (1<<2)

typedef struct PXCompilerMSVC_
{
    char CompilerPathRoot[MAX_PATH];

    const char* CompilerPathInclude;
    const char* CompilerPathLibrarySearch;
    const char* CompilerPathLibraryFile;
}
PXCompilerMSVC;




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



size_t PXBashExecute(const char* command, char** result, size_t resultSize)
{

#if 0
    printf
    (
        "\n\n"
        "-------------------------------------\n"
        "%s\n"
        "-------------------------------------\n\n",
        command
    );
#endif

#if 0
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;
    DWORD bytesRead = 0;

    // Create the pipe for the child process's STDOUT.
    if (!CreatePipe(&hRead, &hWrite, 0, 0)) {
        printf("CreatePipe failed\n");
        return 0;
    }

    size_t len = strnlen(command, 1024*8);

    BOOL READRD = WriteFile(hWrite, (LPVOID)command, len, &bytesRead, NULL);



    char* message = (char*)malloc(1024 * 8);
    memset(message, 0, 1024 * 8);
    BOOL READRDEEE = ReadFile(hRead, message, 1024 * 8, &bytesRead, NULL);

    printf(message);  
    printf("\n");

    free(message);


    CloseHandle(hRead);
    CloseHandle(hWrite);
        
#else

 

    // Expect: "C:\Program Files\Microsoft Visual Studio\2022\Professional"

    if (result) // only if we have an output
    {
        // Execute the command and get the output
        FILE* fp = _popen(command, "r");

        if (fp == NULL)
        {
            printf("Failed to run command\n");
            return 1;
        }

        if (!result) // only if we just have a pointer without target
        {
            const size_t size = ftell(fp);

            // alloc
            *result = (char*)malloc(size);
        }

        char* res = fgets(*result, _MAX_PATH, fp);

        if (!res)
        {
            return 1;
        }

        // Close the pipe
        _pclose(fp);
    }
    else
    {
         // [Warning] 
         // Remember that any input that contains spaces requires surrounding quotation marks.
         // This string gets splitted on spaces, and this string itself will be scanned for this.
         // Ensure that we have surrounding quotationmarks!
      
        system(command);
    }


#endif


    return 0;
}

int PXCompilerInitMSVC(PXCompilerMSVC* const pxCompilerMSVC)
{
    memset(pxCompilerMSVC, 0, sizeof(PXCompilerMSVC));

    // Command to get the Visual Studio installation path
    // "\"%ProgramFiles(x86)%\\Microsoft Visual Studio\\Installer\\vswhere.exe\" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath";
    const char command[] = "\"C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe\" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath";
 
    char* adr = pxCompilerMSVC->CompilerPathRoot;

    PXBashExecute(command, (char**)&adr, _MAX_PATH);


    // Create path for c-compiler

    size_t len = strlen(pxCompilerMSVC->CompilerPathRoot)-1;
    sprintf(&pxCompilerMSVC->CompilerPathRoot[len], "/VC/Tools/MSVC/14.40.33807/bin/Hostx64/x64");

    // VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64


    // Create library include list


    pxCompilerMSVC->CompilerPathInclude = inlcudeList;
    pxCompilerMSVC->CompilerPathLibrarySearch = libraryList;
    pxCompilerMSVC->CompilerPathLibraryFile = libraryFileList;

    
  //    memcpy_s(pxCompilerMSVC->CompilerPathInclude, 700, inlcudeList, sizeof(inlcudeList));    
//memcpy_s(pxCompilerMSVC->CompilerPathLibrarySearch, 700, libraryList, sizeof(libraryList));
  //    memcpy_s(pxCompilerMSVC->CompilerPathLibraryFile, 1024, libraryFileList, sizeof(libraryFileList));

    return 0;
}

int PXCompilerCompileMSVC(PXCompilerMSVC* const pxCompilerMSVC, const char* fileName)
{
    const size_t size = 1024 * 8;
    char* buffer = (char*)malloc(size);
    size_t offset = 0;

    offset += sprintf_s(&buffer[offset], size - offset, "\"");

    // Add executable path
    offset += sprintf_s(&buffer[offset], size - offset, "\"%s/cl.exe\"", pxCompilerMSVC->CompilerPathRoot);

    for (size_t i = 0; i < offset; ++i)
    {
        if (buffer[i] == '/')
        {
            buffer[i] = '\\';
        }
    }

    // Add library path
    const char* cursor = pxCompilerMSVC->CompilerPathInclude;

    for (;;)
    {         
        size_t length = strnlen(cursor, 260);

        if (length == 0 || length == -1)
        {
            break;
        }

        //printf("[+] Include <%s>\n", cursor);

        offset += sprintf_s(&buffer[offset], size - offset, " /I\"%s\"", cursor);

        cursor += length + 1;
    }   

    // Add files to compile
    offset += sprintf_s(&buffer[offset], size - offset, " /c %s", fileName);
    offset += sprintf_s(&buffer[offset], size - offset, "\"");

    PXBashExecute(buffer, 0, 0);

    return 1;
}

int PXCompilerLinkMSVC(PXCompilerMSVC* const pxCompilerMSVC, int flags, const char* fileName, const char* resultName)
{
    const size_t mmsiize = 1024 * 8;
    char* commandBuffer = (char*)malloc(mmsiize);
    size_t offset = 0;

    offset += sprintf_s(commandBuffer+ offset, mmsiize - offset, "\"");


    switch (flags)
    {
    case PXCompilerTargetExecutable:
    {

        // if exe
        offset += sprintf_s(commandBuffer+ offset, mmsiize- offset, "\"%s/cl.exe\"", pxCompilerMSVC->CompilerPathRoot);

        const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

        for (;;)
        {
            size_t length = strnlen(cursor, 260);

            if (length == 0 || length == -1)
            {
                break;
            }

           // printf("[+] LIB <%s>\n", cursor);

            offset += sprintf_s(&commandBuffer[offset], mmsiize - offset, " \"%s\"", cursor);

            cursor += length + 1;
        }

        offset += sprintf_s(commandBuffer + offset, mmsiize - offset, " %s /Fe%s.exe", fileName, resultName);


        break;
    }
    case PXCompilerTargetLibraryStatic:
    {
        offset += sprintf_s(&commandBuffer[offset], mmsiize, "\"%s/lib.exe\" /out:%s.lib ", pxCompilerMSVC->CompilerPathRoot, resultName, fileName);

        const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

        for (;;)
        {
            size_t length = strnlen(cursor, 260);

            if (length == 0 || length == -1)
            {
                break;
            }

            // printf("[+] LIB <%s>\n", cursor);

            offset += sprintf_s(&commandBuffer[offset], mmsiize - offset, " \"%s\"", cursor);

            cursor += length + 1;
        }


        break;
    }
    case PXCompilerTargetLibraryDynamic:
    {
        // if DLL
        offset += sprintf_s(commandBuffer + offset, mmsiize - offset, "\"%s/cl.exe\" /LD", pxCompilerMSVC->CompilerPathRoot);

        const char* cursor = pxCompilerMSVC->CompilerPathLibraryFile;

        for (;;)
        {
            size_t length = strnlen(cursor, 260);

            if (length == 0 || length == -1)
            {
                break;
            }

            // printf("[+] LIB <%s>\n", cursor);

            offset += sprintf_s(&commandBuffer[offset], mmsiize - offset, " \"%s\"", cursor);

            cursor += length + 1;
        }

        offset += sprintf_s(commandBuffer + offset, mmsiize - offset, " %s /Fe%s.dll", fileName, resultName);


        break;
    }
    default:
    {
        return 0;
    }
    }

    offset += sprintf_s(commandBuffer+ offset, mmsiize - offset, "\"");

    PXBashExecute(commandBuffer, 0, 0);

    return 0;
}

PXCompilerMSVC pxCompilerMSVC;

int PXCompilerTEST() 
{  
    PXCompilerInitMSVC(&pxCompilerMSVC);
    PXCompilerCompile(&pxCompilerMSVC, "PXTESTCToCSharp.cpp");
    PXCompilerLink(&pxCompilerMSVC, PXCompilerTargetLibraryStatic, "PXTESTCToCSharp.obj", "PXTESTCToCSharp");

    while (1)
    {
        Sleep(1);
    }   

    return 0;
}
