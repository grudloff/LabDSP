// L138_adaptnoise_2IN_iir_intr.c 
//

#include "L138_LCDK_aic3106_init.h"
#include "bilinear.cof"

#define beta 1E-12       // learning rate      
#define N 128            // number of adaptive filter weights

AIC31_data_type codec_data;

float weights[N];            // adaptive filter weights
float x[N];                  // adaptive filter delay line

float w[NUM_SECTIONS][2];

interrupt void interrupt4(void) // interrupt service routine
{                         
  short i;
  float input, refnoise, signal, signoise, wn, yn, error;
  int section;
  
  codec_data.uint = input_sample();
  refnoise =(codec_data.channel[LEFT]); // reference sensor
  signal = (codec_data.channel[RIGHT]); // primary sensor
  
  input = refnoise;
  for (section=0 ; section<NUM_SECTIONS ; section++)
  {
    wn = input - a[section][1]*w[section][0]
         - a[section][2]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0]
       + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;
  }
  signoise = yn + signal;
    
  yn=0.0;
  x[0] = refnoise;   
  for (i = 0; i < N; i++) // compute adaptive filter output
	yn += (weights[i] * x[i]);
  
  error = (signoise) - yn;   // compute error  
  for (i = N-1; i >= 0; i--) // update weights and delay line
  {            
    weights[i] = weights[i] + beta*error*x[i]; 
    x[i] = x[i-1]; 
  }  
  codec_data.channel[LEFT]= ((uint16_t)(error));
  codec_data.channel[RIGHT]= ((uint16_t)(error));
  output_sample(codec_data.uint);

  return;
}

int main()
{
  int i;                          

  for (i = 0; i < N; i++) // initialise weights and delay line
  {
    weights[i] = 0.0;
    x[i] = 0.0;
  } 
  for (i = 0 ; i<NUM_SECTIONS ; i++)
  {
    w[i][0]=0.0;
    w[i][1]=0.0;
  }
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1); 	
}
