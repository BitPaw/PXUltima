#ifndef ModelINCLUDE
#define ModelINCLUDE

#include <Format/Type.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum ModelType_
	{
		ModelInvalid,
		ModelUnKown,
		ModelA3DS,
		ModelFBX,
		ModelOBJ,
		ModelPLY,
		ModelSTL,
		ModelWRL
	}
	ModelType;


	typedef struct Material_
	{
		wchar_t Name[64];
		wchar_t TextureFilePath[260];
		float Ambient[3];
		float Diffuse[3];
		float Specular[3];
		float Emission[3];
	}
	Material;

	typedef struct MeshSegmentMaterialInfo_
	{
		size_t MaterialIndex;
		size_t Size; // Size of buffer to use given material		
	}
	MeshSegmentMaterialInfo;

	typedef struct MeshSegment_
	{
		wchar_t Name[64];
		size_t IndexDataListSize;
		unsigned int* IndexDataList;

		MeshSegmentMaterialInfo* MaterialInfo;
		size_t MaterialInfoSize;
	}
	MeshSegment;

	typedef struct Mesh_
	{	
		size_t VertexDataListSize;
		float* VertexDataList;

		size_t VertexDataStructureListSize;
		unsigned int VertexDataStructureList[4];

		size_t SegmentListSize;
		MeshSegment* SegmentList;
	}
	Mesh;

	typedef struct Model_
	{
		size_t MaterialListSize;
		Material* MaterialList;

		size_t MeshListSize;
		Mesh* MeshList;
	}
	Model;



#ifdef __cplusplus
}
#endif

#endif