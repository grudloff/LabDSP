// L138_delay_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#define BUF_SIZE 24000

uint16_t input,output,delayed;
uint16_t buffer[BUF_SIZE];
int i = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  input = input_left_sample();
  delayed = buffer[i];
  output = delayed + input;
  buffer[i] = input;
  i = (i+1)%BUF_SIZE;
  output_left_sample(output);
  return;
}

int main(void)
{
  int i;

  for (i=0 ; i<BUF_SIZE ; i++)
  {
    buffer[i] = 0;
  }
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_MIC_INPUT);
  while(1);
}
