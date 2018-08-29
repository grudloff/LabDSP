// L138_averageprn_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define N 5          // number of points averaged
float x[N];          // filter input delay line
float h[N];          // filter coefficients

interrupt void interrupt4(void) 
{
  short i;
  float yn = 0.0;    // filter output
  
  x[0] = (float)(prbs());              // pseudo random input
  for (i=0 ; i<N ; i++)                // calculate output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)            // shift delay line
    x[i] = x[i-1]; 
  output_left_sample((uint16_t)(yn));  // output to DAC
  return;
}

int main(void)
{
  short i;

  for (i=0 ; i<N ; i++)                // initialise coeffs
    h[i] = 1.0/N;
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);                            // loop forever
}
