#pragma once

#ifndef PXTextureIncluded
#define PXTextureIncluded


#define PXTextureNoMinmaps 0

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

typedef enum PXTextureSamplingMode_
{
    PXTextureSamplingModeInvalid,
    PXTextureSamplingModePoint,      // No interpolation. Nearest-neighbor sampling.
    PXTextureSamplingModeBilinear,   // Linear interpolation between texels.
    PXTextureSamplingModeTrilinear,  // Linear interpolation across mip levels.
    PXTextureSamplingModeAnisotropic // High-quality anisotropic filtering.
}
PXTextureSamplingMode;

#endif