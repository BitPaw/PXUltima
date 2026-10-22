
#include "PXOpenGL.h"
#include <PX/Type/PXWindow.hpp>

namespace PX
{
    class OpenGL : public PXOpenGL
    {
        public:
        OpenGL() { PXOpenGLConstruct(this); }
        ~OpenGL() { PXOpenGLDestruct(this); }

         PX::Result Initialize(PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo) { xxxx(this); }
         PX::Result Release(PX::Window& pxWindow) { PXOpenGLRelease(this); }
         PX::Result Select(PX::Window& pxWindow) { PXOpenGLSelect(this, &pxWindow); }
         PX::Result Deselect(PX::Window& pxWindow) { PXOpenGLDeselect(this); }
         PXI64S  IntergetGet(PXOpenGL PXREF pxOpenGL, const GLenum enumID) { PXOpenGLIntergetGet(); }
         PXResult  PXOpenGLDevicePhysicalListAmount(PXI32U PXREF amount) { PXOpenGLDevicePhysicalListAmount(this); }
         PXResult  PXOpenGLDevicePhysicalListFetch(const PXI32U amount, PXGPU PXREF pxGraphicsCardList) { PXOpenGLDevicePhysicalListFetch(this); }

         PXResult ScreenBufferRead(PXTexture2D PXREF PXTexture) { PXOpenGLScreenBufferRead(); }
         void Flush() { PXOpenGLFlush(this); }

         /*
         void PXOpenGLViewPortSet(PXOpenGL PXREF pxOpenGL, const PXViewPort PXREF pxViewPort);
         PXResult PXOpenGLViewPortGet(PXOpenGL PXREF pxOpenGL, PXViewPort PXREF pxViewPort);
         void PXOpenGLPolygonRenderOrder(const PXOpenGL PXREF pxOpenGL, const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);
         void PXOpenGLSettingChange(PXOpenGL PXREF pxOpenGL, const PXOpenGLToggle toggle, const PXBool state);
         void PXOpenGLDrawOrder(PXOpenGL PXREF pxOpenGL, const PXOpenGLDrawOrderMode openGLDrawOrderMode);
         void PXOpenGLDrawMode(PXOpenGL PXREF pxOpenGL, const PXDrawFillMode pxGraphicDrawFillMode, const int cullingMode);
         void PXOpenGLClear(PXOpenGL PXREF pxOpenGL, const PXColorRGBA_F32 PXREF PXColorRGBA_F32);
         PXBool PXOpenGLSceneDeploy(PXOpenGL PXREF pxOpenGL);
         void PXOpenGLDrawScaleF(PXOpenGL PXREF pxOpenGL, const PXF32 x, const PXF32 y, const PXF32 z);
         void PXOpenGLDrawBegin(PXOpenGL PXREF pxOpenGL, const PXDrawMode openGLRenderMode);
         void PXOpenGLDrawVertexXYZF(PXOpenGL PXREF pxOpenGL, const PXF32 x, const PXF32 y, const PXF32 z);
         void PXOpenGLDrawColorRGBF(PXOpenGL PXREF pxOpenGL, const PXF32 red, const PXF32 green, const PXF32 blue);
         void PXOpenGLDrawColorRGBAF(PXOpenGL PXREF pxOpenGL, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha);
         void PXOpenGLDrawEnd(PXOpenGL PXREF pxOpenGL);
         void PXOpenGLTextureParameter(PXOpenGL PXREF pxOpenGL, const PXTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXOpenGLTextureParameterValue openGLTextureParameterValue);
         void PXOpenGLTextureParameterI(PXOpenGL PXREF pxOpenGL, const PXTextureType textureType, const PXOpenGLTextureParameterMode pname, const int param);
         void PXOpenGLTextureParameterF(PXOpenGL PXREF pxOpenGL, const PXTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXF32 param);
         void PXOpenGLBlendingMode(PXOpenGL PXREF pxOpenGL, const PXBlendingMode pxBlendingMode); { PXOpenGLBlendingMode(); }


        //-------------------------------------------------------------------------

        // render

         void  PXOpenGLDrawArrays(const PXOpenGL PXREF pxOpenGL, const PXDrawMode renderMode, const PXSize offset, const PXSize amount);
         void  PXOpenGLDrawElements(const PXOpenGL PXREF pxOpenGL, const PXDrawMode renderMode, const PXSize amount, const PXI32U pxDataType, const void PXREF indexList);



         void  PXOpenGLDrawArraysInstanced(const PXOpenGL PXREF pxOpenGL, const PXDrawMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount);

        //-----------------------------------------------------
        // OpenGL - Texture
        //-----------------------------------------------------
         void  PXOpenGLTextureActivate(PXOpenGL PXREF pxOpenGL, const unsigned int index);
         void  PXOpenGLTexture2DDataWrite(PXOpenGL PXREF pxOpenGL, PXTexture2D PXREF pxTexture2D);
        //-------------------------------------------------------------------------

        //-----------------------------------------------------
        // OpenGL - Shader
        //-----------------------------------------------------
         PXResult  PXOpenGLShaderProgramRegister(PXOpenGL PXREF pxOpenGL, PXShaderProgram PXREF pxShaderProgram);
        PXPrivate PXResult  PXOpenGLShaderProgramLink(PXOpenGL PXREF pxOpenGL, PXShaderProgram PXREF pxShaderProgram);
        PXPrivate PXResult  PXOpenGLShaderCompile(PXOpenGL PXREF pxOpenGL, PXShader PXREF pxShader, const PXI32U shaderID);
        PXPrivate PXResult  PXOpenGLShaderProgramVariablesFetch(PXOpenGL PXREF pxOpenGL, PXShaderProgram PXREF pxShaderProgram);

        PXPrivate PXResult  PXOpenGLTexture2DRegister(PXOpenGL PXREF pxOpenGL, PXTexture2D PXREF pxTexture2D);
        PXPrivate PXResult  PXOpenGLTexture3DRegister(PXOpenGL PXREF pxOpenGL, PXTexture3D PXREF pxTexture3D);
        PXPrivate PXResult  PXOpenGLTextureCubeRegister(PXOpenGL PXREF pxOpenGL, PXTextureCube PXREF pxTextureCube);

        PXPrivate PXResult  PXOpenGLShaderComputeRegister(PXOpenGL PXREF pxOpenGL, PXShaderCompute PXREF pxShaderCompute);
        PXPrivate PXResult  PXOpenGLShaderGeometryRegister(PXOpenGL PXREF pxOpenGL, PXShaderGeometry PXREF pxShaderGeometry);
        PXPrivate PXResult  PXOpenGLShaderPixelFragmentRegister(PXOpenGL PXREF pxOpenGL, PXShaderPixelFragment PXREF pxShaderPixelFragment);
        PXPrivate PXResult  PXOpenGLShaderTessellationControlHullRegister(PXOpenGL PXREF pxOpenGL, PXShaderTessellationControlHull PXREF pxShaderTessellationControlHull);
        PXPrivate PXResult  PXOpenGLShaderTessellationEvaluationDomainRegister(PXOpenGL PXREF pxOpenGL, PXShaderTessellationEvaluationDomain PXREF pxShaderTessellationEvaluationDomain);
        PXPrivate PXResult  PXOpenGLShaderVertexRegister(PXOpenGL PXREF pxOpenGL, PXShaderVertex PXREF pxShaderVertex);




         PXResult  PXOpenGLShaderProgramSelect(PXOpenGL PXREF pxOpenGL, PXShaderProgram PXREF pxShaderProgram);
         PXResult  PXOpenGLShaderProgramDelete(PXOpenGL PXREF pxOpenGL, PXShaderProgram PXREF pxShaderProgram);

         PXResult  PXOpenGLShaderVariableSet(PXOpenGL PXREF pxOpenGL, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariableList, const PXSize amount);
        //-------------------------------------------------------------------------

        //---<Buffers>-------------------------------------------------------------
         void  PXOpenGLVertexArrayGenerate(PXOpenGL PXREF pxOpenGL, const unsigned int amount, unsigned int PXREF vaoList);
         void  PXOpenGLVertexArrayBind(PXOpenGL PXREF pxOpenGL, const unsigned int vaoID);
         void  PXOpenGLVertexArrayUnbind(PXOpenGL PXREF pxOpenGL);
         void  PXOpenGLVertexArrayAttributeDefine
        (
            PXOpenGL PXREF pxOpenGL,
            const PXI32U index,
            const PXI32U size,
            const PXI32U datatype,
            const PXBool normalized,
            const PXI32U stride,
            const PXSize offset
        );
         void  PXOpenGLVertexAttributeDivisor(PXOpenGL PXREF pxOpenGL, const PXSize index, const PXSize divisor);
         void  PXOpenGLVertexArrayEnable(PXOpenGL PXREF pxOpenGL, const unsigned int vertexArrayAtributeID);
         void  PXOpenGLVertexArrayDisable(PXOpenGL PXREF pxOpenGL, const unsigned int vertexArrayAtributeID);

         void  PXOpenGLBufferGenerate(PXOpenGL PXREF pxOpenGL, const unsigned int amount, unsigned int PXREF bufferIDList);
         void  PXOpenGLBufferBind(PXOpenGL PXREF pxOpenGL, const PXOpenGLBufferType bufferType, const unsigned int bufferID);
         void  PXOpenGLBufferData(PXOpenGL PXREF pxOpenGL, const PXOpenGLBufferType bufferType, const unsigned int size, const void PXREF data, const PXOpenGLStoreMode openGLStoreMode);
         void  PXOpenGLBufferUnbind(PXOpenGL PXREF pxOpenGL, const PXOpenGLBufferType bufferType);
        //-------------------------------------------------------------------------

        //-------------------------------------------------------------------------
         void  PXOpenGLFrameBufferCreate(PXOpenGL PXREF pxOpenGL, const unsigned int amount, unsigned int PXREF framebufferIDList);
         void  PXOpenGLRenderBufferStorage(PXOpenGL PXREF pxOpenGL, const PXOpenGLRenderBufferFormat internalformat, const int width, const int height);

        // If we bind to 0, we select the "main window"
         void  PXOpenGLFrameBufferBind(PXOpenGL PXREF pxOpenGL, const PXOpenGLFrameBufferMode target, const unsigned int framebufferID);

         void  PXOpenGLFrameBufferDestroy(PXOpenGL PXREF pxOpenGL, const unsigned int amount, unsigned int PXREF framebufferIDList);

         void  PXOpenGLRenderBufferCreate(PXOpenGL PXREF pxOpenGL, GLsizei n, GLuint* renderbuffers);


         void  PXOpenGLRenderBufferBind(PXOpenGL PXREF pxOpenGL, const unsigned int renderbuffer);

         void  PXOpenGLRenderBufferDelete(PXOpenGL PXREF pxOpenGL, GLsizei n, GLuint* renderbuffers);


         void  PXOpenGLFrameBufferLinkTexture2D(PXOpenGL PXREF pxOpenGL, const PXOpenGLRenderBufferAttachmentPoint attachment, const PXTextureType Arget, const unsigned int textureID, const int level);
         GLuint  PXOpenGLFrameBufferLinkRenderBuffer(PXOpenGL PXREF pxOpenGL, const PXOpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer);
        //-------------------------------------------------------------------------


        //---<PXOpenGL v.1.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.1.0>------------------------------------------------------
         void  PXOpenGLTexture2DFromScreen(PXOpenGL PXREF pxOpenGL, PXTexture2D PXREF pxTexture2D);
         void  PXOpenGLTexture2DFromScreenXYWH(PXOpenGL PXREF pxOpenGL, PXTexture2D PXREF pxTexture2D, const PXI32U x, const PXI32U y, const PXI32U width, const PXI32U height);
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.2.1>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.3.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.5.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.2.0.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.2.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.0.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------


        //---<PXOpenGL v.4.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.1.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.3.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.5.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.6.0>------------------------------------------------------
        //-------------------------------------------------------------------------


        //---<Extensions>----------------------------------------------------------
         PXResult  PXOpenGLSwapIntervalSet(PXOpenGL PXREF pxOpenGL, PXWindow PXREF pxWindow, const PXI32U interval);
         PXResult  PXOpenGLSwapIntervalGet(PXOpenGL PXREF pxOpenGL, PXI32U PXREF interval);
        //-------------------------------------------------------------------------



         PXResult  PXOpenGLSpriteRegister(PXOpenGL PXREF pxOpenGL, PXSprite PXREF pxSprite);

        //-----------------------------------------------------
        // Fixed Scripting
        //-----------------------------------------------------
         PXResult  PXOpenGLDrawScriptCreate(PXOpenGL PXREF pxOpenGL, PXDrawScript PXREF pxDrawScript);
         PXResult  PXOpenGLDrawScriptBegin(PXOpenGL PXREF pxOpenGL, PXDrawScript PXREF pxDrawScript);
         PXResult  PXOpenGLDrawScriptEnd(PXOpenGL PXREF pxOpenGL, PXDrawScript PXREF pxDrawScript);
         PXResult  PXOpenGLDrawScriptDelete(PXOpenGL PXREF pxOpenGL, PXDrawScript PXREF pxDrawScript);
         PXResult  PXOpenGLDrawScriptExecute(PXOpenGL PXREF pxOpenGL, PXDrawScript PXREF pxDrawScript);

        //-----------------------------------------------------
        // Light
        //-----------------------------------------------------
         PXResult  PXOpenGLLightSet(PXOpenGL PXREF pxOpenGL, PXLight PXREF pxLight, const PXI32U index);
         PXResult  PXOpenGLLightGet(PXOpenGL PXREF pxOpenGL, PXLight PXREF pxLight, const PXI32U index);
         PXResult  PXOpenGLLightEnableSet(PXOpenGL PXREF pxOpenGL, PXLight PXREF pxLight, const PXI32U index, const PXBool enable);
         PXResult  PXOpenGLLightEnableGet(PXOpenGL PXREF pxOpenGL, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable);



        //-----------------------------------------------------
        // PX-Extrended
        //-----------------------------------------------------
         PXResult  PXOpenGLRegisterModel(PXOpenGL PXREF pxOpenGL, PXModel PXREF pxModel);
         PXResult  PXOpenGLRegisterMesh(PXOpenGL PXREF pxOpenGL, PXMesh PXREF pxMesh);
         PXResult  PXOpenGLRegisterBufferVertex(PXOpenGL PXREF pxOpenGL, PXVertexBuffer PXREF pxVertexBuffer);
         PXResult  PXOpenGLRegisterBufferIndex(PXOpenGL PXREF pxOpenGL, PXIndexBuffer PXREF pxIndexBuffer);

        typedef struct PXGLDrawInfo_
        {
            const PXOpenGL* OpenGL;
            const PXScene* Scene;

            //PXSize ModelID;
            const PXModel* Model;

            PXSize MeshID;
            const PXMesh* Mesh;

            const PXShaderProgram* ShaderProgram;
            const PXIndexBuffer* IndexBuffer;
            const PXMaterial* Material;
        }
        PXGLDrawInfo;

         PXResult  PXOpenGLDrawModel
        (
            PXOpenGL PXREF pxOpenGL,
            PXScene PXREF pxScene,
            const PXModel PXREF pxModel
        );
         PXResult  PXOpenGLDrawMesh(PXGLDrawInfo PXREF pxGLDrawInfo);
         PXResult  PXOpenGLDrawIndexBuffer(PXGLDrawInfo PXREF pxGLDrawInfo);
         PXResult  PXOpenGLDrawSegment
        (
            PXGLDrawInfo PXREF pxGLDrawInfo,
            const PXSize startIndex,
            const PXSize startAmount,
            const PXSize drawMode
        );

         PXResult  PXOpenGLDrawMaterialApply(PXGLDrawInfo PXREF pxGLDrawInfo);
         PXResult  PXOpenGLDrawMatrixApply(PXGLDrawInfo PXREF pxGLDrawInfo);
         PXResult  PXOpenGLDrawTexturesApply(PXGLDrawInfo PXREF pxGLDrawInfo);

         PXSize  PXOpenGLIndexBufferIDCurrent(const PXOpenGL PXREF pxOpenGL);

         PXResult  PXOpenGLModelDraw(PXOpenGL PXREF pxOpenGL, const void* PXREF pxRenderEntity);
         PXResult  PXOpenGLModelDeregister(PXOpenGL PXREF pxOpenGL, PXModel PXREF pxModel);

         PXResult  PXOpenGLRectangleDraw(PXOpenGL PXREF pxOpenGL, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXI8U mode);
         PXResult  PXOpenGLRectangleDrawTx(PXOpenGL PXREF pxOpenGL, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXF32 txA, const PXF32 tyA, const PXF32 txB, const PXF32 tyB, const PXI8U mode);



        // FINAL
         PXBool  PXOpenGLShaderSupported(const PXOpenGL PXREF pxOpenGL);
         PXSize  PXOpenGLShaderProgramCurrent(const PXOpenGL PXREF pxOpenGL);

         PXResult  PXOpenGLShaderProgramApply
        (
            const PXOpenGL PXREF pxOpenGL,
            const PXShaderProgram PXREF pxShaderProgram
        );

        */
    };
}