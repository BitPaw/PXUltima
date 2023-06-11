#ifndef PXModelINCLUDE
#define PXModelINCLUDE

#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMaterial_
	{
		PXText Name;
		PXText DiffuseTextureFilePath;

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
		PXSize DataSize;
		//-------
		
		PXSize NumberOfMatrials;

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
		PXSize DataVertexListSize;
		
		PXSize DataVertexWidth;
		PXSize DataVertexSize;

		PXSize DataNormalWidth;
		PXSize DataNormalSize;

		PXSize DataTextureWidth;
		PXSize DataTextureSize;

		PXSize DataColorWidth;
		PXSize DataColorSize;
		//----------------


		//----------------
		void* IndexList;
		PXSize DataIndexWidth;
		PXSize DataIndexSize;
		//--------------------
	}
	PXModel;

	typedef PXActionResult(*ModelCompilerFunction)(PXFile* const inputStream, PXFile* const outputStream);
	typedef PXActionResult(*ModelParserFunction)(PXFile* const inputStream, PXModel* const model);


	PXPublic void PXModelConstruct(PXModel* const model);
	PXPublic void PXModelDestruct(PXModel* const model);

	PXPublic PXSize PXModelMaterialAmount(const PXModel* const model);
	PXPublic PXBool PXModelMaterialGet(const PXModel* const model, const PXSize materialID, PXMaterial* const pxMaterial);


	PXPublic unsigned char PXModelSegmentsAmount(const PXModel* const model);
	PXPrivate void* PXModelSegmentsAdressGet(const PXModel* const model, const PXSize index);
	PXPublic void PXModelSegmentsGet(const PXModel* const model, const PXSize index, MeshSegment* const meshSegment);
	PXPublic void PXModelSegmentsAdd(PXModel* const model, const unsigned int renderMode, const unsigned int renderSize, const unsigned int renderMaterial);

	PXPublic PXSize PXModelVertexDataStride(const PXModel* const model);

	PXPublic PXActionResult PXModelLoad(PXModel* const model, const PXText* const filePath);
	PXPublic PXActionResult PXModelLoadD(PXModel* const model, PXFile* const fileStream, const FileFormatExtension modelType);

#ifdef __cplusplus
}
#endif

#endif