#include "PXMultiMedia.h"

#if OSWindows

#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif
#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia 

#endif

#include <OS/Console/PXConsole.h>
#include <OS/Memory/PXMemory.h>


#if OSWindows


typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsA)(UINT_PTR uDeviceID, LPWAVEOUTCAPSA pwoc, UINT cbwoc);
typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsW)(UINT_PTR uDeviceID, LPWAVEOUTCAPSW pwoc, UINT cbwoc);
typedef UINT(WINAPI* PXwaveOutGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXwaveOutGetVolume)(HWAVEOUT hwo, LPDWORD pdwVolume);
typedef MMRESULT(WINAPI* PXwaveOutSetVolume)(HWAVEOUT hwo, DWORD dwVolume);
typedef MMRESULT(WINAPI* PXwaveOutGetErrorTextA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveOutGetErrorTextW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveOutOpen)(LPHWAVEOUT phwo, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXwaveOutClose)(HWAVEOUT hwo);
typedef MMRESULT(WINAPI* PXwaveOutPrepareHeader)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveOutUnprepareHeader)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveOutWrite)(HWAVEOUT hwo, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveOutPause)(HWAVEOUT hwo);
typedef MMRESULT(WINAPI* PXwaveOutRestart)(HWAVEOUT hwo);
typedef MMRESULT(WINAPI* PXwaveOutReset)(HWAVEOUT hwo);
typedef MMRESULT(WINAPI* PXwaveOutBreakLoop)(HWAVEOUT hwo);
typedef MMRESULT(WINAPI* PXwaveOutGetPosition)(HWAVEOUT hwo, LPMMTIME pmmt, UINT cbmmt);
typedef MMRESULT(WINAPI* PXwaveOutGetPitch)(HWAVEOUT hwo, LPDWORD pdwPitch);
typedef MMRESULT(WINAPI* PXwaveOutSetPitch)(HWAVEOUT hwo, DWORD dwPitch);
typedef MMRESULT(WINAPI* PXwaveOutGetPlaybackRate)(HWAVEOUT hwo, LPDWORD pdwRate);
typedef MMRESULT(WINAPI* PXwaveOutSetPlaybackRate)(HWAVEOUT hwo, DWORD dwRate);
typedef MMRESULT(WINAPI* PXwaveOutGetID)(HWAVEOUT hwo, LPUINT puDeviceID);
typedef MMRESULT(WINAPI* PXwaveOutMessage)(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
typedef UINT(WINAPI* PXwaveInGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXwaveInGetDevCapsA)(UINT_PTR uDeviceID, LPWAVEINCAPSA pwic, UINT cbwic);
typedef MMRESULT(WINAPI* PXwaveInGetDevCapsW)(UINT_PTR uDeviceID, LPWAVEINCAPSW pwic, UINT cbwic);
typedef MMRESULT(WINAPI* PXwaveInGetErrorTextA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveInGetErrorTextW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveInOpen)(LPHWAVEIN phwi, UINT uDeviceID, LPCWAVEFORMATEX pwfx, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXwaveInClose)(HWAVEIN hwi);
typedef MMRESULT(WINAPI* PXwaveInPrepareHeader)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveInUnprepareHeader)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveInAddBuffer)(HWAVEIN hwi, LPWAVEHDR pwh, UINT cbwh);
typedef MMRESULT(WINAPI* PXwaveInStart)(HWAVEIN hwi);
typedef MMRESULT(WINAPI* PXwaveInStop)(HWAVEIN hwi);
typedef MMRESULT(WINAPI* PXwaveInReset)(HWAVEIN hwi);
typedef MMRESULT(WINAPI* PXwaveInGetPosition)(HWAVEIN hwi, LPMMTIME pmmt, UINT cbmmt);
typedef MMRESULT(WINAPI* PXwaveInGetID)(HWAVEIN hwi, LPUINT puDeviceID);
typedef MMRESULT(WINAPI* PXwaveInMessage)(HWAVEIN hwi, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);


typedef UINT(WINAPI* PXauxGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXauxGetDevCapsA)(UINT_PTR uDeviceID, LPAUXCAPSA pac, UINT cbac);
typedef MMRESULT(WINAPI* PXauxGetDevCapsW)(UINT_PTR uDeviceID, LPAUXCAPSW pac, UINT cbac);
typedef MMRESULT(WINAPI* PXauxSetVolume)(UINT uDeviceID, DWORD dwVolume);
typedef MMRESULT(WINAPI* PXauxGetVolume)(UINT uDeviceID, LPDWORD pdwVolume);
typedef MMRESULT(WINAPI* PXauxOutMessage)(UINT uDeviceID, UINT uMsg, DWORD_PTR dw1, DWORD_PTR dw2);
typedef UINT(WINAPI* PXmixerGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXmixerGetDevCapsA)(UINT_PTR uMxId, LPMIXERCAPSA pmxcaps, UINT cbmxcaps);
typedef MMRESULT(WINAPI* PXmixerGetDevCapsW)(UINT_PTR uMxId, LPMIXERCAPSW pmxcaps, UINT cbmxcaps);
typedef MMRESULT(WINAPI* PXmixerOpen)(LPHMIXER phmx, UINT uMxId, DWORD_PTR dwCallback, DWORD_PTR dwInstance, DWORD fdwOpen);
typedef MMRESULT(WINAPI* PXmixerClose)(HMIXER hmx);
typedef DWORD(WINAPI* PXmixerMessage)(HMIXER hmx, UINT uMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
typedef MMRESULT(WINAPI* PXmixerGetLineInfoA)(HMIXEROBJ hmxobj, LPMIXERLINEA pmxl, DWORD fdwInfo);
typedef MMRESULT(WINAPI* PXmixerGetLineInfoW)(HMIXEROBJ hmxobj, LPMIXERLINEW pmxl, DWORD fdwInfo);
typedef MMRESULT(WINAPI* PXmixerGetID)(HMIXEROBJ hmxobj, UINT FAR* puMxId, DWORD fdwId);
typedef MMRESULT(WINAPI* PXmixerGetLineControlsA)(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSA pmxlc, DWORD fdwControls);
typedef MMRESULT(WINAPI* PXmixerGetLineControlsW)(HMIXEROBJ hmxobj, LPMIXERLINECONTROLSW pmxlc, DWORD fdwControls);
typedef MMRESULT(WINAPI* PXmixerGetControlDetailsA)(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);
typedef MMRESULT(WINAPI* PXmixerGetControlDetailsW)(HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd, DWORD fdwDetails);

#endif

PXActionResult PXAPI PXMultiMediaDeviceStart(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            PXwaveInStart pxWaveInStart = (PXwaveInStart)pxAudioMultiMedia->WaveInStart;
            HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveInStart(handleWaveIn);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXwaveOutRestart pxwaveOutRestart = (PXwaveOutRestart)pxAudioMultiMedia->WaveOutRestart;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxwaveOutRestart(handleWaveOut);
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceStop(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            PXwaveInStop pxwaveInStop = (PXwaveInStop)pxAudioMultiMedia->WaveInStop;
            HWAVEIN waveHandle = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            result = pxwaveInStop(waveHandle);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXwaveOutPause pxwaveOutPause = (PXwaveOutPause)pxAudioMultiMedia->WaveOutPause;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxwaveOutPause(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePause(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        //case PXAudioDeviceTypeInput:
        //{
        //    result = waveInPause((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
        //    break;
        //}
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutPause pxWaveOutPause = (PXwaveOutPause)pxAudioMultiMedia->WaveOutPause;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;
            
            result = pxWaveOutPause(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceRestart(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        //case PXAudioDeviceTypeInput:
        //{
        //    result = waveInRestart((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
        //    break;
        //}
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutRestart pxWaveOutRestart = (PXwaveOutRestart)pxAudioMultiMedia->WaveOutRestart;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutRestart(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    pxAudioDevice->Info.DirectXInterface = PXNull;

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceReset(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            const PXwaveInReset pxWaveInReset = (PXwaveInReset)pxAudioMultiMedia->WaveInReset;
            const HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveInReset(handleWaveIn);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutReset pxWaveOutReset = (PXwaveOutReset)pxAudioMultiMedia->WaveOutReset;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutReset(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    pxAudioDevice->Info.DirectXInterface = PXNull;

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioInitializeInfo* const pxAudioInitializeInfo)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "WinMM",
        "Initialize",
        "..."
    );
#endif

    // Library
    {
        const PXActionResult loadLibResult = PXLibraryOpenA(&pxAudioMultiMedia->MultiMediaLibrary, "WINMM.DLL");

        if(PXActionSuccessful != loadLibResult)
        {
            return PXActionFailedNotFound;
        }    
    }

    // Fetch
    {
        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &pxAudioMultiMedia->WaveOutGetDevCapsA, "waveOutGetDevCapsA"},
            { &pxAudioMultiMedia->WaveOutGetDevCapsW,"waveOutGetDevCapsW"},
            { &pxAudioMultiMedia->WaveOutGetNumDevs, "waveOutGetNumDevs"},
            { &pxAudioMultiMedia->WaveOutGetVolume,"waveOutGetVolume"},
            { &pxAudioMultiMedia->WaveOutSetVolume,"waveOutSetVolume"},
            { &pxAudioMultiMedia->WaveOutGetErrorTextA,"waveOutGetErrorTextA"},
            { &pxAudioMultiMedia->WaveOutGetErrorTextW,"waveOutGetErrorTextW"},
            { &pxAudioMultiMedia->WaveOutOpen,"waveOutOpen"},
            { &pxAudioMultiMedia->WaveOutClose,"waveOutClose"},
            { &pxAudioMultiMedia->WaveOutPrepareHeader,"waveOutPrepareHeader"},
            { &pxAudioMultiMedia->WaveOutUnprepareHeader,"waveOutUnprepareHeader"},
            { &pxAudioMultiMedia->WaveOutWrite, "waveOutWrite"},
            { &pxAudioMultiMedia->WaveOutPause,"waveOutPause" },
            { &pxAudioMultiMedia->WaveOutRestart,"waveOutRestart" },
            { &pxAudioMultiMedia->WaveOutReset,"waveOutReset" },
            { &pxAudioMultiMedia->WaveOutBreakLoop,"waveOutBreakLoop" },
            { &pxAudioMultiMedia->WaveOutGetPosition,"waveOutGetPosition" },
            { &pxAudioMultiMedia->WaveOutGetPitch,"waveOutGetPitch" },
            { &pxAudioMultiMedia->WaveOutSetPitch,"waveOutSetPitch" },
            { &pxAudioMultiMedia->WaveOutGetPlaybackRate,"waveOutGetPlaybackRate" },
            { &pxAudioMultiMedia->WaveOutSetPlaybackRate,"waveOutSetPlaybackRate" },
            { &pxAudioMultiMedia->WaveOutGetID,"waveOutGetID" },
            { &pxAudioMultiMedia->WaveOutMessage, "waveOutMessage" },
            { &pxAudioMultiMedia->WaveInGetNumDevs, "waveInGetNumDevs" },
            { &pxAudioMultiMedia->WaveInGetDevCapsA, "waveInGetDevCapsA" },
            { &pxAudioMultiMedia->WaveInGetDevCapsW, "waveInGetDevCapsW" },
            { &pxAudioMultiMedia->WaveInGetErrorTextA, "waveInGetErrorTextA" },
            { &pxAudioMultiMedia->WaveInGetErrorTextW,    "waveInGetErrorTextW" },
            { &pxAudioMultiMedia->WaveInOpen,    "waveInOpen" },
            { &pxAudioMultiMedia->WaveInClose,    "waveInClose" },
            { &pxAudioMultiMedia->WaveInPrepareHeader,    "waveInPrepareHeader" },
            { &pxAudioMultiMedia->WaveInUnprepareHeader,    "waveInUnprepareHeader" },
            { &pxAudioMultiMedia->WaveInAddBuffer,    "waveInAddBuffer" },
            { &pxAudioMultiMedia->WaveInStart,    "waveInStart" },
            { &pxAudioMultiMedia->WaveInStop,    "waveInStop" },
            { &pxAudioMultiMedia->WaveInReset,    "waveInReset" },
            { &pxAudioMultiMedia->WaveInGetPosition,    "waveInGetPosition" },
            { &pxAudioMultiMedia->WaveInGetID,    "waveInGetID" },
            { &pxAudioMultiMedia->WaveInMessage,    "waveInMessage" },
            { &pxAudioMultiMedia->MidiOutGetNumDevs,    "midiOutGetNumDevs" },
            { &pxAudioMultiMedia->MidiStreamOpen,"midiStreamOpen" },
            { &pxAudioMultiMedia->MidiStreamClose,"midiStreamClose" },
            { &pxAudioMultiMedia->MidiStreamProperty,"midiStreamProperty" },
            { &pxAudioMultiMedia->MidiStreamPosition,"midiStreamPosition" },
            { &pxAudioMultiMedia->MidiStreamOut,"midiStreamOut" },
            { &pxAudioMultiMedia->MidiStreamPause,"midiStreamPause" },
            { &pxAudioMultiMedia->MidiStreamRestart,"midiStreamRestart" },
            { &pxAudioMultiMedia->MidiStreamStop,"midiStreamStop" },
            { &pxAudioMultiMedia->MidiConnect,"midiConnect" },
            { &pxAudioMultiMedia->MidiDisconnect,"midiDisconnect" },
            { &pxAudioMultiMedia->MidiOutGetDevCapsA,"midiOutGetDevCapsA" },
            { &pxAudioMultiMedia->MidiOutGetDevCapsW,"midiOutGetDevCapsW" },
            { &pxAudioMultiMedia->MidiOutGetVolume,"midiOutGetVolume" },
            { &pxAudioMultiMedia->MidiOutSetVolume,"midiOutSetVolume" },
            { &pxAudioMultiMedia->MidiOutGetErrorTextA,"midiOutGetErrorTextA" },
            { &pxAudioMultiMedia->MidiOutGetErrorTextW,"midiOutGetErrorTextW" },
            { &pxAudioMultiMedia->MidiOutOpen,"midiOutOpen" },
            { &pxAudioMultiMedia->MidiOutClose,"midiOutClose" },
            { &pxAudioMultiMedia->MidiOutPrepareHeader,"midiOutPrepareHeader" },
            { &pxAudioMultiMedia->MidiOutUnprepareHeader,"midiOutUnprepareHeader" },
            { &pxAudioMultiMedia->MidiOutShortMsg,"midiOutShortMsg" },
            { &pxAudioMultiMedia->MidiOutLongMsg,"midiOutLongMsg" },
            { &pxAudioMultiMedia->MidiOutReset,"midiOutReset" },
            { &pxAudioMultiMedia->MidiOutCachePatches,"midiOutCachePatches" },
            { &pxAudioMultiMedia->MidiOutCacheDrumPatches,"midiOutCacheDrumPatches" },
            { &pxAudioMultiMedia->MidiOutGetID,"midiOutGetID" },
            { &pxAudioMultiMedia->MidiOutMessage,"midiOutMessage" },
            { &pxAudioMultiMedia->MidiInGetNumDevs,"midiInGetNumDevs" },
            { &pxAudioMultiMedia->MidiInGetDevCapsA,"midiInGetDevCapsA" },
            { &pxAudioMultiMedia->MidiInGetDevCapsW,"midiInGetDevCapsW" },
            { &pxAudioMultiMedia->MidiInGetErrorTextA,"midiInGetErrorTextA" },
            { &pxAudioMultiMedia->MidiInGetErrorTextW,"midiInGetErrorTextW" },
            { &pxAudioMultiMedia->MidiInOpen,"midiInOpen" },
            { &pxAudioMultiMedia->MidiInClose,"midiInClose" },
            { &pxAudioMultiMedia->MidiInPrepareHeader,"midiInPrepareHeader" },
            { &pxAudioMultiMedia->MidiInUnprepareHeader,"midiInUnprepareHeader" },
            { &pxAudioMultiMedia->MidiInAddBuffer,"midiInAddBuffer" },
            { &pxAudioMultiMedia->MidiInStart,"midiInStart" },
            { &pxAudioMultiMedia->MidiInStop,"midiInStop" },
            { &pxAudioMultiMedia->MidiInReset,"midiInReset" },
            { &pxAudioMultiMedia->MidiInGetID,"midiInGetID" },
            { &pxAudioMultiMedia->MidiInMessage,"midiInMessage" },
            { &pxAudioMultiMedia->AuxGetNumDevs,"auxGetNumDevs" },
            { &pxAudioMultiMedia->AuxGetDevCapsA,"auxGetDevCapsA" },
            { &pxAudioMultiMedia->AuxGetDevCapsW,"auxGetDevCapsW" },
            { &pxAudioMultiMedia->AuxSetVolume,"auxSetVolume" },
            { &pxAudioMultiMedia->AuxGetVolume,"auxGetVolume" },
            { &pxAudioMultiMedia->AuxOutMessage,"auxOutMessage" },
            { &pxAudioMultiMedia->MixerGetNumDevs,"mixerGetNumDevs" },
            { &pxAudioMultiMedia->MixerGetDevCapsA,"mixerGetDevCapsA" },
            { &pxAudioMultiMedia->MixerGetDevCapsW,"mixerGetDevCapsW" },
            { &pxAudioMultiMedia->MixerOpen,"mixerOpen" },
            { &pxAudioMultiMedia->MixerClose,"mixerClose" },
            { &pxAudioMultiMedia->MixerMessage,"mixerMessage" },
            { &pxAudioMultiMedia->MixerGetLineInfoA,"mixerGetLineInfoA" },
            { &pxAudioMultiMedia->MixerGetLineInfoW,"mixerGetLineInfoW" },
            { &pxAudioMultiMedia->MixerGetID,"mixerGetID" },
            { &pxAudioMultiMedia->MixerGetLineControlsA,"mixerGetLineControlsA" },
            { &pxAudioMultiMedia->MixerGetLineControlsW,"mixerGetLineControlsW" },
            { &pxAudioMultiMedia->MixerGetControlDetailsA,"mixerGetControlDetailsA" },
            { &pxAudioMultiMedia->MixerGetControlDetailsW,"mixerGetControlDetailsW" }
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxAudioMultiMedia->MultiMediaLibrary, pxLibraryFuntionEntryList, amount);
    }

    // Link
    {
        PXAudio* const pxAudio = pxAudioInitializeInfo->AudioReference;

        pxAudio->DeviceAmount = PXMultiMediaDeviceAmount;
        pxAudio->DeviceFetch = PXMultiMediaDeviceFetch;
        pxAudio->DeviceFetchAll = PXMultiMediaDeviceFetchAll;
        pxAudio->DeviceOpen = PXMultiMediaDeviceOpen;
        pxAudio->DeviceClose = PXMultiMediaDeviceClose;
        pxAudio->DeviceLoad = PXMultiMediaDeviceLoad;
        pxAudio->DeviceBreakLoop = PXMultiMediaDeviceBreakLoop;
        pxAudio->DevicePositionGet = PXMultiMediaDevicePositionGet;
        pxAudio->DevicePlaybackRateGet = PXMultiMediaDevicePlaybackRateGet;
        pxAudio->DevicePlaybackRateSet = PXMultiMediaDevicePlaybackRateSet;
        pxAudio->DevicePitchIncrease = PXMultiMediaDevicePitchIncrease;
        pxAudio->DevicePitchSet = PXMultiMediaDevicePitchSet;
        pxAudio->DevicePitchReduce = PXMultiMediaDevicePitchReduce;
        pxAudio->DeviceVolumeGet = PXMultiMediaDeviceVolumeGet;
        pxAudio->DeviceVolumeSetEqual = PXMultiMediaDeviceVolumeSetEqual;
        pxAudio->DeviceVolumeSetIndividual = PXMultiMediaDeviceVolumeSetIndividual;
        pxAudio->DeviceStart = PXMultiMediaDeviceStart;
        pxAudio->DeviceStop = PXMultiMediaDeviceStop;
        pxAudio->DevicePause = PXMultiMediaDevicePause;
        pxAudio->DeviceRestart = PXMultiMediaDeviceRestart;
        pxAudio->DeviceReset = PXMultiMediaDeviceReset;
    }

    return PXActionSuccessful;    
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceAmount(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    const PXwaveInGetNumDevs pxWaveInGetNumDevs = (PXwaveInGetNumDevs)pxAudioMultiMedia->WaveInGetNumDevs;
    const PXwaveOutGetNumDevs pxWaveOutGetNumDevs = (PXwaveOutGetNumDevs)pxAudioMultiMedia->WaveOutGetNumDevs;

    pxAudioDeviceAmountInfo->DeviceInput = pxWaveInGetNumDevs();
    pxAudioDeviceAmountInfo->DeviceOutput = pxWaveOutGetNumDevs();
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXClear(PXAudioDevice, pxAudioDevice);

    MMRESULT result;

    pxAudioDevice->Type = pxAudioDeviceType;

    switch (pxAudioDeviceType)
    {
        case PXAudioDeviceTypeInput:
        {
            const UINT size = sizeof(WAVEINCAPS);
            WAVEINCAPS waveInCapsA;

            PXwaveInGetDevCapsA pxWaveInGetDevCapsA = (PXwaveInGetDevCapsA)pxAudioMultiMedia->WaveInGetDevCapsA;

            result = pxWaveInGetDevCapsA(deviceID, &waveInCapsA, size);

            pxAudioDevice->ManufacturerID = waveInCapsA.wMid;
            pxAudioDevice->ProductID = waveInCapsA.wPid;
            pxAudioDevice->DriverVersionMajor = HIBYTE(waveInCapsA.vDriverVersion);
            pxAudioDevice->DriverVersionMinor = LOBYTE(waveInCapsA.vDriverVersion);
            pxAudioDevice->Channels = waveInCapsA.wChannels;

            PXTextCopyA(waveInCapsA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const UINT size = sizeof(WAVEOUTCAPS);
            WAVEOUTCAPS waveOutCapA;

            PXwaveOutGetDevCapsA pxwaveOutGetDevCapsA = (PXwaveOutGetDevCapsA)pxAudioMultiMedia->WaveOutGetDevCapsA;

            result = pxwaveOutGetDevCapsA(deviceID, &waveOutCapA, size);

            pxAudioDevice->ManufacturerID = waveOutCapA.wMid;
            pxAudioDevice->ProductID = waveOutCapA.wPid;
            pxAudioDevice->DriverVersionMajor = HIBYTE(waveOutCapA.vDriverVersion);
            pxAudioDevice->DriverVersionMinor = LOBYTE(waveOutCapA.vDriverVersion);
            pxAudioDevice->Channels = waveOutCapA.wChannels;

            PXTextCopyA(waveOutCapA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    {
        const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

        PXActionReturnOnError(audioResult);
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "WinMM",
        "Device-Fetch",
        "%s",
        pxAudioDevice->DeviceName
    );

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
    for (PXSize i = 0; i < amount; ++i)
    {
        const PXActionResult fetchResult = PXMultiMediaDeviceFetch(pxAudioMultiMedia, pxAudioDeviceType, i, &pxAudioDevice[i]);

        PXActionReturnOnError(fetchResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    DWORD_PTR dwCallback = 0;
    DWORD_PTR dwInstance = 0;
    DWORD fdwOpen = CALLBACK_NULL;

    WAVEFORMATEX waveFormat;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;// pxAudioSource->F;         /* format type */
    waveFormat.nChannels = pxAudioDevice->Channels;          /* number of channels (i.e. mono, stereo...) */
    waveFormat.nSamplesPerSec = pxAudioDevice->SamplesPerSecound;     /* sample rate */
    waveFormat.nAvgBytesPerSec = pxAudioDevice->AverageBytesPerSecound;    /* for buffer estimation */
    waveFormat.nBlockAlign = pxAudioDevice->BlockAlignSize;        /* block size of data */
    waveFormat.wBitsPerSample = pxAudioDevice->BitsPerSample;     /* number of bits per sample of mono data */
    waveFormat.cbSize = sizeof(WAVEFORMATEX);



    waveFormat.nChannels = 2;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.nAvgBytesPerSec = 44100;
    waveFormat.nBlockAlign = 4;
    waveFormat.wBitsPerSample = 8;






    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            const PXwaveInOpen pxWaveInOpen = (PXwaveInOpen)pxAudioMultiMedia->WaveInOpen;
            HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveInOpen
            (
                &handleWaveIn,
                pxAudioDevice->DeviceID,
                &waveFormat,
                dwCallback,
                dwInstance,
                fdwOpen
            );

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutOpen pxWaveOutOpen = (PXwaveOutOpen)pxAudioMultiMedia->WaveOutOpen;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutOpen
            (
                &handleWaveOut,
                pxAudioDevice->DeviceID,
                &waveFormat,
                dwCallback,
                dwInstance,
                fdwOpen
            );

            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    pxAudioDevice->Type = pxAudioDeviceType;

    PXLogPrint
    (
        PXLoggingInfo,
        "WinMM",
        "Device-Open",
        "%s",
        pxAudioDevice->DeviceName
    );

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            PXwaveInClose pxWaveInClose = (PXwaveInClose)pxAudioMultiMedia->WaveInClose;
            HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveInClose(handleWaveIn);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXwaveOutClose pxWaveOutClose = (PXwaveOutClose)pxAudioMultiMedia->WaveOutClose;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutClose(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    pxAudioDevice->Info.DirectXInterface = PXNull;

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

    //pxAudioDevice->FormatTag = pxSound->mod

    pxAudioDevice->Channels = pxSound->NumerOfChannels;
    pxAudioDevice->SamplesPerSecound = pxSound->SampleRate;
    pxAudioDevice->AverageBytesPerSecound = pxSound->SampleRate / pxSound->DataSize;
    pxAudioDevice->BlockAlignSize = pxSound->BlockAllign;
    pxAudioDevice->BitsPerSample = pxSound->BitsPerSample;

    MMRESULT result;

    WAVEHDR waveHeader;
    const UINT waveHeaderSize = sizeof(waveHeader);
    waveHeader.lpData = (LPSTR)pxSound->Data;
    waveHeader.dwBufferLength = pxSound->DataSize;
    waveHeader.dwBytesRecorded = 0;
    waveHeader.dwUser = 0;
    waveHeader.dwFlags = 0;
    waveHeader.dwLoops = 0;
    waveHeader.lpNext = 0;
    waveHeader.reserved = 0;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            const HWAVEIN handleWaveOut = (HWAVEIN)pxAudioDevice->Info.DirectXInterface;

            // Prepare
            {
                const PXwaveInPrepareHeader pxWaveInPrepareHeader = (PXwaveInPrepareHeader)pxAudioMultiMedia->WaveInPrepareHeader;
                const MMRESULT prepareResultID = pxWaveInPrepareHeader(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXActionResult prepareResult = PXWindowsMMAudioConvertFromID(prepareResultID);

                PXActionReturnOnError(prepareResult);
            }

            // Write Data
            {
                return PXActionInvalid;
            }

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            // Prepare
            {
                const PXwaveOutPrepareHeader pxWaveOutPrepareHeader = (PXwaveOutPrepareHeader)pxAudioMultiMedia->WaveOutPrepareHeader;
                const MMRESULT prepareResultID = pxWaveOutPrepareHeader(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXActionResult prepareResult = PXWindowsMMAudioConvertFromID(prepareResultID);

                PXActionReturnOnError(prepareResult);
            }

            // Write Data
            {
                const PXwaveOutWrite pxWaveOutWrite = (PXwaveOutWrite)pxAudioMultiMedia->WaveOutWrite;
                const MMRESULT writeResultID = pxWaveOutWrite(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXActionResult writeResult = PXWindowsMMAudioConvertFromID(writeResultID);

                PXActionReturnOnError(writeResult);
            }
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceBreakLoop(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutBreakLoop pxWaveOutBreakLoop = (PXwaveOutBreakLoop)pxAudioMultiMedia->WaveOutBreakLoop;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutBreakLoop(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePositionGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            MMTIME time;
            UINT xx = 0;

            const PXwaveOutGetPosition pxWaveOutGetPosition = (PXwaveOutGetPosition)pxAudioMultiMedia->WaveOutGetPosition;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutGetPosition(handleWaveOut, &time, xx);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePlaybackRateGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutGetPlaybackRate pxWaveOutGetPlaybackRate = (PXwaveOutGetPlaybackRate)pxAudioMultiMedia->WaveOutGetPlaybackRate;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;
            
            result = pxWaveOutGetPlaybackRate(handleWaveOut, playbackRate);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePlaybackRateSet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch (pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutSetPlaybackRate pxWaveOutSetPlaybackRate = (PXwaveOutSetPlaybackRate)pxAudioMultiMedia->WaveOutSetPlaybackRate;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            result = pxWaveOutSetPlaybackRate(handleWaveOut, playbackRate);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(result);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePitchIncrease(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, float amount)
{
    //assert(amount > 0.0f);

    pxAudioDevice->Pitch += amount;

    if (pxAudioDevice->Pitch > PitchMaximum)
    {
        pxAudioDevice->Pitch = PitchMaximum;
    }
}

PXActionResult PXAPI PXMultiMediaDevicePitchSet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop
    const PXwaveOutSetPitch pxWaveOutSetPitch = (PXwaveOutSetPitch)pxAudioMultiMedia->WaveOutSetPitch;
    const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

    const MMRESULT pitchResultID = pxWaveOutSetPitch(handleWaveOut, pitch);
    const PXActionResult pitchResult = PXWindowsMMAudioConvertFromID(pitchResultID);

    return pitchResult;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePitchReduce(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, float amount)
{
    //assert(amount > 0.0f);

    pxAudioDevice->Pitch -= amount;

    if (pxAudioDevice->Pitch < PitchMinimum)
    {
        pxAudioDevice->Pitch = PitchMinimum;
    }
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    DWORD volumeDW = 0;

    const PXwaveOutGetVolume pxWaveOutGetVolume = (PXwaveOutGetVolume)pxAudioMultiMedia->WaveOutGetVolume;
    const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;
    const MMRESULT volumeResultID = pxWaveOutGetVolume(handleWaveOut, &volumeDW);
    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(volumeResultID);
    const PXBool successful = PXActionSuccessful == audioResult;

    if (!successful)
    {
        *volume = -1;
        return audioResult;
    }

    *volume = volumeDW;
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeSetEqual(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT volumeResultID;

    switch (pxAudioDevice->Type)
    {
        //case PXAudioDeviceTypeInput:
        //    volumeResultID = waveInSetVolume((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, volume);
        //    break;

        case PXAudioDeviceTypeOutput:
        {
            const PXwaveOutSetVolume pxWaveOutSetVolume = (PXwaveOutSetVolume)pxAudioMultiMedia->WaveOutSetVolume;
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.DirectXInterface;

            volumeResultID = pxWaveOutSetVolume(handleWaveOut, volume);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }
    
    const PXActionResult audioResult = PXWindowsMMAudioConvertFromID(volumeResultID);

    return audioResult;
#endif
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeSetIndividual(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    const unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
    const PXActionResult audioResult = PXMultiMediaDeviceVolumeSetEqual(pxAudioMultiMedia, pxAudioDevice, volumeCombined);

    return audioResult;
#endif
}