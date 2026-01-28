#pragma once

#ifndef PXTextureIncluded
#define PXTextureIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Media/PXColor.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/System/PXRenderingSystem.h>


#define PXTexturePropertyWidth      1
#define PXTexturePropertyHeight     2
#define PXTexturePropertyDepth      3
#define PXTexturePropertyFormat     4
#define PXTexturePropertyBBP        5

typedef enum PXTextureLayout_
{
    PXTextureLayoutInvalid,

    // Returns the value of the texture element that is nearest to the specified texture coordinates.
    PXTextureLayoutNearest,

    // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates.
    // These can Included items wrapped or repeated from other parts of a texture, depending on the values of GL_TEXTURE_WRAP_Sand GL_TEXTURE_WRAP_T, and on the exact mapping.
    PXTextureLayoutLinear,

    //Chooses the mipmap that most closely matches the size of the pixel being texturedand
    //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates) to produce a texture value.
    PXTextureLayoutMipMapNearestNearest,

    //Chooses the mipmap that most closely matches the size of the pixel being texturedand
    //uses the GL_LINEAR criterion(a weighted average of the four texture elements that are closest to the specified texture coordinates) to produce a texture value.
    PXTextureLayoutMipMapLinearNearest,

    //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
    //uses the GL_NEAREST criterion(the texture element closest to the specified texture coordinates)
    //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
    PXTextureLayoutMipMapNNearestLinear,

    //Chooses the two mipmaps that most closely match the size of the pixel being texturedand
    //uses the GL_LINEAR criterion(a weighted average of the texture elements that are closest to the specified texture coordinates)
    //to produce a texture value from each mipmap.The final texture value is a weighted average of those two values.
    PXTextureLayoutMipMapLinearLinear
}
PXTextureLayout;

typedef enum PXTextureWrap_
{
    PXTextureWrapInvalid,

    // Images will be used 'as is' and will not be streched whatsoever.
    PXTextureWrapNoModification,

    // Strech
    PXTextureWrapStrechEdges,

    PXTextureWrapStrechEdgesAndMirror,

    // Tiles the image in a gridformat
    PXTextureWrapRepeat,

    // Tiles the image in a gridformat but also flip them every time.
    PXTextureWrapRepeatAndMirror,
}
PXTextureWrap;

typedef enum PXTextureType_
{
    PXTextureTypeInvalid,

    PXTextureType1D,
    PXTextureType1DArray,
    PXTextureType1DBuffer,
    PXTextureType2D,
    PXTextureType2DArray,
    PXTextureType2DProxy,
    PXTextureType3D,

    PXTextureTypeCube,
    PXTextureTypeCubeProxy,
    PXTextureTypeCubeArray,
    PXTextureTypeCubeRight,
    PXTextureTypeCubeLeft,
    PXTextureTypeCubeTop,
    PXTextureTypeCubeDown,
    PXTextureTypeCubeBack,
    PXTextureTypeCubeFront,

    PXTextureTypeBuffer,
    PXTextureTypeRectangle,
    PXTextureTypeRectangleProxy,

    PXTextureType2DMultiSample,
    PXTextureType2DMultiSampleArray
}
PXTextureType;

typedef enum PXResourceAction_
{
    PXResourceActionInvalid,
    PXResourceActionCreate,
    PXResourceActionDelete,
    PXResourceActionUpdate,
    PXResourceActionSelect
}
PXResourceAction;





typedef struct PXTexture_ PXTexture;

typedef struct PXTexture_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;

        void* DirectXHandle;

        // DX11
        struct ID3D11Texture1D* Texture1D_11;
        struct ID3D11Texture2D* Texture2D_11;
        struct ID3D11Texture3D* Texture3D_11;

        // DX9
        struct IDirect3DTexture9* Texture2D_9;
        struct IDirect3DVolumeTexture9* Texture3D_9;
        struct IDirect3DCubeTexture9* TextureCube_D9;
    };

    PXBuffer PixelData;

    PXSize Width;
    PXSize Height;
    PXSize Depth;

    PXRenderFilter Filter;
    PXTextureLayout LayoutNear;
    PXTextureLayout LayoutFar;
    PXTextureWrap WrapHeight;
    PXTextureWrap WrapWidth;

    PXColorFormat Format;
    PXTextureType Type;
}
PXTexture;


typedef struct PXTextureInfo_
{
    PXTexture** TextureReference;
    PXSize Amount;
    PXTextureType Type;
    PXResourceAction Action;
}
PXTextureInfo;


typedef struct PXTextureCreateCubeInfo_
{
    PXText Top;
    PXText Left;
    PXText Right;
    PXText Back;
    PXText Bottom;
    PXText Front;
}
PXTextureCreateCubeInfo;

typedef struct PXTextureCreate2DInfo_
{
    PXSize Width;
    PXSize Height;
}
PXTextureCreate2DInfo;

typedef struct PXTextureCreate3DInfo_
{
    PXSize Width;
    PXSize Height;
    PXSize Depth;
}
PXTextureCreate3DInfo;

typedef struct PXTextureCreateInfo_
{
    PXECSCreateInfo Info;

    PXTexture* Texture;

    PXTextureType Type;

    union
    {
        PXTextureCreateCubeInfo Cube;
        PXTextureCreate2DInfo T2D;
        PXTextureCreate2DInfo T3D;
    };
}
PXTextureCreateInfo;





PXPublic PXResult PXAPI PXTextureProperty(PXTexture PXREF pxTexture, PXECSProperty PXREF pxECSProperty);

PXPublic PXSize PXAPI PXTextureHeight(PXTexture PXREF pxTexture);
PXPublic PXSize PXAPI PXTextureWidth(PXTexture PXREF pxTexture);
PXPublic PXSize PXAPI PXTextureDepth(PXTexture PXREF pxTexture);
PXPublic PXColorFormat PXAPI PXTextureColorFormat(PXTexture PXREF pxTexture);
PXPublic void* PXAPI PXTextureDXInterface(PXTexture PXREF pxTexture);

PXPublic PXBuffer* PXAPI PXTexturePixelData(PXTexture PXREF pxTexture);


PXPublic PXResult PXAPI PXTextureCopyAsIs(PXTexture PXREF pxTexture, const PXTexture PXREF pxTextureSource);
PXPublic PXResult PXAPI PXTextureCopyAsNew(PXTexture PXREF pxTexture, const PXTexture PXREF pxTextureSource);
PXPublic PXResult PXAPI PXTextureResize(PXTexture PXREF pxTexture, const PXColorFormat format, const PXSize width, const PXSize height);
PXPublic PXResult PXAPI PXTextureFlipHorizontal(PXTexture PXREF pxTexture);
PXPublic PXResult PXAPI PXTextureFlipVertical(PXTexture PXREF pxTexture);
PXPublic PXResult PXAPI PXTextureRemoveColor(PXTexture PXREF pxTexture, const PXByte red, const PXByte green, const PXByte blue);
PXPublic PXResult PXAPI PXTextureFillColorRGBA8(PXTexture PXREF pxTexture, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha);
PXPublic void* PXAPI PXTextureDataPoint(const PXTexture PXREF pxTexture, const PXSize x, const PXSize y);
PXPublic PXSize PXAPI PXTexturePixelPosition(const PXTexture PXREF pxTexture, const PXSize x, const PXSize y);
PXPublic void PXAPI PXTexturePixelSetRGB8
(
    PXTexture PXREF pxTexture,
    const PXSize x,
    const PXSize y,
    const PXByte red,
    const PXByte green,
    const PXByte blue
);




PXPublic PXResult PXAPI PXTextureRegisterToECS(void);
PXPublic PXResult PXAPI PXTextureCreate(PXTexture** pxTexture, PXTextureCreateInfo PXREF pxTextureCreateInfo);
PXPublic PXResult PXAPI PXTextureRelease(PXTexture PXREF pxTexture);

PXPublic HBITMAP PXAPI PXBitMapFromImage(const int width, const int height, const int amountofchannels, void* data);

#endif