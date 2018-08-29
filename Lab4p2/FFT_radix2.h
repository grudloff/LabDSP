#ifndef FFT_RADIX2_H
#define FFT_RADIX2_H

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/
#include "L138_LCDK_aic3106_init.h"
#include "complex.h"

#define N (BUFCOUNT/2)

extern Complex W_N[N];


void fft_radix2(unsigned int size, Complex *input, Complex *FFT);
void fft_mag(unsigned int size, Complex *fft_freq_vec, float *fft_mag_vec);

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
