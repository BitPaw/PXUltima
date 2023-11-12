#ifndef PXEngineInclude
#define PXEngineInclude

#include <Media/PXResource.h>
#include <OS/Signal/PXSignal.h>
#include <OS/Window/PXWindow.h>
#include <Engine/PXMod.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXEngine_ PXEngine;

	typedef void (PXAPI* PXEngineStartUpEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineShutDownEvent)(void* const owner, PXEngine* const pxEngine);

	typedef void (PXAPI* PXEngineUserUpdateEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineNetworkUpdateEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineGameUpdateEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineRenderUpdateEvent)(void* const owner, PXEngine* const pxEngine);

	typedef struct PXEngine_
	{
		PXGraphic Graphic;
		PXWindow Window;
		PXModLoader ModLoader;

		void* Owner;

		PXEngineStartUpEvent OnStartUp;
		PXEngineShutDownEvent OnShutDown;
		PXEngineUserUpdateEvent OnUserUpdate;
		PXEngineNetworkUpdateEvent OnNetworkUpdate;
		PXEngineGameUpdateEvent OnGameUpdate;
		PXEngineRenderUpdateEvent OnRenderUpdate;

		PXInt32U CounterTimeLast;
		PXInt32U CounterTimeDelta;

		PXInt32U CounterTimeWindow;
		PXInt32U CounterTimeUser;
		PXInt32U CounterTimeNetwork;
		PXInt64U CounterTimeRenderLast;
		PXInt32U CounterTimeGPU;
		PXInt32U CounterTimeCPU;
		PXInt32U TimeFrequency;

		PXInt32U FramesPerSecound;
		PXInt32U FrameTime;

		PXBool IsRunning;
	}
	PXEngine;

	PXPrivate void PXCDECL PXEngineOnIllegalInstruction(const int signalID);
	PXPrivate void PXCDECL PXEngineOnMemoryViolation(const int signalID);

	PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);
	
	PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);

	PXPublic void PXAPI PXEngineStart(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);

#ifdef __cplusplus
}
#endif

#endif