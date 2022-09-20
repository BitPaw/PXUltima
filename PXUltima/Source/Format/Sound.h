#ifndef SoundINCLUDE
#define SoundINCLUDE

#include <Format/Type.h>

#define PitchMaximum 2.9
#define PitchMinimum 0.2

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum SoundFormat_
	{
		SoundFormatInvalid,
		SoundFormatUnkown,
		SoundFormatAAC,
		SoundFormatFLAC,
		SoundFormatM4A,
		SoundFormatMID,
		SoundFormatMP3,
		SoundFormatOGG,
		SoundFormatWAV,
		SoundFormatWMA
	}
	SoundFormat;

	typedef enum SoundPlayStyle_
	{
		Once,
		Loop
	}
	SoundPlayStyle;

	typedef struct AudioClip_
	{
		unsigned int ID;
		SoundPlayStyle PlayStyle;
	}
	AudioClip;

	typedef struct AudioConfig_
	{
		float VolumeMusic;
		float VolumeSFX;
	}
	AudioConfig;

	typedef struct AudioSource_
	{
		unsigned int ID;
		float Pitch; // [0.5 - 2.0]
		float Volume;
		float Position[3];
		float Velocity[3];
		unsigned char Looping;
	}
	AudioSource;

	typedef struct Sound_
	{
		unsigned short NumerOfChannels;
		unsigned int SampleRate;
		unsigned short BitsPerSample;

		size_t DataSize;
		void* Data;
	}
	Sound;

	void AudioSourceConstruct(AudioSource* const audioSource);
	void AudioSourcePitchIncrease(AudioSource* const audioSource, float amount);
	void AudioSourcePitchReduce(AudioSource* const audioSource, float amount);

#ifdef __cplusplus
}
#endif

#endif