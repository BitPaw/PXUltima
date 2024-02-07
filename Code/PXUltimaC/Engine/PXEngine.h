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


	typedef enum PXEngineCreateType_
	{
		PXEngineCreateTypeInvalid,
		PXEngineCreateTypeCustom,
		PXEngineCreateTypeModel,
		PXEngineCreateTypeFont,
		PXEngineCreateTypeSkybox,
		PXEngineCreateTypeSprite,
		PXEngineCreateTypeText,
		PXEngineCreateTypeUIElement
	}
	PXEngineCreateType;

	typedef struct PXEngineText_
	{
		PXInt32U PXID;

		PXVector2F Position;
		PXVector2F Scaling;

		PXBool DoubleRenderForShadow;

		PXText Text;
		PXFont* Font;
		float FontScaling;
	}
	PXEngineText;

	typedef struct PXEngineTextCreateData_
	{
		PXEngineText* EngineTextReference;
	}
	PXEngineTextCreateData;

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

		union
		{
			PXModelCreateEventData Model;
			PXEngineFontCreateData Font;
			PXSkyBoxCreateEventData SkyBox;
			PXSpriteCreateEventData Sprite;
			PXEngineTextCreateData Text;
			PXEngineUIElementCreateData UIElement;
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

		// Register List
		PXInt32U UniqeIDGeneratorCounter;

		PXDictionary SpritelLookUp;
		PXDictionary FontLookUp;
		PXDictionary UIElementLookUp;
		PXDictionary TextLookUp;

		// Cached most-common objects
		PXModel SpriteScaled;
		PXModel SpriteUnScaled;
	}
	PXEngine;

	PXPrivate void PXCDECL PXEngineOnIllegalInstruction(const int signalID);
	PXPrivate void PXCDECL PXEngineOnMemoryViolation(const int signalID);
	PXPrivate PXInt32U PXAPI PXEngineGenerateUniqeID(PXEngine* const pxEngine);
	
	PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);

	PXPublic void PXAPI PXEngineStart(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);
	PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);


	PXPublic PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXEngineResourceCreateInfo* const pxEngineResourceCreateInfo);
	PXPublic PXActionResult PXAPI PXEngineResourceRender(PXEngine* const pxEngine, PXEngineResourceRenderInfo* const pxEngineResourceRenderInfo);

#ifdef __cplusplus
}
#endif

#endif