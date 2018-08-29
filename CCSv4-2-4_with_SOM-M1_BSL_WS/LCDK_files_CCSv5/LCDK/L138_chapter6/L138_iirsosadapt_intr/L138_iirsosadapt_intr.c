// L138_iirsosadapt_intr.c 
//

#include "L138_LCDK_aic3106_init.h"
#include "elliptic.cof"                 // IIR filter coefficients

float w[NUM_SECTIONS][2] = {0.0, 0.0, 0.0, 0.0};
float beta = 1E-13;          	        // adaptive filter learning rate
#define WLENGTH 256                     // number of adptive filter coefficients
float weights[WLENGTH+1];		// adaptive filter coefficients
float x[WLENGTH+1];	                // adaptive filter delay line
AIC31_data_type codec_data;

interrupt void interrupt4(void)         // interrupt service routine
{
  int i;
  float adaptfir_out;                   // adaptive filter output
  float error;                          // error between adaptive filter output and desired output
  int section;                          // index for IIR filter section
  float input;                          // input to each IIR filter section
  float wn,yn;                          // intermediate and output values in each section
  input = (float)(prbs());              // input pseudorandom noise to IIR filter
  x[0]=input;          	                // and to adaptive filter
  
  for (section=0 ; section< NUM_SECTIONS ; section++) // compute IIR filter output
  {
    wn = input - a[section][1]*w[section][0] - a[section][2]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;                         // output of current IIR filter section will be input to next
  }

  adaptfir_out = 0.0;                   // compute adaptive FIR filter output
  for (i = 0; i < WLENGTH; i++)
    adaptfir_out +=(weights[i]*x[i]);

  error = yn - adaptfir_out;            // and error signal

  for (i = WLENGTH-1; i >= 0; i--)
  {
    weights[i] = weights[i]+(beta*error*x[i]);  // update adaptive FIR filter coefficients
    x[i+1] = x[i];                              // update adaptive FIR filter delay line
  }
  codec_data.channel[RIGHT]= ((uint16_t)(adaptfir_out));
  codec_data.channel[LEFT]= ((uint16_t)(error));
  output_sample(codec_data.uint);
  return;
}

int main()
{
  int i;

  for (i = 0; i < WLENGTH; i++)
  {
    weights[i] = 0.0;                   // initialise adaptive FIR filter coefficients
    x[i] = 0.0;			        // initialise adaptive FIR filter delay line
  }

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);
}
