#ifndef PXAudioMIDIIncluded
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
midiOutGetErrorA

midiOutPrepareHeader
midiOutUnprepareHeader
midiOutShortMsg
midiOutLongMsg

midiOutCachePatches
midiOutCacheDrumPatches
midiOutGetID
midiOutMessage



midiInGetDevCapsW
midiInGetErrorA


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
typedef MMRESULT(WINAPI* PXmidiOutGetErrorA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiOutGetErrorW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
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
typedef MMRESULT(WINAPI* PXmidiInGetErrorA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXmidiInGetErrorW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
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
void CALLBACK PXMIDIInputMessageEvent(void* hMidiIn, PXI32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2)
{
    printf("EX\n");
}
// HMIDIOUT hMidiIn, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2
void CALLBACK PXMIDIOutputMessageEvent(void* hMidiIn, PXI32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2)
{
    printf("EX\n");
}

PXResult PXAPI PXMIDIInitialize(PXAudio PXREF pxAudio)
{
    // load library
    {
        const PXResult loadLibResult = PXLibraryOpenA(&pxAudio->Library, "winmm.dll");

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

PXResult PXAPI PXMIDIRelease(PXAudio PXREF pxAudio)
{
    PXLibraryClose(&pxAudio->Library);

    PXDelete(PXWindowsAudioMultimedia, pxAudio->WindowsMultiMediaInterface);
}

PXResult PXAPI PXMIDIDeviceAmount(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXI32U PXREF amount)
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

PXResult PXAPI PXMIDIDeviceFetch(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice)
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

PXResult PXAPI PXMIDIDeviceFetchAll(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount)
{
    for (PXSize i = 0; i < amount; i++)
    {
        PXMIDIDeviceFetch(pxAudio, pxAudioDeviceType, i, &pxAudioDevice[i]);
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceOpen(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID)
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
    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceClose(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice)
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
    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    pxAudio->DirectSoundInterface = PXNull;

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceLoad(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXSound PXREF pxSound)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXSize offset)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXSize PXREF offset)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U frequency)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF frequency)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePositionSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePositionGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U dwInsideConeAngle, const PXI32U dwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pdwInsideConeAngle, PXI32U PXREF pdwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF plConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U lConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF pflMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 flMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF pflMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 flMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceModeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pdwMode)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceModeSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U dwMode)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceVelocityGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceVelocitySet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceOrientationGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pvOrientFront, PXI32U PXREF pvOrientTop)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceOrientationSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U xFront, const PXI32U yFront, const PXI32U zFront, const PXI32U xTop, const PXI32U yTop, const PXI32U zTop)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePitchIncrease(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePitchSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned int pitch)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDevicePitchReduce(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceVolumeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, unsigned short PXREF volume)
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
    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    pxAudio->DirectSoundInterface = PXNull;

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned int volume)
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
    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    pxAudio->DirectSoundInterface = PXNull;

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXMIDIDeviceStart(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice)
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

    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDeviceStop(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice)
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

    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}

PXResult PXAPI PXMIDIDevicePause(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice)
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

    const PXResult result = PXWindowsMMAudioConvertFromID(resultID);

    PXActionReturnOnError(result);

    return PXActionSuccessful;
}
#endif
#endif
