/*
 * EQ.c
 *
 *  Created on: Aug 20, 2019
 *      Author: Intern_2
 */

#include "EQ.h"



Status EQInit(EQParams *params, EQCoeffs *coeffs, EQStates *states, int sampleRate)
{
	int i;
	Status status;

	for (i = 0; i < EQ_BANDS_NUM; i++)
	{
		status = initBiquadFilter(&params->biquadParams[i], &coeffs->biquadCoeffs[i],
						 &states->biquadStates[i], sampleRate);
	}

	return status;
}

Status EQSetParam(EQParams *params, EQCoeffs *coeffs, EQStates *states,
				  const int16_t band, const uint16_t id, const double value)
{
	if (!params || !coeffs || !states)
		return statusError;

	if (band < 0)
	{
		params->isActive = (int)value;
		coeffs->isActive = (int)value;
		return statusOK;
	}
	else
	{
		return setBiquadParam(&params->biquadParams[band], &coeffs->biquadCoeffs[band],
							  &states->biquadStates[band], id, value);
	}
}

F32x2 EQ_Process(const EQCoeffs *coeffs, EQStates *states, F32x2 sample)
{
	uint8_t i;

	if (coeffs->isActive)
	{
		for (i = 0; i < EQ_BANDS_NUM; i++)
		{
			if (coeffs->biquadCoeffs[i].isActive)
			{
				sample = biquadFilter(sample, &coeffs->biquadCoeffs[i],
									  &states->biquadStates[i]);
			}
		}
	}

	return sample;
}
