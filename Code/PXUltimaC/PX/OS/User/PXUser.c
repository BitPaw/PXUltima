#include "PXUser.h"

#if OSUnix
#include <unistd.h>
#elif OSWindows
#include <windows.h>
#include <userenv.h> // Could use GetUserProfileDirectoryW() but not needed yet

#if WindowsAtleastVista
#include <ShlObj_core.h>
#endif // 0

#endif

PXResult PXAPI  PXUserNameGetAA(PXText* const name)
{
    switch(name->Format)
    {
        case TextFormatUTF8:
        case TextFormatASCII:
        {
#if OSUnix
            name->SizeUsed = getlogin_r(name->A, name->SizeAllocated); // unistd.h

            const PXBool success = name->SizeUsed > 0;

            return success;

#elif OSWindows
            DWORD size = name->SizeAllocated;

            const PXBool successful = GetComputerNameA(name->A, &size); // Windows 2000 (+UWP), Kernel32.dll, winbase.h
            const PXActionResult result = PXErrorCurrent(successful);

            name->SizeUsed = (PXSize)successful * (PXSize)size;

            return result;
#endif

            break;
        }
        case TextFormatUNICODE:
        {
#if OSUnix

            return 0;

#elif OSWindows
            DWORD size = name->SizeAllocated;

            const PXBool successful = GetComputerNameW(name->W, &size); // Windows 2000 (+UWP), Kernel32.dll, winbase.h
            const PXActionResult result = PXErrorCurrent(successful);

            name->NumberOfCharacters = (PXSize)successful * (PXSize)size;
            name->SizeUsed = name->NumberOfCharacters * sizeof(wchar_t);

            return result;
#endif

            break;
        }
    }

    return PXActionRefusedFormatNotSupported;
}

PXBool PXAPI PXUserEnviromentFolderGet(PXText* const name, const PXUserEnviromentFolderID pxUserEnviromentFolderID)
{
#if OSUnix

    return PXFalse;

#elif OSWindows

#if WindowsAtleastVista && PXOSWindowsDestop && 0

    const GUID* pathID = 0;

    switch(pxUserEnviromentFolderID)
    {
        case PXUserEnviromentFolderIDAppData:
            pathID = &FOLDERID_RoamingAppData;
            break;

        case PXUserEnviromentFolderIDDownload:
            pathID = &FOLDERID_Downloads;
            break;

        case PXUserEnviromentFolderIDDocuments:
            pathID = &FOLDERID_Documents;
            break;

        case PXUserEnviromentFolderIDPictures:
            pathID = &FOLDERID_Pictures;
            break;

        case PXUserEnviromentFolderIDScreenshots:
            pathID = &FOLDERID_Screenshots;
            break;

        case PXUserEnviromentFolderIDVideo:
            pathID = &FOLDERID_Videos;
            break;

        case PXUserEnviromentFolderIDDestop:
            pathID = &FOLDERID_Desktop;
            break;

        case PXUserEnviromentFolderIDFonts:
            pathID = &FOLDERID_Fonts;
            break;

        case PXUserEnviromentFolderIDMusic:
            pathID = &FOLDERID_Music;
            break;

        default:
            return PXFalse;
    }

    PXText temporalCache;
    PXTextConstructBufferW(&temporalCache, 260);

    const HRESULT result = SHGetKnownFolderPath((KNOWNFOLDERID*)pathID, KF_FLAG_DEFAULT_PATH, PXNull, (PWSTR*)temporalCache.W); // Windows Vista, Shell32.dll, shlobj_core.h
    const PXBool success = S_OK == result;

    PXTextCopy(&temporalCache, name);

    CoTaskMemFree(temporalCache.W); // Needs to be called in ANY case.

    return success;
#else
    return PXFalse;
#endif
#endif
}

void PXAPI PXUserCreate(const char* username, const char* password)
{
#if OSUnix && 0
    struct passwd pwd;
    struct spwd spwd;
    char* encrypted_password;

    // Encrypt the password
    encrypted_password = crypt(password, "salt");

    // Set up the passwd structure
    pwd.pw_name = username;
    pwd.pw_passwd = "x"; // Placeholder for shadow password
    pwd.pw_uid = 1001; // Example UID
    pwd.pw_gid = 1001; // Example GID
    pwd.pw_gecos = "User";
    pwd.pw_dir = "/home/username";
    pwd.pw_shell = "/bin/bash";

    // Set up the shadow password structure
    spwd.sp_namp = username;
    spwd.sp_pwdp = encrypted_password;
    spwd.sp_lstchg = (long)time(NULL) / (60 * 60 * 24);
    spwd.sp_min = 0;
    spwd.sp_max = 99999;
    spwd.sp_warn = 7;

    // Add the user to the system
    FILE* passwd_file = fopen("/etc/passwd", "a");
    if(passwd_file)
    {
        fprintf(passwd_file, "%s:x:%d:%d:%s:%s:%s\n", pwd.pw_name, pwd.pw_uid, pwd.pw_gid, pwd.pw_gecos, pwd.pw_dir, pwd.pw_shell);
        fclose(passwd_file);
    }

    FILE* shadow_file = fopen("/etc/shadow", "a");
    if(shadow_file)
    {
        fprintf(shadow_file, "%s:%s:%ld:%d:%d:%d:%d:%d:%d\n", spwd.sp_namp, spwd.sp_pwdp, spwd.sp_lstchg, spwd.sp_min, spwd.sp_max, spwd.sp_warn, -1, -1, -1);
        fclose(shadow_file);
    }

    // Create the user's home directory
    mkdir(pwd.pw_dir, 0755);
    chown(pwd.pw_dir, pwd.pw_uid, pwd.pw_gid);

#elif OSWindows

    // ...

#else

#endif
}