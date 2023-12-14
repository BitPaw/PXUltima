#pragma once

#include <OS/Graphic/PXGraphic.h>
#include <OS/Error/PXActionResult.hpp>
#include <Media/PXImage.hpp>
#include <Media/PXResource.hpp>

namespace PX
{
	class Font : public PXFont
	{

	};

	class Shader : public PXShader
	{


	};

	class Texture2D : public PXTexture2D
	{


	};

	class ViewPort : public PXViewPort
	{

	};

	enum class GraphicSystem
	{
		Invalid,
		OpenGL,
		DirectX,
		Vulcan
	};

	class Graphic
	{
		private:
		PXGraphic _internal;

		public:
		PXDLLExport Graphic();
		PXDLLExport ~Graphic();

		PXDLLExport PX::ActionResult Initialize(const PX::GraphicSystem graphicSystem);


		PXDLLExport PX::ActionResult ViewPortSet(const PX::ViewPort& pxViewPort);
		PXDLLExport PX::ActionResult ViewPortGet(PX::ViewPort& pxViewPort);


		PXDLLExport void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable);

		PXDLLExport void PXAPI PXShaderConstruct(PXShader* const shader);

		PXDLLExport void PXAPI PXShaderDataSet(PXShader* const shader, const ShaderType type, const char* data, const PXSize size);
		PXDLLExport void PXAPI PXTextureConstruct(PXTexture2D* const texture);
		PXDLLExport void PXAPI PXTextureDestruct(PXTexture2D* const texture);

		PXDLLExport PX::ActionResult PXAPI UIElementRegister(PXUIElement* const pxUIElement);
		PXDLLExport PX::ActionResult PXAPI UIElementUpdate(PXUIElement* const pxUIElement);
		PXDLLExport PX::ActionResult PXAPI UIElementUnregister(PXUIElement* const pxUIElement);
		//-------------------------------------------------------------------------

		//-----------------------------------------------------
		// Sprite
		//-----------------------------------------------------
		PXDLLExport PX::ActionResult PXAPI SpriteConstruct(PXSprite* const pxSprite);
		PXDLLExport PX::ActionResult PXAPI SpriteDraw(const PXSprite* const pxSprite, const PXCamera* const pxCamera);
		PXDLLExport PX::ActionResult PXAPI SpriteTextureLoadA(PXSprite* const pxSprite, const char* textureFilePath);
		PXDLLExport PX::ActionResult PXAPI SpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y);
		PXDLLExport PX::ActionResult PXAPI SpriteRegister(PXSprite* const pxSprite);


		PXDLLExport void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment);


		//-------------------------------------
		PXDLLExport PX::ActionResult PXAPI Instantiate(InitializeInfo* const pxGraphicInitializeInfo);
		PXDLLExport PX::ActionResult PXAPI Release();

		PXDLLExport void PXAPI HotSwap(const PX::GraphicSystem pxGraphicSystem);

		PXDLLExport void PXAPI ResourceRegister(ResourceInfo* const pxGraphicResourceInfo);
		//-------------------------------------

		//---<Shader>-----------------------------------------------------------------

		PXDLLExport void PXAPI ShaderUpdateMatrix4x4F(const unsigned int locationID, const float* const matrix4x4);

		//-------------------------------------------------------------------------

		// Load image resource and register it to prevent multible loads of the same file
		PXDLLExport PX::ActionResult PXAPI Texture2DLoad(PX::Image& pxImage, PX::Text& pxImageFilePath);


		//---<Texture>----------------------------------------------------------------
		PXDLLExport PX::ActionResult PXAPI TextureScreenShot(PX::Image& pxImage);

		PXDLLExport PX::ActionResult PXAPI Texture2DLoad(PX::Texture2D& texture, const PX::Text& filePath);
		PXDLLExport PX::ActionResult PXAPI Texture2DLoad(PX::Texture2D& texture, const char* const filePath);
		//-------------------------------------------------------------------------


		//---<Font>----------------------------------------------------------------------
		PXDLLExport PX::ActionResult PXAPI FontLoad(PX::Font& pxFont, const PX::Text& filePath);
		PXDLLExport PX::ActionResult PXAPI FontRegister(PX::Font& pxFont);
		PXDLLExport PX::ActionResult PXAPI FontRelease(PX::Font& pxFont);
		PXDLLExport PX::ActionResult PXAPI FontUse(PX::Font& pxFont);
		//-------------------------------------------------------------------------


		//---<Model>---------------------------------------------------------------
		PXDLLExport PX::ActionResult PXAPI SkyboxRegister(PX::SkyBox& const skyBox);
		PXDLLExport PX::ActionResult PXAPI SkyboxRegister
		(

			PX::SkyBox& const skyBox,
			const PX::Text& const shaderVertex,
			const PX::Text& const shaderFragment,
			const PX::Text& const textureRight,
			const PX::Text& const textureLeft,
			const PX::Text& const textureTop,
			const PX::Text& const textureBottom,
			const PX::Text& const textureBack,
			const PX::Text& const textureFront
		);
		PXDLLExport PX::ActionResult PXAPI SkyboxRegister
		(

			PX::SkyBox& const skyBox,
			const char* const shaderVertex,
			const char* const shaderFragment,
			const char* const textureRight,
			const char* const textureLeft,
			const char* const textureTop,
			const char* const textureBottom,
			const char* const textureBack,
			const char* const textureFront
		);
		PXDLLExport PX::ActionResult PXAPI SkyboxUse(PX::SkyBox& skyBox);
		PXDLLExport PX::ActionResult PXAPI SkyboxRelease(PX::SkyBox& skyBox);

		//PXDLLExport PXSize ModelListSize(const );
		//PXDLLExport PXBool ModelListGetFromIndex(const  PXModel** pxModel, const PXSize index);

		PXDLLExport PXSize PXAPI RenderableListSize(const);
		PXDLLExport PXBool PXAPI RenderableListGetFromIndex(const  PXRenderable** pxRenderable, const PXSize index);

		//PXDLLExport PX::ActionResult ModelCreate( PXModel** const pxModel);
		//PXDLLExport PXBool ModelRegister( PXModel* const pxModel);
		PXDLLExport PX::ActionResult PXAPI RenderableCreate(PXRenderable** const pxRenderable);
		PXDLLExport PXBool PXAPI RenderableRegister(PXRenderable* const pxRenderable);


		PXDLLExport void PXAPI ModelShaderSet(PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram);

	};
}