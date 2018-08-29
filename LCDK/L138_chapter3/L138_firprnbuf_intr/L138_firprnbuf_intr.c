// L138_firprnbuf_intr.c
//

#include "L138_aic3106_init.h"

#include "bp41.cof"

#define YNBUFLENGTH 1024

float x[N];                     // filter delay line
float yn_buffer[YNBUFLENGTH];   // output buffer
short ynbufindex = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(prbs());       // input from prbs function
  for (i=0 ; i<N ; i++)         // compute filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)     // shift delay line
    x[i] = x[i-1]; 
  yn_buffer[ynbufindex++] = yn; // buffer output samples
  if(ynbufindex >= YNBUFLENGTH) ynbufindex = 0;
  output_left_sample((uint16_t)(yn)); // output to L DAC
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB);
  while(1);
}

