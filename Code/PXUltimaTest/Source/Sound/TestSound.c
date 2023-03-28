#include "TestSound.h"

#include <Media/PXSound.h>

#include <Media/WAV/WAV.h>
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
	WAV wav;

	PXDataStream waveStream;

	const PXActionResult mappingResult = PXDataStreamMapToMemoryA(&waveStream, "A:/TestWav.wav", 1024*8000, MemoryReadAndWrite);
	const PXActionResult pxActionResult = WAVSerialize(&wav, &waveStream);

	PXDataStreamDestruct(&waveStream);

	printf("\n");
}

void TestSoundWAV()
{
	WAV wav;

	PXDataStream dataStream;

	PXDataStreamMapToMemoryA(&dataStream, "A:/W.wav", 0, MemoryReadOnly);
	
	WAVParse(&wav, &dataStream);

	PXDataStreamDestruct(&dataStream);
}

void TestSoundMP3()
{
	MP3 mp3;

	PXDataStream dataStream;

	PXDataStreamMapToMemoryA(&dataStream, "A:/S.mp3", 0, MemoryReadOnly);

	MP3Parse(&mp3, &dataStream);

	PXDataStreamDestruct(&dataStream);
}