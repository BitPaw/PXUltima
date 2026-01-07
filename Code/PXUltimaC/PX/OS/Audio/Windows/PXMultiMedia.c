#include "PXMultiMedia.h"

#if OSWindows

#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif
#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia

#endif

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>


const char PXMultimediaName[] = "WinMM";
const char PXMultimediaLibraryName[] = "WINMM.DLL";
const char PXMultimediaLibraryNameLength = sizeof(PXMultimediaLibraryName);


#if OSWindows


typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsA)(UINT_PTR uDeviceID, LPWAVEOUTCAPSA pwoc, UINT cbwoc);
typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsW)(UINT_PTR uDeviceID, LPWAVEOUTCAPSW pwoc, UINT cbwoc);
typedef UINT(WINAPI* PXwaveOutGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXwaveOutGetVolume)(HWAVEOUT hwo, LPDWORD pdwVolume);
typedef MMRESULT(WINAPI* PXwaveOutSetVolume)(HWAVEOUT hwo, DWORD dwVolume);
typedef MMRESULT(WINAPI* PXwaveOutGetErrorA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveOutGetErrorW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
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
typedef MMRESULT(WINAPI* PXwaveInGetErrorA)(MMRESULT mmrError, LPSTR pszText, UINT cchText);
typedef MMRESULT(WINAPI* PXwaveInGetErrorW)(MMRESULT mmrError, LPWSTR pszText, UINT cchText);
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



const char PXWindowsMMText[] = "WIN-MM";
PXAudioMultiMedia _pxAudioMultiMedia;

PXResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia* PXREF pxAudioMultiMedia, PXAudioInitializeInfo PXREF pxAudioInitializeInfo)
{
    PXClear(PXAudioMultiMedia, &_pxAudioMultiMedia);

    if(pxAudioMultiMedia)
    {
        *pxAudioMultiMedia = &_pxAudioMultiMedia;
    }

#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowsMMText,
        "Initialize",
        "..."
    );
#endif

    // Library
    {
        PXText pxText;
        PXTextFromAdressA(&pxText, PXMultimediaLibraryName, PXMultimediaLibraryNameLength, PXMultimediaLibraryNameLength);

        const PXResult loadLibResult = PXLibraryOpen(&_pxAudioMultiMedia.MultiMediaLibrary, &pxText);

        if(PXActionSuccessful != loadLibResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    // Fetch
    {
        // TODO: make binding from this!
        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &_pxAudioMultiMedia.WaveOutGetDevCapsA, "waveOutGetDevCapsA"},
            { &_pxAudioMultiMedia.WaveOutGetDevCapsW,"waveOutGetDevCapsW"},
            { &_pxAudioMultiMedia.WaveOutGetNumDevs, "waveOutGetNumDevs"},
            { &_pxAudioMultiMedia.WaveOutGetVolume,"waveOutGetVolume"},
            { &_pxAudioMultiMedia.WaveOutSetVolume,"waveOutSetVolume"},
            { &_pxAudioMultiMedia.WaveOutGetErrorA,"waveOutGetErrorA"},
            { &_pxAudioMultiMedia.WaveOutGetErrorW,"waveOutGetErrorW"},
            { &_pxAudioMultiMedia.WaveOutOpen,"waveOutOpen"},
            { &_pxAudioMultiMedia.WaveOutClose,"waveOutClose"},
            { &_pxAudioMultiMedia.WaveOutPrepareHeader,"waveOutPrepareHeader"},
            { &_pxAudioMultiMedia.WaveOutUnprepareHeader,"waveOutUnprepareHeader"},
            { &_pxAudioMultiMedia.WaveOutWrite, "waveOutWrite"},
            { &_pxAudioMultiMedia.WaveOutPause,"waveOutPause" },
            { &_pxAudioMultiMedia.WaveOutRestart,"waveOutRestart" },
            { &_pxAudioMultiMedia.WaveOutReset,"waveOutReset" },
            { &_pxAudioMultiMedia.WaveOutBreakLoop,"waveOutBreakLoop" },
            { &_pxAudioMultiMedia.WaveOutGetPosition,"waveOutGetPosition" },
            { &_pxAudioMultiMedia.WaveOutGetPitch,"waveOutGetPitch" },
            { &_pxAudioMultiMedia.WaveOutSetPitch,"waveOutSetPitch" },
            { &_pxAudioMultiMedia.WaveOutGetPlaybackRate,"waveOutGetPlaybackRate" },
            { &_pxAudioMultiMedia.WaveOutSetPlaybackRate,"waveOutSetPlaybackRate" },
            { &_pxAudioMultiMedia.WaveOutGetID,"waveOutGetID" },
            { &_pxAudioMultiMedia.WaveOutMessage, "waveOutMessage" },
            { &_pxAudioMultiMedia.WaveInGetNumDevs, "waveInGetNumDevs" },
            { &_pxAudioMultiMedia.WaveInGetDevCapsA, "waveInGetDevCapsA" },
            { &_pxAudioMultiMedia.WaveInGetDevCapsW, "waveInGetDevCapsW" },
            { &_pxAudioMultiMedia.WaveInGetErrorA, "waveInGetErrorA" },
            { &_pxAudioMultiMedia.WaveInGetErrorW,    "waveInGetErrorW" },
            { &_pxAudioMultiMedia.WaveInOpen,    "waveInOpen" },
            { &_pxAudioMultiMedia.WaveInClose,    "waveInClose" },
            { &_pxAudioMultiMedia.WaveInPrepareHeader,    "waveInPrepareHeader" },
            { &_pxAudioMultiMedia.WaveInUnprepareHeader,    "waveInUnprepareHeader" },
            { &_pxAudioMultiMedia.WaveInAddBuffer,    "waveInAddBuffer" },
            { &_pxAudioMultiMedia.WaveInStart,    "waveInStart" },
            { &_pxAudioMultiMedia.WaveInStop,    "waveInStop" },
            { &_pxAudioMultiMedia.WaveInReset,    "waveInReset" },
            { &_pxAudioMultiMedia.WaveInGetPosition,    "waveInGetPosition" },
            { &_pxAudioMultiMedia.WaveInGetID,    "waveInGetID" },
            { &_pxAudioMultiMedia.WaveInMessage,    "waveInMessage" },
            { &_pxAudioMultiMedia.MidiOutGetNumDevs,    "midiOutGetNumDevs" },
            { &_pxAudioMultiMedia.MidiStreamOpen,"midiStreamOpen" },
            { &_pxAudioMultiMedia.MidiStreamClose,"midiStreamClose" },
            { &_pxAudioMultiMedia.MidiStreamProperty,"midiStreamProperty" },
            { &_pxAudioMultiMedia.MidiStreamPosition,"midiStreamPosition" },
            { &_pxAudioMultiMedia.MidiStreamOut,"midiStreamOut" },
            { &_pxAudioMultiMedia.MidiStreamPause,"midiStreamPause" },
            { &_pxAudioMultiMedia.MidiStreamRestart,"midiStreamRestart" },
            { &_pxAudioMultiMedia.MidiStreamStop,"midiStreamStop" },
            { &_pxAudioMultiMedia.MidiConnect,"midiConnect" },
            { &_pxAudioMultiMedia.MidiDisconnect,"midiDisconnect" },
            { &_pxAudioMultiMedia.MidiOutGetDevCapsA,"midiOutGetDevCapsA" },
            { &_pxAudioMultiMedia.MidiOutGetDevCapsW,"midiOutGetDevCapsW" },
            { &_pxAudioMultiMedia.MidiOutGetVolume,"midiOutGetVolume" },
            { &_pxAudioMultiMedia.MidiOutSetVolume,"midiOutSetVolume" },
            { &_pxAudioMultiMedia.MidiOutGetErrorA,"midiOutGetErrorA" },
            { &_pxAudioMultiMedia.MidiOutGetErrorW,"midiOutGetErrorW" },
            { &_pxAudioMultiMedia.MidiOutOpen,"midiOutOpen" },
            { &_pxAudioMultiMedia.MidiOutClose,"midiOutClose" },
            { &_pxAudioMultiMedia.MidiOutPrepareHeader,"midiOutPrepareHeader" },
            { &_pxAudioMultiMedia.MidiOutUnprepareHeader,"midiOutUnprepareHeader" },
            { &_pxAudioMultiMedia.MidiOutShortMsg,"midiOutShortMsg" },
            { &_pxAudioMultiMedia.MidiOutLongMsg,"midiOutLongMsg" },
            { &_pxAudioMultiMedia.MidiOutReset,"midiOutReset" },
            { &_pxAudioMultiMedia.MidiOutCachePatches,"midiOutCachePatches" },
            { &_pxAudioMultiMedia.MidiOutCacheDrumPatches,"midiOutCacheDrumPatches" },
            { &_pxAudioMultiMedia.MidiOutGetID,"midiOutGetID" },
            { &_pxAudioMultiMedia.MidiOutMessage,"midiOutMessage" },
            { &_pxAudioMultiMedia.MidiInGetNumDevs,"midiInGetNumDevs" },
            { &_pxAudioMultiMedia.MidiInGetDevCapsA,"midiInGetDevCapsA" },
            { &_pxAudioMultiMedia.MidiInGetDevCapsW,"midiInGetDevCapsW" },
            { &_pxAudioMultiMedia.MidiInGetErrorA,"midiInGetErrorA" },
            { &_pxAudioMultiMedia.MidiInGetErrorW,"midiInGetErrorW" },
            { &_pxAudioMultiMedia.MidiInOpen,"midiInOpen" },
            { &_pxAudioMultiMedia.MidiInClose,"midiInClose" },
            { &_pxAudioMultiMedia.MidiInPrepareHeader,"midiInPrepareHeader" },
            { &_pxAudioMultiMedia.MidiInUnprepareHeader,"midiInUnprepareHeader" },
            { &_pxAudioMultiMedia.MidiInAddBuffer,"midiInAddBuffer" },
            { &_pxAudioMultiMedia.MidiInStart,"midiInStart" },
            { &_pxAudioMultiMedia.MidiInStop,"midiInStop" },
            { &_pxAudioMultiMedia.MidiInReset,"midiInReset" },
            { &_pxAudioMultiMedia.MidiInGetID,"midiInGetID" },
            { &_pxAudioMultiMedia.MidiInMessage,"midiInMessage" },
            { &_pxAudioMultiMedia.AuxGetNumDevs,"auxGetNumDevs" },
            { &_pxAudioMultiMedia.AuxGetDevCapsA,"auxGetDevCapsA" },
            { &_pxAudioMultiMedia.AuxGetDevCapsW,"auxGetDevCapsW" },
            { &_pxAudioMultiMedia.AuxSetVolume,"auxSetVolume" },
            { &_pxAudioMultiMedia.AuxGetVolume,"auxGetVolume" },
            { &_pxAudioMultiMedia.AuxOutMessage,"auxOutMessage" },
            { &_pxAudioMultiMedia.MixerGetNumDevs,"mixerGetNumDevs" },
            { &_pxAudioMultiMedia.MixerGetDevCapsA,"mixerGetDevCapsA" },
            { &_pxAudioMultiMedia.MixerGetDevCapsW,"mixerGetDevCapsW" },
            { &_pxAudioMultiMedia.MixerOpen,"mixerOpen" },
            { &_pxAudioMultiMedia.MixerClose,"mixerClose" },
            { &_pxAudioMultiMedia.MixerMessage,"mixerMessage" },
            { &_pxAudioMultiMedia.MixerGetLineInfoA,"mixerGetLineInfoA" },
            { &_pxAudioMultiMedia.MixerGetLineInfoW,"mixerGetLineInfoW" },
            { &_pxAudioMultiMedia.MixerGetID,"mixerGetID" },
            { &_pxAudioMultiMedia.MixerGetLineControlsA,"mixerGetLineControlsA" },
            { &_pxAudioMultiMedia.MixerGetLineControlsW,"mixerGetLineControlsW" },
            { &_pxAudioMultiMedia.MixerGetControlDetailsA,"mixerGetControlDetailsA" },
            { &_pxAudioMultiMedia.MixerGetControlDetailsW,"mixerGetControlDetailsW" }
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_pxAudioMultiMedia.MultiMediaLibrary, pxLibraryFuntionEntryList, amount);
    }

    // Link
    {
        PXAudio PXREF pxAudio = pxAudioInitializeInfo->AudioReference;
        pxAudio->DeviceProperty = PXMultiMediaDeviceProperty;

        pxAudio->DeviceFetch = PXMultiMediaDeviceFetch;
        pxAudio->DeviceFetchAll = PXMultiMediaDeviceFetchAll;
        pxAudio->DeviceOpen = PXMultiMediaDeviceOpen;
        pxAudio->DeviceClose = PXMultiMediaDeviceClose;
        pxAudio->DeviceLoad = PXMultiMediaDeviceLoad;
     //   pxAudio->DeviceBreakLoop = PXMultiMediaDeviceBreakLoop;
      //  pxAudio->DeviceStart = PXMultiMediaDeviceStart;
       // pxAudio->DeviceStop = PXMultiMediaDeviceStop;
      //  pxAudio->DevicePause = PXMultiMediaDevicePause;
       // pxAudio->DeviceRestart = PXMultiMediaDeviceRestart;
       // pxAudio->DeviceReset = PXMultiMediaDeviceReset;
    }

    return PXActionSuccessful;
#endif
}




PXResult PXAPI PXMultiMediaDeviceProperty(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty)
{
    if(!(pxAudioDevice && pxSoundDeviceProperty))
    {
        return PXActionRefusedArgumentNull;
    }

    const PXBool isWriteOP = 0 < (PXSoundDevicePropertyWrite & pxSoundDeviceProperty->Flags);

#if OSUnix
#elif OSWindows
    const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->WaveOutHandle;
    const HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->WaveInHandle;
    MMRESULT resultID = 0;
#endif

    switch(pxSoundDeviceProperty->Type)
    {
        case PXSoundDeviceAmount:
        {
            if(isWriteOP)
            {
#if OSUnix
                return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
              
                const PXwaveOutGetNumDevs pxWaveOutGetNumDevs = (PXwaveOutGetNumDevs)_pxAudioMultiMedia.WaveOutGetNumDevs;

                pxSoundDeviceProperty->Value = pxWaveOutGetNumDevs();
#endif
            }
            else
            {
#if OSUnix
                return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
                const PXwaveInGetNumDevs pxWaveInGetNumDevs = (PXwaveInGetNumDevs)_pxAudioMultiMedia.WaveInGetNumDevs;               

                pxSoundDeviceProperty->Value = pxWaveInGetNumDevs();
#endif
            }

            break;
        }
        case PXSoundDevicePropertyStatePlay:
        {        
            switch(pxAudioDevice->Type)
            {
                case PXAudioDeviceTypeInput:
                {
                    PXwaveInStart pxWaveInStart = (PXwaveInStart)_pxAudioMultiMedia.WaveInStart;
                    resultID = pxWaveInStart(handleWaveIn);
                    break;
                }
                case PXAudioDeviceTypeOutput:
                {
                    PXwaveOutRestart pxwaveOutRestart = (PXwaveOutRestart)_pxAudioMultiMedia.WaveOutRestart;
                    resultID = pxwaveOutRestart(handleWaveOut);
                    break;
                }
                default:
                    return PXActionRefusedArgumentInvalid;
            }

            break;
        }
        case PXSoundDevicePropertyStatePause:
        {
#if OSUnix
            return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

            switch(pxAudioDevice->Type)
            {
                case PXAudioDeviceTypeInput:
                {
                    PXwaveInStop pxwaveInStop = (PXwaveInStop)_pxAudioMultiMedia.WaveInStop;
                    resultID = pxwaveInStop(handleWaveIn);
                    break;
                }
                case PXAudioDeviceTypeOutput:
                {
                    PXwaveOutPause pxwaveOutPause = (PXwaveOutPause)_pxAudioMultiMedia.WaveOutPause;
                    resultID = pxwaveOutPause(handleWaveOut);
                    break;
                }

                default:
                    return PXActionRefusedArgumentInvalid;
            }
#endif       
        
            break;
        }
        case PXSoundDevicePropertyStateReset:
        {
            switch(pxAudioDevice->Type)
            {
                case PXAudioDeviceTypeInput:
                {
                    const PXwaveInReset pxWaveInReset = (PXwaveInReset)_pxAudioMultiMedia.WaveInReset;
                    resultID = pxWaveInReset(handleWaveIn);
                    break;
                }
                case PXAudioDeviceTypeOutput:
                {
                    const PXwaveOutReset pxWaveOutReset = (PXwaveOutReset)_pxAudioMultiMedia.WaveOutReset;
                    resultID = pxWaveOutReset(handleWaveOut);
                    break;
                }

                default:
                    return PXActionRefusedArgumentInvalid;
            }

            break;
        }
        case PXSoundDevicePropertyStateRestart:
        {           
            switch(pxAudioDevice->Type)
            {
                //case PXAudioDeviceTypeInput:
                //{
                //    result = waveInRestart((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
                //    break;
                //}
                case PXAudioDeviceTypeOutput:
                {
                    const PXwaveOutRestart pxWaveOutRestart = (PXwaveOutRestart)_pxAudioMultiMedia.WaveOutRestart;
                    resultID = pxWaveOutRestart(handleWaveOut);
                    break;
                }

                default:
                    return PXActionRefusedArgumentInvalid;
            }


            /*
            const PXwaveOutBreakLoop pxWaveOutBreakLoop = (PXwaveOutBreakLoop)_pxAudioMultiMedia.WaveOutBreakLoop;
                    const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->Info.Handle.DirectXInterface;

                    resultID = pxWaveOutBreakLoop(handleWaveOut);
            */

            break;
        }
        case PXSoundDevicePropertyPositionCursor:
        {
#if OSUnix
            resultID = PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
            switch(pxAudioDevice->Type)
            {
                case PXAudioDeviceTypeInput:
                {

                    break;
                }
                case PXAudioDeviceTypeOutput:
                {
                    MMTIME time;
                    UINT xx = 0;

                    const PXwaveOutGetPosition pxWaveOutGetPosition = (PXwaveOutGetPosition)_pxAudioMultiMedia.WaveOutGetPosition;

                    resultID = pxWaveOutGetPosition(handleWaveOut, &time, xx);
                    break;
                }

                default:
                    return PXActionRefusedArgumentInvalid;
            }
#endif

            break;
        }
        case PXSoundDevicePropertySpeed:
        {           
            switch(pxAudioDevice->Type)
            {
                case PXAudioDeviceTypeInput:
                {

                    break;
                }
                case PXAudioDeviceTypeOutput:
                {
                    if(isWriteOP)
                    {
                        const PXwaveOutSetPlaybackRate pxWaveOutSetPlaybackRate = (PXwaveOutSetPlaybackRate)_pxAudioMultiMedia.WaveOutSetPlaybackRate;

                        resultID = pxWaveOutSetPlaybackRate(handleWaveOut, pxSoundDeviceProperty->Value);
                    }
                    else
                    {
                        const PXwaveOutGetPlaybackRate pxWaveOutGetPlaybackRate = (PXwaveOutGetPlaybackRate)_pxAudioMultiMedia.WaveOutGetPlaybackRate;

                        DWORD value;

                        resultID = pxWaveOutGetPlaybackRate(handleWaveOut, &value);

                        pxSoundDeviceProperty->Value = value;
                    }

                    break;
                }

                default:
                    return PXActionRefusedArgumentInvalid;
            }


            break;
        }
        case PXSoundDevicePropertyPositionSpace:
        {
          

            break;
        }
        case PXSoundDevicePropertyAngleCone:
        {


            break;
        }
        case PXSoundDevicePropertyVelocity:
        {
           
        }
        case PXSoundDevicePropertyPitch:
        {
            const PXwaveOutSetPitch pxWaveOutSetPitch = (PXwaveOutSetPitch)_pxAudioMultiMedia.WaveOutSetPitch;

            resultID = pxWaveOutSetPitch(handleWaveOut, pxSoundDeviceProperty->Value);

            break;
        }
        case PXSoundDevicePropertyVolume:
        {
            if(isWriteOP)
            {
                if(1) // Same value for both
                {
                    const PXwaveOutSetVolume pxWaveOutSetVolume = (PXwaveOutSetVolume)_pxAudioMultiMedia.WaveOutSetVolume;

                    resultID = pxWaveOutSetVolume(handleWaveOut, pxSoundDeviceProperty->Value);
                }
                else
                {
                   // const unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
                   // const PXResult audioResult = PXMultiMediaDeviceVolumeSetEqual(pxAudioMultiMedia, pxAudioDevice, volumeCombined);
                } 
            }
            else
            {
                DWORD volumeDW = 0;

                const PXwaveOutGetVolume pxWaveOutGetVolume = (PXwaveOutGetVolume)_pxAudioMultiMedia.WaveOutGetVolume;
                resultID = pxWaveOutGetVolume(handleWaveOut, &volumeDW);

                pxSoundDeviceProperty->Value = volumeDW;
            }           

            break;
        }
        default:
        {
            return PXActionRefusedArgumentInvalid;
        }
    }

#if OSUnix
#elif OSWindows
    const PXResult audioResult = PXWindowsMMAudioConvertFromID(resultID);
    const PXBool successful = PXActionSuccessful == audioResult;

    if(PXActionSuccessful != audioResult)
        return audioResult;
#endif


    return PXActionSuccessful;
}




PXResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXClear(PXAudioDevice, pxAudioDevice);

    MMRESULT result;

    pxAudioDevice->Type = pxAudioDeviceType;

    switch(pxAudioDeviceType)
    {
        case PXAudioDeviceTypeInput:
        {
            const UINT size = sizeof(WAVEINCAPS);
            WAVEINCAPS waveInCapsA;

            PXwaveInGetDevCapsA pxWaveInGetDevCapsA = (PXwaveInGetDevCapsA)_pxAudioMultiMedia.WaveInGetDevCapsA;

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

            PXwaveOutGetDevCapsA pxwaveOutGetDevCapsA = (PXwaveOutGetDevCapsA)_pxAudioMultiMedia.WaveOutGetDevCapsA;

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
        const PXResult audioResult = PXWindowsMMAudioConvertFromID(result);

        if(PXActionSuccessful != audioResult) 
            return audioResult;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowsMMText,
        "Device-Fetch",
        "%s",
        pxAudioDevice->DeviceName
    );

#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        const PXResult fetchResult = PXMultiMediaDeviceFetch(pxAudioMultiMedia, pxAudioDeviceType, i, &pxAudioDevice[i]);

        if(PXActionSuccessful != fetchResult) 
            return fetchResult;;
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID)
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

    switch(pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            const PXwaveInOpen pxWaveInOpen = (PXwaveInOpen)_pxAudioMultiMedia.WaveInOpen;
            HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->WaveInHandle;

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
            const PXwaveOutOpen pxWaveOutOpen = (PXwaveOutOpen)_pxAudioMultiMedia.WaveOutOpen;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->WaveOutHandle;

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

    const PXResult audioResult = PXWindowsMMAudioConvertFromID(result);

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

PXResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    MMRESULT result;

    switch(pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            PXwaveInClose pxWaveInClose = (PXwaveInClose)_pxAudioMultiMedia.WaveInClose;
            HWAVEIN handleWaveIn = (HWAVEIN)pxAudioDevice->WaveInHandle;

            result = pxWaveInClose(handleWaveIn);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXwaveOutClose pxWaveOutClose = (PXwaveOutClose)_pxAudioMultiMedia.WaveOutClose;
            HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->WaveOutHandle;

            result = pxWaveOutClose(handleWaveOut);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXResult audioResult = PXWindowsMMAudioConvertFromID(result);

    pxAudioDevice->GenericHandle = PXNull;

    return audioResult;
#endif
}

PXResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, PXSound PXREF pxSound)
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

    switch(pxAudioDevice->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            const HWAVEIN handleWaveOut = (HWAVEIN)pxAudioDevice->WaveInHandle;

            // Prepare
            {
                const PXwaveInPrepareHeader pxWaveInPrepareHeader = (PXwaveInPrepareHeader)_pxAudioMultiMedia.WaveInPrepareHeader;
                const MMRESULT prepareResultID = pxWaveInPrepareHeader(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXResult prepareResult = PXWindowsMMAudioConvertFromID(prepareResultID);

                if(PXActionSuccessful != prepareResult) 
                    return prepareResult;
            }

            // Write Data
            {
                return PXActionInvalid;
            }

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            const HWAVEOUT handleWaveOut = (HWAVEOUT)pxAudioDevice->WaveOutHandle;

            // Prepare
            {
                const PXwaveOutPrepareHeader pxWaveOutPrepareHeader = (PXwaveOutPrepareHeader)_pxAudioMultiMedia.WaveOutPrepareHeader;
                const MMRESULT prepareResultID = pxWaveOutPrepareHeader(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXResult prepareResult = PXWindowsMMAudioConvertFromID(prepareResultID);

                if(PXActionSuccessful != prepareResult) 
                    return prepareResult;
            }

            // Write Data
            {
                const PXwaveOutWrite pxWaveOutWrite = (PXwaveOutWrite)_pxAudioMultiMedia.WaveOutWrite;
                const MMRESULT writeResultID = pxWaveOutWrite(handleWaveOut, &waveHeader, waveHeaderSize);
                const PXResult writeResult = PXWindowsMMAudioConvertFromID(writeResultID);

                if(PXActionSuccessful != writeResult) 
                    return writeResult;
            }
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }
#endif

    return PXActionSuccessful;
}