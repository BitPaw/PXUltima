#ifndef PXCPPAudioINCLUDE
#define PXCPPAudioINCLUDE

#include <OS/Audio/PXAudio.h>
#include <OS/Error/PXActionResult.hpp>

namespace PX
{
	class AudioSource : public PXAudioSource
	{
		public:
		AudioSource();
		~AudioSource();

		void PitchIncrease(float amount);
		void PitchReduce(float amount);
	};

	class AudioDeviceOutput : public PXAudioDeviceOutput
	{
		public:
		AudioDeviceOutput();
		~AudioDeviceOutput();

		PX::ActionResult Open
		(
			const unsigned int deviceID,
			const unsigned short formatTag,         /* format type */
			const unsigned short channels,          /* number of channels (i.e. mono, stereo...) */
			const unsigned int samplesPerSec,     /* sample rate */
			const unsigned int avgBytesPerSec,    /* for buffer estimation */
			const unsigned short blockAlign,        /* block size of data */
			const unsigned short bitsPerSample,     /* number of bits per sample of mono data */
			const unsigned short cbSize             /* the count in bytes of the size of */
		);
		PX::ActionResult Prepare();
		PX::ActionResult Write
		(
			void* const dataBuffer,
			const PXSize bufferLength,
			const unsigned int bytesRecorded,
			const unsigned int user,
			const unsigned int flags,
			const unsigned int loopControlCounter
		);
		PX::ActionResult Close();
		PX::ActionResult VolumeGet(unsigned short* const volume);

		PX::ActionResult VolumeSetEqual(const unsigned int volume);
		PX::ActionResult VolumeSetIndividual(const unsigned short volumeLeft, const unsigned short volumeRight);
		PX::ActionResult Pause();
		PX::ActionResult PitchSet(const unsigned int pitch);
		PX::ActionResult PlaybackRateSet(const unsigned int pitch);
	};
}

#endif