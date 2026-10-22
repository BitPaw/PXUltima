#include "PXEngineMath.h"

const char PXEngineMathText[] = "Math-PX";
const PXI8U PXEngineMathLength = sizeof(PXEngineMathText);
const PXECSRegisterInfoStatic PXEngineMathInfoStatic =
{
    {sizeof(PXEngineMathText), sizeof(PXEngineMathText), PXEngineMathText, TextFormatASCII},
    sizeof(PXEngineMath),
    __alignof(PXEngineMath),
    PXECSTypeSystem,
    PXEngineMathCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineMathInfoDynamic;

PXEngineMath* _PXEngineMathInstance = 0;

PXResult PXAPI PXEngineMathCreate(PXEngineMath** pxEngineMathREF, PXEngineMathCreateInfo PXREF pxEngineMathCreateInfo)
{
    PXEngineMath* pxEngineMath = PXNull;

    if(!(pxEngineMathREF && pxEngineMathCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(_PXEngineMathInstance)
    {
        return PXResultOK;
    }

    pxEngineMathCreateInfo->Info.Static = &PXEngineMathInfoStatic;
    pxEngineMathCreateInfo->Info.Dynamic = &PXEngineMathInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineMathREF, pxEngineMathCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineMath = *pxEngineMathREF;

    pxEngineMath->StereoToMonoI16U = PXEngineMathStereoToMonoI16U_Scalar;
    pxEngineMath->CompareI8V = PXEngineMathCompareI8V_Scalar;
    pxEngineMath->CompareI16V = PXEngineMathCompareI16V_Scalar;

    _PXEngineMathInstance = pxEngineMath;

    return pxResult;
}

PXResult PXAPI PXEngineMathRelease(PXEngineMath PXREF pxEngineMath, PXEngineMathCreateInfo PXREF pxEngineMathCreateInfo)
{
    return PXResultOK;
}

PXEngineMath* PXAPI PXEngineMathInstanceGet(void)
{
    return _PXEngineMathInstance;
}

PXI8U PXAPI PXEngineMathCompareI8V_Scalar(const PXI8U PXREF textList, const PXI8U listAmount, const PXI8U value)
{
    for(PXI8U i = 0; i < listAmount; ++i)
    {
        const PXBool isMatch = value == textList[i];

        if(isMatch)
        {
            return i;
        }
    }

    return PXIndexI8UNotFound;
}

PXI8U PXAPI PXEngineMathCompareI8V_MMX(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value)
{
    return 0;
}

PXI8U PXAPI PXEngineMathCompareI8V_SSE2(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value)
{
    return 0;
}

PXI8U PXAPI PXEngineMathCompareI8V_AVX512(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value)
{
#if PXMathUseIntrinsics
    const __m512i zero = _mm512_setzero_si512();
    __m512i value_vector = _mm512_set1_epi8(value); // Load target byte gets copy'ed 64x

    for(PXI8U i = 0; i < listAmount; i += 64)
    {
        __mmask64 mask = ((listAmount - i) > 64) ? 0xFFFFFFFFFFFFFFFF : (1LL << (listAmount - (i))) - 1; // How many things can we load?
        __m512i data_vector = _mm512_mask_loadu_epi8(zero, mask, &dataList[i]); // Load compare array

        const PXI64U result = _mm512_cmp_epi8_mask(value_vector, data_vector, _MM_CMPINT_EQ); // Compare both 64x byte vs byte

#if 0
        char bufferA[64 * 2 + 1];
        char bufferB[64 * 2 + 1];

        for(size_t i = 0; i < 64; ++i)
        {
            bufferA[2 * i + 0] = value_vector.m512i_u8[i];
            bufferA[2 * i + 1] = ' ';
            bufferA[2 * i + 2] = 0;

            bufferB[2 * i + 0] = PXTextMakePrintable(data_vector.m512i_u8[i]);
            bufferB[2 * i + 1] = ' ';
            bufferB[2 * i + 2] = 0;
        }

        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "SIMD-Compare",
            "64x (%i) Target: %c\n"
            "%s\n"
            "%s",
            listAmount,
            value,
            bufferA,
            bufferB
        );
#endif


        if(!result) // Not a single match, get to next
        {
            continue;
        }

        // We found a match!

#if OS64B
        const PXI8U match_index = 63 - _lzcnt_u64(result); // Count leading zeros. We want the first one.
#else
        const PXI8U match_index = 63 - _lzcnt_u32(result); // Count leading zeros. We want the first one.
#endif


        return i + match_index; // Index of first hit
    }

#endif

    return (PXI8U)-1; // No match!
}

PXI16U PXAPI PXEngineMathCompareI16V_Scalar(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value)
{
    for(PXI16U i = 0; i < listAmount; ++i)
    {
        PXBool isMatch = value == dataList[i];

        if(isMatch)
        {
            return i;
        }
    }

    return PXIndexI16UNotFound;
}

PXI16U PXAPI PXEngineMathCompareI16V_MMX(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value)
{
    return 0;
}

PXI16U PXAPI PXEngineMathCompareI16V_SSE2(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value)
{
    return 0;
}

PXI16U PXAPI PXEngineMathCompareI16V_AVX512(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value)
{
#if PXMathUseIntrinsics
    const __m512i zero = _mm512_setzero_si512();
    __m512i value_vector = _mm512_set1_epi16(value); // Load target byte gets copy'ed 32x

    for(PXI8U i = 0; i < listAmount; i += 32)
    {
        // How many 16-bit elements remain?
        PXI8U remaining = listAmount - i;

        // Mask for up to 32 16-bit lanes
        __mmask32 mask = (remaining >= 32)
            ? 0xFFFFFFFF
            : ((1u << remaining) - 1u);

        // Masked load of 16-bit values
        __m512i data_vector = _mm512_mask_loadu_epi16(zero, mask, &dataList[i]);

        // Compare 32x 16-bit lanes
        __mmask32 result = _mm512_cmp_epi16_mask(value_vector, data_vector, _MM_CMPINT_EQ);

#if 0
        char bufferA[64 * 2 + 1];
        char bufferB[64 * 2 + 1];

        for(size_t i = 0; i < 64; ++i)
        {
            bufferA[2 * i + 0] = value_vector.m512i_u8[i];
            bufferA[2 * i + 1] = ' ';
            bufferA[2 * i + 2] = 0;

            bufferB[2 * i + 0] = PXTextMakePrintable(data_vector.m512i_u8[i]);
            bufferB[2 * i + 1] = ' ';
            bufferB[2 * i + 2] = 0;
        }

        PXLogPrint
        (
            PXLoggingAllocation,
            PXMemoryLogPrintTitle,
            "SIMD-Compare",
            "64x (%i) Target: %c\n"
            "%s\n"
            "%s",
            listAmount,
            value,
            bufferA,
            bufferB
        );
#endif


        if(!result) // Not a single match, get to next
        {
            continue;
        }

        // We found a match!
        const PXI8U match_index = 31 - _lzcnt_u32(result); // Count leading zeros. We want the first one.

        return i + match_index; // Index of first hit
    }

    return (PXI8U)-1; // No match!

#elif 0
    for(PXI8U i = 0; i < listAmount; i++)
    {
        PXBool isMatch = value == dataList[i];

        if(isMatch)
        {
            return i;
        }
    }

    return (PXI8U)-1;
#endif 
}

void PXAPI PXEngineMathStereoToMonoI16U_Scalar(PXI16U* data, const PXSize dataAmount)
{
    if(!data)
    {
        return;
    }

    PXSize index;

    for(index = 0; index < dataAmount; ++index)
    {
        const PXI16S sampleLeft = data[index * 2 + 0];
        const PXI16S sampleRight = data[index * 2 + 1];
        const PXI32S sampleMixed = ((PXI32S)sampleLeft + (PXI32S)sampleRight) / 2;

        // Store back into our new flat mono array
        data[index] = (PXI16S)sampleMixed;
    }

    PXSize totalWith = dataAmount * 2;

    for(; index < totalWith; index++)
    {
        data[index] = 0;
    }
}

void PXAPI PXEngineMathStereoToMonoI16U_X512(PXI16U* data, const PXSize dataAmount)
{
    // We process 32 frames (64 samples = 128 bytes) per loop iteration
    size_t vectorized_end = dataAmount & ~31;

    // A vector multiplier of all ones to add adjacent [L, R] values seamlessly
    __m512i ones = _mm512_set1_epi16(1);

    PXI16S* dst = data;
    const PXI16S* src = data;

    PXSize i = 0;

    for(; i < vectorized_end; i += 32u)
    {
        // 1. Load 32 stereo frames (64 audio samples total)
        __m512i stereo = _mm512_loadu_si512((const __m512i*)(src + i * 2));

        // 2. Pairwise add adjacent L and R values. 
        // This converts 16-bit [L0, R0, L1, R1...] into 32-bit integers [(L0+R0), (L1+R1)...] 
        // preventing overflow entirely. Yields 32 elements.
        __m512i sum_32bit = _mm512_madd_epi16(stereo, ones); // 
        __m512i mono_32bit = _mm512_srai_epi32(sum_32bit, 1); // Divide by 2 using arithmetic right shift

        // 4. Truncate/pack the 32-bit integers back into 16-bit integers.
        // This creates a 256-bit register out of the 512-bit math.
        __m256i mono_16bit = _mm512_cvtepi32_epi16(mono_32bit);

        // 5. Overwrite the buffer in-place
        _mm256_storeu_si256((__m256i*)(dst + i), mono_16bit);
    }

    PXEngineMathStereoToMonoI16U_Scalar(&src[i], dataAmount - i);
}