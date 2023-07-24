#ifndef PXModelINCLUDE
#define PXModelINCLUDE

#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMaterial_ PXMaterial;

	typedef struct PXMaterialContainer_
	{		
		PXMaterial* MaterialList;
		PXSize MaterialListSize;
	}
	PXMaterialContainer;


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
		PXSize DataSize;

		// MeshRenderList
		// unsigned char -> Mode
		// unsigned int -> Size
		// struct		-> Material
		// 

		//---<Material>---
		PXMaterialContainer MaterialContainer;
		//----------------

		//----------------
		float* DataVertexList;
		PXSize DataVertexWidth;
		PXSize DataVertexSize;

		float* DataNormalList;
		PXSize DataNormalWidth;
		PXSize DataNormalSize;

		float* DataTextureList;
		PXSize DataTextureWidth;
		PXSize DataTextureSize;

		float* DataColorList;
		PXSize DataColorWidth;
		PXSize DataColorSize;

		PXInt32U* DataIndexList;
		PXSize DataIndexWidth;
		PXSize DataIndexSize;
		//--------------------
	}
	PXModel;

	typedef PXActionResult(*ModelParserFunction)(PXModel* const pxModel, PXFile* const pxFile);


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
	PXPublic PXActionResult PXModelLoadD(PXModel* const model, PXFile* const pxFile, const FileFormatExtension modelType);

#ifdef __cplusplus
}
#endif

#endif