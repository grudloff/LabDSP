#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/
//#include "global_defs.h"

extern int excitation_noise[];
extern int excitation_pulses[];
extern int excitation_pulses_2[];
extern int excitation_square[];

extern const int frameLength;

void window(int* win, int* sp_input);

void xcorr(float* RMS, float* corr, int* sp_input, short p);

void filter(float* RMS, float* out, float* Den_coff, short Nr_size, int* iir_input);

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
