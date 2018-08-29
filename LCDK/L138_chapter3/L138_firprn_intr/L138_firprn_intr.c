// L138_firprn_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#include "bp55.cof"

float x[N]; // filter delay line
AIC31_data_type codec_data;

interrupt void interrupt4(void)
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(prbs());          // input from prbs function
  for (i=0 ; i<N ; i++)            // compute filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)        // shift delay line
    x[i] = x[i-1]; 
  codec_data.channel[LEFT] = (uint16_t)(yn);
  codec_data.channel[RIGHT] = (uint16_t)(yn);
  output_sample(codec_data.uint);  // output to L and R DAC
  return;
}

int main(void)
{

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}
