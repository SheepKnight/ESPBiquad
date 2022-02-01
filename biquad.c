#include "biquad.h"

void calc_biquad(Biquad *b){
    ESP_LOGI("Biquad", "recalculating.");
	float norm;
    float V = pow(10, fabs(b->peakGain) / 20.0);
    float K = tan(B_PI * b->Fc);
    switch (b->type) {
        case bq_type_lowpass:
            norm = 1 / (1 + K / b->Q + K * K);
            b->a0 = K * K * norm;
            b->a1 = 2 * b->a0;
            b->a2 = b->a0;
            b->b1 = 2 * (K * K - 1) * norm;
            b->b2 = (1 - K / b->Q + K * K) * norm;
            break;
            
        case bq_type_highpass:
            norm = 1 / (1 + K / b->Q + K * K);
            b->a0 = 1 * norm;
            b->a1 = -2 * b->a0;
            b->a2 = b->a0;
            b->b1 = 2 * (K * K - 1) * norm;
            b->b2 = (1 - K / b->Q + K * K) * norm;
            break;
            
        case bq_type_bandpass:
            norm = 1 / (1 + K / b->Q + K * K);
            b->a0 = K / b->Q * norm;
            b->a1 = 0;
            b->a2 = -b->a0;
            b->b1 = 2 * (K * K - 1) * norm;
            b->b2 = (1 - K / b->Q + K * K) * norm;
            break;
            
        case bq_type_notch:
            norm = 1 / (1 + K / b->Q + K * K);
            b->a0 = (1 + K * K) * norm;
            b->a1 = 2 * (K * K - 1) * norm;
            b->a2 = b->a0;
            b->b1 = b->a1;
            b->b2 = (1 - K / b->Q + K * K) * norm;
            break;
            
        case bq_type_peak:
            if (b->peakGain >= 0) {    // boost
                norm = 1 / (1 + 1/b->Q * K + K * K);
                b->a0 = (1 + V/b->Q * K + K * K) * norm;
                b->a1 = 2 * (K * K - 1) * norm;
                b->a2 = (1 - V/b->Q * K + K * K) * norm;
                b->b1 = b->a1;
                b->b2 = (1 - 1/b->Q * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + V/b->Q * K + K * K);
                b->a0 = (1 + 1/b->Q * K + K * K) * norm;
                b->a1 = 2 * (K * K - 1) * norm;
                b->a2 = (1 - 1/b->Q * K + K * K) * norm;
                b->b1 = b->a1;
                b->b2 = (1 - V/b->Q * K + K * K) * norm;
            }
            break;
        case bq_type_lowshelf:
            if (b->peakGain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                b->a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                b->a1 = 2 * (V * K * K - 1) * norm;
                b->a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                b->b1 = 2 * (K * K - 1) * norm;
                b->b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                b->a0 = (1 + sqrt(2) * K + K * K) * norm;
                b->a1 = 2 * (K * K - 1) * norm;
                b->a2 = (1 - sqrt(2) * K + K * K) * norm;
                b->b1 = 2 * (V * K * K - 1) * norm;
                b->b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
            }
            break;
        case bq_type_highshelf:
            if (b->peakGain >= 0) {    // boost
                norm = 1 / (1 + sqrt(2) * K + K * K);
                b->a0 = (V + sqrt(2*V) * K + K * K) * norm;
                b->a1 = 2 * (K * K - V) * norm;
                b->a2 = (V - sqrt(2*V) * K + K * K) * norm;
                b->b1 = 2 * (K * K - 1) * norm;
                b->b2 = (1 - sqrt(2) * K + K * K) * norm;
            }
            else {    // cut
                norm = 1 / (V + sqrt(2*V) * K + K * K);
                b->a0 = (1 + sqrt(2) * K + K * K) * norm;
                b->a1 = 2 * (K * K - 1) * norm;
                b->a2 = (1 - sqrt(2) * K + K * K) * norm;
                b->b1 = 2 * (K * K - V) * norm;
                b->b2 = (V - sqrt(2*V) * K + K * K) * norm;
            }
            break;
    }
    
}
float process_biquad(Biquad *b, float in){
	float out = in * b->a0 + b->z1;
	b->z1 = in * b->a1 + b->z2 - b->b1 * out;
    b->z2 = in * b->a2 - b->b2 * out;
    return out;
}

void init_biquad(Biquad *b, int type, float Fc, float Q, float peakGain, int sampleRate){
	b->type = type;
	b->Q = Q;
	b->peakGain = peakGain;
	b->Fc = (Fc / (float) sampleRate);
	b->z1 = 0.0;
	b->z2 = 0.0;
	calc_biquad(b);
}
