#ifndef PXEngineInclude
#define PXEngineInclude

#include <Media/PXResource.h>
#include <Math/PXMath.h>
#include <OS/Signal/PXSignal.h>
#include <OS/Window/PXWindow.h>
#include <OS/Audio/PXAudio.h>
#include <Engine/PXMod.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXEngine_ PXEngine;

	typedef enum PXEngineCreateType_
	{
		PXEngineCreateTypeInvalid,
		PXEngineCreateTypeCustom,

		// Fudamental components
		PXEngineCreateTypeModel,
		PXEngineCreateTypeFont,
		PXEngineCreateTypeTexture,
		PXEngineCreateTypeShaderProgram,
		PXEngineCreateTypeSkybox,
		PXEngineCreateTypeSprite,
		PXEngineCreateTypeText,
		PXEngineCreateTypeTimer,
		PXEngineCreateTypeSound,
		PXEngineCreateTypeUIElement,

		// Extended basic components
		PXEngineCreateTypeDialogBox
	}
	PXEngineCreateType;



	//-----------------------------------------------------
	// Timer
	//-----------------------------------------------------
	typedef struct PXEngineTimerEventInfo_
	{
		struct PXEngineTimer_* TimerReference;

		PXInt32U Acuracy;

		PXBool WasHandled;

		//PXInt32S DelayShift;
	}
	PXEngineTimerEventInfo;

	typedef void (PXAPI*PXEngineTimerCallBack)(PXEngine* const pxEngine, PXEngineTimerEventInfo* const pxEngineTimerEventInfo, void* const owner);

	typedef struct PXEngineTimer_
	{
		PXInt32U PXID;
		PXBool Enabled; // Is the timer ticking?

		void* Owner;
		PXEngineTimerCallBack CallBack;

		PXInt32U TimeStampStart;

		PXInt32U TimeDeltaTarget;
		PXInt32S TimeDelayShift;	
	}
	PXEngineTimer;


	typedef struct PXEngineTimerCreateInfo_
	{
		PXEngineTimer* TimerReference;
	}
	PXEngineTimerCreateInfo;
	//-----------------------------------------------------



	//-----------------------------------------------------
	// Text
	//-----------------------------------------------------
	typedef struct PXEngineText_
	{
		PXInt32U PXID;
		PXBool Enabled;

		PXVector2F Position;
		PXVector2F Scaling;

		PXBool DoubleRenderForShadow;

		PXText* Text;
		PXFont* Font;
		float FontScaling;

		PXInt32U TextRenderAmount;
	}
	PXEngineText;

	typedef struct PXEngineTextCreateData_
	{
		PXEngineText* EngineTextReference;
	}
	PXEngineTextCreateData;
	//-----------------------------------------------------





	//-----------------------------------------------------
	// Audio
	//-----------------------------------------------------
	typedef struct PXEngineSound_
	{
		PXInt32U PXID;
		PXSound Sound;
		PXBool SoundLoop;
	}
	PXEngineSound;

	typedef struct PXEngineSoundCreateInfo_
	{
		PXEngineSound* EngineSoundReference;
		char* FilePath;
		PXBool SoundLoop;
	}
	PXEngineSoundCreateInfo;
	//-----------------------------------------------------



	//-----------------------------------------------------
	// ShaderProgram
	//-----------------------------------------------------
	typedef struct PXShaderProgramCreateData_
	{
		PXShaderProgram* ShaderProgrammReference;
		char* VertexShaderFilePath;
		char* PixelShaderFilePath;
	}
	PXShaderProgramCreateData;
	//-----------------------------------------------------


	typedef struct PXEngineUIElementCreateData_
	{
		PXUIElement* UIElementReference;
	}
	PXEngineUIElementCreateData;

	typedef struct PXModelCreateEventData_
	{
		PXModel* ModelReference;
		char* ModelFilePath;
	}
	PXModelCreateEventData;

	typedef struct PXEngineFontCreateData_
	{
		PXFont* FontReference;
		char* FontFilePath;
		PXShaderProgram* ShaderProgramCurrent;
	}
	PXEngineFontCreateData;

	typedef struct PXEngineTexture2DCreateData_
	{
		PXTexture2D* Texture2DReference;
		char* FilePath;
	}
	PXEngineTexture2DCreateData;

	typedef struct PXSkyBoxCreateEventData_
	{
		PXSkyBox* SkyboxReference;
		char* SkyBoxShaderVertex;
		char* SkyBoxShaderPixel;
		char* SkyBoxTextureA;
		char* SkyBoxTextureB;
		char* SkyBoxTextureC;
		char* SkyBoxTextureD;
		char* SkyBoxTextureE;
		char* SkyBoxTextureF;
	}
	PXSkyBoxCreateEventData;

	typedef struct PXSpriteCreateEventData_
	{
		PXSprite* SpriteReference;
		PXTexture2D* TextureCurrent;
		char* TextureName;
		PXShaderProgram* ShaderProgramCurrent;

		PXVector2F TextureScalingPoints[4];

		PXVector3F Position;
		PXVector2F Scaling;

		PXBool ViewRotationIgnore;
		PXBool ViewPositionIgnore;
	}
	PXSpriteCreateEventData;


	typedef struct PXEngineResourceCreateInfo_
	{
		PXEngineCreateType CreateType;

		PXBool SpawnEnabled;

		union
		{
			PXModelCreateEventData Model;
			PXEngineFontCreateData Font;
			PXSkyBoxCreateEventData SkyBox;
			PXSpriteCreateEventData Sprite;
			PXEngineTextCreateData Text;
			PXEngineUIElementCreateData UIElement;
			PXEngineTimerCreateInfo Timer;
			PXEngineSoundCreateInfo Sound;
			PXEngineTexture2DCreateData Texture2D;
			PXShaderProgramCreateData ShaderProgram;
		};
	}
	PXEngineResourceCreateInfo;











	typedef struct PXEngineModelRenderInfo_
	{
		PXModel* ModelReference;
	}
	PXEngineModelRenderInfo;

	typedef struct PXEngineSkyBoxRenderInfo_
	{
		PXSkyBox* SkyBoxReference;
	}
	PXEngineSkyBoxRenderInfo;

	typedef struct PXEngineSpriteRenderInfo_
	{
		PXSprite* SpriteReference;
	}
	PXEngineSpriteRenderInfo;

	typedef struct PXEngineTextRenderInfo_
	{
		PXEngineText* TextReference;
	}
	PXEngineTextRenderInfo;


	typedef struct PXEngineResourceRenderInfo_
	{
		PXEngineCreateType Type;

		PXCamera* CameraReference;

		union
		{
			PXEngineModelRenderInfo ModelRender;
			PXEngineSkyBoxRenderInfo SkyBoxRender;
			PXEngineSpriteRenderInfo SpriteRender;
			PXEngineTextRenderInfo TextRender;
		};
	}
	PXEngineResourceRenderInfo;






	//-----------------------------------------------------
	// StateChange
	//-----------------------------------------------------
	typedef struct PXEngineResourceStateChangeInfo_
	{
		PXEngineCreateType Type;
		void* Object;

		PXBool Enable;
	}
	PXEngineResourceStateChangeInfo;
	//-----------------------------------------------------




	typedef enum PXEngineResourceActionType_
	{
		PXEngineResourceActionTypeInvalid,
		PXEngineResourceActionTypeCustom,
		PXEngineResourceActionTypeCreate,
		PXEngineResourceActionTypeDestroy,
		PXEngineResourceActionTypeUpdate,
		PXEngineResourceActionTypeFetch,
		PXEngineResourceActionTypeSelect,
		PXEngineResourceActionTypeRender,
		PXEngineResourceActionTypePlay,
		PXEngineResourceActionTypeStateChange // Enable or Disable object
	}
	PXEngineResourceActionType;

	typedef struct PXEngineResourceActionInfo_
	{
		PXEngineResourceActionType Type;

		union
		{
			PXEngineResourceCreateInfo Create;
			PXEngineResourceRenderInfo Render;
			PXEngineResourceStateChangeInfo ChangeInfo;
		};
	}
	PXEngineResourceActionInfo;




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

	typedef void (PXAPI* PXEngineInteractCallBack)(void* const owner, PXEngine* const pxEngine);

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
		PXAudio Audio;

		PXAudioDevice AudioStandardOutDevice;

		void* Owner;
		PXCamera* CameraCurrent;

		PXEngineStartUpEvent OnStartUp;
		PXEngineShutDownEvent OnShutDown;
		PXEngineUserUpdateEvent OnUserUpdate;
		PXEngineNetworkUpdateEvent OnNetworkUpdate;
		PXEngineGameUpdateEvent OnGameUpdate;
		PXEngineRenderUpdateEvent OnRenderUpdate;

		PXEngineInteractCallBack OnInteract;

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

		// Register List
		PXInt32U UniqeIDGeneratorCounter;

		PXDictionary SpritelLookUp;
		PXDictionary FontLookUp;
		PXDictionary UIElementLookUp;
		PXDictionary TextLookUp;
		PXDictionary TimerLookUp;
		PXDictionary SoundLookUp;

		// Cached most-common objects
		PXModel SpriteScaled;
		PXModel SpriteUnScaled;

		PXMathRandomGeneratorSeed RandomGeneratorSeed;


		PXBool InteractionLock;
	}
	PXEngine;

	PXPrivate void PXCDECL PXEngineOnIllegalInstruction(const int signalID);
	PXPrivate void PXCDECL PXEngineOnMemoryViolation(const int signalID);
	PXPrivate PXInt32U PXAPI PXEngineGenerateUniqeID(PXEngine* const pxEngine);
	
	// Generate a random number with a maximum of the "limiter"
	PXPublic PXInt32U PXAPI PXEngineGenerateRandom(PXEngine* const pxEngine, const PXInt32U limiter);

	PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);

	PXPublic void PXAPI PXEngineStart(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);

	PXPublic PXActionResult PXAPI PXEngineResourceAction(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfo);
	PXPublic PXActionResult PXAPI PXEngineResourceActionBatch(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfoList, const PXSize amount);

	PXPublic PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXEngineResourceCreateInfo* const pxEngineResourceCreateInfo);
	PXPublic PXActionResult PXAPI PXEngineResourceRender(PXEngine* const pxEngine, PXEngineResourceRenderInfo* const pxEngineResourceRenderInfo);



	PXPublic PXActionResult PXAPI PXEngineSpriteTextureSet(PXEngine* const pxEngine, PXSprite* const pxSprite, PXTexture2D* const pxTexture2D);


#ifdef __cplusplus
}
#endif

#endif