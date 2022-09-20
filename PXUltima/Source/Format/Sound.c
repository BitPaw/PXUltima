#include "Sound.h"

#include <assert.h>

#include <Memory/Memory.h>

void AudioSourceConstruct(AudioSource* const audioSource)
{
	MemorySet(audioSource, sizeof(AudioSource), 0);

	audioSource->Pitch = 0.4;
	audioSource->Volume = 0.02;
	audioSource->Looping = 1;
}

void AudioSourcePitchIncrease(AudioSource* const audioSource, float amount)
{
	assert(amount > 0.0f);

	audioSource->Pitch += amount;

	if (audioSource->Pitch > PitchMaximum)
	{
		audioSource->Pitch = PitchMaximum;
	}
}

void AudioSourcePitchReduce(AudioSource* const audioSource, float amount)
{
	assert(amount > 0.0f);

	audioSource->Pitch -= amount;

	if (audioSource->Pitch < PitchMinimum)
	{
		audioSource->Pitch = PitchMinimum;
	}
}