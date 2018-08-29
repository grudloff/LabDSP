// L138_loopbuf_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#define BUFSIZE 512

int32_t inbuffer[BUFSIZE]; // int32_t for TI Data Format
int16_t buf_ptr = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  int16_t sample_data;

  sample_data = input_left_sample();
  inbuffer[buf_ptr] = sample_data;
  buf_ptr = (buf_ptr+1)%BUFSIZE;
  output_left_sample(sample_data);
  
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
