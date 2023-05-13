#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <Media/PXType.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f
#define SoundDeviceProductName 32u

#if OSUnix
typedef int AudioDeviceOutputHandle;
typedef int AudioDeviceInputHandle;
#elif OSWindows

#include <windows.h>

#if WindowsAtleastVista
#include <mmeapi.h> // MISSING
typedef void* AudioDeviceOutputHandle;
typedef void* AudioDeviceInputHandle;
#else
typedef void* AudioDeviceOutputHandle;
typedef void* AudioDeviceInputHandle;
#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif

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
	PXPrivate AudioResult PXAudioConvertMMResult(const unsigned int mmResultID);
#endif

	PXPublic void PXAudioDeviceCapabilitiesPrinf(AudioDeviceCapabilities* const audioDeviceCapabilities);


	PXPublic void PXAudioSourceConstruct(AudioSource* const audioSource);
	PXPublic void PXAudioSourcePitchIncrease(AudioSource* const audioSource, float amount);
	PXPublic void PXAudioSourcePitchReduce(AudioSource* const audioSource, float amount);


	PXPublic AudioResult PXAudioOutputOpen
	(
		AudioDeviceOutput* const audioDeviceOutput,
		const unsigned int deviceID,
		const unsigned short formatTag,         /* format type */
		const unsigned short channels,          /* number of channels (i.e. mono, stereo...) */
		const unsigned int samplesPerSec,     /* sample rate */
		const unsigned int avgBytesPerSec,    /* for buffer estimation */
		const unsigned short blockAlign,        /* block size of data */
		const unsigned short bitsPerSample,     /* number of bits per sample of mono data */
		const unsigned short cbSize             /* the count in bytes of the size of */
	);
	PXPublic AudioResult PXAudioOutputPrepare(AudioDeviceOutput* const audioDeviceOutput);
	PXPublic AudioResult PXAudioOutputWrite
	(
		AudioDeviceOutput* const audioDeviceOutput,
		void* const dataBuffer,
		const PXSize bufferLength,
		const unsigned int bytesRecorded,
		const unsigned int user,
		const unsigned int flags,
		const unsigned int loopControlCounter
	);
	PXPublic AudioResult PXAudioOutputClose(AudioDeviceOutput* const audioDeviceOutput);
	PXPublic AudioResult PXAudioOutputVolumeGet(AudioDeviceOutput* const audioDeviceOutput, unsigned short* const volume);

	PXPublic AudioResult PXAudioOutputVolumeSetEqual(AudioDeviceOutput* const audioDeviceOutput, const unsigned int volume);
	PXPublic AudioResult PXAudioOutputVolumeSetIndividual(AudioDeviceOutput* const audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight);
	PXPublic AudioResult PXAudioOutputPause(AudioDeviceOutput* const audioDeviceOutput);
	PXPublic AudioResult PXAudioOutputPitchSet(AudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);
	PXPublic AudioResult PXAudioOutputPlaybackRateSet(AudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);


	PXPublic AudioResult PXAudioDevicesFetchOutput(AudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);
	PXPublic AudioResult PXAudioDevicesFetchInput(AudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);

#ifdef __cplusplus
}
#endif

#endif
