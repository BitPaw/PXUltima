#ifndef PXAudioMIDIInclude
#define PXAudioMIDIInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

#include <Windows.h>
#include <mmsystem.h>

#ifdef __cplusplus
extern "C"
{
#endif

	
	typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsA)( UINT_PTR uDeviceID,  LPWAVEOUTCAPSA pwoc,  UINT cbwoc);
	typedef MMRESULT(WINAPI* PXwaveOutGetDevCapsW)( UINT_PTR uDeviceID,  LPWAVEOUTCAPSW pwoc,  UINT cbwoc);
	typedef UINT(WINAPI* PXwaveOutGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXwaveOutGetVolume)( HWAVEOUT hwo,  LPDWORD pdwVolume);
	typedef MMRESULT (WINAPI* PXwaveOutSetVolume)( HWAVEOUT hwo,  DWORD dwVolume);
	typedef MMRESULT (WINAPI* PXwaveOutGetErrorTextA)( MMRESULT mmrError, LPSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXwaveOutGetErrorTextW)( MMRESULT mmrError, LPWSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXwaveOutOpen)( LPHWAVEOUT phwo,  UINT uDeviceID,  LPCWAVEFORMATEX pwfx,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXwaveOutClose)( HWAVEOUT hwo);
	typedef MMRESULT (WINAPI* PXwaveOutPrepareHeader)( HWAVEOUT hwo,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveOutUnprepareHeader)( HWAVEOUT hwo,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveOutWrite)( HWAVEOUT hwo,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveOutPause)( HWAVEOUT hwo);
	typedef MMRESULT (WINAPI* PXwaveOutRestart)( HWAVEOUT hwo);
	typedef MMRESULT (WINAPI* PXwaveOutReset)( HWAVEOUT hwo);
	typedef MMRESULT (WINAPI* PXwaveOutBreakLoop)( HWAVEOUT hwo);
	typedef MMRESULT (WINAPI* PXwaveOutGetPosition)( HWAVEOUT hwo, LPMMTIME pmmt,  UINT cbmmt);
	typedef MMRESULT (WINAPI* PXwaveOutGetPitch)( HWAVEOUT hwo,  LPDWORD pdwPitch);
	typedef MMRESULT (WINAPI* PXwaveOutSetPitch)( HWAVEOUT hwo,  DWORD dwPitch);
	typedef MMRESULT (WINAPI* PXwaveOutGetPlaybackRate)( HWAVEOUT hwo,  LPDWORD pdwRate);
	typedef MMRESULT (WINAPI* PXwaveOutSetPlaybackRate)( HWAVEOUT hwo,  DWORD dwRate);
	typedef MMRESULT (WINAPI* PXwaveOutGetID)( HWAVEOUT hwo,  LPUINT puDeviceID);
	typedef MMRESULT (WINAPI* PXwaveOutMessage)( HWAVEOUT hwo,  UINT uMsg,  DWORD_PTR dw1,  DWORD_PTR dw2);
	typedef UINT (WINAPI* PXwaveInGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXwaveInGetDevCapsA)( UINT_PTR uDeviceID,  LPWAVEINCAPSA pwic,  UINT cbwic);
	typedef MMRESULT (WINAPI* PXwaveInGetDevCapsW)( UINT_PTR uDeviceID, LPWAVEINCAPSW pwic,  UINT cbwic);
	typedef MMRESULT (WINAPI* PXwaveInGetErrorTextA)( MMRESULT mmrError,  LPSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXwaveInGetErrorTextW)( MMRESULT mmrError,  LPWSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXwaveInOpen)( LPHWAVEIN phwi,  UINT uDeviceID,  LPCWAVEFORMATEX pwfx,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXwaveInClose)( HWAVEIN hwi);
	typedef MMRESULT (WINAPI* PXwaveInPrepareHeader)( HWAVEIN hwi,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveInUnprepareHeader)( HWAVEIN hwi,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveInAddBuffer)( HWAVEIN hwi,  LPWAVEHDR pwh,  UINT cbwh);
	typedef MMRESULT (WINAPI* PXwaveInStart)( HWAVEIN hwi);
	typedef MMRESULT (WINAPI* PXwaveInStop)( HWAVEIN hwi);
	typedef MMRESULT (WINAPI* PXwaveInReset)( HWAVEIN hwi);
	typedef MMRESULT (WINAPI* PXwaveInGetPosition)( HWAVEIN hwi, LPMMTIME pmmt,  UINT cbmmt);
	typedef MMRESULT (WINAPI* PXwaveInGetID)( HWAVEIN hwi,  LPUINT puDeviceID);
	typedef MMRESULT (WINAPI* PXwaveInMessage)( HWAVEIN hwi,  UINT uMsg,  DWORD_PTR dw1,  DWORD_PTR dw2);
	typedef UINT (WINAPI* PXmidiOutGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXmidiStreamOpen)( LPHMIDISTRM phms,  LPUINT puDeviceID,  DWORD cMidi,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXmidiStreamClose)( HMIDISTRM hms);
	typedef MMRESULT (WINAPI* PXmidiStreamProperty)( HMIDISTRM hms, LPBYTE lppropdata,  DWORD dwProperty	);
	typedef MMRESULT (WINAPI* PXmidiStreamPosition)( HMIDISTRM hms,LPMMTIME lpmmt,  UINT cbmmt);
	typedef MMRESULT (WINAPI* PXmidiStreamOut)( HMIDISTRM hms,  LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiStreamPause)( HMIDISTRM hms);
	typedef MMRESULT (WINAPI* PXmidiStreamRestart)( HMIDISTRM hms);
	typedef MMRESULT (WINAPI* PXmidiStreamStop)( HMIDISTRM hms);
	typedef MMRESULT (WINAPI* PXmidiConnect)( HMIDI hmi,  HMIDIOUT hmo,  LPVOID pReserved);
	typedef MMRESULT (WINAPI* PXmidiDisconnect)( HMIDI hmi,  HMIDIOUT hmo,  LPVOID pReserved);
	typedef MMRESULT (WINAPI* PXmidiOutGetDevCapsA)( UINT_PTR uDeviceID,  LPMIDIOUTCAPSA pmoc,  UINT cbmoc);
	typedef MMRESULT (WINAPI* PXmidiOutGetDevCapsW)( UINT_PTR uDeviceID, LPMIDIOUTCAPSW pmoc,  UINT cbmoc);
	typedef MMRESULT (WINAPI* PXmidiOutGetVolume)( HMIDIOUT hmo,  LPDWORD pdwVolume);
	typedef MMRESULT (WINAPI* PXmidiOutSetVolume)( HMIDIOUT hmo,  DWORD dwVolume);
	typedef MMRESULT (WINAPI* PXmidiOutGetErrorTextA)( MMRESULT mmrError, LPSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXmidiOutGetErrorTextW)( MMRESULT mmrError, LPWSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXmidiOutOpen)( LPHMIDIOUT phmo,  UINT uDeviceID,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXmidiOutClose)( HMIDIOUT hmo);
	typedef MMRESULT (WINAPI* PXmidiOutPrepareHeader)( HMIDIOUT hmo, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiOutUnprepareHeader)( HMIDIOUT hmo, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiOutShortMsg)( HMIDIOUT hmo,  DWORD dwMsg);
	typedef MMRESULT (WINAPI* PXmidiOutLongMsg)( HMIDIOUT hmo, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiOutReset)( HMIDIOUT hmo);
	typedef MMRESULT (WINAPI* PXmidiOutCachePatches)( HMIDIOUT hmo,  UINT uBank,LPWORD pwpa,  UINT fuCache);
	typedef MMRESULT (WINAPI* PXmidiOutCacheDrumPatches)( HMIDIOUT hmo,  UINT uPatch, LPWORD pwkya,  UINT fuCache);
	typedef MMRESULT (WINAPI* PXmidiOutGetID)( HMIDIOUT hmo,  LPUINT puDeviceID);
	typedef MMRESULT (WINAPI* PXmidiOutMessage)( HMIDIOUT hmo,  UINT uMsg,  DWORD_PTR dw1,  DWORD_PTR dw2);
	typedef UINT (WINAPI* PXmidiInGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXmidiInGetDevCapsA)( UINT_PTR uDeviceID, LPMIDIINCAPSA pmic,  UINT cbmic);
	typedef MMRESULT (WINAPI* PXmidiInGetDevCapsW)( UINT_PTR uDeviceID,  LPMIDIINCAPSW pmic,  UINT cbmic);
	typedef MMRESULT (WINAPI* PXmidiInGetErrorTextA)( MMRESULT mmrError,  LPSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXmidiInGetErrorTextW)( MMRESULT mmrError,  LPWSTR pszText,  UINT cchText);
	typedef MMRESULT (WINAPI* PXmidiInOpen)( LPHMIDIIN phmi,  UINT uDeviceID,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXmidiInClose)( HMIDIIN hmi);
	typedef MMRESULT (WINAPI* PXmidiInPrepareHeader)( HMIDIIN hmi, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiInUnprepareHeader)( HMIDIIN hmi, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiInAddBuffer)( HMIDIIN hmi, LPMIDIHDR pmh,  UINT cbmh);
	typedef MMRESULT (WINAPI* PXmidiInStart)( HMIDIIN hmi);
	typedef MMRESULT (WINAPI* PXmidiInStop)( HMIDIIN hmi);
	typedef MMRESULT (WINAPI* PXmidiInReset)( HMIDIIN hmi);
	typedef MMRESULT (WINAPI* PXmidiInGetID)( HMIDIIN hmi,  LPUINT puDeviceID);
	typedef MMRESULT (WINAPI* PXmidiInMessage)( HMIDIIN hmi,  UINT uMsg,  DWORD_PTR dw1,  DWORD_PTR dw2);
	typedef UINT (WINAPI* PXauxGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXauxGetDevCapsA)( UINT_PTR uDeviceID, LPAUXCAPSA pac,  UINT cbac);
	typedef MMRESULT (WINAPI* PXauxGetDevCapsW)( UINT_PTR uDeviceID, LPAUXCAPSW pac,  UINT cbac);
	typedef MMRESULT (WINAPI* PXauxSetVolume)( UINT uDeviceID,  DWORD dwVolume);
	typedef MMRESULT (WINAPI* PXauxGetVolume)( UINT uDeviceID,  LPDWORD pdwVolume);
	typedef MMRESULT (WINAPI* PXauxOutMessage)( UINT uDeviceID,  UINT uMsg,  DWORD_PTR dw1,  DWORD_PTR dw2);
	typedef UINT (WINAPI* PXmixerGetNumDevs)(void);
	typedef MMRESULT (WINAPI* PXmixerGetDevCapsA)( UINT_PTR uMxId, LPMIXERCAPSA pmxcaps,  UINT cbmxcaps);
	typedef MMRESULT (WINAPI* PXmixerGetDevCapsW)( UINT_PTR uMxId, LPMIXERCAPSW pmxcaps,  UINT cbmxcaps);
	typedef MMRESULT (WINAPI* PXmixerOpen)( LPHMIXER phmx,  UINT uMxId,  DWORD_PTR dwCallback,  DWORD_PTR dwInstance,  DWORD fdwOpen);
	typedef MMRESULT (WINAPI* PXmixerClose)( HMIXER hmx);
	typedef DWORD (WINAPI* PXmixerMessage)( HMIXER hmx,  UINT uMsg,  DWORD_PTR dwParam1,  DWORD_PTR dwParam2);
	typedef MMRESULT (WINAPI* PXmixerGetLineInfoA)( HMIXEROBJ hmxobj, LPMIXERLINEA pmxl,  DWORD fdwInfo);
	typedef MMRESULT (WINAPI* PXmixerGetLineInfoW)( HMIXEROBJ hmxobj, LPMIXERLINEW pmxl,  DWORD fdwInfo);
	typedef MMRESULT (WINAPI* PXmixerGetID)( HMIXEROBJ hmxobj,  UINT FAR* puMxId,  DWORD fdwId);
	typedef MMRESULT (WINAPI* PXmixerGetLineControlsA)( HMIXEROBJ hmxobj, LPMIXERLINECONTROLSA pmxlc,  DWORD fdwControls);
	typedef MMRESULT (WINAPI* PXmixerGetLineControlsW)( HMIXEROBJ hmxobj, LPMIXERLINECONTROLSW pmxlc,  DWORD fdwControls);
	typedef MMRESULT (WINAPI* PXmixerGetControlDetailsA)( HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd,  DWORD fdwDetails);
	typedef MMRESULT (WINAPI* PXmixerGetControlDetailsW)( HMIXEROBJ hmxobj, LPMIXERCONTROLDETAILS pmxcd,  DWORD fdwDetails);


	typedef struct PXWindowsAudioMultimedia_
	{
		PXwaveOutGetDevCapsA		 waveOutGetDevCapsA;
		PXwaveOutGetDevCapsW		 waveOutGetDevCapsW;
		PXwaveOutGetNumDevs		 waveOutGetNumDevs;
		PXwaveOutGetVolume		waveOutGetVolume;
		PXwaveOutSetVolume		waveOutSetVolume;
		PXwaveOutGetErrorTextA		waveOutGetErrorTextA;
		PXwaveOutGetErrorTextW		waveOutGetErrorTextW;
		PXwaveOutOpen		waveOutOpen;
		PXwaveOutClose		waveOutClose;
		PXwaveOutPrepareHeader		waveOutPrepareHeader;
		PXwaveOutUnprepareHeader		waveOutUnprepareHeader;
		PXwaveOutWrite		waveOutWrite;
		PXwaveOutPause		waveOutPause;
		PXwaveOutRestart		waveOutRestart;
		PXwaveOutReset		waveOutReset;
		PXwaveOutBreakLoop		waveOutBreakLoop;
		PXwaveOutGetPosition		waveOutGetPosition;
		PXwaveOutGetPitch		waveOutGetPitch;
		PXwaveOutSetPitch		waveOutSetPitch;
		PXwaveOutGetPlaybackRate		waveOutGetPlaybackRate;
		PXwaveOutSetPlaybackRate		waveOutSetPlaybackRate;
		PXwaveOutGetID		waveOutGetID;
		PXwaveOutMessage		waveOutMessage;
		PXwaveInGetNumDevs		 waveInGetNumDevs;
		PXwaveInGetDevCapsA		waveInGetDevCapsA;
		PXwaveInGetDevCapsW		waveInGetDevCapsW;
		PXwaveInGetErrorTextA		waveInGetErrorTextA;
		PXwaveInGetErrorTextW		waveInGetErrorTextW;
		PXwaveInOpen		waveInOpen;
		PXwaveInClose		waveInClose;
		PXwaveInPrepareHeader		waveInPrepareHeader;
		PXwaveInUnprepareHeader		waveInUnprepareHeader;
		PXwaveInAddBuffer		waveInAddBuffer;
		PXwaveInStart		waveInStart;
		PXwaveInStop		waveInStop;
		PXwaveInReset		waveInReset;
		PXwaveInGetPosition		waveInGetPosition;
		PXwaveInGetID		waveInGetID;
		PXwaveInMessage		waveInMessage;
		PXmidiOutGetNumDevs		 midiOutGetNumDevs;
		PXmidiStreamOpen		midiStreamOpen;
		PXmidiStreamClose		midiStreamClose;
		PXmidiStreamProperty		midiStreamProperty;
		PXmidiStreamPosition		midiStreamPosition;
		PXmidiStreamOut		midiStreamOut;
		PXmidiStreamPause		midiStreamPause;
		PXmidiStreamRestart		midiStreamRestart;
		PXmidiStreamStop		midiStreamStop;
		PXmidiConnect		midiConnect;
		PXmidiDisconnect		midiDisconnect;
		PXmidiOutGetDevCapsA		midiOutGetDevCapsA;
		PXmidiOutGetDevCapsW		midiOutGetDevCapsW;
		PXmidiOutGetVolume		midiOutGetVolume;
		PXmidiOutSetVolume		midiOutSetVolume;
		PXmidiOutGetErrorTextA		midiOutGetErrorTextA;
		PXmidiOutGetErrorTextW		midiOutGetErrorTextW;
		PXmidiOutOpen		midiOutOpen;
		PXmidiOutClose		midiOutClose;
		PXmidiOutPrepareHeader		midiOutPrepareHeader;
		PXmidiOutUnprepareHeader		midiOutUnprepareHeader;
		PXmidiOutShortMsg		midiOutShortMsg;
		PXmidiOutLongMsg		midiOutLongMsg;
		PXmidiOutReset		midiOutReset;
		PXmidiOutCachePatches		midiOutCachePatches;
		PXmidiOutCacheDrumPatches		midiOutCacheDrumPatches;
		PXmidiOutGetID		midiOutGetID;
		PXmidiOutMessage		midiOutMessage;
		PXmidiInGetNumDevs		 midiInGetNumDevs;
		PXmidiInGetDevCapsA		midiInGetDevCapsA;
		PXmidiInGetDevCapsW		midiInGetDevCapsW;
		PXmidiInGetErrorTextA		midiInGetErrorTextA;
		PXmidiInGetErrorTextW		midiInGetErrorTextW;
		PXmidiInOpen		midiInOpen;
		PXmidiInClose		midiInClose;
		PXmidiInPrepareHeader		midiInPrepareHeader;
		PXmidiInUnprepareHeader		midiInUnprepareHeader;
		PXmidiInAddBuffer		midiInAddBuffer;
		PXmidiInStart		midiInStart;
		PXmidiInStop		midiInStop;
		PXmidiInReset		midiInReset;
		PXmidiInGetID		midiInGetID;
		PXmidiInMessage		midiInMessage;
		PXauxGetNumDevs		 auxGetNumDevs;
		PXauxGetDevCapsA		auxGetDevCapsA;
		PXauxGetDevCapsW		auxGetDevCapsW;
		PXauxSetVolume		auxSetVolume;
		PXauxGetVolume		auxGetVolume;
		PXauxOutMessage		auxOutMessage;
		PXmixerGetNumDevs		 mixerGetNumDevs;
		PXmixerGetDevCapsA		mixerGetDevCapsA;
		PXmixerGetDevCapsW		mixerGetDevCapsW;
		PXmixerOpen		mixerOpen;
		PXmixerClose		mixerClose;
		PXmixerMessage		 mixerMessage;
		PXmixerGetLineInfoA		mixerGetLineInfoA;
		PXmixerGetLineInfoW		mixerGetLineInfoW;
		PXmixerGetID		mixerGetID;
		PXmixerGetLineControlsA		mixerGetLineControlsA;
		PXmixerGetLineControlsW		mixerGetLineControlsW;
		PXmixerGetControlDetailsA		mixerGetControlDetailsA;
		PXmixerGetControlDetailsW		mixerGetControlDetailsW;
	}
	PXWindowsAudioMultimedia;




#if OSWindows
	PXPrivate void CALLBACK PXMIDIInputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);
	PXPrivate void CALLBACK PXMIDIOutputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);


	PXPublic PXActionResult PXAPI PXMIDIInitialize(PXAudio* const pxAudio);
	PXPublic PXActionResult PXAPI PXMIDIRelease(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAPI PXMIDIDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAPI PXMIDIDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	PXPublic PXActionResult PXAPI PXMIDIDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAPI PXMIDIDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceLoad
	(
		PXAudio* const pxAudio,
		PXAudioDevice* const pxAudioDevice,
		PXSound* const pxSound
	);

	PXPublic PXActionResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
	PXPublic PXActionResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


	PXPublic PXActionResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
	PXPublic PXActionResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


	PXPublic PXActionResult PXAPI PXMIDIDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
	PXPublic PXActionResult PXAPI PXMIDIDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

	PXPublic PXActionResult PXAPI PXMIDIDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
	PXPublic PXActionResult PXAPI PXMIDIDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

	PXPublic PXActionResult PXAPI PXMIDIDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

	PXPublic PXActionResult PXAPI PXMIDIDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
	PXPublic PXActionResult PXAPI PXMIDIDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

	PXPublic PXActionResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

	PXPublic PXActionResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);


	PXPublic PXActionResult PXAPI PXMIDIDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAPI PXMIDIDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAPI PXMIDIDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);


	PXPublic PXActionResult PXAPI PXMIDIDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif

#ifdef __cplusplus
}
#endif

#endif

/*
midiOutGetNumDevs
midiStreamOpen
midiStreamClose
midiStreamProperty
midiStreamPosition
midiStreamOut
midiStreamPause
midiStreamRestart
midiStreamStop
midiConnect
midiDisconnect


midiOutGetDevCapsA
midiOutGetDevCapsW
midiOutGetVolume
midiOutSetVolume
midiOutGetErrorTextA
midiOutOpen
midiOutClose
midiOutPrepareHeader
midiOutUnprepareHeader
midiOutShortMsg
midiOutLongMsg
midiOutReset
midiOutCachePatches
midiOutCacheDrumPatches
midiOutGetID
midiOutMessage



midiInGetNumDevs
midiInGetDevCapsW
midiInGetErrorTextA

midiInOpen
midiInClose
midiInPrepareHeader
midiInUnprepareHeader
midiInAddBuffer
midiInStart
midiInStop
midiInReset
midiInGetID
midiInMessage




auxGetNumDevs
auxGetDevCapsW
auxSetVolume
auxGetVolume


mixerGetNumDevs
mixerGetDevCapsW
mixerOpen
mixerClose
mixerMessage
mixerGetLineInfoW
mixerGetID
mixerGetLineControlsW
mixerGetControlDetailsW
mixerSetControlDetails
*/