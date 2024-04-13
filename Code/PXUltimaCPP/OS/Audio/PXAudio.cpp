#include "PXAudio.hpp"

#include <OS/Memory/PXMemory.hpp>

PX::Audio::Audio()
{
	PX::Memory::Clear(this);
}

PX::Audio::~Audio()
{
	Release();
}

PX::ActionResult PXAPI PX::Audio::Initialize(const PXAudioSystem pxAudioSystem)
{
	return (PX::ActionResult)PXAudioInitialize(this, pxAudioSystem);
}

PX::ActionResult PXAPI PX::Audio::Release()
{
	return (PX::ActionResult)PXAudioRelease(this);
}