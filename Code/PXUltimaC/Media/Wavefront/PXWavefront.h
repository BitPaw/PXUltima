#ifndef PXWavefrontINCLUDE
#define PXWavefrontINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>

#include <Media/MTL/PXMTL.h>
#include <Media/PXResource.h>

#define PXWavefrontNameSize 64u
#define PXWavefrontElementNameLength 50u
#define PXWavefrontDebug 1

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXWavefrontLineType_
	{
		PXWavefrontLineInvalid,
		PXWavefrontLineNone,
		PXWavefrontLineComment,
		PXWavefrontLineMaterialLibraryInclude,
		PXWavefrontLineMaterialLibraryUse,
		PXWavefrontLineObjectName,
		PXWavefrontLineObjectGroup,
		PXWavefrontLineVertexGeometric,
		PXWavefrontLineVertexTexture,
		PXWavefrontLineVertexNormal,
		PXWavefrontLineVertexParameter,
		PXWavefrontLineSmoothShading,
		PXWavefrontLineFaceElement,

		PXWavefrontEmbeddedMTL
	}
	PXWavefrontLineType;

	typedef struct PXWavefrontElementMaterialInfo_
	{
		PXSize MaterialIndex;
		PXSize Size; // Size of buffer to use given material
	}
	PXWavefrontElementMaterialInfo;

	typedef struct PXWavefrontElement_
	{
		char Name[PXWavefrontElementNameLength];
		float Smoothing;

		PXWavefrontElementMaterialInfo* MaterialInfo;
		PXSize MaterialInfoSize;

		PXSize VertexPositionListSize;
		float* VertexPositionList; 	// List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.

		PXSize TextureCoordinateListSize;
		float* TextureCoordinateList; 	// List of texture coordinates, in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.

		PXSize VertexNormalPositionListSize;
		float* VertexNormalPositionList; 	// List of vertex normals in (x,y,z) form; normals might not be unit vectors.

		PXSize VertexParameterListSize;
		float* VertexParameterList; // Parameter space vertices in ( u [,v] [,w] ) form; free form geometry statement.

		PXSize FaceElementListSize;
		unsigned int* FaceElementList; // Polygonal face element.
	}
	PXWavefrontElement;

	PXPublic void PXAPI PXWavefrontElementConstruct(PXWavefrontElement* objElement);
	PXPublic void PXAPI PXWavefrontElementDestruct(PXWavefrontElement* objElement);

	// [.PXWavefront] Wavefront - 3D model format
	typedef struct PXWavefront_
	{
		char Name[PXWavefrontNameSize];
		PXInt8U VertexStructureSize;

		PXSize MaterialFileListSize;
		PXMTL* MaterialFileList;

		PXSize ElementListSize;
		PXWavefrontElement* ElementList;
	}
	PXWavefront;

	PXPublic void PXAPI PXWavefrontConstruct(PXWavefront* const PXWavefront);
	PXPublic void PXAPI PXWavefrontDestruct(PXWavefront* const PXWavefront);

	PXPrivate PXWavefrontLineType PXAPI PXWavefrontPeekLine(const void* line, const PXSize size);

	PXPublic PXActionResult PXAPI PXWavefrontFileCompile(PXFile* const inputStream, PXFile* const outputStream);

	PXPublic PXActionResult PXAPI PXWavefrontLoadFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWavefrontSaveFromFile(PXVertexStructure* const pxVertexStructure, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
