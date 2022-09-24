#ifndef OSVersionINCLUDE
#define OSVersionINCLUDE

//---<Detect OS version>----------------------------------------------------
#if defined(unix) || defined(__unix) || defined(__unix__)
    #ifndef OSUnix
        #define OSUnix
        #pragma message("[PX] UNIX architecture detected")
    #endif
#elif defined(linux) || defined(__linux) || defined(__linux__)
    #ifndef OSLinux
        #define OSLinux
         #pragma message("[PX] Linux architecture detected")
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #ifndef OSApple
        #define OSApple
         #pragma message("[PX] Apple architecture detected")
    #endif
#elif defined(__ANDROID__)
    #ifndef OSApple
        #define OSAndroid
        #pragma message("[PX] Android architecture detected")
    #endif
#elif defined(WIN64) || defined(_WIN64) || defined(__WIN64) || defined(__WIN64__)
    #define OSWindowsSystem64Bit
    #pragma message("[PX] Windows architecture 64-Bit detected")
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
    #define OSWindowsSystem32Bit
    #pragma message("[PX] Windows architecture 32-Bit detected")
#else
    #error [PX] Architecture not detected!
#endif
//-----------------------------------------------------------------------------

#if (defined(OSWindowsSystem64Bit) || defined(OSWindowsSystem32Bit))
    #ifndef OSWindows
        #define OSWindows
        #pragma message("[PX] Windows architecture detected")
    #endif
#endif
//-----------------------------------------------------------------------------


/*
//-----------------------------------------------------------------------------
#if defined(_AMD64_) || defined(OSWindowsSystem64Bit)
#define OS64Bit
#pragma message("[PX] architecture 64-Bit detected")
#elif defined(_X86_)
#define OS32Bit
#pragma message("[PX] architecture 32-Bit detected")
#else
#error [PX][Error] Invalid bit-version architecture!
#endif*/
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
#if defined(OSWindows) // Detect which windows version is used

    #include <Windows.h>
    #include <SdkDdkVer.h>

    #define Version_Windows WINVER

    #define Version_Windows_NT 0x0400
    #define Version_Windows_2000 0x0500
    #define Version_Windows_XP 0x0501
    #define Version_Windows_Server_2003 0x0502
    #define Version_Windows_Vista 0x0600
    #define Version_Windows_Server_2008 0x0600
    #define Version_Windows_7 0x0601
    #define Version_Windows_8 0x0602
    #define Version_Windows_8_1 0x0603
    #define Version_Windows_10 0x0A00

    #if Version_Windows >= Version_Windows_NT
        #define WindowsAtleastNT
    #endif

    #if Version_Windows >= Version_Windows_2000
        #define WindowsAtleast2000
    #endif

    #if Version_Windows >= Version_Windows_XP
        #define WindowsAtleastXP
    #endif

    #if Version_Windows >= Version_Windows_Vista
        #define WindowsAtleastVista
    #endif

    #if Version_Windows >= Version_Windows_7
        #define WindowsAtleast7
    #endif

    #if Version_Windows >= Version_Windows_8
        #define WindowsAtleast8
    #endif

    #if Version_Windows >= Version_Windows_10
        #define WindowsAtleast10
    #endif

    #if Version_Windows == Version_Windows_XP
        #define OSWindowsXP
        typedef struct IUnknown IUnknown;
    #elif Version_Windows == Version_Windows_7
        #define OSWindows7
    #elif Version_Windows == Version_Windows_10
        #define OSWindows10
    #endif
//-----------------------------------------------------------------------------
#endif

#endif // OSVersionINCLUDE
