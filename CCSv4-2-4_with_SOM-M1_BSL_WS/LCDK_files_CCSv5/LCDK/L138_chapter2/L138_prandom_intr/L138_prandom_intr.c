// L138_prandom_intr.c
//

#include "L138_LCDK_aic3106_init.h"

interrupt void interrupt4(void) // interrupt service routine
{
  output_left_sample(prand());
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
