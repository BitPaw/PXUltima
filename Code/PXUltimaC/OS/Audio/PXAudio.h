#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXAudioUSE OSDeviceToUse == OSDeviceDestop
#if PXAudioUSE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f
#define SoundDeviceProductName 32u

#if OSUnix
typedef int PXAudioDeviceOutputHandle;
typedef int PXAudioDeviceInputHandle;
#elif OSWindows

#include <windows.h>

#if WindowsAtleastVista && PXOSWindowsDestop
#include <mmeapi.h> // MISSING
typedef HWAVEOUT PXAudioDeviceOutputHandle;
typedef HWAVEIN PXAudioDeviceInputHandle;
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

	typedef struct PXAudioSource_
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

#if PXOSWindowsDestop
	PXPrivate PXActionResult PXAudioConvertMMResult(const unsigned int mmResultID);
#endif

	PXPublic void PXAudioSourceConstruct(PXAudioSource* const audioSource);
	PXPublic void PXAudioSourcePitchIncrease(PXAudioSource* const audioSource, float amount);
	PXPublic void PXAudioSourcePitchReduce(PXAudioSource* const audioSource, float amount);


	PXPublic PXActionResult PXAudioOutputOpen
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
	PXPublic PXActionResult PXAudioOutputPrepare(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXActionResult PXAudioOutputWrite
	(
		PXAudioDeviceOutput* const audioDeviceOutput,
		void* const dataBuffer,
		const PXSize bufferLength,
		const unsigned int bytesRecorded,
		const unsigned int user,
		const unsigned int flags,
		const unsigned int loopControlCounter
	);
	PXPublic PXActionResult PXAudioOutputClose(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXActionResult PXAudioOutputVolumeGet(PXAudioDeviceOutput* const audioDeviceOutput, unsigned short* const volume);

	PXPublic PXActionResult PXAudioOutputVolumeSetEqual(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int volume);
	PXPublic PXActionResult PXAudioOutputVolumeSetIndividual(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight);
	PXPublic PXActionResult PXAudioOutputPause(PXAudioDeviceOutput* const audioDeviceOutput);
	PXPublic PXActionResult PXAudioOutputPitchSet(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);
	PXPublic PXActionResult PXAudioOutputPlaybackRateSet(PXAudioDeviceOutput* const audioDeviceOutput, const unsigned int pitch);


	PXPublic PXActionResult PXAudioDevicesFetchOutput(PXAudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);
	PXPublic PXActionResult PXAudioDevicesFetchInput(PXAudioDeviceCapabilities* const audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* const audioDeviceCapabilitiesListSize);

#ifdef __cplusplus
}
#endif

#endif
#endif