# File

## POSIX
Files use the POXIX style FILE* structure.

## Linux
### Icons
X11 does not contain any definitions of icons, as it is a bare drawing API. To still have icons, a seperate library exists that is preinstalled on most distributions called [xdg](https://en.wikipedia.org/wiki/Freedesktop.org). Here we use the [MIME](https://en.wikipedia.org/wiki/MIME) system with the [Media typed](https://en.wikipedia.org/wiki/Media_type).

```C
char* MIMETypeGet(const char *filename)
{
    char command[256];
    snprintf(command, sizeof(command), "xdg-mime query filetype %s", filename);
    FILE *fp = popen(command, "r");
    if (!fp) return NULL;

    static char mime_type[256];
    if (fgets(mime_type, sizeof(mime_type), fp) != NULL)
    {
        mime_type[strcspn(mime_type, "\n")] = '\0'; // Remove newline
    }
    pclose(fp);
    return mime_type;
}

char* PathIconGet(const char *mimeType)
{
    char command[256];
    snprintf(command, sizeof(command), "xdg-icon-resource lookup --theme hicolor %s", mime_type);
    FILE *fp = popen(command, "r");
    if (!fp) return NULL;

    static char icon_path[256];
    if (fgets(icon_path, sizeof(icon_path), fp) != NULL)
    {
        icon_path[strcspn(icon_path, "\n")] = '\0'; // Remove newline
    }
    pclose(fp);
    return icon_path;
}
```
---

## Windows
The system can use the POSIX functions but prefers that programmers use the standard HANDLE as a fileID.

## Physical location on disk
Maybe we want to map out how the file looks in a graph of the whole disk?<br>
1. CreateFile to get a HANDLE to a file or I/O device.
2. DeviceIoControl with FSCTL_GET_RETRIEVAL_POINTERS
3. (optional) GetFileInformationByHandle, for more details.
4. CloseHandle, don't forget 
