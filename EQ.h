/*
 * EQ.h
 *
 *  Created on: Aug 20, 2019
 *      Author: Intern_2
 */

#ifndef EQ
#define EQ

#include "BiquadFilter.h"

#define EQ_BANDS_NUM 10

typedef struct {
	int8_t isActive;
	BiquadParams biquadParams[EQ_BANDS_NUM];
} EQParams;

typedef struct {
	int8_t isActive;
	BiquadCoeffs biquadCoeffs[EQ_BANDS_NUM];
} EQCoeffs;

typedef struct {
	BiquadStates biquadStates[EQ_BANDS_NUM];
} EQStates;


Status EQInit(EQParams *params, EQCoeffs *coeffs, EQStates *states, int sampleRate);
Status EQSetParam(EQParams *params, EQCoeffs *coeffs, EQStates *states,
				  const int16_t band, const uint16_t id, const double value);
F32x2 EQ_Process(const EQCoeffs *coeffs, EQStates *states, F32x2 sample);

#endif /* EQ */
