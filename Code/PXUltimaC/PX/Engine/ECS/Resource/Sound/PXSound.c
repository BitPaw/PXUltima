#include "PXSound.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Math/PXMath.h>

const char PXSoundText[] = "Sound";
const PXI8U PXSoundTextLength = sizeof(PXSoundText);
const PXECSRegisterInfoStatic PXSoundRegisterInfo =
{
    {sizeof(PXSoundText), sizeof(PXSoundText), PXSoundText, TextFormatASCII},
    sizeof(PXSound),
    __alignof(PXSound),
    PXECSTypeResource,
    PXSoundCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXSoundRegisterInfoDynamic;

PXResult PXAPI PXSoundRegisterToECS()
{
    PXECSRegister(&PXSoundRegisterInfo, &PXSoundRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXSoundCreate(PXSound** pxSound, PXSoundCreateInfo PXREF pxSoundCreateInfo)
{
    if(!(pxSound && pxSoundCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxSoundCreateInfo->Info.Static = &PXSoundRegisterInfo;
    pxSoundCreateInfo->Info.Dynamic = &PXSoundRegisterInfoDynamic;

    const PXResult loadResult = PXECSLoad(pxSound, pxSoundCreateInfo);

    if(PXResultOK != loadResult)
    {
        return loadResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXSoundText,
        "Load",
        "successful <%s>.",
        pxSoundCreateInfo->Info.FilePath.A
    );
#endif

    return PXResultOK;
}

#define SAMPLE_RATE 44100

void PXAPI FillBufferWithElectricHum(const int sampleRate, short* samples, int sampleCount, float frequency)
{
    for(int i = 0; i < sampleCount; i++)
    {
        float t = (float)i / sampleRate;
        samples[i] = (short)(log(2 * PXMathConstantPI * frequency * t) * 3000);
    }
}

void PXAPI FillBufferWithRain(short* samples, int sampleCount)
{
    for(int i = 0; i < sampleCount; i++)
    {
        float noise = ((float)(rand() % 32768) / 16384.0f) - 1.0f; // White noise [-1, 1]

        // Modulate with simple envelope or LFO for a "wetter" texture
        float dropChance = rand() % 10000;
        float wetness = 0.2f + ((float)(rand() % 1000) / 5000.0f); // Random slight volume changes

        float value = noise * 2000.0f * wetness;

        // Occasional "drip" pop
        if(dropChance < 2) {
            value += 8000.0f * ((rand() % 2 == 0) ? 1 : -1); // Sharp click
        }

        if(value > 32767) value = 32767;
        if(value < -32768) value = -32768;

        samples[i] = (short)value;
    }
}


void PXAPI FillBufferWithThunder(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float envelope = expf(-3.0f * t); // quick decay
        float rumble = ((rand() % 65536) - 32768) / 32768.0f;
        samples[i] = (short)(rumble * envelope * 12000);
    }
}


// 8-bit snare?
void PXAPI FillBufferWithGravelFootsteps(short* samples, int sampleCount) {
    int stepInterval = SAMPLE_RATE / 2;
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)(i % stepInterval) / (stepInterval / 8.0f);
        float noise = ((rand() % 65536) - 32768) / 32768.0f;
        float env = (t < 1.0f) ? (1.0f - t) : 0.0f; // envelope
        samples[i] = (short)(noise * env * 8000);
    }
}

// blip?
void PXAPI FillBufferWithConcreteFootsteps(short* samples, int sampleCount) {
    int stride = SAMPLE_RATE / 3;
    for(int i = 0; i < sampleCount; i++) {
        samples[i] = (i % stride == 0) ? 8000 : 0; // strong tap impulse
    }
}

// firework cracle?
void PXAPI FillBufferWithFireCrackle(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float noise = ((rand() % 65536) - 32768) / 32768.0f;
        float burst = (rand() % 500 == 0) ? 1.0f : 0.0f;
        samples[i] = (short)(noise * burst * 15000);
    }
}


void PXAPI FillBufferWithWind(short* samples, int sampleCount) {
    float filter = 0.0f;
    float smoothing = 0.02f;
    for(int i = 0; i < sampleCount; i++) {
        float raw = ((rand() % 65536) - 32768) / 32768.0f;
        filter = (1.0f - smoothing) * filter + smoothing * raw;
        samples[i] = (short)(filter * 7000);
    }
}










// Simple 1D Perlin noise implementation
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float grad(int hash, float x) {
    int h = hash & 15;
    float grad = 1.0f + (h & 7);  // Gradient value 1.0 - 8.0
    if(h & 8) grad = -grad;
    return grad * x;
}

int permutation[512];

// Initialize permutation table
void InitPerlin() {
    int p[256];
    for(int i = 0; i < 256; i++) p[i] = i;

    // Shuffle
    for(int i = 255; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = p[i];
        p[i] = p[j];
        p[j] = temp;
    }

    for(int i = 0; i < 512; i++) {
        permutation[i] = p[i & 255];
    }
}

float Perlin1D(float x) {
    int xi = (int)floorf(x) & 255;
    float xf = x - floorf(x);
    float u = fade(xf);

    int a = permutation[xi];
    int b = permutation[xi + 1];

    return lerp(grad(a, xf), grad(b, xf - 1), u);
}







void PXAPI FillBufferWithPerlinNoise(short* samples, int sampleCount) {
    InitPerlin();  // Call once before generating

    float scale = 0.0009f;  // Controls frequency of variation, 0.01f;
    for(int i = 0; i < sampleCount; i++) {
        float x = i * scale;
        float noise = Perlin1D(x);
        samples[i] = (short)(noise * 1000);  // Scale to 16-bit audio, 10000
    }
}












void PXAPI FillBufferWithIceCracks(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float crack = (rand() % 10 < 3) ? ((rand() % 2 ? 1 : -1) * 15) : 0;
        float decay = expf(-0.000005f * (i % SAMPLE_RATE));
        samples[i] = (short)(crack * decay);
    }
}


void PXAPI FillBufferWithScannerBeep(short* samples, int sampleCount) {
    float freq = 880.0f;
    int pulseLength = SAMPLE_RATE / 10;
    for(int i = 0; i < sampleCount; i++) {
        int phase = i % (pulseLength * 4);
        float amp = (phase < pulseLength) ? 1.0f : 0.0f;
        float t = (float)i / SAMPLE_RATE;
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * amp * 10000);
    }
}

void PXAPI FillBufferWithMagneticWobble(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float mod = sinf(2 * PXMathConstantPI * 0.5f * t); // Slow LFO
        float freq = 200.0f + mod * 50.0f;
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * 9000);
    }
}

void PXAPI FillBufferWithBubbles(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float bubble = (rand() % 10000 < 5) ? sinf(2 * PXMathConstantPI * 400.0f * t) * 10000 : 0;
        samples[i] = (short)bubble;
    }
}

void PXAPI FillBufferWithZombieMoan(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float mod = sinf(2 * PXMathConstantPI * 0.2f * t); // LFO
        float freq = 80.0f + mod * 10.0f;
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * 12000);
    }
}


// 8-bit explosion, tsssss
void PXAPI FillBufferWithExplosion(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float envelope = expf(-4.0f * t);  // fast decay
        float noise = ((rand() % 65536) - 32768) / 32768.0f;
        samples[i] = (short)(noise * envelope * 16000);
    }


}

// woowooowoowoowoowoowowo, UFO
void PXAPI FillBufferWithPsychoDrone(short* samples, int sampleCount) {
    float freq1 = 110.0f;
    float freq2 = 112.5f;
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float wave = sinf(2 * PXMathConstantPI * freq1 * t) + sinf(2 * PXMathConstantPI * freq2 * t);
        samples[i] = (short)(wave * 6000);
    }
}

// yea? maybe a bit changes
void PXAPI FillBufferWithSteamHiss(short* samples, int sampleCount) {
    float last = 0.0f;
    for(int i = 0; i < sampleCount; i++) {
        float raw = ((rand() % 65536) - 32768) / 32768.0f;
        float highpass = raw - last;
        last = raw;
        samples[i] = (short)(highpass * 10000);
    }
}

// aliens
void PXAPI FillBufferWithAlienSignal(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float mod = sinf(2 * PXMathConstantPI * 0.25f * t);  // slow LFO
        float freq = 300.0f + mod * 200.0f;
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * 9000);
    }
}

// Electric crackle, 8-bit
void PXAPI FillBufferWithElectricArc(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float burst = (rand() % 10000 < 10) ? sinf(2 * PXMathConstantPI * 2000.0f * t) : 0.0f;
        samples[i] = (short)(burst * 12000);
    }
}

// Bell, 8-bit. meh
void PXAPI FillBufferWithMeditationBell(short* samples, int sampleCount) {
    float baseFreq = 440.0f;
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float env = expf(-2.0f * t);
        float tone = sinf(2 * PXMathConstantPI * baseFreq * t) +
            0.5f * sinf(2 * PXMathConstantPI * baseFreq * 2 * t) +
            0.25f * sinf(2 * PXMathConstantPI * baseFreq * 3 * t);
        samples[i] = (short)(tone * env * 10000);
    }
}

void PXAPI FillBufferWithBrainwavePulse(short* samples, int sampleCount) {
    float freq = 20.0f;  // Theta wave range
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float env = sinf(2 * PXMathConstantPI * 0.6f * t);  // slow amplitude modulation
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * env * 500000);
    }
}


void PXAPI FillBufferWithGeigerClicks(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        samples[i] = (rand() % 100000 < 5) ? (rand() % 2 ? 160000 : -160000) : 0;
    }
}


void PXAPI FillBufferWithFrozenWind(short* samples, int sampleCount) {
    float last = 0.0f;
    float smoothing = 0.01f;
    for(int i = 0; i < sampleCount; i++) {
        float raw = ((rand() % 65536) - 32768) / 32768.0f;
        last = (1.0f - smoothing) * last + smoothing * raw;
        samples[i] = (short)(last * 7000);
    }
}


void PXAPI FillBufferWithMysticChimes(short* samples, int sampleCount) {
    float baseFreq = 660.0f;
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float env = expf(-3.0f * fmodf(t, 1.0f));
        float freq = baseFreq + (rand() % 5) * 110.0f;
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * env * 10000);
    }
}

void PXAPI FillBufferWithLaserZap(short* samples, int sampleCount) {
    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;
        float freq = 2000.0f - t * 1500.0f;  // downward pitch
        samples[i] = (short)(sinf(2 * PXMathConstantPI * freq * t) * 12000);
    }
}






float randf() {
    return ((float)(rand() % 65536) / 32768.0f) - 1.0f;  // [-1, 1]
}

void PXAPI FillBufferWithWindAndRain(short* samples, int sampleCount, float windStrength, float rainStrength)
{




    float windFilter = 0.0f;
    float windSmooth = 0.01f;

    for(int i = 0; i < sampleCount; i++) {
        float t = (float)i / SAMPLE_RATE;

        // --- Wind: filtered noise with LFO
        float rawWind = randf();
        windFilter = (1.0f - windSmooth) * windFilter + windSmooth * rawWind;
        float gust = 0.6f + 0.4f * sinf(2 * PXMathConstantPI * 0.1f * t);  // slow LFO
        float wind = windFilter * gust * windStrength;

        // --- Rain: white noise + occasional drops
        float rainNoise = randf() * 0.5f;
        float drop = (rand() % 10000 < 5) ? (randf() * 0.8f) : 0.0f;
        float rain = (rainNoise + drop) * rainStrength;

        // --- Mix and clamp
        float combined = (wind + rain) * 12000.0f;
        if(combined > 32767) combined = 32767;
        if(combined < -32768) combined = -32768;

        samples[i] = (short)combined;
    }
}

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define CLAMP(v) (MIN(32767, MAX(-32768, (v))))

void PXAPI FillBufferWithWeather(short* samples, int count, float rain, float wind, float thunder)
{
    static float windFilter = 0.0f;
    static int thunderCountdown = 0;

    for(int i = 0; i < count; i++)
    {
        float t = (float)i / (float)SAMPLE_RATE;

        // ---------------- Wind ------------------
        float noise = randf();
        windFilter = 0.98f * windFilter + 0.02f * noise;  // low-pass
        float gust = 0.7f + 0.3f * sinf(2 * PXMathConstantPI * 0.1f * t);  // LFO
        float windSample = windFilter * gust * wind * 9000;

        // ---------------- Rain ------------------
        float hiss = randf() * rain * 4000;
        float drop = 0.0f;
        if((rand() % 10000) < (int)(rain * 20)) {
            drop = (randf() > 0 ? 1.0f : -1.0f) * 12000.0f * rain;
        }

        float rainSample = hiss + drop;

        // ---------------- Thunder ------------------
        float thunderSample = 0.0f;
        if(thunderCountdown <= 0 && (rand() % 10000) < (int)(thunder * 3))
        {
            thunderCountdown = SAMPLE_RATE * (2 + rand() % 3);  // next thunder in 2–5s
        }

        if(thunderCountdown > 0) {
            float decay = expf(-2.0f * ((float)(SAMPLE_RATE * 5 - thunderCountdown) / SAMPLE_RATE));
            float rumble = randf() * decay * thunder * 16000;
            thunderSample += rumble;
            thunderCountdown--;
        }

        // ---------------- Mix & Output ------------------
        float mixed = windSample + rainSample + thunderSample;
        samples[i] = (short)CLAMP(mixed);
    }
}