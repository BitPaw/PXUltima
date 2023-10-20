using System;
using System.Drawing;
using System.Runtime.InteropServices;

namespace PX
{
    public enum GraphicSystem
    {
        Invalid,
        OpenGL,
        DirectX,
        Vulcan
    }

    public enum RefreshRateMode
    {
        Unlimited,
        VSync,
        CustomSync
    }


    public enum PXGraphicResourceLocation
    {
        PXGraphicResourceLocationInvalid,
        PXGraphicResourceLocationFullyUnloaded, // Resource is known but not loaded
        PXGraphicResourceLocationLoadedNotRegisterd, // Rsource is loaded but not known by external rendering systems
        PXGraphicResourceLocationLoadAndRegistered, // Resource si loaded and registerd
        PXGraphicResourceLocationRegisteredOnly, // Resouce is uploaded and not loaded anymore.
    }


    public enum PXGraphicResourceType
    {
        PXGraphicResourceTypeInvalid,
        PXGraphicResourceTypeModel,
        PXGraphicResourceTypeTexure,
        PXGraphicResourceTypeSkyBox,
        PXGraphicResourceTypeFont
    }



    public enum PXGraphicRenderFilter
    {
        PXGraphicRenderFilterInvalid,
        PXGraphicRenderFilterNoFilter, // No filter Option, use this for Pixelated Textures.
        PXGraphicRenderFilterBilinear, // Level I Filter
        PXGraphicRenderFilterTrilinear // Level II Filter
    }


    public enum PXGraphicShaderType
    {
        PXShaderTypeInvalid,
        PXShaderTypeVertex,     // .vert - a vertex shader
        PXShaderTypeFragment,   // .frag - a fragment shader
        PXShaderTypeTessellationControl,    // .tesc - a tessellation control shader
        PXShaderTypeTessellationEvaluation,     // .tese - a tessellation evaluation shader
        PXShaderTypeGeometry,      // .geom - a geometry shader
        PXShaderTypeCompute,   // .comp - a compute shader
    }


    public enum PXGraphicImageLayout
    {
        PXGraphicImageLayoutInvalid,

        // Returns the value of the texture element that is nearest to the specified texture coordinates.
        PXGraphicImageLayoutNearest,

        // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
        // These can include items wrapped or repeated from other parts of a texture, depending on the values of GL_TEXTURE_WRAP_Sand GL_TEXTURE_WRAP_T, and on the exact mapping.
        PXGraphicImageLayoutLinear,

        //Chooses the mipmap that most closely matches the size of the pixel being texturedand
        //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates) to produce a texture value.
        PXGraphicImageLayoutMipMapNearestNearest,

        //Chooses the mipmap that most closely matches the size of the pixel being texturedand
        //uses the GL_LINEAR criterion(a weighted average of the four texture elements that are closest to the specified texture coordinates) to produce a texture value.
        PXGraphicImageLayoutMipMapLinearNearest,

        //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
        //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates)
        //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
        PXGraphicImageLayoutMipMapNNearestLinear,

        //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
        //uses the GL_LINEAR criterion(a weighted average of the texture elements that are closest to the specified texture coordinates)
        //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
        PXGraphicImageLayoutMipMapLinearLinear
    }


    public enum PXGraphicImageWrap
    {
        PXGraphicImageWrapInvalid,

        // Images will be used 'as is' and will not be streched whatsoever.
        PXGraphicImageWrapNoModification,

        // Strech
        PXGraphicImageWrapStrechEdges,

        PXGraphicImageWrapStrechEdgesAndMirror,

        // Tiles the image in a gridformat
        PXGraphicImageWrapRepeat,

        // Tiles the image in a gridformat but also flip them every time.
        PXGraphicImageWrapRepeatAndMirror,
    }


    public enum PXGraphicTextureType
    {
        PXGraphicTextureTypeInvalid,

        PXGraphicTextureType1D,
        PXGraphicTextureType1DArray,
        PXGraphicTextureType1DBuffer,
        PXGraphicTextureType2D,
        PXGraphicTextureType2DArray,
        PXGraphicTextureType2DProxy,
        PXGraphicTextureType3D,

        PXGraphicTextureTypeCubeContainer,
        PXGraphicTextureTypeCubeProxy,
        PXGraphicTextureTypeCubeArray,
        PXGraphicTextureTypeCubeRight,
        PXGraphicTextureTypeCubeLeft,
        PXGraphicTextureTypeCubeTop,
        PXGraphicTextureTypeCubeDown,
        PXGraphicTextureTypeCubeBack,
        PXGraphicTextureTypeCubeFront,

        PXGraphicTextureTypeBuffer,
        PXGraphicTextureTypeRectangle,
        PXGraphicTextureTypeRectangleProxy,

        PXGraphicTextureType2DMultiSample,
        PXGraphicTextureType2DMultiSampleArray
    }


    public enum PXGraphicDrawMode
    {
        PXGraphicDrawModeInvalid,
        PXGraphicDrawModePoint,
        PXGraphicDrawModeLine,
        PXGraphicDrawModeLineLoop,
        PXGraphicDrawModeLineStrip,
        PXGraphicDrawModeLineStripAdjacency,
        PXGraphicDrawModeLineAdjacency,
        PXGraphicDrawModeTriangle,
        PXGraphicDrawModeTriangleAdjacency,
        PXGraphicDrawModeTriangleFAN,
        PXGraphicDrawModeTriangleStrip,
        PXGraphicDrawModeTriangleStripAdjacency,
        PXGraphicDrawModeWireFrame,
        PXGraphicDrawModeSquare,
        PXGraphicDrawModeSquareStrip,
        PXGraphicDrawModePatches
    }


    public enum GraphicDrawFillMode
    {
        Invalid,
        Points,
        Lines,
        Fill
    }



    public enum PXVertexBufferFormat
    {
        PXVertexBufferFormatInvalid,



        // OpenGL

        PXVertexBufferFormatXY, // GL_V2F
        PXVertexBufferFormatC4UB_XY,
        PXVertexBufferFormatC4UB_XYZ,
        PXVertexBufferFormatRGBXYZ,
        PXVertexBufferFormatN3F_XYZ,
        PXVertexBufferFormatC4F_N3F_XYZ,
        PXVertexBufferFormatT2F_XYZ,
        PXVertexBufferFormatT4F_XYZW,
        PXVertexBufferFormatT2F_C4UB_XYZ,
        PXVertexBufferFormatT2F_C3F_XYZ,
        PXVertexBufferFormatT2F_N3F_XYZ,
        PXVertexBufferFormatT2F_C4F_N3F_XYZ,
        PXVertexBufferFormatT4F_C4F_N3F_XYZW,

        // Direct X

        PXVertexBufferFormatXYZ, // Normal spcace (x, y, z)
        PXVertexBufferFormatXYZC, // Normal spcace (x, y, z, color-RGB as 32-int)

        PXVertexBufferFormatXYZRHW, // DirectX only, use for pixel size instead of normal space.

        PXVertexBufferFormatXYZHWC, // X, Y, Z, Tx, Ty, color-RGB as 32-int

        PXVertexBufferFormatXYZB1,
        PXVertexBufferFormatXYZB2,
        PXVertexBufferFormatXYZB3,
        PXVertexBufferFormatXYZB4,
        PXVertexBufferFormatXYZB5,
        PXVertexBufferFormatXYZW,
    }

    // To keep track of the object reference.
    // OpenGL uses 32-Bit Integer as an ID.
    // DirectX uses direct pointers to object references.
    struct PXResourceID
    {
        UIntPtr DirectXInterface;
        uint OpenGLID;
        uint PXID;
    }



    // Offset the whole child-Container
    // (Make empty space)
    struct PXRectangleOffset
    {
        float Left;
        float Top;
        float Right;
        float Bottom;
    };


    // Offset the space as the child container
    // (Make object take more space)
    struct PXPadding
    {
        float LeftTop;
        float Top;
        float Right;
        float Bottom;
    }



    internal struct PXTexture2D
    {
        PXResourceID ResourceID; // IDirect3DTexture9

        PXGraphicRenderFilter Filter;
        PXGraphicImageLayout LayoutNear;
        PXGraphicImageLayout LayoutFar;
        PXGraphicImageWrap WrapHeight;
        PXGraphicImageWrap WrapWidth;

        PXImage Image;
    }

    public class Texture2D
    {
        internal PXTexture2D _texture2D;
    }


    internal struct PXTexture3D
    {
        PXResourceID ResourceID; // IDirect3DVolumeTexture9

        uint Width;
        uint Height;
        uint Depth;

        PXColorFormat Format;
    }

    public class Texture3D
    {
        internal PXTexture3D _pxTexture3D;
    }


    // A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
    internal struct PXTextureCube
    {
        PXResourceID ResourceID; // IDirect3DCubeTexture9

        PXColorFormat Format;

       // PXImage ImageList[6];
    }

    public class TextureCube
    {
        internal PXTextureCube _pxTextureCube;
    }





    internal unsafe struct PXShader
    {
        PXResourceID ResourceID; // IDirect3DVertexShader9, IDirect3DPixelShader9

        PXGraphicShaderType Type;

        ulong ContentSize;
        char* Content;
    }

    public class Shader
    {
        internal PXShader _pxShader;
    }


    internal struct PXShaderProgram
    {
        PXResourceID ResourceID;

        PXShader VertexShader;
        PXShader PixelShader;
    }


    public class ShaderProgram
    {
        internal PXShaderProgram _pxShaderProgram;
    }



    public enum PXDrawScriptType
    {
        PXDrawScriptTypeInvalid,
        PXDrawScriptTypeAll,
        PXDrawScriptTypePixelState,
        PXDrawScriptTypeVertexState
    }
    // A sequence of actions that tells the fixed pipline how to act.
    // This is a precursor of a shader, so it's use is discurraged for
    // modern applications and hardware that support shaders, not only
    // for performance reasons but for versitility too.
    internal struct PXDrawScript
    {
        PXResourceID ResourceID;

        PXDrawScriptType Type;
    }

    public class DrawScript
    {
        internal PXDrawScript _pxDrawScript;
    }

    //-----------------------------------------------------
    // Vertex rendering info
    //-----------------------------------------------------

    enum PXVertexBufferDataType
    {
        PXVertexBufferDataTypeInvalid,

        PXVertexBufferDataTypeVertex,
        PXVertexBufferDataTypeTexture,
        PXVertexBufferDataTypeNormal
    }

    unsafe struct PXVertexBuffer
    {
        PXResourceID ResourceID; // IDirect3DVertexBuffer9, ID3D11Buffer

        void* VertexData;
        UIntPtr VertexDataSize;

        UIntPtr VertexDataRowSize;

        PXVertexBufferFormat Format;
    }


    unsafe struct PXIndexRange
    {
        void* DataPoint;
        UIntPtr DataRange;
    }

    // Index buffer, used to store the vertex render order.
    // Additionally contains info about how to actually render, like modes.
    unsafe struct PXIndexBuffer
    {
        PXResourceID ResourceID; // IDirect3DIndexBuffer9

        byte IndexTypeSize;

        void* IndexData;
        uint IndexDataSize;
        uint IndexDataAmount;

      //  PXDataType DataType;
        uint DrawModeID;

        PXTexture2D* Texture2D;
    };


    public enum DirectXDriverType
    {
        HardwareDevice,
        ReferencDevice,
        ReferencDeviceWithoutRender,
        Software,
    }

    public enum DirectXVersion
    {
        DXInvalid,

        DXNewest,

        DX9, // Windows XP

        DX10x0, // Windows Vista

        DX10x1,  // Windows Vista
        DX10x1Simulate9x1,
        DX10x1Simulate9x2,
        DX10x1Simulate9x3,
        DX10x1Simulate10x0,

        // DirectX 11 - Windows 7, 8, 8.1
        DX11Emulate1x0Core,
        DX11Emulate9x1,
        DX11Emulate9x2,
        DX11Emulate9x3,
        DX11Emulate10x0,
        DX11Emulate10x1,
        DX11Emulate11x0,
        DX11Emulate11x1,
        //DX11Emulate12x0,
        //DX11Emulate12x1,
        //DX11Emulate12x2,

        // DirectX 12 - Windows 10, 11
        DX12Emulate1x0Core,
        DX12Emulate9x1,
        DX12Emulate9x2,
        DX12Emulate9x3,
        DX12Emulate10x0,
        DX12Emulate10x1,
        DX12Emulate11x0,
        DX12Emulate11x1,
        DX12Emulate12x0,
        DX12Emulate12x1,
        DX12Emulate12x2,

    }


    internal unsafe struct PXGraphicInitializeInfo
    {
        public PXWindow* WindowReference;

        public UIntPtr Width;
        public UIntPtr Height;

        public DirectXVersion DirectXVersion;
        public DirectXDriverType DirectXDriverType;

        public GraphicSystem GraphicSystem;

        fixed byte Buffer[7000];
    }


    public enum UIElementType
    {
       Invalid,
       Panel,
       Text,
       Button,
       Image,
       DropDown,
       Toggle,
       CheckBox,
       ColorPicker,
       Slider,
       RadioButton,
       ToolTip,
       RenderFrame,
       Custom
    }

    [StructLayout(LayoutKind.Sequential, Size = 160)]
    internal unsafe struct PXUIElement
    {
        fixed char Buffer[160];
    }


    public class UIElement
    {
        internal PXUIElement _pxUIElement;
    }



    unsafe struct PXModel
    {
        PXResourceID ResourceID;

        //-----------------------------
        // Render info
        //-----------------------------
        PXMatrix4x4F ModelMatrix;
        PXShaderProgram* ShaderProgramReference;
        //-----------------------------


        //-----------------------------
        // Vertex data
        //-----------------------------
        PXVertexBuffer VertexBuffer;
        PXIndexBuffer IndexBuffer;

        PXModel* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
        PXModel* StructureParent; // Structural parent of structure
        PXModel* StructureSibling; // Stuctual sibling, works like a linked list.
        PXModel* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
                                           //-----------------------------


        //-----------------------------
        // Settings
        //-----------------------------
        byte IgnoreViewPosition; // Removes positiondata from the view matrix
        byte IgnoreViewRotation; // remove rotationdata from the view matrix
        PXRectangleOffset Margin;
        //-----------------------------
    };

    public class Model
    {
        internal PXModel _pxModel;
    };




    /*
     * unsafe struct PXVertexElement
    {
        PXDataType Type;
        UIntPtr Length;
        UIntPtr Stride;
        void* StartAdress;
    };*/



    struct PXDepthStencilSurface
    {
        int x;
    };


    unsafe struct PXMaterial
    {
        fixed float Diffuse[4];
        fixed float Ambient[4];
        fixed float Specular[4];  // shininess
        fixed float Emissive[4];
        float Power;        // Sharpness if specular highlight
    };


    internal struct PXViewPort
    {
        public int X;
        public int Y;
        public int Width;
        public int Height;
        public float ClippingMinimum;
        public float ClippingMaximum;
    };

    public class ViewPort
    {
        internal PXViewPort _pxViewPort;

        public int X {get => _pxViewPort.X; set => _pxViewPort.X = value; }
        public int Y { get => _pxViewPort.Y; set => _pxViewPort.Y = value; }
        public int Width { get => _pxViewPort.Width; set => _pxViewPort.Width = value; }
        public int Height { get => _pxViewPort.Height; set => _pxViewPort.Height = value; }
        public float ClippingMinimum { get => _pxViewPort.ClippingMinimum; set => _pxViewPort.ClippingMinimum = value; }
        public float ClippingMaximum { get => _pxViewPort.ClippingMaximum; set => _pxViewPort.ClippingMaximum = value; }
    };



    struct PXRenderTarget
    {
        PXResourceID ResourceID; // IDirect3DVertexBuffer9
    };











    enum PXLightType
    {
        PXLightTypeInvalid,
        PXLightTypePoint,
        PXLightTypeSpot,
        PXLightTypeDirectional
    };

    internal unsafe struct PXLight
    {
        PXResourceID ResourceID; // D3DLIGHT9

        byte Enabled;

        PXLightType Type;            /* Type of light source */
        fixed float Diffuse[4];         /* Diffuse color of light */
        fixed float Specular[4];        /* Specular color of light */
        fixed float Ambient[4];         /* Ambient color of light */
        fixed float Position[3];         /* Position in world space */
        fixed float Direction[3];        /* Direction in world space */
        float CutoffRange;            /* Cutoff range */
        float Falloff;          /* Falloff */
        float AttenuationConstant;     /* Constant attenuation */
        float AttenuationLinear;     /* Linear attenuation */
        float AttenuationQuadratic;     /* Quadratic attenuation */
        float Theta;            /* Inner angle of spotlight cone */
        float Phi;              /* Outer angle of spotlight cone */
    };

    public class Light
    {
        internal PXLight _pxLight;
    }




        unsafe struct PXFontPageCharacter
    {
        uint ID;
        fixed float Position[2]; // Position of the character image in the texture.
        fixed float Size[2];  // Size of the character image in the texture.
        fixed float Offset[2];// Offset from the position-center.
        int XAdvance; // How much the current position should be advanced after drawing the character.
    };

    unsafe struct PXFontPage
    {
        UIntPtr CharacteListSize;
        PXFontPageCharacter* CharacteList;
        PXTexture2D Texture;
    };

    internal unsafe struct PXFont
    {
        PXFontPage MainPage;

        PXFontPage* AdditionalPageList;
        UIntPtr AdditionalPageListSize;
    };

    public class Font
    {
        internal PXFont _pxFont;
    };




    enum PXBlendingMode
    {
        PXBlendingModeInvalid,

        PXBlendingModeNone,

        PXBlendingModeOneToOne // Direct 1:1 mixing
    };



    // Shapes

    struct PXRectangleF
    {
        float X;
        float Y;
        float Width;
        float Height;
    };



    struct PXSprite
    {
        uint PXID;

        //PXVector2F TextureScaleOffset;

        PXModel Model;
    };


    [StructLayout(LayoutKind.Sequential,CharSet = CharSet.Ansi, Size = 1000)]
    internal unsafe struct PXGraphicDevicePhysical
    {
        fixed char DeviceDisplay[32];
        fixed char DeviceName[32];
        fixed char DeviceID[32];
        fixed char DeviceKey[32];
        fixed char Vendor[32];
        fixed char Renderer[32];
        fixed char Shader[32];

        UIntPtr VideoMemoryDedicated; // dedicated video memory, total size (in kb) of the GPU memory
        UIntPtr VideoMemoryCurrent; // total available memory, total size (in Kb) of the memory available for allocations
        UIntPtr VideoMemoryTotal; // current available dedicated video memory (in kb), currently unused GPU memory

        UIntPtr VideoMemoryEvictionCount; // How many times memory got displaced to Main-RAM
        UIntPtr VideoMemoryEvictionSize; // size of total video memory evicted (in kb)

        byte IsConnectedToMonitor;
    }

    public class GraphicDevicePhysical
    {
        internal PXGraphicDevicePhysical _pxGraphicDevicePhysical;
    }


    // Camera
    public enum PXCameraPerspective
    {
        PXCameraPerspectiveInvalid,
        PXCameraPerspective2D,
        PXCameraPerspective3D // Perspective
    }
   
    internal struct PXCamera
    {
        PXMatrix4x4F MatrixModel;
        PXMatrix4x4F MatrixView;
        PXMatrix4x4F MatrixProjection;

        float WalkSpeed;
        float ViewSpeed;

        /*
       PXVector3F LookAtPosition;
       PXVector3F CurrentRotation;

       //---<Follow>---
       PXVector3F Offset;
       PXMatrix4x4F* Target;
       float FollowSpeed; // Ranges from 0 to 1 .. FollowSpeed; = 0.98f

       PXCameraPerspective Perspective;

       float FieldOfView;
       int Height;
       int Width;
       float Near;
       float Far;
       */
    }


    public class Camera
    {
        internal PXCamera _pxCamera;
    }




    unsafe struct PXSkyBox
    {
        PXModel Model;

        PXTextureCube TextureCube;

        PXShaderProgram* ShaderProgramReference;
    }


    unsafe struct PXSound
    {
        void* BaseObject;
        void* Data;

        UIntPtr DataSize;

        uint ChunkSize;
        uint SampleRate;
        uint ByteRate;

        ushort AudioFormat;
        ushort NumerOfChannels;
        ushort BlockAllign;
        ushort BitsPerSample;
    }
    
    struct PXVideo
    {
        int __Dummy__;
    }

    struct PXMaterialContainer
    {
        int __dummy__;
    }

    public class Resource<T>
    {
        public PX.ActionResult Load(string filePath)
        {
            return ActionResult.NotImplemented;
        }
        public PX.ActionResult Save(string filePath)
        {
            return ActionResult.NotImplemented;
        }
    }
}
