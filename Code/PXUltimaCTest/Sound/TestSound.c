#include "TestSound.h"

#include <OS/File/PXFile.h>
#include <OS/Audio/PXAudio.h>
#include <Media/Wave/PXWave.h>
#include <Media/MP3/PXMP3.h>

#include <math.h>

void TestSoundAll()
{
#if 0
	TestSoundPlaySong();
#endif 

#if 1
	TestSoundPlayMIDIDevice();
#endif // 1

#if 0
	TestSoundWAVWrite();
#endif // 1

#if 0
	TestSoundWAV();
#endif // 0

#if 0
	TestSoundMP3();
#endif // 0
}

void TestSoundPlaySong()
{
	PXAudio pxAudio;
	PXAudioDevice pxAudioDevice;
	PXClear(PXAudioDevice, &pxAudioDevice);

	PXActionResult init = PXAudioInitialize(&pxAudio, PXAudioSystemWindowsDirectSound);

	{
		PXAudioDevice pxAudioDevice;

		PXInt32U amount = 0;
		pxAudio.DeviceAmount(&pxAudio, PXAudioDeviceTypeInput, &amount);

		printf("--- Input Devices ---\n");

		for (size_t i = 0; i < amount; i++)
		{
			pxAudio.DeviceFetch(&pxAudio, PXAudioDeviceTypeInput, i, &pxAudioDevice);

			printf("[%i] %s\n", i, pxAudioDevice.DeviceName);
		}

		pxAudio.DeviceAmount(&pxAudio, PXAudioDeviceTypeOutput, &amount);

		printf("--- Output Devices ---\n");

		for (size_t i = 0; i < amount; i++)
		{
			pxAudio.DeviceFetch(&pxAudio, PXAudioDeviceTypeOutput, i, &pxAudioDevice);

			printf("[%i] %s\n", i, pxAudioDevice.DeviceName);
		}
	}

	PXSound pxSound;

//	PXResourceLoadA(&pxSound, "H:\\[Cache]\\chip.wav");
	 PXResourceLoadA(&pxSound, "H:\\[Cache]\\chipMONO.wav");




	//PXActionResult open = PXAudioDeviceOpen(&pxAudio, &pxAudioDevice, AudioDeviceTypeOutput, 0);
	pxAudio.DeviceOpen(&pxAudio, &pxAudioDevice, PXAudioDeviceTypeOutput, 0);

	pxAudioDevice.FXSettingFlagList |=
		//PXAudioFXGargle |
		PXAudioFXChorus;
		//PXAudioFXI3DL2Reverb;// |
		//PXAudioFXEcho;
		//PXAudioFXI3DL2Reverb;

	pxAudio.DeviceLoad(&pxAudio, &pxAudioDevice, &pxSound);

	pxAudio.DeviceStart(&pxAudio, &pxAudioDevice);

	//PXAudioDeviceStart(&pxAudio, &pxAudioDevice);

	//PXAudioDevicePitchSet(&pxAudio, &pxAudioDevice, 0x00011000);
	//PXAudioDeviceVolumeSetEqual(&pxAudio, &pxAudioDevice, 0xFFFF);


	pxAudio.DeferredSettingsCommit(&pxAudio, &pxAudioDevice);

	float x = 0;

	while (1)
	{
		float xPos = sin(x) * 3;

		//pxAudio.PlaySpeedSet(&pxAudio, &pxAudioDevice, (xPos +1)* 50000);	

		//pxAudio.PositionSet(&pxAudio, &pxAudioDevice, xPos, 0, 0);

		x += 0.01;

		printf("| AudioPosition: %9.4f |\n", xPos);

		//PXAudioDeviceStop(&pxAudio, &pxAudioDevice);
		//printf(".");
		PXThreadSleep(0, 1);
		//PXAudioDeviceStart(&pxAudio, &pxAudioDevice);
		//PXThreadSleep(0, 1);
	}


	PXActionResult close = pxAudio.DeviceClose(&pxAudio, &pxAudioDevice);
}

void TestSoundPlayMIDIDevice()
{
	printf("--- BEGIN TEST ---\n");

	PXAudio pxAudio;

	PXAudioInitialize(&pxAudio, PXAudioSystemWindowsMIDI);

	printf("--- Input MIDI devices ---\n");

	PXInt32U amountOfInputDevices = 0;

	pxAudio.DeviceAmount(&pxAudio, PXAudioDeviceTypeInput, &amountOfInputDevices);

	for (size_t i = 0; i < amountOfInputDevices; i++)
	{
		PXAudioDevice pxAudioDevice;

		pxAudio.DeviceFetch(&pxAudio, PXAudioDeviceTypeInput, i, &pxAudioDevice);

		printf("[%i] %s\n", i, pxAudioDevice.DeviceName);
	}

	printf("--- Output MIDI devices ---\n");

	PXInt32U amountOfOutputDevices = 0;

	pxAudio.DeviceAmount(&pxAudio, PXAudioDeviceTypeOutput, &amountOfOutputDevices);

	for (size_t i = 0; i < amountOfOutputDevices; i++)
	{
		PXAudioDevice pxAudioDevice;

		pxAudio.DeviceFetch(&pxAudio, PXAudioDeviceTypeOutput, i, &pxAudioDevice);

		printf("[%i] %s\n", i, pxAudioDevice.DeviceName);
	}



	PXAudioDevice pxAudioDevice;

	pxAudio.DeviceOpen(&pxAudio, &pxAudioDevice, PXAudioDeviceTypeOutput, 0);

	while (1)
	{
		printf("");
	}

	printf("--- END TEST ---\n");
}

void TestSoundWAVWrite()
{
	PXWave wav;

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
	const PXActionResult pxActionResult = PXWaveSaveToFile(&wav, &waveStream);

	PXFileDestruct(&waveStream);

	printf("\n");
}

void TestSoundWAV()
{
	PXWave wav;

	PXFile dataStream;

	//PXFileMapToMemoryA(&dataStream, "A:/W.wav", 0, PXMemoryAccessModeReadOnly);
	
	PXWaveLoadFromFile(&wav, &dataStream);

	PXFileDestruct(&dataStream);
}

void TestSoundMP3()
{
	PXMP3 mp3;

	PXFile dataStream;

	//PXFileMapToMemoryA(&dataStream, "A:/S.mp3", 0, PXMemoryAccessModeReadOnly);

	PXMP3LoadFromFile(&mp3, &dataStream);

	PXFileDestruct(&dataStream);
}