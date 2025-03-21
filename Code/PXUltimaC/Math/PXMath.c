#include "PXMath.h"

#include <math.h>

#include <immintrin.h>
#include <wmmintrin.h>
#include <mmintrin.h>

#if OSUnix
#elif OSWindows
#include <intrin.h>
#endif

#include <OS/Memory/PXMemory.h>

#define PXStandardLibraryMathUse 1

const float PXMathConstantPIF = PXMathConstantPI;
const double PXMathConstantPID = PXMathConstantPI;

PXMath GLOBALPXMath;

void PXAPI PXMathIntrinsicInit()
{
    

  
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
        //__m256i res = _mm512_mask_cvtps_ph(); // [Intrinsic] AVX512F, vcvtps2ph, 16x 32-Bit float -> 16-Bit float

        __m256i simdInput;
        __m512 simdOutput;

        pxWorkSetCounter.BatchSize = 16;

        PXWorkSetCounterCalc(&pxWorkSetCounter);

        // Full-Batches
        for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
        {
            const PXF16* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
            PXF32* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

            int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

            for(char i = 0; i < workSet; ++i)
            {
                simdInput.m256i_u16[i] = extractPoint[i];
            }

            simdOutput = _mm512_cvtph_ps(simdInput);

            for(char i = 0; i < workSet; ++i)
            {
                insertPoint[i] = simdOutput.m512_f32[i];
            }
        }

        return; // DONE
    }

    if(1) // AVX support, 
    {
        // __m128i _mm256_cvtps_ph (__m256 a, int imm8)
        //__m128i resB = _mm256_cvtps_ph(); // [Intrinsic] immintrin.h, F16C, vcvtps2ph, 8x 32-Bit float -> 16-Bit float

        pxWorkSetCounter.BatchSize = 4;

        PXWorkSetCounterCalc(&pxWorkSetCounter);

        __m128i simdInput;
        __m128 simdOutput;

        // Full-Batches
        for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
        {
            const PXF16* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
            PXF32* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

            int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

            for(PXSize i = 0; i < workSet; ++i)
            {
                simdInput.m128i_u16[i] = extractPoint[i];
            }

            simdOutput = _mm_cvtph_ps(simdInput); // [Intrinsic] 4x 16-Bit float -> 32-Bit float

            for(PXSize i = 0; i < workSet; ++i)
            {
                insertPoint[i] = simdOutput.m128_f32[i];
            }
        }

        return; // DONE
    }

    // non-intrinsic
    // ...
}

void PXAPI PXMathF32ToF16(PXF16* const listOut, const PXF32* const listInput, const PXSize inputAmount)
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
        __m512 in;
        __m256i res;

        pxWorkSetCounter.BatchSize = 16;

        PXWorkSetCounterCalc(&pxWorkSetCounter);

        for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
        {
            const PXF32* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
            PXF16* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

            int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

            for(PXSize i = 0; i < workSet; ++i)
            {
                in.m512_f32[i] = extractPoint[i];
            }

            res = _mm512_cvtps_ph(in, _MM_FROUND_TO_NEAREST_INT); // [Intrinsic] AVX512F, vcvtps2ph, 16x 32-Bit float -> 16-Bit float

            for(PXSize i = 0; i < workSet; ++i)
            {
                insertPoint[i] = res.m256i_u16[i];
            }
        }

        return; // DONE
    }


    if(1) // AVX support, 
    {
        // __m128i _mm256_cvtps_ph (__m256 a, int imm8)
        //__m128i resB = _mm256_cvtps_ph(); // [Intrinsic] immintrin.h, F16C, vcvtps2ph, 8x 32-Bit float -> 16-Bit float

        __m128 data;
        __m128i res;

        pxWorkSetCounter.BatchSize = 4;

        PXWorkSetCounterCalc(&pxWorkSetCounter);

        for(PXSize i = 0; i < pxWorkSetCounter.AmountBatchFull + 1; ++i)
        {
            const PXF32* const extractPoint = &listInput[i * pxWorkSetCounter.BatchSize];
            PXF16* const insertPoint = &listOut[i * pxWorkSetCounter.BatchSize];

            int workSet = PXWorkSetCounterPull(&pxWorkSetCounter, i);

            for(PXSize i = 0; i < workSet; ++i)
            {
                data.m128_f32[i] = extractPoint[i];
            }

            res = _mm_cvtps_ph(data, _MM_FROUND_TO_NEAREST_INT); // [Intrinsic] 4x 32-Bit float -> 16-Bit float

            for(PXSize i = 0; i < workSet; ++i)
            {
                insertPoint[i] = res.m128i_u16[i];
            }
        }

        return; // DONE
    }

    // non-intrinsic
    // ...
}

float PXAPI PXMathFastInverseSqaureRoot(float number)
{
    const float threehalfs = 1.5f;
    float x2 = number * 0.5f;
    float y = number;
    long i;

    i = *(long*)&y; // Convert the Binary Value from a float to an long. No conversion! A 1:1 Bitcopy. IEEE 574 standard
    i = 0x5f3759df - (i >> 1); // .The bit shift Halfs i. AProximation of (1/root(y))
    y = *(float*)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    // last line can be repeatet for more acuratecy.

    return y;
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

double PXAPI PXMathRoot(unsigned int rootNr, double value)
{
    switch(rootNr)
    {
        case 0:
            return 0;

        case 1:
            return 0;

        case 2:
            return PXMathRootSquare(value);

        case 3:
            return PXMathRootCubic(value);

        default:
            return 0;
    }
}

double PXAPI PXMathRootSquare(double value)
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

            simdOutput = _mm512_sqrt_ps(simdInput); // AVX512F, 16x 32-Bit float SQRT()

            PXMemoryCopyF32V(insertPoint, simdOutput.m512_f32, workSet);
        }

        return; // DONE
    }

    if(1)
    {
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

            simdOutput = _mm256_sqrt_ps(simdInput); // AVX, 8x 32-Bit float SQRT()

            PXMemoryCopyF32V(insertPoint, simdOutput.m256_f32, workSet);
        }

        return; // DONE
    }

  
    if(1)
    {
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

            simdOutput = _mm_sqrt_ps(simdInput); // SSE, 4x 32-Bit float SQRT()

            PXMemoryCopyF32V(insertPoint, simdOutput.m128_f32, workSet);
        }

        return; // DONE
    }
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

// Newton-Raphson method
double PXAPI PXMathRootN(double x, int amount)
{
    double guess = x / amount;
    double epsilon = 0.00001; // Precision level

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

double PXAPI PXMathLogarithmusBase2(double exponent)
{
#if PXStandardLibraryMathUse
    return log2(exponent);
#else
    return 0;
#endif
}

double PXAPI PXMathLogarithmusBase10(double exponent)
{
#if PXStandardLibraryMathUse
    return log10(exponent);  // TODO: Dependeny problem
#else
    return 0;
#endif
}

double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance)
{
    return PXMathConstantGravity * ((massA * massB) / (PXMathPower(distance, 2)));
}

int PXAPI PXMathCeilingF(const float value)
{
    const int fullNumberBlockInt = (int)value;
    const float fullNumberBlock = fullNumberBlockInt;
    const float remaining = value - fullNumberBlock;

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

//extern _cdecl PXIntrinsicFSIN(float* val);
//extern _cdecl PXIntrinsicFSQRT(float* val);

float PXAPI PXMathSinusF(const float x)
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

    while (term > 0.00001 || term < -0.00001) 
    {
        term *= -x * x / (2 * n * (2 * n + 1));
        sum += term;
        n++;
    }

    return sum;
    
#endif
}

double PXAPI PXMathSinusD(const double value)
{
    return PXMathSinusF(value); // TODO: precision loss!
}

double PXAPI PXMathTangensF(const float x)
{
    return PXMathTangensD(x);
}

double PXAPI PXMathTangensD(const double x)
{
#if PXStandardLibraryMathUse
    return tan(x);
#else
    return PXMathSinusD(x) / PXMathCosinusD(x); 
#endif
}

float PXAPI PXMathCosinusF(const float x)
{
    return PXMathCosinusD(x);
}

void PXAPI PXMathCosinusDEGF32V(PXF32* const outputListY, const PXF32* const inputListX, const PXSize amount)
{
    PXWorkSetCounter pxWorkSetCounter;
    pxWorkSetCounter.WorkToDo = amount;

    if(1)
    {
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

            simdOutput = _mm512_cosd_ps(simdInput); // AVX512F, 

            PXMemoryCopyF32V(insertPoint, simdOutput.m512_f32, workSet);
        }

        return; // DONE
    }

    if(1)
    {
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

            simdOutput = _mm256_cosd_ps(simdInput); // [Intrinsic] AVX

            PXMemoryCopyF32V(insertPoint, simdOutput.m256_f32, workSet);
        }

        return; // DONE
    }

    if(1)
    {
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

            simdOutput = _mm_cosd_ps(simdInput); // [Intrinsic] immintrin.h, SSE, 4x 32-Bit float in DEG -> COS(x)

            PXMemoryCopyF32V(insertPoint, simdOutput.m128_f32, workSet);
        }

        return; // DONE
    }

    // No intrinsic
}

double PXAPI PXMathCosinusD(const double x)
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
    double rootOfFive = PXMathRootSquare(5);

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

double PXAPI PXMathRadiansToDegree(double radians)
{
    return radians * (180.0 / PXMathConstantPI);
}

double PXAPI PXMathDegreeToRadians(double degree)
{
    return degree * (PXMathConstantPI / 180.0);
}

float PXAPI PXMathLiniarF(const float yMinimum, const float yMaximum, const float xMinimum, const float xMaximum, const float xValue)
{
    // Cap Value
    float xCap = xValue > xMaximum ? xMaximum : xValue;
    xCap = xValue < xMinimum ? xMinimum : xValue;

    // ((ydelta * xValueDelta) / xdelta) + yMinimum;

    return (((yMaximum - yMinimum) * (xValue - xMinimum)) / (xMaximum - xMinimum)) + yMinimum;
}

float PXAPI PXMathNormalizeF(const float minimum, const float maximum, const float value)
{
    return PXMathLiniarF(0, 1, minimum, maximum, value);
}

unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const float minimum, const float maximum, const float value)
{
    const float result = PXMathLiniarF(0, 255, minimum, maximum, value);
    const unsigned int convertedResult = (unsigned int)(result);

    return convertedResult;
}

void PXAPI PXMathFormulaQuadratic(const float a, const float b, const float c, float* const resultA, float* const resultB)
{
    const float bNegativ = -b;
    const float dividor = 2.0f * a;
    const float rootData = b * b - 4.0f * a * c;
    const float rootResult = PXMathRootSquare(rootData);

    *resultA = (bNegativ + rootResult) / dividor;
    *resultB = (bNegativ - rootResult) / dividor;
}

void PXAPI PXMathFormulaPQ(const float p, const float q, float* const resultA, float* const resultB)
{
    const float pHalf = p / 2.0f;
    const float pHalfNeg = -pHalf;
    const float rootData = pHalf * pHalf - q;
    const float rootResult = PXMathRootSquare(rootData);

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
