#include "TestSound.h"

#include <OS/File/PXFile.h>
#include <Media/PXSound.h>
#include <Media/WAV/PXWAV.h>
#include <Media/MP3/MP3.h>

void TestSoundAll()
{
#if 1
	TestSoundWAVWrite();
#endif // 1

#if 1
	TestSoundWAV();
#endif // 0

#if 1
	TestSoundMP3();
#endif // 0
}

void TestSoundWAVWrite()
{
	PXWAV wav;

	PXFile waveStream;


	PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
	pxFileOpenFromPathInfo.FileSize = 1024 * 8000;
	pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadAndWrite;
	pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
	pxFileOpenFromPathInfo.AllowMapping = PXTrue;
	pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
	pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

	PXTextMakeFixedA(&pxFileOpenFromPathInfo.Text, "A:/TestWav.wav");

	const PXActionResult mappingResult = PXFileOpenFromPath(&waveStream, &pxFileOpenFromPathInfo);
	const PXActionResult pxActionResult = PXWAVSerialize(&wav, &waveStream);

	PXFileDestruct(&waveStream);

	printf("\n");
}

void TestSoundWAV()
{
	PXWAV wav;

	PXFile dataStream;

	//PXFileMapToMemoryA(&dataStream, "A:/W.wav", 0, PXMemoryAccessModeReadOnly);
	
	PXWAVParse(&wav, &dataStream);

	PXFileDestruct(&dataStream);
}

void TestSoundMP3()
{
	MP3 mp3;

	PXFile dataStream;

	//PXFileMapToMemoryA(&dataStream, "A:/S.mp3", 0, PXMemoryAccessModeReadOnly);

	MP3Parse(&mp3, &dataStream);

	PXFileDestruct(&dataStream);
}