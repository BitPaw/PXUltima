#ifndef PXRegistryINCLUDE
#define PXRegistryINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXRegistryUSE OSDeviceToUse == OSDeviceDestop
#if PXRegistryUSE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Library/PXLibrary.h>

#if OSUnix
typedef unsigned int PXRegistryID;

#elif OSWindows
#include <Windows.h>
#include <winreg.h>

typedef HKEY PXRegistryID;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef	LSTATUS(APIENTRY* PXRegConnectRegistryA)(_In_opt_ LPCSTR lpMachineName, _In_ HKEY hKey, _Out_ PHKEY phkResult);
	typedef	LSTATUS(APIENTRY* PXRegConnectRegistryW)(_In_opt_ LPCWSTR lpMachineName, _In_ HKEY hKey, _Out_ PHKEY phkResult);
	typedef	LSTATUS(APIENTRY* PXRegCloseKey)(_In_ HKEY hKey);
	typedef	LSTATUS(APIENTRY* PXRegDeleteKeyA)(_In_ HKEY hKey, _In_ LPCSTR lpSubKey);
	typedef	LSTATUS(APIENTRY* PXRegDeleteKeyW)(_In_ HKEY hKey, _In_ LPCWSTR lpSubKey);
	typedef	LSTATUS(APIENTRY* PXRegSaveKeyA)(_In_ HKEY hKey, _In_ LPCSTR lpFile, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	typedef	LSTATUS(APIENTRY* PXRegSaveKeyW)(_In_ HKEY hKey, _In_ LPCWSTR lpFile, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes);
	typedef	LSTATUS(APIENTRY* PXRegLoadKeyA)(_In_ HKEY hKey, _In_opt_ LPCSTR lpSubKey, _In_ LPCSTR lpFile);
	typedef	LSTATUS(APIENTRY* PXRegLoadKeyW)(_In_ HKEY hKey, _In_opt_ LPCWSTR lpSubKey, _In_ LPCWSTR lpFile);
	typedef	LSTATUS(APIENTRY* PXRegCreateKeyExA)(_In_ HKEY hKey, _In_ LPCSTR lpSubKey, _Reserved_ DWORD Reserved, _In_opt_ LPSTR lpClass, _In_ DWORD dwOptions, _In_ REGSAM samDesired, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, _Out_ PHKEY phkResult, _Out_opt_ LPDWORD lpdwDisposition);
	typedef	LSTATUS(APIENTRY* PXRegCreateKeyExW)(_In_ HKEY hKey, _In_ LPCWSTR lpSubKey, _Reserved_ DWORD Reserved, _In_opt_ LPWSTR lpClass, _In_ DWORD dwOptions, _In_ REGSAM samDesired, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, _Out_ PHKEY phkResult, _Out_opt_ LPDWORD lpdwDisposition);
	typedef	LSTATUS(APIENTRY* PXRegEnumKeyExA)(_In_ HKEY hKey, _In_ DWORD dwIndex, _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName, _Inout_ LPDWORD lpcchName, _Reserved_ LPDWORD lpReserved, _Out_writes_to_opt_(*lpcchClass, *lpcchClass + 1) LPSTR lpClass, _Inout_opt_ LPDWORD lpcchClass, _Out_opt_ PFILETIME lpftLastWriteTime);
	typedef	LSTATUS(APIENTRY* PXRegEnumKeyExW)(_In_ HKEY hKey, _In_ DWORD dwIndex, _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName, _Inout_ LPDWORD lpcchName, _Reserved_ LPDWORD lpReserved, _Out_writes_to_opt_(*lpcchClass, *lpcchClass + 1) LPWSTR lpClass, _Inout_opt_ LPDWORD lpcchClass, _Out_opt_ PFILETIME lpftLastWriteTime);


	typedef enum PXRegistrySpace_
	{
		PXRegistrySpaceInvalid,
		PXRegistrySpaceLocalMachine,
		PXRegistrySpacePerformanceData,
		PXRegistrySpaceUsers
	}
	PXRegistrySpace;

	typedef struct PXRegistry_
	{
		PXLibrary Library;

		PXRegConnectRegistryA RegistryConnectRegistryA;
		PXRegConnectRegistryW RegistryConnectRegistryW;
		PXRegCloseKey RegistryCloseKey;
		PXRegDeleteKeyA RegistryKeyDeleteA;
		PXRegDeleteKeyW RegistryKeyDeleteW;
		PXRegSaveKeyA RegistryKeySaveA;
		PXRegSaveKeyW RegistryKeySaveW;
		PXRegLoadKeyA RegistryKeyLoadA;
		PXRegLoadKeyW RegistryKeyLoadW;
		PXRegCreateKeyExA RegistryCreateKeyExA;
		PXRegCreateKeyExW RegistryCreateKeyExW;
		PXRegEnumKeyExA RegistryKeyEnumExA;
		PXRegEnumKeyExW RegistryKeyEnumExW;

		PXRegistryID ID;
	}
	PXRegistry;

	PXPublic PXActionResult PXAPI PXRegistryInitialize(PXRegistry* const registry);
	PXPublic PXActionResult PXAPI PXRegistryRelease(PXRegistry* const registry);

	PXPublic PXActionResult PXAPI PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace);
	PXPublic PXActionResult PXAPI PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace);

	PXPublic PXActionResult PXAPI PXRegistryClose(PXRegistry* const registry);

	PXPublic PXActionResult PXAPI PXRegistryKeyListAll(PXRegistry* const registry);

	PXPublic PXActionResult PXAPI PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName);
	PXPublic PXActionResult PXAPI PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile);
	PXPublic PXActionResult PXAPI PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName);
	PXPublic PXActionResult PXAPI PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName);

#ifdef __cplusplus
}
#endif

#endif
#endif