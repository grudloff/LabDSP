// L138_sysid_flange_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define beta 10E-12             // learning rate
#define WLENGTH 256             // number of adaptive FIR filter weights
#define BUFLENGTH 128           // flanger delay line length
#define DELAY_IN_SAMPLES 20     // fixed flanger delay for identification

float w[WLENGTH+1];		// adaptive FIR filter coefficients
float dly_adapt[WLENGTH+1];	// adaptive FIR filter delay line
int16_t buffer[BUFLENGTH];      // flanger delay line stores integer sample values
int inptr = 0;                  // flanger input pointer
int outptr;                     // flanger output pointer


interrupt void interrupt4(void) // interrupt service routine
{
  int16_t left_sample;
  int i;
  float flanger_out, E, adaptfir_out;

  inptr = (inptr+1)%BUFLENGTH;         // increment input pointer
  buffer[inptr] = input_left_sample(); // read new input sample
  outptr = (inptr + BUFLENGTH - DELAY_IN_SAMPLES)%BUFLENGTH;
  flanger_out = (float)(buffer[inptr] + buffer[outptr]); // + or -

  dly_adapt[0] = prand();
  left_sample = (int16_t)(dly_adapt[0]);

  output_left_sample(left_sample);

  adaptfir_out = 0.0;
  for (i = 0; i < WLENGTH; i++)  //compute adaptive filter output
    adaptfir_out +=(w[i]*dly_adapt[i]);
  E = flanger_out - adaptfir_out;
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
    w[i] = 0.0;       	// initialise adaptive FIR filter weights
    dly_adapt[i] = 0.0;	// initialise delay line
  }  
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) ;
}
