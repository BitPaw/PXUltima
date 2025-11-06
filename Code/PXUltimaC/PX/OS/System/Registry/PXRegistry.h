#pragma once

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

PXPublic PXResult PXAPI PXRegistryInitialize(PXRegistry PXREF registry);
PXPublic PXResult PXAPI PXRegistryRelease(PXRegistry PXREF registry);

PXPublic PXResult PXAPI PXRegistryConnectSpace(PXRegistry PXREF registry, const PXRegistrySpace registrySpace);
PXPublic PXResult PXAPI PXRegistryConnectRemote(PXRegistry PXREF registry, const PXText PXREF computerName, const PXRegistrySpace registrySpace);

PXPublic PXResult PXAPI PXRegistryClose(PXRegistry PXREF registry);

PXPublic PXResult PXAPI PXRegistryKeyListAll(PXRegistry PXREF registry);

PXPublic PXResult PXAPI PXRegistryKeyCreate(PXRegistry PXREF registry, const PXText PXREF pxTextKeyName);
PXPublic PXResult PXAPI PXRegistryKeyLoad(PXRegistry PXREF registry, const PXText PXREF pxTextKeyName, const PXText PXREF pxTextFile);
PXPublic PXResult PXAPI PXRegistryKeySave(PXRegistry PXREF registry, const PXText PXREF pxTextKeyName);
PXPublic PXResult PXAPI PXRegistryKeyDelete(PXRegistry PXREF registry, const PXText PXREF pxTextKeyName);

#endif