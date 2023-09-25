using System;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;

namespace PX
{
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate byte PXGraphicInitializeFunction(UIntPtr graphicAPI, ulong width, ulong height, ref PX.Window pxWindow);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate byte PXGraphicReleaseFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicSelectFunction(UIntPtr graphicAPI);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicDeselectFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicLoadFunction(UIntPtr graphicAPI);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicUnloaddFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicViewPortSetFunction(UIntPtr graphicAPI, ref PXViewPort pxViewPort);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicViewPortGetFunction(UIntPtr graphicAPI, ref PXViewPort pxViewPort);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramCreateFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramSelectFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramDeleteFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicSceneBeginFunction(UIntPtr graphicAPI);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicSceneEndFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptCreate(UIntPtr graphicAPI, ref PXDrawScript pxDrawScript, PXDrawScriptType pxDrawScriptType);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptBegin(UIntPtr graphicAPI, ref PXDrawScript pxDrawScript);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptEnd(UIntPtr graphicAPI, ref PXDrawScript pxDrawScript);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptDelete(UIntPtr graphicAPI, ref PXDrawScript pxDrawScript);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptExecute(UIntPtr graphicAPI, ref PXDrawScript pxDrawScript);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTextureCubeRegisterFunction(UIntPtr graphicAPI, ref PXTextureCube textureCube);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTextureCubeRegisterUseFunction(UIntPtr graphicAPI, ref PXTextureCube textureCube);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTextureCubeReleaseFunction(UIntPtr graphicAPI, ref PXTextureCube textureCube);


    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicVertexStructureRegisterFunction(UIntPtr graphicAPI, ref PXVertexStructure pxVertexStructure);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicVertexStructureDrawFunction(UIntPtr graphicAPI, ref PXVertexStructure pxVertexStructure, ref PXCamera pxCamera);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicVertexStructureSelectFunction(UIntPtr graphicAPI, ref PXVertexStructure pxVertexStructure);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicVertexStructureReleaseFunction(UIntPtr graphicAPI, ref PXVertexStructure pxVertexStructure);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicLightSetFunction(UIntPtr graphicAPI, ref PXLight pxLight, uint index);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicLightGetFunction(UIntPtr graphicAPI, ref PXLight pxLight, uint index);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicLightEnableSetFunction(UIntPtr graphicAPI, ref PXLight pxLight, uint index, byte enable);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicLightEnableGetFunction(UIntPtr graphicAPI, ref PXLight pxLight, uint index, ref byte enable);


    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTexture2DRegisterFunction(UIntPtr graphicAPI, ref PXTexture2D pxTexture2D);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTexture2DReleaseFunction(UIntPtr graphicAPI, ref PXTexture2D pxTexture2D);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicTexture2DSelectFunction(UIntPtr graphicAPI, ref PXTexture2D pxTexture2D);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicSwapIntervalSetFunction(UIntPtr graphicAPI, uint interval);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicSwapIntervalGetFunction(UIntPtr graphicAPI, ref uint interval);


    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicRectangleDrawFunction(UIntPtr graphicAPI, float xA, float yA, float xB, float yB, byte mode);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicRectangleDrawTxFunction(UIntPtr graphicAPI, float xA, float yA, float xB, float yB, float txA, float tyA, float txB, float tyB);



    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal unsafe delegate void PXGraphicClearFunction(UIntPtr graphicAPI, PXColorRGBAF* backgroundColor);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate byte PXGraphicSceneDeployFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDevicePhysicalListAmountFunction(UIntPtr graphicAPI, ref uint amount);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDevicePhysicalListFetchFunction(UIntPtr graphicAPI,  uint amount, ref PXGraphicDevicePhysical  pxGraphicDevicePhysicalList);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawColorRGBFFunction(UIntPtr graphicAPI, float red, float green, float blue, float alpha);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawModeSetFunction(UIntPtr graphicAPI, GraphicDrawFillMode pxGraphicDrawFillMode);



    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramCreateFromFileVPFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram, ref PXText vertexShaderFilePath, ref PXText fragmentShaderFilePath);
	 [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal unsafe delegate PX.ActionResult PXGraphicShaderProgramCreateFromFileVPAFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram, char* vertexShaderFilePath, char* fragmentShaderFilePath);
	 [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramCreateFromStringVPFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram, ref PXText vertexShaderFilePath, ref PXText fragmentShaderFilePath);
	 [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal unsafe delegate PX.ActionResult PXGraphicShaderProgramCreateFromStringVPAFunction(UIntPtr graphicAPI, ref PXShaderProgram pxShaderProgram, char* vertexShaderFilePath, char* fragmentShaderFilePath);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal unsafe delegate PX.ActionResult PXGraphicShaderVariableIDFetchFunction(UIntPtr graphicAPI, ref PXShader pxShader, ref uint shaderVariableID, byte* name);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx1Function(UIntPtr graphicAPI, uint location, float v0);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx1xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx1Function(UIntPtr graphicAPI, uint location, uint v0);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx1xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref uint[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx2Function(UIntPtr graphicAPI, uint location, float v0, float v1);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx2xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx2Function(UIntPtr graphicAPI, uint location, uint v0, uint v1);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx2xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref uint[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx3Function(UIntPtr graphicAPI, uint location, float v0, float v1, float v2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx3xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx3Function(UIntPtr graphicAPI, uint location, uint v0, uint v1, uint v2);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx3xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref uint[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx4Function(UIntPtr graphicAPI, uint location, float v0, float v1, float v2, float v3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableFx4xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx4Function(UIntPtr graphicAPI, uint location, uint UIntPtr, uint v1, uint v2, uint v3);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableIx4xNFunction(UIntPtr graphicAPI, uint location, UIntPtr count, ref uint[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableMatrix2fvFunction(UIntPtr graphicAPI, uint location, UIntPtr count, byte transpose, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableMatrix3fvFunction(UIntPtr graphicAPI, uint location, UIntPtr count, byte transpose, ref float[] value);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderVariableMatrix4fvFunction(UIntPtr graphicAPI, uint location, UIntPtr count, byte transpose, ref float[] value);
    //-------------------------------------------------------------------------



    public class Graphic : IDisposable
    {
        [DllImport("PXUltima.dll", CallingConvention = CallingConvention.StdCall)] internal static extern PX.ActionResult PXGraphicInstantiate(ref PXGraphic pxGraphic, ref PXGraphicInitializeInfo pxGraphicInitializeInfo);

        [StructLayout(LayoutKind.Sequential, Size = 9000)]
        internal unsafe struct PXGraphic
        {
            //-------------------
            // References
            //-------------------
            public UIntPtr AttachedWindow;
            public UIntPtr EventOwner;
            //-------------------

            //-------------------------------------------------
            // Core
            //-------------------------------------------------
            public PXGraphicInitializeFunction Initialize;
            public PXGraphicReleaseFunction Release;
            public PXGraphicSelectFunction Select;
            public PXGraphicDeselectFunction Deselect;
            public PXGraphicClearFunction Clear;

            //-------------------------------------------------
            // Info & Settings
            //-------------------------------------------------
            public PXGraphicSwapIntervalSetFunction SwapIntervalSet;
            public PXGraphicSwapIntervalGetFunction SwapIntervalGet;
            public PXGraphicDevicePhysicalListAmountFunction DevicePhysicalListAmount;
            public PXGraphicDevicePhysicalListFetchFunction DevicePhysicalListFetch;

            //-------------------------------------------------
            // Scene
            //-------------------------------------------------
            public PXGraphicSceneBeginFunction SceneBegin;
            public PXGraphicSceneDeployFunction SceneDeploy;
            public PXGraphicSceneEndFunction SceneEnd;

            //-------------------------------------------------
            // View
            //-------------------------------------------------
            public PXGraphicViewPortSetFunction ViewPortSet;
            public PXGraphicViewPortGetFunction ViewPortGet;

            //-------------------------------------------------
            // Scripting
            //-------------------------------------------------
            public PXGraphicDrawScriptCreate DrawScriptCreate;
            public PXGraphicDrawScriptBegin DrawScriptBegin;
            public PXGraphicDrawScriptEnd DrawScriptEnd;
            public PXGraphicDrawScriptDelete DrawScriptDelete;
            public PXGraphicDrawScriptExecute DrawScriptExecute;

            //-------------------------------------------------
            // Textures
            //-------------------------------------------------
            public PXGraphicTexture2DRegisterFunction Texture2DRegister;
            public PXGraphicTexture2DReleaseFunction Texture2DRelease;
            public PXGraphicTexture2DSelectFunction Texture2DSelect;

            public PXGraphicTextureCubeRegisterFunction TextureCubeRegister;
            public PXGraphicTextureCubeRegisterUseFunction TextureCubeRegisterUse;
            public PXGraphicTextureCubeReleaseFunction TextureCubeRelease;


            //-------------------------------------------------
            // Model
            //-------------------------------------------------
            public PXGraphicVertexStructureRegisterFunction VertexStructureRegister;
            public PXGraphicVertexStructureDrawFunction VertexStructureDraw;
            public PXGraphicVertexStructureSelectFunction VertexStructureSelect;
            public PXGraphicVertexStructureReleaseFunction VertexStructureRelease;

            //-------------------------------------------------
            // Light
            //-------------------------------------------------
            public PXGraphicLightSetFunction LightSet;
            public PXGraphicLightGetFunction LightGet;
            public PXGraphicLightEnableSetFunction LightEnableSet;
            public PXGraphicLightEnableGetFunction LightEnableGet;

            //-------------------------------------------------
            // Shader
            //-------------------------------------------------
            public PXGraphicShaderProgramCreateFromFileVPFunction ShaderProgramCreateFromFileVP;
            public PXGraphicShaderProgramCreateFromFileVPAFunction ShaderProgramCreateFromFileVPA;
            public PXGraphicShaderProgramCreateFromStringVPFunction ShaderProgramCreateFromStringVP;
            public PXGraphicShaderProgramCreateFromStringVPAFunction ShaderProgramCreateFromStringVPA;
            public PXGraphicShaderProgramCreateFunction ShaderProgramCreate;
            public PXGraphicShaderProgramSelectFunction ShaderProgramSelect;
            public PXGraphicShaderProgramDeleteFunction ShaderProgramDelete;
            public PXGraphicShaderVariableIDFetchFunction ShaderVariableIDFetch;
            public PXGraphicShaderVariableFx1Function ShaderVariableFx1;
            public PXGraphicShaderVariableFx1xNFunction ShaderVariableFx1xN;
            public PXGraphicShaderVariableIx1Function ShaderVariableIx1;
            public PXGraphicShaderVariableIx1xNFunction ShaderVariableIx1xN;
            public PXGraphicShaderVariableFx2Function ShaderVariableFx2;
            public PXGraphicShaderVariableFx2xNFunction ShaderVariableFx2xN;
            public PXGraphicShaderVariableIx2Function ShaderVariableIx2;
            public PXGraphicShaderVariableIx2xNFunction ShaderVariableIx2xN;
            public PXGraphicShaderVariableFx3Function ShaderVariableFx3;
            public PXGraphicShaderVariableFx3xNFunction ShaderVariableFx3xN;
            public PXGraphicShaderVariableIx3Function ShaderVariableIx3;
            public PXGraphicShaderVariableIx3xNFunction ShaderVariableIx3xN;
            public PXGraphicShaderVariableFx4Function ShaderVariableFx4;
            public PXGraphicShaderVariableFx4xNFunction ShaderVariableFx4xN;
            public PXGraphicShaderVariableIx4Function ShaderVariableIx4;
            public PXGraphicShaderVariableIx4xNFunction ShaderVariableIx4xN;
            public PXGraphicShaderVariableMatrix2fvFunction ShaderVariableMatrix2fv;
            public PXGraphicShaderVariableMatrix3fvFunction ShaderVariableMatrix3fv;
            public PXGraphicShaderVariableMatrix4fvFunction ShaderVariableMatrix4fv;

            //-------------------------------------------------
            // Draw
            //-------------------------------------------------
            public PXGraphicDrawModeSetFunction DrawModeSet;
            public PXGraphicDrawColorRGBFFunction DrawColorRGBAF;
            public PXGraphicRectangleDrawFunction RectangleDraw;
            public PXGraphicRectangleDrawTxFunction RectangleDrawTx;
            //-------------------------------------------------

            public fixed char DeadBuffer[5000];
        }

        private PXGraphic _pxGraphic;

        public ViewPort ViewPortCurrent
        {
            get
            {
                ViewPort viewPort = null;

                if (_pxGraphic.ViewPortGet != null)
                {
                    viewPort = new ViewPort();

                    _pxGraphic.ViewPortGet(_pxGraphic.EventOwner, ref viewPort._pxViewPort);
                }

                return viewPort;
            }
            set
            {
                _pxGraphic.ViewPortSet(_pxGraphic.EventOwner, ref value._pxViewPort);
            }
        }

        public uint SwapIntervall
        {
            get
            {
                uint number = 0;

                if (_pxGraphic.SwapIntervalGet == null)
                {
                    return uint.MaxValue;
                }

                _pxGraphic.SwapIntervalGet(_pxGraphic.EventOwner, ref number);

                return number;
            }
            set
            {
                _pxGraphic.SwapIntervalSet(_pxGraphic.EventOwner, value);
            }
        }

        public uint DevicePhysicalListAmount
        {
            get
            {
                uint number = 0;

                if (_pxGraphic.DevicePhysicalListAmount == null)
                {
                    return uint.MaxValue;
                }

                _pxGraphic.DevicePhysicalListAmount(_pxGraphic.EventOwner, ref number);

                return number;
            }
        }

        public Graphic()
        {
            _pxGraphic = new PXGraphic();
        }

        public void Dispose()
        {
            Release();
        }

        public unsafe PX.ActionResult Initialize(PX.Window pxWindow, GraphicSystem pxGraphicSystem)
        {
            fixed (PXWindow* ptr = &pxWindow._pxWindow)
            {
                PXGraphicInitializeInfo* pXGraphicInitializeInfo = stackalloc PXGraphicInitializeInfo[1];
                pXGraphicInitializeInfo->WindowReference = ptr;
                pXGraphicInitializeInfo->Width = (UIntPtr)ulong.MaxValue;
                pXGraphicInitializeInfo->Height = (UIntPtr)ulong.MaxValue;
                pXGraphicInitializeInfo->DirectXVersion = DirectXVersion.DX9;
                pXGraphicInitializeInfo->DirectXDriverType = DirectXDriverType.HardwareDevice;
                pXGraphicInitializeInfo->GraphicSystem = pxGraphicSystem;

                return PXGraphicInstantiate(ref _pxGraphic, ref *pXGraphicInitializeInfo);
            }     
        }

        public PX.ActionResult Initialize(ulong width, ulong height, GraphicSystem pxGraphicSystem)
        {
            return 0;// _pxGraphic.Initialize(ref _pxGraphic, width, height, );
        }
        public byte Release()
        {
            return _pxGraphic.Release(_pxGraphic.EventOwner);
        }
        public void Select()
        {
            if (_pxGraphic.Select == null)
            {
                return;            
            }

            _pxGraphic.Select(_pxGraphic.EventOwner);
        }
        public void Deselect()
        {
            if ( _pxGraphic.Deselect == null )
            {
                return;             
            }

            _pxGraphic.Deselect(_pxGraphic.EventOwner);
        }

        public PX.ActionResult Load()
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult Unloadd()
        {
            return ActionResult.NotImplemented;
        }

        public void SceneBegin()
        {
            _pxGraphic.SceneBegin(_pxGraphic.EventOwner);
        }
        public void SceneEnd()
        {
            _pxGraphic.SceneEnd(_pxGraphic.EventOwner);
        }

        public PX.ActionResult DrawScriptCreate(DrawScript drawScript, PXDrawScriptType pxDrawScriptType)
        {
            return _pxGraphic.DrawScriptCreate(_pxGraphic.EventOwner, ref drawScript._pxDrawScript, pxDrawScriptType);
        }
        public PX.ActionResult DrawScriptBegin(DrawScript drawScript)
        {
            return _pxGraphic.DrawScriptBegin(_pxGraphic.EventOwner, ref drawScript._pxDrawScript);
        }
        public PX.ActionResult DrawScriptEnd(DrawScript drawScript)
        {
            return _pxGraphic.DrawScriptEnd(_pxGraphic.EventOwner, ref drawScript._pxDrawScript);
        }
        public PX.ActionResult DrawScriptDelete(DrawScript drawScript)
        {
            return _pxGraphic.DrawScriptDelete(_pxGraphic.EventOwner, ref drawScript._pxDrawScript);
        }
        public PX.ActionResult DrawScriptExecute(DrawScript drawScript)
        {
            return _pxGraphic.DrawScriptExecute(_pxGraphic.EventOwner, ref drawScript._pxDrawScript);
        }

        public PX.ActionResult TextureCubeRegister(TextureCube textureCube)
        {
            return _pxGraphic.TextureCubeRegister(_pxGraphic.EventOwner, ref textureCube._pxTextureCube);
        }

        public PX.ActionResult TextureCubeRegisterUse(TextureCube textureCube)
        {
            return _pxGraphic.TextureCubeRegisterUse(_pxGraphic.EventOwner, ref textureCube._pxTextureCube);
        }

        public PX.ActionResult TextureCubeRelease(TextureCube textureCube)
        {
            return _pxGraphic.TextureCubeRelease(_pxGraphic.EventOwner, ref textureCube._pxTextureCube);
        }

        public PX.ActionResult VertexStructureRegister(VertexStructure vertexStructure)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult VertexStructureDraw(VertexStructure vertexStructure, Camera camera)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult VertexStructureSelect(VertexStructure vertexStructure)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult VertexStructureRelease(VertexStructure vertexStructure)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult LightSet(Light pxLight, uint index)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult LightGet(Light pxLight, uint index)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult LightEnableSet(Light pxLight, uint index, bool enable)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult LightEnableGet(Light pxLight, uint index, ref bool enable)
        {
            return PX.ActionResult.NotImplemented;
        }

        public PX.ActionResult Texture2DRegister(PX.Texture2D pxTexture2D)
        {
            return _pxGraphic.Texture2DRegister(_pxGraphic.EventOwner, ref pxTexture2D._texture2D);
        }

        public PX.ActionResult Texture2DRelease(PX.Texture2D pxTexture2D)
        {
            return _pxGraphic.Texture2DRelease(_pxGraphic.EventOwner, ref pxTexture2D._texture2D);
        }

        public PX.ActionResult Texture2DSelect(PX.Texture2D pxTexture2D)
        {
            return _pxGraphic.Texture2DSelect(_pxGraphic.EventOwner, ref pxTexture2D._texture2D);
        }

        public PX.ActionResult DevicePhysicalListFetch(List<GraphicDevicePhysical> pxGraphicDevicePhysicalList)
        {
            return PX.ActionResult.NotImplemented;
        }

        public unsafe void Clear(float red, float green, float blue, float alpha)
        {
            PXColorRGBAF* pXColorRGBAF = stackalloc PXColorRGBAF[1];
            pXColorRGBAF->Alpha = alpha;
            pXColorRGBAF->Red = red;
            pXColorRGBAF->Green = green;
            pXColorRGBAF->Blue = blue;
            _pxGraphic.Clear(_pxGraphic.EventOwner, pXColorRGBAF);
        }

        public byte SceneDeploy()
        {
            return _pxGraphic.SceneDeploy(_pxGraphic.EventOwner);
        }

        public PX.ActionResult DrawMode(GraphicDrawFillMode graphicDrawFillMode)
        {
            return _pxGraphic.DrawModeSet(_pxGraphic.EventOwner, graphicDrawFillMode);
        }

        public PX.ActionResult DrawColor(float red, float green, float blue)
        {
            return DrawColor(red, green, blue, 1);
        }

        public PX.ActionResult DrawColor(float red, float green, float blue, float alpha)
        {
            return _pxGraphic.DrawColorRGBAF(_pxGraphic.EventOwner, red, green, blue, alpha);
        }

        public unsafe PX.ActionResult RectangleDraw(float xA, float yA, float xB, float yB)
        {
            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, xA, yA, xB, yB, 0x01);
        }
        public unsafe PX.ActionResult RectangleDraw(float xA, float yA, float xB, float yB, float red, float green, float blue)
        {
            DrawColor(red, green, blue);

            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, xA, yA, xB, yB, 0x01);
        }
        public unsafe PX.ActionResult RectangleDraw(float xA, float yA, float xB, float yB, float red, float green, float blue, float alpha)
        {
            DrawColor(red, green, blue, alpha);

            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, xA, yA, xB, yB, 0x01);
        }
        public unsafe PX.ActionResult RectangleDrawOffset(float left, float bottom, float right, float top)
        {
            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, left, bottom, right, top, 0x02);
        }
        public unsafe PX.ActionResult RectangleDrawOffset(float left, float bottom, float right, float top, float red, float green, float blue)
        {
            DrawColor(red, green, blue);

            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, left, bottom, right, top, 0x02);
        }
        public unsafe PX.ActionResult RectangleDrawOffset(float left, float bottom, float right, float top, float red, float green, float blue, float alpha)
        {
            DrawColor(red, green, blue, alpha);

            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, left, bottom, right, top, 0x02);
        }
        public PX.ActionResult RectangleDrawTx(float xA, float yA, float xB, float yB, float txA, float tyA, float txB, float tyB)
        {
            return _pxGraphic.RectangleDrawTx(_pxGraphic.EventOwner, xA, yA, xB, yB, txA, tyA, txB, tyB);
        }

        public unsafe PX.ActionResult ShaderProgramCreateFromFile(ShaderProgram shaderProgram, string vertexShaderFilePath, string fragmentShaderFilePath)
        {
            fixed (char* vertexShaderAdress = vertexShaderFilePath.ToCharArray())
            {
                PXText vertexShaderText = PXText.MakeFromStringW(vertexShaderAdress, vertexShaderFilePath.Length);

                fixed (char* fragmentShaderAdress = fragmentShaderFilePath.ToCharArray())
                {
                    PXText fragmentShaderText = PXText.MakeFromStringW(fragmentShaderAdress, fragmentShaderFilePath.Length);

                    return _pxGraphic.ShaderProgramCreateFromFileVP(_pxGraphic.EventOwner, ref shaderProgram._pxShaderProgram, ref vertexShaderText, ref fragmentShaderText);
                }
            }
        }
        public unsafe PX.ActionResult ShaderProgramCreateFromString(ShaderProgram shaderProgram, string vertexShader, string fragmentShader)
        {
            fixed (char* vertexShaderAdress = vertexShader.ToCharArray())
            {
                PXText vertexShaderText = PXText.MakeFromStringW(vertexShaderAdress, vertexShader.Length);

                fixed (char* fragmentShaderAdress = fragmentShader.ToCharArray())
                {
                    PXText fragmentShaderText = PXText.MakeFromStringW(fragmentShaderAdress, fragmentShader.Length);

                    return _pxGraphic.ShaderProgramCreateFromFileVP(_pxGraphic.EventOwner, ref shaderProgram._pxShaderProgram, ref vertexShaderText, ref fragmentShaderText);
                }
            }
        }
        public PX.ActionResult ShaderProgramCreate(ShaderProgram shaderProgram)
        {
            return _pxGraphic.ShaderProgramCreate(_pxGraphic.EventOwner, ref shaderProgram._pxShaderProgram);
        }
        public PX.ActionResult ShaderProgramSelect(ShaderProgram shaderProgram)
        {
            return _pxGraphic.ShaderProgramSelect(_pxGraphic.EventOwner, ref shaderProgram._pxShaderProgram);
        }
        public PX.ActionResult ShaderProgramDelete(ShaderProgram shaderProgram)
        {
            return _pxGraphic.ShaderProgramDelete(_pxGraphic.EventOwner, ref shaderProgram._pxShaderProgram);
        }
        public unsafe PX.ActionResult ShaderVariableIDFetch(Shader shader, ref uint shaderVariableID, string name)
        {
            int size = name.Length;
            byte* bytes = stackalloc byte[260];

            for (int i = 0; i < size; i++) //             byte[] bytes = Encoding.ASCII.GetBytes(inputString);
            {
                bytes[i] = (byte)(name[i] > 0xFF ? '?' : name[i]);
            }

            return _pxGraphic.ShaderVariableIDFetch(_pxGraphic.EventOwner, ref shader._pxShader, ref shaderVariableID, bytes);
        }
        public PX.ActionResult ShaderVariable(uint location, float v0)
        {
            return _pxGraphic.ShaderVariableFx1(_pxGraphic.EventOwner, location, v0);
        }
        public PX.ActionResult ShaderVariable(uint location, UIntPtr count, float[] value)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ShaderVariable(uint location, uint v0)
        {
            return _pxGraphic.ShaderVariableIx1(_pxGraphic.EventOwner, location, v0);
        }
        public PX.ActionResult ShaderVariable(uint location, UIntPtr count, uint[] value)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ShaderVariable(uint location, float v0, float v1)
        {
            return _pxGraphic.ShaderVariableFx2(_pxGraphic.EventOwner, location, v0, v1);
        }
        public PX.ActionResult ShaderVariable(uint location, uint v0, uint v1)
        {
            return _pxGraphic.ShaderVariableIx2(_pxGraphic.EventOwner, location, v0, v1);
        }
        public PX.ActionResult ShaderVariable(uint location, float v0, float v1, float v2)
        {
            return _pxGraphic.ShaderVariableFx3(_pxGraphic.EventOwner, location, v0, v1, v2);
        }
        public PX.ActionResult ShaderVariable(uint location, uint v0, uint v1, uint v2)
        {
            return _pxGraphic.ShaderVariableIx3(_pxGraphic.EventOwner, location, v0, v1, v2);
        }
        public PX.ActionResult ShaderVariable(uint location, float v0, float v1, float v2, float v3)
        {
            return _pxGraphic.ShaderVariableFx4(_pxGraphic.EventOwner, location, v0, v1, v2, v3);
        }
        public PX.ActionResult ShaderVariable(uint location, uint UIntPtr, uint v1, uint v2, uint v3)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ShaderVariableMatrix2fv(uint location, UIntPtr count, bool transpose, float[] value)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ShaderVariableMatrix3fv(uint location, UIntPtr count, bool transpose, float[] value)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ShaderVariableMatrix4fv(uint location, UIntPtr count, bool transpose, float[] value)
        {
            return ActionResult.NotImplemented;
        }
    }
}