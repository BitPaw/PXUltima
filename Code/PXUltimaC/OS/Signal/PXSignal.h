#ifndef OSSignalINCLUDE
#define OSSignalINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
        PXSignalTokenFloatMathError, // Floating-point errorc
        PXSignalTokenIllegalInstruction, // Illegal instruction
        PXSignalTokenINT, // CTRL+C signal
        PXSignalTokenMemoryViolation, // Illegal storage access
        PXSignalTokenTerminationRequest // Termination request
    }
    PXSignalToken;

    typedef void (PXCDECL* PXSignalCallBack)(const int signalID);

    PXPublic PXActionResult PXAPI PXSignalCallBackRegister(const PXSignalToken pxSignalToken, PXSignalCallBack pxSignalCallBack);

#ifdef __cplusplus
}
#endif

#endif
