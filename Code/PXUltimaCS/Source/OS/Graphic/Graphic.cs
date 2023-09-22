using System;
using System.Runtime.InteropServices;

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

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramCreateFunction(UIntPtr graphicAPI, ref PXShaderProgram  pxShaderProgram);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramSelectFunction(UIntPtr graphicAPI, ref PXShaderProgram  pxShaderProgram);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicShaderProgramDeleteFunction(UIntPtr graphicAPI, ref PXShaderProgram  pxShaderProgram);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicSceneBeginFunction(UIntPtr graphicAPI);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate void PXGraphicSceneEndFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptCreate(UIntPtr graphicAPI, ref PXDrawScript  pxDrawScript,  PXDrawScriptType pxDrawScriptType);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptBegin(UIntPtr graphicAPI, ref PXDrawScript  pxDrawScript);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDrawScriptEnd(UIntPtr graphicAPI, ref PXDrawScript  pxDrawScript);
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


    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicRectangleDrawFunction(UIntPtr graphicAPI, float xA, float yA, float xB, float yB);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicRectangleDrawTxFunction(UIntPtr graphicAPI, float xA, float yA, float xB, float yB, float txA, float tyA, float txB, float tyB);



    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal unsafe delegate void PXGraphicClearFunction(UIntPtr graphicAPI,  PXColorRGBAF* backgroundColor);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate byte PXGraphicSceneDeployFunction(UIntPtr graphicAPI);

    [UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDevicePhysicalListAmountFunction(UIntPtr graphicAPI, ref uint  amount);
    //[UnmanagedFunctionPointer(CallingConvention.StdCall)] internal delegate PX.ActionResult PXGraphicDevicePhysicalListFetchFunction(UIntPtr graphicAPI,  uint amount, ref PXGraphicDevicePhysical  pxGraphicDevicePhysicalList);



    public class Graphic : IDisposable
    {
        [DllImport("PXUltima.dll",  CallingConvention = CallingConvention.StdCall)] internal static extern PX.ActionResult PXGraphicInstantiate(ref PXGraphic pxGraphic, int width, int height, ref PXWindow pxWindow);

        [StructLayout(LayoutKind.Sequential, Size = 9000)]
        internal unsafe struct PXGraphic
        {
            //-------------------
            // References
            //-------------------
            public UIntPtr AttachedWindow;
            public UIntPtr EventOwner;
            //-------------------

            //-------------------
            // Functions
            //-------------------       
            public PXGraphicInitializeFunction Initialize;
            public PXGraphicReleaseFunction Release;
            public PXGraphicSelectFunction Select;
            public PXGraphicDeselectFunction Deselect;
            public PXGraphicClearFunction Clear;
            public PXGraphicSceneDeployFunction SceneDeploy;
            public PXGraphicViewPortSetFunction ViewPortSet;
            public PXGraphicViewPortGetFunction ViewPortGet;
            public PXGraphicShaderProgramCreateFunction ShaderProgramCreate;
            public PXGraphicShaderProgramSelectFunction ShaderProgramSelect;
            public PXGraphicShaderProgramDeleteFunction ShaderProgramDelete;
            public PXGraphicSceneBeginFunction SceneBegin;
            public PXGraphicSceneEndFunction SceneEnd;
            public PXGraphicDrawScriptCreate DrawScriptCreate;
            public PXGraphicDrawScriptBegin DrawScriptBegin;
            public PXGraphicDrawScriptEnd DrawScriptEnd;
            public PXGraphicDrawScriptDelete DrawScriptDelete;
            public PXGraphicDrawScriptExecute DrawScriptExecute;
            public PXGraphicTextureCubeRegisterFunction TextureCubeRegister;
            public PXGraphicTextureCubeRegisterUseFunction TextureCubeRegisterUse;
            public PXGraphicTextureCubeReleaseFunction TextureCubeRelease;
            public PXGraphicVertexStructureRegisterFunction VertexStructureRegister;
            public PXGraphicVertexStructureDrawFunction VertexStructureDraw;
            public PXGraphicVertexStructureSelectFunction VertexStructureSelect;
            public PXGraphicVertexStructureReleaseFunction VertexStructureRelease;
            public PXGraphicLightSetFunction LightSet;
            public PXGraphicLightGetFunction LightGet;
            public PXGraphicLightEnableSetFunction LightEnableSet;
            public PXGraphicLightEnableGetFunction LightEnableGet;
            public PXGraphicTexture2DRegisterFunction Texture2DRegister;
            public PXGraphicTexture2DReleaseFunction Texture2DRelease;
            public PXGraphicTexture2DSelectFunction Texture2DSelect;
            public PXGraphicSwapIntervalSetFunction SwapIntervalSet;
            public PXGraphicSwapIntervalGetFunction SwapIntervalGet;
            public PXGraphicRectangleDrawFunction RectangleDraw;
            public PXGraphicRectangleDrawTxFunction RectangleDrawTx;
            public PXGraphicDevicePhysicalListAmountFunction DevicePhysicalListAmount;
            //  public PXGraphicDevicePhysicalListFetchFunction DevicePhysicalListFetch;

            public fixed char DeadBuffer[5000];
        }

        private PXGraphic _pxGraphic;

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

                if (_pxGraphic.DevicePhysicalListAmount  == null)
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

        public PX.ActionResult Initialize(PX.Window pxWindow)
        {
            return PXGraphicInstantiate(ref _pxGraphic, int.MaxValue, int.MaxValue, ref pxWindow._pxWindow);
        }

        public PX.ActionResult Initialize(ulong width, ulong height)
        {
            return 0;// _pxGraphic.Initialize(ref _pxGraphic, width, height, );
        }
        public byte Release()
        {
            return _pxGraphic.Release(_pxGraphic.EventOwner);
        }
        public void Select()
        {
            _pxGraphic.Select(_pxGraphic.EventOwner);
        }
        public void Deselect()
        {
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
        /*
        public PX.ActionResult ViewPortSet(PXViewPort  pxViewPort)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult ViewPortGet(PXViewPort  pxViewPort)
        {
            return ActionResult.NotImplemented;
        }

        public PX.ActionResult ShaderProgramCreate( PXShaderProgram  pxShaderProgram)
        {

        }
        public PX.ActionResult ShaderProgramSelect( PXShaderProgram  pxShaderProgram)
        {

        }
        public PX.ActionResult ShaderProgramDelete( PXShaderProgram  pxShaderProgram);

        public void SceneBegin();
        public void SceneEnd();

        public PX.ActionResult DrawScriptCreate( PXDrawScript*  pxDrawScript,  PXDrawScriptType pxDrawScriptType);
        public PX.ActionResult DrawScriptBegin( PXDrawScript*  pxDrawScript);
        public PX.ActionResult DrawScriptEnd( PXDrawScript*  pxDrawScript);
        public PX.ActionResult DrawScriptDelete( PXDrawScript*  pxDrawScript);
        public PX.ActionResult DrawScriptExecute( PXDrawScript*  pxDrawScript);

        public PX.ActionResult TextureCubeRegister( PXTextureCube*  textureCube);
        public PX.ActionResult TextureCubeRegisterUse( PXTextureCube*  textureCube);
        public PX.ActionResult TextureCubeRelease( PXTextureCube*  textureCube);


        public PX.ActionResult VertexStructureRegister( PXVertexStructure*  pxVertexStructure);
        public PX.ActionResult VertexStructureDraw( PXVertexStructure*  pxVertexStructure,  PXCamera*  pxCamera);
        public PX.ActionResult VertexStructureSelect( PXVertexStructure*  pxVertexStructure);
        public PX.ActionResult VertexStructureRelease( PXVertexStructure*  pxVertexStructure);

        public PX.ActionResult LightSet( PXLight*  pxLight,  PXInt32U index);
        public PX.ActionResult LightGet( PXLight*  pxLight,  PXInt32U index);
        public PX.ActionResult LightEnableSet( PXLight*  pxLight,  PXInt32U index,  PXBool enable);
        public PX.ActionResult LightEnableGet( PXLight*  pxLight,  PXInt32U index, PXBool*  enable);


        public PX.ActionResult Texture2DRegister( PXTexture2D*  pxTexture2D);
        public PX.ActionResult Texture2DRelease( PXTexture2D*  pxTexture2D);
        public PX.ActionResult Texture2DSelect( PXTexture2D*  pxTexture2D);

    

        public PX.ActionResult DevicePhysicalListAmount(void*  graphicAPI, PXInt32U*  amount);
        public PX.ActionResult DevicePhysicalListFetch(void*  graphicAPI,  PXInt32U amount, DevicePhysical*  pxGraphicDevicePhysicalList);*/

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

        public unsafe PX.ActionResult RectangleDraw(float xA, float yA, float xB, float yB)
        {
            return _pxGraphic.RectangleDraw(_pxGraphic.EventOwner, xA, yA, xB, yB);
        }
        public PX.ActionResult RectangleDrawTx(float xA, float yA, float xB, float yB, float txA, float tyA, float txB, float tyB)
        {
            return _pxGraphic.RectangleDrawTx(_pxGraphic.EventOwner, xA, yA, xB, yB, txA, tyA, txB, tyB);
        }
    }
}