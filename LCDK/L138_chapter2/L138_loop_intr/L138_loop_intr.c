// L138_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"

interrupt void interrupt4(void)  // interrupt service routine
{
  uint32_t sample;

  sample = input_sample(); // read L + R samples from ADC
  output_sample(sample);   // write L + R samples to DAC
  return;
}

int main(void)
{
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
