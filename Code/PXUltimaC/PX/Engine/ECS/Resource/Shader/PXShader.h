#pragma once

#ifndef PXShaderIncluded
#define PXShaderIncluded

#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXShaderProgramCreateInfo_
{
    PXFile* ShaderVertexFile;
    PXFile* ShaderPixelFile;

    PXText ShaderVertex;
    PXText ShaderPixel;

    PXText ShaderVertexFilePath;
    PXText ShaderPixelFilePath;
}
PXShaderProgramCreateInfo;



//---------------------------------------------------------
// Shader
//---------------------------------------------------------
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

    PXShaderVariableTypePXF32Single,
    PXShaderVariableTypePXF32Vector2,
    PXShaderVariableTypePXF32Vector3,
    PXShaderVariableTypePXF32Vector4,
    PXShaderVariableTypeDouble,

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

#define PXShaderVariableBehaviourFailedFetch (1<<0) // A resolusion to the shader failed

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
    void* Data; // Raw data data that is defined by the type
    PXSize Amount;
    PXShaderType ShaderType;
    PXShaderVariableLocallity Locallity;
}
PXShaderVariable;

typedef struct PXShader_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;
        void* DirectXHandle;

        struct ID3D11VertexShader* VertexShader_11; // ID3D11VertexShader
        struct ID3D11PixelShader* PixelShader_11;

        struct IDirect3DVertexShader9* VertexShader_9;
        struct IDirect3DPixelShader9* PixelShader_9;
    };

    PXShaderType Type;

    PXI8U VersionMajor;
    PXI8U VersionMinor;

    struct PXFile_* ShaderFile;

    PXSize VariableListAmount;
    PXShaderVariable* VariableListData;
}
PXShader;



typedef struct PXShaderProgram_
{
    PXECSInfo Info;

    union
    {
        PXI32U OpenGLID;// ProgramID;
    };

    PXSize VariableListAmount;
    PXShaderVariable* VariableListData;
}
PXShaderProgram;



PXPublic const char* PXAPI PXShaderTypeToString(const PXShaderType pxShaderType);

PXPublic PXResult PXAPI PXShaderProgramCreate(PXShaderProgram** pxShaderProgramREF, PXShaderProgramCreateInfo PXREF pxShaderProgramCreateInfo);

#endif