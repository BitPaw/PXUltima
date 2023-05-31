#include "PXAudio.hpp"

#include <OS/Memory/PXMemory.h>

PX::AudioSource::AudioSource()
{
	PXAudioSourceConstruct(this);
}

PX::AudioSource::~AudioSource()
{
	
}

void PX::AudioSource::PitchIncrease(float amount)
{
	PXAudioSourcePitchIncrease(this, amount);
}

void PX::AudioSource::PitchReduce(float amount)
{
	PXAudioSourcePitchReduce(this, amount);
}

PX::AudioDeviceOutput::AudioDeviceOutput()
{
	PXMemoryClear(this, sizeof(*this));
}

PX::AudioDeviceOutput::~AudioDeviceOutput()
{

}

PX::ActionResult PX::AudioDeviceOutput::Open
(
	const unsigned int deviceID,
	const unsigned short formatTag, 
	const unsigned short channels,
	const unsigned int samplesPerSec, 
	const unsigned int avgBytesPerSec,
	const unsigned short blockAlign,
	const unsigned short bitsPerSample,
	const unsigned short cbSize
)
{
	return (PX::ActionResult)PXAudioOutputOpen
	(
		this,
		deviceID,
		formatTag,
		channels,
		samplesPerSec,
		avgBytesPerSec,
		blockAlign,
		bitsPerSample,
		cbSize
	);
}

PX::ActionResult PX::AudioDeviceOutput::Prepare()
{
	return (PX::ActionResult)PXAudioOutputPrepare(this);
}

PX::ActionResult PX::AudioDeviceOutput::Write(void* const dataBuffer, const PXSize bufferLength, const unsigned int bytesRecorded, const unsigned int user, const unsigned int flags, const unsigned int loopControlCounter)
{
	return (PX::ActionResult)PXAudioOutputWrite(this, dataBuffer, bufferLength, bytesRecorded, user, flags, loopControlCounter);
}

PX::ActionResult PX::AudioDeviceOutput::Close()
{
	return (PX::ActionResult)PXAudioOutputClose(this);
}

PX::ActionResult PX::AudioDeviceOutput::VolumeGet(unsigned short* const volume)
{
	return (PX::ActionResult)PXAudioOutputVolumeGet(this, volume);
}

PX::ActionResult PX::AudioDeviceOutput::VolumeSetEqual(const unsigned int volume)
{
	return (PX::ActionResult)PXAudioOutputVolumeSetEqual(this, volume);
}

PX::ActionResult PX::AudioDeviceOutput::VolumeSetIndividual(const unsigned short volumeLeft, const unsigned short volumeRight)
{
	return (PX::ActionResult)PXAudioOutputVolumeSetIndividual(this, volumeLeft, volumeRight);
}

PX::ActionResult PX::AudioDeviceOutput::Pause()
{
	return (PX::ActionResult)PXAudioOutputPause(this);
}

PX::ActionResult PX::AudioDeviceOutput::PitchSet(const unsigned int pitch)
{
	return (PX::ActionResult)PXAudioOutputPitchSet(this, pitch);
}

PX::ActionResult PX::AudioDeviceOutput::PlaybackRateSet(const unsigned int pitch)
{
	return (PX::ActionResult)PXAudioOutputPlaybackRateSet(this, pitch);
}