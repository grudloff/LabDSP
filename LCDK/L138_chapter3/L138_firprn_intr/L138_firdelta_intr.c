// L138_firdelta_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#include "comb14.cof"

#define BUFSIZE 256
#define AMPLITUDE 60000.0

float x[N]; // filter delay line
AIC31_data_type codec_data;
float dimpulse[BUFSIZE];
float response[BUFSIZE];
int index = 0;

interrupt void interrupt4(void)
{
  short i;
  float yn = 0.0;
  
  x[0] = dimpulse[index];   // input to first section read from impulse sequence
  for (i=0 ; i<N ; i++)            // compute filter output
    yn += h[i]*x[i];
  for (i=(N-1) ; i>0 ; i--)        // shift delay line
    x[i] = x[i-1]; 
  codec_data.channel[LEFT] = (uint16_t)(yn);
  codec_data.channel[RIGHT] = (uint16_t)(yn);
  output_sample(codec_data.uint);  // output to L and R DAC

  response[index++] = yn;
  if (index >= BUFSIZE) index = 0;

return;
}

int main(void)
{
	  int i;

	  for (i=0 ; i< BUFSIZE ; i++) dimpulse[i] = 0.0;
	  dimpulse[0] = AMPLITUDE;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}
