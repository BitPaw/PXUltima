#pragma once

#ifndef PXDotNetIncludedd
#define PXDotNetIncludedd

#include <PX/Engine/PXResource.h>
#include <PX/Media/PXVersion.h>
#include <PX/OS/Library/PXLibrary.h>


typedef struct PXDotNetCoreCLR_ PXDotNetCoreCLR;

// Modern runtime 
typedef struct PXDotNetHostFX PXDotNetHostFX;

typedef struct PXDotNetMSCoree_ PXDotNetMSCoree;


typedef struct PXDelegate_
{
    PXText NameLibrary;
    PXText NameNamespace;
    PXText NameClass;
    PXText NameFunction;
    void* FunctionAdress;
}
PXDelegate;


typedef PXResult(PXAPI* PXDotNetDelegateFetch)(void PXREF pxDotNetEngine, PXDelegate PXREF pxDelegate);
typedef PXResult(PXAPI* PXDotNetExecute)(void PXREF pxDotNetEngine);

typedef struct PXDotNet_ PXDotNet;

#if OSWindows
PXPrivate PXResult PXAPI PXDotNetInitializeMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree);
PXPrivate PXResult PXAPI PXDotNetInitializeCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR);
PXPrivate PXResult PXAPI PXDotNetInitializeHostFX(PXDotNetHostFX PXREF pxDotNetHostFX);

PXPrivate PXResult PXAPI PXDotNetDelegateFetchMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree, PXDelegate PXREF pxDelegate);
PXPrivate PXResult PXAPI PXDotNetDelegateFetchCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR, PXDelegate PXREF pxDelegate);
PXPrivate PXResult PXAPI PXDotNetDelegateFetchHostFX(PXDotNetHostFX PXREF pxDotNetHostFX, PXDelegate PXREF pxDelegate);

PXPrivate PXResult PXAPI PXDotNetExecuteMSCoree(PXDotNetMSCoree PXREF pxDotNetMSCoree);
PXPrivate PXResult PXAPI PXDotNetExecuteCoreCLR(PXDotNetCoreCLR PXREF pxDotNetCoreCLR);
PXPrivate PXResult PXAPI PXDotNetExecuteHostFX(PXDotNetHostFX PXREF pxDotNetHostFX);
#endif

PXPublic PXResult PXAPI PXDotNetCompile(PXDotNet PXREF pxDotNet);

PXPublic PXResult PXAPI PXDotNetInitialize(PXDotNet PXREF pxDotNet, const PXI32U flagList);

#endif