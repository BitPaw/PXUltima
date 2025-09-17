#ifndef PXRegistryIncluded
#define PXRegistryIncluded

#include <PX/OS/Library/PXLibrary.h>

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

    void* ID;

    void* RegistryConnectRegistryA;
    void* RegistryConnectRegistryW;
    void* RegistryCloseKey;
    void* RegistryKeyDeleteA;
    void* RegistryKeyDeleteW;
    void* RegistryKeySaveA;
    void* RegistryKeySaveW;
    void* RegistryKeyLoadA;
    void* RegistryKeyLoadW;
    void* RegistryCreateKeyExA;
    void* RegistryCreateKeyExW;
    void* RegistryKeyEnumExA;
    void* RegistryKeyEnumExW;
}
PXRegistry;

PXPublic PXResult PXAPI PXRegistryInitialize(PXRegistry* const registry);
PXPublic PXResult PXAPI PXRegistryRelease(PXRegistry* const registry);

PXPublic PXResult PXAPI PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace);
PXPublic PXResult PXAPI PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace);

PXPublic PXResult PXAPI PXRegistryClose(PXRegistry* const registry);

PXPublic PXResult PXAPI PXRegistryKeyListAll(PXRegistry* const registry);

PXPublic PXResult PXAPI PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName);
PXPublic PXResult PXAPI PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile);
PXPublic PXResult PXAPI PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName);
PXPublic PXResult PXAPI PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName);

#endif
