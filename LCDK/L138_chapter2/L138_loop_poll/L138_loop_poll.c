// L138_loop_poll.c
//

#include "L138_LCDK_aic3106_init.h"

int main(void)
{
  uint32_t sample;
  
  L138_initialise_poll(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) 
  {
    sample = input_sample();
    output_sample(sample);
  }
}
