#pragma once
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXShader.h>

typedef struct PXShaderUniform_
{
    PXECSInfo Info;

    char Name[32];
    PXSize NameSize;
    PXShaderVariableType DataType;
    PXI32U RegisterIndex; // ID to make,
    PXSize DataTypeSize;

    void* Data;

    // Type..
}
PXShaderUniform;