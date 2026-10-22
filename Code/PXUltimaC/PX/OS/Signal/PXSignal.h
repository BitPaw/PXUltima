#pragma once

#ifndef OSSignalIncluded
#define OSSignalIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Async/PXProcess.h>

#include <setjmp.h>
#include <PX/OS/Async/PXThread.h>
//#include <csetjmp> C++ lib

#if OSUnix
#define _GNU_SOURCE        // Linux: enables REG_RIP, REG_RSP, etc.
#define _XOPEN_SOURCE 700  // POSIX: enables siginfo_t
#include <signal.h>
#elif OSWindows
#endif // OSUnix

/*

#if OSUnix
// void OSSystemSignalCallBack(const int signalID);
#elif OSWindows
//void __CRTDECL OSSystemSignalCallBack(const int signalID);
#endif*/


typedef struct PXSignalInfo_
{
    int ID;
    PXResult Result;

    PXThreadID ThreadID;
    PXProcessID ProcessID;

    void* FaultAddress; // Memory address that caused fault

    PXSize ParameterAmount;

    PXThreadContext Context;
}
PXSignalInfo;


#if OSUnix
PXPrivate PXResult PXAPI PXSignalIDToResult(const int signalID);
PXPrivate void PXCDECL PXSignalCallBack_Sigaction(int sig, siginfo_t* info, void* ucontext);
PXPrivate void PXCDECL PXSignalCallBack_Signal(const int signalID);
#elif OSWindows
PXPrivate LONG WINAPI PXSignalCallBack_Exception(EXCEPTION_POINTERS* exceptionInfo);
#endif


typedef void (PXAPI* PXSignalCallBackFunction)(PXSignalInfo PXREF pxSignalInfo);

PXPrivate void PXAPI PXSignalDeploy(PXSignalInfo PXREF pxSignalInfo);


PXPublic void PXAPI PXSignalCallBackSet(PXSignalCallBackFunction pxSignalCallBackFunction);


// Current state of the processor.
// Is used to restore a point in execution
typedef struct PXRecoveryPoint_
{
    jmp_buf Enviroment;
}
PXRecoveryPoint;

PXPublic PXResult PXAPI PXRecoveryPointSet(PXRecoveryPoint PXREF pxRecoveryPoint);
PXPublic PXResult PXAPI PXRecoveryPointRestore(PXRecoveryPoint PXREF pxRecoveryPoint);

#endif
