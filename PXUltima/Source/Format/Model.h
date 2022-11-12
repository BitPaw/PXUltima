#ifndef ModelINCLUDE
#define ModelINCLUDE

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <File/DataStream.h>
#include <File/File.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMaterial_
	{
		size_t NameSize;
		char* Name;

		size_t DiffuseTextureFilePathSize;
		char* DiffuseTextureFilePath;

		float Ambient[3];
		float Diffuse[3];
		float Specular[3];
		float Emission[3];
	}
	PXMaterial;

	typedef struct PXMaterialList_
	{
		// private
		void* Data;
		size_t DataSize;
		//-------
		
		size_t NumberOfMatrials;

	}
	PXMaterialList;



	// The renderable part of a mesh.
	typedef struct MeshSegment_
	{
		//wchar_t Name[64];

		unsigned char DrawStrideSize;
		unsigned int DrawClusterSize;

		float* VertexData;
	}
	MeshSegment;

	
	// A collection of positions and its atributes (Vertex).
	typedef struct Mesh_
	{			
		wchar_t Name[64];
	}
	Mesh;

	// A collection of multible meshes.
	// Example, multible flowers and a vase together are a model.
	typedef struct Model_
	{
		// Mesh
		// +-Name
		// +-VBO
		// +-VBO-Texture
		// +-VBO-Normals
		// +-IBO

		// 
		// MeshListLength [Amount]
		// MeshList
		//   | 
		//   +    (Structure)
		//   +--- StructureSize		- Triangle(3), Quad(4), ...
		//   +--- StructureListLength	- 123
		//   +--- StructureListData	- [ x, y, z]
		
		// 
		// 
		// 
		// 


		// MeshSegment
		// +-- Name
		// +-- Structure
		// +-- MateralID
		// +-- RenderLength

		//---------------------------------------------------------------------
		void* Data;

		// MeshRenderList
		// unsigned char -> Mode
		// unsigned int -> Size
		// struct		-> Material
		// 


		void* MaterialList;

		void* DataVertex;
		size_t DataVertexWidth;
		size_t DataVertexStride;
		size_t DataVertexSize;

		size_t DataNormalWidth;
		size_t DataNormalStride;
		size_t DataNormalSize;

		size_t DataTextureWidth;
		size_t DataTextureStride;
		size_t DataTextureSize;

		size_t DataColorWidth;
		size_t DataColorStride;
		size_t DataColorSize;

		size_t DataIndexWidth;
		size_t DataIndexStride;
		size_t DataIndexSize;
		//---------------------------------------------------------------------
	}
	Model;

	typedef ActionResult(*ModelCompilerFunction)(DataStream* const inputStream, DataStream* const outputStream);
	typedef ActionResult(*ModelParserFunction)(DataStream* const inputStream, Model* const model);


	PXPublic void ModelConstruct(Model* const model);
	PXPublic void ModelDestruct(Model* const model);

	PXPublic size_t PXModelMaterialAmount(Model* const model);
	PXPublic PXBool PXModelMaterialGet(Model* const model, const size_t materialID, PXMaterial* const pxMaterial);


	PXPublic unsigned char ModelSegmentsAmount(const Model* const model);
	PXPublic void ModelSegmentsGet(const Model* const model, const size_t index, MeshSegment* const meshSegment);

	PXPublic ActionResult ModelLoadA(Model* const model, const char* const filePath);
	PXPublic ActionResult ModelLoadW(Model* const model, const wchar_t* const filePath);
	PXPublic ActionResult ModelLoadD(Model* const model, DataStream* const fileStream, const FileFormatExtension modelType);

#ifdef __cplusplus
}
#endif

#endif