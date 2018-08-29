// L138_fft_dsplibr2.c
//
// N-point FFT of sequence read from lookup table
//

#include <stdio.h>
#include <math.h> 
#include "dsplib674x.h"

#define N 128
#define TESTFREQ 900.0
#define SAMPLING_FREQ 8000.0
#define PI 3.14159265358979

void gen_w_r2(float* w, int n);
void bit_rev(float* x, int n);

#pragma DATA_ALIGN(x, 8);
#pragma DATA_ALIGN(w, 8);

float x[2*N];
float w[N];

void main()
{    
  int i;

  for(i=0 ; i<N ; i++) // initialise input data
  {
    x[i*2] = cos(2*PI*TESTFREQ*i/SAMPLING_FREQ);
    x[i*2 + 1] = 0.0;
  }
  printf("real input data stored in array x\n");
  gen_w_r2(w, N);
  bit_rev(w, N/2);
  DSPF_sp_cfftr2_dit(x, w, N);
  bit_rev(x, N);
  printf("done!\n");
  return;
}	

/* generate real and imaginary twiddle table of size n/2 complex numbers */
void gen_w_r2(float* w, int n)
{
    int i;
    float pi = 4.0 * atan(1.0);
    float e = pi * 2.0 / n;

    for(i = 0; i < (n >> 1); i++)
    {
        w[2 * i] = cos(i * e);
        w[2 * i + 1] = sin(i * e);
    }
}

/* bit reverse coefficients */
void bit_rev(float* x, int n)
{
    int i, j, k;
    float rtemp, itemp;
    j = 0;

    for(i=1; i < (n-1); i++)
    {
        k = n >> 1;

        while(k <= j)
        {
            j -= k;
            k >>= 1;
        }

        j += k;

        if(i < j)
        {
            rtemp = x[j*2];
            x[j*2] = x[i*2];
            x[i*2] = rtemp;
            itemp = x[j*2+1];
            x[j*2+1] = x[i*2+1];
            x[i*2+1] = itemp;
        }
    }
}

