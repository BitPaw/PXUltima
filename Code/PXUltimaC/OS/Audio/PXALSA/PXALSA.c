#include "PXALSA.h"

#if OSUnix
//#include <sys/asoundlib.h>
//#include <alsa/asoundlib.h>
#pragma comment(lib, "libasound.so")
#endif

PXActionResult PXAPI PXALSAInitialize(PXAudio* const pxAudio)
{
#if OSUnix && 0
    // sound PCM (Pulse Code Modulation)

    snd_pcm_t* SoundDeviceCurrent;


      snd_pcm_hw_params_t *hw_params;

// https://soundprogramming.net/programming/alsa-tutorial-1-initialization/

  int i;
  int err;
  snd_pcm_hw_params_t *hw_params;


#if PXLogEnable
        PXLogPrint
        (
            xxxxxxxxxxxxxxx,
            "ALSA",
            "Initialize",
            "--- Start ---"
        );
#endif


    // Open device
    {
        const PXBool useDefaultDevice = NULL == name;
        const char* deviceName = useDefaultDevice ? "plughw:0,0" : "???";

        const int resultID = snd_pcm_open(&pxAudio->SoundDeviceCurrent, deviceName, SND_PCM_STREAM_PLAYBACK, 0 );
        const PXBool success = 0 == resultID;

        if(!success)
        {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "ALSA",
            "Device-Create",
            "Failed to create Name:<%s>",
            deviceName
        );
#endif
            return PXFailedOpen;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "ALSA",
            "Device-Create",
            "Success. Name:<%s>",
            deviceName
        );
#endif
    }


{
    const int resultID = snd_pcm_hw_params_malloc(&hw_params));
    const PXBool success = 0 == resultID;

    if(!success)
    {
#if PXLogEnable
        const char* errorMessage = snd_strerror(err);

        PXLogPrint
        (
            PXLoggingError,
            "ALSA",
            "PCM-Alloc",
            "Failed hardware parameter reservation. %s",
            errorMessage
        );
#endif

        return PXOutOfMem;
    }
}

  if ((err = snd_pcm_hw_params_any (_soundDevice, hw_params)) < 0)
  {
      cout << "Init: cannot initialize hardware parameter structure (" << snd_strerror (err) << ")" << endl;
      return false;
  }

  // Enable resampling.
  unsigned int resample = 1;
  err = snd_pcm_hw_params_set_rate_resample(_soundDevice, hw_params, resample);
  if (err < 0)
  {
      cout << "Init: Resampling setup failed for playback: " << snd_strerror(err) << endl;
      return err;
  }

  // Set access to RW interleaved.
  if ((err = snd_pcm_hw_params_set_access (_soundDevice, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
      cout << "Init: cannot set access type (" << snd_strerror (err) << ")" << endl;
      return false;
  }

  if ((err = snd_pcm_hw_params_set_format (_soundDevice, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
  {
      cout << "Init: cannot set sample format (" << snd_strerror (err) << ")" << endl;
      return false;
  }

  // Set channels to stereo (2).
  if ((err = snd_pcm_hw_params_set_channels (_soundDevice, hw_params, 2)) < 0)
  {
      cout << "Init: cannot set channel count (" << snd_strerror (err) << ")" << endl;
      return false;
  }

  // Set sample rate.
  unsigned int actualRate = 44100;
  if ((err = snd_pcm_hw_params_set_rate_near (_soundDevice, hw_params, &actualRate, 0)) < 0)
  {
      cout << "Init: cannot set sample rate to 44100. (" << snd_strerror (err) << ")"  << endl;
      return false;
  }
  if( actualRate < 44100 )
  {
      cout << "Init: sample rate does not match requested rate. (" << "44100 requested, " << actualRate << " acquired)" << endl;
  }

  // Apply the hardware parameters that we've set.
  if ((err = snd_pcm_hw_params (_soundDevice, hw_params)) < 0)
  {
      cout << "Init: cannot set parameters (" << snd_strerror (err) << ")" << endl;
      return false;
  }
  else
  {
     cout << "Audio device parameters have been set successfully." << endl;
  }

  // Get the buffer size.
  snd_pcm_uframes_t bufferSize;
  snd_pcm_hw_params_get_buffer_size( hw_params, &bufferSize );
  // If we were going to do more with our sound device we would want to store
  // the buffer size so we know how much data we will need to fill it with.
  cout << "Init: Buffer size = " << bufferSize << " frames." << endl;

  // Display the bit size of samples.
  cout << "Init: Significant bits for linear samples = " << snd_pcm_hw_params_get_sbits(hw_params) << endl;

  // Free the hardware parameters now that we're done with them.
  snd_pcm_hw_params_free (hw_params);

  // Prepare interface for use.
  if ((err = snd_pcm_prepare (_soundDevice)) < 0)
  {
      cout << "Init: cannot prepare audio interface for use (" << snd_strerror (err) << ")" << endl;
      return false;
  }
  else
  {
      cout << "Audio device has been prepared for use." << endl;
  }

  return true;

































    // Allocate memory for the device to hold data
    err = snd_pcm_hw_params_malloc (&hw_params);

    // Fetch parameters
    err = snd_pcm_hw_params_any (pxAudio->SoundDeviceCurrent, hw_params);

    // set our samples to interleaved:
    err = snd_pcm_hw_params_set_access (pxAudio->SoundDeviceCurrent, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)

    // Setup audio channel
    err = snd_pcm_hw_params_set_format (pxAudio->SoundDeviceCurrent, hw_params, SND_PCM_FORMAT_S16_LE);
    err = snd_pcm_hw_params_set_channels (pxAudio->SoundDeviceCurrent, hw_params, 2);
    unsigned int actualRate = 44100;
    err = snd_pcm_hw_params_set_rate_near (pxAudio->SoundDeviceCurrent, hw_params, &actualRate, 0);

    // Apply
    err = snd_pcm_hw_params (pxAudio->SoundDeviceCurrent, hw_params);


    snd_pcm_uframes_t bufferSize;
    snd_pcm_hw_params_get_buffer_size( hw_params, &bufferSize );
    cout << "Init: Buffer size = " << bufferSize << " frames." << endl;
    _playbackFrames = bufferSize;
    cout << "Init: Significant bits for linear samples = " << snd_pcm_hw_params_get_sbits(hw_params) << endl;


    // Release memeory of hardwareparameter
    snd_pcm_hw_params_free (hw_params);




    // Prepare audio interface
    err = snd_pcm_prepare (pxAudio->SoundDeviceCurrent);

    // Do something

    // After use we need to close it
    snd_pcm_close (pxAudio->SoundDeviceCurrent);


#if PXLogEnable
        PXLogPrint
        (
            xxxxxxxxxxxxxxx,
            "ALSA",
            "Initialize",
            "--- Done ---"
        );
#endif

        return PXActionRefusedNotImplemented;

#else
return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXALSARelease(PXAudio* const pxAudio)
{
#if OSUnix && 0
  snd_pcm_close (pxAudio->SoundDeviceCurrent);
  cout << "Audio device has been uninitialized." << endl;

  return PXActionRefusedNotImplemented;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}
