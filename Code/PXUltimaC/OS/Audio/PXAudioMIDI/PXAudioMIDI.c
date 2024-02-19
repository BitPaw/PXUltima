#include "PXAudioMIDI.h"

#if OSUnix
#elif OSWindows
#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif
#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia 
#endif
/*
midiOutGetDevCapsA
midiOutGetDevCapsW
midiOutGetErrorTextA

midiOutPrepareHeader
midiOutUnprepareHeader
midiOutShortMsg
midiOutLongMsg

midiOutCachePatches
midiOutCacheDrumPatches
midiOutGetID
midiOutMessage



midiInGetDevCapsW
midiInGetErrorTextA


midiInPrepareHeader
midiInUnprepareHeader
midiInAddBuffer

midiInGetID
midiInMessage

*/

#if OSWindows
// HMIDIIN hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2
void CALLBACK PXMIDIInputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2)
{
	printf("EX\n");
}
// HMIDIOUT hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2
void CALLBACK PXMIDIOutputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2)
{
	printf("EX\n");
}

PXActionResult PXAPI PXMIDIInitialize(PXAudio* const pxAudio)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
			*amount = midiInGetNumDevs();
			break;

		case PXAudioDeviceTypeOutput:
			*amount = midiOutGetNumDevs();
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT mmresultID = 0;

	PXClear(PXAudioDevice, pxAudioDevice);

	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
		{
			MIDIOUTCAPSA midi;

			mmresultID = midiOutGetDevCapsA(deviceID, &midi, sizeof(midi));

			pxAudioDevice->ManufacturerID = midi.wMid;  
			pxAudioDevice->ProductID = midi.wPid;                  
			pxAudioDevice->DriverVersionMajor = HIBYTE(midi.vDriverVersion);
			pxAudioDevice->DriverVersionMinor = LOBYTE(midi.vDriverVersion);

			PXTextCopyA(midi.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

#if (WINVER >= 0x0400)
			pxAudioDevice->SupportFlags = midi.dwSupport;
#endif
			break;
		}

		case PXAudioDeviceTypeOutput:
		{
			MIDIOUTCAPS midi;

			mmresultID = midiOutGetDevCapsA(deviceID, &midi, sizeof(midi));	

			pxAudioDevice->ManufacturerID = midi.wMid;
			pxAudioDevice->ProductID = midi.wPid;
			pxAudioDevice->DriverVersionMajor = HIBYTE(midi.vDriverVersion);
			pxAudioDevice->DriverVersionMinor = LOBYTE(midi.vDriverVersion);
			pxAudioDevice->Technology = midi.wTechnology;
			pxAudioDevice->Voices = midi.wVoices;
			pxAudioDevice->Notes = midi.wNotes;
			pxAudioDevice->ChannelMask = midi.wChannelMask;

			PXTextCopyA(midi.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

#if (WINVER >= 0x0400)
			pxAudioDevice->SupportFlags = midi.dwSupport;
#endif

			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	for (PXSize i = 0; i < amount; i++)
	{
		PXMIDIDeviceFetch(pxAudio, pxAudioDeviceType, i, &pxAudioDevice[i]);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	MMRESULT resultID = 0;

	pxAudioDevice->Type = pxAudioDeviceType;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			resultID = midiInOpen(&(HMIDIIN*)pxAudio->DirectSoundInterface, deviceID, (DWORD_PTR)PXMIDIInputMessageEvent, (DWORD_PTR)pxAudioDevice, CALLBACK_FUNCTION);
			break;
		}
		case PXAudioDeviceTypeOutput:
			resultID = midiOutOpen(&(HMIDIOUT*)pxAudio->DirectSoundInterface, deviceID, (DWORD_PTR)PXMIDIOutputMessageEvent, (DWORD_PTR)pxAudioDevice, CALLBACK_FUNCTION);
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}
	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT resultID = 0;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:		
			resultID = midiInClose((HMIDIIN)pxAudio->DirectSoundInterface);
			break;
		
		case PXAudioDeviceTypeOutput:
			resultID = midiOutClose((HMIDIOUT)pxAudio->DirectSoundInterface);
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}
	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	pxAudio->DirectSoundInterface = PXNull;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
	MMRESULT resultID = 0;

	switch (pxAudioDevice->Type)
	{
		//case PXAudioDeviceTypeInput:
		//	resultID = midiInGetVolume((HMIDIIN)pxAudio->DirectSoundInterface);
		//	break;

		case PXAudioDeviceTypeOutput:
		{
			DWORD volumeDW = 0;

			resultID = midiOutGetVolume((HMIDIOUT)pxAudio->DirectSoundInterface, &volumeDW);

			*volume = volumeDW;

			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}
	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	pxAudio->DirectSoundInterface = PXNull;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
	MMRESULT resultID = 0;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
			resultID = midiInClose((HMIDIIN)pxAudio->DirectSoundInterface);
			break;

		case PXAudioDeviceTypeOutput:
			resultID = midiOutSetVolume((HMIDIOUT)pxAudio->DirectSoundInterface, 0);
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}
	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	pxAudio->DirectSoundInterface = PXNull;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT resultID = 0;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
			resultID = midiInStart((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;

		//case PXAudioDeviceTypeOutput:
		//	resultID = midiOutStart((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
		//	break;

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{	
	MMRESULT resultID = 0;		

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
			resultID = midiInStop((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;

		//case PXAudioDeviceTypeOutput:
		//	*amount = midiOutStop();
		//	break;

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT resultID = 0;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
			resultID = midiInReset((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;

		case PXAudioDeviceTypeOutput:
			resultID = midiOutReset((HMIDIOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

	PXActionReturnOnError(result);

	return PXActionSuccessful;
}
#endif