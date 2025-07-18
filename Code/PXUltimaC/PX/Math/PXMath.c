#include "PXMath.h"

#include <math.h>

#include <immintrin.h>
#include <wmmintrin.h>
#include <mmintrin.h>

#if OSUnix
#elif OSWindows
#include <intrin.h>
#endif

#include <PX/Math/PXMatrix.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Hardware/PXProcessor.h>

#define PXStandardLibraryMathUse 1

const PXF32 PXMathConstantPIF = PXMathConstantPI;
const double PXMathConstantPID = PXMathConstantPI;

PXMath GLOBALPXMath;

void PXAPI PXMathIntrinsicInit()
{
    PXProcessor pxProcessor;
    PXProcessorFetchInfo(&pxProcessor);  
}

void PXAPI PXMathMatrix4x4TransposeS(PXMatrix4x4* const pxMatrix4x4)
{
   
}

void PXAPI PXMathMatrix4x4TransposeX(PXMatrix4x4* const pxMatrix4x4)
{
    // _mm_loadu_ps
    // _mm_storeu_ps
    __m128 matrixTemp[4];

    PXMemoryCopyF32V(matrixTemp, pxMatrix4x4, 16);

    _MM_TRANSPOSE4_PS(matrixTemp[0], matrixTemp[1], matrixTemp[2], matrixTemp[3]);

    PXMemoryCopyF32V(matrixTemp, pxMatrix4x4, 16);
}

int PXAPI PXMathMinimumI(const int a, const int b)
{
    return PXMathMinimum(a, b);
}

PXSize PXAPI PXMathMinimumIU(const PXSize a, const PXSize b)
{
    return PXMathMinimum(a, b);
}

double PXAPI PXMathMinimumD(const double a, const double b)
{
    return PXMathMinimum(a, b);
}

PXSize PXAPI PXMathMaximumIU(const PXSize a, const PXSize b)
{
    return PXMathMaximum(a, b);
}

int PXAPI PXMathFloorI(const int a)
{
    return PXMathFloor(a);
}

int PXAPI PXMathFloorD(const double a)
{
    return PXMathFloor(a);
}

PXInt16U PXAPI PXMathAbsoluteI16(const PXInt16S value)
{
    return PXMathAbsolute(value);
}

PXInt32U PXAPI PXMathAbsoluteI32(const PXInt32S value)
{
    return PXMathAbsolute(value);
}

double PXAPI PXMathAbsoluteD(const double a)
{
    return PXMathAbsolute(a);
}

char PXAPI PXMathLimitC(const char value, const char minimum, const char maximum)
{
    return PXMathLimit(value, minimum, maximum);
}

unsigned char PXAPI PXMathLimitCU(const unsigned char value, const unsigned char minimum, const unsigned char maximum)
{
    return PXMathLimit(value, minimum, maximum);
}

int PXAPI PXMathLimitI(const int value, const int minimum, const int maximum)
{
    return PXMathLimit(value, minimum, maximum);
}

PXSize PXAPI PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum)
{
    return PXMathLimit(value, minimum, maximum);
}

void PXAPI PXMathF16ToF32(PXF32* const listOut, const PXF16* const listInput, const PXSize inputAmount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;

    if(!(listOut && listInput && inputAmount))
    {
        return;
    }

    // Check if 
    if(1) // AVX512 support
    {
        // __m256i _mm512_mask_cvtps_ph (__m256i src, __mmask16 k, __m512 a, int rounding)
        //__m256i res = _mm512_mask_cvtps_ph(); // [Intrinsic] AVX512F, vcvtps2ph, 16x 32-Bit PXF32 -> 16-Bit PXF32

    

        return; // DONE
    }

    if(1) // AVX support, 
    {
        // __m128i _mm256_cvtps_ph (__m256 a, int imm8)
        //__m128i resB = _mm256_cvtps_ph(); // [Intrinsic] immintrin.h, F16C, vcvtps2ph, 8x 32-Bit PXF32 -> 16-Bit PXF32

      

        return; // DONE
    }

    // non-intrinsic
    // ...
}

void PXAPI PXMathF16ToF32X4(PXF32* const listOut, const PXF16* const listInput, const PXSize inputAmount)
{
    __m128i simdInput;
    __m128 simdOutput;

    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;
    pxWorkSetCounter.BatchSize = 4;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF16* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF16V(simdInput.m128i_u16, extractPoint, workSet);

        simdOutput = _mm_cvtph_ps(simdInput); // [Intrinsic] 4x 16-Bit PXF32 -> 32-Bit PXF32

        PXMemoryCopyF32V(insertPoint, simdOutput.m128_f32, workSet);
    }
}

void PXAPI PXMathF16ToF32X8(PXF32* const listOut, const PXF16* const listInput, const PXSize inputAmount)
{
  
}

void PXAPI PXMathF16ToF32X16(PXF32* const listOut, const PXF16* const listInput, const PXSize inputAmount)
{
    __m256i simdInput;
    __m512 simdOutput;

    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;
    pxWorkSetCounter.BatchSize = 16;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF16* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF16V(simdInput.m256i_u16, extractPoint, workSet);

        simdOutput = _mm512_cvtph_ps(simdInput);

        PXMemoryCopyF32V(insertPoint, simdOutput.m512_f32, workSet);
    }
}

void PXAPI PXMathF32ToF16(PXF16* const listOut, const PXF32* const listInput, const PXSize inputAmount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;

    if(!(listOut && listInput && inputAmount))
    {
        return;
    }

    PXMathF32ToF16X4(listOut, listInput, inputAmount);

    // Check if 
    if(1) // AVX512 support
    {
        // __m256i _mm512_mask_cvtps_ph (__m256i src, __mmask16 k, __m512 a, int rounding)


        return; // DONE
    }


    if(1) // AVX support, 
    {
        // __m128i _mm256_cvtps_ph (__m256 a, int imm8)
        //__m128i resB = _mm256_cvtps_ph(); // [Intrinsic] immintrin.h, F16C, vcvtps2ph, 8x 32-Bit PXF32 -> 16-Bit PXF32

       

        return; // DONE
    }

    // non-intrinsic
    // ...
}

void PXAPI PXMathF32ToF16X4(PXF16* const listOut, const PXF32* const listInput, const PXSize inputAmount)
{
    __m128 simdInput;
    __m128i simdOutput;

    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;
    pxWorkSetCounter.BatchSize = 4;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
        PXF16* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF32V(simdInput.m128_f32, extractPoint, workSet);

        simdOutput = _mm_cvtps_ph(simdInput, _MM_FROUND_TO_NEAREST_INT); // [Intrinsic] 4x 32-Bit PXF32 -> 16-Bit PXF32

        PXMemoryCopyF16V(insertPoint, simdOutput.m128i_u16, workSet);
    }
}

void PXAPI PXMathF32ToF16X8(PXF16* const listOut, const PXF32* const listInput, const PXSize inputAmount)
{

}

void PXAPI PXMathF32ToF16X16(PXF16* const listOut, const PXF32* const listInput, const PXSize inputAmount)
{
    __m512 simdInput;
    __m256i simdOutput;

    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = inputAmount;
    pxWorkSetCounter.BatchSize = 16;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
        PXF16* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF32V(simdInput.m512_f32, extractPoint, workSet);

        simdOutput = _mm512_cvtps_ph(simdInput, _MM_FROUND_TO_NEAREST_INT); // [Intrinsic] AVX512F, vcvtps2ph, 16x 32-Bit PXF32 -> 16-Bit PXF32

        PXMemoryCopyF16V(insertPoint, simdOutput.m256i_u16, workSet);
    }
}

PXF32 PXAPI PXMathFastInverseSqaureRoot(PXF32 number)
{
    const PXF32 threehalfs = 1.5f;
    PXF32 x2 = number * 0.5f;
    PXF32 y = number;
    long i;

    i = *(long*)&y; // Convert the Binary Value from a PXF32 to an long. No conversion! A 1:1 Bitcopy. IEEE 574 standard
    i = 0x5f3759df - (i >> 1); // .The bit shift Halfs i. AProximation of (1/root(y))
    y = *(PXF32*)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    // last line can be repeatet for more acuratecy.

    return y;
}

PXInt32U PXAPI PXMathPowerI32U(const PXInt32U base, const PXInt32U exponent)
{
    PXInt32U result = 1;
  
    for(PXInt32U i = 0; i < exponent; ++i)
    {
        result *= base;
    }

    return result;
}

PXInt64U PXAPI PXMathPowerI64U(const PXInt64U base, const PXInt64U exponent)
{
    PXInt64U result = 1;

    for(PXInt64U i = 0; i < exponent; ++i)
    {
        result *= base;
    }

    return result;
}

double PXAPI PXMathPower(double base, double exponent)
{
#if PXStandardLibraryMathUse
    return pow(base, exponent);
#else
    double result = 1.0;
    for (int i = 0; i < exponent; ++i) 
    {
        result *= base;
    }
    return result;
#endif
}

double PXAPI PXMathPowerOfTwo(double base)
{
    return PXMathPower(base, 2);
}

PXInt32U PXAPI PXMathPowerModulo(const PXInt32U base, const PXInt32U exponent, const PXInt32U modulo)
{
    PXInt32U result = 1;

    for(PXInt32U i = 0; i < exponent; ++i)
    {
        result *= base;
        result %= modulo;
    }

    return result;
}

PXF64 PXAPI PXMathRootF64(unsigned int rootNr, const PXF64 value)
{
    switch(rootNr)
    {
        case 0:
            return 0;

        case 1:
            return 0;

        case 2:
            return PXMathRootSquareF64(value);

        case 3:
            return PXMathRootCubic(value);

        default:
            return 0;
    }
}

PXF64 PXAPI PXMathRootSquareF64(const PXF64  value)
{
#if PXStandardLibraryMathUse        
        return sqrt(value);  // TODO: Dependeny problem
#else
    // Newton-Raphson method
    double guess = value / 2.0;
    const double epsilon = 0.00001; // Precision level

#if 0        
    while ((guess * guess - value) > epsilon || (value - guess * guess) > epsilon) 
#else
    for (int i = 0; i < 10; i++) // Fixed iterations
#endif
    {
        guess = (guess + value / guess) / 2.0;
    }

    return guess;
#endif
}

void PXAPI PXMathRootSquareF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;

    if(1)
    {
     

        return; // DONE
    }

    if(1)
    {
   
    }

  
    if(1)
    {
       

        return; // DONE
    }
}

void PXAPI PXMathRootSquareF32VX4(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;

    __m128 simdInput;
    __m128 simdOutput;

    pxWorkSetCounter.BatchSize = 4;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &inputListX[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &outputListY[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF32V(simdInput.m128_f32, extractPoint, workSet);

        simdOutput = _mm_sqrt_ps(simdInput); // SSE, 4x 32-Bit PXF32 SQRT()

        PXMemoryCopyF32V(insertPoint, simdOutput.m128_f32, workSet);
    }
}

void PXAPI PXMathRootSquareF32VX8(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;

    __m256 simdInput;
    __m256 simdOutput;

    pxWorkSetCounter.BatchSize = 8;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &inputListX[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &outputListY[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF32V(simdInput.m256_f32, extractPoint, workSet);

        simdOutput = _mm256_sqrt_ps(simdInput); // AVX, 8x 32-Bit PXF32 SQRT()

        PXMemoryCopyF32V(insertPoint, simdOutput.m256_f32, workSet);
    }

    return; // DONE
}

void PXAPI PXMathRootSquareF32VX16(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;

    __m512 simdInput;
    __m512 simdOutput;

    pxWorkSetCounter.BatchSize = 16;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &inputListX[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &outputListY[i * pxWorkSetCounter.BatchSize];

        int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

        PXMemoryCopyF32V(simdInput.m512_f32, extractPoint, workSet);

        simdOutput = _mm512_sqrt_ps(simdInput); // AVX512F, 16x 32-Bit PXF32 SQRT()

        PXMemoryCopyF32V(insertPoint, simdOutput.m512_f32, workSet);
    }
}

void PXAPI PXMathRootSquareInverseF16VX8(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
    __m128h dataInput;
    __m128h dataOut;

    PXMemoryCopyF16V(dataInput.m128i_u16, inputListX, 8);
     
   // dataOut = _mm_invsqrt_ph(dataInput); // AVX512_FP16
   
    PXMemoryCopyF16V(outputListY, dataOut.m128i_u16, 8);
}

void PXAPI PXMathRootSquareInverseF32VX4(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    __m128 dataInput;
    __m128 dataOut;

    PXMemoryCopyF32V(dataInput.m128_u16, inputListX, 4);

    dataOut = _mm_invsqrt_ps(dataInput); // SSE

    PXMemoryCopyF32V(outputListY, dataOut.m128_u16, 4);
}

void PXAPI PXMathRootSquareInverseF64VX2(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
    __m128d dataInput;
    __m128d dataOut;

    PXMemoryCopyF64V(dataInput.m128d_f64, inputListX, 2);

    dataOut = _mm_invsqrt_pd(dataInput); // SSE

    PXMemoryCopyF64V(outputListY, dataOut.m128d_f64, 2);
}

double PXAPI PXMathRootCubic(const double x)
{
#if PXStandardLibraryMathUse   
    return cbrt(x);
#else
    double guess = x / 3.0;
    double epsilon = 0.00001; // Precision level

    while ((guess * guess * guess - x) > epsilon || (x - guess * guess * guess) > epsilon) 
    {
        guess = (2.0 * guess + x / (guess * guess)) / 3.0;
    }

    return guess;
#endif  
}

void PXAPI PXMathRootCubeInverseF16VX8(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
    __m128h dataInput;
    __m128h dataOut;

    PXMemoryCopyF16V(dataInput.m128i_u16, inputListX, 8);

   // dataOut = _mm_invcbrt_ph(dataInput); // AVX512_FP16

    PXMemoryCopyF16V(outputListY, dataInput.m128i_u16, 8);
}

void PXAPI PXMathRootCubeInverseF32VX4(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    __m128 dataInput;
    __m128 dataOut;

    PXMemoryCopyF32V(dataInput.m128_f32, inputListX, 4);

    __m128 _mm_invcbrt_ps(dataInput); // SSE

    PXMemoryCopyF32V(outputListY, dataInput.m128_f32, 4);
}

void PXAPI PXMathRootCubeInverseF64VX2(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
    __m128d dataInput;
    __m128d dataOut;

    PXMemoryCopyF64V(dataInput.m128d_f64, inputListX, 2);

    __m128d _mm_invcbrt_pd(dataInput); // SSE

    PXMemoryCopyF64V(outputListY, dataInput.m128d_f64, 2);
}

PXF64 PXAPI PXMathRootNF64(const PXInt32U amount, const PXF64 x)
{
    // Newton-Raphson method
    const PXF64 epsilon = 0.00001; // Precision level
    PXF64 guess = x / amount;

    while (PXMathAbsoluteD(PXMathPower(guess, amount) - x) > epsilon)
    {
        guess = ((amount - 1) * guess + x / PXMathPower(guess, amount - 1)) / amount;
    }

    return guess;
}

double PXAPI PXMathPythagoras(double a, double b)
{
#if PXStandardLibraryMathUse
    return hypot(a, b);
#else 
    return PXMathRootSquare(PXMathPowerOfTwo(a) + PXMathPowerOfTwo(b));
#endif
}

double PXAPI PXMathPythagorasReverse(double c, double a)
{
    return 0.0;
}

PXF64 PXAPI PXMathLogarithm(const PXF64 base, const PXF64 exponentx)
{
    return 0;
}

PXF64 PXAPI PXMathLogarithmE(const PXF64 exponent)
{
#if PXStandardLibraryMathUse
    return log(exponent);
#else
    return 0;
#endif
}

PXF64 PXAPI PXMathLogarithm2(const PXF64 exponent)
{
#if PXStandardLibraryMathUse
    return log2(exponent);
#else
    return 0;
#endif
}

PXF64 PXAPI PXMathLogarithm10(const PXF64 exponent)
{
  //  __m128d _mm_log_pd(__m128d a);
  // __m256d _mm256_log_pd(__m256d a);
  //  __m512d _mm512_log_pd(__m512d a);

#if PXStandardLibraryMathUse
    return log10(exponent);  // TODO: Dependeny problem
#else
    return 0;
#endif
}

double PXAPI PXMathLogarithmus(int x, double exponent)
{
#if OSUnix
    return 0; // TODO: not implemented?
#elif OSWindows
    return 0;// _dlog(exponent, base); // TODO: !!!
#endif

    // Taylor series expansion    
    if (x <= 0)
    {
        return -1; // Error: log is undefined for non-positive values
    }

    double result = 0.0;
    double term = (x - 1) / (x + 1);
    double term_squared = term * term;
    double numerator = term;
    int n = 1;

    while (n < 100) 
    { // More iterations for better accuracy
        result += numerator / (2 * n - 1);
        numerator *= term_squared;
        n++;
    }

    return 2 * result;    
}

double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance)
{
    return PXMathConstantGravity * ((massA * massB) / (PXMathPower(distance, 2)));
}

int PXAPI PXMathCeilingF(const PXF32 value)
{
    const int fullNumberBlockInt = (int)value;
    const PXF32 fullNumberBlock = fullNumberBlockInt;
    const PXF32 remaining = value - fullNumberBlock;

    if (remaining)
    {
        return fullNumberBlockInt + 1;
    }

    return fullNumberBlockInt;
}

int PXAPI PXMathCeilingD(const double value)
{
    const int fullNumberBlockInt = (int)value;
    const double fullNumberBlock = fullNumberBlockInt;
    const double remaining = value - fullNumberBlock;

    if (remaining)
    {
        return fullNumberBlockInt + 1;
    }

    return fullNumberBlockInt;
}

void PXAPI PXMathRandomeSeed(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
{
    pxMathRandomGeneratorSeed->X = 123456789;
    pxMathRandomGeneratorSeed->Y = 362436069;
    pxMathRandomGeneratorSeed->Z = 521288629;
}

PXInt32U PXAPI PXMathRandomeNumber(PXMathRandomGeneratorSeed* const pxMathRandomGeneratorSeed)
{
    PXInt32U t;

    pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X << 16;
    pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X >> 5;
    pxMathRandomGeneratorSeed->X ^= pxMathRandomGeneratorSeed->X << 1;

    t = pxMathRandomGeneratorSeed->X;
    pxMathRandomGeneratorSeed->X = pxMathRandomGeneratorSeed->Y;
    pxMathRandomGeneratorSeed->Y = pxMathRandomGeneratorSeed->Z;
    pxMathRandomGeneratorSeed->Z = t ^ pxMathRandomGeneratorSeed->X ^ pxMathRandomGeneratorSeed->Y;

    return pxMathRandomGeneratorSeed->Z;
}

//extern _cdecl PXIntrinsicFSIN(PXF32* val);
//extern _cdecl PXIntrinsicFSQRT(PXF32* val);

PXF32 PXAPI PXMathSinusRADF32(const PXF32 x)
{
    return PXMathSinusRADF64(x);
}

PXF64 PXAPI PXMathSinusRADF64(const PXF64 x)
{
#if PXStandardLibraryMathUse
    return sin(x);

#elif PXIntrinsicUse

    double dx = value;

    PXIntrinsicFSIN(&dx); // sin(value); // TODO: Dependeny problem

    return dx;
#else

    //  Taylor series expansion:
    double term = x;
    double sum = x;
    int n = 1;

    while(term > 0.00001 || term < -0.00001)
    {
        term *= -x * x / (2 * n * (2 * n + 1));
        sum += term;
        n++;
    }

    return sum;

#endif
}

void PXAPI PXMathSinusRADF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
    //__m128h _mm_sin_ph(__m128h a)
}

void PXAPI PXMathSinusRADF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
   // __m128 _mm_sin_ps(__m128 a)
}

void PXAPI PXMathSinusRADF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
  //  __m128d _mm_sin_pd(__m128d a)
}

void PXAPI PXMathSinusDEGF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
   // __m128h _mm_sind_ph(__m128h a)
}

void PXAPI PXMathSinusDEGF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
   // __m128 _mm_sind_ps(__m128 a)
}

void PXAPI PXMathSinusDEGF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
   // __m128d _mm_sind_pd(__m128d a)
}

void PXAPI PXMathSinusHyperbolicRADF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
   // __m128h _mm_sinh_ph(__m128h a);
}

void PXAPI PXMathSinusHyperbolicRADF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
   // __m128 _mm_sinh_ps(__m128 a)
}

void PXAPI PXMathSinusHyperbolicRADF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
   // __m128d _mm_sinh_pd(__m128d a)
}

void PXAPI PXMathCosinusRADF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
   // __m128h _mm_cos_ph(__m128h a)
}

void PXAPI PXMathCosinusRADF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
   // __m128 _mm_cos_ps(__m128 a)
}

void PXAPI PXMathCosinusRADF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
   // __m128d _mm_cos_pd(__m128d a)
}

void PXAPI PXMathSinusAndCosinusRADF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
    //__m128h _mm_sincos_ph(__m128h * mem_addr, __m128h a)
}

void PXAPI PXMathSinusAndCosinusRADF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
   // __m128 _mm_sincos_ps(__m128 * mem_addr, __m128 a)
}

void PXAPI PXMathSinusAndCosinusRADF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
    //__m128d _mm_sincos_pd(__m128d * mem_addr, __m128d a)
}

PXF32 PXAPI PXMathTangensRADF32(const PXF32 x)
{
    return PXMathTangensRADF64(x);
}

PXF64 PXAPI PXMathTangensRADF64(const PXF64 x)
{
#if PXStandardLibraryMathUse
    return tan(x);
#else
    return PXMathSinusD(x) / PXMathCosinusD(x); 
#endif
}

PXF32 PXAPI PXMathCosinusRADF32(const PXF32 x)
{
    return PXMathCosinusRADF64(x);
}

void PXAPI PXMathCosinusDEGF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
   
}

void PXAPI PXMathCosinusDEGF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;
    pxWorkSetCounter.BatchSize = 4;

    PXWorkSetCounterCalc(&pxWorkSetCounter);

    // Full-Batches
    for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
    {
        const PXF32* const extractPoint = &inputListX[i * pxWorkSetCounter.BatchSize];
        PXF32* const insertPoint = &outputListY[i * pxWorkSetCounter.BatchSize];

        PXMathCosinusDEGF32VX1(insertPoint, extractPoint, i);
    }
}

void PXAPI PXMathCosinusDEGF32VX1(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        outputListY[i] = PXMathCosinusRADF32(inputListX[i]);
    }
}

void PXAPI PXMathCosinusDEGF32VX4(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    __m128 input = _mm_load_ps(outputListY);
    __m128 output = _mm_cosd_ps(input); // [Intrinsic] immintrin.h, SSE, 4x 32-Bit PXF32 in DEG -> COS(x)
    _mm_store_ps(inputListX, output);
}

void PXAPI PXMathCosinusDEGF32VX8(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    __m256 input = _mm256_load_ps(outputListY);
    __m256 output = _mm256_cosd_ps(input); // AVX512F, 16x COS() Degrees
    _mm256_store_ps(inputListX, input);
}

void PXAPI PXMathCosinusDEGF32VX16(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    __m512 input = _mm512_load_ps(outputListY);
    __m512 output = _mm512_cosd_ps(input); // AVX512F, 16x COS() Degrees
    _mm512_store_ps(inputListX, input);
}

void PXAPI PXMathCosinusDEGF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
    __m512d input = _mm512_load_pd(outputListY);
    __m512d output = _mm512_cosd_pd(input); // AVX512F, 16x COS() Degrees
    _mm512_store_pd(inputListX, input);
}

void PXAPI PXMathCosinusHyperbolicRADF16V(PXF16* const outputListY, const PXF16* const inputListX, const PXSize amount)
{
  //  __m128h _mm_cos_ph(__m128h a)
}

void PXAPI PXMathCosinusHyperbolicRADF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
 
}

void PXAPI PXMathCosinusHyperbolicRADF64V(PXF64* const outputListY, const PXF64* const inputListX, const PXSize amount)
{
  
}

PXF64 PXAPI PXMathCosinusRADF64(const PXF64 x)
{
#if PXStandardLibraryMathUse
    return cos(x);
#else
    double term = 1.0;
    double sum = 1.0;
    double x_squared = x * x;
    int n = 1;

    while (term > 0.00001 || term < -0.00001) 
    {
        term *= -x_squared / (2 * n * (2 * n - 1));
        sum += term;
        n++;
    }

    return sum;
#endif
}

double PXAPI PXMathHyperbolicSinus(const double x)
{
#if PXStandardLibraryMathUse
    return sinh(x);
#else
     double ex = PXExponential(x);
    double e_minus_x = PXExponential(-x);
    return (ex - e_minus_x) / 2.0;
#endif
}

double PXAPI PXMathArcusSinus(const double x)
{
#if PXStandardLibraryMathUse
    return asin(x);
#else
    if (x < -1.0 || x > 1.0) 
    {
        return -1; // Error: asin is undefined for values outside [-1, 1]
    }

    double result = x;
    double term = x;
    double x_squared = x * x;
    int n = 1;

    while (term > 0.00001 || term < -0.00001) 
    {
        term *= x_squared * (2 * n - 1) / (2 * n + 1);
        result += term / (2 * n + 1);
        n++;
    }

    return result;
#endif
}

double PXAPI PXMathHyperbolicCosinus(const double x)
{
#if PXStandardLibraryMathUse 
    return cosh(x);
#else
    double ex = PXExponential(x);
    double e_minus_x = PXExponential(-x);
    return (ex + e_minus_x) / 2.0;
#endif 
}

double PXAPI PXMathHyperbolicTangens(const double x)
{
#if PXStandardLibraryMathUse  
    return tanh(x);
#else
    double ex = PXExponential(x);
    double e_minus_x = PXExponential(-x);
    return (ex - e_minus_x) / (ex + e_minus_x);
#endif
}

double PXAPI PXMathArcusTangens(const double x)
{
#if PXStandardLibraryMathUse
    return atan(x);
#else
    double result = 0.0;
    double term = x;
    double x_squared = x * x;
    int n = 1;

    while (term > 0.00001 || term < -0.00001) 
    {
        result += term / (2 * n - 1);
        term *= -x_squared;
        n++;
    }

    return result;
#endif    
}


#define PXQuickSwap(a, b) \
a = a + b; \
b = a - b; \
a = a - b;



void PXAPI PXMathShuffleI8(const PXInt8U* const input, PXInt8U* const output, const PXSize amount, PXInt8U* mask, const PXInt8U maskAmount)
{ 
    PXSize i = 0;

    __m512i shuffleMask;
  
    // Build
    for(PXInt8U j = 0; j < 64; ++j)
    {
        shuffleMask.m512i_u8[j] = (j / maskAmount)* maskAmount + mask[j % maskAmount];
    }

#if 0
    for(; i + 64 <= amount; i += 64)
    {
        __m512i bgrChunk = _mm512_loadu_epi8(&input[i]);
        __m512i rgbChunk = _mm512_shuffle_epi8(bgrChunk, shuffleMask);

        _mm512_storeu_epi8(&output[i], rgbChunk);
    }
#endif

#if 0
    for(size_t j = i; j < amount; ++j)
    {
        output[j] = input[j + shuffleMask.m512i_u8[j % maskAmount]];
    }
#endif
}

// Arctangent Function
double PXAPI PXMathArcusTangens2(double x, double y)
{
#if PXStandardLibraryMathUse
    return atan2(y, x);
#else
    if(x > 0)
    {
        return PXMathArcusTangens(y / x);
    }
    else if(x < 0 && y >= 0)
    {
        return PXMathArcusTangens(y / x) + PXMathConstantPID;
    }
    else if(x < 0 && y < 0)
    {
        return PXMathArcusTangens(y / x) - PXMathConstantPID;
    }
    else if(x == 0 && y > 0)
    {
        return PXMathConstantPID / 2.0f;
    }
    else if(x == 0 && y < 0)
    {
        return -PXMathConstantPID / 2.0f;
    }
    else
    {
        return 0; // Undefined for (0, 0)
    }
#endif    
}

double PXAPI PXExponential(const double x) 
{
    // Taylor series    
    double result = 1.0; // Initialize result to 1 (the first term of the series)
    double term = 1.0;   // Initialize the first term to 1
    int n = 1;

    // Continue adding terms until the term is very small
    while (term > 0.00001 || term < -0.00001) 
    {
        term *= x / n;   // Calculate the next term in the series
        result += term;  // Add the term to the result
        n++;
    }

    return result;
}

unsigned long PXAPI PXMathFibonacci(unsigned long step)
{
    double rootOfFive = PXMathRootSquareF64(5);

    if(step < 2)
    {
        return step;
    }
    else
    {
        // (1 + W/5)^n - (1 - W/5)^n
        // -------------------------
        //         2^n * W/5

        double a = PXMathPower(1 + rootOfFive, step);
        double b = PXMathPower(1 - rootOfFive, step);
        double c = PXMathPower(2, step) * rootOfFive;

        return ((a - b) / c);

        // return FibonacciRetardVersion(step -1) + FibonacciRetardVersion(step -2); Recusive version
    }
}

PXF64 PXAPI PXMathRadiansToDegree(const PXF64  radians)
{
    return (180.0 / PXMathConstantPI) * radians;
}

PXF64 PXAPI PXMathDegreeToRadians(const PXF64  degree)
{
    return (PXMathConstantPI / 180.0) * degree;
}

PXF32 PXAPI PXMathLiniarF(const PXF32 yMinimum, const PXF32 yMaximum, const PXF32 xMinimum, const PXF32 xMaximum, const PXF32 xValue)
{
    // Cap Value
    PXF32 xCap = xValue > xMaximum ? xMaximum : xValue;
    xCap = xValue < xMinimum ? xMinimum : xValue;

    // ((ydelta * xValueDelta) / xdelta) + yMinimum;

    return (((yMaximum - yMinimum) * (xValue - xMinimum)) / (xMaximum - xMinimum)) + yMinimum;
}

PXF32 PXAPI PXMathNormalizeF(const PXF32 minimum, const PXF32 maximum, const PXF32 value)
{
    return PXMathLiniarF(0, 1, minimum, maximum, value);
}

unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const PXF32 minimum, const PXF32 maximum, const PXF32 value)
{
    const PXF32 result = PXMathLiniarF(0, 255, minimum, maximum, value);
    const unsigned int convertedResult = (unsigned int)(result);

    return convertedResult;
}

void PXAPI PXMathFormulaQuadratic(const PXF32 a, const PXF32 b, const PXF32 c, PXF32* const resultA, PXF32* const resultB)
{
    const PXF32 bNegativ = -b;
    const PXF32 dividor = 2.0f * a;
    const PXF32 rootData = b * b - 4.0f * a * c;
    const PXF32 rootResult = PXMathRootSquareF64(rootData);

    *resultA = (bNegativ + rootResult) / dividor;
    *resultB = (bNegativ - rootResult) / dividor;
}

void PXAPI PXMathFormulaPQ(const PXF32 p, const PXF32 q, PXF32* const resultA, PXF32* const resultB)
{
    const PXF32 pHalf = p / 2.0f;
    const PXF32 pHalfNeg = -pHalf;
    const PXF32 rootData = pHalf * pHalf - q;
    const PXF32 rootResult = PXMathRootSquareF64(rootData);

    *resultA = pHalfNeg + rootResult;
    *resultB = pHalfNeg - rootResult;
}

// Horner's Method 
// Reform  2x^3 - 6x^2 + 2x - 1 into 
// double coefficientList[] = {2, -6, 2, -1}; // Each element is a a*x^b
// int degree = 3; // The size of the list -1. 
// double x = 3.0; // The value for x in the term
double PXAPI PXMathHornerD(double* const coefficientList, const PXInt32U degree, const double x)
{
    double result = coefficientList[0];
    
    for (int i = 1; i <= degree; ++i) 
    {
        result = result * x + coefficientList[i];
    }
    
    return result;
}

void PXAPI PXFastFourierTransform()
{
    // TODO: implement
}

// fmod
