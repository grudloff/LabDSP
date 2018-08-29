// L138_notch2_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"
#include "notch2_coeffs.h"

float dly1[N]={0};          // filter1 delay line
float dly2[N]={0};          // filter2 delay line
volatile short out_type = 0;

interrupt void interrupt4() // interrupt service routine
{
  short i;
  float y1out, y2out;

//  dly1[0] = (float)(prbs());
  dly1[0] = (float)(input_left_sample());
  y1out = 0.0;              // compute filter 1 output
  for (i = 0; i< N; i++)	     	
    y1out += h900[i]*dly1[i];
  dly2[0]=(y1out);
  y2out = 0.0;              // compute filter 2 output
  for (i = 0; i< N; i++)
	y2out += h2700[i]*dly2[i];  
  for (i = N-1; i > 0; i--) // shift delay lines
  {
    dly1[i] = dly1[i-1];
    dly2[i] = dly2[i-1];
  }
  if (out_type==0)          // select output 
    output_left_sample((short)(y2out));
  if (out_type==1)        
    output_left_sample((short)(y1out));
  return;
}

int main()
{
  uint8_t DIP_value;
 
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1)
  {
	GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
    if(DIP_value == 0) out_type = 0; else out_type = 1;
  }
} 

