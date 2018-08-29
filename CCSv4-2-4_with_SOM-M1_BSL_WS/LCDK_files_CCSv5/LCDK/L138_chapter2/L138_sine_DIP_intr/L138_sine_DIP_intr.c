// L138_sine_DIP_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"

#define LOOPLENGTH 32

int16_t sine_ptr = 0;
int16_t sine_table[LOOPLENGTH]
                     = {0,195,383,556,707,831,924,981,1000,
					    981,924,831,707,556,383,195,0,-195,
					   -383,-556,-707,-831,-924,-981,-1000,
                        -981,-924,-831,-707,-556,-383,-195};
int16_t gain = 10;
int16_t frequency = 2;

interrupt void interrupt4(void) // interrupt service routine
{
  uint16_t left_sample;

  left_sample = (sine_table[sine_ptr]*gain); 
  sine_ptr += frequency;
  sine_ptr = sine_ptr % LOOPLENGTH;
  output_left_sample(left_sample);
  
  return;
}

int main(void)
{
  uint8_t DIP_value;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1)
  {
	DIP_value = read_LCDK_user_DIP();
    gain = (((DIP_value>>2)%4)+1)*8;
    frequency = ((DIP_value % 4) + 1)*2;
  }
}

