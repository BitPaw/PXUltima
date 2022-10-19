#ifndef ModelINCLUDE
#define ModelINCLUDE

#include <Format/Type.h>
#include <Error/ActionResult.h>

#include <File/DataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum ModelFileFormat_
	{
		ModelFileFormatInvalid,
		ModelFileFormatUnKown,
		ModelFileFormatA3DS,
		ModelFileFormatFBX,
		ModelFileFormatOBJ,
		ModelFileFormatPLY,
		ModelFileFormatSTL,
		ModelFileFormatWRL
	}
	ModelFileFormat;


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

		size_t MaterialListSize;
		Material* MaterialList;
	}
	Model;

	typedef ActionResult(*ModelCompilerFunction)(DataStream* const inputStream, DataStream* const outputStream);
	typedef ActionResult(*ModelParserFunction)(DataStream* const inputStream, Model* const model);


	PXPublic void ModelConstruct(Model* const model);
	PXPublic void ModelDestruct(Model* const model);

	PXPublic unsigned char ModelSegmentsAmount(const Model* const model);
	PXPublic void ModelSegmentsGet(const Model* const model, const size_t index, MeshSegment* const meshSegment);

	PXPrivate ModelFileFormat ModelGuessFormat(const wchar_t* const filePath);

	PXPublic ActionResult ModelLoadA(Model* const model, const char* const filePath);
	PXPublic ActionResult ModelLoadW(Model* const model, const wchar_t* const filePath);
	PXPublic ActionResult ModelLoadD(Model* const model, DataStream* const fileStream, const ModelFileFormat modelType);

#ifdef __cplusplus
}
#endif

#endif