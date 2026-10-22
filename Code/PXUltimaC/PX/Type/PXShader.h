#pragma once

#ifndef PXShaderIncluded
#define PXShaderIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Container/Buffer/PXBuffer.h>

typedef enum PXShaderVariableType_
{
    PXShaderVariableTypeInvalid,

    PXShaderVariableTypeBoolSignle,
    PXShaderVariableTypeBoolVector2,
    PXShaderVariableTypeBoolVector3,
    PXShaderVariableTypeBoolVector4,

    PXShaderVariableTypeInt32SSingle,
    PXShaderVariableTypeInt32SVector2,
    PXShaderVariableTypeInt32SVector3,
    PXShaderVariableTypeInt32SVector4,

    PXShaderVariableTypeInt32USingle,
    PXShaderVariableTypeInt32UVector2,
    PXShaderVariableTypeInt32UVector3,
    PXShaderVariableTypeInt32UVector4,

    PXShaderVariableTypeF32S,
    PXShaderVariableTypeF32D,
    PXShaderVariableTypeF32T,
    PXShaderVariableTypeF32Q,

    PXShaderVariableTypeF64S,
    PXShaderVariableTypeF64D,
    PXShaderVariableTypeF64T,
    PXShaderVariableTypeF64Q,


    // Matrix
    PXShaderVariableTypeMatrix2x2,
    PXShaderVariableTypeMatrix2x3,
    PXShaderVariableTypeMatrix2x4,
    PXShaderVariableTypeMatrix3x2,
    PXShaderVariableTypeMatrix3x3,
    PXShaderVariableTypeMatrix3x4,
    PXShaderVariableTypeMatrix4x2,
    PXShaderVariableTypeMatrix4x3,
    PXShaderVariableTypeMatrix4x4,

    PXShaderVariableTypeSampler2DF,
    PXShaderVariableTypeSampler2DI32S,
    PXShaderVariableTypeSampler2DI32U,
    PXShaderVariableTypeSamplerF2DShadow,

    PXShaderVariableTypeSampler2DArrayF,
    PXShaderVariableTypeSampler2DArrayI32S,
    PXShaderVariableTypeSampler2DArrayI32U,

    PXShaderVariableTypeSamplerF2DArrayShadow,

    PXShaderVariableTypeSampler3DF,
    PXShaderVariableTypeSampler3DI32S,
    PXShaderVariableTypeSampler3DI32U,

    PXShaderVariableTypeSamplerCubeF,
    PXShaderVariableTypeSamplerCubeI32S,
    PXShaderVariableTypeSamplerCubeI32U,

    PXShaderVariableTypeSamplerFCubeShadow,
}
PXShaderVariableType;

typedef enum PXShaderVariableLocallity_
{
    PXShaderVariableLocallityInvalid,
    PXShaderVariableLocallityGlobal, // Uniform
    PXShaderVariableLocallityVertexOnly, // Atrribute
}
PXShaderVariableLocallity;


typedef enum PXShaderType_
{
    PXShaderTypeInvalid,
    PXShaderTypeVertex,                 // .vert - vertex shader
    PXShaderTypePixel,                  // .frag - fragment shader
    PXShaderTypeTessellationControl,    // .tesc - tessellation control shader
    PXShaderTypeTessellationEvaluation, // .tese - tessellation evaluation shader
    PXShaderTypeGeometry,               // .geom - geometry shader
    PXShaderTypeCompute,                // .comp - compute shader
}
PXShaderType;



#define PXShaderVariableNameSize 32

typedef struct PXShaderVariable_
{
    PXECSInfo Info;

    char Name[PXShaderVariableNameSize]; // Shader variable name, used only for and ID fetch as it is very slow!
    PXSize NameSize;
    PXShaderVariableType DataType;
    PXI32U RegisterIndex; // ID to make,
    PXSize DataTypeSize;

    // Only for DirectX
    PXI32U RegisterCount;
    PXI32U Rows;
    PXI32U Columns;
    PXI32U Elements;
    PXI32U StructMembers;

    // Used for moving data only

    union
    {
        void* Data; // Raw data data that is defined by the type
        float* F32;
        int* S32;
        #if OSWindows
        BOOL* Bool;
        #endif
    };

    PXSize Amount;
}
PXShaderVariable;

typedef struct PXShader_
{
    PXECSInfo Info;

    PXBuffer Data;

    PXI8U VersionMajor;
    PXI8U VersionMinor;

    PXSize VariableListAmount;
    PXShaderVariable* VariableListData;
}
PXShader;


typedef struct PXShaderCreateInfo_
{
    PXECSInfo Info;

    PXText Embedded; // DirectText
    PXText FilePath; // Filepath to a external file
    PXFile* File; // File we already loaded and provide
}
PXShaderCreateInfo;


PXPublic const char* PXAPI PXShaderTypeToString(const PXShaderType pxShaderType);

PXInternal void PXAPI PXShaderRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXShaderCreate(PXShader PXREF pxShader, PXShaderCreateInfo PXREF pxShaderCreateInfo);
PXPublic PXResult PXAPI PXShaderRelease(PXShader PXREF pxShader);

#endif
