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


	typedef struct PXSkyBoxCreateEventData_
	{
		int dummy;
	}
	PXSkyBoxCreateEventData;

	typedef struct PXSpriteCreateEventData_
	{
		PXSprite* SpriteReference;
		PXTexture2D* TextureCurrent;
		char* TextureName;
		PXShaderProgram* ShaderProgramCurrent;

		PXBool ViewRotationIgnore;
		PXBool ViewPositionIgnore;
	}
	PXSpriteCreateEventData;


	typedef struct PXPlayerMoveInfo_
	{
		PXVector3F MovementWalk;
		PXVector3F MovementView;
		PXBool IsWindowInFocus;
		PXBool ActionCommit; // If this stays true, action will be commited
	}
	PXPlayerMoveInfo;


	typedef void (PXAPI* PXEngineStartUpEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineShutDownEvent)(void* const owner, PXEngine* const pxEngine);

	typedef void (PXAPI* PXEngineUserUpdateEvent)(void* const owner, PXEngine* const pxEngine, PXPlayerMoveInfo* const pxPlayerMoveInfo);
	typedef void (PXAPI* PXEngineNetworkUpdateEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineGameUpdateEvent)(void* const owner, PXEngine* const pxEngine);
	typedef void (PXAPI* PXEngineRenderUpdateEvent)(void* const owner, PXEngine* const pxEngine);

	typedef struct PXEngine_
	{
		PXGraphic Graphic;
		PXWindow Window;
		PXModLoader ModLoader;
		PXCamera CameraDefault;

		void* Owner;
		PXCamera* CameraCurrent;

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


	
	PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);

	PXPublic void PXAPI PXEngineStart(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);

	PXPublic PXActionResult PXAPI PXSkyBoxCreate(PXEngine* const pxEngine, PXSkyBoxCreateEventData* const pxSkyBoxCreateEventData);
	PXPublic PXActionResult PXAPI PXSpriteCreate(PXEngine* const pxEngine, PXSpriteCreateEventData* const pxSpriteCreateEventData);

#ifdef __cplusplus
}
#endif

#endif