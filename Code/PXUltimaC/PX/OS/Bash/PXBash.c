#include "PXBash.h"

#include <PX/Media/PXText.h>

#include <stdlib.h>
#include <stdio.h>

PXResult PXAPI PXBashExecute(const PXText PXREF programPath, const PXBool syncronous)
{
    /*
    Pipe pipe;
    PXResult result = pipe.Open(programPath, FileOpenMode::Read);
    PXSize dataLength = 0;
    unsigned char* data = 0;

    if(result != PXResultOK)
    {
        return result;
    }

    result = File::ReadFromDisk(pipe.FileMarker, &data, dataLength, true);

    result = pipe.Close();

    printf("[PEx] %s\n", data);

    free(data);
    */

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXBashExecuteA
(
    const char PXREF commandText,
    const PXSize commandTextSize,
    char* PXREF outBuffer,
    const PXSize outBufferSizeMax,
    PXSize PXREF outBufferSizeCurrent,
    const PXBool syncronous
)
{
    if(!commandText)
    {
        return PXResultRefusedParameterNull;
    }

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
    if(!CreatePipe(&hRead, &hWrite, 0, 0)) {
        printf("CreatePipe failed\n");
        return 0;
    }

    size_t len = strnlen(command, 1024 * 8);

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


    // [Warning] 
    // Remember that any input that contains spaces requires surrounding quotation marks.
    // This string gets splitted on spaces, and this string itself will be scanned for this.
    // Ensure that we have surrounding quotationmarks!


    if(!outBuffer)
    {
        const int resultID = system(commandText); // [POSIX]  stdlib.h

        return PXResultOK;
    }

    // Execute the command and get the output
    FILE PXREF pipeHandle = _popen(commandText, "r"); // [POSIX] stdio.h
    const PXResult openError = PXErrorCurrent(pipeHandle>0);

    if(PXResultOK != openError)
    {
        return openError;
    }

    if(!(*outBuffer)) // only if we just have a pointer without target
    {
        const size_t size = ftell(pipeHandle);

        // alloc
        *outBuffer = (char*)malloc(size);
    }

    char* res = fgets(*outBuffer, outBufferSizeMax, pipeHandle);
    const PXResult getError = PXErrorCurrent(!res);

    if(PXResultOK != getError)
    {
        return openError;
    }

    // Close the pipe
    _pclose(pipeHandle); // [POSIX] stdio.h


    // There is often a \n at the end. We dont want that
    const PXSize size = PXTextLengthA(*outBuffer, outBufferSizeMax);

    if(*outBuffer[size-1] == '\n')
    {
        *outBuffer[size - 1] = '\0';
        
        if(outBufferSizeCurrent)
        {
            *outBufferSizeCurrent = size;
        }
    }

#endif

    return PXResultOK;
}