#ifndef PXMTLINCLUDE
#define PXMTLINCLUDE

#include <OS/File/PXFile.h>
#include <Media/PXModel.h>

#define MTLNameSize 32u
#define MTLFilePath 60u

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXMTLLineType_
	{
		MTLLineInvalid,
		MTLLineName,
		MTLLineTexture,
		MTLLineWeight,
		MTLLineColorAmbient,
		MTLLineColorDiffuse,
		MTLLineColorSpecular,
		MTLLineColorEmission,
		MTLLineDissolved,
		MTLLineDensity,
		MTLLineIllumination
	}
	PXMTLLineType;

	typedef enum PXMaterialIlluminationMode_
	{
		IlluminationNone,
		IlluminationColorAndAmbientDisable ,		// [0] Color on and Ambient off
		IlluminationColorAndAmbientEnable,		// [1] Color on and Ambient on
		IlluminationHighlightEnable ,	// [2] Highlight on
		IlluminationReflectionOnRayTraceEnable,	// [3] Reflection on and Ray trace on
		IlluminationReflectionOnRayTraceTransparency, 	// [4] Transparency: Glass on, Reflection : Ray trace on
		IlluminationReflectionOnRayTraceFresnel, 	// [5] Reflection : Fresnel on and Ray trace on
		IlluminationReflectionOnRayTraceTransparencyFresnel, 	// [6] Transparency : Refraction on, Reflection : Fresnel offand Ray trace on
		IlluminationReflectionOnRayTraceFullEnable,	// [7] Transparency : Refraction on, Reflection : Fresnel onand Ray trace on
		IlluminationReflectionEnable, 	// [8] Reflection on and Ray trace off
		IlluminationTransparencyEnable, 	// [9] Transparency : Glass on, Reflection : Ray trace off
		IlluminationShadowsEnable  	// [10] Casts shadows onto invisible surfaces
	}
	PXMaterialIlluminationMode;


	typedef struct MTLMaterial_
	{
		unsigned char NameSize;
		char* Name;

		unsigned char DiffuseTexturePathSize;
		char* DiffuseTexturePath;

		// Ranges between 0 and 1000
		float Weight;

		float Ambient[3];
		float Diffuse[3];
		float Specular[3];
		float Emission[3];

		float Dissolved;

		// range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
		float Density;

		PXMaterialIlluminationMode Illumination;
	}
	PXMTLMaterial;


	typedef struct PXMTL_
	{
		PXSize MaterialListSize;
		PXMTLMaterial* MaterialList;
	}
	PXMTL;

	PXPublic void PXMTLConstruct(PXMTL* const mtl);
	PXPublic void PXMTLDestruct(PXMTL* const mtl);

	PXPrivate PXMaterialIlluminationMode PXMTLIlluminationModeFromID(const unsigned int illuminationModeID);

	PXPrivate PXMTLLineType PXMTLPeekLine(const char* const line, const PXSize lineSize);

	PXPublic PXSize PXMTLFetchAmount(const void* const data, const PXSize dataSize);
	PXPublic PXBool PXMTLFetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, PXMTLMaterial* const mtlMaterial);

	PXPublic PXActionResult PXMTLFileCompile(PXFile* const inputStream, PXFile* const outputStream);

	PXPublic PXActionResult PXMTLParseToMaterial(PXFile* const inputStream, PXMaterialList* const pxMaterialList);

#ifdef __cplusplus
}
#endif

#endif
