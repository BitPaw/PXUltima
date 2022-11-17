#ifndef OBJInclude
#define OBJInclude

#include <stddef.h>

#include <Error/ActionResult.h>
#include <Format/MTL/MTL.h>
#include <File/DataStream.h>
#include <Format/Model.h>

#define OBJNameSize 64u
#define OBJElementNameLength 50u
#define OBJDebug 1

#ifdef __cplusplus
extern "C"
{
#endif	

	typedef enum OBJLineType_
	{
		OBJLineInvalid,
		OBJLineNone,
		OBJLineComment,
		OBJLineMaterialLibraryInclude,
		OBJLineMaterialLibraryUse,
		OBJLineObjectName,
		OBJLineObjectGroup,
		OBJLineVertexGeometric,
		OBJLineVertexTexture,
		OBJLineVertexNormal,
		OBJLineVertexParameter,
		OBJLineSmoothShading,
		OBJLineFaceElement,

		OBJEmbeddedMTL
	}
	OBJLineType;

	typedef struct OBJElementMaterialInfo_
	{
		size_t MaterialIndex;
		size_t Size; // Size of buffer to use given material		
	}
	OBJElementMaterialInfo;

	typedef struct OBJElement_
	{
		char Name[OBJElementNameLength];
		float Smoothing;

		OBJElementMaterialInfo* MaterialInfo;
		size_t MaterialInfoSize;

		size_t VertexPositionListSize;
		float* VertexPositionList; 	// List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.

		size_t TextureCoordinateListSize;
		float* TextureCoordinateList; 	// List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.

		size_t VertexNormalPositionListSize;
		float* VertexNormalPositionList; 	// List of vertex normals in (x,y,z) form; normals might not be unit vectors.

		size_t VertexParameterListSize;
		float* VertexParameterList; // Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement.

		size_t FaceElementListSize;
		unsigned int* FaceElementList; // Polygonal face element.	
	}
	OBJElement;

	extern void OBJElementConstruct(OBJElement* objElement);
	extern void OBJElementDestruct(OBJElement* objElement);

	// [.OBJ] Wavefront - 3D model format
	typedef struct OBJ_
	{
		wchar_t Name[OBJNameSize];
		unsigned char VertexStructureSize;

		size_t MaterialFileListSize;
		MTL* MaterialFileList;

		size_t ElementListSize;
		OBJElement* ElementList;
	}
	OBJ;

	PXPublic void OBJConstruct(OBJ* const obj);
	PXPublic void OBJDestruct(OBJ* const obj);

	PXPrivate OBJLineType OBJPeekLine(const void* line, const size_t size);

	PXPublic ActionResult OBJFileCompile(DataStream* const inputStream, DataStream* const outputStream);

	PXPublic ActionResult OBJParseToModel(DataStream* const inputStream, Model* const model);

#ifdef __cplusplus
}
#endif

#endif