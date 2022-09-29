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

	// The renderable part of a mesh.
	typedef struct MeshSegment_
	{
		wchar_t Name[64];
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

		void* Data;



		



		size_t MaterialListSize;
		Material* MaterialList;
	}
	Model;

#ifdef __cplusplus
}
#endif

#endif