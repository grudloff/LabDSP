// L138_fir_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#include "ave5.cof"

float x[N]; // filter delay line

interrupt void interrupt4(void)
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(input_left_sample()); // input from ADC
  for (i=0 ; i<N ; i++)                // compute filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)            // shift delay line
    x[i] = x[i-1]; 
  output_left_sample((uint16_t)(yn));  // output to DAC
  return;
}

int main(void)
{

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);
}
