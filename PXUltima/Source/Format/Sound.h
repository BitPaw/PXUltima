#ifndef SoundINCLUDE
#define SoundINCLUDE

#include <Format/Type.h>

#define PitchMaximum 2.9
#define PitchMinimum 0.2
#define SoundDeviceProductName 32u

#if OSUnix
#define AudioDeviceOutputHandle int
#define AudioDeviceInputHandle int
#elif OSWindows
#include <Windows.h>
#include <mmeapi.h> // MISSING
#define AudioDeviceOutputHandle HWAVEOUT
#define AudioDeviceInputHandle HWAVEIN
#endif

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
		SoundPlayStyleOnce,
		SoundPlayStyleLoop
	}
	SoundPlayStyle;

	typedef enum AudioResult_
	{
		AudioResultInvalid,

		AudioResultSucessful,

		AudioResultDeviceIDInvalid,
		AudioResultDeviceHandleInvalid,
		AudioResultDeviceNoDriver,

		AudioResultDriverNotReachable, // Maybe diabled, or deactivated
		AudioResultDriverNotEnabled, // No driver to use


		AudioResultDeviceAlreadyAllocated,
		AudioResultOutOfMemory,

		AudioResultNotSupported,

		AudioResultErrorValueInvalid,


		AudioResultFlagInvalid,
		AudioResultParameterInvalid,

		AudioResultDeviceHandleBusy,

		AudioResultAliasNotFound,



		AudioResultRegistryValueNotFound,

		AudioResultDriverNoCallback,

		AudioResultMoreData,


		// Custom
		AudioResultDeviceListNotBigEnough,


		// Windows spesific
		AudioResultRegistryDatabaseInvalid,
		AudioResultRegistryKeyNotFound,
		AudioResultRegistryWriteError,
		AudioResultRegistryReadError,
		AudioResultRegistryDeleteError,

		AudioResultErrorUnspecified, 		// Bad, but windows, am i right?



		// only
		AudioResultWaveFormatUnsupported,
		AudioResultDeviceIsStillPlaying,
		AudioResultReaderIsNotPrepared,
		AudioResultDeviceIsSynchronous
	}
	AudioResult;

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

	typedef struct AudioDeviceOutput_
	{
		AudioDeviceOutputHandle Handle;
	}
	AudioDeviceOutput;

	typedef struct AudioDeviceInput_
	{
		AudioDeviceInputHandle Handle;
	}
	AudioDeviceInput;


	typedef enum AudioDeviceType_
	{
		AudioDeviceTypeInvalid,
		AudioDeviceTypeInput,
		AudioDeviceTypeOutput
	}
	AudioDeviceType;

	typedef struct AudioDeviceCapabilities_
	{
		AudioDeviceType Type;
		unsigned int DeviceID;
		unsigned short ManufacturerID;
		unsigned short ProductID;
		unsigned short DriverVersionMajor;
		unsigned short DriverVersionMinor;

		wchar_t ProductName[32u];

		unsigned int   dwFormats;             /* formats supported */
		unsigned short Channels;
		unsigned int  dwSupport;             /* functionality supported by driver */
	}
	AudioDeviceCapabilities;

#if OSWindows
	CPrivate AudioResult AudioConvertMMResult(const unsigned int mmResultID);
#endif

	CPublic void AudioDeviceCapabilitiesPrinf(AudioDeviceCapabilities* const audioDeviceCapabilities);


	CPublic void AudioSourceConstruct(AudioSource* const audioSource);
	CPublic void AudioSourcePitchIncrease(AudioSource* const audioSource, float amount);
	CPublic	void AudioSourcePitchReduce(AudioSource* const audioSource, float amount);


	CPublic AudioResult  AudioOutputOpen
	(
		AudioDeviceOutput* audioDeviceOutput,
		unsigned int deviceID,
		unsigned short formatTag,         /* format type */
		unsigned short channels,          /* number of channels (i.e. mono, stereo...) */
		unsigned int samplesPerSec,     /* sample rate */
		unsigned int avgBytesPerSec,    /* for buffer estimation */
		unsigned short blockAlign,        /* block size of data */
		unsigned short bitsPerSample,     /* number of bits per sample of mono data */
		unsigned short cbSize             /* the count in bytes of the size of */
	);
	CPublic AudioResult  AudioOutputPrepare(AudioDeviceOutput* audioDeviceOutput);
	CPublic AudioResult  AudioOutputWrite
	(
		AudioDeviceOutput* audioDeviceOutput,
		void* dataBuffer,
		size_t bufferLength,
		unsigned int bytesRecorded,
		unsigned int user,
		unsigned int flags,
		unsigned int loopControlCounter
	);
	CPublic AudioResult  AudioOutputClose(AudioDeviceOutput* audioDeviceOutput);
	CPublic AudioResult  AudioOutputVolumeGet(AudioDeviceOutput* audioDeviceOutput, unsigned short* volume);

	CPublic AudioResult  AudioOutputVolumeSetEqual(AudioDeviceOutput* audioDeviceOutput, const unsigned int volume);
	CPublic AudioResult  AudioOutputVolumeSetIndividual(AudioDeviceOutput* audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight);
	CPublic AudioResult  AudioOutputPause(AudioDeviceOutput* audioDeviceOutput);
	CPublic AudioResult  AudioOutputPitchSet(AudioDeviceOutput* audioDeviceOutput, const unsigned int pitch);
	CPublic AudioResult  AudioOutputPlaybackRateSet(AudioDeviceOutput* audioDeviceOutput, const unsigned int pitch);


	CPublic AudioResult  AudioDevicesFetchOutput(AudioDeviceCapabilities* audioDeviceCapabilitiesList, const size_t audioDeviceCapabilitiesListSizeMax, size_t* audioDeviceCapabilitiesListSize);
	CPublic AudioResult  AudioDevicesFetchInput(AudioDeviceCapabilities* audioDeviceCapabilitiesList, const size_t audioDeviceCapabilitiesListSizeMax, size_t* audioDeviceCapabilitiesListSize);

#ifdef __cplusplus
}
#endif

#endif
