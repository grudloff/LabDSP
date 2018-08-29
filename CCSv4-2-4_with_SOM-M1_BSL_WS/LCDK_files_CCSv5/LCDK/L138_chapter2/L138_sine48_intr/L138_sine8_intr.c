// L138_sine48_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#define N 8
int16_t sine_table[N] = {0,7071,10000,7071,0,-7071,-10000,-7071};
int sine_ptr = 0; // pointer into lookup table

interrupt void interrupt4(void) // interrupt service routine
{
  uint16_t left_sample;

  left_sample =  sine_table[sine_ptr];
  sine_ptr = (sine_ptr+1)%N;
  output_left_sample(left_sample);
  
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
