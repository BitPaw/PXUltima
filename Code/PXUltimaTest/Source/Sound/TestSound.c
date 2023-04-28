#include "TestSound.h"

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

	const PXActionResult mappingResult = PXFileMapToMemoryA(&waveStream, "A:/TestWav.wav", 1024*8000, PXMemoryAccessModeReadAndWrite);
	const PXActionResult pxActionResult = PXWAVSerialize(&wav, &waveStream);

	PXFileDestruct(&waveStream);

	printf("\n");
}

void TestSoundWAV()
{
	PXWAV wav;

	PXFile dataStream;

	PXFileMapToMemoryA(&dataStream, "A:/W.wav", 0, PXMemoryAccessModeReadOnly);
	
	PXWAVParse(&wav, &dataStream);

	PXFileDestruct(&dataStream);
}

void TestSoundMP3()
{
	MP3 mp3;

	PXFile dataStream;

	PXFileMapToMemoryA(&dataStream, "A:/S.mp3", 0, PXMemoryAccessModeReadOnly);

	MP3Parse(&mp3, &dataStream);

	PXFileDestruct(&dataStream);
}