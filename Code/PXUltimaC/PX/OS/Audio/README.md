# Audio System

## Components
|Name|Use|
|:-|:-|
|AudioOutputDevice| Speaker or headset|
|AudioInputDevice| Microphone or singal-in|

### Windows
|Version|Name|Library|Description|
|:-|:-:|:-:|:-|
|Since Windows 3.1| Windows Multi-Media | WINMM.DLL | First Standard Audio system from windows.<br>Simple to use but suffers from poor design choices when pushed to the limit. A bottleneck is the KMixer (Kernel), it has a fixed 30ms latency which makes low latency work impossible. There are ways to bypass it.   
|Windows 98 to XP| DirectSound<br>DirectX Audio| DSOUND.DLL | Reworked API works very similar to WINMM but also adds effects if available. Important to note is that this API has been debricated by Windows and is beeing emulated on Vista and onwards. Because of this, Hardware acceleration is bypassed by the software. Although the lost performance might not be detectable, as CPUs have gotten very fast, the use should be omitted in favor of XAudio2.| 
|Windows Vista and up| Windows Audio Session API (WASAPI) |Audioclient.h<br>Audiopolicy.h |Avoids latency and corruption by bypassing mixers and effects. Used for exact sound generation and production. |
|Windows Vista and up| XAudio2<br>DirectX Audio 2|xaudio2.h, 	Xaudio2.lib | A replacement for DirectSound, is a wrapper for the WASAPI that adds effects and 3D audio. Alot of functionality like device discovery still requires WASAPI. | 
|Windows Vista and up| CoreAudio<br>MMDeviceAPI| mmdeviceapi.h| An Element of WSSAPI that is its own element. It is only used for device descovery. | 

With given APIs we can build a system like this:

|Task|Windows 3.1 to Windows 95| Windows 95 to XP|Vista and up|
|:-|:-|:-|:-|
|Device selection/discovery      |WinMM + Registry|DirectSound| MMDevice API|
|Input (microphones)	 |WinMM|DirectSound| WASAPI|
|Output (Low-latency?)    |WinMM|DirectSound| XAudio2 or WASAPI (exclusive)|
|Playback & mixing	   |WinMM|DirectSound|XAudio2|
|3D-Audio and effects              |WinMM|DirectSound| XAudio2 + X3DAudio|



# Links
- [microsoft - Audio & Video](https://learn.microsoft.com/en-us/windows/win32/audio-and-video)
- [wikipedia - WINMM](https://en.wikipedia.org/wiki/Windows_legacy_audio_components)
- [wikipedia - WSAPI](https://en.wikipedia.org/wiki/Technical_features_new_to_Windows_Vista#Audio)
- [microsoft - mmdeviceapi](https://learn.microsoft.com/en-us/windows/win32/api/mmdeviceapi/nn-mmdeviceapi-immnotificationclient)
- [microsoft - coreaudio](https://learn.microsoft.com/en-us/windows/win32/coreaudio/core-audio-interfaces)
- [microsoft - xaudio](https://learn.microsoft.com/en-us/windows/win32/api/xaudio2/nf-xaudio2-xaudio2create)
