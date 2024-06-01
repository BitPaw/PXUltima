#include "PXIAudio.h"

#if OSUnix
#elif OSWindows
#include <audioclient.h> // Windows Vista (+UWP) 
//#pragma comment(lib, "dsound.lib") 
#endif

PXActionResult PXAPI PXIAudioInitialize(PXAudio* const pxAudio)
{
    // Open Library
    {

    }

    // Load functions
    {

    }

    // Link functions
    {
        pxAudio->DeviceAmount = PXIAudioDeviceAmount;
        pxAudio->DeviceFetch = PXIAudioDeviceFetch;
        pxAudio->DeviceFetchAll = PXIAudioDeviceFetchAll;
        pxAudio->DeviceOpen = PXIAudioDeviceOpen;
        pxAudio->DeviceClose = PXIAudioDeviceClose;
        pxAudio->DeviceLoad = PXIAudioDeviceLoad;
        pxAudio->DevicePitchIncrease = PXIAudioDevicePitchIncrease;
        pxAudio->DevicePitchSet = PXIAudioDevicePitchSet;
        pxAudio->DevicePitchReduce = PXIAudioDevicePitchReduce;
        pxAudio->DeviceVolumeGet = PXIAudioDeviceVolumeGet;
        pxAudio->DeviceVolumeSetEqual = PXIAudioDeviceVolumeSetEqual;
        pxAudio->DeviceVolumeSetIndividual = PXIAudioDeviceVolumeSetIndividual;
        pxAudio->DeviceStart = PXIAudioDeviceStart;
        pxAudio->DeviceStop = PXIAudioDeviceStop;
        pxAudio->DevicePause = PXIAudioDevicePause;
        pxAudio->PlayCursorSet = PXIAudioDevicePlayCursorSet;
        pxAudio->PlayCursorGet = PXIAudioDevicePlayCursorGet;
        pxAudio->PlaySpeedSet = PXIAudioDevicePlaySpeedSet;
        pxAudio->PlaySpeedGet = PXIAudioDevicePlaySpeedGet;
        pxAudio->PositionSet = PXIAudioDevicePositionSet;
        pxAudio->PositionGet = PXIAudioDevicePositionGet;
        pxAudio->ConeAnglesSet = PXIAudioDeviceConeAnglesSet;
        pxAudio->ConeAnglesGet = PXIAudioDeviceConeAnglesGet;
        pxAudio->ConeOrientationGet = PXIAudioDeviceConeOrientationGet;
        pxAudio->ConeOrientationSet = PXIAudioDeviceConeOrientationSet;
        pxAudio->ConeOutsideVolumeGet = PXIAudioDeviceConeOutsideVolumeGet;
        pxAudio->ConeOutsideVolumeSet = PXIAudioDeviceConeOutsideVolumeSet;
        pxAudio->DistanceMaxGet = PXIAudioDeviceDistanceMaxGet;
        pxAudio->DistanceMaxSet = PXIAudioDeviceDistanceMaxSet;
        pxAudio->DistanceMinGet = PXIAudioDeviceDistanceMinGet;
        pxAudio->DistanceMinSet = PXIAudioDeviceDistanceMinSet;
        pxAudio->ModeGet = PXIAudioDeviceModeGet;
        pxAudio->ModeSet = PXIAudioDeviceModeSet;
        pxAudio->VelocityGet = PXIAudioDeviceVelocityGet;
        pxAudio->VelocitySet = PXIAudioDeviceVelocitySet;
        pxAudio->DistanceFactorGet = PXIAudioDeviceDistanceFactorGet;
        pxAudio->DistanceFactorSet = PXIAudioDeviceDistanceFactorSet;
        pxAudio->DopplerFactorGet = PXIAudioDeviceDopplerFactorGet;
        pxAudio->DopplerFactorSet = PXIAudioDeviceDopplerFactorSet;
        pxAudio->OrientationGet = PXIAudioDeviceOrientationGet;
        pxAudio->OrientationSet = PXIAudioDeviceOrientationSet;
        pxAudio->RolloffFactorGet = PXIAudioDeviceRolloffFactorGet;
        pxAudio->RolloffFactorSet = PXIAudioDeviceRolloffFactorSet;
        pxAudio->DeferredSettingsCommit = PXIAudioDeviceDeferredSettingsCommit;
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXIAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}