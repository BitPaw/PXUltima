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
		PXEngineCreateTypeTexture2D,

		PXEngineCreateTypeImage,
		PXEngineCreateTypeTextureCube,

		PXEngineCreateTypeShaderProgram,
		PXEngineCreateTypeSkybox,
		PXEngineCreateTypeSprite,
		PXEngineCreateTypeText,
		PXEngineCreateTypeTimer,
		PXEngineCreateTypeSound,
		PXEngineCreateTypeEngineSound,
		PXEngineCreateTypeUIElement,

		PXEngineCreateTypeHitBox,

		// Extended basic components
		PXEngineCreateTypeDialogBox
	}
	PXEngineCreateType;

	PXPublic const char* PXAPI PXEngineCreateTypeToString(const PXEngineCreateType pxEngineCreateType);


#define PXEngineResourceInfoExist			0b00000001// Indicated deleted Resource
#define PXEngineResourceInfoEnabled			0b00000010// Shall it be rendered? Does it tick?
#define PXEngineResourceInfoStorageDrive	0b00010000// Resource is in permanent storage
#define PXEngineResourceInfoStorageCached	0b00100000// Resource is in semi-permanent cache (temp file)
#define PXEngineResourceInfoStorageMemory	0b01000000// Resource exists in RAM
#define PXEngineResourceInfoStorageDevice	0b10000000// Resource exists in spesific device

	typedef struct PXEngineResourceInfo_
	{
		PXInt32U ID;
		PXInt32U Flags;
	}
	PXEngineResourceInfo;



	//-----------------------------------------------------
	// HitBox
	//-----------------------------------------------------
	typedef enum PXHitBoxForm_
	{
		PXHitBoxTypeInvalid,
		PXHitBoxTypeBox,
		PXHitBoxTypeCube,
		PXHitBoxTypeCircle

	}
	PXHitBoxForm;

	//---------------
	// BehaviourFlag
	//---------------
#define PXHitBoxBehaviourKeepOut 0b00000001 // Prevent from entering 
#define PXHitBoxBehaviourKeepIn  0b00000010 // Prevent from leaving
#define PXHitBoxBehaviourDetect  0b00000100 // Trigger if in inside
//#define PXHitBoxBehaviourDetect  0b00001000 // Trigger if in inside
	//---------------

	typedef enum PXHitBoxCollisionVarriant_
	{
		PXHitBoxCollisionVarriantInvalid,
		PXHitBoxCollisionVarriantEnter,
		PXHitBoxCollisionVarriantLeave
	}
	PXHitBoxCollisionVarriant;

	typedef void(PXAPI* PXHitBoxCollisionDetect)(void* owner, struct PXHitBox_* const pxHitBox, const PXHitBoxCollisionVarriant pxHitBoxCollisionVarriant);

	typedef struct PXHitBox_
	{
		PXInt32U PXID;
		PXBool Enabled;

		PXMatrix4x4F* ModelPosition;
		
		PXInt8U BehaviourFlag;
		PXHitBoxForm Form;

		void* CallBackOwner;
		PXHitBoxCollisionDetect CollisionDetectCallBack;

	}
	PXHitBox;
	//-----------------------------------------------------



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

	typedef PXActionResult(PXAPI*PXEngineTimerCallBack)(PXEngine* const pxEngine, PXEngineTimerEventInfo* const pxEngineTimerEventInfo, void* const owner);

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
	//-----------------------------------------------------





	//-----------------------------------------------------
	// Audio
	//-----------------------------------------------------
	typedef struct PXEngineSound_
	{
		PXSound* Sound;

		PXInt32U PXID;
		
		PXBool SoundLoop;
	}
	PXEngineSound;

	typedef struct PXEngineSoundCreateInfo_
	{
		PXBool SoundLoop;
	}
	PXEngineSoundCreateInfo;
	//-----------------------------------------------------



	//-----------------------------------------------------
	// ShaderProgram
	//-----------------------------------------------------
	typedef struct PXShaderProgramCreateData_
	{
		char* VertexShaderFilePath;
		char* PixelShaderFilePath;
	}
	PXShaderProgramCreateData;
	//-----------------------------------------------------


	typedef struct PXEngineFontCreateData_
	{
		PXShaderProgram* ShaderProgramCurrent;
	}
	PXEngineFontCreateData;

	typedef struct PXSkyBoxCreateEventData_
	{
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
		PXTexture2D* TextureCurrent;
		PXShaderProgram* ShaderProgramCurrent;

		PXVector2F TextureScalingPoints[4];

		PXBool HitBoxCreate;
		PXHitBox* HitBox;

		PXVector3F Position;
		PXVector2F Scaling;

		PXBool ViewRotationIgnore;
		PXBool ViewPositionIgnore;
	}
	PXSpriteCreateEventData;


	typedef struct PXTextureCubeCreateData_
	{
		char* FilePathA;
		char* FilePathB;
		char* FilePathC;
		char* FilePathD;
		char* FilePathE;
		char* FilePathF;
	}
	PXTextureCubeCreateData;

	typedef struct PXModelCreateInfo_
	{
		PXShaderProgram* ShaderProgramReference;
		float Scale;
	}
	PXModelCreateInfo;


	typedef struct PXUIElementSceneRenderInfo_
	{
		struct PXEngine_* Engine;
	}
	PXUIElementSceneRenderInfo;


	typedef enum PXUIElementTreeViewItemInsertMode_
	{
		PXUIElementTreeViewItemInsertModeROOT,
		PXUIElementTreeViewItemInsertModeFIRST,
		PXUIElementTreeViewItemInsertModeLAST,
		PXUIElementTreeViewItemInsertModeSORT
	}
	PXUIElementTreeViewItemInsertMode;

	typedef struct PXUIElementTreeViewItemInfo_
	{
		char* TextDataOverride;
		PXSize TextSizeOverride;

		struct PXUIElement_* ItemParent;
		struct PXUIElement_* TreeView;

		PXUIElementTreeViewItemInsertMode InsertMode;

		// Result
		struct _TREEITEM* ItemHandle;
	}
	PXUIElementTreeViewItemInfo;

	typedef struct PXUIElementCreateData_
	{
		struct PXWindow_* WindowReference;
		struct PXUIElement_* Paranet;

		PXUIElementType Type;
		PXInt32U BehaviourFlags;
		PXColorRGBAF* ColorTintReference;

		PXColorRGBAF Color;

		PXUIElementPosition Position;

		PXBool OSButton;

		// Text
		union
		{
			PXUIElementTextInfo TextInfo;
			PXUIElementButtonInfo ButtonInfo;
			PXUIElementTreeViewItemInfo TreeViewItem;
			PXUIElementSceneRenderInfo SceneRender;
		};

	
		PXUIOnMouseEnter OnMouseEnterCallback;
		PXUIOnMouseLeave OnMouseLeaveCallback;
	}
	PXUIElementCreateData;





	typedef struct PXEngineResourceCreateInfo_
	{
		void** ObjectReference;
		char* FilePath;

		char* Name;

		PXEngineCreateType CreateType;

		PXBool SpawnEnabled;

		union
		{
			PXEngineFontCreateData Font;
			PXSkyBoxCreateEventData SkyBox;
			PXSpriteCreateEventData Sprite;
			PXEngineSoundCreateInfo Sound;
			PXShaderProgramCreateData ShaderProgram;
			PXTextureCubeCreateData TextureCube;
			PXUIElementCreateData UIElement;
			PXModelCreateInfo Model;
			PXUIElementSceneRenderInfo SceneRender;
		
		};
	}
	PXEngineResourceCreateInfo;












	typedef struct PXEngineResourceRenderInfo_
	{
		PXCamera* CameraReference; // Camera required for rendering

		void* ObjectReference; // Containing the object, type described in 'Type'

		PXEngineCreateType Type;
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


	typedef void (PXAPI* PXEngineResourceAdded)(void* const owner, PXEngine* const pxEngine, PXEngineResourceCreateInfo* const pxEngineResourceCreateInfo);

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

		PXEngineResourceAdded ResourceAdded;

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
		PXDictionary HitBoxLookUp;
		PXDictionary ImageLookUp;
		PXDictionary TextureLookUp;
		PXDictionary ModelLookUp;
		PXDictionary ShaderProgramLookup;

		// Cached most-common objects
		PXModel SpriteScaled;
		PXModel SpriteUnScaled;

		PXMathRandomGeneratorSeed RandomGeneratorSeed;

		PXBool HasGraphicInterface;
		PXBool InteractionLock;

		char ApplicationName[64];
	}
	PXEngine;

	PXPrivate void PXCDECL PXEngineOnIllegalInstruction(const int signalID);
	PXPrivate void PXCDECL PXEngineOnMemoryViolation(const int signalID);
	PXPrivate PXInt32U PXAPI PXEngineGenerateUniqeID(PXEngine* const pxEngine);
	PXPrivate void PXAPI PXEngineWindowLookupHelper(PXEngine* const pxEngine, PXWindowHelperLookupInfo* const pxWindowHelperLookupInfo);

	// Generate a random number with a maximum of the "limiter"
	PXPublic PXInt32U PXAPI PXEngineGenerateRandom(PXEngine* const pxEngine, const PXInt32U limiter);

	PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);

	typedef struct PXEngineStartInfo_
	{
		PXGraphicInitializeMode Mode;
		PXGraphicSystem System;

		PXSize Width;
		PXSize Height;

		struct PXUIElement_* UIElement;
	}
	PXEngineStartInfo;

	PXPublic PXActionResult PXAPI PXEngineStart(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);
	PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);

	PXPublic PXActionResult PXAPI PXEngineResourceAction(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfo);
	PXPublic PXActionResult PXAPI PXEngineResourceActionBatch(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfoList, const PXSize amount);

	PXPublic PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXEngineResourceCreateInfo* const pxEngineResourceCreateInfo);
	PXPublic PXActionResult PXAPI PXEngineResourceRender(PXEngine* const pxEngine, PXEngineResourceRenderInfo* const pxEngineResourceRenderInfo);


	PXPublic void PXAPI PXEngineCollsisionSolve(PXEngine* const pxEngine);

	PXPublic PXActionResult PXAPI PXEngineSpriteTextureSet(PXEngine* const pxEngine, PXSprite* const pxSprite, PXTexture2D* const pxTexture2D);


#ifdef __cplusplus
}
#endif

#endif