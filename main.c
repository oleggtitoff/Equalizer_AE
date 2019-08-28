/*
 * main.c
 *
 *  Created on: Aug 20, 2019
 *      Author: Intern_2
 */

#include "FilesOperations.h"
#include "EQ.h"

#define INPUT_FILE_NAME "Sine -12.wav"
#define OUTPUT_FILE_NAME "Output.wav"

#define BYTES_PER_SAMPLE 4
#define DATA_BUFF_SIZE 1000
#define SAMPLE_RATE 48000
#define CHANNELS 2

#define EQ_HEADROOM 4

// FILTER TYPES
// lowpass = 1,
// highpass = 2,
// bandpass = 3,
// notch = 4,
// peak = 5,
// lowShelf = 6,
// highShelf = 7

// BAND 1
#define B1_IS_ACTIVE	1
#define	B1_FILTER_TYPE	2
#define B1_FC			2200
#define B1_Q_VALUE		0.707
#define B1_PEAK_GAIN	6

// BAND 2
#define B2_IS_ACTIVE	1
#define	B2_FILTER_TYPE	5
#define B2_FC			4380
#define B2_Q_VALUE		3
#define B2_PEAK_GAIN	9

// BAND 3
#define B3_IS_ACTIVE	1
#define	B3_FILTER_TYPE	5
#define B3_FC			6560
#define B3_Q_VALUE		3
#define B3_PEAK_GAIN	6

// BAND 4
#define B4_IS_ACTIVE	1
#define	B4_FILTER_TYPE	5
#define B4_FC			8740
#define B4_Q_VALUE		3
#define B4_PEAK_GAIN	2

// BAND 5
#define B5_IS_ACTIVE	1
#define	B5_FILTER_TYPE	5
#define B5_FC			10920
#define B5_Q_VALUE		3
#define B5_PEAK_GAIN	6

// BAND 6
#define B6_IS_ACTIVE	1
#define	B6_FILTER_TYPE	5
#define B6_FC			13100
#define B6_Q_VALUE		3
#define B6_PEAK_GAIN	6

// BAND 7
#define B7_IS_ACTIVE	1
#define	B7_FILTER_TYPE	5
#define B7_FC			15280
#define B7_Q_VALUE		3
#define B7_PEAK_GAIN	-6

// BAND 8
#define B8_IS_ACTIVE	1
#define	B8_FILTER_TYPE	5
#define B8_FC			17460
#define B8_Q_VALUE		3
#define B8_PEAK_GAIN	-6

// BAND 9
#define B9_IS_ACTIVE	1
#define	B9_FILTER_TYPE	5
#define B9_FC			19640
#define B9_Q_VALUE		3
#define B9_PEAK_GAIN	6

// BAND 10
#define B10_IS_ACTIVE	1
#define	B10_FILTER_TYPE 7
#define B10_FC			21820
#define B10_Q_VALUE		0.707
#define B10_PEAK_GAIN	-10


ALWAYS_INLINE void setParams(EQParams *params, EQCoeffs *coeffs, EQStates *states);
void run(FILE *inputFilePtr, FILE *outputFilePtr, const EQCoeffs *coeffs, EQStates *states);


int main()
{
	FILE *inputFilePtr = openFile(INPUT_FILE_NAME, 0);
	FILE *outputFilePtr = openFile(OUTPUT_FILE_NAME, 1);
	uint8_t headerBuff[FILE_HEADER_SIZE];
	EQParams params;
	EQCoeffs coeffs;
	EQStates states;

	EQInit(&params, &coeffs, &states, SAMPLE_RATE);
	setParams(&params, &coeffs, &states);

	readHeader(headerBuff, inputFilePtr);
	writeHeader(headerBuff, outputFilePtr);
	run(inputFilePtr, outputFilePtr, &coeffs, &states);

	fclose(inputFilePtr);
	fclose(outputFilePtr);
	return 0;
}


ALWAYS_INLINE void setParams(EQParams *params, EQCoeffs *coeffs, EQStates *states)
{
	EQSetParam(&params->biquadParams[0], &coeffs->biquadCoeffs[0], &states->biquadStates[0], isActiveID,   B1_IS_ACTIVE);
	EQSetParam(&params->biquadParams[0], &coeffs->biquadCoeffs[0], &states->biquadStates[0], filterTypeID, B1_FILTER_TYPE);
	EQSetParam(&params->biquadParams[0], &coeffs->biquadCoeffs[0], &states->biquadStates[0], FcID, 		 B1_FC);
	EQSetParam(&params->biquadParams[0], &coeffs->biquadCoeffs[0], &states->biquadStates[0], Qid, 		 B1_Q_VALUE);
	EQSetParam(&params->biquadParams[0], &coeffs->biquadCoeffs[0], &states->biquadStates[0], peakGain, 	 B1_PEAK_GAIN);

	EQSetParam(&params->biquadParams[1], &coeffs->biquadCoeffs[1], &states->biquadStates[1], isActiveID,   B2_IS_ACTIVE);
	EQSetParam(&params->biquadParams[1], &coeffs->biquadCoeffs[1], &states->biquadStates[1], filterTypeID, B2_FILTER_TYPE);
	EQSetParam(&params->biquadParams[1], &coeffs->biquadCoeffs[1], &states->biquadStates[1], FcID, 		 B2_FC);
	EQSetParam(&params->biquadParams[1], &coeffs->biquadCoeffs[1], &states->biquadStates[1], Qid, 		 B2_Q_VALUE);
	EQSetParam(&params->biquadParams[1], &coeffs->biquadCoeffs[1], &states->biquadStates[1], peakGain, 	 B2_PEAK_GAIN);

	EQSetParam(&params->biquadParams[2], &coeffs->biquadCoeffs[2], &states->biquadStates[2], isActiveID,   B3_IS_ACTIVE);
	EQSetParam(&params->biquadParams[2], &coeffs->biquadCoeffs[2], &states->biquadStates[2], filterTypeID, B3_FILTER_TYPE);
	EQSetParam(&params->biquadParams[2], &coeffs->biquadCoeffs[2], &states->biquadStates[2], FcID, 		 B3_FC);
	EQSetParam(&params->biquadParams[2], &coeffs->biquadCoeffs[2], &states->biquadStates[2], Qid, 		 B3_Q_VALUE);
	EQSetParam(&params->biquadParams[2], &coeffs->biquadCoeffs[2], &states->biquadStates[2], peakGain, 	 B3_PEAK_GAIN);

	EQSetParam(&params->biquadParams[3], &coeffs->biquadCoeffs[3], &states->biquadStates[3], isActiveID,   B4_IS_ACTIVE);
	EQSetParam(&params->biquadParams[3], &coeffs->biquadCoeffs[3], &states->biquadStates[3], filterTypeID, B4_FILTER_TYPE);
	EQSetParam(&params->biquadParams[3], &coeffs->biquadCoeffs[3], &states->biquadStates[3], FcID, 		 B4_FC);
	EQSetParam(&params->biquadParams[3], &coeffs->biquadCoeffs[3], &states->biquadStates[3], Qid, 		 B4_Q_VALUE);
	EQSetParam(&params->biquadParams[3], &coeffs->biquadCoeffs[3], &states->biquadStates[3], peakGain, 	 B4_PEAK_GAIN);

	EQSetParam(&params->biquadParams[4], &coeffs->biquadCoeffs[4], &states->biquadStates[4], isActiveID,   B5_IS_ACTIVE);
	EQSetParam(&params->biquadParams[4], &coeffs->biquadCoeffs[4], &states->biquadStates[4], filterTypeID, B5_FILTER_TYPE);
	EQSetParam(&params->biquadParams[4], &coeffs->biquadCoeffs[4], &states->biquadStates[4], FcID, 		 B5_FC);
	EQSetParam(&params->biquadParams[4], &coeffs->biquadCoeffs[4], &states->biquadStates[4], Qid, 		 B5_Q_VALUE);
	EQSetParam(&params->biquadParams[4], &coeffs->biquadCoeffs[4], &states->biquadStates[4], peakGain, 	 B5_PEAK_GAIN);

	EQSetParam(&params->biquadParams[5], &coeffs->biquadCoeffs[5], &states->biquadStates[5], isActiveID,   B6_IS_ACTIVE);
	EQSetParam(&params->biquadParams[5], &coeffs->biquadCoeffs[5], &states->biquadStates[5], filterTypeID, B6_FILTER_TYPE);
	EQSetParam(&params->biquadParams[5], &coeffs->biquadCoeffs[5], &states->biquadStates[5], FcID, 		 B6_FC);
	EQSetParam(&params->biquadParams[5], &coeffs->biquadCoeffs[5], &states->biquadStates[5], Qid, 		 B6_Q_VALUE);
	EQSetParam(&params->biquadParams[5], &coeffs->biquadCoeffs[5], &states->biquadStates[5], peakGain, 	 B6_PEAK_GAIN);

	EQSetParam(&params->biquadParams[6], &coeffs->biquadCoeffs[6], &states->biquadStates[6], isActiveID,   B7_IS_ACTIVE);
	EQSetParam(&params->biquadParams[6], &coeffs->biquadCoeffs[6], &states->biquadStates[6], filterTypeID, B7_FILTER_TYPE);
	EQSetParam(&params->biquadParams[6], &coeffs->biquadCoeffs[6], &states->biquadStates[6], FcID, 		 B7_FC);
	EQSetParam(&params->biquadParams[6], &coeffs->biquadCoeffs[6], &states->biquadStates[6], Qid, 		 B7_Q_VALUE);
	EQSetParam(&params->biquadParams[6], &coeffs->biquadCoeffs[6], &states->biquadStates[6], peakGain, 	 B7_PEAK_GAIN);

	EQSetParam(&params->biquadParams[7], &coeffs->biquadCoeffs[7], &states->biquadStates[7], isActiveID,   B8_IS_ACTIVE);
	EQSetParam(&params->biquadParams[7], &coeffs->biquadCoeffs[7], &states->biquadStates[7], filterTypeID, B8_FILTER_TYPE);
	EQSetParam(&params->biquadParams[7], &coeffs->biquadCoeffs[7], &states->biquadStates[7], FcID, 		 B8_FC);
	EQSetParam(&params->biquadParams[7], &coeffs->biquadCoeffs[7], &states->biquadStates[7], Qid, 		 B8_Q_VALUE);
	EQSetParam(&params->biquadParams[7], &coeffs->biquadCoeffs[7], &states->biquadStates[7], peakGain, 	 B8_PEAK_GAIN);

	EQSetParam(&params->biquadParams[8], &coeffs->biquadCoeffs[8], &states->biquadStates[8], isActiveID,   B9_IS_ACTIVE);
	EQSetParam(&params->biquadParams[8], &coeffs->biquadCoeffs[8], &states->biquadStates[8], filterTypeID, B9_FILTER_TYPE);
	EQSetParam(&params->biquadParams[8], &coeffs->biquadCoeffs[8], &states->biquadStates[8], FcID, 		 B9_FC);
	EQSetParam(&params->biquadParams[8], &coeffs->biquadCoeffs[8], &states->biquadStates[8], Qid, 		 B9_Q_VALUE);
	EQSetParam(&params->biquadParams[8], &coeffs->biquadCoeffs[8], &states->biquadStates[8], peakGain, 	 B9_PEAK_GAIN);

	EQSetParam(&params->biquadParams[9], &coeffs->biquadCoeffs[9], &states->biquadStates[9], isActiveID,   B10_IS_ACTIVE);
	EQSetParam(&params->biquadParams[9], &coeffs->biquadCoeffs[9], &states->biquadStates[9], filterTypeID, B10_FILTER_TYPE);
	EQSetParam(&params->biquadParams[9], &coeffs->biquadCoeffs[9], &states->biquadStates[9], FcID, 		 B10_FC);
	EQSetParam(&params->biquadParams[9], &coeffs->biquadCoeffs[9], &states->biquadStates[9], Qid, 		 B10_Q_VALUE);
	EQSetParam(&params->biquadParams[9], &coeffs->biquadCoeffs[9], &states->biquadStates[9], peakGain, 	 B10_PEAK_GAIN);
}

void run(FILE *inputFilePtr, FILE *outputFilePtr, const EQCoeffs *coeffs, EQStates *states)
{
	int32_t dataBuff[DATA_BUFF_SIZE * CHANNELS];
	size_t samplesRead;
	uint32_t i;

	while (1)
	{
		samplesRead = fread(dataBuff, BYTES_PER_SAMPLE, DATA_BUFF_SIZE, inputFilePtr);

		if (!samplesRead)
		{
			break;
		}

		for (i = 0; i < samplesRead / CHANNELS; i++)
		{
			F32x2 sample = F32x2RightShiftA(F32x2Set(dataBuff[i * CHANNELS + 1]), EQ_HEADROOM);
			sample = EQ_Process(coeffs, states, sample);
			dataBuff[i * CHANNELS + 1] = F32x2ToI32Extract_h(F32x2LeftShiftAS(sample, EQ_HEADROOM));
		}

		fwrite(dataBuff, BYTES_PER_SAMPLE, samplesRead, outputFilePtr);
	}
}
