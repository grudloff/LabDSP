// L138_am_poll.c
//

#include "L138_LCDK_aic3106_init.h"

short amp = 20;						//index for modulation

int main(void)
{
 int16_t baseband[20]={1000,951,809,587,309,0,-309,
  	                 -587,-809,-951,-1000,-951,-809,
  	                 -587,-309,0,309,587,809,951}; // 400 Hz
 int16_t carrier[20] ={1000,0,-1000,0,1000,0,-1000,
  	                  0,1000,0,-1000,0,1000,0,-1000,
  	                  0,1000,0,-1000,0}; // 2 kHz 
 int16_t output[20];
 int16_t k;

 L138_initialise_poll(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

 while(1)
 {
  for (k=0; k<20; k++)
  {
   output[k]= carrier[k] + ((amp*baseband[k]*carrier[k]/10)>>12);
   output_left_sample(20*output[k]);
  }
 }
}
