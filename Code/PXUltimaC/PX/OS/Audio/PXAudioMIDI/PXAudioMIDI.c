#ifndef PXAudioMIDIInclude
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




typedef UINT(WINAPI* PXmidiOutGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXmidiStreamOpen)(LPHMIDISTRM phms, LPUINT puDeviceID, DWORD cMidi, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXmidiStreamClose)(HMIDISTRM hms);
typedef MMRESULT(WINAPI* PXmidiStreamProperty)(HMIDISTRM hms, LPBYTE lppropdata, DWORD dwProperty);
typedef MMRESULT(WINAPI* PXmidiStreamPosition)(HMIDISTRM hms, LPMMTIME lpmmt, UINT cbmmt);
typedef MMRESULT(WINAPI* PXmidiStreamOut)(HMIDISTRM hms, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiStreamPause)(HMIDISTRM hms);
typedef MMRESULT(WINAPI* PXmidiStreamRestart)(HMIDISTRM hms);
typedef MMRESULT(WINAPI* PXmidiStreamStop)(HMIDISTRM hms);
typedef MMRESULT(WINAPI* PXmidiConnect)(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved);
typedef MMRESULT(WINAPI* PXmidiDisconnect)(HMIDI hmi, HMIDIOUT hmo, LPVOID pReserved);
typedef MMRESULT(WINAPI* PXmidiOutGetDevCapsA)(UINT_PTR uDeviceID, LPMIDIOUTCAPSA pmoc, UINT cbmoc);
typedef MMRESULT(WINAPI* PXmidiOutGetDevCapsW)(UINT_PTR uDeviceID, LPMIDIOUTCAPSW pmoc, UINT cbmoc);
typedef MMRESULT(WINAPI* PXmidiOutGetVolume)(HMIDIOUT hmo, LPDWORD pdwVolume);
typedef MMRESULT(WINAPI* PXmidiOutSetVolume)(HMIDIOUT hmo, DWORD dwVolume);
typedef MMRESULT(WINAPI* PXmidiOutGetErrorTextA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiOutGetErrorTextW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiOutOpen)(LPHMIDIOUT phmo, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXmidiOutClose)(HMIDIOUT hmo);
typedef MMRESULT(WINAPI* PXmidiOutPrepareHeader)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiOutUnprepareHeader)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiOutShortMsg)(HMIDIOUT hmo, DWORD dwMsg);
typedef MMRESULT(WINAPI* PXmidiOutLongMsg)(HMIDIOUT hmo, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiOutReset)(HMIDIOUT hmo);
typedef MMRESULT(WINAPI* PXmidiOutCachePatches)(HMIDIOUT hmo, UINT uBank, LPWORD pwpa, UINT fuCache);
typedef MMRESULT(WINAPI* PXmidiOutCacheDrumPatches)(HMIDIOUT hmo, UINT uPatch, LPWORD pwkya, UINT fuCache);
typedef MMRESULT(WINAPI* PXmidiOutGetID)(HMIDIOUT hmo, LPUINT puDeviceID);
typedef MMRESULT(WINAPI* PXmidiOutMessage)(HMIDIOUT hmo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
typedef UINT(WINAPI* PXmidiInGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXmidiInGetDevCapsA)(UINT_PTR uDeviceID, LPMIDIINCAPSA pmic, UINT cbmic);
typedef MMRESULT(WINAPI* PXmidiInGetDevCapsW)(UINT_PTR uDeviceID, LPMIDIINCAPSW pmic, UINT cbmic);
typedef MMRESULT(WINAPI* PXmidiInGetErrorTextA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiInGetErrorTextW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiInOpen)(LPHMIDIIN phmi, UINT uDeviceID, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXmidiInClose)(HMIDIIN hmi);
typedef MMRESULT(WINAPI* PXmidiInPrepareHeader)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiInUnprepareHeader)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiInAddBuffer)(HMIDIIN hmi, LPMIDIHDR pmh, UINT cbmh);
typedef MMRESULT(WINAPI* PXmidiInStart)(HMIDIIN hmi);
typedef MMRESULT(WINAPI* PXmidiInStop)(HMIDIIN hmi);
typedef MMRESULT(WINAPI* PXmidiInReset)(HMIDIIN hmi);
typedef MMRESULT(WINAPI* PXmidiInGetID)(HMIDIIN hmi, LPUINT puDeviceID);
typedef MMRESULT(WINAPI* PXmidiInMessage)(HMIDIIN hmi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
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
    // load library
    {
        const PXActionResult loadLibResult = PXLibraryOpenA(&pxAudio->Library, "winmm.dll");

        if (PXActionSuccessful != loadLibResult)
        {
            return PXActionLibraryNotFound;
        }
    }


    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = PXNew(PXWindowsAudioMultimedia);
    pxAudio->WindowsMultiMediaInterface = pxWindowsAudioMultimedia;

    // MAke new
    {

    }

    // Fetch all functions



    // Link
    {
        pxAudio->DeviceAmount = PXMIDIDeviceAmount;
        pxAudio->DeviceFetch = PXMIDIDeviceFetch;
        pxAudio->DeviceFetchAll = PXMIDIDeviceFetchAll;
        pxAudio->DeviceOpen = PXMIDIDeviceOpen;
        pxAudio->DeviceClose = PXMIDIDeviceClose;
        pxAudio->DeviceLoad = PXMIDIDeviceLoad;
        pxAudio->DevicePitchIncrease = PXMIDIDevicePitchIncrease;
        pxAudio->DevicePitchSet = PXMIDIDevicePitchSet;
        pxAudio->DevicePitchReduce = PXMIDIDevicePitchReduce;
        pxAudio->DeviceVolumeGet = PXMIDIDeviceVolumeGet;
        pxAudio->DeviceVolumeSetEqual = PXMIDIDeviceVolumeSetEqual;
        pxAudio->DeviceVolumeSetIndividual = PXMIDIDeviceVolumeSetIndividual;
        pxAudio->DeviceStart = PXMIDIDeviceStart;
        pxAudio->DeviceStop = PXMIDIDeviceStop;
        pxAudio->DevicePause = PXMIDIDevicePause;
        pxAudio->PlayCursorSet = PXMIDIDevicePlayCursorSet;
        pxAudio->PlayCursorGet = PXMIDIDevicePlayCursorGet;
        pxAudio->PlaySpeedSet = PXMIDIDevicePlaySpeedSet;
        pxAudio->PlaySpeedGet = PXMIDIDevicePlaySpeedGet;
        pxAudio->PositionSet = PXMIDIDevicePositionSet;
        pxAudio->PositionGet = PXMIDIDevicePositionGet;
        pxAudio->ConeAnglesSet = PXMIDIDeviceConeAnglesSet;
        pxAudio->ConeAnglesGet = PXMIDIDeviceConeAnglesGet;
        pxAudio->ConeOrientationGet = PXMIDIDeviceConeOrientationGet;
        pxAudio->ConeOrientationSet = PXMIDIDeviceConeOrientationSet;
        pxAudio->ConeOutsideVolumeGet = PXMIDIDeviceConeOutsideVolumeGet;
        pxAudio->ConeOutsideVolumeSet = PXMIDIDeviceConeOutsideVolumeSet;
        pxAudio->DistanceMaxGet = PXMIDIDeviceDistanceMaxGet;
        pxAudio->DistanceMaxSet = PXMIDIDeviceDistanceMaxSet;
        pxAudio->DistanceMinGet = PXMIDIDeviceDistanceMinGet;
        pxAudio->DistanceMinSet = PXMIDIDeviceDistanceMinSet;
        pxAudio->ModeGet = PXMIDIDeviceModeGet;
        pxAudio->ModeSet = PXMIDIDeviceModeSet;
        pxAudio->VelocityGet = PXMIDIDeviceVelocityGet;
        pxAudio->VelocitySet = PXMIDIDeviceVelocitySet;
        pxAudio->DistanceFactorGet = PXMIDIDeviceDistanceFactorGet;
        pxAudio->DistanceFactorSet = PXMIDIDeviceDistanceFactorSet;
        pxAudio->DopplerFactorGet = PXMIDIDeviceDopplerFactorGet;
        pxAudio->DopplerFactorSet = PXMIDIDeviceDopplerFactorSet;
        pxAudio->OrientationGet = PXMIDIDeviceOrientationGet;
        pxAudio->OrientationSet = PXMIDIDeviceOrientationSet;
        pxAudio->RolloffFactorGet = PXMIDIDeviceRolloffFactorGet;
        pxAudio->RolloffFactorSet = PXMIDIDeviceRolloffFactorSet;
        pxAudio->DeferredSettingsCommit = PXMIDIDeviceDeferredSettingsCommit;
    }


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIRelease(PXAudio* const pxAudio)
{
    PXLibraryClose(&pxAudio->Library);

    PXDelete(PXWindowsAudioMultimedia, pxAudio->WindowsMultiMediaInterface);
}

PXActionResult PXAPI PXMIDIDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    switch (pxAudioDeviceType)
    {
    case PXAudioDeviceTypeInput:
        *amount = pxWindowsAudioMultimedia->midiInGetNumDevs();
        break;

    case PXAudioDeviceTypeOutput:
        *amount = pxWindowsAudioMultimedia->midiOutGetNumDevs();
        break;

    default:
        return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT mmresultID = 0;

    PXClear(PXAudioDevice, pxAudioDevice);

    switch (pxAudioDeviceType)
    {
    case PXAudioDeviceTypeInput:
    {
        MIDIOUTCAPSA midi;

        mmresultID = pxWindowsAudioMultimedia->midiOutGetDevCapsA(deviceID, &midi, sizeof(midi));

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

        mmresultID = pxWindowsAudioMultimedia->midiOutGetDevCapsA(deviceID, &midi, sizeof(midi));

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
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    pxAudioDevice->Type = pxAudioDeviceType;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
    {
        resultID = pxWindowsAudioMultimedia->midiInOpen
                   (
                       &(HMIDIIN*)pxAudio->DirectSoundInterface,
                       deviceID,
                       (DWORD_PTR)PXMIDIInputMessageEvent,
                       (DWORD_PTR)pxAudioDevice,
                       CALLBACK_FUNCTION
                   );
        break;
    }
    case PXAudioDeviceTypeOutput:
        resultID = pxWindowsAudioMultimedia->midiOutOpen
                   (
                       &(HMIDIOUT*)pxAudio->DirectSoundInterface,
                       deviceID,
                       (DWORD_PTR)PXMIDIOutputMessageEvent,
                       (DWORD_PTR)pxAudioDevice,
                       CALLBACK_FUNCTION
                   );
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
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
        resultID = pxWindowsAudioMultimedia->midiInClose((HMIDIIN)pxAudio->DirectSoundInterface);
        break;

    case PXAudioDeviceTypeOutput:
        resultID = pxWindowsAudioMultimedia->midiOutClose((HMIDIOUT)pxAudio->DirectSoundInterface);
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

PXActionResult PXAPI PXMIDIDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
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

PXActionResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
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

PXActionResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const pflMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 flMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const pflMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 flMinDistance)
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

PXActionResult PXAPI PXMIDIDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
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

PXActionResult PXAPI PXMIDIDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXMIDIDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    //case PXAudioDeviceTypeInput:
    //    resultID = midiInGetVolume((HMIDIIN)pxAudio->DirectSoundInterface);
    //    break;

    case PXAudioDeviceTypeOutput:
    {
        DWORD volumeDW = 0;

        resultID = pxWindowsAudioMultimedia->midiOutGetVolume((HMIDIOUT)pxAudio->DirectSoundInterface, &volumeDW);

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
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
        resultID = pxWindowsAudioMultimedia->midiInClose((HMIDIIN)pxAudio->DirectSoundInterface);
        break;

    case PXAudioDeviceTypeOutput:
        resultID = pxWindowsAudioMultimedia->midiOutSetVolume((HMIDIOUT)pxAudio->DirectSoundInterface, 0);
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
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
        resultID = pxWindowsAudioMultimedia->midiInStart((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
        break;

    //case PXAudioDeviceTypeOutput:
    //    resultID = midiOutStart((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
    //    break;

    default:
        return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
        resultID = pxWindowsAudioMultimedia->midiInStop((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
        break;

    //case PXAudioDeviceTypeOutput:
    //    *amount = midiOutStop();
    //    break;

    default:
        return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMIDIDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    PXWindowsAudioMultimedia* pxWindowsAudioMultimedia = (PXWindowsAudioMultimedia*)pxAudio->WindowsMultiMediaInterface;

    MMRESULT resultID = 0;

    switch (pxAudioDevice->Type)
    {
    case PXAudioDeviceTypeInput:
        resultID = pxWindowsAudioMultimedia->midiInReset((HMIDIIN)pxAudioDevice->ResourceID.DirectXInterface);
        break;

    case PXAudioDeviceTypeOutput:
        resultID = pxWindowsAudioMultimedia->midiOutReset((HMIDIOUT)pxAudioDevice->ResourceID.DirectXInterface);
        break;

    default:
        return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}
#endif
#endif
