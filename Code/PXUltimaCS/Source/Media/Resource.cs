﻿using System;

namespace PX
{
    public enum PXGraphicSystem
    {
        PXGraphicSystemInvalid,
        PXGraphicSystemOpenGL,
        PXGraphicSystemDirectX,
        PXGraphicSystemVulcan
    }

    public enum PXRefreshRateMode
    {
        PXRefreshRateUnlimited,
        PXRefreshRateVSync,
        PXRefreshRateCustomSync
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


    public enum PXGraphicDrawFillMode
    {
        PXGraphicDrawFillModeInvalid,
        PXGraphicDrawFillModePoints,
        PXGraphicDrawFillModeLines,
        PXGraphicDrawFillModeFill
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



    struct PXTexture2D
    {
        PXResourceID ResourceID; // IDirect3DTexture9

        PXGraphicRenderFilter Filter;
        PXGraphicImageLayout LayoutNear;
        PXGraphicImageLayout LayoutFar;
        PXGraphicImageWrap WrapHeight;
        PXGraphicImageWrap WrapWidth;

        PXImage Image;
    }

    struct PXTexture3D
    {
        PXResourceID ResourceID; // IDirect3DVolumeTexture9

        uint Width;
        uint Height;
        uint Depth;

        PXColorFormat Format;
    }

    // A Texture for a cube. 6 Sides, used for actual boxes like a skybox.
    struct PXTextureCube
    {
        PXResourceID ResourceID; // IDirect3DCubeTexture9

        PXColorFormat Format;

       // PXImage ImageList[6];
    }






    unsafe struct PXShader
    {
        PXResourceID ResourceID; // IDirect3DVertexShader9, IDirect3DPixelShader9

        PXGraphicShaderType Type;

        ulong ContentSize;
        char* Content;
    }


    internal struct PXShaderProgram
    {
        PXResourceID ResourceID;

        PXShader VertexShader;
        PXShader PixelShader;
    }


    public class ShaderProgram
    {
        PXResourceID ResourceID;

        PXShader VertexShader;
        PXShader PixelShader;
    }



    enum PXDrawScriptType
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
    struct PXDrawScript
    {
        PXResourceID ResourceID;

        PXDrawScriptType Type;
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


    unsafe struct PXVertexStructure
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

        PXVertexStructure* StructureOverride; // Used to take the model data from another structure, ther values like matrix stay unaffected
        PXVertexStructure* StructureParent; // Structural parent of structure
        PXVertexStructure* StructureSibling; // Stuctual sibling, works like a linked list.
        PXVertexStructure* StructureChild; // Structure can only have one child, all others are siblings to a core child, the first born.
                                           //-----------------------------


        //-----------------------------
        // Settings
        //-----------------------------
        byte IgnoreViewPosition; // Removes positiondata from the view matrix
        byte IgnoreViewRotation; // remove rotationdata from the view matrix
        PXRectangleOffset Margin;
        //-----------------------------
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


    struct PXViewPort
    {
        int X;
        int Y;
        int Width;
        int Height;
        float ClippingMinimum;
        float ClippingMaximum;
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

    unsafe struct PXLight
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

    unsafe struct PXFont
    {
        PXFontPage MainPage;

        PXFontPage* AdditionalPageList;
        UIntPtr AdditionalPageListSize;
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

        PXVertexStructure VertexStructure;
    };




    // Camera
    enum PXCameraPerspective
    {
        PXCameraPerspectiveInvalid,
        PXCameraPerspective2D,
        PXCameraPerspective3D // Perspective
    }
   
    struct PXCamera
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



    unsafe struct PXSkyBox
    {
        PXVertexStructure VertexStructure;

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
