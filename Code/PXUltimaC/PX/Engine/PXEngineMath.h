#pragma once


#ifndef PXEngineMathIncluded
#define PXEngineMathIncluded

#include <PX/Type/PXType.h>
#include "ECS/PXECS.h"

typedef void (PXAPI* PXEngineMathStereoToMonoI16UFunction)(PXI16U* data, const PXSize dataAmount);


typedef PXI8U(PXAPI* PXEngineMathCompareI8VFunction)(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value);
typedef PXI16U(PXAPI* PXEngineMathCompareI16VFunction)(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value);


static const PXI8U PXIndexI8UNotFound = (PXI8U)-1;
static const PXI16U PXIndexI16UNotFound = (PXI16U)-1;
static const PXI32U PXIndexI32UNotFound = (PXI32U)-1;
static const PXI64U PXIndexI64UNotFound = (PXI64U)-1;


typedef struct PXEngineMath_
{
    PXECSInfo Info;

    PXEngineMathStereoToMonoI16UFunction StereoToMonoI16U; // Downsample 16-bit Stereo to 16-Mono
    PXEngineMathCompareI8VFunction CompareI8V;
    PXEngineMathCompareI16VFunction CompareI16V;
}
PXEngineMath;

typedef struct PXEngineMathCreateInfo_
{
    PXECSCreateInfo Info;

    int x;
}
PXEngineMathCreateInfo;

PXPublic PXResult PXAPI PXEngineMathCreate(PXEngineMath** pxEngineMath, PXEngineMathCreateInfo PXREF pxEngineMathCreateInfo);
PXPublic PXResult PXAPI PXEngineMathRelease(PXEngineMath PXREF pxEngineMath, PXEngineMathCreateInfo PXREF pxEngineMathCreateInfo);
PXPublic PXEngineMath* PXAPI PXEngineMathInstanceGet(void);

PXPublic PXI8U PXAPI PXEngineMathCompareI8V_Scalar(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value);
PXPublic PXI8U PXAPI PXEngineMathCompareI8V_MMX(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value);
PXPublic PXI8U PXAPI PXEngineMathCompareI8V_SSE2(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value);
PXPublic PXI8U PXAPI PXEngineMathCompareI8V_AVX512(const PXI8U PXREF dataList, const PXI8U listAmount, const PXI8U value);

PXPublic PXI16U PXAPI PXEngineMathCompareI16V_Scalar(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value);
PXPublic PXI16U PXAPI PXEngineMathCompareI16V_MMX(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value);
PXPublic PXI16U PXAPI PXEngineMathCompareI16V_SSE2(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value);
PXPublic PXI16U PXAPI PXEngineMathCompareI16V_AVX512(const PXI16U PXREF dataList, const PXI16U listAmount, const PXI16U value);

PXPublic void PXAPI PXEngineMathStereoToMonoI16U_Scalar(PXI16U* data, const PXSize dataAmount);
PXPublic void PXAPI PXEngineMathStereoToMonoI16U_X512(PXI16U* data, const PXSize dataAmount);


#endif