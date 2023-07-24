#ifndef PXSoundINCLUDE
#define PXSoundINCLUDE

#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSound_
	{
		PXInt32U SampleRate;
		PXInt16U NumerOfChannels;
		PXInt16U BitsPerSample;

		PXSize DataSize;
		void* Data;
	}
	PXSound;

	typedef PXActionResult(*PXSoundTranslateFunction)(const PXSound* const pxSound, PXFile* const pxFile);

	PXPublic void PXSoundConstruct(PXSound* const pxSound);
	PXPublic void PXSoundDestruct(PXSound* const pxSound);

	PXPublic PXActionResult PXSoundLoad(PXSound* const pxSound, const PXText* const filePath);
	PXPublic PXActionResult PXSoundLoadAs(PXSound* const pxSound, const PXFile* const pxFile, const FileFormatExtension guessedFormat);

	PXPublic PXActionResult PXSoundSave(PXSound* const pxSound, const PXText* const filePath, const FileFormatExtension fileFormat);
	PXPublic PXActionResult PXSoundSaveAs
	(
		PXSound* const pxSound,
		const PXFile* const pxFile,
		const FileFormatExtension fileFormat
	);

#ifdef __cplusplus
}
#endif

#endif
