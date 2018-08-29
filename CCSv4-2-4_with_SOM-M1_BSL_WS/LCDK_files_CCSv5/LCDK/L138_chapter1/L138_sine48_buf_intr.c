// L138_sine48_buf_intr.c
//

#include "L138_aic3106_init.h"
#define LOOPLENGTH 48
#define BUFLENGTH 256

int16_t sine_table[LOOPLENGTH] = 
  {0, 1305, 2588, 3827, 5000, 6088, 7071, 7934,
   8660, 9239, 9659, 9914, 10000, 9914, 9659, 9239,
   8660, 7934, 7071, 6088, 5000, 3827, 2588, 1305,
   0, -1305, -2588, -3827, -5000, -6088, -7071, -7934,
   -8660, -9239, -9659, -9914, -10000, -9914, -9659, -9239,
   -8660, -7934, -7071, -6088, -5000, -3827, -2588, -1305};

int16_t sine_ptr = 0; // pointer into lookup table

int32_t buffer[BUFLENGTH];
int16_t buf_ptr = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  int16_t sample;

  sample = sine_table[sine_ptr];       // read sample from table
  output_left_sample(sample);          // output sample
  sine_ptr = (sine_ptr+1)%LOOPLENGTH;  // increment table index
  buffer[buf_ptr] = (int32_t)(sample); // store sample in buffer
  buf_ptr = (buf_ptr+1)%BUFLENGTH;     // increment buffer index
  return;
}

int main(void)
{
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB);
  while(1);
}

