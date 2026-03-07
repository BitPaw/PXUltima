#include "PXChorus.h"

#include <PX/Math/PXMath.h>
#include <PX/Container/Buffer/PXBuffer.h>

void PXAPI PXFilterChorusProcess(PXFilterChorus PXREF pxFilterChorus, PXAudioFilterBuffer PXREF pxAudioFilterBuffer)
{
    PXBuffer PXREF PXBuffer = &pxAudioFilterBuffer->Buffer;
    PXSize samples = pxAudioFilterBuffer->Samples;

    /*

    for(PXSize sample = 0; sample < samples; ++sample)
    {
        // LFO
        pxFilterChorus->Phase += pxFilterChorus->Rate / pxAudioFilterBuffer->SampleRate;

        if(pxFilterChorus->Phase >= 1.0f)
            pxFilterChorus->Phase -= 1.0f;

        PXFAudio lfo = PXMathSinusRADF64(2.0 * PXMathPI * pxFilterChorus->Phase);
        PXFAudio delay = pxFilterChorus->Base + pxFilterChorus->Depth * lfo;

        PXFAudio rp = (PXFAudio)pxFilterChorus->IDX - delay;

        while(rp < 0.0f)
            rp += (PXFAudio)pxFilterChorus->size;

        int i0 = (int)rp;
        int i1 = i0 + 1;

        if(i1 >= pxFilterChorus->size)
            i1 = 0;

        PXFAudio frac = rp - (PXFAudio)i0;
        PXFAudio d0 = pxAudioFilterBuffer->Buffer.F64[i0];
        PXFAudio d1 = pxAudioFilterBuffer->Buffer.F64[i1];
        PXFAudio delayed = d0 + (d1 - d0) * frac;

        PXFAudio x = pxAudioFilterBuffer->Buffer.F64[sample];
        PXFAudio y = x * pxFilterChorus->Dry + delayed * pxFilterChorus->Wet;

        pxAudioFilterBuffer->Buffer->F64[pxFilterChorus->IDX] = x + delayed * pxFilterChorus->Feedback;
        ++pxFilterChorus->IDX;

        if(pxFilterChorus->IDX >= c->size)
            pxFilterChorus->IDX = 0;

        pxAudioFilterBuffer->Buffer.F64[sample] = y;
    }

    */
}