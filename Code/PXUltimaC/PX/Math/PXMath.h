#pragma once

#ifndef PXMathIncluded
#define PXMathIncluded

#include <PX/Media/PXType.h>

//---<Constants>---------------------------------------------------------------
PXPublic const PXF64 PXMathPI;  // Pi, Archimedes' constant or Ludolph's number
PXPublic const PXF64 PXMathE;
PXPublic const PXF64 PXMathP;
PXPublic const PXF64 PXMathA;
PXPublic const PXF64 PXMathGravity;
//-----------------------------------------------------------------------------

//---<Makro Functions>---------------------------------------------------------
#define PXMathIsInRange(v, minimum, maximum) (minimum <= v && v <= maximum)
#define PXMathMinimum(a, b) ((a < b) * a + (a >= b) * b)
#define PXMathMaximum(a, b) ((a > b) * a + (a <= b) * b)
#define PXMathLimit(x, low, high)  (low*(x <= low) + high*(x >= high) + x *((x > low) && (x < high)))
#define PXMathFloor(a) ((int)(a))
//#define PXMathCeiling(a)


//#define PXMathAbsolute(a) (a * ((a >= 0) - (a < 0)))
#define PXMathAbsolute(x) (((x) > 0) ? (x) : -(x))

//-----------------------------------------------------------------------------

typedef struct PXMathRandomGeneratorSeed_
{
    PXI32U X;
    PXI32U Y;
    PXI32U Z;
}
PXMathRandomGeneratorSeed;


typedef void (PXAPI* PXSIMDFunction)();

typedef void (PXAPI* PXSIMDF16ToU16Function)(PXF16 PXREF listOut, const PXI16U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDF32ToU32Function)(PXF32 PXREF listOut, const PXI32U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDF64ToU64Function)(PXF64 PXREF listOut, const PXI64U PXREF listInput, const PXSize inputAmount);

typedef void (PXAPI* PXSIMDI16ToU16Function)(PXI16S PXREF listOut, const PXI16U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDI32ToU32Function)(PXI32S PXREF listOut, const PXI32U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDI64ToU64Function)(PXI64S PXREF listOut, const PXI64U PXREF listInput, const PXSize inputAmount);

typedef void (PXAPI* PXSIMDU8Function)(PXI8U PXREF listOut, const PXI8U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDU16Function)(PXI16U PXREF listOut, const PXI16U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDU32Function)(PXI32U PXREF listOut, const PXI32U PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDU64Function)(PXI64U PXREF listOut, const PXI64U PXREF listInput, const PXSize inputAmount);

typedef void (PXAPI* PXSIMDF16Function)(PXF16 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDF32Function)(PXF32 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDF64Function)(PXF64 PXREF listOut, const PXF64 PXREF listInput, const PXSize inputAmount);

typedef void (PXAPI* PXSIMDF16ToF32Function)(PXF32 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);
typedef void (PXAPI* PXSIMDF32ToF16Function)(PXF16 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);


// Global object, single instance. 
// [Warning] Needs to be inizilized once 
// as it uses intrinsics like CPUID to redirect 
// functions if availible
typedef struct PXMath_
{
    // abs()
    PXSIMDI16ToU16Function AbsoluteI16ToU16;
    PXSIMDI32ToU32Function AbsoluteI32ToU32;
    PXSIMDI64ToU64Function AbsoluteI64ToU64;

    // avg()

    // ceil()
    PXSIMDF16ToU16Function CeilingF16ToU16;
    PXSIMDF32ToU32Function CeilingF32ToU32;
    PXSIMDF64ToU64Function CeilingF64ToU64;

    // floor()
    PXSIMDF16ToU16Function FloorF16ToU16;
    PXSIMDF32ToU32Function FloorF32ToU32;
    PXSIMDF64ToU64Function FloorF64ToU64;

    // max()

    // min()


    // normal cumulative distribution


      // normal inverse cumulative distribution

    // error function




    // Bit logic
    // and

    // nand()

    // or
    
    // xor






    

    PXSIMDF16Function SinusF16;
    PXSIMDF32Function SinusF32;
    PXSIMDF64Function SinusF64;

    PXSIMDF16Function CosinusF16;
    PXSIMDF32Function CosinusF32;
    PXSIMDF64Function CosinusF64;

    PXSIMDF16Function TangensF16;
    PXSIMDF32Function TangensF32;
    PXSIMDF64Function TangensF64;

    PXSIMDF16Function RootSquareF16;
    PXSIMDF32Function RootSquareF32;
    PXSIMDF64Function RootSquareF64;

    PXSIMDF16Function RootSquareInverseF16;
    PXSIMDF32Function RootSquareInverseF32;
    PXSIMDF64Function RootSquareInverseF64;

    PXSIMDF16Function RootCubeF16;
    PXSIMDF32Function RootCubeF32;
    PXSIMDF64Function RootCubeF64;

    PXSIMDF16Function RootCubeInverseF16;
    PXSIMDF32Function RootCubeInverseF32;
    PXSIMDF64Function RootCubeInverseF64;



    PXSIMDU8Function  CRCU8;
    PXSIMDU16Function CRCU16;
    PXSIMDU32Function CRCU32;
    PXSIMDU64Function CRCU64;

    // Matrix4x4
    PXSIMDFunction Matrix4x4Transpose;

    /*

        // Selective bit moving
        BitScatterDeposit
        BitGatherExtract
        Movemask
        ExtraBits

        // Bit masking
        ZeroHighBits
        ResteLowestBit
        MaskUpToLowestBit
        FindLowestBit




        // Trigeometric

    // Specials
        Algorithms
        AESKeyGenAssist
        AESInverseMMixColums
        AESEncypt
        AESDecrypt
        CRC32

        // Miscellaneous
        Pause
        MonitorMemory
        MonitorWait
        GetMXCSRRegister

        // TransactionalMemory
        TransactionBegin
        TransactionCommit
        TransactionAbort
        */
}
PXMath;

// This function checks what code can be 
// executed and what not.
// Flags: Allow defined range what should be used
// ALL to use everything availibe
PXPublic void PXAPI PXMathIntrinsicInit();



//---------------------------------------------------------
// Range - 
//---------------------------------------------------------
PXPublic PXBool PXAPI PXMathAdressInRange(const void PXREF dataAdress, const PXSize dataAdressSize, const void* target);



//---------------------------------------------------------
// Minimum - Smaler of two numbers
//---------------------------------------------------------
typedef struct PXMatrix4x4F_ PXMatrix4x4F;

PXPublic void PXAPI PXMathMatrix4x4TransposeS(PXMatrix4x4F PXREF pxMatrix4x4);
PXPublic void PXAPI PXMathMatrix4x4TransposeX(PXMatrix4x4F PXREF pxMatrix4x4);


//---------------------------------------------------------
// Minimum - Smaler of two numbers
//---------------------------------------------------------
PXPublic int PXAPI PXMathMinimumI(const int a, const int b);
PXPublic PXSize PXAPI PXMathMinimumIU(const PXSize a, const PXSize b);
PXPublic double PXAPI PXMathMinimumD(const double a, const double b);
//---------------------------------------------------------


//---------------------------------------------------------
// Maximum - Bigger of two numbers
//---------------------------------------------------------
PXPublic PXSize PXAPI PXMathMaximumIU(const PXSize a, const PXSize b);
//---------------------------------------------------------


//---------------------------------------------------------
// Floor - Round down to the next whole number
//---------------------------------------------------------
PXPublic int PXAPI PXMathFloorI(const int a);
PXPublic int PXAPI PXMathFloorD(const double a);
//---------------------------------------------------------


//---------------------------------------------------------
// Absolute - Convert to positive
//---------------------------------------------------------
PXPublic PXI16U PXAPI PXMathAbsoluteI16(const PXI16S value);
PXPublic PXI32U PXAPI PXMathAbsoluteI32(const PXI32S value);
PXPublic PXF32 PXAPI PXMathAbsoluteF(const PXF32 value);
PXPublic PXF64 PXAPI PXMathAbsoluteD(const PXF64 value);
//---------------------------------------------------------


//---------------------------------------------------------
// Ceiling - Round to the next whole number
//---------------------------------------------------------
PXPublic int PXAPI PXMathCeilingF(const PXF32 value);
PXPublic int PXAPI PXMathCeilingD(const double value);
//---------------------------------------------------------


//---------------------------------------------------------
PXPublic PXF32 PXAPI PXMathClampF(const PXF32 value, const PXF32 minimum, const PXF32 maximum);
PXPublic char PXAPI PXMathLimitC(const char value, const char minimum, const char maximum);
PXPublic unsigned char PXAPI PXMathLimitCU(const unsigned char  value, const unsigned char  minimum, const unsigned char maximum);
PXPublic int PXAPI PXMathLimitI(const int value, const int minimum, const int maximum);
PXPublic PXSize PXAPI PXMathLimitIU(const PXSize value, const PXSize minimum, const PXSize maximum);
//---------------------------------------------------------

PXPublic PXF32 PXAPI PXMathCopysignf(const PXF32 a, const PXF32 b);


//---------------------------------------------------------
// DEG <=> RAD
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathRadiansToDegree(const PXF64 radians);
PXPublic PXF64 PXAPI PXMathDegreeToRadians(const PXF64 degree);
//---------------------------------------------------------


//---------------------------------------------------------
// 16-Bit - PXF32 - Math
//---------------------------------------------------------
PXPublic void PXAPI PXMathF16ToF32(PXF32 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF16ToF32X4(PXF32 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF16ToF32X8(PXF32 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF16ToF32X16(PXF32 PXREF listOut, const PXF16 PXREF listInput, const PXSize inputAmount);

PXPublic void PXAPI PXMathF32ToF16(PXF16 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF32ToF16X4(PXF16 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF32ToF16X8(PXF16 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);
PXPublic void PXAPI PXMathF32ToF16X16(PXF16 PXREF listOut, const PXF32 PXREF listInput, const PXSize inputAmount);
//---------------------------------------------------------



//---------------------------------------------------------
// Logarithmus
//---------------------------------------------------------
PXPublic double PXAPI PXMathLogarithmus(int base, double exponent);
//---------------------------------------------------------



//---------------------------------------------------------
// Power
//---------------------------------------------------------
PXPublic PXI32U PXAPI PXMathPowerI32U(const PXI32U base, const PXI32U exponent);
PXPublic PXI64U PXAPI PXMathPowerI64U(const PXI64U base, const PXI64U exponent);

PXPublic double PXAPI PXMathPower(double base, double exponent);
PXPublic double PXAPI PXMathPowerOfTwo(double base);
PXPublic PXI32U PXAPI PXMathPowerModulo(const PXI32U base, const PXI32U exponent, const PXI32U modulo);
//---------------------------------------------------------


//---------------------------------------------------------
// Root
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathRootNF64(const PXI32U amount, const PXF64 x);

PXPublic PXF64 PXAPI PXMathRootF64(unsigned int rootNr, const PXF64 value);

PXPublic PXF32 PXAPI PXMathRootSquareF32(const PXF32 value);
PXPublic PXF64 PXAPI PXMathRootSquareF64(const PXF64 value);
PXPublic void PXAPI PXMathRootSquareF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootSquareF32VX4(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootSquareF32VX8(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootSquareF32VX16(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathRootSquareInverseF16VX8(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootSquareInverseF32VX4(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootSquareInverseF64VX2(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);


PXPublic double PXAPI PXMathRootCubic(double value);

PXPublic void PXAPI PXMathRootCubeInverseF16VX8(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootCubeInverseF32VX4(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathRootCubeInverseF64VX2(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);
//---------------------------------------------------------



//---------------------------------------------------------
// Sinus
//---------------------------------------------------------
PXPublic PXF32 PXAPI PXMathSinusRADF32(const PXF32 x);
PXPublic PXF64 PXAPI PXMathSinusRADF64(const PXF64 x);
PXPublic PXF32 PXAPI PXMathSinusDEGF32(const PXF32 x);
PXPublic PXF64 PXAPI PXMathSinusDEGF64(const PXF64 x);

PXPublic void PXAPI PXMathSinusRADF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusRADF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusRADF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathSinusDEGF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusDEGF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusDEGF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathSinusHyperbolicRADF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusHyperbolicRADF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathSinusHyperbolicRADF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Argus Sinus
//-----------------------------------------------------------------------------
PXPublic double PXAPI PXMathArcusSinus(const double x);
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Hyperbolic sinus
//-----------------------------------------------------------------------------
PXPublic double PXAPI PXMathHyperbolicSinus(const double x);


//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Cosinus
//-----------------------------------------------------------------------------
PXPublic PXF32 PXAPI PXMathCosinusRADF32(const PXF32 x);
PXPublic PXF64 PXAPI PXMathCosinusRADF64(const PXF64 x);
PXPublic PXF32 PXAPI PXMathCosinusDEGF32(const PXF32 x);
PXPublic PXF64 PXAPI PXMathCosinusDEGF64(const PXF64 x);



PXPublic void PXAPI PXMathCosinusRADF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusRADF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusRADF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathCosinusDEGF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathCosinusDEGF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusDEGF32VX1(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusDEGF32VX4(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusDEGF32VX8(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusDEGF32VX16(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathCosinusDEGF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);

PXPublic void PXAPI PXMathCosinusHyperbolicRADF16V(PXF16 PXREF outputListY, const PXF16 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusHyperbolicRADF32V(PXF32 PXREF outputListY, const PXF32 PXREF inputListX, const PXSize amount);
PXPublic void PXAPI PXMathCosinusHyperbolicRADF64V(PXF64 PXREF outputListY, const PXF64 PXREF inputListX, const PXSize amount);
//-----------------------------------------------------------------------------


PXPublic PXF32 PXAPI PXMathArcusCosinusF32(const PXF32 x);

PXPublic double PXAPI PXMathHyperbolicCosinus(const double x);




//-----------------------------------------------------------------------------
// Sinus & Cosinus Combo
//-----------------------------------------------------------------------------
PXPublic void PXAPI PXMathSinCosRADF32(const PXF32 angle, PXF32 PXREF valueSIN, PXF32 PXREF valueCOS);

PXPublic void PXAPI PXMathSinCosRADF16V(PXF16 PXREF data, const PXSize amount);
PXPublic void PXAPI PXMathSinCosRADF32V(PXF32 PXREF data, const PXSize amount);
PXPublic void PXAPI PXMathSinCosRADF64V(PXF64 PXREF data, const PXSize amount);
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Tangens
//-----------------------------------------------------------------------------
PXPublic PXF32 PXAPI PXMathTangensRADF32(const PXF32 x);
PXPublic PXF64 PXAPI PXMathTangensRADF64(const PXF64 x);
//-----------------------------------------------------------------------------


//---------------------------------------------------------
// Arcus Tangens
//---------------------------------------------------------
PXPublic double PXAPI PXMathArcusTangens(const double x);
//---------------------------------------------------------


//---------------------------------------------------------
// Hyperbolic
//---------------------------------------------------------
// tanh(x)= (e^x + e^(−x)) / (e^x − e^(−x))
PXPublic double PXAPI PXMathHyperbolicTangens(const double x);
//---------------------------------------------------------








PXPublic double PXAPI PXMathPythagoras(double a, double b);
PXPublic double PXAPI PXMathPythagorasReverse(double c, double a);
//---------------------------------------------------------


//---------------------------------------------------------
// log() natural
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathLogarithm(const PXF64 base, const PXF64 exponent);
//---------------------------------------------------------


//---------------------------------------------------------
// log() natural
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathLogarithmE(const PXF64 exponent);
//---------------------------------------------------------



//---------------------------------------------------------
// log2()
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathLogarithm2(const PXF64 exponent);
//---------------------------------------------------------



//---------------------------------------------------------
// log10()
//---------------------------------------------------------
PXPublic PXF64 PXAPI PXMathLogarithm10(const PXF64 exponent);
//---------------------------------------------------------



//---------------------------------------------------------
// Shuffle
//---------------------------------------------------------
PXPublic void PXAPI PXMathShuffleI8
(
    const PXI8U PXREF input,
    PXI8U PXREF output,
    const PXSize amount,
    const PXI8U* mask, 
    const PXI8U maskAmount
);

//---------------------------------------------------------











//---------------------------------------------------------
// Interpolation
//---------------------------------------------------------

// [Imprecise], does not guarantee v=v1 when t=1
// -> floating-point arithmetic error.
// May use fused multiply-add instruction.
PXPublic PXF32 PXAPI PXMathLerpF32L(const PXF32 v0, const PXF32 v1, const PXF32 t);

// [Precise], guarantees v=v1 when t=1.
// This method is monotonic only when v0 * v1 < 0.
PXPublic PXF32 PXAPI PXMathLerpF32H(const PXF32 v0, const PXF32 v1, const PXF32 t);
//---------------------------------------------------------



// Tangens inverse that can handle the values from all the 4 different quadrants
//PXPublic double PXAPI PXMathTangensArc2(double x, double y);
PXPublic double PXAPI PXMathArcusTangens2(const double x, const double y);



// exp()
PXPublic double PXAPI PXExponential(const double x);


PXPublic unsigned long PXAPI PXMathFibonacci(unsigned long step);

PXPublic PXF32 PXAPI PXMathFastInverseSqaureRoot(PXF32 number);

PXPublic double PXAPI PXMathNewtonGravitation(double massA, double massB, double distance);

PXPublic void PXAPI PXMathRandomeSeed(PXMathRandomGeneratorSeed PXREF pxMathRandomGeneratorSeed);
PXPublic PXI32U PXAPI PXMathRandomeNumber(PXMathRandomGeneratorSeed PXREF pxMathRandomGeneratorSeed);





// Liniar interpolate a value from the range of 'x' min and max to another range 'y'.
PXPublic PXF32 PXAPI PXMathLiniarF(const PXF32 yMinimum, const PXF32 yMaximum, const PXF32 xMinimum, const PXF32 xMaximum, const PXF32 xValue);
PXPublic PXF32 PXAPI PXMathNormalizeF(const PXF32 minimum, const PXF32 maximum, const PXF32 value);
PXPublic unsigned int PXAPI PXMathLiniarClampAsRGBColorF(const PXF32 minimum, const PXF32 maximum, const PXF32 value);

// x = (-b +/- b^2 - 4ac) / 2a
PXPublic void PXAPI PXMathFormulaQuadratic(const PXF32 a, const PXF32 b, const PXF32 c, PXF32 PXREF resultA, PXF32 PXREF resultB);

// x = -(p/2) +/- sqrt((p/2)^2 - q)
PXPublic void PXAPI PXMathFormulaPQ(const PXF32 p, const PXF32 q, PXF32 PXREF resultA, PXF32 PXREF resultB);

PXPublic double PXAPI PXMathHornerD(double PXREF coefficientList, const PXI32U degree, const double x);

PXPublic void PXAPI PXFastFourierTransform();




#endif