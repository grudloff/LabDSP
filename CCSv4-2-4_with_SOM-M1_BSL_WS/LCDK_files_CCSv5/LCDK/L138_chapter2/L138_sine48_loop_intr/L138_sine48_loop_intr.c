// L138_sine48_loop_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define BUFLENGTH 128
#define LOOPLENGTH 48

AIC31_data_type codec_data;

int16_t inbuffer[BUFLENGTH];
int16_t sine_table[LOOPLENGTH] = {0, 1305, 2588, 3827,
	  5000, 6088, 7071, 7934, 8660, 9239, 9659, 9914, 10000,
	   9914, 9659, 9239, 8660, 7934, 7071, 6088, 5000, 3827,
	2588, 1305, 0, -1305, -2588, -3827, -5000, -6088, -7071,
	-7934, -8660, -9239, -9659, -9914, -10000, -9914, -9659,
	 -9239, -8660, -7934, -7071, -6088, -5000, -3827, -2588,
	 -1305};
int16_t inbuf_ptr = 0;
int sine_ptr = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  uint16_t left_sample;
  
  inbuffer[inbuf_ptr] = input_left_sample();
  inbuf_ptr = (inbuf_ptr+1) % BUFLENGTH;
  left_sample = sine_table[sine_ptr];
  sine_ptr = (sine_ptr+1)%LOOPLENGTH;
  codec_data.channel[RIGHT]=left_sample;
  codec_data.channel[LEFT]=left_sample;
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}