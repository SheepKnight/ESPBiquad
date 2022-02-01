#ifndef BIQUAD_H
#define BIQUAD_H
#include <math.h>
#include "esp_log.h"

#define B_PI 3.14159265359

#define bq_type_lowpass 0
#define bq_type_highpass 1
#define bq_type_bandpass 2
#define bq_type_notch 3
#define bq_type_peak 4
#define bq_type_lowshelf 5
#define bq_type_highshelf 6

typedef struct biquad_struct {

	int type;
	
	
	//Parameters
	float Fc, Q, peakGain;
	
	//Coefficients
	float a0, a1, a2, b1, b2;
	//Buffer
	float z1, z2;
	
	
} Biquad;

void init_biquad(Biquad *b, int type, float Fc, float Q, float peakGain, int sampleRate);
float process_biquad(Biquad *b, float in);
void calc_biquad(Biquad *b);

#endif