#include "TestSound.h"

#include <Format/PXSound.h>

#include <Format/WAV/WAV.h>
#include <Format/MP3/MP3.h>

void TestSoundAll()
{
#if 1
	TestSoundWAV();
#endif // 0

#if 1
	TestSoundMP3();
#endif // 0
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