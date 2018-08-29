// L138_prbs_intr.c
//

#include "L138_LCDK_aic3106_init.h"

AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{
  codec_data.channel[0] = prbs();
  codec_data.channel[1] = codec_data.channel[0];
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1); 
}


