// L138_firprnbuf_DIP_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"

#include "bp41.cof"

#define YNBUFLENGTH 1024
#define OUTPUT_YN 0
#define OUTPUT_XN 1

float x[N];                     // filter delay line
float yn_buffer[YNBUFLENGTH];   // output buffer
short ynbufindex = 0;
volatile short op = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  short i;
  float yn = 0.0;
  
  x[0] = (float)(prbs());       // input from prbs function
  for (i=0 ; i<N ; i++)         // compute filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)     // shift delay line
    x[i] = x[i-1]; 
if(op == OUTPUT_XN)
{  output_left_sample((uint16_t)(x[0]/2)); //output to codec
  yn_buffer[ynbufindex++] = x[0];
}
else
{  output_left_sample((uint16_t)(yn)); //output to codec
  yn_buffer[ynbufindex++] = yn;
}
  if(ynbufindex >= YNBUFLENGTH) ynbufindex = 0;
  return;
}

int main(void)
{
  uint8_t  DIP_value;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1)
  {
	GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
    if(DIP_value == 0)
      {
      	op = OUTPUT_YN;
      }
    else
      {
        op = OUTPUT_XN;
      }
  }
}

