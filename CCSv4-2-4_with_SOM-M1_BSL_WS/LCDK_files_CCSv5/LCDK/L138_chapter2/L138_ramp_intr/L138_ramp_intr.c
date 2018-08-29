// L138_ramp_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#define LOOPLENGTH 64								

int16_t output = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  output_left_sample(output);   // output to L DAC
  output += 2000;		        // increment output value
  if (output >= 30000)	        // if peak is reached
  output = -30000;              // reinitialize
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
