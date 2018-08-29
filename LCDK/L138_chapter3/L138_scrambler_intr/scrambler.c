// L138_scrambler_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#include "sine160.h" 
#include "lp3k64.cof"           // filter coefficient file
float yn1, yn2;                 // filter outputs
float x1[N],x2[N];              // filter delay lines
int index = 0;

interrupt void interrupt4(void) // interrupt service routine
{  
  short i;

  x1[0] = (float)(input_left_sample()); // input from ADC
  yn1 = 0.0;                            // compute filter 1
  for (i=0 ; i<N ; i++) yn1 += h[i]*x1[i];
  for (i=(N-1) ; i>0 ; i--) x1[i] = x1[i-1];
  yn1 *= sine160[index++];              // mix with 3300 Hz
  if (index >= NSINE) index = 0;
  x2[0] = yn1; //get new input into delay line
  yn2 = 0.0;                            // compute filter 2
  for (i=0 ; i<N ; i++) yn2 += h[i]*x2[i];
  for (i=(N-1) ; i>0 ; i--) x2[i] = x2[i-1];  
  output_left_sample((short)(yn2));     //output to DAC
  return;
}

void main()
{
  L138_initialise_intr(FS_16000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
