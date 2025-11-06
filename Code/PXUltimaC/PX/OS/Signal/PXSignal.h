#pragma once

#ifndef OSSignalIncluded
#define OSSignalIncluded

#include <PX/Engine/PXResource.h>

#include <setjmp.h>
//#include <csetjmp> C++ lib

/*

#if OSUnix
// void OSSystemSignalCallBack(const int signalID);
#elif OSWindows
//void __CRTDECL OSSystemSignalCallBack(const int signalID);
#endif*/

typedef enum PXSignalToken_
{
    PXSignalTokenInvalid,
    PXSignalTokenAbort, // Abnormal termination
    PXSignalTokenPXF32MathError, // PXF32ing-point errorc
    PXSignalTokenIllegalInstruction, // Illegal instruction
    PXSignalTokenINT, // CTRL+C signal
    PXSignalTokenMemoryViolation, // Illegal storage access
    PXSignalTokenTerminationRequest // Termination request
}
PXSignalToken;

typedef void (PXCDECL* PXSignalCallBack)(const int signalID);

PXPublic PXResult PXAPI PXSignalCallBackRegister(const PXSignalToken pxSignalToken, PXSignalCallBack pxSignalCallBack);

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
