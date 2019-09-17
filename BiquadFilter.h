/*
 * BiquadFilter.h
 *
 *  Created on: Aug 20, 2019
 *      Author: Intern_2
 */

#ifndef BIQUAD_FILTER
#define BIQUAD_FILTER

#include <math.h>

#include "ExternalAndInternalTypesConverters.h"
#include "InternalTypesArithmetic.h"

#define PI	  3.14159265358979323846
#define SQRT2 1.41421356237309504880


typedef enum {
	lowpass	  = 1,
	highpass  = 2,
	bandpass  = 3,
	notch	  = 4,
	peak	  = 5,
	lowShelf  = 6,
	highShelf = 7
} BiquadType;

typedef enum {
	isActiveID	 = 1,
	filterTypeID = 2,
	FcID		 = 3,
	Qid		 	 = 4,
	peakGain	 = 5
} BiquadParamID;


typedef struct {
	int sampleRate;
	uint8_t isActive;
	int type;
	double Fc;
	double Q;
	double peakGain;
} BiquadParams;

typedef struct {
	uint8_t isActive;
	F32x2 a[3];			// Q27
	F32x2 b[2];			// Q27
} BiquadCoeffs;

typedef struct {
	F32x2 x[2];
	F32x2 y[2];
	F32x2 err;
} BiquadStates;


ALWAYS_INLINE Status initBiquadParams(BiquadParams *biquadParams, int sampleRate)
{
	if (!biquadParams)
		return statusError;

	biquadParams->sampleRate = sampleRate;

	biquadParams->isActive 	 = 0;
	biquadParams->type 		 = 1;
	biquadParams->Fc 		 = 0;
	biquadParams->Q 		 = 0;
	biquadParams->peakGain 	 = 0;

	return statusOK;
}

ALWAYS_INLINE Status initBiquadCoeffs(BiquadCoeffs *biquadCoeffs)
{
	if (!biquadCoeffs)
		return statusError;

	biquadCoeffs->isActive = 0;

	biquadCoeffs->a[0] = F32x2Zero();
	biquadCoeffs->a[1] = F32x2Zero();
	biquadCoeffs->a[2] = F32x2Zero();
	biquadCoeffs->b[0] = F32x2Zero();
	biquadCoeffs->b[1] = F32x2Zero();

	return statusOK;
}

ALWAYS_INLINE Status initBiquadStates(BiquadStates *biquadStates)
{
	if (!biquadStates)
		return statusError;

	biquadStates->x[0] = F32x2Zero();
	biquadStates->x[1] = F32x2Zero();
	biquadStates->y[0] = F32x2Zero();
	biquadStates->y[1] = F32x2Zero();
	biquadStates->err  = F32x2Zero();

	return statusOK;
}

ALWAYS_INLINE Status initBiquadFilter(BiquadParams *biquadParams,
									  BiquadCoeffs *biquadCoeffs,
									  BiquadStates *biquadStates, int sampleRate)
{
	Status status = statusOK;

	status |= initBiquadParams(biquadParams, sampleRate);
	status |= initBiquadCoeffs(biquadCoeffs);
	status |= initBiquadStates(biquadStates);

	return status;
}

ALWAYS_INLINE void resetBiquadStates(BiquadStates *biquadStates)
{
	biquadStates->x[0] = F32x2Zero();
	biquadStates->x[1] = F32x2Zero();
	biquadStates->y[0] = F32x2Zero();
	biquadStates->y[1] = F32x2Zero();
	biquadStates->err  = F32x2Zero();
}

static Status calcBiquadCoeffs(const BiquadParams *biquadParams,
									  BiquadCoeffs *biquadCoeffs)
{
	if (!biquadParams || !biquadCoeffs)
		return statusError;

	double norm;
	double Q = biquadParams->Q;
	double V = pow(10, fabs(biquadParams->peakGain) / 20);
	double K = tan(PI * biquadParams->Fc / biquadParams->sampleRate);
	double Kpow2 = K * K;
	double a[3];
	double b[2];

	switch (biquadParams->type)
	{
	case lowpass:
		norm = 1 / (1 + K / Q + Kpow2);
		a[0] = Kpow2 * norm;
		a[1] = 2 * a[0];
		a[2] = a[0];
		b[0] = 2 * (Kpow2 - 1) * norm;
		b[1] = (1 - K / Q + Kpow2) * norm;
		break;

	case highpass:
		norm = 1 / (1 + K / Q + Kpow2);
		a[0] = norm;
		a[1] = -2 * a[0];
		a[2] = a[0];
		b[0] = 2 * (Kpow2 - 1) * norm;
		b[1] = (1 - K / Q + Kpow2) * norm;
		break;

	case bandpass:
		norm = 1 / (1 + K / Q + Kpow2);
		a[0] = K / Q * norm;
		a[1] = 0;
		a[2] = -a[0];
		b[0] = 2 * (Kpow2 - 1) * norm;
		b[1] = (1 - K / Q + Kpow2) * norm;
		break;

	case notch:
		norm = 1 / (1 + K / Q + Kpow2);
		a[0] = (1 + Kpow2) * norm;
		a[1] = 2 * (Kpow2 - 1) * norm;
		a[2] = a[0];
		b[0] = a[1];
		b[1] = (1 - K / Q + Kpow2) * norm;
		break;

	case peak:
		if (biquadParams->peakGain >= 0)
		{
			norm = 1 / (1 + 1 / Q * K + Kpow2);
			a[0] = (1 + V / Q * K + Kpow2) * norm;
			a[1] = 2 * (Kpow2 - 1) * norm;
			a[2] = (1 - V / Q * K + Kpow2) * norm;
			b[0] = a[1];
			b[1] = (1 - 1 / Q * K + Kpow2) * norm;
		}
		else
		{
			norm = 1 / (1 + V / Q * K + Kpow2);
			a[0] = (1 + 1 / Q * K + Kpow2) * norm;
			a[1] = 2 * (Kpow2 - 1) * norm;
			a[2] = (1 - 1 / Q * K + Kpow2) * norm;
			b[0] = a[1];
			b[1] = (1 - V / Q * K + Kpow2) * norm;
		}
		break;

	case lowShelf:
		if (biquadParams->peakGain >= 0)
		{
			norm = 1 / (1 + SQRT2 * K + Kpow2);
			a[0] = (1 + sqrt(2 * V) * K + V * Kpow2) * norm;
			a[1] = 2 * (V * Kpow2 - 1) * norm;
			a[2] = (1 - sqrt(2 * V) * K + V * Kpow2) * norm;
			b[0] = 2 * (Kpow2 - 1) * norm;
			b[1] = (1 - SQRT2 * K + Kpow2) * norm;
		}
		else
		{
			norm = 1 / (1 + sqrt(2 * V) * K + V * Kpow2);
			a[0] = (1 + SQRT2 * K + Kpow2) * norm;
			a[1] = 2 * (Kpow2 - 1) * norm;
			a[2] = (1 - SQRT2 * K + Kpow2) * norm;
			b[0] = 2 * (V * Kpow2 - 1) * norm;
			b[1] = (1 - sqrt(2 * V) * K + V * Kpow2) * norm;
		}
		break;

	case highShelf:
		if (biquadParams->peakGain >= 0)
		{
			norm = 1 / (1 + SQRT2 * K + Kpow2);
			a[0] = (V + sqrt(2 * V) * K + Kpow2) * norm;
			a[1] = 2 * (Kpow2 - V) * norm;
			a[2] = (V - sqrt(2 * V) * K + Kpow2) * norm;
			b[0] = 2 * (Kpow2 - 1) * norm;
			b[1] = (1 - SQRT2 * K + Kpow2) * norm;
		}
		else
		{
			norm = 1 / (V + sqrt(2 * V) * K + Kpow2);
			a[0] = (1 + SQRT2 * K + Kpow2) * norm;
			a[1] = 2 * (Kpow2 - 1) * norm;
			a[2] = (1 - SQRT2 * K + Kpow2) * norm;
			b[0] = 2 * (Kpow2 - V) * norm;
			b[1] = (V - sqrt(2 * V) * K + Kpow2) * norm;
		}
		break;
	}

	biquadCoeffs->a[0] = doubleToF32x2Set(a[0] / 16);
	biquadCoeffs->a[1] = doubleToF32x2Set(a[1] / 16);
	biquadCoeffs->a[2] = doubleToF32x2Set(a[2] / 16);
	biquadCoeffs->b[0] = doubleToF32x2Set(b[0] / 16);
	biquadCoeffs->b[1] = doubleToF32x2Set(b[1] / 16);

	return statusOK;
}

ALWAYS_INLINE Status setBiquadParam(BiquadParams *biquadParams, BiquadCoeffs *biquadCoeffs,
					  BiquadStates *biquadStates, const uint16_t id, const double value)
{
	if (!biquadParams || !biquadCoeffs || !biquadStates)
		return statusError;

	switch (id)
	{
	case isActiveID:
		if ((int)value == 0)
		{
			resetBiquadStates(biquadStates);
		}

		biquadParams->isActive = (int8_t)value;
		biquadCoeffs->isActive = (int8_t)value;
		break;

	case filterTypeID:
		biquadParams->type = (int)value;
		calcBiquadCoeffs(biquadParams, biquadCoeffs);
		break;

	case FcID:
		biquadParams->Fc = value;
		calcBiquadCoeffs(biquadParams, biquadCoeffs);
		break;

	case Qid:
		biquadParams->Q = value;
		calcBiquadCoeffs(biquadParams, biquadCoeffs);
		break;

	case peakGain:
		biquadParams->peakGain = value;
		calcBiquadCoeffs(biquadParams, biquadCoeffs);
		break;

	default:
		return statusError;
	}

	return statusOK;
}

ALWAYS_INLINE F32x2 biquadFilter(const F32x2 sample, const BiquadCoeffs *coeffs,
								 BiquadStates *states)
{
	F64x2 acc = F32x2ToF64x2_l(states->err);

	acc = F32x2MacF64x2(acc, coeffs->a[0], sample);
	acc = F32x2MSubF64x2(acc, coeffs->b[0], states->y[0]);
	acc = F32x2MacF64x2(acc, coeffs->a[1], states->x[0]);
	acc = F32x2MSubF64x2(acc, coeffs->b[1], states->y[1]);
	acc = F32x2MacF64x2(acc, coeffs->a[2], states->x[1]);

	states->err = F64x2ToF32x2_l(F64x2AND(acc, F64x2Set(0xFFFFFFFLL)));

	acc = F64x2LeftShiftAS(acc, 4);

	states->x[1] = states->x[0];
	states->x[0] = sample;
	states->y[1] = states->y[0];
	states->y[0] = F64x2ToF32x2_h(acc);

	return states->y[0];
}

#endif /* BIQUAD_FILTER */
