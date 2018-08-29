// L138_sysid_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define beta 10E-12          	  // learning rate
#define WLENGTH 256

float w[WLENGTH+1];		  // adaptive filter weights
float dly_adapt[WLENGTH+1];	  // adaptive filter delay line

interrupt void interrupt4(void) // interrupt service routine
{
  int16_t left_sample;
  int i;
  float fir_out, E, adaptfir_out;

  left_sample = input_left_sample(); // read from L ADC
  fir_out = (float)(left_sample);
  dly_adapt[0] = prbs();             // apply noise to input
  left_sample = (int16_t)(dly_adapt[0]); // of adaptive filter
  output_left_sample(left_sample);   // and to L DAC 
  adaptfir_out = 0.0;
  for (i = 0; i < WLENGTH; i++)
   adaptfir_out +=(w[i]*dly_adapt[i]); // compute filter output 
  E = fir_out - adaptfir_out;          // and error signal           
  for (i = WLENGTH-1; i >= 0; i--)         
  {
   w[i] = w[i]+(beta*E*dly_adapt[i]);  // update weights
   dly_adapt[i+1] = dly_adapt[i];      // update delay line   
  } 
  return;
}

int main(void)
{
  int i;

  for (i = 0; i <= WLENGTH; i++)
  {
    w[i] = 0.0;          // initialise adaptive filter weights
    dly_adapt[i] = 0.0;	 // and delay line
  }  
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1); 
}

