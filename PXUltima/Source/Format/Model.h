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
		unsigned int TextureID;

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
	typedef struct PXModel_
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

		//---<Material>---
		void* MaterialList;	
		//----------------

		//----------------
		void* DataVertexList;
		size_t DataVertexListSize;
		
		size_t DataVertexWidth;
		size_t DataVertexSize;

		size_t DataNormalWidth;
		size_t DataNormalSize;

		size_t DataTextureWidth;
		size_t DataTextureSize;

		size_t DataColorWidth;
		size_t DataColorSize;
		//----------------


		//----------------
		void* IndexList;
		size_t DataIndexWidth;
		size_t DataIndexSize;
		//--------------------
	}
	PXModel;

	typedef ActionResult(*ModelCompilerFunction)(DataStream* const inputStream, DataStream* const outputStream);
	typedef ActionResult(*ModelParserFunction)(DataStream* const inputStream, PXModel* const model);


	PXPublic void ModelConstruct(PXModel* const model);
	PXPublic void ModelDestruct(PXModel* const model);

	PXPublic size_t PXModelMaterialAmount(PXModel* const model);
	PXPublic PXBool PXModelMaterialGet(PXModel* const model, const size_t materialID, PXMaterial* const pxMaterial);


	PXPublic unsigned char ModelSegmentsAmount(const PXModel* const model);
	PXPublic void ModelSegmentsGet(const PXModel* const model, const size_t index, MeshSegment* const meshSegment);

	PXPublic size_t ModelVertexDataStride(const PXModel* const model);

	PXPublic ActionResult ModelLoadA(PXModel* const model, const char* const filePath);
	PXPublic ActionResult ModelLoadW(PXModel* const model, const wchar_t* const filePath);
	PXPublic ActionResult ModelLoadD(PXModel* const model, DataStream* const fileStream, const FileFormatExtension modelType);

#ifdef __cplusplus
}
#endif

#endif