#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <OS/System/OSVersion.h>

#define PXAudioUSE OSDeviceToUse == OSDeviceDestop
#if PXAudioUSE

#include <Media/PXType.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f
#define SoundDeviceProductName 32u

#if OSUnix
typedef int PXAudioDeviceOutputHandle;
typedef int PXAudioDeviceInputHandle;
#elif OSWindows

#include <windows.h>

#if WindowsAtleastVista
#include <mmeapi.h> // MISSING
typedef void* PXAudioDeviceOutputHandle;
typedef void* PXAudioDeviceInputHandle;
#else
typedef void* PXAudioDeviceOutputHandle;
typedef void* PXAudioDeviceInputHandle;
#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXSoundPlayStyle_
	{
		SoundPlayStyleOnce,
		SoundPlayStyleLoop
	}
	PXSoundPlayStyle;

	typedef enum PXAudioResult_
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



		AudioResultPXRegistryValueNotFound,

		AudioResultDriverNoCallback,

		AudioResultMoreData,


		// Custom
		AudioResultDeviceListNotBigEnough,


		// Windows spesific
		AudioResultPXRegistryDatabaseInvalid,
		AudioResultPXRegistryKeyNotFound,
		AudioResultPXRegistryWriteError,
		AudioResultPXRegistryReadError,
		AudioResultPXRegistryDeleteError,

		AudioResultErrorUnspecified, 		// Bad, but windows, am i right?



		// only
		AudioResultWaveFormatUnsupported,
		AudioResultDeviceIsStillPlaying,
		AudioResultReaderIsNotPrepared,
		AudioResultDeviceIsSynchronous
	}
	PXAudioResult;

	typedef struct PXAudioClip_
	{
		unsigned int ID;
		PXSoundPlayStyle PlayStyle;
	}
	PXAudioClip;

	typedef struct PXAudioConfig_
	{
		float VolumeMusic;
		float VolumeSFX;
	}
	PXAudioConfig;

	typedef struct AudioSource_
	{
		unsigned int ID;
		float Pitch; // [0.5 - 2.0]
		float Volume;
		float Position[3];
		float Velocity[3];
		unsigned char Looping;
	}
	PXAudioSource;

	typedef struct AudioDeviceOutput_
	{
		PXAudioDeviceOutputHandle Handle;
	}
	PXAudioDeviceOutput;

	typedef struct PXAudioDeviceInput_
	{
		PXAudioDeviceInputHandle Handle;
	}
	PXAudioDeviceInput;


	typedef enum PXAudioDeviceType_
	{
		AudioDeviceTypeInvalid,
		AudioDeviceTypeInput,
		AudioDeviceTypeOutput
	}
	PXAudioDeviceType;

	typedef struct PXAudioDeviceCapabilities_
	{
		PXAudioDeviceType Type;
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
	PXAudioDeviceCapabilities;

#if OSWindows
	PXPrivate PXAudioResult PXAudioConvertMMResult(const unsigned int mmResultID);
#endif

	PXPublic void PXAudioDeviceCapabilitiesPrinf(PXAudioDeviceCapabilities* const audioDeviceCapabilities);


	PXPublic void PXAudioSourceConstruct(PXAudioSource* const audioSource);
	PXPublic void PXAudioSourcePitchIncrease(PXAudioSource* const audioSource, float amount);
	PXPublic void PXAudioSourcePitchReduce(PXAudioSource* const audioSource, float amount);


	PXPublic PXAudioResult PXAudioOutputOpen
	(
		PXAudioDeviceOutput* const audioDeviceOutput,
		const unsigned int deviceID,
		const unsigned short formatTag,         /* format type */
		const unsigned short channels,          /* number of channels (i.e. mono, stereo...) */
		const unsigned int samplesPerSec,     /* sample rate */
		const unsigned int avgBytesPerSec,    /* for buffer estimation */
		const unsigned short blockAlign,        /* block size of data */
		const unsigned short bitsPerSample,     /* number of bits per sample of mono data */
		const unsigned short cbSize             /* the count in bytes of the size of */
	);
	PXPublic PXAudioResult PXAudioOutputPrepare(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXAudioResult PXAudioOutputWrite
	(
		PXAudioDeviceOutput* const audioDeviceOutput,
		void* const dataBuffer,
		const PXSize bufferLength,
		const unsigned int bytesRecorded,
		const unsigned int user,
		const unsigned int flags,
		const unsigned int loopControlCounter
	);
	PXPublic PXAudioResult PXAudioOutputClose(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXAudioResult PXAudioOutputVolumeGet(PXAudioDeviceOutput* const audioDeviceOutput, unsigned short* const volume);

	PXPublic PXAudioResult PXAudioOutputVolumeSetEqual(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int volume);
	PXPublic PXAudioResult PXAudioOutputVolumeSetIndividual(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight);
	PXPublic PXAudioResult PXAudioOutputPause(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXAudioResult PXAudioOutputPitchSet(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);
	PXPublic PXAudioResult PXAudioOutputPlaybackRateSet(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);


	PXPublic PXAudioResult PXAudioDevicesFetchOutput(PXAudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);
	PXPublic PXAudioResult PXAudioDevicesFetchInput(PXAudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);

#ifdef __cplusplus
}
#endif

#endif
#endif