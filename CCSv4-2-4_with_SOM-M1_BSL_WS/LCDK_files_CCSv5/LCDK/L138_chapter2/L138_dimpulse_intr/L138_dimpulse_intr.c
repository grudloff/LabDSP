// L138_dimpulse_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define LOOPLENGTH 64								
int16_t impulse_table[LOOPLENGTH] =
{10000, 0, 0, 0, 0, 0, 0, 0,      //contains one non-zero
0, 0, 0, 0, 0, 0, 0, 0,            //value in 64
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0};

int16_t loopindex = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  output_left_sample(impulse_table[loopindex++]);
  if (loopindex >= LOOPLENGTH)
    loopindex = 0;
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
