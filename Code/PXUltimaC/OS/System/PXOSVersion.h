#ifndef PXOSVersionINCLUDE
#define PXOSVersionINCLUDE

//---------------------------------------------------------
// Windows setup settings
//---------------------------------------------------------
#define OSForcePOSIXForWindows 0
//---------------------------------------------------------

// Device
#define OSDeviceDestop 'D'
#define OSDeviceConsole 'C'

#define OSDeviceToUse OSDeviceDestop

//---<Detect OS version>----------------------------------------------------
#if defined(unix) || defined(__unix) || defined(__unix__)
#define OSUnix 1u
#else
#define OSUnix 0u
#endif
//-----------------------------------------------------------------------------
#if defined(linux) || defined(__linux) || defined(__linux__)
#define OSLinux 1u
#else
#define OSLinux 0u
#endif
//-----------------------------------------------------------------------------
#if defined(__APPLE__) || defined(__MACH__)
#define OSApple 1u
#else
#define OSApple 0u
#endif
//-----------------------------------------------------------------------------
#if defined(__ANDROID__)
#define OSAndroid 1u
#else
#define OSAndroid 0u
#endif
//-----------------------------------------------------------------------------
#if \
defined(__NT__) || \
defined(WIN32) || \
defined(_WIN32) || \
defined(__WIN32__) || \
defined(WIN64) || \
defined(_WIN64) || \
defined(__WIN64) || \
defined(__WIN64__)
#define OSWindows 1u
#else
#define OSWindows 0u
#endif
//-----------------------------------------------------------------------------
#define PXNOOS !(OSAndroid || OSApple || OSLinux || PXWindows)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Detect x86, x64, ARM, ...
//-----------------------------------------------------------------------------
#if \
defined(__x86_64__ ) || \
defined(__ppc64__) || \
defined(WIN64) || \
defined(_WIN64) || \
defined(__WIN64) || \
defined(__WIN64__)
  
#define OS32B 0u
#define OS64B 1u

#elif \
defined(__x86__ ) || \
defined(__ppc86__) || \
defined(WIN32) || \
defined(_WIN32) || \
defined(__WIN32) || \
defined(__WIN32__)

#define OS32B 1u
#define OS64B 0u

#endif
//-----------------------------------------------------------------------------



// Linux OS-Bit Version
#if OSLinux && OS64B
#define OSLinux32B 0u
#define OSLinux64B 1u
#elif OSLinux && OS32B
#define OSLinux32B 1u
#define OSLinux64B 0u
#else
#define OSLinux32B 0u
#define OSLinux64B 0u
#endif
//----------------------------------

// Windows OS-BitVersion
#if OSWindows && OS64B
#define OSWindows32B 0u
#define OSWindows64B 1u
#elif OSWindows && OS32B
#define OSWindows32B 1u
#define OSWindows64B 0u
#else
#define OSWindows32B 0u
#define OSWindows64B 0u
#endif
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
#define OSVersionOutput 0u

#if OSVersionOutput
#if OSUnix
    #pragma message("[PX] Operating system: UNIX")
#elif OSLinux
    #pragma message("[PX] Operating system: Linux")
#elif OSApple
    #pragma message("[PX] Operating system: Apple")
#elif OSAndroid
    #pragma message("[PX] Operating system: Android")
#elif OSWindows64B
    #pragma message("[PX] Operating system: Windows 64-Bit detected")
#elif OSWindows32B
    #pragma message("[PX] Operating system: Windows 32-Bit detected")
#else
    #error [PX] Operating system could not be detected!
#endif
#endif
//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------
typedef enum PXWindowsVersion_
{
    PXWindowsVersionInvalid,
    PXWindowsVersionNT,
    PXWindowsVersion2000,
    PXWindowsVersionXP,
    PXWindowsVersionServer2003,
    PXWindowsVersionVista,
    PXWindowsVersionServer2008,
    PXWindowsVersion7,
    PXWindowsVersion8,
    PXWindowsVersion8x1,
    PXWindowsVersion10,
    PXWindowsVersion11,
    PXWindowsVersion12
}
PXWindowsVersion;

#if OSWindows // Detect which windows version is used

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
    #define Version_Windows_11 0x0A01
    #define Version_Windows_12 0x0A02

    #if Version_Windows >= Version_Windows_NT
        #define WindowsAtleastNT 1u
    #else
        #define WindowsAtleastNT 0u
    #endif

    #if Version_Windows >= Version_Windows_2000
        #define WindowsAtleast2000 1u
    #else
        #define WindowsAtleast2000 0u
    #endif

    #if Version_Windows >= Version_Windows_XP
        #define WindowsAtleastXP 1u
    #else
        #define WindowsAtleastXP 0u
    #endif

    #if Version_Windows >= Version_Windows_Vista
        #define WindowsAtleastVista 1u
    #else
        #define WindowsAtleastVista 0u
    #endif

    #if Version_Windows >= Version_Windows_7
        #define WindowsAtleast7 1u
    #else
        #define WindowsAtleast7 0u
    #endif

    #if Version_Windows >= Version_Windows_8
        #define WindowsAtleast8 1u
    #else
        #define WindowsAtleast8 0u
    #endif

    #if Version_Windows >= Version_Windows_10
        #define WindowsAtleast10 1u
    #else
        #define WindowsAtleast10 0u
    #endif

    #if Version_Windows >= Version_Windows_11
        #define WindowsAtleast11 1u
    #else
        #define WindowsAtleast11 0u
    #endif

    #if Version_Windows >= Version_Windows_12
        #define WindowsAtleast12 1u
    #else
        #define WindowsAtleast12 0u
    #endif


    #if Version_Windows == Version_Windows_XP
        #define OSWindowsXP 1u
        typedef struct IUnknown IUnknown;
    #else
        #define OSWindowsXP 0u
    #endif

    #if Version_Windows == Version_Windows_7
        #define OSWindows7 1u
    #else
        #define OSWindows7 0u
    #endif

    #if Version_Windows == Version_Windows_10
        #define OSWindows10 1u
    #else
        #define OSWindows10 0u
    #endif

    #if Version_Windows == Version_Windows_11
        #define OSWindows11 1u
    #else
        #define OSWindows11 0u
    #endif

    #if Version_Windows == Version_Windows_12
        #define OSWindows12 1u
    #else
        #define OSWindows12 0u
    #endif

//-----------------------------------------------------------------------------
#endif

//---------------------------------------------------------
// Windows Universal Windows Platform detection
//---------------------------------------------------------
#if OSWindows

#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP) && false
#define PXOSWindowsUseUWP 1
#define PXOSWindowsDestop 0
#else
#define PXOSWindowsUseUWP 0
#define PXOSWindowsDestop 1
#endif

#else

#define PXOSWindowsUseUWP 0
#define PXOSWindowsDestop 0

#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Calling convection
//---------------------------------------------------------
#if OSUnix
#define PXOSAPI
#define PXOSAPIInternal
#elif OSWindows
#define PXOSAPI WINAPI
#define PXOSAPIInternal
#endif
//---------------------------------------------------------

typedef enum PXOSManufacturer_
{
    PXOSManufacturerInvalid,
    PXOSManufacturerLinux,
    PXOSManufacturerAppleOSX,
    PXOSManufacturerWindows
}
PXOSManufacturer;

typedef enum PXOSWindowsversion_
{
    PXOSWindowsversionInvalid,
    PXOSWindowsversion3x11,
    PXOSWindowsversionNT,
    PXOSWindowsversion95,
    PXOSWindowsversion98,
    PXOSWindowsversion98SE,
    PXOSWindowsversion2000,
    PXOSWindowsversionXP,
    PXOSWindowsversionVista,
    PXOSWindowsversion7,
    PXOSWindowsversion8,
    PXOSWindowsversion8x1,
    PXOSWindowsversion10,
    PXOSWindowsversion11,
    PXOSWindowsversion12
}
PXOSWindowsversion;

typedef struct PXOSVersionInfo_
{
    PXOSManufacturer Manufacturer;

    int VersionMajor;
    int VersionMinor;

    union
    {
        PXOSWindowsversion VersionWindows;
    };

}
PXOSVersionInfo;

void PXOSVersionGet(PXOSVersionInfo* const pxOSVersionInfo);

#endif